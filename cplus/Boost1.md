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


###异步输入输出

####概述

####I/O服务与I/O对象
使用Boost.Asio进行异步数据处理的应用程序基于两个概念：I/O服务和I/O对象。I/O服务抽象了操作系统的接口，允许第一时间进行异步数据处理，而I/O对象则用于初始化特定的操作。鉴于Boost.Asio只提供了一个名为boost::asio::io_service的类作为I/O服务，它针对所支持的每一个操作系统都分别实现了优化的类，另外库中还包含了针对不同I/O对象的几个类。其中，类boost::asio::ip::tcp::socket用于通过网络发送和接收数据，而类boost::asio::deadline_timer则提供了一个计时器，用于测量某个固定时间点到来或是一段指定的时长过去了。以下第一个例子中就使用了计时器，因为与Asio所提供的其它I/O对象相比较而言，它不需要任何有关于网络编程的知识。

    #include <boost/asio.hpp> 
    #include <iostream> 
    
    void handler(const boost::system::error_code &ec) 
    { 
      std::cout << "5 s." << std::endl; 
    } 
    
    int main() 
    { 
      boost::asio::io_service io_service; 
      boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(5)); 
      timer.async_wait(handler); 
      io_service.run(); 
    } 
函数main()首先定义了一个I/O服务io_service，用于初始化I/O对象timer。就像boost::asio::deadline_timer那样，所有I/O对象通常都需要一个I/O服务作为它们的构造函数的第一个参数。由于timer的作用类似于一个闹钟，所以boost::asio::deadline_timer的构造函数可以传入第二个参数，用于表示在某个时间点或是在某段时长之后闹钟停止。 以上例子指定了五秒的时长，该闹钟在timer被定义之后立即开始计时。

虽然我们可以调用一个在五秒后返回的函数，但是通过调用方法async_wait()并传入handler()函数的名字作为唯一参数，可以让Asio启动一个异步操作。请留意，我们只是传入了handler()函数的名字，而该函数本身并没有被调用。

async_wait()的好处是，该函数调用会立即返回，而不是等待五秒钟。一旦闹钟时间到，作为参数所提供的函数就会被相应调用。因此，应用程序可以在调用了async_wait()之后执行其它操作，而不是阻塞在这里。

像async_wait()这样的方法被称为是非阻塞式的。I/O对象通常还提供了阻塞式的方法，可以让执行流在特定操作完成之前保持阻塞。例如，可以调用阻塞式的wait()方法，取代boost::asio::deadline_timer的调用。由于它会阻塞调用，所以它不需要传入一个函数名，而是在指定时间点或指定时长之后返回。

再看看上面的源代码，可以留意到在调用async_wait()之后，又在I/O服务之上调用了一个名为run()的方法。这是必须的，因为控制权必须被操作系统接管，才能在五秒之后调用handler()函数。

async_wait()会启动一个异步操作并立即返回，而run()则是阻塞的。因此调用run()后程序执行会停止。具有讽刺意味的是，许多操作系统只是通过阻塞函数来支持异步操作。以下例子显示了为什么这个限制通常不会成为问题。

    #include <boost/asio.hpp> 
    #include <iostream> 
    
    void handler1(const boost::system::error_code &ec) 
    { 
      std::cout << "5 s." << std::endl; 
    } 
    
    void handler2(const boost::system::error_code &ec) 
    { 
      std::cout << "10 s." << std::endl; 
    } 
    
    int main() 
    { 
      boost::asio::io_service io_service; 
      boost::asio::deadline_timer timer1(io_service, boost::posix_time::seconds(5)); 
      timer1.async_wait(handler1); 
      boost::asio::deadline_timer timer2(io_service, boost::posix_time::seconds(10)); 
      timer2.async_wait(handler2); 
      io_service.run(); 
    } 
上面的程序用了两个boost::asio::deadline_timer类型的I/O对象。第一个I/O对象表示一个五秒后触发的闹钟，而第二个则表示一个十秒后触发的闹钟。每一段指定时长过去后，都会相应地调用函数handler1()和handler2()。

在main()的最后，再次在唯一的I/O服务之上调用了run()方法。如前所述，这个函数将阻塞执行，把控制权交给操作系统以接管异步处理。在操作系统的帮助下，handler1()函数会在五秒后被调用，而handler2()函数则在十秒后被调用。

乍一看，你可能会觉得有些奇怪，为什么异步处理还要调用阻塞式的run()方法。然而，由于应用程序必须防止被中止执行，所以这样做实际上不会有任何问题。如果run()不是阻塞的，main()就会结束从而中止该应用程序。如果应用程序不应被阻塞，那么就应该在一个新的线程内部调用run()，它自然就会仅仅阻塞那个线程。

一旦特定的I/O服务的所有异步操作都完成了，控制权就会返回给run()方法，然后它就会返回。以上两个例子中，应用程序都会在闹钟到时间后马上结束。

####可扩展性与多线程
用Boost.Asio这样的库来开发应用程序，与一般的C++风格不同。那些可能需要较长时间才返回的函数不再是以顺序的方式来调用。不再是调用阻塞式的函数，Boost.Asio是启动一个异步操作。而那些需要在操作结束后调用的函数则实现为相应的句柄。这种方法的缺点是，本来顺序执行的功能变得在物理上分割开来了，从而令相应的代码更难理解。

像Boost.Asio这样的库通常是为了令应用程序具有更高的效率。应用程序不需要等待特定的函数执行完成，而可以在期间执行其它任务，如开始另一个需要较长时间的操作。

可扩展性是指，一个应用程序从新增资源有效地获得好处的能力。如果那些执行时间较长的操作不应该阻塞其它操作的话，那么建议使用Boost.Asio。由于现今的PC机通常都具有多核处理器，所以线程的应用可以进一步提高一个基于Boost.Asio的应用程序的可扩展性。

如果在某个boost::asio::io_service类型的对象之上调用run()方法，则相关联的句柄也会在同一个线程内被执行。通过使用多线程，应用程序可以同时调用多个run()方法。一旦某个异步操作结束，相应的I/O服务就将在这些线程中的某一个之中执行句柄。如果第二个操作在第一个操作之后很快也结束了，则I/O服务可以在另一个线程中执行句柄，而无需等待第一个句柄终止。

    #include <boost/asio.hpp> 
    #include <boost/thread.hpp> 
    #include <iostream> 
    
    void handler1(const boost::system::error_code &ec) 
    { 
      std::cout << "5 s." << std::endl; 
    } 
    
    void handler2(const boost::system::error_code &ec) 
    { 
      std::cout << "5 s." << std::endl; 
    } 
    
    boost::asio::io_service io_service; 
    
    void run() 
    { 
      io_service.run(); 
    } 
    
    int main() 
    { 
      boost::asio::deadline_timer timer1(io_service, boost::posix_time::seconds(5)); 
      timer1.async_wait(handler1); 
      boost::asio::deadline_timer timer2(io_service, boost::posix_time::seconds(5)); 
      timer2.async_wait(handler2); 
      boost::thread thread1(run); 
      boost::thread thread2(run); 
      thread1.join(); 
      thread2.join(); 
    } 
上一节中的例子现在变成了一个多线程的应用。通过使用在boost/thread.hpp中定义的boost::thread类，它来自于Boost C++库Thread，我们在main()中创建了两个线程。这两个线程均针对同一个I/O服务调用了run()方法。这样当异步操作完成时，这个I/O服务就可以使用两个线程去执行句柄函数。

这个例子中的两个计时数均被设为在五秒后触发。由于有两个线程，所以handler1()和handler2()可以同时执行。如果第二个计时器触发时第一个仍在执行，则第二个句柄就会在第二个线程中执行。如果第一个计时器的句柄已经终止，则I/O服务可以自由选择任一线程。

线程可以提高应用程序的性能。因为线程是在处理器内核上执行的，所以创建比内核数更多的线程是没有意义的。这样可以确保每个线程在其自己的内核上执行，而没有同一内核上的其它线程与之竞争。

要注意，使用线程并不总是值得的。以上例子的运行会导致不同信息在标准输出流上混合输出，因为这两个句柄可能会并行运行，访问同一个共享资源：标准输出流std::cout。这种访问必须被同步，以保证每一条信息在另一个线程可以向标准输出流写出另一条信息之前被完全写出。在这种情形下使用线程并不能提供多少好处，如果各个独立句柄不能独立地并行运行。

多次调用同一个I/O服务的run()方法，是为基于Boost.Asio的应用程序增加可扩展性的推荐方法。另外还有一个不同的方法：不要绑定多个线程到单个I/O服务，而是创建多个I/O服务。然后每一个I/O服务使用一个线程。如果I/O服务的数量与系统的处理器内核数量相匹配，则异步操作都可以在各自的内核上执行。

    #include <boost/asio.hpp> 
    #include <boost/thread.hpp> 
    #include <iostream> 
    
    void handler1(const boost::system::error_code &ec) 
    { 
      std::cout << "5 s." << std::endl; 
    } 
    
    void handler2(const boost::system::error_code &ec) 
    { 
      std::cout << "5 s." << std::endl; 
    } 
    
    boost::asio::io_service io_service1; 
    boost::asio::io_service io_service2; 
    
    void run1() 
    { 
      io_service1.run(); 
    } 
    
    void run2() 
    { 
      io_service2.run(); 
    } 
    
    int main() 
    { 
      boost::asio::deadline_timer timer1(io_service1, boost::posix_time::seconds(5)); 
      timer1.async_wait(handler1); 
      boost::asio::deadline_timer timer2(io_service2, boost::posix_time::seconds(5)); 
      timer2.async_wait(handler2); 
      boost::thread thread1(run1); 
      boost::thread thread2(run2); 
      thread1.join(); 
      thread2.join(); 
    } 
前面的那个使用两个计时器的例子被重写为使用两个I/O服务。这个应用程序仍然基于两个线程；但是现在每个线程被绑定至不同的I/O服务。此外，两个I/O对象timer1和timer2现在也被绑定至不同的I/O服务。

这个应用程序的功能与前一个相同。在一定条件下使用多个I/O服务是有好处的，每个I/O服务有自己的线程，最好是运行在各自的处理器内核上，这样每一个异步操作连同它们的句柄就可以局部化执行。如果没有远端的数据或函数需要访问，那么每一个I/O服务就象一个小的自主应用。这里的局部和远端是指像高速缓存、内存页这样的资源。由于在确定优化策略之前需要对底层硬件、操作系统、编译器以及潜在的瓶颈有专门的了解，所以应该仅在清楚这些好处的情况下使用多个I/O服务。

####网络编程
虽然Boost.Asio是一个可以异步处理任何种类数据的库，但是它主要被用于网络编程。这是由于，事实上Boost.Asio在加入其它I/O对象之前很久就已经支持网络功能了。网络功能是异步处理的一个很好的例子，因为通过网络进行数据传输可能会需要较长时间，从而不能直接获得确认或错误条件。

Boost.Asio提供了多个I/O对象以开发网络应用。以下例子使用了boost::asio::ip::tcp::socket类来建立与中另一台PC的连接，并下载'Highscore'主页；就象一个浏览器在指向www.highscore.de时所要做的。

    #include <boost/asio.hpp> 
    #include <boost/array.hpp> 
    #include <iostream> 
    #include <string> 
    
    boost::asio::io_service io_service; 
    boost::asio::ip::tcp::resolver resolver(io_service); 
    boost::asio::ip::tcp::socket sock(io_service); 
    boost::array<char, 4096> buffer; 
    
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) 
    { 
      if (!ec) 
      { 
        std::cout << std::string(buffer.data(), bytes_transferred) << std::endl; 
        sock.async_read_some(boost::asio::buffer(buffer), read_handler); 
      } 
    } 
    
    void connect_handler(const boost::system::error_code &ec) 
    { 
      if (!ec) 
      { 
        boost::asio::write(sock, boost::asio::buffer("GET / HTTP 1.1\r\nHost: highscore.de\r\n\r\n")); 
        sock.async_read_some(boost::asio::buffer(buffer), read_handler); 
      } 
    } 
    
    void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it) 
    { 
      if (!ec) 
      { 
        sock.async_connect(*it, connect_handler); 
      } 
    } 
    
    int main() 
    { 
      boost::asio::ip::tcp::resolver::query query("www.highscore.de", "80"); 
      resolver.async_resolve(query, resolve_handler); 
      io_service.run(); 
    } 
这个程序最明显的部分是三个句柄的使用：connect_handler()和read_handler()函数会分别在连接被建立后以及接收到数据后被调用。那么为什么需要resolve_handler()函数呢？

互联网使用了所谓的IP地址来标识每台PC。IP地址实际上只是一长串数字，难以记住。而记住像www.highscore.de这样的名字就容易得多。为了在互联网上使用类似的名字，需要通过一个叫作域名解析的过程将它们翻译成相应的IP地址。这个过程由所谓的域名解析器来完成，对应的I/O对象是：boost::asio::ip::tcp::resolver。

域名解析也是一个需要连接到互联网的过程。有些专门的PC，被称为DNS服务器，其作用就像是电话本，它知晓哪个IP地址被赋给了哪台PC。由于这个过程本身的透明的，只要明白其背后的概念以及为何需要boost::asio::ip::tcp::resolver I/O对象就可以了。由于域名解析不是发生在本地的，所以它也被实现为一个异步操作。一旦域名解析成功或被某个错误中断，resolve_handler()函数就会被调用。

因为接收数据需要一个成功的连接，进而需要一次成功的域名解析，所以这三个不同的异步操作要以三个不同的句柄来启动。resolve_handler()访问I/O对象sock，用由迭代器it所提供的解析后地址创建一个连接。而sock也在connect_handler()的内部被使用，发送HTTP请求并启动数据的接收。因为所有这些操作都是异步的，各个句柄的名字被作为参数传递。取决于各个句柄，需要相应的其它参数，如指向解析后地址的迭代器it或用于保存接收到的数据的缓冲区buffer。

开始执行后，该应用将创建一个类型为boost::asio::ip::tcp::resolver::query的对象query，表示一个查询，其中含有名字www.highscore.de以及互联网常用的端口80。这个查询被传递给async_resolve()方法以解析该名字。最后，main()只要调用I/O服务的run()方法，将控制交给操作系统进行异步操作即可。

当域名解析的过程完成后，resolve_handler()被调用，检查域名是否能被解析。如果解析成功，则存有错误条件的对象ec被设为0。只有在这种情况下，才会相应地访问socket以创建连接。服务器的地址是通过类型为boost::asio::ip::tcp::resolver::iterator的第二个参数来提供的。

调用了async_connect()方法之后，connect_handler()会被自动调用。在该句柄的内部，会访问ec对象以检查连接是否已建立。如果连接是有效的，则对相应的socket调用async_read_some()方法，启动读数据操作。为了保存接收到的数据，要提供一个缓冲区作为第一个参数。在以上例子中，缓冲区的类型是boost::array，它来自Boost C++库Array，定义于boost/array.hpp。

每当有一个或多个字节被接收并保存至缓冲区时，read_handler()函数就会被调用。准确的字节数通过std::size_t类型的参数bytes_transferred给出。同样的规则，该句柄应该首先看看参数ec以检查有没有接收错误。如果是成功接收，则将数据写出至标准输出流。

请留意，read_handler()在将数据写出至std::cout之后，会再次调用async_read_some()方法。这是必需的，因为无法保证仅在一次异步操作中就可以接收到整个网页。async_read_some()和read_handler()的交替调用只有当连接被破坏时才中止，如当web服务器已经传送完整个网页时。这种情况下，在read_handler()内部将报告一个错误，以防止进一步将数据输出至标准输出流，以及进一步对该socket调用async_read()方法。这时该例程将停止，因为没有更多的异步操作了。

上个例子是用来取出www.highscore.de的网页的，而下一个例子则示范了一个简单的web服务器。其主要差别在于，这个应用不会连接至其它PC，而是等待连接。

    #include <boost/asio.hpp> 
    #include <string> 
    
    boost::asio::io_service io_service; 
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 80); 
    boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint); 
    boost::asio::ip::tcp::socket sock(io_service); 
    std::string data = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!"; 
    
    void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) 
    { 
    } 
    
    void accept_handler(const boost::system::error_code &ec) 
    { 
      if (!ec) 
      { 
        boost::asio::async_write(sock, boost::asio::buffer(data), write_handler); 
      } 
    } 
    
    int main() 
    { 
      acceptor.listen(); 
      acceptor.async_accept(sock, accept_handler); 
      io_service.run(); 
    } 

类型为boost::asio::ip::tcp::acceptor的I/O对象acceptor被初始化为指定的协议和端口号，用于等待从其它PC传入的连接。初始化工作是通过endpoint对象完成的，该对象的类型为boost::asio::ip::tcp::endpoint，将本例子中的接收器配置为使用端口80来等待IPv4的传入连接，这是www通常所使用的端口和协议。

接收器初始化完成后，main()首先调用listen()方法将接收器置于接收状态，然后再用async_accept()方法等待初始连接。用于发送和接收数据的socket被作为第一个参数传递。

当一个PC试图建立一个连接时，accept_handler()被自动调用。如果该连接请求成功，就执行自由函数boost::asio::async_write()来通过socket发送保存在data中的信息。boost::asio::ip::tcp::socket还有一个名为async_write_some()的方法也可以发送数据，不过它会在发送了至少一个字节之后调用相关联的句柄。该句柄需要计算还剩余多少字节，并反复调用async_write_some()直至所有字节发送完毕。而使用boost::asio::async_write()可以避免这些，因为这个异步操作仅在缓冲区的所有字节都被发送后才结束。

在这个例子中，当所有数据发送完毕，空函数write_handler()将被调用。由于所有异步操作都已完成，所以应用程序终止。与其它PC的连接也被相应关闭。





