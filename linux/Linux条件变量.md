##Linux条件变量

特点：
a.条件必须被Mutex保护起来

头文件：#include <pthread.h>

####条件变量的初始化
可以用宏PTHREAD_COND_INITIALIZER来静态的初始化条件变量，条件变量是pthread_cond_t的结构体，例：
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
也可以用pthread_cond_init函数动态的创建，函数原型如下：
int pthread_cond_init(pthread_cond_t *cond, const pthread_condxattr_t *attr);
其中cond是一个指向结构pthread_cond_t的指针，attr是一个指向结构pthread_condxattr_t的指针。结构pthread_condxattr_t是条件变量的属性结构，和互斥锁一样我们可以用它来设置条件变量是进程内可用还是进程间可用，默认值是PTHREAD_PROCESS_PRIVATE，即此条件变量被同一进程内的各个线程使用。注意初始化条件变量只有未被使用时才能重新初始化或被释放。
例：
/* 用默认属性初始化一个条件变量对象*/
pthread_cond_init(&cond, NULL);

####条件变量的释放
int pthread_cond_destroy(pthread_cond_t *cond);

####条件变量的等待
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *timeout);
pthread_cond_wait函数用于等待条件发生(=true)。pthread_cond_timedwait类似，只是当等待超时的时候返回一个错误值ETIMEDOUT。超时的时间用timespec结构指定。此外，两个函数都需要传入一个mutex用于保护条件
timespec结构定义如下：
struct timespec {
    time_t tv_sec;       /* seconds */
    long   tv_nsec;      /* nanoseconds */
};
注意timespec的时间是绝对时间而非相对时间，因此需要先调用gettimeofday函数获得当前时间，再转换成timespec结构，加上偏移量。

####条件变量的通知
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
这两个函数用于通知线程条件被满足(=true)。两者的区别是前者会唤醒单个线程，而后者会唤醒所有线程。

【总结】
需要注意的是，条件变量只是起阻塞和唤醒线程的作用，具体的判断条件还需用户给出，例如一个变量是否为0等。
