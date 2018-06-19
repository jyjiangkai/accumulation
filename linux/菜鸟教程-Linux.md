##Linux相关知识

###Linux系统启动过程
####Linux关机
在linux领域内大多用在服务器上，很少遇到关机的操作。毕竟服务器上跑一个服务是永无止境的，除非特殊情况下，不得已才会关机。
正确的关机流程为：sync > shutdown > reboot > halt
关机指令为：shutdown ，你可以man shutdown 来看一下帮助文档。
例如你可以运行如下命令关机：
*sync 将数据由内存同步到硬盘中。
*shutdown 关机指令，你可以man shutdown 来看一下帮助文档。例如你可以运行如下命令关机：
*shutdown –h 10 ‘This server will shutdown after 10 mins’ 这个命令告诉大家，计算机将在10分钟后关机，并且会显示在登陆用户的当前屏幕中。
*Shutdown –h now 立马关机
*Shutdown –h 20:25 系统会在今天20:25关机
*Shutdown –h +10 十分钟后关机
*Shutdown –r now 系统立马重启
*Shutdown –r +10 系统十分钟后重启
*reboot 就是重启，等同于 shutdown –r now
*halt 关闭系统，等同于shutdown –h now 和 poweroff
最后总结一下，不管是重启系统还是关闭系统，首先要运行 sync 命令，把内存中的数据写到磁盘中。

###Linux文件与目录管理
####Linux文件内容查看
Linux系统中使用以下命令来查看文件的内容：
*cat  由第一行开始显示文件内容
*tac  从最后一行开始显示，可以看出 tac 是 cat 的倒著写！
*nl   显示的时候，顺道输出行号！
*more 一页一页的显示文件内容
*less 与 more 类似，但是比 more 更好的是，他可以往前翻页！
*head 只看头几行
*tail 只看尾几行

cat
由第一行开始显示文件内容
语法：
cat [-AbEnTv]
选项与参数：
-A ：相当於 -vET 的整合选项，可列出一些特殊字符而不是空白而已；
-b ：列出行号，仅针对非空白行做行号显示，空白行不标行号！
-E ：将结尾的断行字节 $ 显示出来；
-n ：列印出行号，连同空白行也会有行号，与 -b 的选项不同；
-T ：将 [tab] 按键以 ^I 显示出来；
-v ：列出一些看不出来的特殊字符

tail
取出文件后面几行
语法：
tail [-n number] 文件 
选项与参数：
-n ：后面接数字，代表显示几行的意思
-f ：表示持续侦测后面所接的档名，要等到按下[ctrl]-c才会结束tail的侦测

vim
/字串  向下搜索  n：向下  N：向上
?字串  向上搜索  n：向上  N：向下

###Linux用户和用户组管理
####Linux系统用户账号的管理
#####添加新的用户账号使用useradd命令，其语法如下：
useradd 选项 用户名
参数说明：
选项:
-c comment 指定一段注释性描述。
-d 目录 指定用户主目录，如果此目录不存在，则同时使用-m选项，可以创建主目录。
-g 用户组 指定用户所属的用户组。
-G 用户组，用户组 指定用户所属的附加组。
-s Shell文件 指定用户的登录Shell。
-u 用户号 指定用户的用户号，如果同时有-o选项，则可以重复使用其他用户的标识号。
用户名:
指定新账号的登录名。

#####删除帐号，其语法如下：
userdel 选项 用户名
常用的选项是 -r，它的作用是把用户的主目录一起删除。
例如：
# userdel -r sam

#####修改帐号，其语法如下：
usermod 选项 用户名
常用的选项包括-c, -d, -m, -g, -G, -s, -u以及-o等，这些选项的意义与useradd命令中的选项一样，可以为用户指定新的资源值。
另外，有些系统可以使用选项：-l 新用户名
这个选项指定一个新的账号，即将原来的用户名改为新的用户名。
例如：
# usermod -s /bin/ksh -d /home/z –g developer sam

#####用户口令的管理
passwd 选项 用户名
可使用的选项：
-l 锁定口令，即禁用账号。
-u 口令解锁。
-d 使账号无口令。
-f 强迫用户下次登录时修改口令。
如果默认用户名，则修改当前用户的口令。

####Linux系统用户组的管理
#####增加一个新的用户组使用groupadd命令。其格式如下：
groupadd 选项 用户组
可以使用的选项有：
-g GID 指定新用户组的组标识号（GID）。
-o 一般与-g选项同时使用，表示新用户组的GID可以与系统已有用户组的GID相同。

#####删除一个已有的用户组，使用groupdel命令，其格式如下：
groupdel 用户组

#####修改用户组的属性使用groupmod命令。其语法如下：
groupmod 选项 用户组
常用的选项有：
-g GID 为用户组指定新的组标识号。
-o 与-g选项同时使用，用户组的新GID可以与系统已有用户组的GID相同。
-n新用户组 将用户组的名字改为新名字

####与用户账号有关的系统文件
#####/etc/passwd文件
Linux系统中的每个用户都在/etc/passwd文件中有一个对应的记录行，它记录了这个用户的一些基本属性。
/etc/passwd中一行记录对应着一个用户，每行记录又被冒号(:)分隔为7个字段，其格式和具体含义如下：
用户名:口令:用户标识号:组标识号:注释性描述:主目录:登录Shell

#####/etc/group文件
用户组的所有信息都存放在/etc/group文件中。此文件的格式也类似于/etc/passwd文件，由冒号(:)隔开若干个字段，这些字段有：
组名:口令:组标识号:组内用户列表

###Linux磁盘管理
####df
df -h [dir]

####du
du -sh [dir]|[file]|[*]

####fdisk
fdisk -l        //列出所有分区信息
fdisk /dev/hdc  //查阅/dev/hdc硬盘内的相关信息
Command (m for help):     <==等待你的输入！
Command (m for help): m   <== 输入 m 后，就会看到底下这些命令介绍
Command (m for help): p  <== 这里可以输出目前磁盘的状态
离开 fdisk 时按下 q，那么所有的动作都不会生效！相反的， 按下w就是动作生效的意思。

####磁盘格式化
磁盘分割完毕后自然就是要进行文件系统的格式化，格式化的命令非常的简单，使用 mkfs（make filesystem） 命令。
语法：
mkfs [-t 文件系统格式] 装置文件名
选项与参数：
-t ：可以接文件系统格式，例如 ext3, ext2, vfat 等(系统有支持才会生效)
例：
    mkfs
    mkfs -t ext3 /dev/hdc6

####磁盘挂载与卸除
Linux 的磁盘挂载使用 mount 命令，卸载使用 umount 命令。
磁盘挂载语法：
mount [-t 文件系统] [-L Label名] [-o 额外选项] [-n]  装置文件名  挂载点
例：
    mount /dev/hdc6 /mnt/hdc6    //将/dev/hdc6分区挂载到/mnt/hdc6目录上面
磁盘卸载命令 umount 语法：
umount [-fn] 装置文件名或挂载点
选项与参数：
-f ：强制卸除！可用在类似网络文件系统 (NFS) 无法读取到的情况下；
-n ：不升级 /etc/mtab 情况下卸除。
例：
    umount /dev/hdc6
	
###Linux yum
yum提供了查找、安装、删除某一个、一组甚至全部软件包的命令，而且命令简洁而又好记。
####yum语法
yum [options] [command] [package ...]
options：可选，选项包括-h（帮助），-y（当安装过程提示选择全部为"yes"），-q（不显示安装的过程）等等。
command：要进行的操作。
package：操作的对象。
####yum常用命令
1.列出所有可更新的软件清单命令：yum check-update
2.更新所有软件命令：yum update
3.仅安装指定的软件命令：yum install <package_name>
4.仅更新指定的软件命令：yum update <package_name>
5.列出所有可安裝的软件清单命令：yum list
6.删除软件包命令：yum remove <package_name>
7.查找软件包命令：yum search <keyword>
8.清除缓存命令:
yum clean packages: 清除缓存目录下的软件包
yum clean headers: 清除缓存目录下的headers
yum clean oldheaders: 清除缓存目录下旧的headers
yum clean, yum clean all (= yum clean packages; yum clean oldheaders) :清除缓存目录下的软件包及旧的headers



