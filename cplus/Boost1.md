###多线程

####概述

####线程管理[一般]
在这个库最重要的一个类就是boost::thread，它是在boost/thread.hpp里定义的，用来创建一个新线程。下面的示例来说明如何运用它。

	#include <boost/thread.hpp>
    #include <iostream>

    void wait(int seconds) 
    {  
      boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
    } 

    void thread() 
	{ 
	  for (int i = 0; i < 5; ++i) 
	  { 
	    wait(1); 
	    std::cout << i << std::endl; 
	  } 
	} 
	
	int main() 
	{ 
	  boost::thread t(thread); 
	  t.join(); 
	}

新建线程里执行的那个函数的名称被传递到boost::thread的构造函数。一旦上述示例中的变量t被创建，该thread()函数就在其所在线程中被立即执行。同时在main()里也并发地执行该thread()。

正如在上面的例子中看到，一个特定的线程可以通过诸如t的变量访问，通过这个变量等待着它的使用join()方法终止。但是，即使t越界或者析构了，该线程也将继续执行。一个线程总是在一开始就绑定到一个类型为boost::thread的变量，但是一旦创建，就不在取决于它。

任何一个函数内可以做的事情也可以在一个线程内完成。归根结底，一个线程只不过是一个函数，除了它是同时执行的。在上述例子中，使用一个循环把5个数字写入标准输出流。为了减缓输出，每一个循环中调用wait()函数让执行延迟了一秒。wait()可以调用一个名为sleep()的函数，这个函数也来自于Boost.Thread，位于boost::this_thread命名空间内。

sleep()要么在预计的一段时间或一个特定的时间点后时才让线程继续执行。通过传递一个类型为boost::posix_time::seconds的对象，在这个例子里我们指定了一段时间。boost::posix_time::seconds来自于Boost.DateTime库，它被Boost.Thread用来管理和处理时间的数据。

####同步[较重要]
虽然多线程的使用可以提高应用程序的性能，但也增加了复杂性。如果使用线程在同一时间执行几个函数，访问共享资源时必须相应地同步。一旦应用达到了一定规模，这涉及相当一些工作。本段介绍了Boost.Thread提供同步线程的类。

    #include <boost/thread.hpp> 
    #include <iostream> 

    void wait(int seconds) 
    { 
      boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
    } 

    boost::mutex mutex; 

    void thread() 
    { 
      for (int i = 0; i < 5; ++i) 
      { 
        wait(1); 
        mutex.lock(); 
        std::cout << "Thread " << boost::this_thread::get_id() << ": " << i << std::endl; 
        mutex.unlock(); 
      } 
    } 

    int main() 
    { 
      boost::thread t1(thread); 
      boost::thread t2(thread); 
      t1.join(); 
      t2.join(); 
    }

多线程程序使用所谓的互斥对象来同步。Boost.Thread提供多个的互斥类，boost::mutex是最简单的一个。互斥的基本原则是当一个特定的线程拥有资源的时候防止其他线程夺取其所有权。一旦释放，其他的线程可以取得所有权。这将导致线程等待至另一个线程完成处理一些操作，从而相应地释放互斥对象的所有权。

上面的示例使用一个类型为boost::mutex的mutex全局互斥对象。thread()函数获取此对象的所有权才在for循环内使用lock()方法写入到标准输出流的。一旦信息被写入，使用unlock()方法释放所有权。

main()创建两个线程，同时执行thread()函数。利用for循环，每个线程数到5，用一个迭代器写一条消息到标准输出流。不幸的是，标准输出流是一个全局性的被所有线程共享的对象。 该标准不提供任何保证std::cout可以安全地从多个线程访问。因此，访问标准输出流必须同步：在任何时候，只有一个线程可以访问std::cout。

由于两个线程试图在写入标准输出流前获得互斥体，实际上只能保证一次只有一个线程访问std::cout。不管哪个线程成功调用lock()方法，其他所有线程必须等待，直到unlock()被调用。

获取和释放互斥体是一个典型的模式，是由Boost.Thread通过不同的数据类型支持。例如，不直接地调用lock()和unlock()，使用boost::lock_guard类也是可以的。

	#include <boost/thread.hpp> 
	#include <iostream> 
	
	void wait(int seconds) 
	{ 
	  boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
	} 
	
	boost::mutex mutex; 
	
	void thread() 
	{ 
	  for (int i = 0; i < 5; ++i) 
	  { 
	    wait(1); 
	    boost::lock_guard<boost::mutex> lock(mutex); 
	    std::cout << "Thread " << boost::this_thread::get_id() << ": " << i << std::endl; 
	  } 
	} 
	
	int main() 
	{ 
	  boost::thread t1(thread); 
	  boost::thread t2(thread); 
	  t1.join(); 
	  t2.join(); 
	} 

boost::lock_guard在其内部构造和析构函数分别自动调用lock()和unlock()。访问共享资源是需要同步的，因为它显式地被两个方法调用。boost::lock_guard类是另一个出现在第2章智能指针的RAII用语。

除了boost::mutex和boost::lock_guard之外，Boost.Thread也提供其他的类支持各种同步。其中一个重要的就是boost::unique_lock，相比较boost::lock_guard而言，它提供许多有用的方法。

	#include <boost/thread.hpp> 
	#include <iostream> 
	
	void wait(int seconds) 
	{ 
	  boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
	} 
	
	boost::timed_mutex mutex; 
	
	void thread() 
	{ 
	  for (int i = 0; i < 5; ++i) 
	  { 
	    wait(1); 
	    boost::unique_lock<boost::timed_mutex> lock(mutex, boost::try_to_lock); 
	    if (!lock.owns_lock()) 
	      lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(1)); 
	    std::cout << "Thread " << boost::this_thread::get_id() << ": " << i << std::endl; 
	    boost::timed_mutex *m = lock.release(); 
	    m->unlock(); 
	  } 
	} 
	
	int main() 
	{ 
	  boost::thread t1(thread); 
	  boost::thread t2(thread); 
	  t1.join(); 
	  t2.join(); 
	} 

boost::unique_lock通过多个构造函数来提供不同的方式获得互斥体。这个期望获得互斥体的函数简单地调用了lock()方法，一直等到获得这个互斥体。所以它的行为跟boost::lock_guard的那个是一样的。

如果第二个参数传入一个boost::try_to_lock类型的值，对应的构造函数就会调用try_lock()方法。这个方法返回bool型的值：如果能够获得互斥体则返回true，否则返回false。相比lock()函数，try_lock()会立即返回，而且在获得互斥体之前不会被阻塞。

上面的程序向boost::unique_lock的构造函数的第二个参数传入boost::try_to_lock。然后通过owns_lock()可以检查是否可获得互斥体。如果不能，owns_lock()返回false。这也用到boost::unique_lock提供的另外一个函数：timed_lock()等待一定的时间以获得互斥体。给定的程序等待长达1秒，应较足够的时间来获取更多的互斥。

其实这个例子显示了三个方法获取一个互斥体：lock()会一直等待，直到获得一个互斥体。try_lock()则不会等待，但它只会在互斥体可用的时候才能获得，否则返回false。最后，timed_lock()试图获得在一定的时间内获取互斥体。和try_lock()一样，返回bool类型的值意味着成功与否。

虽然boost::mutex提供了lock()和try_lock()两个方法，但是boost::timed_mutex只支持timed_lock()，这就是上面示例那么使用的原因。如果不用timed_lock()的话，也可以像以前的例子那样用boost::mutex。

就像boost::lock_guard一样，boost::unique_lock的析构函数也会相应地释放互斥量。此外，可以手动地用unlock()释放互斥量。也可以像上面的例子那样，通过调用release()解除boost::unique_lock和互斥量之间的关联。然而在这种情况下，必须显式地调用unlock()方法来释放互斥量，因为boost::unique_lock的析构函数不再做这件事情。

boost::unique_lock这个所谓的独占锁意味着一个互斥量同时只能被一个线程获取。其他线程必须等待，直到互斥体再次被释放。除了独占锁，还有非独占锁。Boost.Thread里有个boost::shared_lock的类提供了非独占锁。正如下面的例子，这个类必须和boost::shared_mutex型的互斥量一起使用。

    #include <boost/thread.hpp> 
    #include <iostream> 
    #include <vector> 
    #include <cstdlib> 
    #include <ctime> 
    
    void wait(int seconds) 
    { 
      boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
    } 
    
    boost::shared_mutex mutex; 
    std::vector<int> random_numbers; 
    
    void fill() 
    { 
      std::srand(static_cast<unsigned int>(std::time(0))); 
      for (int i = 0; i < 3; ++i) 
      { 
        boost::unique_lock<boost::shared_mutex> lock(mutex); 
        random_numbers.push_back(std::rand()); 
        lock.unlock(); 
        wait(1); 
      } 
    } 
    
    void print() 
    { 
      for (int i = 0; i < 3; ++i) 
      { 
        wait(1); 
        boost::shared_lock<boost::shared_mutex> lock(mutex); 
        std::cout << random_numbers.back() << std::endl; 
      } 
    } 
    
    int sum = 0; 
    
    void count() 
    { 
      for (int i = 0; i < 3; ++i) 
      { 
        wait(1); 
        boost::shared_lock<boost::shared_mutex> lock(mutex); 
        sum += random_numbers.back(); 
      } 
    } 
    
    int main() 
    { 
      boost::thread t1(fill); 
      boost::thread t2(print); 
      boost::thread t3(count); 
      t1.join(); 
      t2.join(); 
      t3.join(); 
      std::cout << "Sum: " << sum << std::endl; 
    }
boost::shared_lock类型的非独占锁可以在线程只对某个资源读访问的情况下使用。一个线程修改的资源需要写访问，因此需要一个独占锁。这样做也很明显：只需要读访问的线程不需要知道同一时间其他线程是否访问。因此非独占锁可以共享一个互斥体。

在给定的例子，print()和count()都可以只读访问random_numbers。虽然print()函数把random_numbers里的最后一个数写到标准输出，count()函数把它统计到sum变量。由于没有函数修改random_numbers，所有的都可以在同一时间用boost::shared_lock类型的非独占锁访问它。

在fill()函数里，需要用一个boost::unique_lock类型的非独占锁，因为它插入了一个新的随机数到random_numbers。在unlock()显式地调用unlock()来释放互斥量之后，fill()等待了一秒。相比于之前的那个样子，在for循环的尾部调用wait()以保证容器里至少存在一个随机数，可以被print()或者count()访问。对应地，这两个函数在for循环的开始调用了wait()。

考虑到在不同的地方每个单独地调用wait()，一个潜在的问题变得很明显:函数调用的顺序直接受CPU执行每个独立进程的顺序决定。利用所谓的条件变量，可以同步哪些独立的线程，使数组的每个元素都被不同的线程立即添加到random_numbers。

    #include <boost/thread.hpp> 
    #include <iostream> 
    #include <vector> 
    #include <cstdlib> 
    #include <ctime> 
    
    boost::mutex mutex; 
    boost::condition_variable_any cond; 
    std::vector<int> random_numbers; 
    
    void fill() 
    { 
      std::srand(static_cast<unsigned int>(std::time(0))); 
      for (int i = 0; i < 3; ++i) 
      { 
        boost::unique_lock<boost::mutex> lock(mutex); 
        random_numbers.push_back(std::rand()); 
        cond.notify_all(); 
        cond.wait(mutex); 
      } 
    } 
    
    void print() 
    { 
      std::size_t next_size = 1; 
      for (int i = 0; i < 3; ++i) 
      { 
        boost::unique_lock<boost::mutex> lock(mutex); 
        while (random_numbers.size() != next_size) 
          cond.wait(mutex); 
        std::cout << random_numbers.back() << std::endl; 
        ++next_size; 
        cond.notify_all(); 
      } 
    } 
    
    int main() 
    { 
      boost::thread t1(fill); 
      boost::thread t2(print); 
      t1.join(); 
      t2.join(); 
    } 

这个例子的程序删除了wait()和count()。线程不用在每个循环迭代中等待一秒，而是尽可能快地执行。此外，没有计算总额；数字完全写入标准输出流。

为确保正确地处理随机数，需要一个允许检查多个线程之间特定条件的条件变量来同步不每个独立的线程。

正如上面所说，fill()函数用在每个迭代产生一个随机数，然后放在random_numbers容器中。为了防止其他线程同时访问这个容器，就要相应得使用一个排它锁。不是等待一秒，实际上这个例子却用了一个条件变量。调用notify_all()会唤醒每个那些正在分别通过调用wait()等待此通知的线程。

通过查看print()函数里的for循环，可以看到相同的条件变量被wait()函数调用了。如果这个线程被notify_all()唤醒，它就会试图这个互斥量，但只有在fill()函数完全释放之后才能成功。

这里的窍门就是调用wait()会释放相应的被参数传入的互斥量。在调用notify_all()后，fill()函数会通过wait()相应地释放线程。然后它会阻止和等待其他的线程调用notify_all()，一旦随机数已写入标准输出流，这就会在print()里发生。

注意到在print()函数里调用wait()事实上发生在一个单独while循环里。这样做的目的是为了处理在print()函数里第一次调用wait()函数之前随机数已经放到容器里。通过比较random_numbers里元素的数目与预期值，发现这成功地处理了把随机数写入到标准输出流。







