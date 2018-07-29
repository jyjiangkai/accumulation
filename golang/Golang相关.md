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





