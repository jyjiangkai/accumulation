##Linux原子操作(atomic)
所谓原子操作，就是该操作绝不会在执行完毕前被任何其他任务或事件打断，也就说，它的最小的执行单位。

原子操作主要用于实现资源计数，很多引用计数(refcnt)就是通过原子操作实现的。

####原子类型定义
typedef struct
{
    volatile int counter;
}atomic_t;
volatile修饰字段告诉gcc不要对该类型的数据做优化处理，对它的访问都是对内存的访问，而不是对寄存器的访问。

####原子操作API
atomic_read(atomic_t* v);
该函数对原子类型的变量进行原子读操作，它返回原子类型的变量v的值。

atomic_set(atomic_t* v, int i);
该函数设置原子类型的变量v的值为i。

voidatomic_add(int i, atomic_t *v);
该函数给原子类型的变量v增加值i。
atomic_sub(inti, atomic_t *v);

该函数从原子类型的变量v中减去i。
intatomic_sub_and_test(int i, atomic_t *v);

该函数从原子类型的变量v中减去i，并判断结果是否为0，如果为0，返回真，否则返回假。
voidatomic_inc(atomic_t *v);

该函数对原子类型变量v原子地增加1。
voidatomic_dec(atomic_t *v);

该函数对原子类型的变量v原子地减1。
intatomic_dec_and_test(atomic_t *v);

该函数对原子类型的变量v原子地减1，并判断结果是否为0，如果为0，返回真，否则返回假。
intatomic_inc_and_test(atomic_t *v);

该函数对原子类型的变量v原子地增加1，并判断结果是否为0，如果为0，返回真，否则返回假。
intatomic_add_negative(int i, atomic_t*v);

该函数对原子类型的变量v原子地增加I，并判断结果是否为负数，如果是，返回真，否则返回假。
intatomic_add_return(int i, atomic_t *v);

该函数对原子类型的变量v原子地增加i，并且返回指向v的指针。
intatomic_sub_return(int i, atomic_t *v);

该函数从原子类型的变量v中减去i，并且返回指向v的指针。

intatomic_inc_return(atomic_t * v);

该函数对原子类型的变量v原子地增加1并且返回指向v的指针。

intatomic_dec_return(atomic_t * v);

该函数对原子类型的变量v原子地减1并且返回指向v的指针。

原子操作通常用于实现资源的引用计数，在TCP/IP协议栈的IP碎片处理中，就使用了引用计数，碎片队列结构structipq描述了一个IP碎片，字段refcnt就是引用计数器，它的类型为atomic_t，当创建IP碎片时（在函数ip_frag_create中），使用atomic_set函数把它设置为1，当引用该IP碎片时，就使用函数atomic_inc把引用计数加1，当不需要引用该IP碎片时，就使用函数ipq_put来释放该IP碎片，ipq_put使用函数atomic_dec_and_test把引用计数减1并判断引用计数是否为0，如果是就释放Ip碎片。函数ipq_kill把IP碎片从ipq队列中删除，并把该删除的IP碎片的引用计数减1（通过使用函数atomic_dec实现）。