Linux多线程Pthread学习小结

简介
POSIX thread 简称为pthread，Posix线程是一个POSIX标准线程.该标准定义内部API创建和操纵线程.
 
作用
线程库实行了POSIX线程标准通常称为pthreads.pthreads是最常用的POSIX系统如Linux和Unix,而微软Windowsimplementations同时存在.举例来说,pthreads-w32可支持MIDP的pthread 　　
Pthreads定义了一套 C程序语言类型、函数与常量，它以 pthread.h 头文件和一个线程库实现。
 
数据类型
pthread_t：线程句柄 　　
pthread_attr_t：线程属性
线程操纵函数（简介起见，省略参数）
pthread_create()：创建一个线程 　　
pthread_exit()：终止当前线程 　　
pthread_cancel()：中断另外一个线程的运行 　　
pthread_join()：阻塞当前的线程，直到另外一个线程运行结束 　　
pthread_attr_init()：初始化线程的属性 　　
pthread_attr_setdetachstate()：设置脱离状态的属性（决定这个线程在终止时是否可以被结合）
pthread_attr_getdetachstate()：获取脱离状态的属性 　　
pthread_attr_destroy()：删除线程的属性 　　
pthread_kill()：向线程发送一个信号
 
同步函数
用于 mutex 和条件变量 　　
pthread_mutex_init() 初始化互斥锁 　　
pthread_mutex_destroy() 删除互斥锁 　　
pthread_mutex_lock()：占有互斥锁（阻塞操作） 　　
pthread_mutex_trylock()：试图占有互斥锁（不阻塞操作）。当互斥锁空闲时将占有该锁；否则立即返回　　
pthread_mutex_unlock(): 释放互斥锁 　　
pthread_cond_init()：初始化条件变量 　　
pthread_cond_destroy()：销毁条件变量 　　
pthread_cond_wait(): 等待条件变量的特殊条件发生
pthread_cond_signal(): 唤醒第一个调用pthread_cond_wait()而进入睡眠的线程 　　 　　
Thread-local storage（或者以Pthreads术语，称作 线程特有数据）: 　　
pthread_key_create(): 分配用于标识进程中线程特定数据的键 　　
pthread_setspecific(): 为指定线程特定数据键设置线程特定绑定 　　
pthread_getspecific(): 获取调用线程的键绑定，并将该绑定存储在 value 指向的位置中 　　
pthread_key_delete(): 销毁现有线程特定数据键
 
与一起工作的工具函数
pthread_equal(): 对两个线程的线程标识号进行比较 　　
pthread_detach(): 分离线程 　　
pthread_self(): 查询线程自身线程标识号
 
详细请参见：
linux多线程pthread：     http://blog.csdn.net/Sunboy_2050/archive/2010/10/04/5920936.aspx 
Pthread多线程学习小结： http://blog.csdn.net/Sunboy_2050/archive/2010/10/04/5921003.aspx
===================================================================
 
多线程创建
参考代码：
[cpp] view plain copy print?
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
    printf("%s pid: %u tid: %u (0x%x)/n",  
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
        printf("create thread error: %s/n",strerror(err));  
        return 1;  
    }  
    printf("main thread: pid: %u tid: %u (0x%x)/n",   
                (unsigned int)getpid(),  
                (unsigned int)pthread_self(),  
                (unsigned int)pthread_self());  
    print_ids("main thread:");  
    sleep(1);  
    return 0;  
}  
 
运行结果：
[work@db-testing-com06-vm3.db01.baidu.com pthread]$ gcc -Wall -o pthread_create pthread_create.c -lpthread   
[work@db-testing-com06-vm3.db01.baidu.com pthread]$ ./pthread_create 
main thread: pid: 12531 tid: 2505487232 (0x9556b380)
main thread: pid: 12531 tid: 2505487232 (0x9556b380)
new  thread: pid: 12531 tid: 1084229984 (0x40a00960)
 
===================================================================
 
多线程条件变量
参考代码：
[cpp] view plain copy print?
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
    printf("counter: %d/n", counter);  
    pthread_t thd1, thd2;  
    int ret;  
  
    //初始化  
    pthread_mutex_init(&counter_lock, NULL);  
    pthread_cond_init(&counter_nonzero, NULL);  
      
    ret = pthread_create(&thd1, NULL, decrement_counter, NULL); //创建线程1  
    if(ret){  
        perror("del:/n");  
        return 1;  
    }  
  
    ret = pthread_create(&thd2, NULL, increment_counter, NULL); //创建线程2  
    if(ret){  
        perror("inc: /n");  
        return 1;  
    }  
  
    int counter = 0;  
    while(counter != 10){  
        printf("counter(main): %d/n", counter); //主线程  
        sleep(1);  
        counter++;  
    }  
  
    pthread_exit(0);  
      
    return 0;  
}  
  
void *decrement_counter(void *argv)  
{  
    printf("counter(decrement): %d/n", counter);  
    pthread_mutex_lock(&counter_lock);  
    while(counter == 0)  
        pthread_cond_wait(&counter_nonzero, &counter_lock); //进入阻塞(wait)，等待激活(signal)  
      
    printf("counter--(before): %d/n", counter);      
    counter--; //等待signal激活后再执行  
    printf("counter--(after): %d/n", counter);      
    pthread_mutex_unlock(&counter_lock);   
  
    return &estatus;  
}  
  
void *increment_counter(void *argv)  
{  
    printf("counter(increment): %d/n", counter);  
    pthread_mutex_lock(&counter_lock);  
    if(counter == 0)  
        pthread_cond_signal(&counter_nonzero); //激活(signal)阻塞(wait)的线程(先执行完signal线程，然后再执行wait线程)  
  
    printf("counter++(before): %d/n", counter);      
    counter++;   
    printf("counter++(after): %d/n", counter);      
    pthread_mutex_unlock(&counter_lock);  
  
    return &estatus;  
}  

运行结果：
[work@db-testing-com06-vm3.db01.baidu.com pthread]$ gcc -Wall -o pthread_cond2 pthread_cond2.c -lpthread
[work@db-testing-com06-vm3.db01.baidu.com pthread]$ ./pthread_cond2 
counter: 0
counter(main): 0
counter(decrement): 0
counter(increment): 0
counter++(before): 0
counter++(after): 1
counter--(before): 1
counter--(after): 0
counter(main): 1
counter(main): 2
counter(main): 3
counter(main): 4
counter(main): 5
counter(main): 6
counter(main): 7
counter(main): 8
counter(main): 9
 
详细解释，请见：http://blog.csdn.net/Sunboy_2050/archive/2010/11/24/6031723.aspx
===================================================================
 
多线程的创建特殊数据键
参考代码：
[cpp] view plain copy print?
#include <stdio.h>  
#include <pthread.h>  
#include <unistd.h>  
  
pthread_key_t key; //声明参数key  
  
void echomsg(void *arg) //析构处理函数  
{  
    printf("destruct executed in thread = %u, arg = %p/n",   
                (unsigned int)pthread_self(),  
                arg);     
}  
  
void *child_1(void *arg)  
{  
    pthread_t tid;  
     
    tid = pthread_self();  
    printf("%s: thread %u enter/n", (char *)arg, (unsigned int)tid);  
      
    pthread_setspecific(key, (void *)tid);  // 与key值绑定的value(tid)  
    printf("%s: thread %u returns %p/n",    // %p 表示输出指针格式   
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
    printf("%s: thread %u enter/n", (char *)arg, (unsigned int)tid);  
      
    pthread_setspecific(key, (void *)tid);  
    printf("%s: thread %u returns %p/n",   
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
      
    printf("hello main/n");  
      
    pthread_key_create(&key, echomsg); //创建key  
      
    pthread_create(&tid1, NULL, child_1, (void *)"child_1"); //创建带参数的线程，需要强制转换  
    pthread_create(&tid2, NULL, child_2, (void *)"child_2");  
  
    sleep(3);  
    pthread_key_delete(key); //清除key  
    printf("bye main/n");  
      
    pthread_exit(0);  
    return 0;  
}  
运行结果：
[work@db-testing-com06-vm3.db01.baidu.com pthread]$ gcc -Wall -o pthread_setspecific pthread_setspecific.c -lpthread
[work@db-testing-com06-vm3.db01.baidu.com pthread]$ ./pthread_setspecific                                           
hello main
child_1: thread 1084229984 enter
child_1: thread 1084229984 returns 0x40a00960
child_2: thread 1094719840 enter
child_2: thread 1094719840 returns 0x41401960
destruct executed in thread = 1084229984, arg = 0x40a00960
destruct executed in thread = 1094719840, arg = 0x41401960
bye main
 
附加参考——函数原型：
Posix定义了两个API分别用来创建和注销TSD：
int pthread_key_create(pthread_key_t *key, void (*destr_function) (void *))
注销一个TSD采用如下API：
int pthread_key_delete(pthread_key_t key)
int pthread_setspecific(pthread_key_t key, const void *pointer)
void * pthread_getspecific(pthread_key_t key)
参考网址：
http://blog.sina.com.cn/s/blog_46d528490100lxm0.html
http://xunet.blog.51cto.com/138167/22011（推荐）
 
===================================================================
多线程的创建特殊数据键
参考代码：
[cpp] view plain copy print?
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

运行结果：
work@db-testing-com06-vm3.db01.baidu.com pthread]$ gcc -Wall -o pthread_once pthread_once.c -lpthread
[work@db-testing-com06-vm3.db01.baidu.com pthread]$ ./pthread_once                                    
hello main
Func: once_run in thread: 1084229984
child_1: thread 1084229984 returns
child_2: thread 1094719840 returns
bye main