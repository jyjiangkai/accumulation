## linux命令--dd

### 功能
dd的主要功能是拷贝文件（默认从标准输入拷贝到标准输出，这意味dd可以在管道中使用）。不过在拷贝文件的过程中，它给用户提供了很多灵活的选项来控制拷贝的过程。提供的选项我归纳有如下三点：

（1）可以自由控制读取源文件和写目的文件的块大小（block size）。

（2）可以自由控制从源文件的什么位置开始读以及从什么位置往目的文件写。

（3）在源文件读取数据后，在往目的文件写之前，提供了很多灵活有用的转换规则（conversion rules）。

（4）其他赋值功能。例如status用来控制信息输出。

参数注释：

    if=文件名：输入文件名，缺省为标准输入。即指定源文件。< if=input file >
    of=文件名：输出文件名，缺省为标准输出。即指定目的文件。< of=output file >
    ibs=bytes：一次读入bytes个字节，即指定一个块大小为bytes个字节。
    obs=bytes：一次输出bytes个字节，即指定一个块大小为bytes个字节。
    bs=bytes：同时设置读入/输出的块大小为bytes个字节。
    cbs=bytes：一次转换bytes个字节，即指定转换缓冲区大小。
    skip=blocks：从输入文件开头跳过blocks个块后再开始复制。
    seek=blocks：从输出文件开头跳过blocks个块后再开始复制。
    注意：通常只用当输出文件是磁盘或磁带时才有效，即备份到磁盘或磁带时才有效。
    count=blocks：仅拷贝blocks个块，块大小等于ibs指定的字节数。
    conv=conversion：用指定的参数转换文件。

        ascii：转换ebcdic为ascii
        ebcdic：转换ascii为ebcdic
        ibm：转换ascii为alternate ebcdic
        block：把每一行转换为长度为cbs，不足部分用空格填充
        unblock：使每一行的长度都为cbs，不足部分用空格填充
        lcase：把大写字符转换为小写字符
        ucase：把小写字符转换为大写字符
        swab：交换输入的每对字节
        noerror：出错时不停止
        notrunc：不截短输出文件
        sync：将每个输入块填充到ibs个字节，不足部分用空（NUL）字符补齐。

### dd使用实例
1. dd命令可以轻易实现创建指定大小的文件，如
dd if=/dev/zero of=test bs=1M count=1000
 
在当前目录下会生成一个1000M的test文件，文件内容为全0（因从/dev/zero中读取，/dev/zero为0源），但是这样为实际写入硬盘，文件产生速度取决于硬盘读写速度，如果欲产生超大文件，速度很慢。在某种场景下，我们只想让文件系统认为存在一个超大文件在此，但是并不实际写入硬盘

则可以
dd if=/dev/zero of=test bs=1M count=0 seek=100000
 
此时创建的文件在文件系统中的显示大小为100000MB，但是并不实际占用block，因此创建速度与内存速度相当，seek的作用是跳过输出文件中指定大小的部分，这就达到了创建大文件，但是并不实际写入的目的。当然，因为不实际写入硬盘，所以你在容量只有10G的硬盘上创建100G的此类文件都是可以的。


2. 随机生成1百万个1K的文件
seq 1000000 | xargs -i dd if=/dev/zero of={}.dat bs=1024 count=1