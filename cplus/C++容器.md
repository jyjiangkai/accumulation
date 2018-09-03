##Array
array<int,10> ial;
array<int,10> ial1={0,1,2,3};

##Vector
vector<int> v
1.push_back			在数组的最后添加一个数据
2.pop_back			去掉数组的最后一个数据
3.at				得到编号位置的数据
4.begin				得到数组头的指针
5.end				得到数组的最后一个单元+1的指针
6.front				得到数组头的引用
7.back				得到数组的最后一个单元的引用
8.max_size			得到vector最大可以是多大
9.capacity			当前vector分配的大小
10.size				当前使用数据的大小
11.resize			改变当前使用数据的大小，如果它比当前使用的大，则填充默认值
12.reserve			改变当前vector所分配空间的大小
13.erase			删除指针指向的数据项
14.clear			清空当前vector
15.rbegin			将vector反转后的开始指针返回(其实就是原来的end-1)
16.rend				将vector反转后的结束指针返回(其实就是原来的begin-1)
17.empty			判断vector是否为空
18.swap				与另一个vector交换数据

###详细的函数实现功能：其中
vector<int> c
c.clear()			移除容器中所有数据
c.empty()			判断容器是否为空
c.erase(pos)		删除pos位置的shuju
c.erase(beg,end)	删除[beg,end)区间的数据
c.front()			传回第一个数据
c.insert(pos,elem)	在pos位置插入一个elem拷贝
c.pop_back()		删除最后一个数据
c.push_back(elem)	在尾部加入一个数据
c.resize(num)		重新设置该容器的大小
c.size()			返回容器中实际数据的个数
c.begin()			返回指向容器第一个元素的迭代器
c.end()				返回指向容器最后一个元素的迭代器

###内存管理与效率
使用reserve()函数提前设定容量大小，避免多次扩容操作导致效率低下。
关于STL容器，最令人称赞的特性之一就是只要不超过它们的最大大小，它们就可以自动增长到足以容纳你放进去的数据(要知道这个最大值，只要调用名叫max_size的成员函数)。对于vector和string，如果需要更多空间，就以类似realloc的思想来增长大小。
vector容器支持随机访问，因此为了提高效率，它内部使用动态数组的方式来实现的。在通过reserve()来申请特定大小的时候总是按照指数边界来增大其内部缓冲区。当进行insert或push_back等增加元素的操作时，如果此时动态数组的内存不够用，就要动态的重新分配当前大小的1.5~2倍的新内存区，再把原数组的内容复制过去。所以，在一般情况下，其访问速度同一般数组，只有在重新分配发生时，其性能才会下降。正如上面的代码告诉你的那样。而进行pop_back操作时，capacity并不会因为vector容器里的元素减少而有所下降，还会持续操作之前的大小。对于vector容器来说，如果有大量的数据需要进行push_back，应当使用reserve()函数提前设定其容量大小，否则会出现许多次容量扩充的操作，导致效率低下。

我简要介绍有时候令人困惑的四个相关成员函数。在标准容器中，只有vector和string提供了所有这些函数。
(1)size()告诉你容器中有多少元素。它没有告诉你容器为它容纳的元素分配了多少内存。
(2)capacity()告诉你容器在它已经分配的内存中可以容纳多少元素。那是容器在那块内存中总共可以容纳多少元素，而不是还可以容纳多少元素。如果你想知道一个vector和string中有多少没有被占用的内存，你必须从capacity()中减去size()。如果size和capacity返回同样的值，容器中就没有剩余空间了，而下一次插入(通过insert或push_back等)，会引发上面的重新分配步骤。
(3)resize(Container::size_type n)强制把容器改为容纳n个元素。调用resize之后，size将会返回n。如果n小于当前大小，容器尾部的元素会被销毁。如果n大于当前大小，新默认构造的元素会添加到容器尾部。如果n大于当前容量，在元素加入之前会发生重新分配。
(4)reserve(Container::size_type n)强制容器把它的容量改为至少n，提供的n不小于当前大小。这一般强迫进行一次重新分配，因为容量需要增加。(如果n小于当前容量，vector忽略它，这个调用什么都不做，string可能把它的容量减少为size()和n中大的数，但string的大小没有改变。)

###vector查找某元素是否存在
方法一：
#include <algorithm>
//C++的标准模版库（STL）中最重要的头文件之一，提供了大量基于迭代器的非成员模版函数。
vector<string> v;
int nRet = std::count(v.begin(), v.end(), "xiaochun");    //判断vector中是否有"xiaochun"这个元素
方法二：
#include <algorithm>
//C++的标准模版库（STL）中最重要的头文件之一，提供了大量基于迭代器的非成员模版函数。
vector<string> vec;
vector<string>::iterator iter;
iter = find(vec.begin(), vec.end(), "SZ000001");
if(iter != vec.end())
{
    //vec中存在"SZ000001"
    cout<<(*iter)<<endl;
}
注意：
如果vector中保存的是自定义类型(结构体/类)，则需要为该类型重载==操作符，再用find。
结构体中重载操作符实例：
struct point  
{  
    int elem;  
    bool operator==(const point b) const  //重载“==”操作符，函数最后的const别忘了，否则会报错。
    {  
        return this->elem == b.elem;  
    }  
    bool operator!=(const point b) const  
    {  
        return this->elem != b.elem;  
    }  
    bool operator<=(const point b) const  
    {  
        return this->elem <= b.elem;  
    }  
    bool operator<(const point b) const  
    {  
        return this->elem < b.elem;  
    }  
    bool operator>=(const point b) const  
    {  
        return this->elem >= b.elem;  
    }  
    bool operator>(const point b) const  
    {  
        return this->elem > b.elem;  
    }  
}now; 

###vector的四种遍历方式
遍历方式1，采用迭代器
for(vector<CVector2>::iterator it = obbject.TexCoord.begin(); it != obbject.TexCoord.end(); it++)
{
	cout<<(*it).x<<endl;
	cout<<(*it).y<<endl;
}
遍历方式2，采用迭代器，并采用C++11新标准中的auto关键字
for(auto it = obbject.TexCoord.begin(); it != obbject.TexCoord.end(); it++)
{
	cout<<(*it).x<<endl;
	cout<<(*it).y<<endl;
}
遍历方式3，采用下角标进行数据元素访问
for(size_t i = 0; i < obbject.TexCoord.size(); i++)
{
	cout<<obbject.TexCoord[i].x<<endl;
	cout<<obbject.TexCoord[i].y<<endl;
}
遍历方式4，采用C++11新标准中的auto关键字
for(auto i:obbject.TexCoord)
{
	cout<<i.x<<endl;
	cout<<i.y<<endl;
}


##Set
set集合是c++ stl库中自带的一个容器，set具有以下两个特点：
1、set中的元素都是排好序的
2、set集合中没有重复的元素

常用操作：
begin()		返回set容器的第一个元素的地址
end()		返回set容器的最后一个元素地址
clear()		删除set容器中的所有的元素
empty()		判断set容器是否为空
max_size()	返回set容器可能包含的元素最大个数
size()		返回当前set容器中的元素个数
erase(it)	删除迭代器指针it处元素
insert(a)	插入某个元素 

当set集合中的元素为结构体时，该结构体必须实现运算符‘<’的重载


##Map
C++中map提供的是一种键值对容器，里面的数据都是成对出现的，每一对中的第一个值称为关键字(key)，每个关键字只能在map中出现一次；第二个称为该关键字的对应值。

//头文件
#include<map>

map<int,string> ID_Name;

//使用{}赋值是从C++11开始的，因此编译器版本过低时会报错，如visual studio 2012
map<int,string> ID_Name = {{2015,"Jim"}, {2016,"Tom"}, {2017,"Bob"}};

###变量声明
map<int,string> myMap;

###插入操作
ID_Name[2015] = "Tom";
ID_Name.insert(std::pair<int,string>(2018,"Tom"));

###取值操作
Map中元素取值主要有at和[]两种操作，at会做下标检查，而[]不会。
cout<<ID_Name[2016].c_str()<<endl;
ID_Name.at(2016) = "Bob";

###查询容量
//查询map是否为空
bool empty();
//查询map中键值对的数量
size_t size();
//查询map所能包含的最大键值对数量，和系统的应用库有关。
//此外，这并不意味着用户一定可以存这么多，很可能还没达到就已经开始开辟内存失败了
size_t max_size();
//查询关键字为key的元素的个数，在map里面结果非0即1
size_t count(const Key& key) const;

###迭代器

###删除交换
//删除迭代器指向位置的键值对，并返回一个指向下一元素的迭代器
iterator erase(iterator pos)
//删除一定范围内的元素，并返回一个指向下一元素的迭代器
iterator erase(const_iterator first, const_iterator last);
//根据Key来进行删除，返回删除的元素数量，在map里结果非0即1
size_t erase(const key_type& key);
//清空map，清空后的size为0
void clear();
//就是两个map的内容互换
void swap(map& other);

###顺序比较
比较两个关键字在map中的位置的先后
key_compare key_comp() const;
示例：
map<char,int> mymap;
map<char,int>::key_compare mycomp = mymap.key_comp();
mymap['a'] = 100;
mymap['b'] = 200;
mycomp('a', 'b');    //a排在b前面，因此返回结果为true

###查找
//关键字查询，找到则返回指向该关键字的迭代器，否则返回指向end的迭代器
//根据map的类型，返回的迭代器为iterator或者const_iterator
iterator find(const key_type& k);
const_iterator find(const key_type& k) const;
举例：
std::map<char,int> mymap;
std::map<char,int>::iterator it;

mymap['a'] = 50;
mymap['b'] = 100;
mymap['c'] = 150;
mymap['d'] = 200;

it = mymap.find('b');
if(it != mymap.end())
	mymap.erase(it);    //b被成功删除

###操作符
operator: == != < <= > >=
注意：对于==运算符，只有键值对以及顺序完全相等才算成立。

##unordered_map
#include<unordered_map>
unordered_map<int,string> ID_Name;

##c++中map与unordered_map的区别
###内部实现机理
map： map内部实现了一个红黑树，该结构具有自动排序的功能，因此map内部的所有元素都是有序的，红黑树的每一个节点都代表着map的一个元素，因此，对于map进行的查找，删除，添加等一系列的操作都相当于是对红黑树进行这样的操作，故红黑树的效率决定了map的效率。
unordered_map: unordered_map内部实现了一个哈希表，因此其元素的排列顺序是杂乱的，无序的
优缺点以及适用处

map 
优点： 
有序性，这是map结构最大的优点，其元素的有序性在很多应用中都会简化很多的操作
红黑树，内部实现一个红黑书使得map的很多操作在lgnlgn的时间复杂度下就可以实现，因此效率非常的高
缺点： 
空间占用率高，因为map内部实现了红黑树，虽然提高了运行效率，但是因为每一个节点都需要额外保存父节点，孩子节点以及红/黑性质，使得每一个节点都占用大量的空间
适用处，对于那些有顺序要求的问题，用map会更高效一些
unordered_map 
优点： 
因为内部实现了哈希表，因此其查找速度非常的快
缺点： 
哈希表的建立比较耗费时间
适用处，对于查找问题，unordered_map会更加高效一些，因此遇到查找问题，常会考虑一下用unordered_map

###结果分析
运行效率方面：unordered_map最高，hash_map其次，而map效率最低
占用内存方面：hash_map内存占用最低，unordered_map其次，而map占用最高

###什么时候需要用hash_map，什么时候需要用map?
总体来说，hash_map 查找速度会比map快，而且查找速度基本和数据数据量大小，属于常数级别;而map的查找速度是log(n)级别。并不一定常数就比log(n)小，hash还有hash函数的耗时，明白了吧，如果你考虑效率，特别是在元素达到一定数量级时，考虑考虑hash_map。但若你对内存使用特别严格，希望程序尽可能少消耗内存，那么一定要小心，hash_map可能会让你陷入尴尬，特别是当你的hash_map对象特别多时，你就更无法控制了，而且hash_map的构造速度较慢。
现在知道如何选择了吗？权衡三个因素: 查找速度, 数据量, 内存使用。

##Queue
boost::lockfree::queue<int, boost::lockfree::fixed_sized<false>, boost::lockfree::capacity<10000>> m_queue;    //boost库无锁队列的定义
m_queue.push(<int>);	//入队
m_queue.pop(<int>);	//出队
front();				//返回第一个元素
back();					//返回最后一个元素
empty();				//如果队列为空则返回true
size();					//返回队列中元素的个数










































