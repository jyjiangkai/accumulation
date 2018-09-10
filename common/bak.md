====<<面试题目>>===
##O(1)复杂度入栈出栈设计，全复杂度为1
使用链式存储栈即可

##go-micro
应用了go-micro的可插拔理念

##beego
应用了beego的web化框架，开发了一些可视化界面工具。

##给出一个链表，能用快排进行排序吗?(不能开辟额外空间)
ok

##用过网络编程吗？
用golang写过网络编程

##手写程序，给出一颗完全二叉树，现在要插入下一个节点，怎么实现，时间复杂度多少？
typedef struct node
{
	int key;
	struct node* lchild;
	struct node* rchild;
}BSTree;

##有两个字符串，例如S1=abcbcd，S2=abcd，求S2是否包含在S1中，如果有，则输出每种结果对应的下标，例如S1=abcbcd，S2=abcd时，下标的组合：{0125,0145,0345} 

##现有32G的数据，但是只有8G的内存，怎么对这些数进行排序
1TB数据使用32GB内存如何排序 
1、把磁盘上的1TB数据分割为40块（chunks），每份25GB。（注意，要留一些系统空间！） 
2、顺序将每份25GB数据读入内存，使用quick sort算法排序。 
3、把排序好的数据（也是25GB）存放回磁盘。 
4、循环40次，现在，所有的40个块都已经各自排序了。（剩下的工作就是如何把它们合并排序！） 
5、从40个块中分别读取25G/40=0.625G入内存（40 input buffers）。 
6、执行40路合并，并将合并结果临时存储于2GB基于内存的输出缓冲区中。当缓冲区写满2GB时，写入硬盘上最终文件，并清空输出缓冲区；当40个输入缓冲区中任何一个处理完毕时，写入该缓冲区所对应的块中的下一个0.625GB，直到全部处理完成。

##有一个大文件，里面存着电话号码，现给出一个电话号码，判断该电话号码在不在该文件中

##现有一个大文件，存着电话号码该该电话号码对应的话费记录，选出前10000个消费最高的电话号码（NlogK）
step1：根据内存大小，对大文件进行分块，然后逐个将大文件读入内存。
step2：创建hash_map,遍历读入内存中的文件。对于每条记录，先在hash_map中搜索，若有，将hash_map中记录count+1，若没有，插入hash_map
step3:在这个hash_map中，找出count最大的数据

##内存泄漏常用定位手段
一个非常强大的工具valgrind
Valgrind的使用请见手册http://valgrind.org/docs/manual/manual.html

##多个线程顺序执行，如果有一个线程运行超时了，那么如何保证超时线程不影响其他线程执行？
使用Thread.join(超时时长)。

##string-stringbuffer-stringbuilder区别
String 字符串常量
StringBuffer 字符串变量（线程安全）
StringBuilder 字符串变量（非线程安全）

##熟悉哪些linux的命令，如何查找出内存占用率高的代码？
首先使用top命令查看内存占用较高的进程，通过进程判断是哪部分代码内存占用过高
或者ps -aux | sort -k4nr | head -N1

##shell脚本取出log中访问最多的十个地址，常用shell命令
cat file | grep "url" | caculate

##一个数组中有一个元素只出现一次，其他元素都出现两次，如何找到这个元素
int singleNumber(int nums[]) 
{
  int res = nums[0];
  for (int i = 1; i < nums.length; i++)
    res = res^nums[i];

  return res;
}

##一个数组中有一个元素只出现一次，其他元素都出现三次，如何找到这个元素
int singleNumber(int nums[]) {
    int result = 0;
    for (int i = 0; i < 32; i++) {
        int sum = 0;
        for (int j = 0; j < 13; j++) {
            sum += (nums[j] >> i) & 1;
        }
        result |= (sum % 3) << i;
    }
    return result;
}
   
##数组中所有元素出现两次，其中有两个元素只出现一次，找出这两个元素
public static int[] geTwoEle(int[] res){
        if(res.length==2)return res;
        int []val=new int[2];
        int temp=res[0];
        for(int i=1;i<res.length;i++){
            temp^=res[i];
        }
        //find the leftmost pos where two element is different
        int i=0;
        while(temp!=0)temp>>=++i;
        i--;

        int left[]=new int[res.length];
        int right[]=new int[res.length];
        int lefti=0;
        int righti=0;
        //split val array into two arrays according to the different pos
        for(int j=0;j<res.length;j++){
            if(((res[j]>>i)&1)==0){
                left[lefti++]=res[j];
            }else{
                right[righti++]=res[j];
            }
        }
        //in left and right array only one element is different,use ^ to find the element
        val[0]=left[0];
        for(int j=1;j<lefti;j++){
            val[0]^=left[j];
        }

        val[1]=right[0];
        for(int j=1;j<righti;j++){
            val[0]^=right[j];
        }

        return val;
}

https://blog.csdn.net/guoyuguang0/article/details/51034572

##编程题，高阶函数的实现
高阶函数就是把函数作为参数或者返回值的一类函数

##指定一个场景，考察算法选择和设计
如何实现可以获取最小值的栈？
一般方法：使用一个变量来保存栈中的最小值，如果最小值被pop出去了，就没有记录了，不可取。时间复杂度为o(n)，空间复杂度o(1)
高级方法：申请一个辅助栈，辅助栈中就保存最小值。时间复杂度o(1)，空间复杂度o(n)
究极方法：申请一个辅助栈，辅助栈中保存最小值的索引，当push插入元素时，与最小值相同元素的时候就不需要变更辅助栈，而pop出栈的时候，pop出的元素的索引如果和辅助栈中保存的索引不匹配的话，则辅助栈不出栈。这样在保持时间复杂度不变的情况下，减少了空间复杂度。

##两个链表做加法
如
链表1：1->3->4
链表2：   2->6
相加后得到：
链表3：1->5->0 （不考虑进位）
链表4：1->6->0 （考虑进位）

##找出二叉树和无向图任意两个节点间的最大距离（代码实现）
https://blog.csdn.net/beitiandijun/article/details/41978369
基本思想：定义两个vector分别保存二叉树中的节点(vec1和vec2)。从根节点开始向两个待查找节点进行遍历，把第一个节点经过的节点保存在vec1中，把第二个节点经过的节点保存在vec2中，然后遍历对比vec1和vec2，计算出两个vec中的公共节点数，然后用两个vec中的总节点数减去两倍公共节点数即为代查找的两个节点间的最大距离。

##使用stack实现queue（代码实现）

##介绍自己比较擅长什么，举两个例子（技术方面）

#算法题
##有k个已排序数组，元素可能会重复，求所有元素的中位数和时间复杂度
int selectMid(const vector<vector<int>> &nums)

#基础题
##MySQL的底层数据结构是什么？
MySQL的底层数据结构是B树、B+树

##B+数和红黑树的区别？为啥不用红黑树？如果索引是全内存，不写磁盘B+数还有优势吗？
二叉树、红黑树的树的高度是不限的，并且每个节点只有一条数据,那么要查找某些数据的时候会执行很多次IO
如果索引是全内存就没有优势了，因为磁盘的特点是IO读写比数据读取慢得多，所以需要尽可能减少IO次数来提升效率，同等数据规模下，二叉树比B树的IO次数多得多，如果索引都是内存的话，B树就没有这些优势了，而且会占据更多的内存。

##++i，i++，i += 1，三者有什么区别？（我算法题里面有用到这样的写法，所以估计是顺便问的）

#项目相关

##爬取内容相关的问题，碰到什么问题，怎么解决？
创建一个hashmap，对爬取到的内容计算hash值，并保存在hashmap中，下次爬取到内容时，首先在hashmap中找一下是否之前已经爬取过。

##跳棋AI设计
##电商促销功能架构设计

#笔试（图片见附件，求能装多少水）
##横平竖直的多边形，求周长K等分点的坐标
##只使用单向链表，做十进制加法
##用一组数来表述连续的柱子的高度，求下雨后，这些柱子能接多少雨水？
如[0,1,0,1,2,1,0,3,1,0]，下雨后，雨水数量为4。黑色为柱子，蓝色为雨水，见附件图

#一面
##Http code 502,504的含义，如何truble shooting

##分布式系统下，如何保证有请求重试的情况下，业务逻辑的正确性，以及解决并发的问题
设计幂等性系统，使用全局唯一ID来实现幂等性。

#二面
##使用面向对象的思路，设计多层，每层有多个电梯的大楼的电梯系统

##分布式系统下，如果提供服务限流，如短信服务，每分钟，每个服务调用方只能给每个手机发60条短信

##域名逆序
如：www.baidu.com -> com.baidu.www
##一个链表a1->a2->a3->...a(n-3)->a(n-2)->a(n-1)->an,对折，后部分逆序和前部分合并。输出a1->an->a2->a(n-1)->a3->a(n-2)->...
##矩阵倾斜输出，给定任意矩阵(m x n):
(0,0),(1,0),(2,0),(3,0)
(0,1),(1,1),(2,1),(3,1)
(0,2),(1,2),(2,2),(3,2)
输出顺序：
(0,0)
(1,0),(0,1)
(2,0),(1,1),(0,2)
(3,0),(2,1),(1,2)
(3,1),(2,2)
(3,2)
##天空中最多的飞机数量。航班时刻表中，每个航班都有在起飞时间和降落时间(不跨天)。航班时刻表数量巨大。给定所有航班起飞和降落时刻
(depTime,arrTime)
求在天上飞行的飞机的数量最多是多少
如：
int foo(List{(9:00,14:00),(8:00,11:00),(18:00,23:00)......})
{
    return 最多多少飞机
}
##解释微信浏览器登录，手机扫描二维码实现浏览器登录微信。具体原理
##设计一个长链转短链的系统
##执行抓取任务的时候，任务有时效性，任务异步执行。越新的任务优先级越高。使用redis实现这个任务管理。

##字符串转32位整数，要求能检测无效字符以及整数溢出
int str2int(const char *s){
    /* your code here */
}

##原地反转单链表
例如：1->2->3->4->5->(nil)
变成：5->4->3->2->1->(nil)

struct Node{
    struct Node *next;
    int data;
};

void reverse(struct Node *list){
    /* your code here */
}

##合并有序链表
例如：1->4->5->(nil)
例如：2->3->6->(nil)
变成：1->2->3->4->5->6->(nil)

struct Node{
    struct Node *next;
    int data;
};

struct Node *merge(struct Node *a, struct Node *b){
    /* your code here */
}

##计算某个元素在二叉树中的遍历顺序
例如：
      1
     / \
    2   3
   /\    \
  4  5    6
输入：5
输出：1,2,5

struct Node{
    struct Node *left;
    struct Node *right;
};

void print_path(struct Node *root, int value){
    /* your code here */
}

##求两个等长排序数组的中位数
例如：1,3,5,7,9
以及：2,4,6,8,10
输出：5.5

double get_mid(int *a, int *b, int count){
    /* your code here */
}

##雨水问题
例如：见附件图片
输入：0,1,0,2,1,0,1,3,2,1,2,1
输出：6

##做过的项目及产品详细讲述
