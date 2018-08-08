##Golang相关知识点总结

####小结1
Golang中的sync.WaitGroup

刚才看见golang的sync包，看见一个很有用的功能。就是WaitGroup。

先说说WaitGroup的用途：它能够一直等到所有的goroutine执行完成，并且阻塞主线程的执行，直到所有的goroutine执行完成。

这里要注意一下，他们的执行结果是没有顺序的，调度器不能保证多个goroutine执行次序，且进程退出时不会等待它们结束。

WaitGroup总共有三个方法：

* Add(delta int):添加或者减少等待的goroutine的数量
* Done():相当于Add(-1)
* Wait():执行阻塞，直到所有的WaitGroup数量变成0

如：golang中的同步是通过sync.WaitGroup来实现的，WaitGroup的功能：它实现了一个类似队列的结构，可以一直向队列中添加任务，当任务完成后便从队列中删除，如果队列总的任务没有完全完成，可以通过Wait()函数来触发阻塞，防止程序继续进行，直到所有的队列任务都完成为止。

WaitGroup的特点是Wait()可以用来阻塞直到队列中的所有任务都完成时才解除阻塞，而不需要sleep一个固定的时间来等待，但是其缺点是无法指定固定的goroutine数目，可能通过使用channel解决此问题。

####小结2
	fmt.Sprintf("%T, %v\n", y, y)    //输出类型和值

####小结3
	fmt.Printf("%b, %#o, %#x\n", a, b, c)    //按二进制、八进制、十六进制输出
	fmt.Println(math.MinInt8, math.MaxInt8)    //输出int8的最小值和最大值

####小结4
golang中左花括号不允许另起一行。

####小结5
字符串拼接使用bytes.Buffer提升性能。

####小结6
并发和并行的区别
* 并发：逻辑上具备同时处理多个任务的能力
* 并行：物理上在同一时刻执行多个并发任务

####小结7
将Mutex作为匿名字段时，相关方法必须实现为pointer-receiver，否则会因复制导致锁机制失效

####小结8
* 对性能要求较高时，应避免使用defer Unlock
* 读写并发时，用RWMutex性能会更好一些
* 对单个数据读写保护，可尝试用原子操作
* 执行严格测试，尽可能打开数据竞争检查

####小结9
通过实际对象获取类型
package main

import (
	"fmt"
	"reflect"
)

type X int
func main(){
	var x X = 100
	t := reflect.TypeOf(x)

	fmt.Println(t.Name(), t.Kind())
}

输出：
X int

####小结10
构造基础符合类型
package main

import (
	"fmt"
	"reflect"
)

func main(){
	a := reflect.ArrayOf(10, reflect.TypeOf(byte(0)))
	m := reflect.MapOf(reflect.TypeOf(""), reflect.TypeOf(0))

	fmt.Println(a, m)
}

输出：
[10]uint8 map[string]int

####小结11
make([]int)
make([]int, 10)
make(map[string]int)
make(map[string]int, 10)
make(chan int)
make(chan int, 10)

####小结12
方法Elem返回指针、数组、切片、字典(值)或通道的基类型
package main

import (
	"fmt"
	"reflect"
)

func main(){
	var v int8 = 1
	
	fmt.Println(reflect.TypeOf(map[string]int{}).Elem())
	fmt.Println(reflect.TypeOf([]int32{}).Elem())
	fmt.Println(reflect.TypeOf(&v).Elem())
	fmt.Println(reflect.TypeOf(make(chan int)).Elem())
}

输出：
int
int32
int8
int

####小结13
内存分配
1.每次从操作系统申请一大块内存，以减少系统调用
2.将申请到的大块内存按照特定大小预先切分成小块，构成链表
3.为对象分配内存时，只须从大小合适的链表提取一个小块即可
4.回收对象内存时，将该小块内存重新归还到原链表，以便复用
5.如闲置内存过多，则尝试归还部分内存给操作系统，降低整体开销

####小结14
内存块
分配器将其管理的内存块分为两种：
* span：由多个地址连续的页组成的大块内存
* object：将span按特定大小切分成多个小块，每个小块可存储一个对象

####小结15
* 释放闲置超过5分钟的span物理内存
* 如果超过2分钟没有垃圾回收，则强制执行
* 将长时间未处理的neipoll结果添加到任务队列
* 向长时间运行的G任务发出抢占调度
* 收回因syscall而长时间阻塞的P































