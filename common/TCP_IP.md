##TCP/IP

####TCP/IP七层模型
7.应用层  提供应用程序间通信			|
6.表示层  处理数据格式、数据加密等		|}高层：负责主机之间的数据传输
5.会话层  建立、维护和管理会话			|
4.传输层  建立主机端到端连接
3.网络层  寻址和路由选择				|
2.数据链路层  提供介质访问、链路管理等	|}底层：负责网络数据传输
1.物理层  比特流传输					|

####TCP三次握手
第一次握手：Client将标志位SYN置为1，随机产生一个值seq=a，并将该数据包发送给Server，Client进入SYN_SENT状态，等待Server确认。SYN：同步序列编号
第二次握手：Server收到数据包后由标志位SYN=1知道Client请求建立连接，Server将标志位SYN和ACK都置为1，ack=a+1，随机产生一个值seq=b，并将该数据包发送给Client以确认连接请求，Server进入SYN_RCVD状态。
第三次握手：Client收到确认后，检查ack是否为a+1，ACK是否为1，如果正确则将标志位ACK置为1，ack=b+1，并将该数据包发送给Server，Server检查ack是否为b+1，ACK是否为1，如果正确则连接建立成功，Client和Server进入ESTABLISHED状态，完成三次握手，随后Client与Server之间可以开始传输数据了。
	
DSF Client												DSF Server
   |---------------------- SYN=1 seq=a -------------------->|
   |<--------------- SYN=1 ACK=1 ack=a+1 seq=b -------------|
   |---------------------- ACK=1 ack=b+1 ------------------>|
   |                                                        |
   
####TCP四次挥手   
第一次挥手：Client将标志位FIN置为1，随机产生一个值seq=a，并将该数据包发送给Server，用来关闭Client到Server的数据传送，Client进入FIN_WAIT_1状态。
第二次挥手：Server收到数据包后，将标志位ACK置为1，ack=a+1，并将该数据包发送给Client，Server进入CLOSE_WAIT状态。
第三次挥手：Server将标志位FIN置为1，随机产生一个值seq=b，并将该数据包发送给Client，用来关闭Server到Client的数据传送，Server进入LAST_ACK状态。
第四次挥手：Client收到数据包后，将标志位ACK置为1，ack=a+1，并将该数据包发送给Server，Client进入CLOSED状态，完成四次挥手。

DSF Client												DSF Server
   |---------------------- FIN=1 seq=a -------------------->|
   |<--------------------- ACK=1 ack=a+1 -------------------|
   |                                                        |
   |<--------------------- FIN=1 seq=b ---------------------|
   |---------------------- ACK=1 ack=b+1 ------------------>|
   |                                                        |
   
   