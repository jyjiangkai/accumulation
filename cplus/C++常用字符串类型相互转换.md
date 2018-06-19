##C++ char[], char*, const char*, string的相互转换

1.string转const char*
string s = "abc";
const char* c_s = s.c_str();

2.const char*转string
直接赋值即可
const char* c_s = "abc";
string s(c_s);
string s = c_s;

3.string转char*
string s = "abc";
char* c;
int len = s.length();
c = new char[len+1];
strncpy_s(c, len, s.c_str(), len);

4.char*转string
char* c = "abc";
string s(c);
string s = c;

5.const char*转char*
const char* cpc = "abc";
int len = strlen(cpc);
char* pc = new char[len+1];
strncpy_s(pc, len, cpc, len);

6.char[]转string
char ch[] = "ABCDEF"
string str(ch);
string str = ch;

7.string转char[]
char buf[10];
string str = "ABCDEF";
int len = str.length();
memcpy_s(buf, len, str.c_str(), len);	//或strncpy_s(buf, len, str.c_str(), len);
buf[len] = '\0';
