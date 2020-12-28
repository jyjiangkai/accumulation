# 硬盘驱动器了解资料
- 参阅：https://en.wikipedia.org/wiki/Hard_disk_drive_interface

---

## 1. 常见硬盘驱动器

Hard disk drives are accessed over one of a number of bus types, including parallel ATA (PATA, also called IDE or EIDE; described before the introduction of SATA as ATA), Serial ATA (SATA), SCSI, Serial Attached SCSI (SAS), and Fibre Channel. 

- parallel ATA (PTAT,也叫 IDE 或 EIDE)
    - ATA:Advanced Technology Attachment
- Serial ATA (SATA)
- SCSI
- Serial Attached SCSI (SAS)
- Fibre Channel

## 2. 早期的接口  Bit serial data interface
- 最早的HDD接口是 `位串行数据接口`，通过两条电缆链接HDD和controller，一条是为了control，一条是为了data。
- controller提供功能如 串行/并行转换，数据分离，磁道格式化功能，需要在格式化后与驱动器匹配，来确保可靠性。
- 每条control电缆可以为两个或多个驱动器服务，专用的较小的data电缆为每个驱动器服务。

- SMD(Storage Module Device)
    - Standard interface for many mini-computers in the 1970s and 1980s.
- ST-506
    - Standard interfaces for most small HDDs in the 1980s and early 1990s.
- ST412
    - Standard interfaces for most small HDDs in the 1980s and early 1990s.
- ESDI
    - A higher data rate follow on to the ST-506 family into the mid-1990s, superseded by SCSI

## 3. Word serial interfaces
- 用一根电缆连接HDD到一个bus适配器，用于conbined data/control。每个驱动器还具有一条额外的电源线，通常直接连接到电源设备。

- CTL-I(Controller Interface)
- SCSI(Small Computer System Interfac)
    - https://en.wikipedia.org/wiki/SCSI

    - SCSi is a set of standards for physically connecting and transferring data between computers and peripheral devices.
        - 是一组用于在计算机和外围设备之间物理连接和传输数据的标准。
    - Standard interfaces for most enterprise HDDs in this century; superseded by SAS
        - 1980s 大多数企业硬盘的标准接口，被SAS取代。
    
    - SCSI is most commonly used for hard disk drives and tape drives, but it can connect a wide range of other devices, including scanners and CD drives, although not all controllers can handle all devices. 
        - SCSI最常用于硬盘驱动器和磁带驱动器，但是它可以连接各种其他设备，包括扫描仪和CD驱动器，尽管并非所有控制器都可以处理所有设备。
    
    - Parallel interface
        - SCSI is derived from "SASI", the "Shugart Associates System Interface", developed circa 1978 and publicly disclosed in 1981.
        - A SASI controller provided a bridge between a hard disk drive's low-level interface and a host computer, which needed to read blocks of data. 
            - SASI控制器连接了硬盘驱动器的低级接口和需要读取数据块的主机。

        - Since its standardization in 1986, SCSI has been commonly used in the Amiga, Atari, Apple Macintosh and Sun Microsystems computer lines and PC server systems.
            - SCSI接口被广泛使用。
        - SCSI has never been popular in the low-priced IBM PC world, owing to the lower cost and adequate performance of ATA hard disk standard.
            - SCSI 在在价位上，没有ATA有优势。
        - However, SCSI drives and even SCSI RAIDs became common in PC workstations for video or audio production.
            - SCSI驱动器普遍用于音视频制作。
    
    - Modern SCSI
        - Recent physical versions of SCSI‍—‌Serial Attached SCSI (SAS), SCSI-over-Fibre Channel Protocol (FCP), and USB Attached SCSI (UAS)‍—‌break from the traditional parallel SCSI bus and perform data transfer via serial communications using point-to-point links. 
            - SCSI最新的物理版本 串行SCSI(SAS)，SCSI-over-Fibre Channel Protocol (FCP)，USB Attached SCSI (UAS)‍，通过点对点的串行通信进行数据传输。
        - Serial interfaces have a number of advantages over parallel SCSI, including higher data rates, simplified cabling, longer reach, improved fault isolation and full-duplex capability. 
            - 串行接口相对于并行SCSI有很多优势，包括 更高的数据速率，简化的布线，更长的覆盖范围，改进的故障隔离 和 全双工能力。
        - The primary reason for the shift to serial interfaces is the clock skew issue of high speed parallel interfaces, which makes the faster variants of parallel SCSI susceptible to problems caused by cabling and termination.
            - 导致由并行向串行转换的主要原因是 clock skew issue of high speed parallel interfaces
        - The non-physical iSCSI preserves the basic SCSI paradigm, especially the command set, almost unchanged, through embedding of SCSI-3 over TCP/IP. 
            - 通过在TCP / IP上嵌入SCSI-3，非物理 iSCSI 保留了基本SCSI的模式。
        - iSCSI uses logical connections instead of physical links and can run on top of any network supporting IP.
            - iSCSI使用逻辑连接而不是物理链接，并且可以在支持IP的任何网络之上运行。
        - The actual physical links are realized on lower network layers, independently from iSCSI. Predominantly, Ethernet is used which is also of serial nature.
            - 实际的物理链接独立于iSCSI在较低的网络层上实现。主要使用以太网，它也是串行的。
        - SCSI is popular on high-performance workstations, servers, and storage appliances. 
        - Instead of SCSI, modern desktop computers and notebooks typically use SATA interfaces for internal hard disk drives.

        - SCSI interfaces have often been included on computers from various manufacturers for use under Microsoft Windows, classic Mac OS, Unix, Commodore Amiga and Linux operating systems, either implemented on the motherboard or by the means of plug-in adaptors. With the advent of SAS and SATA drives, provision for parallel SCSI on motherboards was discontinued.


- Parallel ATA (PATA, IDE)
    - https://en.wikipedia.org/wiki/Parallel_ATA
    - originally AT Attachment, is an interface standard for the connection of storage devices such as hard disk drives, floppy disk drives, and optical disc drives in computers. 
        - 是一个连接存储设备的接口标准。
    - The ATA interface itself evolved in several stages from Western Digital's original Integrated Drive Electronics (IDE) interface.
        - ATA 从最初的IDE接口发展而来。
    - Standard HDD interface on all but enterprise HDDs until superseded by SATA
        - 除企业级硬盘外，所有硬盘上都有标准硬盘接口，直到被SATA取代
    - After the introduction of Serial ATA (SATA) in 2003, the original ATA was renamed to Parallel ATA, or PATA for short.
        - 在2003年推出SATA后，原来的ATA被重命名为PATA（Parallel ATA）
    - Parallel ATA cables have a maximum allowable length of 18 in (457 mm)
        - 电缆最大允许长度为457mm（18英寸）
    - It has largely been replaced by SATA in newer systems.
        - 在较新的系统中，已被SATA取代

    - Physical ATA interfaces became a standard component in all PCs, initially on host bus adapters, sometimes on a sound card but ultimately as two physical interfaces embedded in a Southbridge chip on a motherboard. 
        - 物理ATA接口成为所有PC的标准组件，有时在主机bus适配器上，有时在声卡上，最终是两个物理接口嵌入在主板的南桥芯片中。
    - Called the "primary" and "secondary" ATA interfaces, they were assigned to base addresses 0x1F0 and 0x170 on ISA bus systems. They were replaced by SATA interfaces.
        - 被称为 主要 和 次要 ATA接口，被分配给ISA总线系统的基地址0x1F0和0x170，已被SATA取代。

    - IDE (ATA-1)
    - EIDE ( Enhanced IDE )：提出外围设备也可以放置在ATA接口上。ATA接口上的设备一直是硬盘驱动器，但是只要符合该标准，任何形式的存储设备都可以放置在ATA接口上。
    - ATAPI ( ATA Packet Interface) : 
        - allowed ATA to be used for a variety of other devices that require functions beyond those necessary for hard disk drives. 
            - 允许ATA用于其他功能要求超出硬盘驱动器所需的其他设备。
        - ATAPI is actually a protocol allowing the ATA interface to carry SCSI commands and responses; therefore, all ATAPI devices are actually "speaking SCSI" other than at the electrical interface. 
            - ATAPI实际上是一种协议，它允许ATA接口携带SCSI命令和响应。 因此，除了电接口以外，所有ATAPI设备实际上都是“speaking SCSI”。
        - The SCSI commands and responses are embedded in "packets" (hence "ATA Packet Interface") for transmission on the ATA cable. 
            - SCSI命令和响应嵌入在“数据包”（因此称为“ATA Packet Interface”）中，以便在ATA电缆上传输。
        - ATAPI devices include CD-ROM and DVD-ROM drives, tape drives, and large-capacity floppy drives such as the Zip drive and SuperDisk drive.
            - ATAPI设备包括CD-ROM和DVD-ROM驱动器，磁带驱动器以及大容量软盘驱动器，例如Zip驱动器和SuperDisk驱动器。
    - UDMA and ATA-4
    - Ultra ATA
        - Ultra ATA, abbreviated UATA, is a designation that has been primarily used by Western Digital for different speed enhancements to the ATA/ATAPI standards.(对ATA/ATAPI标准不同速度增强的名称)
    - Current terminology
        - The terms "integrated drive electronics" (IDE), "enhanced IDE" and "EIDE" have come to be used interchangeably with ATA (now Parallel ATA, or PATA).（现在，IDE 、enhanced IDE 、EIDE 和 ATA(PATA) 互换使用 ）
    - Primacy and obsolescence
        - PATA 在某些系统中，提供链第三和第四主板接口，最多可以连接8个ATA设备到主板
        - 2003年推出SATA后不久，PATA使用下降，到2007年，某些PC芯片组，删除了对PATA的支持，还需要提供PATA的主板供应商必须包括一个额外的接口芯片。在较新的计算机中，即使存在PATA接口也很少使用，因为主板上通常提供4个或更多SATA连接器，而这些SATA设备都很常见。
        - 随着Western Digital退出PATA市场，带有PATA接口的HDD在2013年12月之后不再用于特殊应用。



## 4. Bit serial interfaces
- 用一根data/control电缆将HDD连接到主机bus接口适配器上。每个驱动器有一条额外的电源线，直接连到电源设备。

- SDI(Standard Disk Interconnect )
- FC(Fibre Channel)
- SATA(Serial ATA)
    - https://en.wikipedia.org/wiki/Serial_ATA
    - Most common interface for all but enterprise HDDs.
        - 除企业硬盘外，所有其他接口的最通用接口。
    - SATA is a computer bus interface that connects host bus adapters to mass storage devices such as hard disk drives, optical drives, and solid-state drives. 
    - SATA host adapters and devices communicate via a high-speed serial cable over two pairs of conductors.
        - SATA主机适配器和设备通过两对导体上的高速串行电缆进行通信。
    - To ensure backward compatibility with legacy ATA software and applications, SATA uses the same basic ATA and ATAPI command sets as legacy ATA devices.
        - 为确保与旧式ATA软件和应用程序向后兼容，SATA使用与旧式ATA设备相同的基本ATA和ATAPI命令集。

    - SATA has replaced parallel ATA in consumer desktop and laptop computers, PATA has mostly been replaced by SATA for any use.

    - Hot plug
        - The Serial ATA spec requires SATA device hot plugging
            - 串行ATA规范要求SATA设备热插拔
        - The powered host and device do not need to be in an idle state for safe insertion and removal, although unwritten data may be lost when power is removed.
            - 上电的主机和设备无需处于空闲状态即可安全地插入和卸下，尽管断开电源后未写的数据可能会丢失。
        - For eSATA, hot plugging is supported in AHCI mode only. IDE mode does not support hot plugging.
            - 对于eSATA，仅在AHCI模式下支持热插拔。 IDE模式不支持热插拔。
    - Advanced Host Controller Interface
        - Advanced Host Controller Interface (AHCI) is an open host controller interface published and used by Intel, which has become a de facto standard. 
            - AHCI 是由Intel发布并被使用的开放主机控制器接口，已成为标准。
        - It allows the use of advanced features of SATA such as hotplug and native command queuing (NCQ). 
            - 允许使用SATA的高级功能，例如热插拔和NCQ。
        - If AHCI is not enabled by the motherboard and chipset, SATA controllers typically operate in "IDE emulation" mode, which does not allow access to device features not supported by the ATA (also called IDE) standard.
            - 如果主板和芯片组未启用AHCI，则SATA控制器通常在“IDE emulation”模式下运行，该模式不允许访问ATA（也称为IDE）标准不支持的设备。

    
- SAS(Serial Attached SCSI)
    - https://en.wikipedia.org/wiki/Serial_Attached_SCSI
    - Most common interface for enterprise HDDs.
        - 企业硬盘最常用的接口。
    - SAS is a point-to-point serial protocol that moves data to and from computer-storage devices such as hard drives and tape drives. 
    
    - History
        - SAS-1: 3.0 Gbit/s, introduced in 2004
        - SAS-2: 6.0 Gbit/s, available since February 2009
        - SAS-3: 12.0 Gbit/s, available since March 2013
        - SAS-4: 22.5 Gbit/s called "24G",standard completed in 2017

    - Comparison with parallel SCSI
        - The SAS "bus" operates point-to-point while the SCSI bus is multidrop. Each SAS device is connected by a dedicated link to the initiator, unless an expander is used. If one initiator is connected to one target, there is no opportunity for contention; with parallel SCSI, even this situation could cause contention.
            - SCSI总线为多点连接，SAS“总线”是点对点。除非使用扩展器，否则每个SAS设备均通过专用链接连接到启动器。如果将一个启动器连接到一个目标，则没有竞争；对于并行SCSI，即使这种情况也可能引起竞争。
        - SAS has no termination issues and does not require terminator packs like parallel SCSI.
            - SAS没有终止问题，并且不需要如并行SCSI的终止包。
        - SAS eliminates clock skew.
            - SAS 消除了clock skew
        - SAS allows up to 65,535 devices through the use of expanders, while Parallel SCSI has a limit of 8 or 16 devices on a single channel.
            - SAS通过使用扩展器最多可容纳65,535个设备，而并行SCSI在单个通道上最多可限制8或16个设备。
        - SAS allows a higher transfer speed (3, 6 or 12 Gbit/s) than most parallel SCSI standards. SAS achieves these speeds on each initiator-target connection, hence getting higher throughput, whereas parallel SCSI shares the speed across the entire multidrop bus.
            - SAS可以提供​​比大多数并行SCSI标准更高的传输速度（3、6或12 Gbit / s）。 SAS在每个启动器-目标连接上都达到了这些速度，因此获得了更高的吞吐量，而并行SCSI在整个多点总线上共享了速度。
        - SAS devices feature dual ports, allowing for redundant backplanes or multipath I/O; this feature is usually referred to as the dual-domain SAS.
            - SAS设备具有双端口，可实现冗余背板或多路径I/O。此功能通常称为双域SAS。
        - SAS controllers may connect to SATA devices, either directly connected using native SATA protocol or through SAS expanders using Serial ATA Tunneling Protocol (STP).
            - SAS控制器可以使用本地SATA协议直接连接或通过使用串行ATA隧道协议（STP）的SAS扩展器连接到SATA设备。
        - Both SAS and parallel SCSI use the SCSI command set.
            - SAS和并行SCSI都使用SCSI命令集。
    
    - Comparison with SATA
        - SAS protocol provides for multiple initiators in a SAS domain, while SATA has no analogous provision.
            - SAS协议在SAS域中提供了多个启动器，而SATA没有类似的规定。
        - Most SAS drives provide tagged command queuing, while most newer SATA drives provide native command queuing.
            - 大多数SAS驱动器提供带标记的命令队列，而大多数较新的SATA驱动器提供本机命令队列。
        - SATA uses a command set that is based on the parallel ATA command set and then extended beyond that set to include features like native command queuing, hot-plugging, and TRIM. SAS uses the SCSI command set, which includes a wider range of features like error recovery, reservations and block reclamation. Basic ATA has commands only for direct-access storage. However SCSI commands may be tunneled through ATAPI for devices such as CD/DVD drives.
            - SATA使用基于并行ATA命令集的命令集，然后扩展到包括诸如本机命令队列，热插拔和TRIM的功能。- SAS使用SCSI命令集，其中包括更广泛的功能，例如错误恢复，保留和块回收。
            - 基本ATA仅具有用于直接访问存储的命令。但是，可以通过ATAPI为如CD/DVD驱动器之类的设备传送SCSI命令。
        - SAS hardware allows multipath I/O to devices while SATA (prior to SATA 2.0) does not. Per specification, SATA 2.0 makes use of port multipliers to achieve port expansion, and some port multiplier manufacturers have implemented multipath I/O using port multiplier hardware.
            - SAS硬件允许对设备的多路径I/O，而SATA（SATA 2.0之前的版本）则不允许。
            - 根据规范，SATA 2.0利用端口倍增器来实现端口扩展，并且某些端口倍增器制造商已使用端口倍增器硬件实现了多路径I/O。
        - SATA is marketed as a general-purpose successor to parallel ATA and has become common in the consumer market, whereas the more-expensive SAS targets critical server applications.
            - SATA作为并行ATA的通用后继产品销售，并已在消费市场上普及，而价格昂贵的SAS则针对关键服务器应用。
        - SAS error-recovery and error-reporting uses SCSI commands, which have more functionality than the ATA SMART commands used by SATA drives.
            - SAS错误恢复和错误报告使用SCSI命令，其功能比SATA驱动器使用的ATA SMART命令更多。
        - SAS uses higher signaling voltages (800–1,600 mV for transmit, and 275–1,600 mV for receive) than SATA (400–600 mV for transmit, and 325–600 mV for receive). The higher voltage offers (among other features) the ability to use SAS in server backplanes.
            - SAS使用的信号电压比SATA（发送400-600 mV，接收325-600 mV）的信号电压更高（发送800-1600 mV，接收275-1600 mV）。较高的电压（除其他功能外）提供了在服务器底板中使用SAS的能力。
        - Because of its higher signaling voltages, SAS can use cables up to 10 m (33 ft) long, whereas SATA has a cable-length limit of 1 m (3.3 ft) or 2 m (6.6 ft) for eSATA.
            - 由于其较高的信号电压，SAS可以使用最长10m（33 ft）的电缆，而SATA对于eSATA的电缆长度限制为1m（3.3 ft）或2m（6.6 ft）。
        - SAS is full duplex, whereas SATA is half duplex. The SAS transport layer can transmit data at the full speed of the link in both directions at once, so a SCSI command executing over the link can transfer data to and from the device simultaneously. However, because SCSI commands that can do that are rare, and an SAS link must be dedicated to an individual command at a time, this is generally not an advantage.
            - SAS是全双工，而SATA是半双工。 SAS传输层传输一次数据，可以以链路的全速在两个方向上传输，因此在链路上执行的SCSI命令可以同时向设备传输数据和从设备传输数据。但是，因为很少有能执行此操作的SCSI命令，并且一个SAS链接必须一次专用于单个命令，所以通常这不是优点。


## 5. virtio 和 kvm  qemu
- kvm 使用virtio API为各种操作系统的guest虚拟机提供半虚拟化支持。其中包括半虚拟以太网卡，disk I/O controller，balloon device，和 VGA 图形接口。

- kvm 负责虚拟 cpu 和 内存
    - KVM是Linuxkernel的一个模块。可以用命令modprobe去加载KVM模块。加载了模块后，才能进一步通过其他工具创建虚拟机。但仅有KVM模块是远远不够的，因为用户无法直接控制内核模块去作事情,你还必须有一个运行在用户空间的工具才行。

- qemu 虚拟I/O设备等

- qemu 
    - 通过 ioctl 调用 /dev/kvm 虚拟cpu 内存
    - 用 virtio 的方式虚拟I/O设备（半虚拟化）

- qemu和kvm
    - Qemu是一套独立的虚拟化解决方案，KVM是另一套虚拟化解决方案，不过因为这个方案实际上只实现了内核中对处理器(Intel VT, AMDSVM)虚拟化特性的支持，换言之，它缺乏设备虚拟化以及相应的用户空间管理虚拟机的工具，所以它借用了QEMU的代码并加以精简，连同KVM一起构成了另一个独立的虚拟化解决方案，不妨称之为：KVM+QEMU.。
    - KVM只模拟CPU和内存，因此一个客户机操作系统可以在宿主机上跑起来，但是你看不到它，无法和它沟通。于是，有人修改了QEMU代码，把他模拟CPU、内存的代码换成KVM，而网卡、显示器等留着，因此QEMU+KVM就成了一个完整的虚拟化平台。
    - KVM只是内核模块，用户并没法直接跟内核模块交互，需要借助用户空间的管理工具，而这个工具就是QEMU。KVM和QEMU相辅相成，QEMU通过KVM达到了硬件虚拟化的速度，而KVM则通过QEMU来模拟设备。

    - 简单直接的理解：QEMU是个计算机模拟器，而KVM为计算机的模拟提供加速功能。

    - QEMU发起ioctl来调用KVM接口，KVM则利用硬件扩展直接将虚拟机代码运行于主机之上，一旦vCPU需要操作设备寄存器，vCPU将会停止并退回到QEMU，QEMU去模拟出操作结果。
    - 虚拟机内存会被映射到QEMU的进程地址空间，在启动时分配。在虚拟机看来，QEMU所分配的主机上的虚拟地址空间为虚拟机的物理地址空间。
    - QEMU在主机用户态模拟虚拟机的硬件设备，vCPU对硬件的操作结果会在用户态进行模拟，如虚拟机需要将数据写入硬盘，实际结果是将数据写入到了主机中的一个镜像文件中。


- virtio virtio-blk  virtio-scsi
    - 参阅
        - https://www.linux-kvm.org/images/f/f9/2012-forum-virtio-blk-performance-improvement.pdf
        - https://ervikrant06.wordpress.com/2016/10/16/difference-between-virtio-blk-vs-virtio-scsi/
        - https://www.ovirt.org/develop/release-management/features/storage/virtio-scsi.html

        - https://www.ibm.com/developerworks/cn/linux/l-virtio/index.html

    - virtio: virtio 是对半虚拟化 hypervisor 中的一组通用模拟设备的抽象。
        - virtio-blk: 块设备（磁盘）
        - virtio-net
        - virtio-pci
        - virtio-balloon
        - virtio-console
    
    - virtio-blk: 是虚拟化KVM平台下虚拟磁盘的一种实现方式。（半虚拟化）
        - qemu模拟 ide scsi设备，是全虚拟化
    
    - virtio-scsi: 是vitio-blk 的一种改进。
        - https://www.ovirt.org/develop/release-management/features/storage/virtio-scsi.html
        - https://www.cnblogs.com/allcloud/p/5626485.html
        - https://www.ibm.com/developerworks/cn/linux/l-virtio/index.html

        - 与blk相比的提升
            - Improved scalability ：虚拟机可以连接到更多存储设备（virtio-scsi可以为每个虚拟SCSI适配器处理多个块设备）。
                - virtio-blk: 每个虚拟磁盘就要占用一个pci设备，这样一个虚拟机的磁盘个数就要受限于PCI设备的个数。
            - Standard command set ：virtio-scsi使用标准SCSI命令集，简化了新功能的添加。
            - Standard device naming ：virtio-scsi磁盘使用与裸机系统相同的路径。这简化了从物理到虚拟以及从虚拟到虚拟的迁移。
            - SCSI device passthrough ：virtio-scsi可以将物理存储设备直接呈现给guests。
