在Linux内核中，有很多同步机制。比较经典的有原子操作、spin_lock（忙等待的锁）、mutex（互斥锁）、semaphore（信号量）等。并且它们几乎都有对应的rw_XXX（读写锁），以便在能够区分读与写的情况下，让读操作相互不互斥（读写、写写依然互斥）。而seqlock和rcu应该可以不算在经典之列，它们是两种比较有意思的同步机制。

atomic（原子操作）：

所谓原子操作，就是该操作绝不会在执行完毕前被任何其他任务或事件打断，也就说，它的最小的执行单位，不可能有比它更小的执行单位，因此这里的原子实际是使用了物理学里的物质微粒的概念。
原子操作需要硬件的支持，因此是架构相关的，其API和原子类型的定义都定义在内核源码树的include/asm/atomic.h文件中，它们都使用汇编语言实现，因为C语言并不能实现这样的操作。

原子操作主要用于实现资源计数，很多引用计数(refcnt)就是通过原子操作实现的。

原子类型定义如下：
typedefstruct { volatile int counter; }atomic_t;
volatile修饰字段告诉gcc不要对该类型的数据做优化处理，对它的访问都是对内存的访问，而不是对寄存器的访问。

原子操作API包括：
atomic_read(atomic_t* v);

该函数对原子类型的变量进行原子读操作，它返回原子类型的变量v的值。
atomic_set(atomic_t* v, int i);

该函数设置原子类型的变量v的值为i。
voidatomic_add(int i, atomic_t *v);

该函数给原子类型的变量v增加值i。
atomic_sub(inti, atomic_t *v);

该函数从原子类型的变量v中减去i。
intatomic_sub_and_test(int i, atomic_t *v);

该函数从原子类型的变量v中减去i，并判断结果是否为0，如果为0，返回真，否则返回假。
voidatomic_inc(atomic_t *v);

该函数对原子类型变量v原子地增加1。
voidatomic_dec(atomic_t *v);

该函数对原子类型的变量v原子地减1。
intatomic_dec_and_test(atomic_t *v);

该函数对原子类型的变量v原子地减1，并判断结果是否为0，如果为0，返回真，否则返回假。
intatomic_inc_and_test(atomic_t *v);

该函数对原子类型的变量v原子地增加1，并判断结果是否为0，如果为0，返回真，否则返回假。
intatomic_add_negative(int i, atomic_t*v);

该函数对原子类型的变量v原子地增加I，并判断结果是否为负数，如果是，返回真，否则返回假。
intatomic_add_return(int i, atomic_t *v);

该函数对原子类型的变量v原子地增加i，并且返回指向v的指针。
intatomic_sub_return(int i, atomic_t *v);

该函数从原子类型的变量v中减去i，并且返回指向v的指针。

intatomic_inc_return(atomic_t * v);

该函数对原子类型的变量v原子地增加1并且返回指向v的指针。

intatomic_dec_return(atomic_t * v);

该函数对原子类型的变量v原子地减1并且返回指向v的指针。

原子操作通常用于实现资源的引用计数，在TCP/IP协议栈的IP碎片处理中，就使用了引用计数，碎片队列结构structipq描述了一个IP碎片，字段refcnt就是引用计数器，它的类型为atomic_t，当创建IP碎片时（在函数ip_frag_create中），使用atomic_set函数把它设置为1，当引用该IP碎片时，就使用函数atomic_inc把引用计数加1，当不需要引用该IP碎片时，就使用函数ipq_put来释放该IP碎片，ipq_put使用函数atomic_dec_and_test把引用计数减1并判断引用计数是否为0，如果是就释放Ip碎片。函数ipq_kill把IP碎片从ipq队列中删除，并把该删除的IP碎片的引用计数减1（通过使用函数atomic_dec实现）。



Spanlock(自旋锁)

自旋锁与互斥锁有点类似，只是自旋锁不会引起调用者睡眠，如果自旋锁已经被别的执行单元保持，调用者就一直循环在那里看是否该自旋锁的保持者已经释放了锁，"自旋"一词就是因此而得名。由于自旋锁使用者一般保持锁时间非常短，因此选择自旋而不是睡眠是非常必要的，自旋锁的效率远高于互斥锁。

信号量和读写信号量适合于保持时间较长的情况，它们会导致调用者睡眠，因此只能在进程上下文使用（_trylock的变种能够在中断上下文使用），而自旋锁适合于保持时间非常短的情况，它可以在任何上下文使用。如果被保护的共享资源只在进程上下文访问，使用信号量保护该共享资源非常合适，如果对共巷资源的访问时间非常短，自旋锁也可以。但是如果被保护的共享资源需要在中断上下文访问（包括底半部即中断处理句柄和顶半部即软中断），就必须使用自旋锁。

自旋锁保持期间是抢占失效的，而信号量和读写信号量保持期间是可以被抢占的。自旋锁只有在内核可抢占或SMP的情况下才真正需要，在单CPU且不可抢占的内核下，自旋锁的所有操作都是空操作。

跟互斥锁一样，一个执行单元要想访问被自旋锁保护的共享资源，必须先得到锁，在访问完共享资源后，必须释放锁。如果在获取自旋锁时，没有任何执行单元保持该锁，那么将立即得到锁；如果在获取自旋锁时锁已经有保持者，那么获取锁操作将自旋在那里，直到该自旋锁的保持者释放了锁。

无论是互斥锁，还是自旋锁，在任何时刻，最多只能有一个保持者，也就说，在任何时刻最多只能有一个执行单元获得锁。

自旋锁的API有：

spin_lock_init(x)

该宏用于初始化自旋锁x。自旋锁在真正使用前必须先初始化。该宏用于动态初始化。

DEFINE_SPINLOCK(x)

该宏声明一个自旋锁x并初始化它。该宏在2.6.11中第一次被定义，在先前的内核中并没有该宏。

SPIN_LOCK_UNLOCKED

该宏用于静态初始化一个自旋锁。

DEFINE_SPINLOCK(x)等同于spinlock_tx = SPIN_LOCK_UNLOCKED

spin_is_locked(x)

该宏用于判断自旋锁x是否已经被某执行单元保持（即被锁），如果是，返回真，否则返回假。

spin_unlock_wait(x)

该宏用于等待自旋锁x变得没有被任何执行单元保持，如果没有任何执行单元保持该自旋锁，该宏立即返回，否则将循环在那里，直到该自旋锁被保持者释放。

spin_trylock(lock)

该宏尽力获得自旋锁lock，如果能立即获得锁，它获得锁并返回真，否则不能立即获得锁，立即返回假。它不会自旋等待lock被释放。

spin_lock(lock)

该宏用于获得自旋锁lock，如果能够立即获得锁，它就马上返回，否则，它将自旋在那里，直到该自旋锁的保持者释放，这时，它获得锁并返回。总之，只有它获得锁才返回。

spin_lock_irqsave(lock, flags)

该宏获得自旋锁的同时把标志寄存器的值保存到变量flags中并失效本地中断。

spin_lock_irq(lock)

该宏类似于spin_lock_irqsave，只是该宏不保存标志寄存器的值。

spin_lock_bh(lock)

该宏在得到自旋锁的同时失效本地软中断。

spin_unlock(lock)

该宏释放自旋锁lock，它与spin_trylock或spin_lock配对使用。如果spin_trylock返回假，表明没有获得自旋锁，因此不必使用spin_unlock释放。

spin_unlock_irqrestore(lock, flags)

该宏释放自旋锁lock的同时，也恢复标志寄存器的值为变量flags保存的值。它与spin_lock_irqsave配对使用。

spin_unlock_irq(lock)

该宏释放自旋锁lock的同时，也使能本地中断。它与spin_lock_irq配对应用。

spin_unlock_bh(lock)

该宏释放自旋锁lock的同时，也使能本地的软中断。它与spin_lock_bh配对使用。

spin_trylock_irqsave(lock, flags)

该宏如果获得自旋锁lock，它也将保存标志寄存器的值到变量flags中，并且失效本地中断，如果没有获得锁，它什么也不做。因此如果能够立即获得锁，它等同于spin_lock_irqsave，如果不能获得锁，它等同于spin_trylock。如果该宏获得自旋锁lock，那需要使用spin_unlock_irqrestore来释放。

spin_trylock_irq(lock)

该宏类似于spin_trylock_irqsave，只是该宏不保存标志寄存器。如果该宏获得自旋锁lock，需要使用spin_unlock_irq来释放。

spin_trylock_bh(lock)

该宏如果获得了自旋锁，它也将失效本地软中断。如果得不到锁，它什么也不做。因此，如果得到了锁，它等同于spin_lock_bh，如果得不到锁，它等同于spin_trylock。如果该宏得到了自旋锁，需要使用spin_unlock_bh来释放。

spin_can_lock(lock)

该宏用于判断自旋锁lock是否能够被锁，它实际是spin_is_locked取反。如果lock没有被锁，它返回真，否则，返回假。该宏在2.6.11中第一次被定义，在先前的内核中并没有该宏。

获得自旋锁和释放自旋锁有好几个版本，因此让读者知道在什么样的情况下使用什么版本的获得和释放锁的宏是非常必要的。

如果被保护的共享资源只在进程上下文访问和软中断上下文访问，那么当在进程上下文访问共享资源时，可能被软中断打断，从而可能进入软中断上下文来对被保护的共享资源访问，因此对于这种情况，对共享资源的访问必须使用spin_lock_bh和spin_unlock_bh来保护。当然使用spin_lock_irq和spin_unlock_irq以及spin_lock_irqsave和spin_unlock_irqrestore也可以，它们失效了本地硬中断，失效硬中断隐式地也失效了软中断。但是使用spin_lock_bh和spin_unlock_bh是最恰当的，它比其他两个快。

如果被保护的共享资源只在进程上下文和tasklet或timer上下文访问，那么应该使用与上面情况相同的获得和释放锁的宏，因为tasklet和timer是用软中断实现的。

如果被保护的共享资源只在一个tasklet或timer上下文访问，那么不需要任何自旋锁保护，因为同一个tasklet或timer只能在一个CPU上运行，即使是在SMP环境下也是如此。实际上tasklet在调用tasklet_schedule标记其需要被调度时已经把该tasklet绑定到当前CPU，因此同一个tasklet决不可能同时在其他CPU上运行。timer也是在其被使用add_timer添加到timer队列中时已经被帮定到当前CPU，所以同一个timer绝不可能运行在其他CPU上。当然同一个tasklet有两个实例同时运行在同一个CPU就更不可能了。

如果被保护的共享资源只在两个或多个tasklet或timer上下文访问，那么对共享资源的访问仅需要用spin_lock和spin_unlock来保护，不必使用_bh版本，因为当tasklet或timer运行时，不可能有其他tasklet或timer在当前CPU上运行。如果被保护的共享资源只在一个软中断（tasklet和timer除外）上下文访问，那么这个共享资源需要用spin_lock和spin_unlock来保护，因为同样的软中断可以同时在不同的CPU上运行。

如果被保护的共享资源在两个或多个软中断上下文访问，那么这个共享资源当然更需要用spin_lock和spin_unlock来保护，不同的软中断能够同时在不同的CPU上运行。

如果被保护的共享资源在软中断（包括tasklet和timer）或进程上下文和硬中断上下文访问，那么在软中断或进程上下文访问期间，可能被硬中断打断，从而进入硬中断上下文对共享资源进行访问，因此，在进程或软中断上下文需要使用spin_lock_irq和spin_unlock_irq来保护对共享资源的访问。而在中断处理句柄中使用什么版本，需依情况而定，如果只有一个中断处理句柄访问该共享资源，那么在中断处理句柄中仅需要spin_lock和spin_unlock来保护对共享资源的访问就可以了。因为在执行中断处理句柄期间，不可能被同一CPU上的软中断或进程打断。但是如果有不同的中断处理句柄访问该共享资源，那么需要在中断处理句柄中使用spin_lock_irq和spin_unlock_irq来保护对共享资源的访问。

在使用spin_lock_irq和spin_unlock_irq的情况下，完全可以用spin_lock_irqsave和spin_unlock_irqrestore取代，那具体应该使用哪一个也需要依情况而定，如果可以确信在对共享资源访问前中断是使能的，那么使用spin_lock_irq更好一些，因为它比spin_lock_irqsave要快一些，但是如果你不能确定是否中断使能，那么使用spin_lock_irqsave和spin_unlock_irqrestore更好，因为它将恢复访问共享资源前的中断标志而不是直接使能中断。当然，有些情况下需要在访问共享资源时必须中断失效，而访问完后必须中断使能，这样的情形使用spin_lock_irq和spin_unlock_irq最好。

需要特别提醒读者，spin_lock用于阻止在不同CPU上的执行单元对共享资源的同时访



mutex(互斥锁)

/linux/include/linux/mutex.h

47struct mutex {

48

49 atomic_t count;

50 spinlock_t wait_lock;

51 struct list_head wait_list;

52#ifdef CONFIG_DEBUG_MUTEXES

53 struct thread_info *owner;

54 const char *name;

55 void *magic;

56#endif

57#ifdef CONFIG_DEBUG_LOCK_ALLOC

58 struct lockdep_map dep_map;

59#endif

60};



一、作用及访问规则：

互斥锁主要用于实现内核中的互斥访问功能。内核互斥锁是在原子API之上实现的，但这对于内核用户是不可见的。对它的访问必须遵循一些规则：同一时间只能有一个任务持有互斥锁，而且只有这个任务可以对互斥锁进行解锁。互斥锁不能进行递归锁定或解锁。一个互斥锁对象必须通过其API初始化，而不能使用memset或复制初始化。一个任务在持有互斥锁的时候是不能结束的。互斥锁所使用的内存区域是不能被释放的。使用中的互斥锁是不能被重新初始化的。并且互斥锁不能用于中断上下文。但是互斥锁比当前的内核信号量选项更快，并且更加紧凑，因此如果它们满足您的需求，那么它们将是您明智的选择。

二、各字段详解：

1、atomic_t count; --指示互斥锁的状态：1没有上锁，可以获得；0被锁定，不能获得；负数被锁定，且可能在该锁上有等待进程初始化为没有上锁。

2、spinlock_t wait_lock;--等待获取互斥锁中使用的自旋锁。在获取互斥锁的过程中，操作会在自旋锁的保护中进行。初始化为为锁定。

3、struct list_head wait_list;--等待互斥锁的进程队列。

四、操作：

1、定义并初始化：

struct mutex mutex;

mutex_init(&mutex);

79# define mutex_init(mutex) \

80do { \

81 static struct lock_class_key __key; \

82 \

83 __mutex_init((mutex), #mutex, &__key); \

84} while (0)

42void

43__mutex_init(struct mutex *lock, const char *name, structlock_class_key *key)

44{

45 atomic_set(&lock->count, 1);

46 spin_lock_init(&lock->wait_lock);

47 INIT_LIST_HEAD(&lock->wait_list);

48

49 debug_mutex_init(lock, name, key);

50}

直接定于互斥锁mutex并初始化为未锁定，己count为1，wait_lock为未上锁，等待队列wait_list为空。

2、获取互斥锁：

(1)具体参见linux/kernel/mutex.c

void inline __sched mutex_lock(struct mutex *lock)

{

might_sleep();

__mutex_fastpath_lock(&lock->count,__mutex_lock_slowpath);

}

获取互斥锁。实际上是先给count做自减操作，然后使用本身的自旋锁进入临界区操作。首先取得count的值，再将count置为－1，判断如果原来count的值为1，也即互斥锁可以获得，则直接获取，跳出。否则进入循环反复测试互斥锁的状态。在循环中，也是先取得互斥锁原来的状态，再将其置为－1，判断如果可以获取(等于1)，则退出循环，否则设置当前进程的状态为不可中断状态，解锁自身的自旋锁，进入睡眠状态，待被在调度唤醒时，再获得自身的自旋锁，进入新一次的查询其自身状态(该互斥锁的状态)的循环。

(2)具体参见linux/kernel/mutex.c

int __sched mutex_lock_interruptible(struct mutex *lock)

{

might_sleep();

return __mutex_fastpath_lock_retval(&lock->count,__mutex_lock_interruptible_slowpath);

}

和mutex_lock()一样，也是获取互斥锁。在获得了互斥锁或进入睡眠直到获得互斥锁之后会返回0。如果在等待获取锁的时候进入睡眠状态收到一个信号(被信号打断睡眠)，则返回_EINIR。

(3)具体参见linux/kernel/mutex.c

int __sched mutex_trylock(struct mutex *lock)

{

return __mutex_fastpath_trylock(&lock->count,

__mutex_trylock_slowpath);

}

试图获取互斥锁，如果成功获取则返回1，否则返回0，不等待。

3、释放互斥锁：

具体参见linux/kernel/mutex.c

void __sched mutex_unlock(struct mutex *lock)

{

__mutex_fastpath_unlock(&lock->count,__mutex_unlock_slowpath);

}

释放被当前进程获取的互斥锁。该函数不能用在中断上下文中，而且不允许去释放一个没有上锁的互斥锁。

4.void mutex_destroy(struct mutex *lock) --清除互斥锁，使互斥锁不可用

用mutex_destroy()函数解除由lock指向的互斥锁的任何状态。在调用执行这个函数的时候，lock指向的互斥锁不能在被锁状态。储存互斥锁的内存不被释放。

返回值--mutex_destroy()在成功执行后返回零。其他值意味着错误。在以下情况发生时，函数失败并返回相关值。

EINVAL 非法参数

EFAULT mp指向一个非法地址。

5.static inline int mutex_is_locked(struct mutex *lock)--测试互斥锁的状态

这个调用实际上编译成一个内联函数。如果互斥锁被持有（锁定），那么就会返回1；否则，返回 0。

五、使用形式：

struct mutex mutex;

mutex_init(&mutex);

...

mutex_lock(&mutex);

...

mutex_unlock(&mutex);

semaphore (信号量)

Linux内核的信号量在概念和原理上与用户态的SystemV的IPC机制信号量是一样的，但是它绝不可能在内核之外使用，因此它与SystemV的IPC机制信号量毫不相干。

信号量在创建时需要设置一个初始值，表示同时可以有几个任务可以访问该信号量保护的共享资源，初始值为1就变成互斥锁（Mutex），即同时只能有一个任务可以访问信号量保护的共享资源。一个任务要想访问共享资源，首先必须得到信号量，获取信号量的操作将把信号量的值减1，若当前信号量的值为负数，表明无法获得信号量，该任务必须挂起在该信号量的等待队列等待该信号量可用；若当前信号量的值为非负数，表示可以获得信号量，因而可以立刻访问被该信号量保护的共享资源。当任务访问完被信号量保护的共享资源后，必须释放信号量，释放信号量通过把信号量的值加1实现，如果信号量的值为非正数，表明有任务等待当前信号量，因此它也唤醒所有等待该信号量的任务。

信号量的API有：

DECLARE_MUTEX(name)

该宏声明一个信号量name并初始化它的值为0，即声明一个互斥锁。

DECLARE_MUTEX_LOCKED(name)

该宏声明一个互斥锁name，但把它的初始值设置为0，即锁在创建时就处在已锁状态。因此对于这种锁，一般是先释放后获得。

void sema_init (struct semaphore *sem, int val);

该函用于数初始化设置信号量的初值，它设置信号量sem的值为val。

void init_MUTEX (struct semaphore *sem);

该函数用于初始化一个互斥锁，即它把信号量sem的值设置为1。

void init_MUTEX_LOCKED (struct semaphore *sem);

该函数也用于初始化一个互斥锁，但它把信号量sem的值设置为0，即一开始就处在已锁状态。

void down(struct semaphore * sem);

该函数用于获得信号量sem，它会导致睡眠，因此不能在中断上下文（包括IRQ上下文和softirq上下文）使用该函数。该函数将把sem的值减1，如果信号量sem的值非负，就直接返回，否则调用者将被挂起，直到别的任务释放该信号量才能继续运行。

int down_interruptible(struct semaphore * sem);

该函数功能与down类似，不同之处为，down不会被信号（signal）打断，但down_interruptible能被信号打断，因此该函数有返回值来区分是正常返回还是被信号中断，如果返回0，表示获得信号量正常返回，如果被信号打断，返回-EINTR。

int down_trylock(struct semaphore * sem);

该函数试着获得信号量sem，如果能够立刻获得，它就获得该信号量并返回0，否则，表示不能获得信号量sem，返回值为非0值。因此，它不会导致调用者睡眠，可以在中断上下文使用。

void up(struct semaphore * sem);

该函数释放信号量sem，即把sem的值加1，如果sem的值为非正数，表明有任务等待该信号量，因此唤醒这些等待者。

信号量在绝大部分情况下作为互斥锁使用，下面以console驱动系统为例说明信号量的使用。

在内核源码树的kernel/printk.c中，使用宏DECLARE_MUTEX声明了一个互斥锁console_sem，它用于保护console驱动列表console_drivers以及同步对整个console驱动系统的访问，其中定义了函数acquire_console_sem来获得互斥锁console_sem，定义了release_console_sem来释放互斥锁console_sem，定义了函数try_acquire_console_sem来尽力得到互斥锁console_sem。这三个函数实际上是分别对函数down，up和down_trylock的简单包装。需要访问console_drivers驱动列表时就需要使用acquire_console_sem来保护console_drivers列表，当访问完该列表后，就调用release_console_sem释放信号量console_sem。函数console_unblank，console_device，console_stop，console_start，register_console和unregister_console都需要访问console_drivers，因此它们都使用函数对acquire_console_sem和release_console_sem来对console_drivers进行保护。



rw_semaphore （读写信号量）

读写信号量对访问者进行了细分，或者为读者，或者为写者，读者在保持读写信号量期间只能对该读写信号量保护的共享资源进行读访问，如果一个任务除了需要读，可能还需要写，那么它必须被归类为写者，它在对共享资源访问之前必须先获得写者身份，写者在发现自己不需要写访问的情况下可以降级为读者。读写信号量同时拥有的读者数不受限制，也就说可以有任意多个读者同时拥有一个读写信号量。如果一个读写信号量当前没有被写者拥有并且也没有写者等待读者释放信号量，那么任何读者都可以成功获得该读写信号量；否则，读者必须被挂起直到写者释放该信号量。如果一个读写信号量当前没有被读者或写者拥有并且也没有写者等待该信号量，那么一个写者可以成功获得该读写信号量，否则写者将被挂起，直到没有任何访问者。因此，写者是排他性的，独占性的。

读写信号量有两种实现，一种是通用的，不依赖于硬件架构，因此，增加新的架构不需要重新实现它，但缺点是性能低，获得和释放读写信号量的开销大；另一种是架构相关的，因此性能高，获取和释放读写信号量的开销小，但增加新的架构需要重新实现。在内核配置时，可以通过选项去控制使用哪一种实现。

读写信号量的相关API有：

DECLARE_RWSEM(name)

该宏声明一个读写信号量name并对其进行初始化。

void init_rwsem(struct rw_semaphore *sem);

该函数对读写信号量sem进行初始化。

void down_read(struct rw_semaphore *sem);

读者调用该函数来得到读写信号量sem。该函数会导致调用者睡眠，因此只能在进程上下文使用。

int down_read_trylock(struct rw_semaphore *sem);

该函数类似于down_read，只是它不会导致调用者睡眠。它尽力得到读写信号量sem，如果能够立即得到，它就得到该读写信号量，并且返回1，否则表示不能立刻得到该信号量，返回0。因此，它也可以在中断上下文使用。

void down_write(struct rw_semaphore *sem);

写者使用该函数来得到读写信号量sem，它也会导致调用者睡眠，因此只能在进程上下文使用。

int down_write_trylock(struct rw_semaphore *sem);

该函数类似于down_write，只是它不会导致调用者睡眠。该函数尽力得到读写信号量，如果能够立刻获得，就获得该读写信号量并且返回1，否则表示无法立刻获得，返回0。它可以在中断上下文使用。

void up_read(struct rw_semaphore *sem);

读者使用该函数释放读写信号量sem。它与down_read或down_read_trylock配对使用。如果down_read_trylock返回0，不需要调用up_read来释放读写信号量，因为根本就没有获得信号量。

void up_write(struct rw_semaphore *sem);

写者调用该函数释放信号量sem。它与down_write或down_write_trylock配对使用。如果down_write_trylock返回0，不需要调用up_write，因为返回0表示没有获得该读写信号量。

void downgrade_write(struct rw_semaphore *sem);

该函数用于把写者降级为读者，这有时是必要的。因为写者是排他性的，因此在写者保持读写信号量期间，任何读者或写者都将无法访问该读写信号量保护的共享资源，对于那些当前条件下不需要写访问的写者，降级为读者将，使得等待访问的读者能够立刻访问，从而增加了并发性，提高了效率。

读写信号量适于在读多写少的情况下使用，在linux内核中对进程的内存映像描述结构的访问就使用了读写信号量进行保护。在Linux中，每一个进程都用一个类型为task_t或structtask_struct的结构来描述，该结构的类型为structmm_struct的字段mm描述了进程的内存映像，特别是mm_struct结构的mmap字段维护了整个进程的内存块列表，该列表将在进程生存期间被大量地遍利或修改，因此mm_struct结构就有一个字段mmap_sem来对mmap的访问进行保护，mmap_sem就是一个读写信号量，在proc文件系统里有很多进程内存使用情况的接口，通过它们能够查看某一进程的内存使用情况，命令free、ps和top都是通过proc来得到内存使用信息的，proc接口就使用down_read和up_read来读取进程的mmap信息。当进程动态地分配或释放内存时，需要修改mmap来反映分配或释放后的内存映像，因此动态内存分配或释放操作需要以写者身份获得读写信号量mmap_sem来对mmap进行更新。系统调用brk和munmap就使用了down_write和up_write来保护对mmap的访问。



seqlock（顺序锁）

用于能够区分读与写的场合，并且是读操作很多、写操作很少，写操作的优先权大于读操作。
seqlock的实现思路是，用一个递增的整型数表示sequence。写操作进入临界区时，sequence++；退出临界区时，sequence再++。写操作还需要获得一个锁（比如mutex），这个锁仅用于写写互斥，以保证同一时间最多只有一个正在进行的写操作。
当sequence为奇数时，表示有写操作正在进行，这时读操作要进入临界区需要等待，直到sequence变为偶数。读操作进入临界区时，需要记录下当前sequence的值，等它退出临界区的时候用记录的sequence与当前sequence做比较，不相等则表示在读操作进入临界区期间发生了写操作，这时候读操作读到的东西是无效的，需要返回重试。

seqlock写写是必须要互斥的。但是seqlock的应用场景本身就是读多写少的情况，写冲突的概率是很低的。所以这里的写写互斥基本上不会有什么性能损失。
而读写操作是不需要互斥的。seqlock的应用场景是写操作优先于读操作，对于写操作来说，几乎是没有阻塞的（除非发生写写冲突这一小概率事件），只需要做sequence++这一附加动作。而读操作也不需要阻塞，只是当发现读写冲突时需要retry。

seqlock的一个典型应用是时钟的更新，系统中每1毫秒会有一个时钟中断，相应的中断处理程序会更新时钟（见《linux时钟浅析》）（写操作）。而用户程序可以调用gettimeofday之类的系统调用来获取当前时间（读操作）。在这种情况下，使用seqlock可以避免过多的gettimeofday系统调用把中断处理程序给阻塞了（如果使用读写锁，而不用seqlock的话就会这样）。中断处理程序总是优先的，而如果gettimeofday系统调用与之冲突了，那用户程序多等等也无妨。

seqlock的实现非常简单：
写操作进入临界区时：
voidwrite_seqlock(seqlock_t *sl)
{
spin_lock(&sl->lock); //上写写互斥锁
++sl->sequence;      // sequence++
}
写操作退出临界区时：
voidwrite_sequnlock(seqlock_t *sl)
{
sl->sequence++;        // sequence再++
spin_unlock(&sl->lock);// 释放写写互斥锁
}

读操作进入临界区时：
unsignedread_seqbegin(const seqlock_t *sl)
{
unsigned ret;
repeat:
ret= sl->sequence;      // 读sequence值
if(unlikely(ret & 1)) { // 如果sequence为奇数自旋等待
gotorepeat;
}
return ret;
}
读操作尝试退出临界区时：
intread_seqretry(const seqlock_t *sl, unsigned start)
{
return(sl->sequence != start); //看看sequence与进入临界区时是否发生过改变
}
而读操作一般会这样进行：
do{
seq = read_seqbegin(&seq_lock);     // 进入临界区
do_something();
}while (read_seqretry(&seq_lock, seq)); //尝试退出临界区，存在冲突则重试

RCU（read-copy-update）

RCU也是用于能够区分读与写的场合，并且也是读多写少，但是读操作的优先权大于写操作（与seqlock相反）。
RCU的实现思路是，读操作不需要互斥、不需要阻塞、也不需要原子指令，直接读就行了。而写操作在进行之前需要把被写的对象copy一份，写完之后再更新回去。其实RCU所能保护的并不是任意的临界区，它只能保护由指针指向的对象（而不保护指针本身）。读操作通过这个指针来访问对象（这个对象就是临界区）；写操作把对象复制一份，然后更新，最后修改指针使其指向新的对象。由于指针总是一个字长的，对它的读写对于CPU来说总是原子的，所以不用担心更新指针只更新到一半就被读取的情况（指针的值为0x11111111，要更新为0x22222222，不会出现类似0x11112222这样的中间状态）。所以，当读写操作同时发生时，读操作要么读到指针的旧值，引用了更新前的对象、要么读到了指针的新值，引用了更新后的对象。即使同时有多个写操作发生也没关系（是否需要写写互斥跟写操作本身的场景相关）。

RCU封装了rcu_dereference和rcu_assign_pointer两个函数，分别用于对指针进行读和写。
rcu_assign_pointer(p,v) => (p) = (v)
rcu_dereference(p) => (p)
里面其实就是简单的指针读和写，然后可能设置内存屏障（以避免编译器或CPU指令乱序对程序造成影响）。当然，如果出现了一种奇怪的不能直接保证原子性读写指针的体系结构，还需要这两个函数来保证原子性。

可以看到，使用了RCU之后，读写操作竟然神奇地都不需要阻塞了，临界区已经不是临界区了。只不过写操作稍微麻烦些，需要read、copy再update。不过RCU的核心问题并不是如何同步，而是如何释放旧的对象。指向对象的指针被更新了，但是之前发生的读操作可能还在引用旧的对象呢，旧的对象什么时候释放掉呢？让读操作来释放旧的对象似乎并不是很和理，它不知道对象是否已经被更新了，也不知道有多少读操作都引用了这个旧对象。给对象加一个引用计数呢？这或许可以奏效，但是这也太不通用了，RCU是一种机制，如果要求每个使用RCU的对象都在对象的某某位置维护一个引用计数，相当于RCU机制要跟具体的对象耦合上了。并且对引用计数的修改还需要另一套同步机制来提供保障。
为解决旧对象释放的问题，RCU提供了四个函数（另外还有一些它们的变形）：
rcu_read_lock(void)、rcu_read_unlock(void)
synchronize_rcu(void)、call_rcu(struct rcu_head *head, void (*func)(struct rcu_head*head))。
当读操作要调用rcu_dereference访问对象之前，需要先调用rcu_read_lock；当不再需要访问对象时，调用rcu_read_unlock。
当写操作调用rcu_assign_pointer完成对对象的更新之后，需要调用synchronize_rcu或call_rcu。其中synchronize_rcu会阻塞等待在此之前所有调用了rcu_read_lock的读操作都已经调用rcu_read_unlock，synchronize_rcu返回后写操作一方就可以将被它替换掉的旧对象释放了；而call_rcu则是通过注册回调函数的方式，由回调函数来释放旧对象，写操作一方将不需要阻塞等待。同样，等到在此之前所有调用了rcu_read_lock的读操作都调用rcu_read_unlock之后，回调函数将被调用。

如果你足够细心，可能已经注意到了这样一个问题。synchronize_rcu和call_rcu会等待的是“在此之前所有调用了rcu_read_lock的读操作都已经调用了rcu_read_unlock”，然而在rcu_assign_pointer与synchronize_rcu或call_rcu之间，可能也有读操作发生（调用了rcu_read_lock），它们引用到的是写操作rcu_assign_pointer后的新对象。按理说写操作一方想要释放旧对象时，是不需要等待这样的读操作的。但是由于这些读操作发生在synchronize_rcu或call_rcu之前，按照RCU的机制，还真得等它们都rcu_read_unlock。这岂不是多等了一些时日？
实际情况的确是这样，甚至可能更糟。因为目前linux内核里面的RCU是一个全局的实现，注意，rcu_read_lock、synchronize_rcu、等等操作都是不带参数的。它不像seqlock或其他同步机制那样，一把锁保护一个临界区。这个全局的RCU将保护使用RCU机制的所有临界区。所以，对于写操作一方来说，在它调用synchronize_rcu或call_rcu之前发生的所有读操作它都得等待（不管读的对象与该写操作有无关系），直到这些读操作都rcu_read_unlock之后，旧的对象才能被释放。所以，写操作更新对象之后，旧对象并不是精确地在它能够被释放之时立刻被释放的，可能会存在一定的延迟。
不过话说回来，这样的实现减少了很多不必要的麻烦，因为旧的对象晚一些释放是不会有太大关系的。想一想，精确旧对象的释放时机有多大意义呢？无非是尽可能早的回收一些内存（一般来说，内核里面使用的这些对象并不会太大吧，晚一点回收也不会晚得太过分吧）。但是为此你得花费很大的代价去跟踪每一个对象的引用情况，这是不是有些得不偿失呢？

最后，RCU要求，读操作在rcu_read_lock与rcu_read_unlock之间是不能睡眠的（WHY？），call_rcu提供的回调函数也不能睡眠（因为回调函数一般会在软中断里面去调用，中断上下文是不能睡眠的，见《linux中断处理浅析》）。

那么，RCU具体是怎么实现的呢？尽管没有要求在精确的时间回收旧对象，RCU的实现还是很复杂的。以下简单讨论一下rcu_read_lock、rcu_read_unlock、call_rcu三个函数的实现。而synchronize_rcu实际上是利用call_rcu来实现的（调用call_rcu提交一个回调函数，然后自己进入睡眠，而回调函数要做的事情就是把自己唤醒）。
在linux2.6.30版本中，RCU有三种实现，分别命名为rcuclassic、rcupreempt、rcutree。这三种实现也是逐步发展出来的，最开始是rcuclassic，然后rcupreempt，最后rcutree。在编译内核时可以通过编译选项选择需要使用的RCU实现。

rcuclassic
rcuclassic的实现思路是，读操作在rcu_read_lock时禁止内核抢占、在rcu_read_unlock时重新启用内核抢占。由于RCU只会在内核态里面使用，而且RCU也要求rcu_read_lock与rcu_read_unlock之间不能睡眠。所以在rcu_read_lock之后，这个读操作的相关代码肯定会在当前CPU上持续被执行，直到rcu_read_unlock之后才可能被调度。而同一时间，在一个CPU上，也最多只能有一个正在进行的读操作。可以说，rcuclassic是基于CPU来跟踪读操作的。
于是，如果发现一个CPU已经发生了调度，就说明这个CPU上的读操作肯定已经rcu_read_unlock了（注意这里又是一次延迟，rcu_read_unlock之后可能还要过一段时间才会发生调度。RCU的实现中，这样的延迟随处可见，因为它根本就不要求在精确的时间点回收旧对象）。于是，从一次call_rcu被调用之后开始，如果等到所有CPU都已经发生了调度，这次call_rcu需要等待的读操作就必定都已经rcu_read_unlock了，这时候就可以处理这个call_rcu提交的回调函数了。
但是实现上，rcuclassic并不是为每一次call_rcu都提供一个这样的等待周期（等待所有CPU都已发生调度），那样的话粒度太细，实现起来会比较复杂。rcuclassic将现有的全部call_rcu提交的回调函数分为两个批次（batch），以批次为单位来进行等待。如果所有CPU都已发生调度，则第一批次的所有回调函数将被调用，然后将第一批次清空、第二批变为第一批，并继续下一次的等待。而所有新来的call_rcu总是将回调函数提交到第二批。
rcuclassic逻辑上通过三个链表来管理call_rcu提交的回调函数，分别是第二批次链表、第一批次链表、待处理链表（2.6.30版本的实现实际用了四个链表，把待处理链表分解成两个链表）。call_rcu总是将回调函数提交到第二批次链表中，如果发现第一批次链表为空（之前的call_rcu都已经处理完了），就将第二批次链表中的回调函数都移入第一批次链表（第二批次链表清空）；从回调函数被移入第一批次链表开始，如果所有CPU都发生了调度，则将第一批次链表中的回调函数都移入待处理链表（第一批次链表清空，同时第二批次链表中新的回调函数又被移过来）；待处理链表里面的回调函数都是等待被调用的，下一次进入软中断的时候就要调用它们。
什么时候检查“所有CPU都已发生调度”呢？并不是在CPU发生调度的时候。调度的时候只是做一个标记，标记这个CPU已经调度过了。而检查是放在每毫秒一次的时钟中断处理函数里面来进行的。
另外，这里提到的第二批次链表、第一批次链表、待处理链表其实是每个CPU维护一份的，这样可以避免操作链表时CPU之间的竞争。
rcuclassic的实现利用了禁止内核抢占，这对于一些实时性要求高的环境是不适用的（实时性要求不高则无妨），所以后来又有了rcupreempt的实现。

rcupreempt
rcupreempt是相对于rcuclassic禁止内核抢占而言的，rcupreempt允许内核抢占，以满足更高的实时性要求。
rcupreempt的实现思路是，通过计数器来记录rcu_read_lock与rcu_read_unlock发生的次数。读操作在rcu_read_lock时给计数器加1，rcu_read_unlock时则减1。只要计数器的值为0，说明所有的读操作都rcu_read_unlock了，则在此之前所有call_rcu提交的回调函数都可以被执行。不过，这样的话，新来的rcu_read_lock会使得之前的call_rcu不断延迟（如果rcu_read_unlock总是跟不上rcu_read_lock的速度，那么计数器可能永远都无法减为0。但是对于之前的某个call_rcu来说，它所关心的读操作却可能都已经rcu_read_unlock了）。所以，rcupreempt还是像rcuclassic那样，将call_rcu提交的回调函数分为两个批次，然后由两个计数器分别计数。
跟rcuclassic一样，call_rcu提交的回调函数总是加入到第二批次，所以rcu_read_lock总是增加第二批次的计数。而当第一批次为空时，第二批次将移动到第一批次，计数值也应该一起移过来。所以，rcu_read_unlock必须知道它应该减少哪个批次的计数（rcu_read_lock增加第二批次的计数，之后第一批次可能被处理，然后第二批次被移动到第一批次。这种情况下对应的rcu_read_unlock应该减少的是第一批次的计数了）。
实现上，rcupreempt提供了两个[等待队列+计数器]，并且交替的选择其中的一个作为“第一批次”。之前说的将第二批次移动到第一批次的过程实际上就是批次交替一次的过程，批次并没移动，只是两个[等待队列+计数器]的含义发生了交换。于是，rcu_read_lock的时候需要记录下现在增加的是第几个计数器的计数，rcu_read_unlock就相应减少那个计数就行了。
那么rcu_read_lock与rcu_read_unlock怎么对应上呢？rcupreempt已经不禁止内核抢占了，同一个读操作里面的rcu_read_lock和rcu_read_unlock可能发生在不同CPU上，不能通过CPU来联系rcu_read_lock与rcu_read_unlock，只能通过上下文，也就是执行rcu_read_lock与rcu_read_unlock的进程。所以，在进程控制块（task_struct）中新增了一个index字段，用来记录这个进程上执行的rcu_read_lock增加了哪个计数器的计数，于是这个进程上执行的rcu_read_unlock也应该减少相应的计数。
rcupreempt也维护了一个待处理链表。于是，当第一批次的计数为0时，第一批次里面的回调函数将被移动到待处理链表中，等到下一次进入软中断的时候就调用它们。然后第一批次被清空，两个批次做交换（相当于第二批次移动到第一批次）。
跟rcuclassic类似，对于计数值的检查并不是在rcu_read_unlock的时候进行的，rcu_read_unlock只管修改计数值。而检查也是放在每毫秒一次的时钟中断处理函数里面来进行的。
同样，这里提到的等待队列和计数器也是每个CPU维护一份的，以避免操作链表和计数器时CPU之间的竞争。那么当然，要检查第一批次计数为0，是需要把所有CPU的第一批次计数值进行相加的。

rcutree
最后说说rcutree。它跟rcuclassic的实现思路几乎是一模一样的，通过禁止抢占、检查每一个CPU是否已经发生过调度，来判断发生在某一批次rcu_call之前的所有读操作是否都已经rcu_read_unlock。并且实现上，批次的管理、各种队列、等等都几乎一样，CPU发生调度时也是通过设置一个标记来表示自己已经调度过了，然后又在时钟中断的处理程序中判断是否所有CPU都已经发生过调度……那么，不同之处在哪里呢？在于“判断是否每一个CPU都调度过”这一细节上。
rcuclassic对于多个CPU的管理是对称的，在时钟中断处理函数中，要判断是否每一个CPU都调度过就得去看每一个CPU所设置的标记，而这个“看”的过程势必是需要互斥的（因为这些标记也会被其他CPU读或写）。这样就造成了CPU之间的竞争。如果CPU个数不多，就这么竞争一下倒也无妨。要是CPU很多的话（比如64个？或更多？），那当然越少竞争越好。rcutree就是为了这种拥有很多CPU的环境而设计的，以期减少竞争。
rcutree的思路是提供一个树型结构，其中的每一个非叶子节点提供一个锁（代表了一次竞争），而每个CPU就对应到树的叶子节点上。然后呢？当需要判断“是否每一个CPU都调度过”的时候，CPU尝试在自己的父节点上锁（这个锁只会由它的子节点来竞争，而不会被所有CPU竞争），然后判断这个“父节点”的子节点（CPU）是否都已经调度过。如果不是，则显然“每一个CPU都调度过”不成立。而如果是，则再向上遍历，直到走到树根，那么就可以知道所有CPU都已经调度过了。使用这样的树型结构就缩小了每一次加锁的粒度，减少了CPU间的竞争。


BKL（大内核锁）



大内核锁这个简单且不常用的内核加锁机制一直是内核开发者之间颇具争议的话题。它在早期linux版本里的广泛使用，从2.4内核开始逐渐被各种各样的自旋锁替代，可是直到现在还不能完全将它抛弃；它曾经使用自旋锁实现，到了2.6.11版修改为信号量，可是在2.6.26-rc2又退回到使用自旋锁的老路上；它甚至引发了linux的创始人LinusTorvalds和著名的完全公平调度(CFS)算法的贡献者IngoMolnar之间的一场争议。这究竟是怎么回事呢？

1.1 应运而生，特立独行

使用过自旋锁或信号量这些内核互斥机制的人几乎不会想到还有大内核锁这个东西。和自旋锁或信号量一样，大内核锁也是用来保护临界区资源，避免出现多个处理器上的进程同时访问同一区域的。但这把锁独特的地方是，它不象自旋锁或信号量一样可以创建许多实例或者叫对象，每个对象保护特定的临界区。事实上整个内核只有一把这样的锁，一旦一个进程获得大内核锁，进入了被它保护的临界区，不但该临界区被锁住，所有被它保护的其它临界区都将无法访问，直到该进程释放大内核锁。这看似不可思议：一个进程在一个处理器上操作一个全局的链表，怎么可能导致其它进程无法访问另一个全局数组呢？使用两个自旋锁，一个保护链表，另一个保护数组不就解决了吗？可是如果你使用大内核锁，效果就是这样的。

大内核锁的产生是有其历史原因的。早期linux版本对对称多处理(SMP)器的支持非常有限，为了保证可靠性，对处理器之间的互斥采取了‘宁可错杀三千，不可放过一个’的方式：在内核入口处安装一把‘巨大’的锁，一旦一个处理器进入内核态就立刻上锁，其它将要进入内核态的进程只能在门口等待，以此保证每次只有一个进程处于内核态运行。这把锁就是大内核锁。有了大内核锁保护的系统当然可以安全地运行在多处理器上：由于同时只有一个处理器在运行内核代码，内核的执行本质上和单处理器没有什么区别；而多个处理器同时运行于进程的用户态也是安全的，因为每个进程有自己独立的地址空间。但是这样粗鲁地加锁其缺点也是显而易见的：多处理器对性能的提示只能体现在用户态的并行处理上，而在内核态下还是单线执行，完全无法发挥多处理器的威力。于是内核开发者就开始想办法逐步缩小这把锁保护的范围。实际上内核大部分代码是多处理器安全的，只有少数全局资源需要需要在做互斥加以保护，所以没必要限制同时运行于内核态处理器的个数。所有处理器都可随时进入内核态运行，只要把这些需要保护的资源一一挑出来，限制同时访问这些资源的处理器个数就可以了。这样一来，大内核锁从保护整个内核态缩小为零散地保护内核态某些关键片段。这是一个进步，可步伐还不够大，仍有上面提到的，‘锁了卧室厨房也没法进’的毛病。随着自旋锁的广泛应用，新的内核代码里已经不再有人使用大内核锁了。

1.2 食之无味，挥之不去

既然已经有了替代物，大内核锁应该可以‘光荣下岗’了。可事实上没这么简单。如果大内核锁仅仅是‘只有一个实例’的自旋锁，睿智的内核开发者早就把它替换掉了：为每一种处于自旋锁保护下的资源创建一把自旋锁，把大内核锁加锁/解锁替换成相应的自旋锁的加锁/解锁就可以了。但如今的大内核锁就象一个被宠坏的孩子，内核在一些关键点给予了它许多额外关照，使得大内核锁的替换变得有点烦。下面是IngoMolnar在一封名为 ’kill the BigKernel Lock (BKL)’的邮件里的抱怨：

The biggest technical complication is that the BKL is unlike anyother lock: it "self-releases" when schedule() is called.This makes the BKL spinlock very "sticky", "invisible"and viral: it's very easy to add it to a piece of code (evenunknowingly) and you never really know whether it's held or not.PREEMPT_BKL made it even more invisible, because it made its effectseven less visible to ordinary users.

这段话的大意是：最大的技术难点是大内核锁的与众不同：它在调用schedule()时能够‘自动释放’。这一点使得大内核锁非常麻烦和隐蔽：它使你能够非常容易地添加一段代码而几乎从不知道它锁上与否。PREEMPT_BKL选项使得它更加隐蔽，因为这导致它的效果在普通用户面前更加‘遁形’。

翻译linux开发者的话比看懂他们写的代码更难，但有一点很明白：是schedule()函数里对于大内核锁的自动释放导致了问题的复杂化。那就看看schedule()里到底对大内核锁执行了什么操作：

1 /*

2 * schedule() is the main scheduler function.

3 */

4 asmlinkage void __sched schedule(void)

5 {

…

19 release_kernel_lock(prev);

…

55 context_switch(rq, prev, next); /* unlocks the rq */

…

67 if (unlikely(reacquire_kernel_lock(current) < 0)) {

68 prev = rq->curr;

69 switch_count = &prev->nivcsw;

70 goto need_resched_nonpreemptible;

71 }

…

code 1.2 1 linux_2.6.34/kernel/sched.c

在第19行release_kernel_lock(prev)函数释放当前进程(prev)所占据的大内核锁，接着在第55行执行进程的切换，从当前进程prev切换到了下一个进程next。context_switch()可以看做一个超级函数，调用它不是去执行一段代码，而是去执行另一个进程。系统的多任务切换就是依靠这个超级函数从一个进程切换到另一个进程，从另一个进程再切换下一个进程，如此连续不断地轮转。只要被切走的进程还处于就绪状态，总有一天还会有机会调度回来继续运行，效果看起来就象函数context_switch()运行完毕返回到了schedule()。继续运行到第67行，调用函数reacquire_kernel_lock()。这是和release_kernel_lock()配对的函数，将前面释放的大内核锁又重新锁起来。If语句测试为真表示对大内核锁尝试加锁失败，这时可以做一些优化。正常的加锁应该是‘原地踏步’，在同一个地方反复查询大内核锁的状态，直到其它进程释放为止。但这样做会浪费宝贵的处理器时间，尤其是当运行队列里有进程在等待运行时。所以release_lernel_lock()只是做了’try_lock’的工作，即假如没人把持大内核锁就把它锁住，返回0表示成功；假如已经被锁住就立即返回-1表示失败。一旦失败就重新执行一遍schedule()的主体部分，检查运行队列，挑选一个合适的进程运行，等到下一次被调度运行时可能锁就解开了。这样做利用另一个进程（假如有进程在排队等候）的运行代替了原地死等，提高了处理器利用率。

除了在schedule()中的‘照顾’，大内核锁还有另外的优待：在同一进程中你可以对它反复嵌套加锁解锁，只要加锁个数和解锁个数能配上对就不会有任何问题，这是自旋锁望尘莫及的，同一进程里自旋锁如果发生嵌套加锁就会死锁。为此在进程控制块(PCB)中专门为大内核锁开辟了加锁计数器，即task_struct中的lock_depth域。该域的初始值为-1，表示进程没有获得大内核锁。每次加锁时lock_depth都会加1，再检查如果lock_depth为0就执行真正的加锁操作，这样保证在加了一次锁以后所有嵌套的加锁操作都会被忽略，从而避免了死锁。解锁过程正好相反，每次都将lock_depth减1，直到发现其值变为-1时就执行真正的解锁操作。

内核对大内核锁的偏袒导致开发者在锁住了它，进入被它保护的临界区后，执行了不该执行的代码却还无法察觉。其一：程序在锁住临界区后必须尽快退出，否则会阻塞其它将要进入临界区的进程。所以在临界区里绝对不可以调用schedule()函数，否则一旦发生进程切换何时能解锁就变得遥遥无期。另外在使用自旋锁保护的临界区中做进程切换很容易造成死锁。比如一个进程锁住了一把自旋锁，期间调用schedule()切换到另一个进程，而这个进程又要获得这把锁，这是系统就会挂死在这个进程等待解锁的自旋处。这个问题在大内核锁保护的临界区是不存在的，因为schedule()函数在调度到新进程之前会自动解锁已经获得的大内核锁；在切回该进程时又会自动将大内核锁锁住。用户在锁住了大内核锁后，几乎无法察觉期间是否用过schedule()函数。这一点就是上面IngoMolnar提到的’technicalcomplication’：将大内核锁替换成自旋锁后，万一在加锁过程中调用了schedule()，会造成不可预估的，灾难性的后果。当然作为一个训练有素的程序员，即使大内核锁放宽了约束条件，也不会在临界区中有意识地调用schedule()函数的。可是如果是调用陌生模块的代码，再高超的程序员也无法保证其中不会调用到该函数。其二就是上面提到的，在临界区中不能再次获得保护该临界区的锁，否则会死锁。可是由于大内核锁有加锁计数器的保护，怎样嵌套也不会有事。这也是一个’technicalcomplication’：将大内核锁替换成自旋锁后，万一发生了同一把自旋锁的嵌套加锁后果也是灾难性的。同schedule()函数一样，训练有素的程序员是不会有意识地多次锁住大内核锁，但在获得自旋锁后调用了陌生模块的代码就无法保证这些模块中不会再次使用大内核锁。这种情况在开发大型系统时非常常见：每个人都很小心地避免自己模块的死锁，可谁也无法避免当调用其它模块时可能引入的死锁问题。

IngoMolnar还提到了大内核锁的另一弊端：大内核锁没有被lockdep所覆盖。lockdep是linux内核的一个调试模块，用来检查内核互斥机制尤其是自旋锁潜在的死锁问题。自旋锁由于是查询方式等待，不释放处理器，比一般的互斥机制更容易死锁，故引入lockdep检查以下几种情况可能的死锁（lockdep将有专门的文章详细介绍，在此只是简单列举）：

· 同一个进程递归地加锁同一把锁；

· 一把锁既在中断（或中断下半部）使能的情况下执行过加锁操作，又在中断（或中断下半部）里执行过加锁操作。这样该锁有可能在锁定时由于中断发生又试图在同一处理器上加锁；

· 加锁后导致依赖图产生成闭环，这是典型的死锁现象。

由于大内核锁游离于lockdep之外，它自身以及和其它互斥机制之间的依赖关系没有受到监控，可能会导致死锁的场景也无法被记录下来，使得它的使用越来越混乱，处于失控状态。

如此看来，大内核锁已经成了内核的鸡肋，而且不能与时俱进，到了非整改不可的地步。可是将大内核锁完全从内核中移除将要面临重重挑战，对于那些散落在‘年久失修’，多年无人问津的代码里的大内核锁，更是没人敢去动它们。既然完全移除希望不大，那就想办法优化它也不失为一种权宜之计。

1.3 一改再改：无奈的选择

早些时候大内核锁是在自旋锁的基础上实现的。自旋锁是处理器之间临界区互斥常用的机制。当临界区非常短暂，比如只改变几个变量的值时，自旋锁是一种简单高效的互斥手段。但自旋锁的缺点是会增大系统负荷，因为在自旋等待过程中进程依旧占据处理器，这部分等待时间是在做无用功。尤其是使用大内核锁时，一把锁管所有临界区，发生‘碰撞’的机会就更大了。另外为了使进程能够尽快全速‘冲’出临界区，自旋锁在加锁的同时关闭了内核抢占式调度。因此锁住自旋锁就意味着在一个处理器上制造了一个调度‘禁区’：期间既不被其它进程抢占，又不允许调用schedule()进行自主进程切换。也就是说，一旦处理器上某个进程获得了自旋锁，该处理器就只能一直运行该进程，即便有高优先级的实时进程就绪也只能排队等候。调度禁区的出现增加了调度延时，降低了系统实时反应的速度，这与大家一直努力从事的内核实时化改造是背道而驰的。于是在2.6.7版本的linux中对自旋锁做了彻底改造，放弃了自旋锁改用信号量。信号量没有上面提到的两个问题：在等待信号量空闲时进程不占用处理器，处于阻塞状态；在获得信号量后内核抢占依旧是使能的，不会出现调度盲区。这样的解决方案应该毫无争议了。可任何事情都是有利有弊的。信号量最大的缺陷是太复杂了，每次阻塞一个进程时都要产生费时的进程上下文切换，信号量就绪唤醒等待的进程时又有一次上下文切换。除了上下文切换耗时，进程切换造成的TLB刷新，cache冷却等都有较大开销。如果阻塞时间比较长，达到毫秒级，这样的切换是值得的。但是大部分情况下只需在临界区入口等候几十上百个指令循环另一个进程就可以交出临界区，这时候这种切换就有点牛刀杀鸡了。这就好象去医院看普通门诊，当医生正在为病人看病时，别的病人在门口等待一会就会轮到了，不必留下电话号码回家睡觉，直到医生空闲了打电话通知再匆匆赶往医院。

由于使用信号量引起的进程频繁切换导致大内核锁在某些情况下出现严重性能问题，LinusTorvalds不得不考虑将大内核锁的实现改回自旋锁，自然调度延时问题也会跟着回来。这使得以‘延时迷(latencyjunkie)’自居的IngoMolnar不太高兴。但linux还是LinusTorvalds说了算，于是在2.6.26-rc2版大内核锁又变成了自旋锁，直到现在。总的来说LinusTorvalds的改动是有道理的。使用繁琐，重量级的信号量保护短暂的临界区确实不值得；而且Linux也不是以实时性见长的操作系统，不应该片面追求实时信而牺牲了整体性能。

1.4 日薄西山：谢幕在即

改回自旋锁并不意味着LinusTorvalds不关心调度延时，相反他真正的观点是有朝一日彻底铲除大内核锁，这一点他和IngoMolnar是英雄所见略同。可是由于铲除大内核锁的难度和风险巨大，IngoMolnar觉得‘在当前的游戏规则下解决大内核锁是不现实的’必须使用新的游戏规则。他专门建立一个版本分支叫做kill-the-BLK，在这个分支上将大内核锁替换为新的互斥机制，一步一步解决这个问题：

· 解决所有已知的，利用到了大内核锁自动解锁机制的临界区；也就是说，消除使用大内核锁的代码对自动解锁机制的依赖，使其更加接近普通的互斥机制；

· 添加许多调试设施用来警告那些在新互斥机制下不再有效的假设；

· 将大内核锁转换为普通的互斥体，并删除遗留在调度器里的自动解锁代码；

· 添加lockdep对它的监控；

· 极大简化大内核锁代码，最终将它从内核里删除。

这已经是两年前的事情了。现在这项工作还没结束，还在‘义无反顾’地向前推进。期待着在不远的将来大内核锁这一不和谐的音符彻底淡出linux的内核。



rwlock （读写锁）

读写锁实际是一种特殊的自旋锁，它把对共享资源的访问者划分成读者和写者，读者只对共享资源进行读访问，写者则需要对共享资源进行写操作。这种锁相对于自旋锁而言，能提高并发性，因为在多处理器系统中，它允许同时有多个读者来访问共享资源，最大可能的读者数为实际的逻辑CPU数。写者是排他性的，一个读写锁同时只能有一个写者或多个读者（与CPU数相关），但不能同时既有读者又有写者。

在读写锁保持期间也是抢占失效的。

如果读写锁当前没有读者，也没有写者，那么写者可以立刻获得读写锁，否则它必须自旋在那里，直到没有任何写者或读者。如果读写锁没有写者，那么读者可以立即获得该读写锁，否则读者必须自旋在那里，直到写者释放该读写锁。

读写锁的API看上去与自旋锁很象，只是读者和写者需要不同的获得和释放锁的API。下面是读写锁API清单：

rwlock_init(x)

该宏用于动态初始化读写锁x。

DEFINE_RWLOCK(x)

该宏声明一个读写锁并对其进行初始化。它用于静态初始化。

RW_LOCK_UNLOCKED

它用于静态初始化一个读写锁。

DEFINE_RWLOCK(x)等同于rwlock_tx = RW_LOCK_UNLOCKED

read_trylock(lock)

读者用它来尽力获得读写锁lock，如果能够立即获得读写锁，它就获得锁并返回真，否则不能获得锁，返回假。无论是否能够获得锁，它都将立即返回，绝不自旋在那里。

write_trylock(lock)

写者用它来尽力获得读写锁lock，如果能够立即获得读写锁，它就获得锁并返回真，否则不能获得锁，返回假。无论是否能够获得锁，它都将立即返回，绝不自旋在那里。

read_lock(lock)

读者要访问被读写锁lock保护的共享资源，需要使用该宏来得到读写锁lock。如果能够立即获得，它将立即获得读写锁并返回，否则，将自旋在那里，直到获得该读写锁。

write_lock(lock)

写者要想访问被读写锁lock保护的共享资源，需要使用该宏来得到读写锁lock。如果能够立即获得，它将立即获得读写锁并返回，否则，将自旋在那里，直到获得该读写锁。

read_lock_irqsave(lock, flags)

读者也可以使用该宏来获得读写锁，与read_lock不同的是，该宏还同时把标志寄存器的值保存到了变量flags中，并失效了本地中断。

write_lock_irqsave(lock, flags)

写者可以用它来获得读写锁，与write_lock不同的是，该宏还同时把标志寄存器的值保存到了变量flags中，并失效了本地中断。

read_lock_irq(lock)

读者也可以用它来获得读写锁，与read_lock不同的是，该宏还同时失效了本地中断。该宏与read_lock_irqsave的不同之处是，它没有保存标志寄存器。

write_lock_irq(lock)

写者也可以用它来获得锁，与write_lock不同的是，该宏还同时失效了本地中断。该宏与write_lock_irqsave的不同之处是，它没有保存标志寄存器。

read_lock_bh(lock)

读者也可以用它来获得读写锁，与与read_lock不同的是，该宏还同时失效了本地的软中断。

write_lock_bh(lock)

写者也可以用它来获得读写锁，与write_lock不同的是，该宏还同时失效了本地的软中断。

read_unlock(lock)

读者使用该宏来释放读写锁lock。它必须与read_lock配对使用。

write_unlock(lock)

写者使用该宏来释放读写锁lock。它必须与write_lock配对使用。

read_unlock_irqrestore(lock, flags)

读者也可以使用该宏来释放读写锁，与read_unlock不同的是，该宏还同时把标志寄存器的值恢复为变量flags的值。它必须与read_lock_irqsave配对使用。

write_unlock_irqrestore(lock, flags)

写者也可以使用该宏来释放读写锁，与write_unlock不同的是，该宏还同时把标志寄存器的值恢复为变量flags的值，并使能本地中断。它必须与write_lock_irqsave配对使用。

read_unlock_irq(lock)

读者也可以使用该宏来释放读写锁，与read_unlock不同的是，该宏还同时使能本地中断。它必须与read_lock_irq配对使用。

write_unlock_irq(lock)

写者也可以使用该宏来释放读写锁，与write_unlock不同的是，该宏还同时使能本地中断。它必须与write_lock_irq配对使用。

read_unlock_bh(lock)

读者也可以使用该宏来释放读写锁，与read_unlock不同的是，该宏还同时使能本地软中断。它必须与read_lock_bh配对使用。

write_unlock_bh(lock)

写者也可以使用该宏来释放读写锁，与write_unlock不同的是，该宏还同时使能本地软中断。它必须与write_lock_bh配对使用。

读写锁的获得和释放锁的方法也有许多版本，具体用哪个与自旋锁一样，因此参考自旋锁部分就可以了。只是需要区分读者与写者，读者要用读者版本，而写者必须用写者版本。