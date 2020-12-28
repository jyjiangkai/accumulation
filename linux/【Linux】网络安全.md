## 网络安全
### 防火墙
netfilter/iptables是集成在Linux 2.4x版本内核中的包过滤防火墙系统
netfilter/iptables框架可以实现数据包过滤、网络地址转换以及数据包管理功能。
Linux中的防火墙系统包括两部分：netfilte和iptables。netfilte位于内核空间，目前是Linux内核的组成部分。Netfilter可以对本机所有流入、流出、转发的数据包进行查看、修改、丢弃、拒绝等操作。由于netfilter在内核空间，用户通常无法接触内核和修改内核，此时就需要一个命令行工具。这种命令行工具有很多，比如iptables，firewalld等工具。使用iptables可以添加、删除具体的过滤规则，iptables默认维护着四个表和五个链，所有的防火墙策略规则都将被分别写入这些表与链中。Linux防火墙因为已经集成在内核中，所有相对于应用层防火墙产品而言，基于内核的Linux防火墙更加快捷、高效。CentOS 7操作系统在原有的netfilter/iptables架构基础上又增加了firewalld。iptables工具是一个比较低级别的工具，它仅可以调整IPv4的防火墙规则，所有在CentOS 7系统中使用了枫irewalld作为默认的用户防火墙工具。但当用户使用firewalld编写IPv4防火墙规则时，firewalld依然是调用了底层的iptables实现具体的功能。

### iptables
#### 传统iptables的使用
iptables默认维护着四个表和五个链，每个规则表中包含多个数据链，防火墙规则需要写入到这些具体的数据链中。
**规则表**
* filter表
  过滤规则表
* nat表
  地址转换规则表
* mangle表
  修改数据标记位规则表
* raw表
  跟踪数据表规则表
说明：
4个表的优先级由高到低：raw-->mangle-->nat-->filter

**数据链**
* INPUT
  入站数据过滤
* OUTPUT
  出站数据过滤
* FORWARD
  转发数据过滤
* PREROUTING
  路由前过滤
* POSTROUTING
  路由后过滤

**场景**
场景一：入站数据流向

  从外界到达防火墙的数据包，先被PREROUTING规则链处理（是否修改数据包地址等），之后会进行路由选择（判断该数据包应该发往何处），如果数据包 的目标主机是防火墙本机（比如说Internet用户访问防火墙主机中的web服务器的数据包），那么内核将其传给INPUT链进行处理（决定是否允许通 过等），通过以后再交给系统上层的应用程序（比如Apache服务器）进行响应。

场景二：转发数据流向
  来自外界的数据包到达防火墙后，首先被PREROUTING规则链处理，之后会进行路由选择，如果数据包的目标地址是其它外部地址（比如局域网用户通过网 关访问QQ站点的数据包），则内核将其传递给FORWARD链进行处理（是否转发或拦截），然后再交给POSTROUTING规则链（是否修改数据包的地 址等）进行处理。

场景三：出站数据流向
  防火墙本机向外部地址发送的数据包（比如在防火墙主机中测试公网DNS服务器时），首先被OUTPUT规则链处理，之后进行路由选择，然后传递给POSTROUTING规则链（是否修改数据包的地址等）进行处理。

#### iptables语法格式
iptables [-t 表名] {-A|-D|-I|-F|-L|-R|-Z|-P|} [链名] [条件匹配] [-j 目标动作或跳转]

**说明**
- 表名、链名用于指定iptables命令所操作的表和链
- 条件匹配用于指定对符合条件的数据包进行处理
- 目标动作或跳转用于指定数据包的处理方式（比如允许通过、拒绝、丢弃、跳转（Jump）给其它链处理）
- iptables防火墙规则的顺序非常重要，内核按顺序检查这些规则，如果发现有匹配的规则条目，则立刻执行相关动作，停止继续向下查找规则条目，如果所有的防火墙都未能匹配成功，则按默认策略处理。
- 使用-A选项添加防火墙规则会将该规则追加到整个链的最后
- 使用-I选项添加防火墙规则会将该规则插入到链中作为第一条规则

**选项**
-t      指定需要维护的防火墙规则表，不使用-t时，则默认操作对象为filter表
-A      追加防火墙规则
    -n  数字方式显示
    -v  显示详细信息
    -x  显示精确信息
-D      删除防火墙规则
-I      插入防火墙规则
-F      清空防火墙规则
-L      列出防火墙规则
-R      替换防火墙规则
-Z      清空防火墙数据表统计信息
-P      设置链默认规则

**条件匹配参数**
[!] -p              匹配协议，！代表取反，[tcp|udp|icmp]
[!] -s              匹配源地址
[!] -d              匹配目标地址
[!] -i              匹配入站网卡接口
[!] -o              匹配出站网卡接口
[!] --sport         匹配源端口
[!] --dport         匹配目标端口
[!] --src-range     匹配源地址范围
[!] --dst-range     匹配目标地址范围
[!] --limit         匹配数据表速率
[!] --mac-source    匹配源MAC地址
[!] --sports        匹配源端口
[!] --dports        匹配目标端口
[!] --state         匹配状态，[INVALID|ESTABLISHED|NEW|RELATED]
[!] --string        匹配应用层字符串

**目标动作或跳转**
ACCEPT      允许数据包通过
DROP        丢弃数据包
REJECT      拒绝数据包通过
LOG         将数据包信息记录syslog日志
DNAT        目标地址转换
SNAT        源地址转换
MASQUERADE  地址欺骗
REDIRECT    重定向

#### 防火墙备份与还原
默认的iptables防火墙规则会立刻生效，但如果不保存，当计算机重启后所有的规则都将丢失，所以对防火墙规则进行及时保存的操作是非常重要的。
iptables软件包提供了两个工具实现防火墙规则的保存与还原，这两个工具是iptables-save和iptables-restore，这两个工具的最大优势是处理庞大规则集时速度非常快。
CentOS 7系统中防火墙规则默认保存在/etc/sysconfig/iptables文件中

#### iptables常用命令
1. service iptables save 命令
  
  它会保存在/etc/sysconfig/iptables这个文件中

2. iptables-save 命令

  iptables-save > /etc/sysconfig/iptables

3. iptables-restore 命令

  开机的时候，它会自动加载/etc/sysconfig/iptabels
  如果开机不能加载或者没有加载，而你想让一个自己写的配置文件（假设为iptables.2）手动生效的话：
  iptables-restore < /etc/sysconfig/iptables.2
  则完成了将iptables中定义的规则手动生效

4. sysctl -p
  
  重新加载内核参数配置文件/etc/sysctl.conf

### firewalld
#### firewalld简介
firewalld将所有的网络流量都分类汇集到zones中，firewalld通过zones管理防火墙规则。每一个进入系统的数据包，都会首先检查它的源IP地址和接口（进出的网卡接口），如果地址与某个zone匹配，则该zone中的规则将生效。每个zone都会有开启或关闭的服务和端口的列表，以实现允许或拒绝连接服务和端口。如果数据包的源IP地址和网卡接口都不能和任何zone匹配，则该数据包将匹配默认zone，一般情况下是一个名称为public的默认zone。firewalld会提供block、dmz、drop、external、home、internal、public、trusted、work这九个zone。

firewalld预定义
trusted     允许所有入站流量
home        允许其他主机入站访问本机的ssh、mdns、ipp-client、samba-client或者dhcpv6-client这些预定义的服务
            本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包
internal    允许其他主机入站访问本机的ssh、mdns、ipp-client、samba-client或者dhcpv6-client这些预定义的服务
            本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包
work        允许其他主机入站访问本机的ssh或者dhcpv6-client这些预定义的服务
            本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包
external    允许其他主机入站访问本机的ssh服务
            本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包
            通过本zone进行转发的IPv4数据包，都将会进行NAT后再转发除去（不管数据包的真实来源IP是多少，该数据包的源地址会被修改为防火墙本机出站网卡的IP地址）
dmz         允许其他主机入站访问本机的ssh服务
            本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包
block       本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包
drop        本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包
public      允许其他主机入站访问本机的ssh或者dhcpv6-client这些预定义的服务
            本机访问其他主机后，对方返回的入站数据，都将被允许
            拒绝其他所有入站数据包

#### firewalld-cmd命令
**命令描述**
firewalld防火墙规则管理工具

**用法**
firewalld-cmd [OPTIONS...]

**选项**
--get-default-zone                              获取默认zone信息
--set-default-zone=<zone>                       设置默认zone
--get-active-zones                              显示当前正在使用的zone信息
--get-zones                                     显示系统预定义的zone
--get-services                                  显示系统预定义的服务名称
--get-zone-of-interface=<interface>             查询某个接口与哪个zone匹配
--get-zone-of-source=<source>[/<mask>]          查询某个源地址与哪个zone匹配
--list-all-zones                                显示所有的zone信息的所有规则
--add-service=<service>                         向zone中添加允许访问的服务
--add-port=<portid>[-<portid>]/<protocol>       向zone中添加允许访问的端口
--add-interface=<interface>                     将接口与zone绑定
--add-source=<source>[/<mask>]                  将源地址与zone绑定
--list-all                                      列出某个zone的所有规则信息
--remove-service=<service>                      从zone中移除允许某个服务的规则
--remove-port=<portid>[-<portid>]/<protocol>    从zone中移除允许某个端口的规则
--remove-source=<source>[/<mask>]               将源地址与zone解除绑定
--remove-interface=<interface>                  将网卡接口与zone解除绑定
--permanent                                     设置永久有效规则，默认情况规则都是临时的
--reload                                        重新加载防火墙规则

#### firewalld常用命令
1. firewall-cmd --state

  检查状态

2. systemctl start|stop|restart|stauts|enable|disable firewalld.service

  防火墙服务管理

3. firewall-cmd --zone=public --add-port=5901/tcp --permanent

  对外网开放5901端口
  --zone=public：作用域
  --add-port=5901/tcp：开放基于tcp的5901端口
  --permanent：永久

4. firewall-cmd --query-port=5901/tcp

  查看5901端口是否开放

5. firewall-cmd --list-ports

  查看所有开放的端口


  ### 参考资料
  https://www.cnblogs.com/liang2580/articles/8400140.html
  