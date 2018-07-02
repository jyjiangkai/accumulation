##Linux自旋锁(Spinlock)

自旋锁是一种用于保护多线程共享资源的锁，与一般互斥锁(mutex)不同之处在于当自旋锁尝试获取锁时以忙等待(busy waiting)的形式不断地循环检查锁是否可用。
在多CPU的环境中，对持有锁较短的程序来说，使用自旋锁代替一般的互斥锁往往能够提高程序的性能。
无论是互斥锁，还是自旋锁，在任何时刻，最多只能有一个保持者，也就说，在任何时刻最多只能有一个执行单元获得锁。

###自旋锁介绍
自选锁的主要特征：当自旋锁被一个线程获得时，它不能被其它线程获得。如果其他线程尝试去phtread_spin_lock()获得该锁，那么它将不会从该函数返回，而是一直自旋(spin)，直到自旋锁可用为止。

使用自旋锁时要注意：
由于自旋时不释放CPU，因而持有自旋锁的线程应该尽快释放自旋锁，否则等待该自旋锁的线程会一直在哪里自旋，这就会浪费CPU时间。
持有自旋锁的线程在sleep之前应该释放自旋锁以便其他咸亨可以获得该自旋锁。内核编程中，如果持有自旋锁的代码sleep了就可能导致整个系统挂起。(下面会解释)
使用任何锁都需要消耗系统资源(内存资源和CPU时间)，这种资源消耗可以分为两类：
    1.建立锁所需要的资源
    2.当线程被阻塞时所需要的资源

POSIX提供的与自旋锁相关的函数有以下几个，都在<pthread.h>中。
####锁的初始化
int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
如果初始化自旋锁的线程设置第二个参数为PTHREAD_PROCESS_SHARED，那么该自旋锁不仅被初始化线程所在的进程中所有线程看到，而且可以被其他进程中的线程看到，PTHREAD_PROESS_PRIVATE则只被同一进程中线程看到。如果不设置该参数，默认为后者。

####锁的释放
int pthread_spin_destroy(pthread_spinlock_t *lock);

####锁的加锁
int pthread_spin_lock(pthread_spinlock_t *lock);
加锁函数，功能上文都说过了，不过这么一点值得注意：

####锁的加锁
int pthread_spin_trylock(pthread_spinlock_t *lock);
这个函数一般很少用到。

####锁的解锁
int pthread_spin_unlock(pthread_spinlock_t *lock);
解锁函数。不是持有锁的线程调用或者解锁一个没有lock的spin lock这样的行为都是undefined的。

###自旋锁和互斥锁的区别
从实现原理上来讲，mutex属于sleep-waiting类型的锁，而spin lock属于busy-waiting类型的锁。
*对于自旋锁来说，它只需要消耗很少的资源来建立锁；随后当线程被阻塞时，它就会一直重复检查看锁是否可用了，也就是说当自旋锁处于等待状态时它会一直消耗CPU时间。
*对于互斥锁来说，与自旋锁相比它需要消耗大量的系统资源来建立锁；随后当线程被阻塞时，线程的调度状态被修改，并且线程被加入等待线程队列；最后当锁可用 时，在获取锁之前，线程会被从等待队列取出并更改其调度状态；但是在线程被阻塞期间，它不消耗CPU资源。
*因此自旋锁和互斥锁适用于不同的场景。自旋锁适用于那些仅需要阻塞很短时间的场景，而互斥锁适用于那些可能会阻塞很长时间的场景。

###自旋锁与linux内核进程调度关系
如果临界区可能包含引起睡眠的代码则不能使用自旋锁，否则可能引起死锁。
先看下自旋锁的实现方法吧，自旋锁的基本形式如下：
spin_lock(&mr_lock):
//critical region
spin_unlock(&mr_lock);
总结下自旋锁的特点：
*单CPU非抢占内核下：自旋锁会在编译时被忽略（因为单CPU且非抢占模式情况下，不可能发生进程切换，时钟只有一个进程处于临界区（自旋锁实际没什么用了）
*单CPU抢占内核下：自选锁仅仅当作一个设置抢占的开关（因为单CPU不可能有并发访问临界区的情况，禁止抢占就可以保证临街区唯一被拥有）
*多CPU下：此时才能完全发挥自旋锁的作用，自旋锁在内核中主要用来防止多处理器中并发访问临界区，防止内核抢占造成的竞争。

###spin_lock和mutex实际效率对比
1.++i是否需要加锁?
我分别使用POSIX的spin_lock和mutex写了两个累加的程序，启动了两个线程，并利用时间戳计算它们执行完累加所用的时间。

下面这个是使用spin_lock的代码，我启动两个线程同时对num进行++，使用spin_lock保护临界区，实际上可能会有疑问++i(++i和++num本文中是一个意思)为什么还要加锁？

i++需要加锁是很明显的事情，对i++的操作的影响是，它一般是三步曲，从内存中取出i放入寄存器中，在寄存器中对i执行inc操作，然后把i放回内存中。这三步明显是可打断的，所以需要加锁。

但是++i可能就有点犹豫了。实际上印象流是不行的，来看一下i++和++i的汇编代码，其实他们是一样的，都是三步，我只上一个图就行了，如下：
movl $0, -4(%rbp)
addl $1, -4(%rbp)
movl $0, %eax
popq %rbp
所以++i也不是原子操作，在多核的机器上，多个线程在读取内存中的i时，可能读取到同一个值，这就导致多个线程同时执行+1，但实际上它们得到的结果是一样的，即i只加了一次。

2.spin_lock代码
首先是spin_lock实现两个线程同时加一个数，每个线程均++num，然后计算花费的时间。
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

int num = 0;
pthread_t thread_t1;
pthread_t thread_t2;
pthread_spinlock_t spin_lock;
 
int64_t get_current_timestamp()
{
    struct timeval now = {0, 0};
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 * 1000 + now.tv_usec;
}

void *thread_proc(void *arg)
{
    for(int i=0; i<100000000; ++i){
        pthread_spin_lock(&spin_lock);
        ++num;
        pthread_spin_unlock(&spin_lock);
    }   
}
 
int main()
{
    pthread_spin_init(&spin_lock, PTHREAD_PROCESS_PRIVATE);//maybe PHREAD_PROCESS_PRIVATE or PTHREAD_PROCESS_SHARED
 
    int64_t start = get_current_timestamp();
 
    (void)pthread_create(&thread_t1, NULL, thread_proc, NULL);
    (void)pthread_create(&thread_t2, NULL, thread_proc, NULL);

    pthread_join(thread_t1, NULL);
    pthread_join(thread_t2, NULL);
 
    std::cout<<"num:"<<num<<std::endl;
    int64_t end = get_current_timestamp();
    std::cout<<"cost:"<<end-start<<std::endl;
 
    pthread_exit(0);  
 
    return 0;
}

3.mutex代码
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
 
int num = 0;
pthread_t thread_t1;
pthread_t thread_t2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 
int64_t get_current_timestamp()
{   
    struct timeval now = {0, 0}; 
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000 * 1000 + now.tv_usec;
}
 
void *thread_proc(void *arg)
{
    for(int i=0; i<100000000; ++i){
        pthread_mutex_lock(&mutex);
        ++num;
        pthread_mutex_unlock(&mutex);
    }   
}
 
int main()
{
    int64_t start = get_current_timestamp();

    (void)pthread_create(&thread_t1, NULL, thread_proc, NULL);
    (void)pthread_create(&thread_t2, NULL, thread_proc, NULL);

    pthread_join(thread_t1, NULL);
    pthread_join(thread_t2, NULL);
    
    std::cout<<"num:"<<num<<std::endl;
    int64_t end = get_current_timestamp();
    std::cout<<"cost:"<<end-start<<std::endl;

    pthread_mutex_destroy(&mutex);    //maybe you always foget this
    pthread_exit(0);  
 
    return 0;
}

4.结果分析
得出的结果如图，num是最终结果，cost是花费时间，单位为us，main2是使用spin lock，
//spin lock
SZX1000411335:/usr1/home/jiangkai/cpp # g++ -o main main.cpp -lpthread
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:11690563
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:7474905
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:12336958
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:11742599
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:10557203
//mutex lock
SZX1000411335:/usr1/home/jiangkai/cpp # g++ -o main main.cpp -lpthread
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:12536262
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:14567421
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:14485015
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:16207016
SZX1000411335:/usr1/home/jiangkai/cpp # ./main
num:200000000
cost:16524145
SZX1000411335:/usr1/home/jiangkai/cpp #

显然，在临界区只有++num这一条语句的情况下，自旋锁相对花费的时间短一些，实际上它们有可能接近的情况，取决于CPU的调度情况，但始终会是自旋锁执行的效率在本情况中花费时间更少。

我修改了两个程序中临界区的代码，改为：
for(int i=0; i<100000000; ++i){
    pthread_spin_lock(&spin_lock);
    ++num;
    for(int i=0; i<100; ++i){
        //do nothing
    }
    pthread_spin_unlock(&spin_lock);
}

实验结果是如此的明显，仅仅是在临界区内加了一个10圈的循环，spin lock就需要花费比mutex更长的时间了。
所以，自旋锁虽然lock/unlock的性能更好（花费很少的CPU指令），但是它只适应于临界区运行时间很短的场景。实际开发中，程序员如果对自己程序的锁行为不是很了解，否则使用自旋锁不是一个好主意。更保险的方法是使用mutex，如果对性能有进一步的要求，那么再考虑自旋锁。

