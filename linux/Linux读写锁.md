##Linux读写锁

特点：
a.多个读者可以同时进行读  
b.写者必须互斥(只允许一个写者写，也不能读者写者同时进行)
c.写者优先于读者(一旦有写者，则后续读者必须等待，唤醒时优先考虑写者)

头文件：#include <pthread.h>

####锁的初始化
可以用宏PTHREAD_RWLOCK_INITIALIZER来静态的初始化锁，读写锁是pthread_rwlock_t的结构体，例：
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
也可以用pthread_rwlock_init函数动态的创建，函数原型如下：
int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr);
例：
/* 用默认属性初始化一个读写锁对象*/
pthread_rwlock_init(&rwlock, NULL);

####锁的加锁
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
	
####锁的解锁
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

####锁的释放
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
