## mpstat
### 简介
mpstat是Multiprocessor Statistics的缩写，是实时系统监控工具。其报告与CPU的一些统计信息，这些信息存放在/proc/stat文件中。在多CPUs系统里，其不但能查看所有CPU的平均状况信息，而且能够查看特定CPU的信息。mpstat最大的特点是：可以查看多核心cpu中每个计算核心的统计数据；而类似工具vmstat只能查看系统整体cpu情况。

### 语法
mpstat [-P {|ALL}] [internal [count]]
参数 解释
-P {|ALL} 表示监控哪个CPU， cpu在[0,cpu个数-1]中取值
internal 相邻的两次采样的间隔时间、
count 采样的次数，count只能和delay一起使用
当没有参数时，mpstat则显示系统启动以后所有信息的平均值。有interval时，第一行的信息自系统启动以来的平均信息。从第二行开始，输出为前一个interval时间段的平均信息。

### 实例
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
#### 字段解释
* %user      在internal时间段里，用户态的CPU时间(%)，不包含nice值为负进程  (usr/total)*100
* %nice      在internal时间段里，nice值为负进程的CPU时间(%)   (nice/total)*100
* %sys       在internal时间段里，内核时间(%)       (system/total)*100
* %iowait    在internal时间段里，硬盘IO等待时间(%) (iowait/total)*100
* %irq       在internal时间段里，硬中断时间(%)     (irq/total)*100
* %soft      在internal时间段里，软中断时间(%)     (softirq/total)*100
* %idle      在internal时间段里，CPU除去等待磁盘IO操作外的因为任何原因而空闲的时间闲置时间(%) (idle/total)*100

#### 计算公式
* total_cur = user + system + nice + idle + iowait + irq + softirq
* total_pre = pre_user + pre_system + pre_nice + pre_idle + pre_iowait + pre_irq + pre_softirq
* user = user_cur – user_pre
* total = total_cur – total_pre

其中_cur 表示当前值，_pre表示interval时间前的值。上表中的所有值可取到两位小数点。