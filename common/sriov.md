软件的共享IO虚拟化技术
硬件辅助的IO虚拟化主要包括英特尔VT-d， AMD的IOMMU（AMD-Vi）和PCI-SIG 的SR-IOV。前两种属于Directed I/O。物理设备直接分配给虚拟机使用，通过硬件芯片完成GPA到MA 的翻译。IOV 通过在硬件设备中增加一个PCIe 设备，用于呈现一个PF或多个VF，从而可以将每个VF单独分配给不同的虚机使用。

SR-IOV简介
SR-IOV 技术是一种基于硬件的虚拟化解决方案，可提高性能和可伸缩性。SR-IOV 标准将一个PCIe的网络控制器虚拟化成多个PCIe设备，并且每个设备可以直接分配给一个虚拟机，允许在虚拟机之间高效共享 PCIe（Peripheral Component Interconnect Express，快速外设组件互连）设备，并且它是在硬件中实现的，可以获得能够与本机性能媲美的 I/O 性能。SR-IOV 规范定义了新的标准，根据该标准，创建的新设备可允许将虚拟机直接连接到 I/O 设备，越过了hypervisor与虚拟交换机层，这样可以带来低延迟和接近线缆的速度。

SR-IOV 两种功能（function）:
物理功能（Physical Functions，PF）：包含完整的PCIe 功能，包括SR-IOV的扩张能力，该功能用于SR-IOV的配置和管理。PF 能像普通 PCI 设备那样被发现、管理和配置。
虚拟功能（Virtual Functions，VF）：包含轻量级的PCIe 功能，它只能处理I/O, 包含数据移动所需要的最小的必要的资源。每个 VF 都是从 PF 中分离出来的。每个物理硬件都有一个 VF 数目的限制。一个 PF，能被虚拟成多个 VF 用于分配给多个虚拟机。

PF和VF：每个物理网卡（比如p1p1）就是一个PF，在开启SRIOV后，每个PF可以生成固定数量的VF，每个VF都可以在宿主上作为一张网卡直接使用，或者直通到QEMU虚拟机里作为虚拟机里的网卡使用，这就实现了bypass宿主内核。

查看网卡是否支持sriov：
[root@node-1 ark]# lspci -nn | grep Eth
[root@node-1 ark]#  lspci -s 05:00.0 -vvv | grep Capabilities
	Capabilities: [40] Power Management version 3
	Capabilities: [50] MSI: Enable- Count=1/1 Maskable+ 64bit+
	Capabilities: [70] MSI-X: Enable+ Count=64 Masked-
	Capabilities: [a0] Express (v2) Endpoint, MSI 00
	Capabilities: [e0] Vital Product Data
	Capabilities: [100 v1] Advanced Error Reporting
	Capabilities: [140 v1] Device Serial Number 90-e2-ba-ff-ff-76-86-a8
	Capabilities: [150 v1] Alternative Routing-ID Interpretation (ARI)
	Capabilities: [160 v1] Single Root I/O Virtualization (SR-IOV)
其中也可以获取最大支持的vfs 数量；

	Capabilities: [160 v1] Single Root I/O Virtualization (SR-IOV)
		IOVCap:	Migration-, Interrupt Message Number: 000
		IOVCtl:	Enable- Migration- Interrupt- MSE- ARIHierarchy+
		IOVSta:	Migration-
		Initial VFs: 64, Total VFs: 64, Number of VFs: 0, Function Dependency Link: 00
		VF offset: 128, stride: 2, Device ID: 10ed
		Supported Page Size: 00000553, System Page Size: 00000001
		Region 0: Memory at 00000000daa00000 (64-bit, non-prefetchable)
		Region 3: Memory at 00000000dab00000 (64-bit, non-prefetchable)
		VF Migration: offset: 00000000, BIR: 0
	Kernel driver in use: ixgbe
	Kernel modules: ixgbe


sriov无法进行热迁移的原因:

Passthrough I/O 就是将分配给网卡 DMA 的物理地址给到了 Guest，Guest 就可以直接访问这个物理地址来接收和发送数据报文。那么，显然的，使用了 Passthrough I/O 的 Guest 就不再是 “无状态” 的 Guest，它具有一个 Host 物理地址与 Guest 虚拟地址的映射表。也正是这一 “状态” 注定 Guest 无法随意的进行迁移。必须执行网卡 Detached 再 Reattach 的过程（重新建立地址映射表）

Red Hat Virtualization 中使用SR-IOV的前提条件：

CPU 必须支持IOMMU(input/output memory management unit 输入输出内存管理单元)（比如英特尔的 VT-d 或者AMD的 AMD-Vi，Power8 处理器默认支持IOMMU）
固件Firmware 必须支持IOMMU
CPU 根桥必须支持 ACS 或者ACS等价特性
PCIe 设备必须支持ACS 或者ACS等价特性
建议根桥和PCIe 设备中间的所有PCIe 交换设备都支持ACS，如果某个PCIe交换设备不支持ACS，其后的所有PCIe设备只能共享某个IOMMU 组，所以只能分配给1台虚机。


SR-IOV 技术是一种基于硬件的虚拟化解决方案，可提高性能和可伸缩性。
SR-IOV 标准允许在虚拟机之间高效共享 PCIe（Peripheral Component Interconnect Express，快速外设组件互连）设备，并且它是在硬件中实现的，可以获得能够与本机性能媲美的 I/O 性能。
SR-IOV 规范定义了新的标准，根据该标准，创建的新设备可允许将虚拟机直接连接到 I/O 设备。
SR-IOV 是 PCI-SIG 的一个IOV的规范，目的是提供一种标准规范，通过为虚拟机提供独立的内存空间、中断、DMA流，来绕过VMM实现数据移动。

SR-IOV 的优点

      SR-IOV 标准允许在 IO 来宾域之间高效共享 PCIe 设备。
      SR-IOV 设备可以具有数百个与某个物理功能 (Physical Function, PF) 关联的虚拟功能 (Virtual Function, VF)。
      VF 的创建可由 PF 通过设计用来开启 SR-IOV 功能的寄存器以动态方式进行控制。缺省情况下，SR-IOV 功能处于禁用状态，PF 充当传统 PCIe 设备。


1） 创建VF
首先通过lscpi 验证物理网卡，通过重新加载内核模块参数，来创建VF：

modprobe -r igb
modprobe igb max_vfs=7
或者在rc.local 中配置：

echo 2 > /sys/class/net/enp14s0f0/device/sriov_numvfs


2） 将VF分配给虚机

virsh nodedev-dumpxml pci_0000_0b_00_0 >> /tmp/new-i nterface. xml
virsh attach-device MyGuest /tmp/new-i nterface. xml --live --config



使用BCM57810网卡，在BIOS中打开SR-IOV。

在/sys/class/net/<eth0>/device/下会产生两个文件sriov_xxxx，分别代表PF总数和设定的VF个数，用echo 4> 设定。

完成之后在ip a可以看到新的网卡，在virsh中可以看到新的设备。在PF下会看到capability中为virt_function。


检查网卡是否支持SR-IOV
 1 [root@node1 ~]# lspci -nn | grep Eth
 2 08:00.0 Ethernet controller [0200]: Intel Corporation I350 Gigabit Network Connection [8086:1521] (rev 01)
 3 08:00.1 Ethernet controller [0200]: Intel Corporation I350 Gigabit Network Connection [8086:1521] (rev 01)
 4 
 5 [root@node1 ~]# lspci -s 08:00.0 -vvv | grep Capabilities 
 6         Capabilities: [40] Power Management version 3
 7         Capabilities: [50] MSI: Enable- Count=1/1 Maskable+ 64bit+
 8         Capabilities: [70] MSI-X: Enable+ Count=10 Masked-
 9         Capabilities: [a0] Express (v2) Endpoint, MSI 00
10         Capabilities: [100 v2] Advanced Error Reporting
11         Capabilities: [140 v1] Device Serial Number f8-0f-41-ff-ff-f4-af-6c
12         Capabilities: [150 v1] Alternative Routing-ID Interpretation (ARI)
13         Capabilities: [160 v1] Single Root I/O Virtualization (SR-IOV)
14         Capabilities: [1a0 v1] Transaction Processing Hints
15         Capabilities: [1c0 v1] Latency Tolerance Reporting
16         Capabilities: [1d0 v1] Access Control Services


PCI 与 PCIe 设备
PCI（Peripheral Component Interconnect，外设组件互连）：符合 PCI 总线标准的设备就被称为 PCI 设备，PCI 总线架构中可以包含多个 PCI 设备。
PCIe（Peripheral Component Interconnect Express，快速外设组件互连）：PCI Express，简称 PCIe，是电脑总线 PCI 的一种，它沿用了现有的 PCI 编程概念及通讯标准，但建基于更快的串行通信系统。是 Intel 提出的新一代的总线接口，PCI Express 采用了目前业内流行的点对点串行连接，比起 PCI 以及更早期的计算机总线的共享并行架构每个 PCIe 设备都有自己的专用连接，不需要向整个总线请求带宽，而且可以把数据传输率提高到一个很高的频率，达到 PCI 所不能提供的高带宽。

nic
kind = openvswitch/veth/vxlan/gretap/gre/bridge/tun
/sys/class/net/enp5s0f0/type

第一个成员表示硬件地址的格式，就是图中的“硬件类型”，它有如下这些：

#define ARPHRD_NETROM 0  /* From KA9Q: NET/ROM pseudo. */
#define ARPHRD_ETHER  1  /* Ethernet 10/100Mbps.  */
#define ARPHRD_EETHER 2  /* Experimental Ethernet.  */
#define ARPHRD_AX25 3  /* AX.25 Level 2.  */
#define ARPHRD_PRONET 4  /* PROnet token ring.  */
#define ARPHRD_CHAOS 5  /* Chaosnet.  */
#define ARPHRD_IEEE802 6  /* IEEE 802.2 Ethernet/TR/TB.  */
#define ARPHRD_ARCNET 7  /* ARCnet.  */
#define ARPHRD_APPLETLK 8  /* APPLEtalk.  */
#define ARPHRD_DLCI 15  /* Frame Relay DLCI.  */
#define ARPHRD_ATM 19  /* ATM.  */
#define ARPHRD_METRICOM 23  /* Metricom STRIP (new IANA id).  */
#define ARPHRD_IEEE1394 24  /* IEEE 1394 IPv4 - RFC 2734.  */
#define ARPHRD_EUI64  27  /* EUI-64.  */
#define ARPHRD_INFINIBAND 32  /* InfiniBand.  */


