##C++ 常用字符串操作函数

###获取字符串长度
常用函数：size()、sizeof()、strlen()、length()
C/C++ strlen(str)和str.length()和str.size()都可以求字符串长度。其中str.length()和str.size()是用于求string类对象的成员函数，
strlen(str)是用于求字符数组的长度，其参数是char*，sizeof(str)用于求所占总空间的字节数。
第一种：strlen(char*)函数求的是字符串的实际长度，它求得方法是从开始到遇到第一个'\0',如果你只定义没有给它赋初值，这个结果是不定的，它会从aa首地址一直找下去，直到遇到'\0'停止。
第二种：sizeof(),求所占总空间的字节数。
例如：
char[] a={'a','b','c'};
sizeof(a)的值应该为3。
char[] b={"abc"};
sizeof(b)的值应该是4。
若string str={'a','b','c','\0','X'};
那么sizeof(str)为5，strlen(str)为3。

###字符串操作函数
####内存复制
######memcpy_s
######memmove_s

####内存初始化 
######memset_s

####字符串复制
######strncpy_s

####字符串连接
######strncat_s

####字符串分割
######strtok_s
######strsep

####其它
######strncmp
函数原型：int strncmp(const char* str1, const char* str2, size_t num);
参数说明：str1为第一个要比较的字符串，str2为第二个要比较的字符串，num为指定的str1与str2的比较的字符数。
如果返回值 < 0，则表示 str1 小于 str2。
如果返回值 > 0，则表示 str2 小于 str1。
如果返回值 = 0，则表示 str1 等于 str2。

######strchr
函数原型：extern char *strchr(char *str,char character)
参数说明：str为一个字符串的指针，character为一个待查找字符。
函数功能：从字符串str中寻找字符character第一次出现的位置。
返回说明：返回指向第一次出现字符character位置的指针，如果没找到则返回NULL。
其它说明：还有一种格式char *strchr( const char *string, int c )，这里字符串是以int型给出的。

######c_str
c_str()函数返回一个指向正规C字符串的指针常量, 内容与本string串相同。这是为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象
的成员函数c_str()把string对象转换成c中的字符串样式。

######find
size_type find(const char *str, size_type index, size_type length);
返回str在字符串中第一次出现的位置（从index开始查找，长度为length）。如果没找到就返回string::npos。
示例：
string str = "abcdefg";
unsigned int loc = str.find("abc", 0, str.length());
if( loc != string::npos )
    cout << "Found abc at" << loc << endl;
else
    cout << "Didn't find abc" << endl;
    
str.find("abcde", 2, 3);
//取abcde得前3个字符(abc)参与匹配，相当于str.find("abc", 2);
str.find("abcde", 0, 5) << endl;
//str.find("abcde", 0)
str.find("abcde", 0, 6) << endl;
//第3个参数超出第1个参数的长度时，返回string::npos

######strnlen
strnlen比strlen多一个参数，就是数据总体长度，当检测到字符串长度比数据总体长度还长（就是给定数据长度范围内没有结束符'\0'）时就会终止扫描。strnlen这个函数一般用于检测不可信的数据（如网络数据），因为这种数据中可能没有'\0'，这时如果用strlen的话会一直扫描无法停止（直到越界触碰到无效内存），而strnlen限制住了扫描范围所以不会出事。
strnlen(const char *str, size_t maxsize)

######stringstream
C++引入了ostringstream、istringstream、stringstream这三个类，要使用他们创建对象就必须包含sstream.h头文件。
istringstream类用于执行C++风格的串流的输入操作。 
ostringstream类用于执行C风格的串流的输出操作。 
stringstream类同时可以支持C风格的串流的输入输出操作。
示例：
//stringstream通常是用来做数据转换的。  
//int 与 string的转换  
std::stringstream m_sstream;  
std::string result;  
int i=1000;  
m_sstream<<i;  
m_sstream>>result;  
std::cout<<result<<"\n";  

m_sstream.clear();   
m_sstream<<“6666”;
std::cout<<m_sstream.str()<<std::endl;  

m_sstream.clear();  
char res[8];  
m_sstream<<8888;  
m_sstream>>res;  
std::cout<<res<<std::endl;  

m_sstream.clear();  
int first,second;  
m_sstream<<"666";  
m_sstream>>first;  
std::cout<<first<<std::endl;  

m_sstream.clear();  
m_sstream<<true;  
m_sstream>>second;  
std::cout<<second<<std::endl;  
