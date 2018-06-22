##Linux线程-互斥锁
假设各个线程向同一个文件顺序写入数据，最后得到的结果是不可想象的。所以用互斥锁来保证一段时间内只有一个线程在执行一段代码。
所在头文件：#include <pthread.h>
####锁的初始化
可以用宏PTHREAD_MUTEX_INITIALIZER来静态的初始化锁，互斥锁是pthread_mutex_t的结构体，例：
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
也可以用pthread_mutex_init函数动态的创建，函数原型如下：
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
例：
/* 用默认属性初始化一个互斥锁对象*/
pthread_mutex_init(&mutex, NULL);

####锁的加锁
int pthread_mutex_lock(pthread_mutex_t *mutex);

####锁的解锁
int pthread_mutex_unlock(pthread_mutex_t *mutex);

####锁的释放
调用pthread_mutex_destory之后，可以释放锁占用的资源，前提是锁当前处于解锁状态。
int pthread_mutex_destroy(pthread_mutex_t *mutex);

####锁的属性
互斥锁属性可以由pthread_mutexattr_init(pthread_mutexattr_t *mattr)来初始化，然后可以调用其他的属性设置方法来设置其属性

####锁的范围
可以指定进程与其他进程同步还是同一进程内不同的线程之间同步。可以设置为PTHREAD_PROCESS_SHARE和PTHREAD_PROCESS_PRIVATE。默认是后者，表示进程内使用锁。
int pthread_mutexattr_setpshared(pthread_mutexattr_t *mattr, int pshared);
pthread_mutexattr_getshared(pthread_mutexattr_t *mattr, int *pshared);

####锁的类型
PTHREAD_MUTEX_NORMAL：
*这种类型的互斥锁不会自动检测死锁。
*如果一个线程试图对一个互斥锁重复锁定，将会引起这个线程的死锁。
*如果试图解锁一个由别的线程锁定的互斥锁会引发不可预料的结果。
*如果一个线程试图解锁已经被解锁的互斥锁也会引发不可预料的结果。
   
PTHREAD_MUTEX_ERRORCHECK：
*这种类型的互斥锁会自动检测死锁。
*如果一个线程试图对一个互斥锁重复锁定，将会返回一个错误代码。
*如果试图解锁一个由别的线程锁定的互斥锁将会返回一个错误代码。
*如果一个线程试图解锁已经被解锁的互斥锁也将会返回一个错误代码。
   
PTHREAD_MUTEX_RECURSIVE：
*如果一个线程对这种类型的互斥锁重复上锁，不会引起死锁。
*一个线程对这类互斥锁的多次重复上锁必须由这个线程来重复相同数量的解锁，这样才能解开这个互斥锁，别的线程才能得到这个互斥锁。
*如果试图解锁一个由别的线程锁定的互斥锁将会返回一个错误代码。
*如果一个线程试图解锁已经被解锁的互斥锁也将会返回一个错误代码。
*这种类型的互斥锁只能是进程私有的（作用域属性为PTHREAD_PROCESS_PRIVATE）
   
PTHREAD_MUTEX_DEFAULT(缺省类型)：
*这种类型的互斥锁不会自动检测死锁。
*如果一个线程试图对一个互斥锁重复锁定，将会引起不可预料的结果。
*如果试图解锁一个由别的线程锁定的互斥锁会引发不可预料的结果。
*如果一个线程试图解锁已经被解锁的互斥锁也会引发不可预料的结果。
*POSIX标准规定，对于某一具体的实现，可以把这种类型的互斥锁定义为其他类型的互斥锁。

设置和获取互斥锁的类型：
pthread_mutexattr_settype(pthread_mutexattr_t *attr , int type)
pthread_mutexattr_gettype(pthread_mutexattr_t *attr , int *type)

【总结】
1、在使用互斥锁的过程中很有可能会出现死锁：两个线程试图同时占用两个资源，并按不同的次序锁定相应的互斥锁，例如两个线程都需要锁定互斥锁1和互斥锁2，a线程先锁定互斥锁1，b 线程先锁定互斥锁2，这时就出现了死锁。
2、互斥是两个线程之间不可以同时运行，他们会相互排斥，必须等待一个线程运行完毕，另一个才能运行，而同步也是不能同时运行，但他是必须要安照某种次序来运行相应的线程(也是一种互斥)。
互斥：是指某一资源同时只允许一个访问者对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的。
同步：是指在互斥的基础上（大多数情况），通过其它机制实现访问者对资源的有序访问。在大多数情况下，同步已经实现了互斥，特别是所有写入资源的情况必定是互斥的。少数情况是指可以允许多个访问者同时访问资源。
3、互斥锁特点，一次只能一个线程拥有互斥锁，其他线程只有等待  
