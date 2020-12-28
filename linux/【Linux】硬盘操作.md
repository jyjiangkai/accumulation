## Linux硬盘操作
###查看磁盘情况
执行`fdisk -l`命令查看所有磁盘分区情况
一般/dev/vda指的是系统盘，/dev/sda指的就是数据盘
/dev/sda1指的是数据盘的第一个分区
###创建分区
指定要创建分区的磁盘，执行命令`fdisk /dev/sda`进入fdisk命令窗口。
常用命令：
 * p命令查看当前磁盘的分区情况；
 * m命令查看命令指导
 * n命令创建新分区
 * d命令删除分区
 * w命令保存修改并退出
执行n命令创建新分区，这时候会提示输入分区类型、分区编号和分区范围，然后创建新分区成功
###删除分区
执行d命令删除分区，这时候会提示输入要删除的分区编号，然后删除成功
###格式化分区
`mkfs -t ext3 /dev/sda1`将/dev/sda1分区格式化未ext3
`mkfs -t ext2 /dev/sda2`将/dev/sda2分区格式化未ext2
###挂载分区
`mount /dev/sda1 /home/jiangkai1`将/dev/sda1分区挂载到/home/jiangkai1目录下
`mount /dev/sda2 /home/jiangkai2`将/dev/sda2分区挂载到/home/jiangkai2目录下
###卸载分区
`umount /dev/sda1`卸载/dev/sda1分区
`umount /dev/sda2`卸载/dev/sda2分区
###分区生效
此时使用fdisk命令如果看不到新建的分区信息，可以使用partprobe命令解决这个问题而不用重启系统，因为partprobe可以使kernel重新读取分区信息，从而避免重启系统。
执行`partprobe`命令即可。

dd if=/dev/zero of=test bs=1000M count=20