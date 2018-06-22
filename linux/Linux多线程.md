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
|Process Primitive	|Thread Primitive		|Description
|fork		 		|pthread_create			|创建新的控制流
|exit				|pthread_exit			|退出已有的控制流
|waitpid			|pthread_join			|等待控制流并获得结束代码
|atexit				|pthread_cleanup_push	|注册在控制流退出时候被调用的函数
|getpid				|pthread_self			|获得控制流的id
|abort				|pthread_cancel			|请求非正常退出


在传统的Unix模型中，当一个进程需要由另一个实体执行某件事时，该进程派生(fork)一个子进程，让子进程去进行处理。Unix下的大多数网络服务器程序都是这么编写的，即父进程接受连接，派生子进程，子进程处理与客户的交互。虽然这种模型很多年来使用得很好，但是fork时有一些问题：
    1.fork是昂贵的。内存映像要从父进程拷贝到子进程，所有描述字要在子进程中复制等等。目前有的Unix实现使用一种叫做写时拷贝(copy-on-write)的技术，可避免父进程数据空间向子进程的拷贝。尽管有这种优化技术，fork仍然是昂贵的。
    2.fork子进程后，需要用进程间通信(IPC)在父子进程之间传递信息。Fork之前的信息容易传递，因为子进程从一开始就有父进程数据空间及所有描述字的拷贝。但是从子进程返回信息给父进程需要做更多的工作。

线程有助于解决这两个问题。线程有时被称为轻权进程(lightweight process)，因为线程比进程“轻权”，一般来说，创建一个线程要比创建一个进程快10～100倍。
一个进程中的所有线程共享相同的全局内存，这使得线程很容易共享信息，但是这种简易性也带来了同步问题。

一个进程中的所有线程不仅共享全局变量，而且共享：进程指令、大多数数据、打开的文件（如描述字）、信号处理程序和信号处置、当前工作目录、用户ID和组ID。但是每个线程有自己的线程ID、寄存器集合（包括程序计数器和栈指针）、栈（用于存放局部变量和返回地址）、error、信号掩码、优先级。









 

 


