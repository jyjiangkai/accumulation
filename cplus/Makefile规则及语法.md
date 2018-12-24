##Makefile

###Makefile规则
首先我们通过实例来理解makefile的规则
创建两个文件：a.c b.c,在a中调用b函数func_b
```
//a.c
#include <stdio.h>
int main(){
    func_b();
    return 0;
}
//b.c
#include <stdio.h>
void func_b(){
    printf("This is B\n");
}
```
这时候我们使用：gcc -o test a.c b.c 可以生成test可执行文件  执行./test输出This is B，程序执行成功   
我们知道由一个.c文件到可执行文件中间经历了4个步骤：预处理->编译->汇编->链接。   
虽然这个命令很简单，但是它在执行时会对所有的文件都执行4步，当文件很多的时候，就会带来很大的性能问题。对这些文件我们应该分别处理，最后在链接。类似以下方式：   
步骤一：gcc -c -o a.o a.c  //处理a文件   
步骤二：gcc -c -o b.o b.c  //处理b文件   
步骤三：gcc -o test a.o b.o  //最终链接   
如果我们只修改了a文件，则只需要执行1、3步骤即可，直接使用旧的b.o文件，这样就省去一些没必要的步骤。   
那么问题是我们该如何判断文件是否修改过呢？其实makefile是根据 .c文件是否比 .o文件新（生成或修改时间） 如果新，则重新执行编译，再重新生成test程序   

* 编写一个makefile：
   * 规则：   
     目标：依赖1 依赖2 ...   
     [TAB] 命令
      
makefille文件：
```
#如果b.o、a.o比test文件新，则执行下面的命令生成test
test:a.o b.o 
    gcc -o test a.o b.o
#如果a.c比a.o文件新，则执行下面的命令生成a.o
a.o : a.c
    gcc -c -o a.o a.c
#如果b.c比b.o文件新，则执行下面的命令生成b.o
b.o : b.c
    gcc -c -o b.o b.c
```

这个makefile文件包含三条基本规则：   
第一次执行make的时候，执行第一个规则，生成test文件，但是需要依赖a.o与b.o，但是没有这两个文件，于是查找其他规则有没有生成这两个的文件的规则，如果有其则执行命令，生成这两个文件，最后执行第一个规则的命令生成test文件。   
第二次执行make的时候，发现a.o与b.o都没有比test文件新，则不执行任何命令   
当我们修改a.c文件时，再次执行make命令，这时发现a.c比a.o新，则执行规则2的命令生成新的a.o，而b.o没有变化，则执行规则1的命令生成test，所以只执行了1+2步骤。   

* 总结下makefile规则：   
目标：依赖1 依赖2 ...   
[TAB] 命令   
当 "目标文件"不存在或某个依赖文件比目标文件新   
则：执行"命令"   

###Makefile基本语法
####通配符
如果我们有大量的.c文件，我们要为每一个.c生成.o文件写一条规则，显然是不可能的，makefile使用通配符解决这种重复性的规则：
```
test : a.o b.o
    gcc -o test a.o b.o
%.o : %.c
    gcc -c -o $@ $<
```
%通配符，当想生成a.o文件时发现与%.o : %.c匹配，则将%替换为a，也就是a.o : a.c，则会执行规则的命令。   
命令中的$@只目标文件，$<表示第一个依赖。   
这样我们增加多个c.c文件时只需要修改第一条规则：    
```
test : a.o b.o c.o
    gcc -o test a.o b.o c.o
%.o : %.c
    gcc -c -o $@ $<
```
使用通配符减少了makefile的书写。   
其实我们还可以简化书写，使用$^来代替所有的依赖：   
```
test : a.o b.o c.o
    gcc -o test $^
%.o : %.c
    gcc -c -o $@ $<
```
% 表示通配符   
$@ 表示目标   
$< 表示第1个依赖文件   
$^ 表示所有依赖文件   

####假想目标：.PHONY
添加一个清除文件命令：    
```
test : a.o b.o c.o
    gcc -o test $^
%.o : %.c
    gcc -c -o $@ $<
clean：
    rm *.o test
```
使用make clean命令，则会清除所有.o文件与test文件   
如果我们的目录中有一个名为clean的文件，则执行make clean的时候会失败，因为根据makefile的语法，检查clean文件已经是最新的，不会执行这个规则。解决方法就是使用假想目标：   
```
test : a.o b.o c.o
    gcc -o test $^
%.o : %.c
    gcc -c -o $@ $<
clean：
    rm *.o test
.PHONY： clean
```
当将clean定义为假想目标后，就不会去判断clean文件了，make ckean 成功执行删除操作   
**<font color=#DC143C>注：makefile使用：make [目标] 若无目标，默认执行第一个目标</font>**   

####即时变量、延时变量
* 即时变量 A := xxx  # A的值在定义时立即确定   
* 延时变量 B  = xxx  # B的值在使用到的时候才确定    
* 延时变量 C ?=xxx  #表示如果是第一次定义C才起效，如果已经定义过则忽略    
* 附加变量 D +=xxx  #具体是即时变量还是延时变量，取决与D的定义   

```
#$()表示引用变量
A := $(C)
B  = $(C)
C  = abc

all:
    @echo A = $(A)
    @echo B = $(B)
```
输出：   
A =   
B = abc   
由于A在定义的时候没有C变量，所有A为空。B是运行时赋值，makefile会先加载整个文件，再运行，所以此时C已经有值，B = abc了   
```
A := $(C)
B = $(C)
C = abc

D   = zhang 
D ?= yang

all:
    @echo A = $(A)
    @echo B = $(B)
    @echo D = $(D)

C += 123
```
输出：   
A =    
B = abc 123    
D = zhang    
由于D已经定义过了，所以 D ?= yang 被忽略   
我们还可以再执行make的时候传入变量，如：   
```
D ?= yang

all:
    @echo D = $(D)
```
执行 make D=zhang,输出：D = zhang 而D ?= yang又被忽略。    
