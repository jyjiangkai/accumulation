##C++ 用模板来展示new与delete操作符原理
C++中的new与delete可以认为是C中的malloc与free的升级版本。
new包含两部分，一部分是与malloc功能相同，是从堆上面申请内存块，第二部分是调用类的构造方法来初始化刚申请的内存。
delete是new的逆过程，先调用类的析构方法来反初始化，再把刚申请的内存还给堆。
new[]与delete[]是针对数组操作符，要注意是通过new[]分配的对象，不能用delete来释放对象，否则会有内存泄漏，当然通过new分配的对象，不能用delete[]来释放对象，后面我会通过代码来说明为什么。
下面是C++中的new与delete函数原型，申请内存失败会抛出异常bad_alloc
void* operator new(std::size_t) thown (std::bad_alloc);
void* operator new[](std::size_t) thown (std::bad_alloc);
void* operator delete(void*) thown ();
void* operator delete[](void*) thown ();

使用举例
int* p1 = new int;
delete p2;

int* p2 = new int[5];
delete []p2;
终于到了用模板来模拟new与delete操作符，代码中有注释说明，其中对于调用类的构造方法，采用一种C++标准中称作in-place construtor的方式。使用原型为T* = new(pbuff) T()，直译的话就是在pbuff这块内存构造T类，而不用再去堆上面申请内存，这种技巧大量应用在对象池的实现中，即pbuff这块内存可以挂在链表中反复的使用。


