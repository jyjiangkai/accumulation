##Linux线程变量
在单线程的程序里，有两种基本的数据：全局变量和局部变量。但在多线程程序里，还有第三种数据类型：线程数据(TSD: Thread-Specific Data)。它和全局变量很象，在线程内部，各个函数可以象使用全局变量一样调用它，但它对线程外部的其它线程是不可见的。例如我们常见的变量errno，它返回标准的出错信息。它显然不能是一个局部变量，几乎每个函数都应该可以调用它；但它又不能是一个全局变量，否则在A线程里输出的很可能是B线程的出错信息。要实现诸如此类的变量，我们就必须使用线程数据。我们为每个线程数据创建一个键，它和这个键相关联，在各个线程里，都使用这个键来指代线程数据，但在不同的线程里，这个键代表的数据是不同的，在同一个线程里，它代表同样的数据内容。

####线程变量的声明
pthread_key_t key;
线程变量是pthread_key_t的结构体。

####线程变量的创建
int pthread_key_create(pthread_key_t *key, void (*function)(void*));
第一个参数为指向一个键值的指针，第二个参数指明了一个function函数，如果这个参数不为空，那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块。
这个函数常和函数pthread_once(pthread_once_t *once, void (*function)(void))一起使用，为了让这个键只被创建一次。
函数pthread_once声明一个function函数，第一次调用pthread_once时它执行这个函数，以后的调用将被它忽略。

####线程变量的绑定
int pthread_setspecific (pthread_key_t key, const void *pointer); 
该函数设置一个线程专有数据的值，赋给由pthread_key_create创建的键，调用成功返回值为0，否则返回错误代码。

####线程变量的获取
void *pthread_getspecific (pthread_key_t key);
该函数获得绑定到指定键上的值。调用成功返回给定参数key所对应的数据。如果没有数据绑定到该键，则返回NULL。

####线程变量的删除
int pthread_key_delete(pthread_key_t *key);
该函数用于删除一个由pthread_key_create函数调用创建的键。调用成功返回值为0，否则返回错误代码。

####注意
1、用pthread_setspecific为一个键指定新的线程变量时，必须自己释放原有的线程变量以回收空间。这个过程函数pthread_key_delete用来删除一个键，这个键占用的内存将被释放，但同样要注意的是，它只释放键占用的内存，并不释放该键关联的线程数据所占用的内存资源，而且它也不会触发函数pthread_key_create中定义的destructor函数。线程数据的释放必须在释放键之前完成。
2、pthread_setspecfic和pthread_getspecfic要成对出现，不然一个set的值，第一个get取的值是正确的，第二个就不正确了，如果要2个get就需要2个set,才能取到对应的值。


例：
/* 声明一个键*/
pthread_key_t key;
/* 函数 createWindow */
void create(void)
{
    int *win;
    static pthread_once_t once= PTHREAD_ONCE_INIT;
    /* 调用函数createMyKey，创建键*/
    pthread_once(&once, createMyKey);
    /*win指向一个新建立的窗口*/
    win=new int(1);
    /* 设置win */
    set(win);
    /* 将win值绑定在键key上*/
    pthread_setpecific(key, win);
}

/* 函数createMyKey，创建一个键，并指定了destructor */
void createMyKey(void)
{
    pthread_keycreate(&key, freeKey);
}

/* 函数 freeKey，释放空间*/
void freeKey(int *win)
{
    delete win;
}
