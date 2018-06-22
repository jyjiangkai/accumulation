## C++线程数据应用代码实例
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