##Linux平时小结

####SUSE两台机器时间如何同步
在要求单板之间的时间一致的情况下，如果手工使用date命令修改，很难做到时间一致。下面的命令，可以单板之间时间同步。
```Linux44：~# rcntp restart```
```Shutting down network time protocol daemon (NTPD) done```
```Starting network time protocol daemon (NTPD) done```
```Linux45：~# ntpdate 10.71.160.44```
```10 Mar 00:27:06 ntpdate[11072]: step time server 10.71.160.44 offset -3.089698 sec```
####SUSE如何修改主机名
整个操作使用root用户
1.使用hostname命令，但重启机器之后会失效
```# hostname newname```
2.修改/etc/HOSTNAME，重启也可以生效
```# vi /etc/HOSTNAME```
3.修改/etc/hosts，供域名解析
```# vi /etc/hosts```
