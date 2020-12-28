##nvidia-smi
###NVIDIA-SMI介绍
nvidia-smi简称NVSMI，提供监控GPU使用情况和更改GPU状态的功能，是一个跨平台工具，它支持所有标准的NVIDIA驱动程序支持的Linux发行版以及从WindowsServer 2008 R2开始的64位的系统。该工具是N卡驱动附带的，只要安装好驱动后就会有它。SMI全称是串行管理接口(Serial Management Interface)。
Windows下程序位置：C:\Program Files\NVIDIACorporation\NVSMI\nvidia-smi.exe。Linux下程序位置：/usr/bin/nvidia-smi，由于所在位置已经加入PATH路径，可直接输入nvidia-smi运行。

###NVIDIA-SMI命令系列详解
####nvidia-smi
显示所有GPU的当前信息状态
表头释义：
GPU：本机中的GPU编号
Name：GPU 类型
Persistence-M：
Fan：显示风扇转速，数值在0到100%之间，是计算机的期望转速，如果计算机不是通过风扇冷却或者风扇坏了，显示出来就是N/A；
Temp：显卡内部的温度，单位是摄氏度；
Perf：表征性能状态，从P0到P12，P0表示最大性能，P12表示状态最小性能；
Pwr:Usage/Cap：能耗表示；
Bus-Id：涉及GPU总线的相关信息；
Disp.A：是Display Active的意思，表示GPU的显示是否初始化；
Memory Usage：显存的使用率；
Volatile GPU-Util：浮动的GPU利用率；
Compute M：计算模式；
Processes 显示每块GPU上每个进程所使用的显存情况。

附加选项：
nvidia-smi –i xxx
指定某个GPU
nvidia-smi –l xxx
动态刷新信息（默认5s刷新一次），按Ctrl+C停止，可指定刷新频率，以秒为单位
nvidia-smi –f xxx
将查询的信息输出到具体的文件中，不在终端显示

####nvidia-smi -q
查询所有GPU的当前详细信息
附加选项：
nvidia-smi –q –u
显示单元而不是GPU的属性
nvidia-smi –q –i xxx
指定具体的GPU或unit信息
nvidia-smi –q –f xxx
将查询的信息输出到具体的文件中，不在终端显示
nvidia-smi –q –x
将查询的信息以xml的形式输出
nvidia-smi -q –d xxx
指定显示GPU卡某些信息，xxx参数可以为MEMORY, UTILIZATION, ECC, TEMPERATURE, POWER,CLOCK, COMPUTE, PIDS, PERFORMANCE, SUPPORTED_CLOCKS, PAGE_RETIREMENT,ACCOUNTING
nvidia-smi –q –l xxx
动态刷新信息，按Ctrl+C停止，可指定刷新频率，以秒为单位

nvidia-smi --query-gpu=gpu_name,gpu_bus_id,vbios_version--format=csv
选择性查询选项，可以指定显示的属性选项
可查看的属性有：timestamp，driver_version，pci.bus，pcie.link.width.current等。（可查看nvidia-smi--help-query–gpu来查看有哪些属性）

####设备修改选项
可以手动设置GPU卡设备的状态选项
nvidia-smi –pm 0/1
设置持久模式：0/DISABLED,1/ENABLED
nvidia-smi –e 0/1
切换ECC支持：0/DISABLED, 1/ENABLED
nvidia-smi –p 0/1
重置ECC错误计数：0/VOLATILE, 1/AGGREGATE
nvidia-smi –c
设置计算应用模式：0/DEFAULT,1/EXCLUSIVE_PROCESS,2/PROHIBITED
nvidia-smi –r
GPU复位
nvidia-smi –vm
设置GPU虚拟化模式
nvidia-smi –ac xxx,xxx
设置GPU运行的工作频率。e.g. nvidia-smi –ac2000,800
nvidia-smi –rac
将时钟频率重置为默认值
nvidia-smi –acp 0/1
切换-ac和-rac的权限要求，0/UNRESTRICTED, 1/RESTRICTED
nvidia-smi –pl
指定最大电源管理限制（瓦特）
nvidia-smi –am 0/1
启用或禁用计数模式，0/DISABLED,1/ENABLED
nvidia-smi –caa
清除缓冲区中的所有已记录PID，0/DISABLED,1/ENABLED

####nvidia-smi dmon
设备监控命令，以滚动条形式显示GPU设备统计信息。
GPU统计信息以一行的滚动格式显示，要监控的指标可以基于终端窗口的宽度进行调整。 监控最多4个GPU，如果没有指定任何GPU，则默认监控GPU0-GPU3（GPU索引从0开始）。
附加选项：
nvidia-smi dmon –i xxx
用逗号分隔GPU索引，PCI总线ID或UUID
nvidia-smi dmon –d xxx
指定刷新时间（默认为1秒）
nvidia-smi dmon –c xxx
显示指定数目的统计信息并退出
nvidia-smi dmon –s xxx
指定显示哪些监控指标（默认为puc），其中：
p：电源使用情况和温度（pwr：功耗，temp：温度）
u：GPU使用率（sm：流处理器，mem：显存，enc：编码资源，dec：解码资源）
c：GPU处理器和GPU内存时钟频率（mclk：显存频率，pclk：处理器频率）
v：电源和热力异常
m：FB内存和Bar1内存
e：ECC错误和PCIe重显错误个数
t：PCIe读写带宽
nvidia-smi dmon –o D/T
指定显示的时间格式D：YYYYMMDD，THH:MM:SS
nvidia-smi dmon –f xxx
将查询的信息输出到具体的文件中，不在终端显示

####nvidia-smi pmon
进程监控命令，以滚动条形式显示GPU进程状态信息。
GPU进程统计信息以一行的滚动格式显示，此工具列出了GPU所有进程的统计信息。要监控的指标可以基于终端窗口的宽度进行调整。 监控最多4个GPU，如果没有指定任何GPU，则默认监控GPU0-GPU3（GPU索引从0开始）。
附加选项：
nvidia-smi pmon –i xxx
用逗号分隔GPU索引，PCI总线ID或UUID
nvidia-smi pmon –d xxx
指定刷新时间（默认为1秒，最大为10秒）
nvidia-smi pmon –c xxx
显示指定数目的统计信息并退出
nvidia-smi pmon –s xxx
指定显示哪些监控指标（默认为u），其中：
u：GPU使用率
m：FB内存使用情况
nvidia-smi pmon –o D/T
指定显示的时间格式D：YYYYMMDD，THH:MM:SS
nvidia-smi pmon –f xxx
将查询的信息输出到具体的文件中，不在终端显示


####常用场景
1. nvidia-smi -L 命令：列出所有可用的 NVIDIA 设备
2. nvidia-smi topo --matrix 命令：查看系统拓扑
要正确地利用更先进的 NVIDIA GPU 功能 (如 GPUDirect)，使用系统拓扑正确配置往往是至关重要的。该拓扑指的是 PCI Express 设备 (GPUs, InfiniBand HCAs, storage controllers, 等) 如何互相连接以及如何连接到系统的CPU。如果使用不正确的拓扑, 某些功能可能会减慢甚至停止工作。
3. nvidia-smi -q -d CLOCK 命令：查看当前的 GPU 时钟速度、默认时钟速度和最大可能的时钟速度
4. nvidia-smi -q -d SUPPORTED_CLOCKS 命令：显示每个 GPU 的可用时钟速度列表
5. watch -n 10 nvidia-smi 命令：周期性的输出显卡的使用情况，命令行参数-n后边跟的是执行命令的周期，以s为单位。

