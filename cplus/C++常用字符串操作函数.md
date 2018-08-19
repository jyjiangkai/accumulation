##C++ 常用字符串操作函数

###常用获取字符串长度函数
常用函数：size()、sizeof()、strlen()、length()、strnlen()
C/C++ strlen(str)和str.length()和str.size()都可以求字符串长度。其中str.length()和str.size()是用于求string类对象的成员函数，
strlen(str)是用于求字符数组的长度，其参数是char*，sizeof(str)用于求所占总空间的字节数。
####size()
str.size()用于求string类对象长度的成员函数
####length()
str.length()用于求string类对象长度的成员函数
####strlen()
strlen(char*)函数求的是字符串的实际长度，它求得方法是从开始到遇到第一个'\0',如果你只定义没有给它赋初值，这个结果是不定的，它会从首地址一直找下去，直到遇到'\0'停止。
####strnlen()
strnlen比strlen多一个参数，就是数据总体长度，当检测到字符串长度比数据总体长度还长（就是给定数据长度范围内没有结束符'\0'）时就会终止扫描。strnlen这个函数一般用于检测不可信的数据（如网络数据），因为这种数据中可能没有'\0'，这时如果用strlen的话会一直扫描无法停止（直到越界触碰到无效内存），而strnlen限制住了扫描范围所以不会出事。
strnlen(const char *str, size_t maxsize)
####sizeof()
求所占总空间的字节数。
例如：
char[] a={'a','b','c'};
sizeof(a)的值应该为3。
char[] b={"abc"};
sizeof(b)的值应该是4。
char[] str={'a','b','c','\0','X'};
sizeof(str)为5
strlen(str)为3

###常用字符串操作函数
####内存复制
######memcpy_s
errno_t memcpy_s(void *dest, size_t numberOfElements, const void *src, size_t count);
memcpy_s指的是c和c++使用的内存拷贝函数，memcpy_s函数的功能是从源src所指的内存地址的起始位置开始拷贝count个字节到目标dest所指的内存地址的起始位置中。
dest-目标地址
numberOfElenments-目标地址的字符个数(可使用sizeof获取)
src-源地址
count-需要拷贝的源字符个数（可使用sizeof获取）。
注意：numberOfElenments必须大于等于count，否则拷贝将出现中断。

######memmove_s
errno_t memmove_s(void *dest, size_t numberOfElements, const void *src, size_t count);
memmove_s用于从src拷贝count个字节到dest，如果目标区域和源区域有重叠的话，memmove_s能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中。但复制后src内容会被更改。但是当目标区域与源区域没有重叠则和memcpy_s函数功能相同。但memcpy_s比memmove_s的速度要快一些
dest-目标地址
numberOfElenments-目标地址的字符个数(可使用sizeof获取)
src-源地址
count-需要拷贝的源字符个数（可使用sizeof获取）。

####内存初始化 
######memset_s
errno_t memset_s(void *s, rsize_t smax, int c, rsize_t n);
memset_s是计算机中C/C++语言初始化函数。将s所指向的某一块内存中的后n个字节的内容全部设置为ch指定的ASCII值，第一个值为指定的内存地址，块的大小由第三个参数指定，这个函数通常为新申请的内存做初始化工作，其返回值为s。第二个参数smax用于保证最多只会初始化s前smax个参数。

####字符串复制
######strncpy_s
errno_t strncpy_s(char *dest, size_t n, char *src, int size_t n);
把src所指向的字符串中以src地址开始的前n个字节复制到dest所指的数组中，并返回被复制后的dest。
注意：memcpy_s用来在内存中复制数据，由于字符串是以“\0”结尾的，所以对于在数据中包含“\0”的数据只能用memcpy_s。strncpy_s和memcpy_s很相似，只不过它在一个终止的空字符处停止。

######substr
string substr(size_type _Off = 0,size_type _Count = npos) const;

string s = "0123456789";
string sub1 = s.substr(5); //只有一个数字5表示从下标为5开始一直到结尾：sub1 = "56789"
string sub2 = s.substr(5, 3); //从下标为5开始截取长度为3位：sub2 = "567"

####字符串连接
######strncat_s
errno_t strncat_s(char *dest, size_t n, char *src, int size_t n);
把src所指字符串的前n个字符添加到dest结尾处。src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。返回指向dest的指针。

######boost::algorithm::join
函数boost::algorithm::join()接受一个字符串的容器作为第一个参数，根据第二个参数将这些字符串连接起来。
std::vector<std::string> v; 
v.push_back("Hello"); 
v.push_back("World");
std::cout << boost::algorithm::join(v, " ") << std::endl;

####字符串分割
######strtok
char *strtok (char *str, const char * delimiters);
参数：str，待分割的字符串（c-string）；delimiters，分割符字符串。
该函数用来将字符串分割成一个个片段。参数str指向欲分割的字符串，参数delimiters则为分割字符串中包含的所有字符。当strtok()在参数s的字符串中发现参数delimiters中包涵的分割字符时，则会将该字符改为\0 字符。
实例：
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main()
{
    char str[]="ab,cd,ef";
    char *ptr;
    printf("before strtok:  str=%s\n",str);
    printf("begin:\n");
    ptr = strtok(str, ",");
    while(ptr != NULL){
        printf("str=%s\n",str);
        printf("ptr=%s\n",ptr);
        ptr = strtok(NULL, ",");
    }
    system("pause");
    return 0;
}


######strtok_s
char *strtok_s(char *strToken, const char *strDelimit, char **buf);
strtok_s是windows下的一个分割字符串安全函数。
这个函数将剩余的字符串存储在buf变量中，而不是静态变量中，从而保证了安全性。
实例：
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

int main()
{
	char str[] = "aa,bb,cc,dd";
	char *p;
	char *buf;
	p[0]=strtok_s(str, ",", &buf);
	int i = 0;
	while (p[i])
	{
		i++;
		p[i] = strtok_s(NULL, ",", &buf);
	}
	p[i] = "\0";
	for (int i = 0; i < 4; i++)
	{
		cout << p[i] << endl;
	}

	system("pause");
    return 0;
}

######strtok_r
char *strtok_r(char *str, const char *delim, char **saveptr);
strtok_r函数是linux下分割字符串的安全函数。
该函数也会破坏带分解字符串的完整性，但是其将剩余的字符串保存在saveptr变量中，保证了安全性。
实例：
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main()
{
    char str[]="ab,cd,ef";
    char *ptr;
    char *p;
    printf("before strtok:  str=%s\n",str);
    printf("begin:\n");
    ptr = strtok_r(str, ",", &p);
    while(ptr != NULL){
        printf("str=%s\n",str);
        printf("ptr=%s\n",ptr);
        ptr = strtok_r(NULL, ",", &p);
    }
    return 0;
}

######strsep
char *strsep(char **stringp, const char *delim);
strsep函数是linux下分割字符串的函数。
分解字符串为一组字符串，此函数会改变字符串，返回开始到第一个出现delim的地方并在返回的字符串末尾自动加‘/0’，如果没有delim，则返回NULL，此函数比strtok好处是接受空字符串并且是线程安全的。
实例：
#include <stdio.h>
#include <string.h>

void main()
{
    char str[]="Number=100&maxMtu=200";
    char *name,*value,*next;
    int i;

    value=str; //使指针value 指向字符串str;

    for(i=0 ;i<2 ;i++)
    {
		// 第一次执行时
        name = strsep(&value,"="); // 以"="分割字符串,这时strsep函数返回值为 "Number",即"="号之前的字符串
        next =value; // 这时指针value指向"="号后面的字符串,即"100&maxMtu=200"
        value=strsep(&next,"&"); // 这时通过"&"分割字符串,返回值为100,next指 向"maxMtu=200"
        printf(" name= %s/n",name); //打印出一轮分割后name的值
        printf("value= %s/n",value);
        value=next;
    }
}

######split
自定义split函数
int split(string &str, string &sep, vector<string> &vec)
{
    size_t pos = 0, found = 0;
    while ( found != string::npos )
    {
        found = str.find(sep, pos);
        if (string(str, pos, found - pos) == "")
        {
            return 1;
        }
        vec.push_back(string(str, pos, found - pos));
        pos = found + 1;
    }
    return 0;
}
str：输入字符串
sep：分隔字符串
vec：用于保存分割后的字符串片段

######boost::split
boost库提供了split函数，可以直接使用
split(vector<string> &vec, string &str, string &sep, token_compress_mode_type eCompress=token_compress_off);
例：
boost::split(vec, str, boost::is_any_of(","));
以","字符分割字符串s，并将结果保存在容器vec中
boost::split(vec, str, boost::is_any_of(",;"));
以",;"中任意字符分割字符串s，并将结果保存在容器vec中
boost::split(vec, str, boost::is_any_of(" ,;"), boost::token_compress_on);
以" ,;"中任意字符分割字符串str，并将结果保存在容器vec中，若最后一个参数配置为token_compress_on，则表示分割后剔除空行

######boost::algorithm::split
将一个字符串拆分为一个字符串容器
例：
std::vector<std::string> v; 
boost::algorithm::split(v, str, boost::algorithm::is_space());  //以空格作为分隔符
boost::algorithm::split(v, str, boost::algorithm::is_digit());  //以数字作为分隔符
boost::algorithm::split(v, str, boost::algorithm::is_any_of(",;"));  //以",;"中任意字符作为分隔符

####字符串比较
######strncmp
函数原型：int strncmp(const char* str1, const char* str2, size_t num);
参数说明：str1为第一个要比较的字符串，str2为第二个要比较的字符串，num为指定的str1与str2的比较的字符数。
如果返回值 < 0，则表示 str1 小于 str2。
如果返回值 > 0，则表示 str2 大于 str1。
如果返回值 = 0，则表示 str1 等于 str2。

######boost::algorithm::starts_with
比较字符串头部是否相等，若相等，返回1，否则返回0。
boost::algorithm::starts_with(str, "Hello");

######boost::algorithm::ends_with
比较字符串尾部是否相等，若相等，返回1，否则返回0。
boost::algorithm::ends_with(str, "World");

######boost::algorithm::contains
比较字符串是否包含子串，若包含，返回1，否则返回0。
boost::algorithm::contains(str, "llo");

####字符串查找
######strchr
函数原型：extern char *strchr(char *str,char character)
参数说明：str为一个字符串的指针，character为一个待查找字符。
函数功能：从字符串str中寻找字符character第一次出现的位置。
返回说明：返回指向第一次出现字符character位置的指针，如果没找到则返回NULL。
其它说明：还有一种格式char *strchr( const char *string, int c )，这里字符串是以int型给出的。
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

######rfind
rfind()与find()很相似，差别在于查找顺序不一样，rfind()是从指定位置起向前查找，直到串首。

######find_first_of
在源串中从位置pos起往后查找，只要在源串中遇到一个字符，该字符与目标串中任意一个字符相同，就停止查找，返回该字符在源串中的位置；若匹配失败，返回npos。

######find_last_of
该函数与find_first_of()函数相似，只不过查找顺序是从指定位置向前

######find_first_not_of
在源串中从位置pos开始往后查找，只要在源串遇到一个字符，该字符与目标串中的任意一个字符都不相同，就停止查找，返回该字符在源串中的位置；若遍历完整个源串，都找不到满  足条件的字符，则返回npos。

######find_last_not_of
find_last_not_of()与find_first_not_of()相似，只不过查找顺序是从指定位置向前

######boost::algorithm::find_first
从字符串头部查找子串
boost::algorithm::find_first(str, "Hello");

######boost::algorithm::find_last
从字符串尾部查找子串
boost::algorithm::find_last(str, "Hello");

######boost::algorithm::find_nth
从字符串头部查找指定子串
boost::algorithm::find_nth(str, "Hello", 0);

####字符串大小写切换
######toupper
int toupper(int c);  //转换单个字符

######tolower
int tolower(int c);  //转换单个字符

######strupr
char* strupr(char* str);  //转换char*
errno_t _strupr_s(char *str, size_t numberOfElements);  //安全函数

######strlwr
char* strlwr(char* str);  //转换char* 
errno_t _strlwr_s(char *str, size_t numberOfElements);  //安全函数

######transform和tolower及toupper结合，转换string
transform(str.begin(), str.end(), str.begin(), toupper);
transform(str.begin(), str.end(), str.begin(), tolower);

######boost::algorithm::to_upper_copy
转换一个字符串为大写形式，不改变入参，返回值为字符串
boost::algorithm::to_upper_copy(str);  //不改变字符串str

######boost::algorithm::to_lower_copy
转换一个字符串为小写形式，不改变入参，返回值为字符串
boost::algorithm::to_lower_copy(str);  //不改变字符串str

######boost::algorithm::to_upper
转换一个字符串为大写形式，改变入参，无返回值
boost::algorithm::to_upper(str);  //改变字符串str

######boost::algorithm::to_lower
转换一个字符串为小写形式，改变入参，无返回值
boost::algorithm::to_lower(str);  //改变字符串str

####字符串删除
######erase
string& erase(size_t pos = 0, size_t n = npos);
erase(pos,n); 删除从pos开始的n个字符，比如erase(0,1)就是删除第一个字符，会修改原字符串

######boost::algorithm::erase_first_copy
在字符串头部删除若干字符
boost::algorithm::erase_first_copy(str, "ss");  //在字符串str头部删除字符串“ss” 

######boost::algorithm::erase_last_copy
在字符串尾部删除若干字符
boost::algorithm::erase_last_copy(str, "ss");  //在字符串str尾部删除字符串“ss”

######boost::algorithm::erase_nth_copy
删除字符串中指定字符
boost::algorithm::erase_nth_copy(str, "ss", 0);  //删除字符串str第一个字符串“ss”
boost::algorithm::erase_nth_copy(str, "ss", 1);  //删除字符串str第二个字符串“ss”

######boost::algorithm::erase_all_copy
删除字符串中所有指定字符
boost::algorithm::erase_all_copy(str, "ss");  //删除字符串str中所有字符串“ss”

######boost::algorithm::erase_head_copy
删除字符串头部几个字符
boost::algorithm::erase_head_copy(str, 3);  //删除字符串str头部3个字符

######boost::algorithm::erase_tail_copy
删除字符串尾部几个字符
boost::algorithm::erase_tail_copy(str, 5);  //删除字符串str尾部5个字符

####字符串替换
######boost::algorithm::replace_first_copy
替换字符串中第一个子串
boost::algorithm::replace_first_copy(s, "l", "L");  //替换字符串str中第一个"l"为"L"

######boost::algorithm::replace_last_copy
替换字符串中最后一个子串
boost::algorithm::replace_last_copy(s, "l", "L");  //替换字符串str中最后一个"l"为"L"

######boost::algorithm::replace_nth_copy
替换字符创中指定子串
boost::algorithm::replace_nth_copy(s, "l", 0, "L");  //替换字符串str中第一个"l"为"L"
boost::algorithm::replace_nth_copy(s, "l", 1, "L");  //替换字符串str中第二个"l"为"L"

######boost::algorithm::replace_all_copy
替换字符串中所有子串
boost::algorithm::replace_all_copy(s, "l", "L");  //替换字符串str中所有"l"为"L"

######boost::algorithm::replace_head_copy
替换字符串头部指定长度字串
boost::algorithm::replace_head_copy(s, 3, "###");  //替换字符串str头部3个字符为"###"

######boost::algorithm::replace_tail_copy
替换字符串尾部指定长度字串
boost::algorithm::replace_tail_copy(s, 5, "###");  //替换字符串str尾部5个字符为"###"

####其他
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
