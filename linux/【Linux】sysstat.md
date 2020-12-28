## sysstat
### 简介
sysstat是一个软件包，包含监测系统性能及效率的一组工具，这些工具对于我们收集系统性能数据，比如：CPU 使用率、硬盘和网络吞吐数据，这些数据的收集和分析，有利于我们判断系统是否正常运行，是提高系统运行效率、安全运行服务器的得力助手。

官方网站: http://sebastien.godard.pagesperso-orange.fr

### 包含的工具
* iostat

  输出CPU的统计信息和所有I/O设备的输入输出（I/O）统计信息

* mpstat

  关于CPU的详细信息（单独输出或者分组输出）

* pidstat

  关于运行中的进程/任务、CPU、内存等的统计信息

* sar

  保存并输出不同系统资源（CPU、内存、IO、网络、内核等）的详细信息

* sadc

  系统活动数据收集器，用于收集sar工具的后端数据

* sa1

  系统收集并存储sadc数据文件的二进制数据，与sadc工具配合使用

* sa2

  配合sar工具使用，产生每日的摘要报告

* sadf

  用于以不同的数据格式（CVS或者XML）来格式化sar工具的输出

* sysstat

  sysstat 工具包的 man 帮助页面。

* nfsiostat

  NFS（Network File System）的I/O统计信息

* cifsiostat

  CIFS(Common Internet File System)的统计信息

### 安装
#### CentOS
* 通过yum安装：
  - yum install sysstat
* 通过rpm包安装：
  - wget -c http://pagesperso-orange.fr/sebastien.godard/sysstat-11.7.3-1.x86_64.rpm
  - sudo rpm -Uvh sysstat-11.7.3-1.x86_64.rpm

#### Ubuntu
```
# apt-get install sysstat
```

#### 编译安装
```
# wget http://pagesperso-orange.fr/sebastien.godard/sysstat-11.0.0.tar.gz
# tar -xvf sysstat-11.0.0.tar.gz 
# cd sysstat-11.0.0/
# ./configure
# make
# make install
```

**安装后检查**
```
# mpstat -V
 
sysstat version 11.0.0
(C) Sebastien Godard (sysstat <at> orange.fr)
```

### 定时统计任务
如果是用yum或apt-get方式安装，默认已经在/etc/cron.d/sysstat中配置好了计划日志；如果是编译安装或没有，可以手动配置，内容大致如下：
```
# Run system activity accounting tool every 10 minutes
*/10 * * * * root /usr/lib64/sa/sa1 1 1
# 0 * * * * root /usr/lib64/sa/sa1 600 6 &
# Generate a daily summary of process accounting at 23:53
53 23 * * * root /usr/lib64/sa/sa2 -A
```
统计的日志文件会存放在/var/log/sa这个目录下。每10分钟就进行一次日志的记录，在23:53对一天的日志进行汇总。
* /usr/lib64/sa/sa1是一个可以使用 cron 进行调度生成二进制日志文件的 shell 脚本
* /usr/lib64/sa/sa2是一个可以将二进制日志文件转换为用户可读的编码方式的 shell 脚本

**可能会碰到的问题：**
安装后首次执行sar会报如下错误：
```
无法打开 /var/log/sa/sa25: 没有那个文件或目录
```
原因是安装完sysstat后，定时任务还没生成那个文件。此处的 25 指的是日期。可以手动生成文件：
```
sudo sar -o 25
```

### 常用工具介绍
#### iostat
**简介**
Linux系统中的iostat是I/O statistics（输入/输出统计）的缩写，iostat工具将对系统的磁盘操作活动进行监视。它的特点是汇报磁盘活动统计情况，同时也会汇报出CPU使用情况。同vmstat一样，iostat也有一个弱点，就是它不能对某个进程进行深入分析，仅对系统的整体情况进行分析。

**语法**
iostat [参数] [时间] [次数]

**参数说明**
* -c 显示CPU使用情况
* -d 显示磁盘使用情况
* -k 以K为单位显示
* -m 以M为单位显示
* -N 显示磁盘阵列(LVM) 信息
* -n 显示NFS使用情况
* -p 可以报告出每块磁盘的每个分区的使用情况
* -t 显示终端和CPU的信息
* -x 显示详细信息

**应用场景**
1. iostat
不带参数的iostat命令将会输出CPU和每个分区的输出/输出的统计信息

2. iostat -x
显示详细信息

**屏幕显示如下：**

```
[user1@Test_Server ~]$ iostat -x
Linux 3.10.0-693.2.2.el7.x86_64 (jellythink)    01/05/2019      _x86_64_        (1 CPU)

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           1.83    0.00    0.31    0.09    0.00   97.77

Device:         rrqm/s   wrqm/s     r/s     w/s    rkB/s    wkB/s avgrq-sz avgqu-sz   await r_await w_await  svctm  %util
vda               0.03     0.78    0.24    1.38    12.64    20.67    41.01     0.02   10.98   55.50    3.17   0.71   0.12
```

**输出项说明：**
* %user：CPU处在用户模式下的时间百分比
* %nice：CPU处在带NICE值的用户模式下的时间百分比
* %system：CPU处在系统模式下的时间百分比
* %iowait：CPU等待输入输出完成时间的百分比
* %steal：管理程序维护另一个虚拟处理器时，虚拟CPU的无意识等待时间百分比
* %idle：CPU空闲时间百分比

当然了，iostat命令的重点不是用来看CPU的，重点是用来监测磁盘性能的。

* Device：设备名称
* rrqm/s：每秒合并到设备的读取请求数
* wrqm/s：每秒合并到设备的写请求数
* r/s：每秒向磁盘发起的读操作数
* w/s：每秒向磁盘发起的写操作数
* rkB/s：每秒读K字节数
* wkB/s:每秒写K字节数
* avgrq-sz：平均每次设备I/O操作的数据大小
* avgqu-sz：平均I/O队列长度
* await：平均每次设备I/O操作的等待时间 (毫秒)，一般地，系统I/O响应时间应该低于5ms，如果大于 10ms就比较大了
* r_await：每个读操作平均所需的时间；不仅包括硬盘设备读操作的时间，还包括了在kernel队列中等待的时间
* w_await：每个写操作平均所需的时间；不仅包括硬盘设备写操作的时间，还包括了在kernel队列中等待的时间
* svctm：平均每次设备I/O操作的服务时间 (毫秒)（这个数据不可信！）
* %util：一秒中有百分之多少的时间用于I/O操作，即被IO消耗的CPU百分比，一般地，如果该参数是100%表示设备已经接近满负荷运行了

3. iostat -d 2 3

显示磁盘使用情况

**屏幕显示如下：**

```
[user1@Test_Server ~]$ iostat -d 2 3
Linux 3.10.0-693.2.2.el7.x86_64 (jellythink)    01/05/2019      _x86_64_        (1 CPU)

Device:            tps    kB_read/s    kB_wrtn/s    kB_read    kB_wrtn
vda               1.62        12.64        20.67  337375593  551756524

Device:            tps    kB_read/s    kB_wrtn/s    kB_read    kB_wrtn
vda               1.00         0.00         8.00          0         16

Device:            tps    kB_read/s    kB_wrtn/s    kB_read    kB_wrtn
vda               0.00         0.00         0.00          0          0
```

**输出项说明：**
* tps：每秒I/O数（即IOPS。磁盘连续读和连续写之和）
* kB_read/s：每秒从磁盘读取数据大小，单位KB/s
* kB_wrtn/s：每秒写入磁盘的数据的大小，单位KB/s
* kB_read：从磁盘读出的数据总数，单位KB
* kB_wrtn：写入磁盘的的数据总数，单位KB

**性能监控关键指标**
* %iowait：如果该值较高，表示磁盘存在I/O瓶颈
* await：一般地，系统I/O响应时间应该低于5ms，如果大于10ms就比较大了
* avgqu-sz：如果I/O请求压力持续超出磁盘处理能力，该值将增加。如果单块磁盘的队列长度持续超过2，一般认为该磁盘存在I/O性能问题。需要注意的是，如果该磁盘为磁盘阵列虚拟的逻辑驱动器，需要再将该值除以组成这个逻辑驱动器的实际物理磁盘数目，以获得平均单块硬盘的I/O等待队列长度
* %util：一般地，如果该参数是100%表示设备已经接近满负荷运行了

4. iostat -c
iostat命令的-c参数仅输出CPU的统计信息

5. iostat -p sda
在默认情况下iostat命令会输出所有分区的统计信息，但是若在iostat命令后加上-p参数和磁盘设备名，该命令将会仅输出列出的磁盘的输入/输出统计信息

6. iostat -N
iostat命令的-N(大写)参数将会输出LVM（LCTT译注：LVM是linux环境下对磁盘分区进行管理的一种机制，是磁盘分区和文件系统间的一个逻辑层）的统计信息

**推荐阅读**
容易被误读的IOSTAT 
http://linuxperf.com/?p=156
辩证看待 iostat
https://zhuanlan.zhihu.com/p/33417860

#### mpstat
**简介**
mpstat是Multiprocessor Statistics的缩写，是实时系统监控工具。其报告与CPU的一些统计信息，这些信息存放在/proc/stat文件中。在多CPUs系统里，其不但能查看所有CPU的平均状况信息，而且能够查看特定CPU的信息。mpstat最大的特点是：可以查看多核心cpu中每个计算核心的统计数据；而类似工具vmstat只能查看系统整体cpu情况。

**语法**

mpstat [-P {|ALL}] [internal [count]]

参数解释
-P {|ALL} 表示监控哪个CPU， cpu在[0,cpu个数-1]中取值
internal 相邻的两次采样的间隔时间、
count 采样的次数，count只能和delay一起使用
当没有参数时，mpstat则显示系统启动以后所有信息的平均值。有interval时，第一行的信息自系统启动以来的平均信息。从第二行开始，输出为前一个interval时间段的平均信息。

**实例**
查看多核CPU核心的当前运行状况信息， 每2秒更新一次
mpstat -P ALL 2
```
19:43:58     CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest   %idle
19:43:59     all    0.00    0.00    0.04    0.00    0.00    0.00    0.00    0.00   99.96
19:43:59       0    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00  100.00
19:43:59       1    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00  100.00
.......
19:43:59      13    0.99    0.00    0.00    0.00    0.00    0.00    0.00    0.00   99.01
19:43:59      14    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00  100.00
19:43:59      15    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00  100.00
```
**字段解释**
* %user      在internal时间段里，用户态的CPU时间(%)，不包含nice值为负进程  (usr/total)*100
* %nice      在internal时间段里，nice值为负进程的CPU时间(%)   (nice/total)*100
* %sys       在internal时间段里，内核时间(%)       (system/total)*100
* %iowait    在internal时间段里，硬盘IO等待时间(%) (iowait/total)*100
* %irq       在internal时间段里，硬中断时间(%)     (irq/total)*100
* %soft      在internal时间段里，软中断时间(%)     (softirq/total)*100
* %idle      在internal时间段里，CPU除去等待磁盘IO操作外的因为任何原因而空闲的时间闲置时间(%) (idle/total)*100

**计算公式**
* total_cur = user + system + nice + idle + iowait + irq + softirq
* total_pre = pre_user + pre_system + pre_nice + pre_idle + pre_iowait + pre_irq + pre_softirq
* user = user_cur – user_pre
* total = total_cur – total_pre

其中_cur 表示当前值，_pre表示interval时间前的值。上表中的所有值可取到两位小数点。

#### pidstat
**简介**

pidstat是sysstat工具的一个命令，用于监控全部或指定进程的cpu、内存、线程、设备IO等系统资源的占用情况。pidstat首次运行时显示自系统启动开始的各项统计信息，之后运行pidstat将显示自上次运行该命令以后的统计信息。用户可以通过指定统计的次数和时间来获得所需的统计信息。

**命令格式**

pidstat [ 选项 ] [ <时间间隔> ] [ <次数> ]

**参数介绍**
* -u：默认的参数，显示各个进程的cpu使用统计
* -r：显示各个进程的内存使用统计
* -d：显示各个进程的IO使用情况
* -p：指定进程号
* -w：显示每个进程的上下文切换情况
* -t：显示选择任务的线程的统计信息外的额外信息
* -T { TASK | CHILD | ALL }
  这个选项指定了pidstat监控的。TASK表示报告独立的task，CHILD关键字表示报告进程下所有线程统计信息。ALL表示报告独立的task和task下面的所有线程。
  **注意：task和子线程的全局的统计信息和pidstat选项无关。这些统计信息不会对应到当前的统计间隔，这些统计信息只有在子线程kill或者完成的时候才会被收集。**
* -V：版本号
* -h：在一行上显示了所有活动，这样其他程序可以容易解析。
* -I：在SMP环境，表示任务的CPU使用率/内核数量
* -l：显示命令名和所有参数

**应用场景**

1. 查看所有进程的 CPU 使用情况（ -u -p ALL）
```
pidstat
pidstat -u -p ALL
```
**屏幕显示如下：**
```
16时58分10秒   UID       PID    %usr %system  %guest    %CPU   CPU  Command
16时58分10秒     0         1    0.09    0.06    0.00    0.16    12  systemd
16时58分10秒     0         2    0.00    0.00    0.00    0.00     7  kthreadd
16时58分10秒     0         3    0.00    0.01    0.00    0.01     0  ksoftirqd/0
```

**输出项说明：**
* PID：进程ID
* %usr：进程在用户空间占用cpu的百分比
* %system：进程在内核空间占用cpu的百分比
* %guest：进程在虚拟机占用cpu的百分比
* %CPU：进程占用cpu的百分比
* CPU：处理进程的cpu编号
* Command：当前进程对应的命令

2. cpu使用情况统计(-u)
```
pidstat -u
```
使用-u选项，pidstat将显示各活动进程的cpu使用统计，执行”pidstat -u”与单独执行”pidstat”的效果一样。

3. 内存使用情况统计(-r)
```
pidstat -r
```
使用-r选项，pidstat将显示各活动进程的内存使用统计

**屏幕显示如下：**
```
17时00分44秒   UID       PID  minflt/s  majflt/s     VSZ    RSS   %MEM  Command
17时00分44秒     0         1     43.98      0.00  198048   9708   0.03  systemd
17时00分44秒 42436       312      0.43      0.02  829152  68156   0.20  nova-novncproxy
17时00分44秒     0       394      0.01      0.00  172352    480   0.00  rsyslogd
```

**输出项说明：**
* PID：进程标识符
* Minflt/s:任务每秒发生的次要错误，不需要从磁盘中加载页
* Majflt/s:任务每秒发生的主要错误，需要从磁盘中加载页
* VSZ：虚拟地址大小，虚拟内存的使用KB
* RSS：常驻集合大小，非交换区五里内存使用KB
* Command：task命令名

4. 显示各个进程的IO使用情况（-d）
```
pidstat -d
```

**屏幕显示如下：**
```
17时01分56秒   UID       PID   kB_rd/s   kB_wr/s kB_ccwr/s  Command
17时01分56秒     0         1     59.53     11.50      2.02  systemd
17时01分56秒 42436       312     10.99      0.00      0.00  nova-novncproxy
17时01分56秒     0       394      0.13      0.00      0.00  rsyslogd
```

**输出项说明：**
* PID：进程id
* kB_rd/s：每秒从磁盘读取的KB
* kB_wr/s：每秒写入磁盘KB
* kB_ccwr/s：任务取消的写入磁盘的KB。当任务截断脏的pagecache的时候会发生。
* COMMAND:task的命令名

5. 显示每个进程的上下文切换情况（-w）
```
pidstat -w -p 2831
```

6. 显示选择任务的线程的统计信息外的额外信息 (-t)
```
pidstat -t -p 2831
```

7. pidstat -T
```
pidstat -T TASK
pidstat -T CHILD
pidstat -T ALL
```
* TASK表示报告独立的task。
* CHILD关键字表示报告进程下所有线程统计信息。
* ALL表示报告独立的task和task下面的所有线程。

**注意：task和子线程的全局的统计信息和pidstat选项无关。这些统计信息不会对应到当前的统计间隔，这些统计信息只有在子线程kill或者完成的时候才会被收集。**

#### sar
**简介**

sar（System Activity Reporter系统活动情况报告）是目前 Linux 上最为全面的系统性能分析工具之一，可以从多方面对系统的活动进行报告，包括：文件的读写情况、系统调用的使用情况、磁盘I/O、CPU效率、内存使用状况、进程活动及IPC有关的活动等。本文主要以CentOS 6.3 x64系统为例，介绍sar命令。

**命令格式**

sar [options] [-A] [-o file] t [n]

其中：
* t为采样间隔，n为采样次数，默认值是1；
* -o file表示将命令结果以二进制格式存放在文件中，file 是文件名。

**options 为命令行选项，sar命令常用选项如下：**
* -A：所有报告的总和
* -u：输出CPU使用情况的统计信息
* -v：输出inode、文件和其他内核表的统计信息
* -d：输出每一个块设备的活动信息
* -r：输出内存和交换空间的统计信息
* -b：显示I/O和传送速率的统计信息
* -a：文件读写情况
* -c：输出进程统计信息，每秒创建的进程数
* -R：输出内存页面的统计信息
* -y：终端设备活动情况
* -w：输出系统交换活动信息

**应用场景**
1. CPU资源监控

每10秒采样一次，连续采样3次，观察CPU的使用情况，并将采样结果以二进制形式存入当前目录下的文件test中
```
sar -u -o test 10 3
```
**屏幕显示如下：**
```
08时50分01秒     CPU     %user     %nice   %system   %iowait    %steal     %idle
09时00分01秒     all      6.48      0.04      2.82      1.01      0.01     89.64
09时10分01秒     all      6.58      0.04      2.87      1.18      0.01     89.32
09时20分01秒     all      6.29      0.04      2.73      1.01      0.01     89.92
平均时间:     all      6.98      0.04      3.12      1.63      0.01     88.21
```
**输出项说明：**
* CPU：all 表示统计信息为所有 CPU 的平均值。
* %user：显示在用户级别(application)运行使用 CPU 总时间的百分比。
* %nice：显示在用户级别，用于nice操作，所占用 CPU 总时间的百分比。
* %system：在核心级别(kernel)运行所使用 CPU 总时间的百分比。
* %iowait：显示用于等待I/O操作占用 CPU 总时间的百分比。
* %steal：管理程序(hypervisor)为另一个虚拟进程提供服务而等待虚拟 CPU 的百分比。
* %idle：显示 CPU 空闲时间占用 CPU 总时间的百分比。
  1. 若 %iowait 的值过高，表示硬盘存在I/O瓶颈
  2. 若 %idle 的值高但系统响应慢时，有可能是 CPU 等待分配内存，此时应加大内存容量
  3. 若 %idle 的值持续低于1，则系统的 CPU 处理能力相对较低，表明系统中最需要解决的资源是 CPU 。

如果要查看二进制文件test中的内容，需键入如下sar命令：
```
sar -u -f test
```

2. inode、文件和其他内核表监控

每10秒采样一次，连续采样3次，观察核心表的状态，需键入如下命令：
```
sar -v 10 3
```
**屏幕显示如下：**
```08时50分01秒 dentunusd   file-nr  inode-nr    pty-nr
09时00分01秒     70394     27392    167965         1
09时10分01秒     70705     27456    167648         1
09时20分01秒     71665     27424    167911         1
平均时间:    151055     28959    236529         3
```

**输出项说明：**
* dentunusd：目录高速缓存中未被使用的条目数量
* file-nr：文件句柄（file handle）的使用数量
* inode-nr：索引节点句柄（inode handle）的使用数量
* pty-nr：使用的pty数量

3. 内存和交换空间监控

每10秒采样一次，连续采样3次，监控内存分页
```
sar -r 10 3
```
**屏幕显示如下：**
```
08时50分01秒 kbmemfree kbmemused  %memused kbbuffers  kbcached  kbcommit   %commit  kbactive   kbinact   kbdirty
09时00分01秒   6190240  28588896     82.20    149972   2774352 129442268    372.18  24158620   2289764       760
09时10分01秒   6205992  28573144     82.16    152656   2785600 129452252    372.21  24132156   2300608      1016
09时20分01秒   6139540  28639596     82.35    155260   2816316 129502988    372.36  24176156   2322224       892
平均时间:   6255550  28523586     82.01     81218   2292328 130536476    375.33  24492853   1783438      1182
```

**输出项说明：**
* kbmemfree：这个值和free命令中的free值基本一致,所以它不包括buffer和cache的空间.
* kbmemused：这个值和free命令中的used值基本一致,所以它包括buffer和cache的空间.
* %memused：这个值是kbmemused和内存总量(不包括swap)的一个百分比.
* kbbuffers和kbcached：这两个值就是free命令中的buffer和cache.
* kbcommit：保证当前系统所需要的内存,即为了确保不溢出而需要的内存(RAM+swap).
* %commit：这个值是kbcommit与内存总量(包括swap)的一个百分比.

4. 内存分页监控
每10秒采样一次，连续采样3次，监控内存分页
```
sar -B 10 3
```
**屏幕显示如下：**
```
08时50分01秒  pgpgin/s pgpgout/s   fault/s  majflt/s  pgfree/s pgscank/s pgscand/s pgsteal/s    %vmeff
09时00分01秒    404.23   1529.08  57247.48      0.16  27014.90      0.00      0.00      0.00      0.00
09时10分01秒    658.10   1765.83  57604.23      0.16  27382.25      0.00      0.00      0.00      0.00
09时20分01秒    366.77   1325.85  56565.60      0.16  26729.90      0.00      0.00      0.00      0.00
平均时间:   2841.67   1859.98  61908.05     11.07  30514.84      0.00      0.00      0.00      0.00
```
**输出项说明：**
* pgpgin/s：表示每秒从磁盘或SWAP置换到内存的字节数(KB)
* pgpgout/s：表示每秒从内存置换到磁盘或SWAP的字节数(KB)
* fault/s：每秒钟系统产生的缺页数,即主缺页与次缺页之和(major + minor)
* majflt/s：每秒钟产生的主缺页数.
* pgfree/s：每秒被放入空闲队列中的页个数
* pgscank/s：每秒被kswapd扫描的页个数
* pgscand/s：每秒直接被扫描的页个数
* pgsteal/s：每秒钟从cache中被清除来满足内存需要的页个数
* %vmeff：每秒清除的页(pgsteal)占总扫描页(pgscank+pgscand)的百分比

5. I/O和传送速率监控
每10秒采样一次，连续采样3次，报告缓冲区的使用情况
```
sar -b 10 3
```
**屏幕显示如下：**
```
08时50分01秒       tps      rtps      wtps   bread/s   bwrtn/s
09时00分01秒    114.16     51.30     62.86    803.19   2555.48
09时10分01秒    119.52     55.55     63.96    711.68   2709.64
09时20分01秒    113.19     50.36     62.83    728.54   2366.62
平均时间:    153.53     87.71     65.82   5585.24   3179.88
```

**输出项说明：**
* tps：每秒钟物理设备的 I/O 传输总量
* rtps：每秒钟从物理设备读入的数据总量
* wtps：每秒钟向物理设备写入的数据总量
* bread/s：每秒钟从物理设备读入的数据量，单位为 块/s
* bwrtn/s：每秒钟向物理设备写入的数据量，单位为 块/s

6. 进程队列长度和平均负载状态监控

每10秒采样一次，连续采样3次，监控进程队列长度和平均负载状态
```
sar -q 10 3
```
**屏幕显示如下：**
```
08时50分01秒   runq-sz  plist-sz   ldavg-1   ldavg-5  ldavg-15   blocked
09时00分01秒         3     11214      1.28      1.93      2.01         0
09时10分01秒         4     11206      2.46      2.56      2.29         0
09时20分01秒        10     11211      1.48      1.67      2.02         0
平均时间:         4     11808      7.97      5.78      5.00         1
```

**输出项说明：**
* runq-sz：运行队列的长度（等待运行的进程数）
* plist-sz：进程列表中进程（processes）和线程（threads）的数量
* ldavg-1：最后1分钟的系统平均负载（System load average）
* ldavg-5：过去5分钟的系统平均负载
* ldavg-15：过去15分钟的系统平均负载

7. 系统交换活动信息监控
每10秒采样一次，连续采样3次，监控系统交换活动信息
```
sar -W 10 3
```
**屏幕显示如下：**
```
08时50分01秒  pswpin/s pswpout/s
09时00分01秒      0.00      0.00
09时10分01秒      0.00      0.00
09时20分01秒      0.00      0.00
平均时间:      0.00      0.00
```

**输出项说明：**
* pswpin/s：每秒系统换入的交换页面（swap page）数量
* pswpout/s：每秒系统换出的交换页面（swap page）数量

8. 设备使用情况监控
每10秒采样一次，连续采样3次，报告设备使用情况
```
sar -d 10 3 –p
```
参数-p可以打印出sda,hdc等磁盘设备名称,如果不用参数-p,设备节点则有可能是dev8-0,dev22-0

**屏幕显示如下：**
```
00时00分01秒       DEV       tps  rd_sec/s  wr_sec/s  avgrq-sz  avgqu-sz     await     svctm     %util
00时10分02秒       sda    117.81    805.63   2393.34     27.15      0.32      2.76      1.33     15.65
00时10分02秒       sdc      0.25      2.00      0.00      8.00      0.00      0.32      0.31      0.01
00时10分02秒       sdb      0.25      2.00      0.00      8.00      0.00      0.15      0.15      0.00
00时10分02秒   os-root     52.24      0.64    860.66     16.49      0.15      2.86      0.88      4.61
平均时间:      rbd6      0.01      0.28      0.05     42.53      0.00      7.20      0.88      0.00
```

**输出项说明：**
* tps:每秒从物理磁盘I/O的次数.多个逻辑请求会被合并为一个I/O磁盘请求,一次传输的大小是不确定的.
* rd_sec/s:每秒读扇区的次数.
* wr_sec/s:每秒写扇区的次数.
* avgrq-sz:平均每次设备I/O操作的数据大小(扇区).
* avgqu-sz:磁盘请求队列的平均长度.
* await:从请求磁盘操作到系统完成处理,每次请求的平均消耗时间,包括请求队列等待时间,单位是毫秒(1秒=1000毫秒).
* svctm:系统处理每次请求的平均时间,不包括在请求队列中消耗的时间.
* %util:I/O请求占CPU的百分比,比率越大,说明越饱和.
  1. avgqu-sz 的值较低时，设备的利用率较高。
  2. 当%util的值接近 1% 时，表示设备带宽已经占满。

##### 要判断系统瓶颈问题，有时需几个 sar 命令选项结合起来
* 怀疑CPU存在瓶颈，可用 sar -u 和 sar -q 等来查看
* 怀疑内存存在瓶颈，可用 sar -B、sar -r 和 sar -W 等来查看
* 怀疑I/O存在瓶颈，可用 sar -b、sar -u 和 sar -d 等来查看

##### 常用命令汇总，因版本和平台不同，有部分命令可能没有或显示结果不一致：
默认监控: sar 5 5     //  CPU和IOWAIT统计状态 
* (1) sar -b 5 5        // IO传送速率
* (2) sar -B 5 5        // 页交换速率
* (3) sar -c 5 5        // 进程创建的速率
* (4) sar -d 5 5        // 块设备的活跃信息
* (5) sar -n DEV 5 5    // 网路设备的状态信息
* (6) sar -n SOCK 5 5   // SOCK的使用情况
* (7) sar -n ALL 5 5    // 所有的网络状态信息
* (8) sar -P ALL 5 5    // 每颗CPU的使用状态信息和IOWAIT统计状态 
* (9) sar -q 5 5        // 队列的长度（等待运行的进程数）和负载的状态
* (10) sar -r 5 5       // 内存和swap空间使用情况
* (11) sar -R 5 5       // 内存的统计信息（内存页的分配和释放、系统每秒作为BUFFER使用内存页、每秒被cache到的内存页）
* (12) sar -u 5 5       // CPU的使用情况和IOWAIT信息（同默认监控）
* (13) sar -v 5 5       // inode, file and other kernel tablesd的状态信息
* (14) sar -w 5 5       // 每秒上下文交换的数目
* (15) sar -W 5 5       // SWAP交换的统计信息(监控状态同iostat 的si so)
* (16) sar -x 2906 5 5  // 显示指定进程(2906)的统计信息，信息包括：进程造成的错误、用户级和系统级用户CPU的占用情况、运行在哪颗CPU上
* (17) sar -y 5 5       // TTY设备的活动状态
* (18) 将输出到文件(-o)和读取记录信息(-f)

sar也可以监控非实时数据，通过cron周期的运行到指定目录下
例如:我们想查看本月27日,从0点到23点的内存资源.
sa27就是本月27日,指定具体的时间可以通过-s(start)和-e(end)来指定.
```
sar -f /var/log/sa/sa27 -s 00:00:00 -e 23:00:00 -r
```

##### 补充
sadf -d sarfile.log
将二进制数据文件转换成数据库可读的格式

sadf -d sarfile.log | sed 's/;/,/g' > sarfile.csv
也可以将这些数据存储在一个 csv 文档中，然后绘制成图表展示方式

sar -A -o sar_upgrade.log 5 > /dev/null 2>&1 &

Example:
For single day file:
# sar -A -f /var/log/sa/sa19 > /tmp/sa19_$(hostname).txt
# chmod 755 /tmp/sa19_$(hostname).txt

For multiple days files:
Works only in [Tool-Beta] page.
# ls /var/log/sa/sa?? | xargs -i sar -A -f {} > /tmp/sar_$(uname -n).txt
# chmod 755 /tmp/sar_$(uname -n).txt


