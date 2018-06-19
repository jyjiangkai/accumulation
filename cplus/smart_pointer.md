##智能指针
###什么是智能指针
众所周知，C++的内存申请与释放机制与GO和JAVA等语言不同，它是完全交给程序员的。
而内存由谁释放，几时释放的问题常常困扰着C++程序员。智能指针就是为了解决这个问题而产生的。
智能指针(smart pointer)是存储指向动态分配(堆)对象指针的类，用于生存期控制，能够确保自动正确的销毁动态分配的对象，防止内存泄漏。
STL一共给我们提供了四种智能指针：auto_ptr、unique_ptr、shared_ptr和weak_ptr。
本文主要介绍常用的shared_ptr和weak_ptr。

###shared_ptr介绍
####什么是shared_ptr
shared_ptr的一种通用实现技术是引用技术(reference count)。智能指针类将一个计数器与类指向的对象相关联，引用计数器跟踪该类有多少个对象共享同一指针。
####shared_ptr的实现
