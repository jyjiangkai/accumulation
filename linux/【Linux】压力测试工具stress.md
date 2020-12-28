## stress
stress是一个linux下的压力测试工具，专门为那些想要测试自己的系统，完全高负荷和监督这些设备运行的用户。

### 安装包下载
https://fossies.org/linux/privat/stress-1.0.4.tar.gz

### 安装
```
wget https://fossies.org/linux/privat/stress-1.0.4.tar.gz
tar -zxvf stress-1.0.4.tar.gz
cd stress-1.0.4
./configure
make
make check
make install
```

### 参数说明
```
[root@iZ2ze7aznw5uolxelrna1gZ ~]# stress --help
`stress' imposes certain types of compute stress on your system

Usage: stress [OPTION [ARG]] ...
 -?, --help         show this help statement  // 显示软件帮助信息
     --version      show version statement  // 显示软件版本信息
 -v, --verbose      be verbose  // 显示 stress 程序运行过程中的详细信息
 -q, --quiet        be quiet  // 程序在运行的过程中不输出信息
 -n, --dry-run      show what would have been done
 -t, --timeout N    timeout after N seconds  // 指定运行多少秒
     --backoff N    wait factor of N microseconds before work starts  // 等待xx微秒后才开始运行
 -c, --cpu N        spawn N workers spinning on sqrt()  // 产生N个处理sqrt()函数的CPU进程
 -i, --io N         spawn N workers spinning on sync()  // 产生N个处理sync()函数的磁盘I/O进程
 -m, --vm N         spawn N workers spinning on malloc()/free()  // 产生N个不断分配和释放内存的进程
     --vm-bytes B   malloc B bytes per vm worker (default is 256MB)  // 指定分配内存的大小
     --vm-stride B  touch a byte every B bytes (default is 4096)  // 不断的给部分内存赋值，让 COW(Copy On Write)发生
     --vm-hang N    sleep N secs before free (default none, 0 is inf)  // 指示每个消耗内存的进程在分配到内存后转入睡眠状态 N 秒，然后释放内存，一直重复执行这个过程
     --vm-keep      redirty memory instead of freeing and reallocating  //  一直占用内存，区别于不断的释放和重新分配(默认是不断释放并重新分配内存)
 -d, --hdd N        spawn N workers spinning on write()/unlink()  // 产生N个不断执行 write 和 unlink 函数的进程(创建文件，写入内容，删除文件)
     --hdd-bytes B  write B bytes per hdd worker (default is 1GB)  // 指定文件大小

Example: stress --cpu 8 --io 4 --vm 2 --vm-bytes 128M --timeout 10s

Note: Numbers may be suffixed with s,m,h,d,y (time) or B,K,M,G (size).
```

### 使用
#### 压测CPU
stress --cpu 120000 --timeout 120s
stress --cpu 1 --timeout 600 (占用一个CPU核心。使用时长600s [增加1个cpu进程，处理sqrt()函数函数，以提高系统CPU负荷])
* 注: 
A Container might or might not be allowed to exceed its CPU limit for extended periods of time. However, it will not be killed for excessive CPU usage.
压cpu不应该导致Pods重启
来自：https://kubernetes.io/docs/concepts/configuration/manage-compute-resources-container/#how-pods-with-resource-limits-are-run
 
#### 压测内存
压满1G内存，执行10分钟：
stress --vm 10 --vm-bytes 100m --vm-hang 100 --timeout 600s
–vm-bytes 表示malloc分配多少内存
–vm-hang 表示malloc分配的内存多少时间后在free()释放掉
–vm 指定进程数量

stress --vm 1 --vm-bytes 100m --vm-hang 60 --timeout 60s
stress --vm 1 --vm-bytes 400m --vm-hang 10 --timeout 10s

#### 压测磁盘IO
stress -i 命令只是调用sync(）系统调用刷新缓冲区内容到磁盘，但是他是刷新的系统已有的缓冲区，当系统缓冲区内容已经刷新到磁盘上时，%iowait便不会增加。所以可以尝试stree -d 命令，即产生新的文件进行写文件的压力测试。写文件时最终也会调用sync命令刷新到磁盘上。

产生 4 个进程，每个进程都反复调用 sync 函数将内存上的内容写到硬盘上
stress -i 4
新增1个写进程，每次写3G文件块
stress -d 1 --hdd-bytes 1G --timeout 600s
创建一个进程不断的在磁盘上创建 10M 大小的文件并写入内容
stress -d 1 --hdd-bytes 10M



stress <options>

常用选项：
-c, --cpu N              产生 N 个进程，每个进程都反复不停的计算随机数的平方根
-i, --io N                  产生 N 个进程，每个进程反复调用 sync() 将内存上的内容写到硬盘上
-m, --vm N             产生 N 个进程，每个进程不断分配和释放内存
    --vm-bytes B      指定分配内存的大小
    --vm-stride B     不断的给部分内存赋值，让 COW(Copy On Write)发生
    --vm-hang N      指示每个消耗内存的进程在分配到内存后转入睡眠状态 N 秒，然后释放内存，一直重复执行这个过程
    --vm-keep          一直占用内存，区别于不断的释放和重新分配(默认是不断释放并重新分配内存)
-d, --hadd N           产生 N 个不断执行 write 和 unlink 函数的进程(创建文件，写入内容，删除文件)
    --hadd-bytes B  指定文件大小
-t, --timeout N       在 N 秒后结束程序
--backoff N            等待N微妙后开始运行
-q, --quiet              程序在运行的过程中不输出信息
-n, --dry-run          输出程序会做什么而并不实际执行相关的操作
--version                显示版本号

-v, --verbose 显示详细的信息
sudo yum install -y stress 

stress -c 4  # 消耗cpu资源

stress --vm 2 --vm-bytes 300M --vm-keep  # 消耗内存资源

stress -i 4  # 消耗IO资源

stress -d 1 --hdd-bytes 10M  # 磁盘资源

同时指定多项消耗
stress --cpu 3 --io 3 --vm 2 --vm-bytes 10M --vm-keep 



