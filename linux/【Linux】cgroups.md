## cgroups
### 简介
cgroups(Control Groups) 是 linux 内核提供的一种机制，这种机制可以根据需求把一系列系统任务及其子任务整合(或分隔)到按资源划分等级的不同组内，从而为系统资源管理提供一个统一的框架。
简单说，cgroups 可以限制、记录任务组所使用的物理资源。本质上来说，cgroups 是内核附加在程序上的一系列钩子(hook)，通过程序运行时对资源的调度触发相应的钩子以达到资源追踪和限制的目的。

**为什么要了解 cgroups**
在以容器技术为代表的虚拟化技术大行其道的时代了解 cgroups 技术是非常必要的！比如我们可以很方便的限制某个容器可以使用的 CPU、内存等资源，这究竟是如何实现的呢？通过了解 cgroups 技术，我们可以窥探到 linux 系统中整个资源限制系统的脉络。从而帮助我们更好的理解和使用 linux 系统。

**cgroups 的主要作用**
实现 cgroups 的主要目的是为不同用户层面的资源管理提供一个统一化的接口。从单个任务的资源控制到操作系统层面的虚拟化，它最主要的作用，就是限制一个进程组能够使用的资源上限，包括CPU、内存、磁盘、网络带宽等等。此外，还能够对进程进行优先级设置，以及将进程挂起和恢复等操作。cgroups 提供了四大功能：

 - 资源限制：cgroups 可以对任务需要的资源总额进行限制。比如设定任务运行时使用的内存上限，一旦超出就发 OOM。
 - 优先级分配：通过分配的 CPU 时间片数量和磁盘 IO 带宽，实际上就等同于控制了任务运行的优先级。
 - 资源统计：cgoups 可以统计系统的资源使用量，比如 CPU 使用时长、内存用量等。这个功能非常适合当前云端产品按使用量计费的方式。
 - 任务控制：cgroups 可以对任务执行挂起、恢复等操作。

 ### 相关概念
  - **Task(任务)** 在 linux 系统中，内核本身的调度和管理并不对进程和线程进行区分，只是根据 clone 时传入的参数的不同来从概念上区分进程和线程。这里使用 task 来表示系统的一个进程或线程。

 - **Cgroup(控制组)** cgroups 中的资源控制以 cgroup 为单位实现。Cgroup 表示按某种资源控制标准划分而成的任务组，包含一个或多个子系统。一个任务可以加入某个 cgroup，也可以从某个 cgroup 迁移到另一个 cgroup。

 - **Subsystem(子系统)** cgroups 中的子系统就是一个资源调度控制器(又叫 controllers)。比如 CPU 子系统可以控制 CPU 的时间分配，内存子系统可以限制内存的使用量。以笔者使用的 Ubuntu 16.04.3 为例，其内核版本为 4.10.0，支持的 subsystem 如下( cat /proc/cgroups)：
    * blkio         对块设备的 IO 进行限制。
    * cpu           限制 CPU 时间片的分配，与 cpuacct 挂载在同一目录。
    * cpuacct     生成 cgroup 中的任务占用 CPU 资源的报告，与 cpu 挂载在同一目录。
    * cpuset       给 cgroup 中的任务分配独立的 CPU(多处理器系统) 和内存节点。
    * devices     允许或禁止 cgroup 中的任务访问设备。
    * freezer      暂停/恢复 cgroup 中的任务。
    * hugetlb     限制使用的内存页数量。              
    * memory    对 cgroup 中的任务的可用内存进行限制，并自动生成资源占用报告。
    * net_cls      使用等级识别符（classid）标记网络数据包，这让 Linux 流量控制器（tc 指令）可以识别来自特定 cgroup 任务的数据包，并进行网络限制。
    * net_prio    允许基于 cgroup 设置网络流量(netowork traffic)的优先级。
    * perf_event  允许使用 perf 工具来监控 cgroup。
    * pids          限制任务的数量。

 - **Hierarchy(层级)** 层级有一系列 cgroup 以一个树状结构排列而成，每个层级通过绑定对应的子系统进行资源控制。层级中的 cgroup 节点可以包含零个或多个子节点，子节点继承父节点挂载的子系统。一个操作系统中可以有多个层级。

 ### cgroups子系统
这里简单介绍几个常见子系统的概念和用法，包括cpu、cpuacct、cpuset、memory、blkio。
linux cgroups所在路径为：/sys/fs/cgroup

#### cpu子系统
cpu子系统限制对CPU的访问，每个参数独立存在于cgroups虚拟文件系统的伪文件中，参数解释如下：

 - cpu.shares: cgroup对时间的分配。比如cgroup A设置的是1，cgroup B设置的是2，那么B中的任务获取cpu的时间，是A中任务的2倍。
 - cpu.cfs_period_us: 完全公平调度器的调整时间配额的周期。
 - cpu.cfs_quota_us: 完全公平调度器的周期当中可以占用的时间。
 - cpu.stat 统计值
    - nr_periods 进入周期的次数
    - nr_throttled 运行时间被调整的次数
    - throttled_time 用于调整的时间
#### cpuacct子系统
子系统生成cgroup任务所使用的CPU资源报告，不做资源限制功能。

 - cpuacct.usage: 该cgroup中所有任务总共使用的CPU时间（ns纳秒）
 - cpuacct.stat: 该cgroup中所有任务总共使用的CPU时间，区分user和system时间。
 - cpuacct.usage_percpu: 该cgroup中所有任务使用各个CPU核数的时间。
通过cpuacct如何计算CPU利用率呢？可以通过cpuacct.usage来计算整体的CPU利用率，计算如下：
```
# 1. 获取当前时间（纳秒）
tstart=$(date +%s%N)
# 2. 获取cpuacct.usage
cstart=$(cat /xxx/cpuacct.usage)
# 3. 间隔5s统计一下
sleep 5
# 4. 再次采点
tstop=$(date +%s%N)
cstop=$(cat /xxx/cpuacct.usage)
# 5. 计算利用率
($cstop - $cstart) / ($tstop - $tstart) * 100
```
#### cpuset子系统
适用于分配独立的CPU节点和Mem节点，比如将进程绑定在指定的CPU或者内存节点上运行，各参数解释如下：

 - cpuset.cpus: 可以使用的cpu节点
 - cpuset.mems: 可以使用的mem节点
 - cpuset.memory_migrate: 内存节点改变是否要迁移？
 - cpuset.cpu_exclusive: 此cgroup里的任务是否独享cpu？
 - cpuset.mem_exclusive： 此cgroup里的任务是否独享mem节点？
 - cpuset.mem_hardwall: 限制内核内存分配的节点（mems是用户态的分配）
 - cpuset.memory_pressure: 计算换页的压力。
 - cpuset.memory_spread_page: 将page cache分配到各个节点中，而不是当前内存节点。
 - cpuset.memory_spread_slab: 将slab对象(inode和dentry)分散到节点中。
 - cpuset.sched_load_balance: 打开cpu set中的cpu的负载均衡。
 - cpuset.sched_relax_domain_level: the searching range when migrating tasks
 - cpuset.memory_pressure_enabled: 是否需要计算 memory_pressure?
#### memory子系统
memory子系统主要涉及内存一些的限制和操作，主要有以下参数：

 - memory.usage_in_bytes # 当前内存中的使用量
 - memory.memsw.usage_in_bytes # 当前内存和交换空间中的使用量
 - memory.limit_in_bytes # 设置or查看内存使用量
 - memory.memsw.limit_in_bytes # 设置or查看 内存加交换空间使用量
 - memory.failcnt # 查看内存使用量被限制的次数
 - memory.memsw.failcnt # - 查看内存和交换空间使用量被限制的次数
 - memory.max_usage_in_bytes # 查看内存最大使用量
 - memory.memsw.max_usage_in_bytes # 查看最大内存和交换空间使用量
 - memory.soft_limit_in_bytes # 设置or查看内存的soft limit
 - memory.stat # 统计信息
 - memory.use_hierarchy # 设置or查看层级统计的功能
 - memory.force_empty # 触发强制page回收
 - memory.pressure_level # 设置内存压力通知
 - memory.swappiness # 设置or查看vmscan swappiness 参数
 - memory.move_charge_at_immigrate # 设置or查看 controls of moving charges?
 - memory.oom_control # 设置or查看内存超限控制信息(OOM killer)
 - memory.numa_stat # 每个numa节点的内存使用数量
 - memory.kmem.limit_in_bytes # 设置or查看 内核内存限制的硬限
 - memory.kmem.usage_in_bytes # 读取当前内核内存的分配
 - memory.kmem.failcnt # 读取当前内核内存分配受限的次数
 - memory.kmem.max_usage_in_bytes # 读取最大内核内存使用量
 - memory.kmem.tcp.limit_in_bytes # 设置tcp 缓存内存的hard limit
 - memory.kmem.tcp.usage_in_bytes # 读取tcp 缓存内存的使用量
 - memory.kmem.tcp.failcnt # tcp 缓存内存分配的受限次数
 - memory.kmem.tcp.max_usage_in_bytes # tcp 缓存内存的最大使用量
#### blkio子系统 - block io
主要用于控制设备IO的访问。有两种限制方式：权重和上限，权重是给不同的应用一个权重值，按百分比使用IO资源，上限是控制应用读写速率的最大值。

按权重分配IO资源：

 - blkio.weight：填写 100-1000 的一个整数值，作为相对权重比率，作为通用的设备分配比。
 - blkio.weight_device： 针对特定设备的权重比，写入格式为 device_types:node_numbers weight，空格前的参数段指定设备，weight参数与blkio.weight相同并覆盖原有的通用分配比。
按上限限制读写速度：

 - blkio.throttle.read_bps_device：按每秒读取块设备的数据量设定上限，格式device_types:node_numbers bytes_per_second。
 - blkio.throttle.write_bps_device：按每秒写入块设备的数据量设定上限，格式device_types:node_numbers bytes_per_second。
 - blkio.throttle.read_iops_device：按每秒读操作次数设定上限，格式device_types:node_numbers operations_per_second。
 - blkio.throttle.write_iops_device：按每秒写操作次数设定上限，格式device_types:node_numbers operations_per_second
针对特定操作 (read, write, sync, 或 async) 设定读写速度上限

 - blkio.throttle.io_serviced：针对特定操作按每秒操作次数设定上限，格式device_types:node_numbers operation operations_per_second
 - blkio.throttle.io_service_bytes：针对特定操作按每秒数据量设定上限，格式device_types:node_numbers operation bytes_per_second


 ### Ref
 https://zhuanlan.zhihu.com/p/81668069
 https://gist.github.com/mcastelino/b8ce9a70b00ee56036dadd70ded53e9f