##Boost
内容源于书籍《The Boost C++ Libraries》
在线学习地址：[http://zh.highscore.de/cpp/boost/](http://zh.highscore.de/cpp/boost/)

###简介
####C++与Boost
####开发过程
####安装
####概述

###智能指针

####概述
第一版C++标准只提供了一种智能指针：std::auto_ptr。它基本上就像是个普通的指针：通过地址来访问一个动态分配的对象。std::auto_ptr之所以被看作是智能指针，是因为它会在析构的时候调用delete操作符来自动释放所包含的对象。当然这要求在初始化的时候，传给它一个由new 操作符返回的对象的地址。

####RAII

####作用域指针[一般]
一个作用域指针独占一个动态分配的对象。 对应的类名为 boost::scoped_ptr，它的定义在 boost/scoped_ptr.hpp 中。不像std::auto_ptr，一个作用域指针不能传递它所包含的对象的所有权到另一个作用域指针。一旦用一个地址来初始化，这个动态分配的对象将在析构阶段释放。因为一个作用域指针只是简单保存和独占一个内存地址，所以boost::scoped_ptr的实现就要比std::auto_ptr简单。在不需要所有权传递的时候应该优先使用boost::scoped_ptr。在这些情况下，比起std::auto_ptr它是一个更好的选择，因为可以避免不经意间的所有权传递。
实例：
#include <boost/scoped_ptr.hpp> 

int main() 
{
    boost::scoped_ptr<int> i(new int); 
    *i = 1; 
    *i.get() = 2; 
    i.reset(new int); 
}
一经初始化，智能指针boost::scoped_ptr所包含的对象，可以通过类似于普通指针的接口来访问。这是因为重载了相关的操作符operator*()，operator->()和operator bool()。此外，还有get()和reset()方法。
前者返回所含对象的地址，后者用一个新的对象来重新初始化智能指针。 在这种情况下，新创建的对象赋值之前会先自动释放所包含的对象。

boost::scoped_ptr的析构函数中使用delete操作符来释放所包含的对象。这对boost::scoped_ptr所包含的类型加上了一条重要的限制。 boost::scoped_ptr不能用动态分配的数组来做初始化，因为这需要调用delete[]来释放。 在这种情况下，可以使用下面将要介绍的boost:scoped_array类。

####作用域数组[一般]
作用域数组的使用方式与作用域指针相似。关键不同在于，作用域数组的析构函数使用delete[]操作符来释放所包含的对象。因为该操作符只能用于数组对象，所以作用域数组必须通过动态分配的数组来初始化。
对应的作用域数组类名为boost::scoped_array，它的定义在boost/scoped_array.hpp里。
实例：
#include <boost/scoped_array.hpp> 

int main() 
{ 
    boost::scoped_array<int> i(new int[2]); 
    *i.get() = 1; 
    i[1] = 2; 
    i.reset(new int[3]); 
}
boost:scoped_array类重载了操作符operator[]()和operator bool()。可以通过operator[]()操作符访问数组中特定的元素，于是boost::scoped_array类型对象的行为就酷似它所含的数组。

正如boost::scoped_ptr那样，boost:scoped_array也提供了get()和reset()方法，用来返回和重新初始化所含对象的地址。

####共享指针[重要]
这是使用率最高的智能指针。如果开发环境支持的话，可以使用memory中定义的std::shared_ptr。在Boost C++ 库里，这个智能指针命名为boost::shared_ptr，定义在boost/shared_ptr.hpp里。

智能指针boost::shared_ptr基本上类似于boost::scoped_ptr。关键不同之处在于boost::shared_ptr不一定要独占一个对象。它可以和其他boost::shared_ptr类型的智能指针共享所有权。 在这种情况下，当引用对象的最后一个智能指针销毁后，对象才会被释放。

因为所有权可以在boost::shared_ptr之间共享，任何一个共享指针都可以被复制，这跟boost::scoped_ptr是不同的。这样就可以在标准容器里存储智能指针了。你不能在标准容器中存储std::auto_ptr，因为它们在拷贝的时候传递了所有权。
实例：
#include <boost/shared_ptr.hpp> 
#include <vector> 

int main() 
{ 
  std::vector<boost::shared_ptr<int> > v; 
  v.push_back(boost::shared_ptr<int>(new int(1))); 
  v.push_back(boost::shared_ptr<int>(new int(2))); 
}
有了boost::shared_ptr，我们才能像上例中那样，在标准容器中安全的使用动态分配的对象。因为boost::shared_ptr能够共享它所含对象的所有权，所以保存在容器中的拷贝（包括容器在需要时额外创建的拷贝）都是和原件相同的。如前所述，std::auto_ptr做不到这一点，所以绝对不应该在容器中保存它们。

类似于boost::scoped_ptr，boost::shared_ptr类重载了以下这些操作符：operator*()，operator->()和operator bool()。另外还有get()和reset()函数来获取和重新初始化所包含的对象的地址。
实例：
#include <boost/shared_ptr.hpp> 

int main() 
{ 
  boost::shared_ptr<int> i1(new int(1)); 
  boost::shared_ptr<int> i2(i1); 
  i1.reset(new int(2)); 
}
本例中定义了2个共享指针i1和i2，它们都引用到同一个int类型的对象。i1通过new操作符返回的地址显示的初始化，i2 通过i1拷贝构造而来。i1接着调用reset()，它所包含的整数的地址被重新初始化。不过它之前所包含的对象并没有被释放，因为i2仍然引用着它。智能指针boost::shared_ptr记录了有多少个共享指针在引用同一个对象，只有在最后一个共享指针销毁时才会释放这个对象。

默认情况下，boost::shared_ptr使用delete操作符来销毁所含的对象。然而，具体通过什么方法来销毁，也是可以指定的。

####共享数组[较重要]
共享数组的行为类似于共享指针。关键不同在于共享数组在析构时，默认使用delete[]操作符来释放所含的对象。因为这个操作符只能用于数组对象，共享数组必须通过动态分配的数组的地址来初始化。

共享数组对应的类型是boost::shared_array，它的定义在boost/shared_array.hpp里。

#include <boost/shared_array.hpp> 
#include <iostream> 

int main() 
{ 
  boost::shared_array<int> i1(new int[2]); 
  boost::shared_array<int> i2(i1); 
  i1[0] = 1; 
  std::cout << i2[0] << std::endl; 
}
就像共享指针那样，所含对象的所有权可以跟其他共享数组来共享。这个例子中定义了2个变量i1和i2，它们引用到同一个动态分配的数组。i1通过operator[]()操作符保存了一个整数1，这个整数可以被i2引用，比如打印到标准输出。

和本章中所有的智能指针一样，boost::shared_array也同样提供了get()和reset()方法。另外还重载了operator bool()。

####弱指针[不重要]
到目前为止介绍的各种智能指针都能在不同的场合下独立使用。相反，弱指针只有在配合共享指针一起使用时才有意义。弱指针boost::weak_ptr的定义在boost/weak_ptr.hpp里。

####介入式指针[不重要]
介入式指针的工作方式和共享指针完全一样。boost::shared_ptr在内部记录着引用到某个对象的共享指针的数量，可是对介入式指针来说，程序员就得自己来做记录。对于框架对象来说这就特别有用，因为它们记录着自身被引用的次数。
介入式指针boost::intrusive_ptr定义在boost/intrusive_ptr.hpp里。

####指针容器[较重要]
在你见过BoostC++库的各种智能指针之后，应该能够编写安全的代码，来使用动态分配的对象和数组。多数时候，这些对象要存储在容器里，如上所述，使用boost::shared_ptr和boost::shared_array这就相当简单了。

#include <boost/shared_ptr.hpp> 
#include <vector> 

int main() 
{ 
    std::vector<boost::shared_ptr<int> > v; 
    v.push_back(boost::shared_ptr<int>(new int(1))); 
    v.push_back(boost::shared_ptr<int>(new int(2))); 
} 
上面例子中的代码当然是正确的，智能指针确实可以这样用，然而因为某些原因，实际情况中并不这么用。 第一，反复声明boost::shared_ptr需要更多的输入。其次，将boost::shared_ptr拷进拷出，或者在容器内部做拷贝，需要频繁的增加或者减少内部引用计数，这肯定效率不高。由于这些原因，BoostC++库提供了指针容器专门用来管理动态分配的对象。

#include <boost/ptr_container/ptr_vector.hpp> 

int main() 
{ 
    boost::ptr_vector<int> v; 
    v.push_back(new int(1)); 
    v.push_back(new int(2)); 
}
boost::ptr_vector类的定义在boost/ptr_container/ptr_vector.hpp里，它跟前一个例子中用boost::shared_ptr模板参数来初始化的容器具有相同的工作方式。boost::ptr_vector专门用于动态分配的对象，它使用起来更容易也更高效。boost::ptr_vector独占它所包含的对象，因而容器之外的共享指针不能共享所有权，这跟std::vector<boost::shared_ptr<int> >相反。

除了boost::ptr_vector之外，专门用于管理动态分配对象的容器还包括：boost::ptr_deque，boost::ptr_list，boost::ptr_set，boost::ptr_map，boost::ptr_unordered_set和boost::ptr_unordered_map。这些容器等价于C++标准里提供的那些。最后两个容器对应于std::unordered_set和std::unordered_map，它们作为技术报告1的一部分加入C++标准。如果所使用的C++标准实现不支持技术报告1的话，还可以使用BoostC++库里实现的boost::unordered_set和boost::unordered_map。

###函数对象
####概述
####Boost.Bind[较重要]
Boost.Bind是这样的一个库，它简化了由C++标准中的std::bind1st()和std::bind2nd()模板函数所提供的一个机制：将这些函数与几乎不限数量的参数一起使用，就可以得到指定签名的函数。 这种情形的一个最好的例子就是在C++标准中定义的多个不同算法。

#include <iostream>
#include <vector>
#include <algorithm>

void print(int i) 
{ 
    std::cout << i << std::endl; 
} 

int main() 
{ 
    std::vector<int> v; 
    v.push_back(1); 
    v.push_back(3); 
    v.push_back(2); 

    std::for_each(v.begin(), v.end(), print); 
}
算法std::for_each()要求它的第三个参数是一个仅接受正好一个参数的函数或函数对象。如果std::for_each()被执行，指定容器中的所有元素。在上例中，这些元素的类型为int，将按顺序被传入至print()函数。但是，如果要使用一个具有不同签名的函数的话，事情就复杂了。例如，如果要传入的是以下函数add()，它要将一个常数值加至容器中的每个元素上，并显示结果。

void add(int i, int j) 
{ 
  std::cout << i + j << std::endl; 
} 
由于std::for_each()要求的是仅接受一个参数的函数，所以不能直接传入add()函数。源代码必须要修改。

#include <iostream> 
#include <vector> 
#include <algorithm> 
#include <functional> 

class add : public std::binary_function<int, int, void> 
{
    public:
    void operator()(int i, int j) const 
    { 
        std::cout << i + j << std::endl; 
    } 
}; 

int main() 
{ 
    std::vector<int> v; 
    v.push_back(1); 
    v.push_back(3); 
    v.push_back(2); 

    std::for_each(v.begin(), v.end(), std::bind1st(add(), 10)); 
}
以上程序将值10加至容器v的每个元素之上，并使用标准输出流显示结果。 源代码必须作出大幅的修改，以实现此功能。add()函数已被转换为一个派生自std::binary_function的函数对象。

Boost.Bind简化了不同函数之间的绑定。它只包含一个boost::bind()模板函数，定义于boost/bind.hpp中。 使用这个函数，可以如下实现以上例子：

#include <boost/bind.hpp> 
#include <iostream> 
#include <vector> 
#include <algorithm> 

void add(int i, int j) 
{ 
  std::cout << i + j << std::endl; 
} 

int main() 
{ 
  std::vector<int> v; 
  v.push_back(1); 
  v.push_back(3); 
  v.push_back(2); 

  std::for_each(v.begin(), v.end(), boost::bind(add, 10, _1)); 
}
像add()这样的函数不再需要为了要用于std::for_each()而转换为函数对象。使用boost::bind()，这个函数可以忽略其第一个参数而使用。

因为add()函数要求两个参数，两个参数都必须传递给boost::bind()。第一个参数是常数值10，而第二个参数则是一个怪异的_1。

“_1”被称为占位符(placeholder)，定义于Boost.Bind。除了“_1”，Boost.Bind还定义了“_2”和“_3”。通过使用这些占位符，boost::bind()可以变为一元、二元或三元的函数。对于_1，boost::bind()变成了一个一元函数，即只要求一个参数的函数。这是必需的，因为std::for_each()正是要求一个一元函数作为其第三个参数。

当这个程序执行时，std::for_each()对容器v中的第一个元素调用该一元函数。元素的值通过占位符_1传入到一元函数中。这个占位符和常数值被进一步传递到add()函数。通过使用这种机制，std::for_each()只看到了由boost::bind()所定义的一元函数。而boost::bind()本身则只是调用了另一个函数，并将常数值或占位符作为参数传入给它。

下面这个例子通过boost::bind()定义了一个二元函数，用于std::sort()算法，该算法要求一个二元函数作为其第三个参数。

#include <boost/bind.hpp> 
#include <vector> 
#include <algorithm> 

bool compare(int i, int j) 
{
    return i > j;//将容器中的数据降序排列，若修改为i<j，则为升序排列
}

int main() 
{ 
    std::vector<int> v; 
    v.push_back(1); 
    v.push_back(3); 
    v.push_back(2); 

    std::sort(v.begin(), v.end(), boost::bind(compare, _1, _2)); 
}
因为使用了两个占位符_1和_2，所以boost::bind()定义了一个二元函数。std::sort()算法以容器v的两个元素来调用该函数，并根据返回值来对容器进行排序。基于compare()函数的定义，容器将被按降序排列。

但是，由于compare()本身就是一个二元函数，所以使用boost::bind()确是多余的。
#include <boost/bind.hpp> 
#include <vector> 
#include <algorithm> 

bool compare(int i, int j) 
{ 
    return i > j; 
} 

int main() 
{ 
    std::vector<int> v; 
    v.push_back(1); 
    v.push_back(3); 
    v.push_back(2); 

    std::sort(v.begin(), v.end(), compare); 
}
不过使用boost::bind()还是有意义的。例如，如果容器要按升序排列而又不能修改compare()函数的定义。

#include <boost/bind.hpp> 
#include <vector> 
#include <algorithm> 

bool compare(int i, int j) 
{ 
  return i > j; 
} 

int main() 
{ 
  std::vector<int> v; 
  v.push_back(1); 
  v.push_back(3); 
  v.push_back(2); 

  std::sort(v.begin(), v.end(), boost::bind(compare, _2, _1)); 
}
该例子仅改变了占位符的顺序：_2被作为第一参数传递，而_1则被作为第二参数传递至compare()，这样即可改变排序的顺序。

####Boost.Ref[较不重要]
本库Boost.Ref通常与Boost.Bind一起使用，所以我把它们挨着写。它提供了两个函数boost::ref()和boost::cref()，定义于boost/ref.hpp。

当要用于boost::bind()的函数带有至少一个引用参数时，Boost.Ref就很重要了。由于boost::bind()会复制它的参数，所以引用必须特别处理。

#include <boost/bind.hpp> 
#include <iostream> 
#include <vector> 
#include <algorithm> 

void add(int i, int j, std::ostream &os) 
{ 
    os << i + j << std::endl; 
} 

int main() 
{ 
    std::vector<int> v; 
    v.push_back(1); 
    v.push_back(3); 
    v.push_back(2); 

    std::for_each(v.begin(), v.end(), boost::bind(add, 10, _1, boost::ref(std::cout))); 
}
以上例子使用了上一节中的add()函数。不过这一次该函数需要一个流对象的引用来打印信息。因为传给boost::bind()的参数是以值方式传递的，所以std::cout不能直接使用，否则该函数会试图创建它的一份拷贝。

通过使用模板函数boost::ref()，像std::cout这样的流就可以被以引用方式传递，也就可以成功编译上面这个例子了。

要以引用方式传递常量对象，可以使用模板函数boost::cref()。

####Boost.Function[较重要]
为了封装函数指针，Boost.Function提供了一个名为boost::function的类。它定义于boost/function.hpp，用法如下：

#include <boost/function.hpp> 
#include <iostream> 
#include <cstdlib> 
#include <cstring> 

int main() 
{ 
    boost::function<int (const char*)> f = std::atoi; 
    std::cout << f("1609") << std::endl; 
    f = std::strlen; 
    std::cout << f("1609") << std::endl; 
}
boost::function可以定义一个指针，指向具有特定签名的函数。以上例子定义了一个指针f，它可以指向某个接受一个类型为const char*的参数且返回一个类型为int的值的函数。定义完成后，匹配此签名的函数均可赋值给这个指针。
这个例程就是先将std::atoi()赋值给f，然后再将它重赋值为std::strlen()。

注意，给定的数据类型并不需要精确匹配：虽然std::strlen()是以std::size_t作为返回类型的，但是它也可以被赋值给f。

因为f是一个函数指针，所以被赋值的函数可以通过重载的operator()()操作符来调用。取决于当前被赋值的是哪一个函数，在以上例子中将调用std::atoi()或std::strlen()。

如果f未赋予一个函数而被调用，则会抛出一个boost::bad_function_call异常。

#include <boost/function.hpp> 
#include <iostream> 

int main() 
{ 
  try 
  { 
    boost::function<int (const char*)> f; 
    f(""); 
  } 
  catch (boost::bad_function_call &ex) 
  { 
    std::cout << ex.what() << std::endl; 
  } 
}
注意，将值0赋给一个boost::function类型的函数指针，将会释放当前所赋的函数。释放之后再调用它也会导致boost::bad_function_call异常被抛出。要检查一个函数指针是否被赋值某个函数，可以使用empty()函数或operator bool()操作符。

通过使用Boost.Function，类成员函数也可以被赋值给类型为boost::function的对象。

#include <boost/function.hpp> 
#include <iostream> 

struct world 
{ 
  void hello(std::ostream &os) 
  { 
    os << "Hello, world!" << std::endl; 
  } 
}; 

int main() 
{ 
  boost::function<void (world*, std::ostream&)> f = &world::hello; 
  world w; 
  f(&w, boost::ref(std::cout)); 
}
在调用这样的一个函数时，传入的第一个参数表示了该函数被调用的那个特定对象。因此，在模板定义中的左括号后的第一个参数必须是该特定类的指针。 接下来的参数才是表示相应的成员函数的签名。

这个程序还使用了来自Boost.Ref库的boost::ref()，它提供了一个方便的机制向Boost.Function传递引用。

####Boost.Lambda[较不重要]
匿名函数，又称为lambda函数。已经在多种编程语言中存在，但C++除外。不过在Boost.Lambda库的帮助下，现在在C++应用中也可以使用它们了。

lambda函数的目标是令源代码更为紧凑，从而也更容易理解。以本章第一节中的代码例子为例。

#include <iostream> 
#include <vector> 
#include <algorithm> 

void print(int i) 
{ 
  std::cout << i << std::endl; 
} 

int main() 
{ 
  std::vector<int> v; 
  v.push_back(1); 
  v.push_back(3); 
  v.push_back(2); 

  std::for_each(v.begin(), v.end(), print); 
}
这段程序接受容器v中的元素并使用print()函数将它们写出到标准输出流。由于print()只是写出一个简单的int，所以该函数的实现相当简单。严格来说，它是如此地简单，以致于如果可以在std::for_each()算法里面直接定义它的话，会更为方便；从而省去增加一个函数的需要。另外一个好处是代码更为紧凑，使得算法与负责数据输出的函数不是局部性分离的。Boost.Lambda正好使之成为现实。

#include <boost/lambda/lambda.hpp> 
#include <iostream> 
#include <vector> 
#include <algorithm> 

int main() 
{ 
    std::vector<int> v; 
    v.push_back(1); 
    v.push_back(3); 
    v.push_back(2); 

    std::for_each(v.begin(), v.end(), std::cout << boost::lambda::_1 << "\n");
}
Boost.Lambda提供了几个结构来定义匿名函数。代码就被置于执行的地方，从而省去将它包装为一个函数再进行相应的函数调用的这些开销。与原来的例子一样，这个程序将容器v的所有元素写出至标准输出流。

与Boost.Bind相类似，Boost.Lambda也定义了三个占位符，名为_1,_2和_3。但与Boost.Bind不同的是，这些占位符是定义在单独的名字空间的。因此，该例中的第一个占位符是通过boost::lambda::_1来引用的。为了满足编译器的要求，必须包含相应的头文件boost/lambda/lambda.hpp。

虽然代码的位置位于std::for_each()的第三个参数处，看起来很怪异，但boost.Lambda可以写出正常的C++代码。通过使用占位符，容器v的元素可以通过<<传给std::cout以将它们写出到标准输出流。

虽然Boost.Lambda非常强大，但也有一些缺点。要在以上例子中插入换行的话，必须用"\n"来替代std::endl才能成功编译。因为一元std::endl模板函数所要求的类型不同于lambda函数std::cout<<boost::lambda::_1的函数，所以在此不能使用它。

下一个版本的C++标准很可能会将lambda函数作为C++语言本身的组成部分加入，从而消除对单独的库的需要。但是在下一个版本到来并被不同的编译器厂商所采用可能还需要好几年。在此之前，Boost.Lambda被证明是一个完美的替代品，从以下例子可以看出，这个例子只将大于1的元素写出到标准输出流。

#include <boost/lambda/lambda.hpp> 
#include <boost/lambda/if.hpp> 
#include <iostream> 
#include <vector> 
#include <algorithm> 

int main() 
{ 
    std::vector<int> v; 
    v.push_back(1); 
    v.push_back(3); 
    v.push_back(2); 

    std::for_each(v.begin(), v.end(), 
    boost::lambda::if_then(boost::lambda::_1 > 1, 
    std::cout << boost::lambda::_1 << "\n")); 
}
头文件boost/lambda/if.hpp定义了几个结构，允许在lambda函数内部使用if语句。最基本的结构是boost::lambda::if_then()模板函数，它要求两个参数：第一个参数对条件求值，如果为真，则执行第二个参数。如例中所示，每个参数本身都可以是lambda函数。

除了boost::lambda::if_then()，Boost.Lambda还提供了boost::lambda::if_then_else()和boost::lambda::if_then_else_return()模板函数，它们都要求三个参数。另外还提供了用于实现循环、转型操作符，甚至是throw，允许lambda函数抛出异常的模板函数。

虽然可以用这些模板函数在C++中构造出复杂的lambda函数，但是你必须要考虑其它方面，如可读性和可维护性。因为别人需要学习并理解额外的函数，如用boost::lambda::if_then()来替代已知的C++关键字if和else，lambda函数的好处通常随着它的复杂性而降低。多数情况下，更为合理的方法是用熟悉的C++结构定义一个单独的函数。


###事件处理

####概述

####信号Signals[一般]
Boost.Signals提供了一个名为boost::signal的类，定义于boost/signal.hpp，这个头文件是唯一一个需要知道的，它会自动包含其它相关的头文件。

Boost.Signals定义了其它一些类，位于boost::signals名字空间中。由于boost::signal是最常被用到的类，所以它是位于名字空间boost中的。
实例：
#include <boost/signals2.hpp> 
#include <iostream> 

void func() 
{ 
    std::cout << "Hello, world!" << std::endl;
} 

int main() 
{ 
    boost::signals2::signal< void ()> s; 
    s.connect(func); 
    s(); 
}
boost::signal实际上被实现为一个模板函数，具有被用作为事件处理器的函数的签名，该签名也是它的模板参数。在这个例子中，只有签名为void()的函数可以被成功关联至信号s。

函数func()被通过connect()方法关联至信号s。由于func()符合所要求的void()签名，所以该关联成功建立。因此当信号s被触发时，func()将被调用。

信号是通过调用s来触发的，就像普通的函数调用那样。这个函数的签名对应于作为模板参数传入的签名：因为void()不要求任何参数，所以括号内是空的。

调用s会引发一个触发器，进而执行相应的func()函数，之前用connect()关联了的。

同一例子也可以用Boost.Function来实现。
#include <boost/function.hpp> 
#include <iostream> 

void func() 
{ 
    std::cout << "Hello, world!" << std::endl; 
} 

int main() 
{ 
    boost::function<void ()> f; 
    f = func; 
    f(); 
}
和前一个例子相类似，func()被关联至f。当f被调用时，就会相应地执行func()。Boost.Function仅限于这种情形下适用，而Boost.Signals则提供了更多的方式，如关联多个函数至单个特定信号，示例如下。
#include <boost/signals2.hpp> 
#include <iostream> 

void func1() 
{ 
  std::cout << "Hello" << std::flush; 
} 

void func2() 
{ 
  std::cout << ", world!" << std::endl; 
} 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  s.connect(func1); 
  s.connect(func2); 
  s(); 
}
boost::signal可以通过反复调用connect()方法来把多个函数赋值给单个特定信号。当该信号被触发时，这些函数被按照之前用connect()进行关联时的顺序来执行。

另外，执行的顺序也可通过connect()方法的另一个重载版本来明确指定，该重载版本要求以一个int类型的值作为额外的参数。
#include <boost/signals2.hpp> 
#include <iostream> 

void func1() 
{ 
  std::cout << "Hello" << std::flush; 
} 

void func2() 
{ 
  std::cout << ", world!" << std::endl; 
} 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  s.connect(1, func2); 
  s.connect(0, func1); 
  s(); 
}
和前一个例子一样，func1()在func2()之前执行。

要释放某个函数与给定信号的关联，可以用disconnect()方法。
#include <boost/signals2.hpp> 
#include <iostream> 

void func1() 
{ 
  std::cout << "Hello" << std::flush; 
} 

void func2() 
{ 
  std::cout << ", world!" << std::endl; 
} 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  s.connect(func1); 
  s.connect(func2); 
  s.disconnect(func2); 
  s(); 
}
这个例子仅输出Hello，因为与func2()的关联在触发信号之前已经被释放。

除了connect()和disconnect()以外，boost::signal还提供了几个方法。
#include <boost/signals2.hpp> 
#include <iostream> 

void func1() 
{ 
  std::cout << "Hello" << std::flush; 
} 

void func2() 
{ 
  std::cout << ", world!" << std::endl; 
} 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  s.connect(func1); 
  s.connect(func2); 
  std::cout << s.num_slots() << std::endl; 
  if (!s.empty()) 
    s(); 
  s.disconnect_all_slots(); 
}
num_slots()返回已关联函数的数量。如果没有函数被关联，则num_slots()返回0。 这种特定情况下，可以用empty()方法来替代。disconnect_all_slots()方法所做的实际上正是它的名字所表达的：释放所有已有的关联。

看完了函数如何被关联至信号，以及弄明白了信号被触发时会发生什么事之后，还有一个问题：这些函数的返回值去了哪里？以下例子回答了这个问题。
#include <boost/signal.hpp> 
#include <iostream> 

int func1() 
{ 
  return 1; 
} 

int func2() 
{ 
  return 2; 
} 

int main() 
{ 
  boost::signal<int ()> s; 
  s.connect(func1); 
  s.connect(func2); 
  std::cout << s() << std::endl; 
}
func1()和func2()都具有int类型的返回值。s将处理两个返回值，并将它们都写出至标准输出流。那么，到底会发生什么呢？

以上例子实际上会把2写出至标准输出流。两个返回值都被s正确接收，但除了最后一个值，其它值都会被忽略。缺省情况下，所有被关联函数中，实际上只有最后一个返回值被返回。

####连接Connections[一般]
函数可以通过由boost::signal所提供的connect()和disconnect()方法的帮助来进行管理。由于connect()会返回一个类型为boost::signals::connection的值，它们可以通过其它方法来管理。
实例：
#include <boost/signals2.hpp> 
#include <iostream> 

void func() 
{ 
  std::cout << "Hello, world!" << std::endl; 
} 

int main() 
{ 
  boost::signals2::signal<void ()> s; 
  boost::signals2::connection c = s.connect(func); 
  s(); 
  c.disconnect(); 
}
boost::signal的disconnect()方法需要传入一个函数指针，而直接调用boost::signals::connection对象上的disconnect()方法则略去该参数。

除了disconnect()方法之外，boost::signals::connection还提供了其它方法，如block()和unblock()。
实例：
#include <boost/signal.hpp> 
#include <iostream> 

void func() 
{ 
  std::cout << "Hello, world!" << std::endl; 
} 

int main() 
{ 
  boost::signal<void ()> s; 
  boost::signals::connection c = s.connect(func); 
  c.block(); 
  s(); 
  c.unblock(); 
  s(); 
}
以上程序只会执行一次func()。虽然信号s被触发了两次，但是在第一次触发时func()不会被调用，因为连接c实际上已经被block()调用所阻塞。由于在第二次触发之前调用了unblock()，所以之后func()被正确地执行。


###字符串处理

####前言
####区域设置[较不重要]
区域设置在标准C++中封装了文化习俗相关的内容，包括货币符号，日期时间格式，分隔整数部分与分数部分的符号(基数符)以及多于三个数字时的分隔符(千位符)。
在字符串处理方面，区域设置和特定文化中对字符次序以及特殊字符的描述有关。

####Boost.StringAlgorithms[重要]
Boost C++字符串算法库Boost.StringAlgorithms提供了很多字符串操作函数。字符串的类型可以是std::string，std::wstring或任何其他模板类std::basic_string的实例。

这些函数分类别在不同的头文件定义。例如，大小写转换函数定义在文件boost/algorithm/string/case_conv.hpp中。因为Boost.StringAlgorithms类中包括超过20个类别和相同数目的头文件，为了方便起见，头文件boost/algorithm/string.hpp包括了所有其他的头文件。后面所有例子都会使用这个头文件。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream>

int main() 
{ 
  std::string s = "Hello World.";
  std::cout << boost::algorithm::to_upper_copy(s) << std::endl;  //不改变s
  std::cout << boost::algorithm::to_lower_copy(s) << std::endl; 
	
  boost::algorithm::to_upper(s);  //改变s
  std::cout << "s = " << s << std::endl;
  boost::algorithm::to_lower(s);
  std::cout << "s = " << s << std::endl;
}
函数boost::algorithm::to_upper_copy()用于转换一个字符串为大写形式，自然也有提供相反功能的函数boost::algorithm::to_lower_copy()把字符串转换为小写形式。这两个函数都返回转换过的字符串作为结果。如果作为参数传入的字符串自身需要被转换为大(小)写形式，可以使用函数 boost::algorithm::to_upper()或boost::algorithm::to_lower()。

Boost.StringAlgorithms库提供了几个从字符串中删除单独字母的函数，可以明确指定在哪里删除，如何删除。例如，可以使用函数boost::algorithm::erase_all_copy()从整个字符串中删除特定的某个字符。如果只在此字符首次出现时删除，可以使用函数boost::algorithm::erase_first_copy()。如果要在字符串头部或尾部删除若干字符，可以使用函数boost::algorithm::erase_head_copy()和boost::algorithm::erase_tail_copy()。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 

int main() 
{
  std::string s = "Hello World."; 
  std::cout << boost::algorithm::erase_first_copy(s, "l") << std::endl; 
  std::cout << boost::algorithm::erase_last_copy(s, "l") << std::endl; 
  std::cout << boost::algorithm::erase_nth_copy(s, "l", 0) << std::endl;
  std::cout << boost::algorithm::erase_all_copy(s, "l") << std::endl; 
  std::cout << boost::algorithm::erase_head_copy(s, 5) << std::endl; 
  std::cout << boost::algorithm::erase_tail_copy(s, 8) << std::endl; 
}

boost::algorithm::erase_first_copy(s, "l")：删除字符串s中第一个'l'字符
boost::algorithm::erase_last_copy(s, "l")：删除字符串s中最后一个'l'字符
boost::algorithm::erase_nth_copy(s, "l", 0)：删除字符串s中第0个'l'字符
boost::algorithm::erase_all_copy(s, "l")：删除字符串s中所有'l'字符
boost::algorithm::erase_head_copy(s, 5)：删除字符串s中前5个字符
boost::algorithm::erase_tail_copy(s, 8)：删除字符串s中后8个字符

以下各个不同函数boost::algorithm::find_first()、boost::algorithm::find_last()、boost::algorithm::find_nth()、boost::algorithm::find_head()以及boost::algorithm::find_tail()可以用于在字符串中查找子串。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 

int main() 
{
  std::string s = "Hello World."; 
  boost::iterator_range<std::string::iterator> r;
  r = boost::algorithm::find_first(s, "Hello"); 
  std::cout << r << std::endl; 
  r = boost::algorithm::find_first(s, "xyz"); 
  std::cout << r << std::endl; 
  r = boost::algorithm::find_last(s, "Hello"); 
  std::cout << r << std::endl; 
  r = boost::algorithm::find_nth(s, "Hello", 0); 
  std::cout << r << std::endl;
}
所有这些函数的共同点是均返回类型为boost::iterator_range类的一对迭代器。此类起源于Boost C++的Boost.Range库，它在迭代器的概念上定义了“范围”。因为操作符<<由boost::iterator_range类重载而来，单个搜索算法的结果可以直接写入标准输出流。

实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 
#include <vector> 

int main() 
{ 
  std::vector<std::string> v; 
  v.push_back("Hello"); 
  v.push_back("World");
  std::cout << boost::algorithm::join(v, " ") << std::endl;
}
函数boost::algorithm::join()接受一个字符串的容器作为第一个参数，根据第二个参数将这些字符串连接起来。相应地这个例子会输出Hello World。

Boost.StringAlgorithms库不但提供了查找子串或删除字母的函数，而且提供了使用字符串替代子串的函数，包括boost::algorithm::replace_first_copy()，boost::algorithm::replace_nth_copy()，boost::algorithm::replace_last_copy()，boost::algorithm::replace_all_copy()，boost::algorithm::replace_head_copy()以及boost::algorithm::replace_tail_copy()等等。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 

int main() 
{ 
  std::string s = "Hello World."; 
  std::cout << boost::algorithm::replace_first_copy(s, "l", "L") << std::endl; 
  std::cout << boost::algorithm::replace_nth_copy(s, "l", 0, "L") << std::endl; 
  std::cout << boost::algorithm::replace_last_copy(s, "l", "L") << std::endl; 
  std::cout << boost::algorithm::replace_all_copy(s, "l", "L") << std::endl; 
  std::cout << boost::algorithm::replace_head_copy(s, 5, "###") << std::endl; 
  std::cout << boost::algorithm::replace_tail_copy(s, 8, "###") << std::endl; 
}
它们的使用方法同查找和删除函数是差不多一样的，所不同的是还需要一个替代字符串作为附加参数。

可以使用修剪函数boost::algorithm::trim_left_copy()，boost::algorithm::trim_right_copy()以及boost::algorithm::trim_copy()等自动去除字符串中的空格或者字符串的结束符。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 

int main() 
{
  std::string s = "\t Hello World. \t"; 
  std::cout << "=" << s << "=" << std::endl; 
  std::cout << "=" << boost::algorithm::trim_left_copy(s) << "=" << std::endl; 
  std::cout << "=" << boost::algorithm::trim_right_copy(s) << "=" << std::endl; 
  std::cout << "=" << boost::algorithm::trim_copy(s) << "=" << std::endl; 
}

Boost.StringAlgorithms库的函数可以接受一个附加的谓词参数，以决定函数作用于字符串的哪些字符。谓词版本的修剪函数相应地被命名为boost::algorithm::trim_left_copy_if()，boost::algorithm::trim_right_copy_if()和boost::algorithm::trim_copy_if()。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 

int main() 
{
  std::string s = "--Hello World.--"; 
  std::cout << "=" << boost::algorithm::trim_left_copy_if(s, boost::algorithm::is_any_of("-")) << "=" << std::endl; 
  std::cout << "=" <<boost::algorithm::trim_right_copy_if(s, boost::algorithm::is_any_of("-")) << "=" << std::endl; 
  std::cout << "=" <<boost::algorithm::trim_copy_if(s, boost::algorithm::is_any_of("-")) << "=" << std::endl; 
}
以上程序调用了一个辅助函数boost::algorithm::is_any_of()，它用于生成谓词以验证作为参数传入的字符是否在给定的字符串中存在。使用函数boost::algorithm::is_any_of后，正如例子中做的那样，修剪字符串的字符被指定为连字符。

Boost.StringAlgorithms类也提供了众多返回通用谓词的辅助函数。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 

int main() 
{
  std::string s = "123456789Hello World123456789"; 
  std::cout << "=" << boost::algorithm::trim_left_copy_if(s, boost::algorithm::is_digit()) << "=" << std::endl; 
  std::cout << "=" <<boost::algorithm::trim_right_copy_if(s, boost::algorithm::is_digit()) << "=" << std::endl; 
  std::cout << "=" <<boost::algorithm::trim_copy_if(s, boost::algorithm::is_digit()) << "=" << std::endl; 
}
函数boost::algorithm::is_digit()返回的谓词在字符为数字时返回布尔值true。检查字符是否为大写或小写的辅助函数分别是boost::algorithm::is_upper()和boost::algorithm::is_lower()。

除了检验单独字符的谓词之外， Boost.StringAlgorithms 库还提供了处理字符串的函数。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 

int main() 
{ 
  std::string s = "Hello World"; 
  std::cout << boost::algorithm::starts_with(s, "Hello") << std::endl; 
  std::cout << boost::algorithm::ends_with(s, "World") << std::endl; 
  std::cout << boost::algorithm::contains(s, "llo") << std::endl; 
  std::cout << boost::algorithm::lexicographical_compare(s, "Hello") << std::endl; 
}
函数boost::algorithm::starts_with()、boost::algorithm::ends_with()、boost::algorithm::contains()均可以比较两个字符串。

以下介绍一个字符串切割函数。
实例：
#include <boost/algorithm/string.hpp>
#include <iostream> 
#include <vector> 

int main() 
{
  std::string s = "Hello World"; 
  std::vector<std::string> v; 
  boost::algorithm::split(v, s, boost::algorithm::is_space()); 
  std::cout << v.size() << std::endl; 
}
在给定分界符后，使用函数boost::algorithm::split()可以将一个字符串拆分为一个字符串容器。它需要给定一个谓词作为第三个参数以判断应该在字符串的哪个位置分割。这个例子使用了辅助函数boost::algorithm::is_space()创建一个谓词，在每个空格字符处分割字符串。

本节中许多函数都有忽略字符串大小写的版本，这些版本一般都有与原函数相似的名称，所相差的只是以'i'开头。例如，与函数 boost::algorithm::erase_all_copy()相对应的是函数boost::algorithm::ierase_all_copy()。

最后，值得注意的是类Boost.StringAlgorithms中许多函数都支持正则表达式。以下程序使用函数boost::algorithm::find_regex()搜索正则表达式。
实例：
#include <boost/algorithm/string.hpp> 
#include <boost/algorithm/string/regex.hpp>
#include <iostream> 

int main() 
{ 
  std::string s = "Hello World"; 
  boost::iterator_range<std::string::iterator> r;
  r = boost::algorithm::find_regex(s, boost::regex("\\w\\s\\w")); 
  std::cout << r << std::endl; 
}
为了使用正则表达式，此程序使用了Boost C++库中的boost::regex，这将在下一节介绍。


###多线程

####概述

####线程管理
在这个库最重要的一个类就是boost::thread，它是在boost/thread.hpp里定义的，用来创建一个新线程。下面的示例来说明如何运用它。
实例：
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

sleep()要么在预计的一段时间或一个特定的时间点后时才让线程继续执行。通过传递一个类型为boost::posix_time::seconds的对象，在这个例子里我们指定了一段时间。boost::posix_time::seconds来自于Boost.DateTime库，它被 Boost.Thread 用来管理和处理时间的数据。

虽然前面的例子说明了如何等待一个不同的线程，但下面的例子演示了如何通过所谓的中断点让一个线程中断。

#include <boost/thread.hpp> 
#include <iostream> 

void wait(int seconds) 
{ 
  boost::this_thread::sleep(boost::posix_time::seconds(seconds)); 
} 

void thread() 
{ 
  try 
  { 
    for (int i = 0; i < 5; ++i) 
    { 
      wait(1); 
      std::cout << i << std::endl; 
    } 
  } 
  catch (boost::thread_interrupted&) 
  { 
  } 
} 

int main() 
{ 
  boost::thread t(thread); 
  wait(3); 
  t.interrupt(); 
  t.join(); 
}
在一个线程对象上调用interrupt()会中断相应的线程。在这方面，中断意味着一个类型为boost::thread_interrupted的异常，它会在这个线程中抛出。然后这只有在线程达到中断点时才会发生。

如果给定的线程不包含任何中断点，简单调用interrupt()就不会起作用。每当一个线程中断点，它就会检查interrupt()是否被调用过。只有被调用过了，boost::thread_interrupted异常才会相应地抛出。

Boost.Thread定义了一系列的中断点，例如sleep()函数。由于sleep()在这个例子里被调用了五次，该线程就检查了五次它是否应该被中断。然而sleep()之间的调用，却不能使线程中断。

一旦该程序被执行，它只会打印三个数字到标准输出流。这是由于在main里3秒后调用interrupt()方法。因此，相应的线程被中断，并抛出一个boost::thread_interrupted异常。这个异常在线程内也被正确地捕获，catch处理虽然是空的。由于thread()函数在处理程序后返回，线程也被终止。这反过来也将终止整个程序，因为main()等待该线程使用join()终止该线程。

Boost.Thread定义包括上述sleep()函数十个中断。有了这些中断点，线程可以很容易及时中断。然而，他们并不总是最佳的选择，因为中断点必须事前读入以检查boost::thread_interrupted异常。

为了提供一个对 Boost.Thread 里提供的多种函数的整体概述，下面的例子将会再介绍两个。
实例：
#include <boost/thread.hpp> 
#include <iostream> 

int main() 
{ 
  std::cout << boost::this_thread::get_id() << std::endl; 
  std::cout << boost::thread::hardware_concurrency() << std::endl; 
}
使用boost::this_thread命名空间，能提供独立的函数应用于当前线程，比如前面出现的sleep()。另一个是 get_id()，它会返回一个当前线程的ID号。它也是由boost::thread提供的。

boost::thread类提供了一个静态方法hardware_concurrency()，它能够返回基于CPU数目或者CPU内核数目的刻在同时在物理机器上运行的线程数。在常用的双核机器上调用这个方法，返回值为2。 这样的话就可以确定在一个多核程序可以同时运行的理论最大线程数。

####同步
虽然多线程的使用可以提高应用程序的性能，但也增加了复杂性。 如果使用线程在同一时间执行几个函数，访问共享资源时必须相应地同步。 一旦应用达到了一定规模，这涉及相当一些工作。 本段介绍了Boost.Thread提供同步线程的类。

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
下载源代码
多线程程序使用所谓的互斥对象来同步。 Boost.Thread提供多个的互斥类，boost::mutex是最简单的一个。 互斥的基本原则是当一个特定的线程拥有资源的时候防止其他线程夺取其所有权。 一旦释放，其他的线程可以取得所有权。 这将导致线程等待至另一个线程完成处理一些操作，从而相应地释放互斥对象的所有权。

上面的示例使用一个类型为 boost::mutex 的 mutex 全局互斥对象。 thread() 函数获取此对象的所有权才在 for 循环内使用 lock() 方法写入到标准输出流的。 一旦信息被写入，使用 unlock() 方法释放所有权。

main() 创建两个线程，同时执行 thread ()函数。 利用 for 循环，每个线程数到5，用一个迭代器写一条消息到标准输出流。 不幸的是，标准输出流是一个全局性的被所有线程共享的对象。 该标准不提供任何保证 std::cout 可以安全地从多个线程访问。 因此，访问标准输出流必须同步：在任何时候，只有一个线程可以访问 std::cout。

由于两个线程试图在写入标准输出流前获得互斥体，实际上只能保证一次只有一个线程访问 std::cout。 不管哪个线程成功调用 lock() 方法，其他所有线程必须等待，直到 unlock() 被调用。

获取和释放互斥体是一个典型的模式，是由Boost.Thread通过不同的数据类型支持。 例如，不直接地调用 lock() 和 unlock()，使用 boost::lock_guard 类也是可以的。

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
下载源代码
boost::lock_guard 在其内部构造和析构函数分别自动调用 lock() 和 unlock() 。 访问共享资源是需要同步的，因为它显示地被两个方法调用。 boost::lock_guard 类是另一个出现在 第 2 章 智能指针 的RAII用语。

除了boost::mutex 和 boost::lock_guard 之外，Boost.Thread也提供其他的类支持各种同步。 其中一个重要的就是 boost::unique_lock ，相比较 boost::lock_guard 而言，它提供许多有用的方法。

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
下载源代码
上面的例子用不同的方法来演示 boost::unique_lock 的功能。 当然了，这些功能的用法对给定的情景不一定适用；boost::lock_guard 在上个例子的用法还是挺合理的。 这个例子就是为了演示 boost::unique_lock 提供的功能。

boost::unique_lock 通过多个构造函数来提供不同的方式获得互斥体。 这个期望获得互斥体的函数简单地调用了 lock() 方法，一直等到获得这个互斥体。 所以它的行为跟 boost::lock_guard 的那个是一样的。

如果第二个参数传入一个 boost::try_to_lock 类型的值，对应的构造函数就会调用 try_lock() 方法。 这个方法返回 bool 型的值：如果能够获得互斥体则返回true，否则返回 false 。 相比 lock() 函数，try_lock() 会立即返回，而且在获得互斥体之前不会被阻塞。

上面的程序向 boost::unique_lock 的构造函数的第二个参数传入boost::try_to_lock。 然后通过 owns_lock() 可以检查是否可获得互斥体。 如果不能， owns_lock() 返回 false。 这也用到 boost::unique_lock 提供的另外一个函数： timed_lock() 等待一定的时间以获得互斥体。 给定的程序等待长达1秒，应较足够的时间来获取更多的互斥。

其实这个例子显示了三个方法获取一个互斥体：lock() 会一直等待，直到获得一个互斥体。 try_lock() 则不会等待，但如果它只会在互斥体可用的时候才能获得，否则返回 false 。 最后，timed_lock() 试图获得在一定的时间内获取互斥体。 和 try_lock() 一样，返回bool 类型的值意味着成功是否。

虽然 boost::mutex 提供了 lock() 和 try_lock() 两个方法，但是 boost::timed_mutex 只支持 timed_lock() ，这就是上面示例那么使用的原因。 如果不用 timed_lock() 的话，也可以像以前的例子那样用 boost::mutex。

就像 boost::lock_guard 一样， boost::unique_lock 的析构函数也会相应地释放互斥量。此外，可以手动地用 unlock() 释放互斥量。也可以像上面的例子那样，通过调用 release() 解除boost::unique_lock 和互斥量之间的关联。然而在这种情况下，必须显式地调用 unlock() 方法来释放互斥量，因为 boost::unique_lock 的析构函数不再做这件事情。

boost::unique_lock 这个所谓的独占锁意味着一个互斥量同时只能被一个线程获取。 其他线程必须等待，直到互斥体再次被释放。 除了独占锁，还有非独占锁。 Boost.Thread里有个 boost::shared_lock 的类提供了非独占锁。 正如下面的例子，这个类必须和 boost::shared_mutex 型的互斥量一起使用。

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
下载源代码
boost::shared_lock 类型的非独占锁可以在线程只对某个资源读访问的情况下使用。 一个线程修改的资源需要写访问，因此需要一个独占锁。 这样做也很明显：只需要读访问的线程不需要知道同一时间其他线程是否访问。 因此非独占锁可以共享一个互斥体。

在给定的例子， print() 和 count() 都可以只读访问 random_numbers 。 虽然 print() 函数把 random_numbers 里的最后一个数写到标准输出，count() 函数把它统计到 sum 变量。 由于没有函数修改 random_numbers，所有的都可以在同一时间用 boost::shared_lock 类型的非独占锁访问它。

在 fill() 函数里，需要用一个 boost::unique_lock 类型的非独占锁，因为它插入了一个新的随机数到 random_numbers。 在 unlock() 显式地调用 unlock() 来释放互斥量之后， fill() 等待了一秒。 相比于之前的那个样子， 在 for 循环的尾部调用 wait() 以保证容器里至少存在一个随机数，可以被print() 或者 count() 访问。 对应地，这两个函数在 for 循环的开始调用了 wait() 。

考虑到在不同的地方每个单独地调用 wait() ，一个潜在的问题变得很明显:函数调用的顺序直接受CPU执行每个独立进程的顺序决定。 利用所谓的条件变量，可以同步哪些独立的线程，使数组的每个元素都被不同的线程立即添加到 random_numbers 。

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
下载源代码
这个例子的程序删除了 wait() 和 count() 。线程不用在每个循环迭代中等待一秒，而是尽可能快地执行。此外，没有计算总额；数字完全写入标准输出流。

为确保正确地处理随机数，需要一个允许检查多个线程之间特定条件的条件变量来同步不每个独立的线程。

正如上面所说， fill() 函数用在每个迭代产生一个随机数，然后放在 random_numbers 容器中。 为了防止其他线程同时访问这个容器，就要相应得使用一个排它锁。 不是等待一秒，实际上这个例子却用了一个条件变量。 调用 notify_all() 会唤醒每个哪些正在分别通过调用wait() 等待此通知的线程。

通过查看 print() 函数里的 for 循环，可以看到相同的条件变量被 wait() 函数调用了。 如果这个线程被 notify_all() 唤醒，它就会试图这个互斥量，但只有在 fill() 函数完全释放之后才能成功。

这里的窍门就是调用 wait() 会释放相应的被参数传入的互斥量。 在调用 notify_all()后， fill() 函数会通过 wait() 相应地释放线程。 然后它会阻止和等待其他的线程调用 notify_all() ，一旦随机数已写入标准输出流，这就会在 print() 里发生。

注意到在 print() 函数里调用 wait() 事实上发生在一个单独 while 循环里。 这样做的目的是为了处理在 print() 函数里第一次调用 wait() 函数之前随机数已经放到容器里。 通过比较 random_numbers 里元素的数目与预期值，发现这成功地处理了把随机数写入到标准输出流。









