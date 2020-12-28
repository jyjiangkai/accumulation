## iotop
iotop是一款开源、免费的用来监控磁盘I/O使用状况的类似top命令的工具，iotop可以监控进程的I/O信息。它是Python语言编写的，与iostat工具比较，iostat是系统级别的IO监控，而iotop是进程级别IO监控。目前最新的版本为iotop 0.6。
其官方网址http://guichaz.free.fr/iotop/

### 安装
wget http://guichaz.free.fr/iotop/files/iotop-0.4.4.tar.gz
tar -zxvf iotop-0.4.4.tar.gz
cd iotop-0.4.4
python setup.py build
python setup.py install
cd ../

### 参数说明
# iotop --help
Usage: /usr/local/sbin/iotop [OPTIONS]
 
DISK READ and DISK WRITE are the block I/O bandwidth used during the sampling
period. SWAPIN and IO are the percentages of time the thread spent respectively
while swapping in and waiting on I/O more generally. PRIO is the I/O priority at
which the thread is running (set using the ionice command).
 
Controls: left and right arrows to change the sorting column, r to invert the
sorting order, o to toggle the --only option, p to toggle the --processes
option, a to toggle the --accumulated option, i to change I/O priority, q to
quit, any other key to force a refresh.
 
Options:
  --version             show program's version number and exit
                        显示版本号
  -h, --help            show this help message and exit
                        显示帮助信息
  -o, --only            only show processes or threads actually doing I/O
                        只显示正在产生I/O的进程或线程，运行过程中，可以通过按o随时切换
  -b, --batch           non-interactive mode
                        非交互模式下运行，一般用来记录日志。
  -n NUM, --iter=NUM    number of iterations before ending [infinite]
                        设置监控（显示）NUM次，主要用于非交互模式。默认无限
  -d SEC, --delay=SEC   delay between iterations [1 second]
                        设置显示的间隔秒数，支持非整数
  -p PID, --pid=PID     processes/threads to monitor [all]
                        只显示指定进程（PID）的信息
  -u USER, --user=USER  users to monitor [all]
                        显示指定的用户的进程的信息
  -P, --processes       only show processes, not all threads
                        只显示进程，不显示所有线程
  -a, --accumulated     show accumulated I/O instead of bandwidth
                        累积的I/O,显示从iotop启动后每个进程累积的I/O总数，便于诊断问题
  -k, --kilobytes       use kilobytes instead of a human friendly unit
                        显示使用KB单位
  -t, --time            add a timestamp on each line (implies --batch)
                        非交互模式下，加上时间戳。
  -q, --quiet           suppress some lines of header (implies --batch)
                        只在第一次监测时显示列名. 去除头部一些行：这个参数可以设置最多3次来移除头部行：-q列头部只在最初交互显示一次；-qq列头部不显示；-qqq，I/O的总结不显示



### 命令样例
```
```

### iotop信息解释
Total DISK READ 和 Total DISK WRITE 的值一方面表示了进程和内核线程之间的总的读写带宽，另一方面也表示内核块设备子系统的。
Actual DISK READ 和 Actual DISK WRITE 的值表示在内核块设备子系统和下面硬件（HDD、SSD 等等）对应的实际磁盘 I/O 带宽。