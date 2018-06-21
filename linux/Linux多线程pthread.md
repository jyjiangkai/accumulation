##linux多线程pthread

####所需文件头：	
`#include <pthread.h>`

####创建一个线程
函数原型：
int pthread_create(pthread_t* thread, pthread_attr_r* attr, void*(*start_routine) (void*), void* arg)

函数传入值：
thread:线程标识符
attr: 线程属性设置 null表示采用默认
start_roitine : 线程函数的起始地址
arg :传递给start_routine的参数

函数返回值：
成功：0
出错：-1

####退出一个线程
函数原型：
void pthread_exit(void *retval)

函数传入值：
retval：调用者线程的返回值，可由其他函数如pthread_join来检索获取。

####等待指定线程终止
函数原型：
int pthread_join(pthread_t th, void **thread_return)

函数传入值：
th: 等待线程的标识符 
thread_return:用户定义的指针，用来存储被等待线程的返回值（不为NULL时）

函数返回值：
成功：0
出错：-1

####取消线程
有时候，我们想让一个线程能够请求另外一个线程结束，就像给它发送一个信号似的，用线程是可以完成这一操作的。
函数原型：
int pthread_cancel(pthread_t thread);

函数传入值：
thread:线程的标识符

函数返回值：
成功：0
出错：-1

####设置线程取消状态
在取消线程请求的接收端，事情会稍微复杂一些。线程可以用pthread_setcancelstate设置自己的取消状态，下面是这个函数的定义：
函数原型：
int pthread_setcancelstate(int state,int *oldstate);

函数传入值：
state:可以是PTHREAD_CANCEL_ENABLE，这个值允许线程接收取消请求；还可以是PTHREAD_CANCEL_DISABLE,它的作用是屏蔽它们。 
线程以前的取消状态可以用oldstate指针检索出来。如果没兴趣可以传一个NULL进去。

函数返回值：
成功：0
出错：-1

####设置线程取消类型
如果取消请求被接受了，线程会进入第二个控制层次----用pthread_setcanceltype设置取消类型。
函数原型：
int pthread_setcanceltype(int type,int *oldstate);

函数传入值：
type:可以有两种取值，一个是PTHREAD_CANCEL_ASYNCHORONOUS，接收到取消请求之后立刻采取行动；另一个是PTHREAD_CANCEL_DEFERRED，在接收到取消请求之后、采取实际行动之前，先执行以下几个函数之一：pthread_join、pthread_cond_wait、pthread_cond_timewait、pthread_testcancel、sem_wait或sigwait。

函数返回值：
成功：0
出错：-1


####单个线程有三种退出方式：
1.线程从启动例程中返回(return方式)，返回值是现成的退出码
2.线程被同一进程内的其他线程取消
3.线程调用pthread_exit退出。void pthread_exit(void *rval_ptr)

####线程识别函数
1.pthread_t用于表示Thread ID，具体内容根据实现的不同而不同，有可能是一个Structure，因此不能将其看作为整数
2.pthread_equal函数用于比较两个pthread_t是否相等
函数原型：
int pthread_equal(pthread_t tid1, pthread_t tid2)
3.pthread_self函数用于获得本线程的thread id
函数原型：
pthread_t pthread_self(void);

####进程函数和线程函数的相关性：
|Process Primitive	|Thread Primitive	|Description
|fork			|pthread_create		|创建新的控制流
|exit			|pthread_exit		|退出已有的控制流
|waitpid		|pthread_join		|等待控制流并获得结束代码
|atexit			|pthread_cleanup_push	|注册在控制流退出时候被调用的函数
|getpid			|pthread_self		|获得控制流的id
|abort			|pthread_cancel		|请求非正常退出

####线程同步
1.互斥量：Mutex
    a.用于互斥访问
    b.类型为pthread_mutex_t，必须被初始化为PTHREAD_MUTEX_INITIALIZER(用于静态分配的mutex，等价于 pthread_mutex_init(..., NULL))或者调用pthread_mutex_init。Mutex也应该用pthread_mutex_destroy来销毁。这两个函数原型如下：(attr的具体含义下一章讨论)
    #include <pthread.h>
    int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
    int pthread_mutex_destroy(pthread_mutex_t *mutex);
    c.pthread_mutex_lock用于Lock Mutex，如果Mutex已经被Lock，该函数调用会Block直到Mutex被Unlock，然后该函数会Lock Mutex并返回。pthread_mutex_trylock类似，只是当Mutex被Lock的时候不会Block，而是返回一个错误值EBUSY。 pthread_mutex_unlock则是unlock一个mutex。这三个函数原型如下：
    #include <pthread.h>
    int pthread_mutex_lock(pthread_mutex_t *mutex);
    int pthread_mutex_trylock(pthread_mutex_t *mutex);
    int pthread_mutex_unlock(pthread_mutex_t *mutex);
 
2.读写锁：Reader-Writer Locks
    a.多个线程可以同时获得读锁(Reader-Writer lock in read mode)，但是只有一个线程能够获得写锁(Reader-writer lock in write mode)
    b.读写锁有三种状态
        i.一个或者多个线程获得读锁，其他线程无法获得写锁
        ii.一个线程获得写锁，其他线程无法获得读锁
        iii.没有线程获得此读写锁
    c.类型为pthread_rwlock_t
    d.创建和关闭方法如下：
    #include <pthread.h>
    int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
    int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
    e.获得读写锁的方法如下：
    #include <pthread.h>
    int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
    int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
 
    pthread_rwlock_rdlock：获得读锁
    pthread_rwlock_wrlock：获得写锁
    pthread_rwlock_unlock：释放锁，不管是读锁还是写锁都是调用此函数
    注意具体实现可能对同时获得读锁的线程个数有限制，所以在调用 pthread_rwlock_rdlock的时候需要检查错误值，而另外两个pthread_rwlock_wrlock和 pthread_rwlock_unlock则一般不用检查，如果我们代码写的正确的话。
3.Conditional Variable：条件变量
    a.条件必须被Mutex保护起来
    b.类型为：pthread_cond_t，必须被初始化为PTHREAD_COND_INITIALIZER(用于静态分配的条件，等价于pthread_cond_init(..., NULL))
    #include <pthread.h>
    int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condxattr_t *restrict attr);
    int pthread_cond_destroy(pthread_cond_t *cond);
    c.pthread_cond_wait函数用于等待条件发生(=true)。pthread_cond_timedwait类似，只是当等待超时的时候返回一个错误值ETIMEDOUT。超时的时间用timespec结构指定。此外，两个函数都需要传入一个Mutex用于保护条件
    #include <pthread.h>
    int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
    int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict timeout);
    d.timespec结构定义如下：
    struct timespec {
       time_t tv_sec;       /* seconds */
       long   tv_nsec;      /* nanoseconds */
    };
    注意timespec的时间是绝对时间而非相对时间，因此需要先调用gettimeofday函数获得当前时间，再转换成timespec结构，加上偏移量。
    e.有两个函数用于通知线程条件被满足（=true）：
    #include <pthread.h>
    int pthread_cond_signal(pthread_cond_t *cond);
    int pthread_cond_broadcast(pthread_cond_t *cond);
    两者的区别是前者会唤醒单个线程，而后者会唤醒多个线程。

在传统的Unix模型中，当一个进程需要由另一个实体执行某件事时，该进程派生(fork)一个子进程，让子进程去进行处理。Unix下的大多数网络服务器程序都是这么编写的，即父进程接受连接，派生子进程，子进程处理与客户的交互。虽然这种模型很多年来使用得很好，但是fork时有一些问题：
    1.fork是昂贵的。内存映像要从父进程拷贝到子进程，所有描述字要在子进程中复制等等。目前有的Unix实现使用一种叫做写时拷贝(copy-on-write)的技术，可避免父进程数据空间向子进程的拷贝。尽管有这种优化技术，fork仍然是昂贵的。
    2.fork子进程后，需要用进程间通信(IPC)在父子进程之间传递信息。Fork之前的信息容易传递，因为子进程从一开始就有父进程数据空间及所有描述字的拷贝。但是从子进程返回信息给父进程需要做更多的工作。

线程有助于解决这两个问题。线程有时被称为轻权进程(lightweight process)，因为线程比进程“轻权”，一般来说，创建一个线程要比创建一个进程快10～100倍。
一个进程中的所有线程共享相同的全局内存，这使得线程很容易共享信息，但是这种简易性也带来了同步问题。

一个进程中的所有线程不仅共享全局变量，而且共享：进程指令、大多数数据、打开的文件（如描述字）、信号处理程序和信号处置、当前工作目录、用户ID和组ID。但是每个线程有自己的线程ID、寄存器集合（包括程序计数器和栈指针）、栈（用于存放局部变量和返回地址）、error、信号掩码、优先级。


####线程数据
在单线程的程序里，有两种基本的数据：全局变量和局部变量。但在多线程程序里，还有第三种数据类型：线程数据(TSD: Thread-Specific Data)。它和全局变量很象，在线程内部，各个函数可以象使用全局变量一样调用它，但它对线程外部的其它线程是不可见的。例如我们常见的变量errno，它返回标准的出错信息。它显然不能是一个局部变量，几乎每个函数都应该可以调用它；但它又不能是一个全局变量，否则在A线程里输出的很可能是B线程的出错信息。要实现诸如此类的变量，我们就必须使用线程数据。我们为每个线程数据创建一个键，它和这个键相关联，在各个线程里，都使用这个键来指代线程数据，但在不同的线程里，这个键代表的数据是不同的，在同一个线程里，它代表同样的数据内容。

和线程数据相关的函数主要有4个：创建一个键；为一个键指定线程数据；从一个键读取线程数据；删除键。
创建键的函数原型为：
int pthread_key_create(pthread_key_t*__key, void (*__destr_function)(void*));
第一个参数为指向一个键值的指针，第二个参数指明了一个destructor函数，如果这个参数不为空，那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块。这个函数常和函数pthread_once(pthread_once_t*once_control, void (*initroutine) (void))一起使用，为了让这个键只被创建一次。函数pthread_once声明一个初始化函数，第一次调用pthread_once时它执行这个函数，以后的调用将被它忽略。
int pthread_key_delete(pthread_key_t *key);
该函数用于删除一个由pthread_key_create函数调用创建的键。调用成功返回值为0，否则返回错误代码。
在下面的例子中，我们创建一个键，并将它和某个数据相关联。我们要定义一个函数 createWindow，这个函数定义一个图形窗口（数据类型为Fl_Window *，这是图形界面开发工具FLTK中的数据类型）。由于各个线程都会调用这个函数，所以我们使用线程数据。
/* 声明一个键*/
pthread_key_t myWinKey;
/* 函数 createWindow */
void createWindow ( void ) {
Fl_Window * win;
static pthread_once_t once= PTHREAD_ONCE_INIT;
/* 调用函数createMyKey，创建键*/
pthread_once ( & once, createMyKey) ;
/*win指向一个新建立的窗口*/
win=new Fl_Window( 0, 0, 100, 100, "MyWindow");
/* 对此窗口作一些可能的设置工作，如大小、位置、名称等*/
setWindow(win);
/* 将窗口指针值绑定在键myWinKey上*/
pthread_setpecific ( myWinKey, win);
}
/* 函数 createMyKey，创建一个键，并指定了destructor */
void createMyKey ( void ) {
pthread_keycreate(&myWinKey, freeWinKey);
}
/* 函数 freeWinKey，释放空间*/
void freeWinKey ( Fl_Window * win){
delete win;
}
这样，在不同的线程中调用函数createMyWin，都可以得到在线程内部均可见的窗口变量，这个变量通过函数 pthread_getspecific得到。在上面的例子中，我们已经使用了函数pthread_setspecific来将线程数据和一个键绑定在一起。这两个函数的原型如下：
　　
int pthread_setspecific __P ((pthread_key_t __key,__const void *__pointer)); 该函数设置一个线程专有数据的值，赋给由pthread_key_create 创建的键，调用成功返回值为0，否则返回错误代码。
void *pthread_getspecific __P ((pthread_key_t __key));                                                  该函数获得绑定到指定键上的值。调用成功，返回给定参数key 所对应的数据。如果没有数据连接到该键，则返回NULL。

　　这两个函数的参数意义和使用方法是显而易见的。要注意的是，用pthread_setspecific为一个键指定新的线程数据时，必须自己释放原有的线程数据以回收空间。这个过程函数pthread_key_delete用来删除一个键，这个键占用的内存将被释放，但同样要注意的是，它只释放键占用的内存，并不释放该键关联的线程数据所占用的内存资源，而且它也不会触发函数pthread_key_create中定义的destructor函数。线程数据的释放必须在释放键之前完成。

注意：pthread_setspecfic和pthread_getspecfic要成对出现，不然一个set的值，第一个get取的值是正确的，第二个就不正确的，如果要2个get就需要2个set,才能取到对应的值。

####互斥锁
假设各个线程向同一个文件顺序写入数据，最后得到的结果是不可想象的。所以用互斥锁来保证一段时间内只有一个线程在执行一段代码。
使用int pthread_mutex_lock锁住互斥锁，使用int pthread_mutex_unlock解琐。
如果我们试图为一个已被其他线程锁住的互斥锁加锁，程序便会阻塞直到该互斥对象解锁。
如果在共享内存中分配一个互斥锁，我们必须在运行时调用ptgread_mutex_init函数进行初始化。
void reader_function(void);
void writer_function(void); 
char buffer;
int buffer_has_item=0;
pthread_mutex_t mutex;
struct timespec delay;
void main(void)
{
    pthread_t reader;
    /* 定义延迟时间*/
    delay.tv_sec = 2;
    delay.tv_nec = 0;
    /* 用默认属性初始化一个互斥锁对象*/
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&reader, pthread_attr_default, (void*)&reader_function), NULL);
    writer_function();
}
void writer_function(void)
{
    while(1)
    {
	/* 锁定互斥锁*/
	pthread_mutex_lock(&mutex);
	if (buffer_has_item == 0)
	{
	    buffer=make_new_item( );
	    buffer_has_item=1;
	}
	/* 打开互斥锁*/
	pthread_mutex_unlock(&mutex);
	pthread_delay_np(&delay);
    }
}
void reader_function(void)
{
    while(1)
    {
	pthread_mutex_lock(&mutex);
	if(buffer_has_item == 1)
	{
	    consume_item(buffer);
	    buffer_has_item=0;
	}
	pthread_mutex_unlock(&mutex);
	pthread_delay_np(&delay);
    }
}

函数pthread_mutex_init用来生成一个互斥锁。NULL参数表明使用默认属性。如果需要声明特定属性的互斥锁，须调用函数 pthread_mutexattr_init。函数pthread_mutexattr_setpshared和函数 pthread_mutexattr_settype用来设置互斥锁属性。前一个函数设置属性pshared，它有两个取值， PTHREAD_PROCESS_PRIVATE和PTHREAD_PROCESS_SHARED。前者用来不同进程中的线程同步，后者用于同步本进程的不同线程。在上面的例子中，我们使用的是默认属性PTHREAD_PROCESS_PRIVATE。后者用来设置互斥锁类型，可选的类型有PTHREAD_MUTEX_NORMAL、PTHREAD_MUTEX_ERRORCHECK、PTHREAD_MUTEX_RECURSIVE和PTHREAD_MUTEX_DEFAULT。它们分别定义了不同的上所、解锁机制，一般情况下，选用最后一个默认属性。
需要注意的是在使用互斥锁的过程中很有可能会出现死锁：两个线程试图同时占用两个资源，并按不同的次序锁定相应的互斥锁，例如两个线程都需要锁定互斥锁1和互斥锁2，a线程先锁定互斥锁1，b 线程先锁定互斥锁2，这时就出现了死锁。此时我们可以使用函数 pthread_mutex_trylock，它是函数pthread_mutex_lock的非阻塞版本，当它发现死锁不可避免时，它会返回相应的信息，程序员可以针对死锁做出相应的处理。另外不同的互斥锁类型对死锁的处理不一样，但最主要的还是要程序员自己在程序设计注意这一点。

####条件变量
互斥锁一个明显的缺点是它只有两种状态：锁定和非锁定。而条件变量通过允许线程阻塞和等待另一个线程发送信号的方法弥补了互斥锁的不足，它常和互斥锁一起使用。使用时，条件变量被用来阻塞一个线程，当条件不满足时，线程往往解开相应的互斥锁并等待条件发生变化。一旦其它的某个线程改变了条件变量，它将通知相应的条件变量唤醒一个或多个正被此条件变量阻塞的线程。这些线程将重新锁定互斥锁并重新测试条件是否满足。一般说来，条件变量被用来进行线程间的同步。
条件变量的结构为pthread_cond_t，函数pthread_cond_init()被用来初始化一个条件变量。它的原型为：
int pthread_cond_init __P ((pthread_cond_t *__cond,__const pthread_condattr_t *__cond_attr));

其中cond是一个指向结构pthread_cond_t的指针，cond_attr是一个指向结构pthread_condattr_t的指针。结构pthread_condattr_t是条件变量的属性结构，和互斥锁一样我们可以用它来设置条件变量是进程内可用还是进程间可用，默认值是PTHREAD_PROCESS_PRIVATE，即此条件变量被同一进程内的各个线程使用。注意初始化条件变量只有未被使用时才能重新初始化或被释放。
在pthread中，条件变量是一个pthread_cond_t类型的变量，条件变量使用下面两个函数：

pthread_cond_wait 函数用于阻塞，线程可以被函数pthread_cond_signal和函数   pthread_cond_broadcast唤醒，但是要注意的是，条件变量只是起阻塞和唤醒线程的作用，具体的判断条件还需用户给出，例如一个变量是否为0等等，这一点我们从后面的例子中可以看到。线程被唤醒后，它将重新检查判断条件是否满足，如果还不满足，一般说来线程应该仍阻塞在这里，被等待被下一次唤醒。这个过程一般用while语句实现。
另一个用来阻塞线程的函数是pthread_cond_timedwait()它比函数pthread_cond_wait()多了一个时间参数，经历abstime段时间后，即使条件变量不满足，阻塞也被解除。
函数pthread_cond_signal()用来释放被阻塞在条件变量cond上的一个线程。
函数pthread_cond_broadcast(pthread_cond_t *cond)用来唤醒所有被阻塞在条件变量cond上的线程。这些线程被唤醒后将再次竞争相应的互斥锁，所以必须小心使用这个函数。
下面是使用函数pthread_cond_wait()和函数pthread_cond_signal()的一个简单的例子:
pthread_mutex_t count_lock;
pthread_cond_t count_nonzero;
unsigned count;
decrement_count()
{
    pthread_mutex_lock(&count_lock);
    while(count==0) 
        pthread_cond_wait(&count_nonzero, &count_lock);
    count=count-1;
    pthread_mutex_unlock(&count_lock);
}

increment_count()
{
    pthread_mutex_lock(&count_lock);
    if(count==0)
        pthread_cond_signal(&count_nonzero);
    count=count+1;
    pthread_mutex_unlock(&count_lock);
}
count 值为0时， decrement函数在pthread_cond_wait处被阻塞，并打开互斥锁count_lock。此时，当调用到函数 increment_count时，pthread_cond_signal()函数改变条件变量，告知decrement_count()停止阻塞。


互斥是两个线程之间不可以同时运行，他们会相互排斥，必须等待一个线程运行完毕，另一个才能运行，而同步也是不能同时运行，但他是必须要安照某种次序来运行相应的线程(也是一种互斥)。
【总结】
互斥：是指某一资源同时只允许一个访问者对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的。
同步：是指在互斥的基础上（大多数情况），通过其它机制实现访问者对资源的有序访问。在大多数情况下，同步已经实现了互斥，特别是所有写入资源的情况必定是互斥的。少数情况是指可以允许多个访问者同时访问资源。

读写锁特点：
1）多个读者可以同时进行读  
2）写者必须互斥（只允许一个写者写，也不能读者写者同时进行）  
3）写者优先于读者（一旦有写者，则后续读者必须等待，唤醒时优先考虑写者）  

互斥锁特点：  
一次只能一个线程拥有互斥锁，其他线程只有等待  

互斥锁
pthread_mutex_init()  
pthread_mutex_lock()  
pthread_mutex_unlock()  
读写锁
pthread_rwlock_init()  
pthread_rwlock_rdlock()  
pthread_rwlock_wrlock()  
pthread_rwlock_unlock()  
条件变量
pthread_cond_init()  
pthread_cond_wait()  
pthread_cond_signal()  


#include <stdio.h>  
#include <pthread.h>  
#include <unistd.h>  
  
pthread_mutex_t counter_lock;   //互斥锁  
pthread_cond_t counter_nonzero; //条件变量  
int counter = 0;  
int estatus = -1;  
  
void *decrement_counter(void *argv);  
void *increment_counter(void *argv);  
  
//******* 主函数 *******//  
int main(int argc, char **argv)  
{  
    printf("counter: %d\n", counter);  
    pthread_t thd1, thd2;  
    int ret;  
  
    //初始化  
    pthread_mutex_init(&counter_lock, NULL);  
    pthread_cond_init(&counter_nonzero, NULL);  
      
    ret = pthread_create(&thd1, NULL, decrement_counter, NULL); //创建线程1  
    if(ret){  
        perror("del:\n");  
        return 1;  
    }  
  
    ret = pthread_create(&thd2, NULL, increment_counter, NULL); //创建线程2  
    if(ret){  
        perror("inc: \n");  
        return 1;  
    }  
  
    int counter = 0;  
    while(counter != 10){  
        printf("counter(main): %d\n", counter); //主线程  
        sleep(1);  
        counter++;  
    }  
  
    pthread_exit(0);  
      
    return 0;  
}  
  
void *decrement_counter(void *argv)  
{  
    printf("counter(decrement): %d\n", counter);  
    pthread_mutex_lock(&counter_lock);  
    while(counter == 0)  
        pthread_cond_wait(&counter_nonzero, &counter_lock); //进入阻塞(wait)，等待激活(signal)  
      
    printf("counter--(before): %d\n", counter);      
    counter--; //等待signal激活后再执行  
    printf("counter--(after): %d\n", counter);      
    pthread_mutex_unlock(&counter_lock);   
  
    return &estatus;  
}  
  
void *increment_counter(void *argv)  
{  
    printf("counter(increment): %d\n", counter);  
    pthread_mutex_lock(&counter_lock);  
    if(counter == 0)  
        pthread_cond_signal(&counter_nonzero); //激活(signal)阻塞(wait)的线程(先执行完signal线程，然后再执行wait线程)  
  
    printf("counter++(before): %d\n", counter);      
    counter++;   
    printf("counter++(after): %d\n", counter);      
    pthread_mutex_unlock(&counter_lock);  
  
    return &estatus;  
}  



#include<stdio.h>  
#include<pthread.h>  
#include<string.h>  
#include<sys/types.h>  
#include<unistd.h>  
pthread_t main_tid;  
void print_ids(const char *str)  
{  
    pid_t pid;      //进程id  
    pthread_t tid;  //线程id  
    pid = getpid();       //获取当前进程id  
    tid = pthread_self(); //获取当前线程id  
    printf("%s pid: %u tid: %u (0x%x)\n",  
                str,  
                (unsigned int)pid,  
                (unsigned int)tid,  
                (unsigned int)tid);  
}  
void *func(void *arg)  
{  
    print_ids("new  thread:");  
    return ((void *)0);  
}  
int main()  
{  
    int err;  
    err = pthread_create(&main_tid, NULL, func, NULL); //创建线程  
    if(err != 0){  
        printf("create thread error: %s\n",strerror(err));  
        return 1;  
    }  
    printf("main thread: pid: %u tid: %u (0x%x)\n",   
                (unsigned int)getpid(),  
                (unsigned int)pthread_self(),  
                (unsigned int)pthread_self());  
    print_ids("main thread:");  
    sleep(1);  
    return 0;  
}  


#include <stdio.h>  
#include <pthread.h>  
#include <unistd.h>  
  
pthread_key_t key; //声明参数key  
  
void echomsg(void *arg) //析构处理函数  
{  
    printf("destruct executed in thread = %u, arg = %p\n",   
                (unsigned int)pthread_self(),  
                arg);     
}  
  
void *child_1(void *arg)  
{  
    pthread_t tid;  
     
    tid = pthread_self();  
    printf("%s: thread %u enter\n", (char *)arg, (unsigned int)tid);  
      
    pthread_setspecific(key, (void *)tid);  // 与key值绑定的value(tid)  
    printf("%s: thread %u returns %p\n",    // %p 表示输出指针格式   
                (char *)arg,  
                (unsigned int)tid,   
                pthread_getspecific(key));  // 获取key值的value  
    sleep(1);  
    return NULL;  
}  
  
void *child_2(void *arg)  
{  
    pthread_t tid;  

    tid = pthread_self();  
    printf("%s: thread %u enter\n", (char *)arg, (unsigned int)tid);  

    pthread_setspecific(key, (void *)tid);  
    printf("%s: thread %u returns %p\n",   
                (char *)arg,  
                (unsigned int)tid,   
                pthread_getspecific(key));  
    sleep(1);  
    return NULL;  
}  
  
//******* 主函数 *******//  
int main(void)  
{  
    pthread_t tid1, tid2;  
      
    printf("hello main\n");  
      
    pthread_key_create(&key, echomsg); //创建key  
      
    pthread_create(&tid1, NULL, child_1, (void *)"child_1"); //创建带参数的线程，需要强制转换  
    pthread_create(&tid2, NULL, child_2, (void *)"child_1");  
  
    sleep(3);  
    pthread_key_delete(key); //清除key  
    printf("bye main\n");  
      
    pthread_exit(0);  
    return 0;  
}  


#include <stdio.h>  
#include <pthread.h>  
#include <unistd.h>  
  
pthread_once_t once = PTHREAD_ONCE_INIT; //声明变量  
  
//once_run()函数仅执行一次，且究竟在哪个线程中执行是不定的  
//尽管pthread_once(&once,once_run)出现在两个线程中  
//函数原型：int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))  
void once_run(void)  
{  
    printf("Func: %s in thread: %u/n",   
                __func__,   
                (unsigned int)pthread_self());  
}  
  
void *child_1(void *arg)  
{  
    pthread_t tid;  
  
    tid = pthread_self();  
    pthread_once(&once, once_run); //调用once_run  
    printf("%s: thread %d returns/n", (char *)arg, (unsigned int)tid);  
  
    return NULL;  
}  
  
void *child_2(void *arg)  
{  
    pthread_t tid;  
  
    tid = pthread_self();  
    pthread_once(&once, once_run); //调用once_run  
    printf("%s: thread %d returns/n", (char *)arg, (unsigned int)tid);  
  
    return NULL;  
}  
  
//******* main *******//  
int main(void)  
{  
    pthread_t tid1, tid2;  
  
    printf("hello main/n");  
    pthread_create(&tid1, NULL, child_1, (void *)"child_1");  
    pthread_create(&tid2, NULL, child_2, (void *)"child_2");  
  
    pthread_join(tid1, NULL);  //main主线程等待线程tid1返回  
    pthread_join(tid2, NULL);  //main主线程等待线程tid2返回  
    printf("bye main/n");  
  
    return 0;  
}  
