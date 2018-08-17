###日期与时间

####概述
库Boost.DateTime可用于处理时间数据，如历法日期和时间。另外，Boost.DateTime还提供了扩展来处理时区的问题，且支持历法日期和时间的格式化输入与输出。本章将覆盖Boost.DateTime的各个部分。

####历法日期
用于处理历法日期的类和函数位于名字空间boost::gregorian中，定义于boost/date_time/gregorian/gregorian.hpp。要创建一个日期，请使用boost::gregorian::date类。

    #include <boost/date_time/gregorian/gregorian.hpp> 
    #include <iostream> 
    
    int main() 
    { 
      boost::gregorian::date d(2010, 1, 30); 
      std::cout << d.year() << std::endl; 
      std::cout << d.month() << std::endl; 
      std::cout << d.day() << std::endl; 
      std::cout << d.day_of_week() << std::endl; 
      std::cout << d.end_of_month() << std::endl; 
    } 
boost::gregorian::date提供了多个构造函数来进行日期的创建。最基本的构造函数接受一个年份、一个月份和一个日期作为参数。如果给定的是一个无效值，则将分别抛出boost::gregorian::bad_year，boost::gregorian::bad_month或boost::gregorian::bad_day_of_month类型的异常，这些异常均派生自std::out_of_range。

正如在这个例子中所示的，有多个方法用于访问一个日期。像year()，month()和day()这些方法访问用于初始化的初始值，像day_of_week()和end_of_month()这些方法则访问计算得到的值。

而boost::gregorian::date的构造函数则接受年份、月份和日期的值来设定一个日期，调用month()方法实际上会显示Jan，而调用day_of_week()则显示Sat。它们不是普通的数字值，而分别是boost::gregorian::date::month_type和boost::gregorian::date::day_of_week_type类型的值。不过，Boost.DateTime为格式化的输入输出提供了全面的支持，可以将以上输出从Jan调整为1。

请留意，boost::gregorian::date的缺省构造函数会创建一个无效的日期。这样的无效日期也可以通过将boost::date_time::not_a_date_time作为单一参数传递给构造函数来显式地创建。

除了直接调用构造函数，也可以通过自由函数或其它对象的方法来创建一个boost::gregorian::date类型的对象。

    #include <boost/date_time/gregorian/gregorian.hpp> 
    #include <iostream> 
    
    int main() 
    { 
      boost::gregorian::date d = boost::gregorian::day_clock::universal_day(); 
      std::cout << d.year() << std::endl; 
      std::cout << d.month() << std::endl; 
      std::cout << d.day() << std::endl; 
    
      d = boost::gregorian::date_from_iso_string("20100131"); 
      std::cout << d.year() << std::endl; 
      std::cout << d.month() << std::endl; 
      std::cout << d.day() << std::endl; 
    } 
这个例子使用了boost::gregorian::day_clock类，它是一个返回当前日期的时钟类。方法universal_day()返回一个与时区及夏时制无关的UTC日期。UTC是世界时间(universal time)的国际缩写。boost::gregorian::day_clock还提供了另一个方法local_day()，它接受本地设置。要取出本地时区的当前日期，必须使用local_day()。

boost::gregorian::date表示的是一个特定的时间点，而boost::gregorian::date_duration则表示了一段时间。

    #include <boost/date_time/gregorian/gregorian.hpp> 
    #include <iostream> 
    
    int main() 
    { 
      boost::gregorian::date d1(2008, 1, 31); 
      boost::gregorian::date d2(2008, 8, 31); 
      boost::gregorian::date_duration dd = d2 - d1; 
      std::cout << dd.days() << std::endl; 
    } 
由于boost::gregorian::date重载了operator-()操作符，所以两个时间点可以如上所示那样相减。返回值的类型为boost::gregorian::date_duration，表示了两个日期之间的时间长度。

boost::gregorian::date_duration所提供的最重要的方法是days()，它返回一段时间内所包含的天数。

####位置无关的时间

####位置相关的时间

####格式化输入输出
本章中的所有例子在执行后都提供形如2009-Jan-07这样的输出结果。有的人可能更喜欢用其它格式来显示结果。Boost.DateTime允许boost::date_time::date_facet和boost::date_time::time_facet类来格式化历法日期和时间。

    #include <boost/date_time/gregorian/gregorian.hpp> 
    #include <iostream> 
    #include <locale> 
    
    int main() 
    { 
      boost::gregorian::date d(2009, 1, 7); 
      boost::gregorian::date_facet *df = new boost::gregorian::date_facet("%A, %d %B %Y"); 
      std::cout.imbue(std::locale(std::cout.getloc(), df)); 
      std::cout << d << std::endl; 
    } 
Boost.DateTime使用了locales的概念，它来自于C++标准，在第5章字符串处理中有概括的介绍。要格式化一个历法日期，必须创建一个boost::date_time::date_facet类型的对象并安装在一个locale内。一个描述新格式的字符串被传递给boost::date_time::date_facet的构造函数。上面的例子传递的是%A, %d %B %Y，指定格式为：星期几后跟日月年全名：Wednesday, 07 January 2009。

Boost.DateTime提供了多个格式化标志，标志由一个百分号后跟一个字符组成。Boost.DateTime的文档中对于所支持的所有标志有一个完整的介绍。例如，%A表示星期几的全名。


###序列化
####概述
Boost C++的序列化库允许将C++应用程序中的对象转换为一个字节序列，此序列可以被保存，并可在将来恢复对象的时候再次加载。各种不同的数据格式，包括XML，只要具有一定规则的数据格式，在序列化后都产生一个字节序列。

####归档
Boost.Serialization的主要概念是归档。归档的文件是相当于序列化的C++对象的一个字节流。对象可以通过序列化添加到归档文件，相应地也可从归档文件中加载。为了恢复和之前存储相同的C++对象，需假定数据类型是相同的。

下面看一个简单的例子。

    #include <boost/archive/text_oarchive.hpp> 
    #include <iostream> 
    
    int main() 
    { 
      boost::archive::text_oarchive oa(std::cout); 
      int i = 1; 
      oa << i; 
    } 
Boost.Serialization提供了多个归档类，如boost::archive::text_oarchive类，它定义在boost/archive/text_oarchive.hpp文件中。boost::archive::text_oarchive可将对象序列化为文本流。上面的应用程序将22 serialization::archive 5 1 写出到标准输出流。

可见，boost::archive::text_oarchive类型的对象oa可以用来像流(stream)一样通过<<来序列化对象。尽管如此，归档也不能被认为是可以存储任何数据的常规的流。为了以后恢复数据，必须以相同的顺序使用和先前存储时用的一样的数据类型。下面的例子序列化和恢复了int类型的变量。

    #include <boost/archive/text_oarchive.hpp> 
    #include <boost/archive/text_iarchive.hpp> 
    #include <iostream> 
    #include <fstream> 
    
    void save() 
    { 
      std::ofstream file("archiv.txt"); 
      boost::archive::text_oarchive oa(file); 
      int i = 1; 
      oa << i; 
    } 
    
    void load() 
    { 
      std::ifstream file("archiv.txt"); 
      boost::archive::text_iarchive ia(file); 
      int i = 0; 
      ia >> i; 
      std::cout << i << std::endl; 
    } 
    
    int main() 
    { 
      save(); 
      load(); 
    } 
当boost::archive::text_oarchive被用来把数据序列化为文本流，boost::archive::text_iarchive就用来从文本流恢复数据。为了使用这些类，必须包含boost/archive/text_iarchive.hpp头文件。

归档的构造函数需要一个输入或者输出流作为参数。流分别用来序列化或恢复数据。虽然上面的应用程序使用了一个文件流，其他流，如stringstream流也是可以的。

    #include <boost/archive/text_oarchive.hpp> 
    #include <boost/archive/text_iarchive.hpp> 
    #include <iostream> 
    #include <sstream> 
    
    std::stringstream ss; 
    
    void save() 
    { 
      boost::archive::text_oarchive oa(ss); 
      int i = 1; 
      oa << i; 
    } 
    
    void load() 
    { 
      boost::archive::text_iarchive ia(ss); 
      int i = 0; 
      ia >> i; 
      std::cout << i << std::endl; 
    } 
    
    int main() 
    { 
      save(); 
      load(); 
    } 
这个应用程序也向标准输出流写了1。然而，与前面的例子相比, 数据却是用stringstream流序列化的。

####指针和引用

####对象类层次结构的序列化

####优化用封装函数


###词法分析器

####概述

####
