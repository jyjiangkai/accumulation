
## stress
iperf是一个网络性能测试工具。iperf可以测试TCP和UDP带宽质量。iperf可以测量最大TCP带宽，具有多种参数和UDP特性。iperf可以报告带宽，延迟抖动和数据包丢失。

### 安装包下载
http://downloads.es.net/pub/iperf/iperf-3.0.6.tar.gz

### 安装
```
yum install epel-release
yum update
yum install -y iperf
```

或者：
```
wget http://downloads.es.net/pub/iperf/iperf-3.0.6.tar.gz
tar zxvf iperf-3.0.6.tar.gz
cd iperf-3.0.6
./configure
make
make install
```

### 参数说明
Qt@aplex:~$ iperf -h

Usage: iperf [-s|-c host] [options]
       iperf [-h|--help] [-v|--version]

Client/Server:
  -f, --format    [kmKM]   format to report: Kbits, Mbits, KBytes, MBytes
  -i, --interval  #        seconds between periodic bandwidth reports
  -l, --len       #[KM]    length of buffer to read or write (default 8 KB)
  -m, --print_mss          print TCP maximum segment size (MTU - TCP/IP header)
  -o, --output    <filename> output the report or error message to this specified file
  -p, --port      #        server port to listen on/connect to
  -u, --udp                use UDP rather than TCP
  -w, --window    #[KM]    TCP window size (socket buffer size)
  -B, --bind      <host>   bind to <host>, an interface or multicast address
  -C, --compatibility      for use with older versions does not sent extra msgs
  -M, --mss       #        set TCP maximum segment size (MTU - 40 bytes)
  -N, --nodelay            set TCP no delay, disabling Nagle's Algorithm
  -V, --IPv6Version        Set the domain to IPv6

Server specific:
  -s, --server             run in server mode
  -U, --single_udp         run in single threaded UDP mode
  -D, --daemon             run the server as a daemon

Client specific:
  -b, --bandwidth #[KM]    for UDP, bandwidth to send at in bits/sec
                           (default 1 Mbit/sec, implies -u)
  -c, --client    <host>   run in client mode, connecting to <host>
  -d, --dualtest           Do a bidirectional test simultaneously
  -n, --num       #[KM]    number of bytes to transmit (instead of -t)
  -r, --tradeoff           Do a bidirectional test individually
  -t, --time      #        time in seconds to transmit for (default 10 secs)
  -F, --fileinput <name>   input the data to be transmitted from a file
  -I, --stdin              input the data to be transmitted from stdin
  -L, --listenport #       port to receive bidirectional tests back on
  -P, --parallel  #        number of parallel client threads to run
  -T, --ttl       #        time-to-live, for multicast (default 1)
  -Z, --linux-congestion <algo>  set TCP congestion control algorithm (Linux only)

Miscellaneous:
  -x, --reportexclude [CDMSV]   exclude C(connection) D(data) M(multicast) S(settings) V(server) reports
  -y, --reportstyle C      report as a Comma-Separated Values
  -h, --help               print this message and quit
  -v, --version            print version information and quit

[KM] Indicates options that support a K or M suffix for kilo- or mega-

The TCP window size option can be set by the environment variable
TCP_WINDOW_SIZE. Most other options can be set by an environment variable
IPERF_<long option name>, such as IPERF_BANDWIDTH.

Report bugs to iperf-users@lists.sourceforge.net
下面介绍一个简单用法：
iperf 是一个server 和client 的 组成的工作方式

如，我要测试 B地入流量，测试 A地出的流量 ，刚配置方法如下
A地要配置成client
B地配置为server

server:
iperf -s -p <port> -d
iperf -s -p 8080 -d

client:
iperf -c <serverIP> -p <port> -P 30 -t 300
iperf -c x.x.x.x -p 8080 -P 30 -t 300

对上面几个参数做简单说明
-s 表示是server， -p 8080 监听8080， -d 表示以daemon方式运行
-c 以client 方式运行， -P 表示开启的线程数， -t 表示持续的时间（单位为s）