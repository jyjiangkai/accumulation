oslo.messaging
定义
oslo.messaging库就是把rabbitmq的Python库做了封装，在openstack中调用RPC通信就要调用oslo.messaging库，为RPC和事件通知提供一套统一的接口。

**oslo.messaging对象**
1. Transport
   Transport(传输层)主要实现RPC底层的通信(比如socket)以及事件循环、多线程等其他功能。可以通过URL来获得指向不同Transport实现的句柄。URL格式：
   Transport://user:pass@host1:port[,hostN:portN]/virtual_host
   从cfg对象读取transport_url、rpc_backend、control_exchange信息构造Transport对象。用户可以使用oslo.messaging.get_transport函数来获得transport对象实例的句柄。

2. Target
Target封装指定某一个消息最终目的地的所有信息。
Target对象属性
参数 = 默认值        说 明
exchange=None      (字符串类型)交换器，topic所属的范围，默认使用配置文件中的control_exchange选项
topic=None         (字符串类型)一个topic表示服务器暴露的一组接口(可被远程调用的方法)。 允许多个服务器暴露同一组接口，消息会轮询发给多个服务器中的某一个
namespace=None     (字符串类型)表示服务器暴露的某个特定的接口(多个可被远程调用的方法)
version=None       服务器所暴露的接口支持M.N类型的版本号
server=None        (字符串类型)客户端可以指定 此参数要求消息的目的地是某个特定的服务器， 而不是 topic 一组服务器的一台。
fanout=None        (布尔类型)当设置为真时，消息会被广播发送给 同属某个topic的所有服务器，而不是

**注意：不同场景下构造Target对象需要不同的参数：**
(1)、创建RPC服务器时，需要topic和server参数，exchange参数可选；指定一个endpoint的target时，namespace和version是可选的
(2)、客户端发送消息时，需要topic参数，其他可选

