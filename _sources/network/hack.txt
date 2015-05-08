===============================================
hack
===============================================


Dos 攻击
---------------------------------------

Dos 攻击的种类很多，但原理都是发送大量的请求，使server不能服务正常的请求. 知名的攻击类别有:

- SYN Flood攻击
- DDos 分布式Dos攻击
- Pingofdeath 死亡之Ping

|

如何攻击
---------------------------------------

**Http-Alive**

善用Http服务的Keep-Alive来进行攻击，如果发送大量请求需要创建大量同Http服务的连接, 那么连接数太大时，容易被服务器上游的防火墙防御, 所以最要用已建立的连接，发送大量请求


**HEAD请求**

如果靠发送大量的GET, POST请求来进行攻击，服务端对每个GET/POST请求都会response, 那么服务器的response可能在还没有攻击完成前，就把hacker机器的带宽沾满了,从而无法继续进行攻击. 而HEAD请求不同， 它只要求server返回HTTP头部, 详情见 :ref:`HTTP HEAD协议<http_protocol_head>`

**Choose your target wisely**

Dos攻击的目的是消耗server的CPU和RAM, 所以挑选请求的时候，要挑选CPU密集型的请求，如搜索(搜索词语注意随机性)

**Keep dead工具**

下载链接: http://www.esrun.co.uk/blog/wp-content/uploads/2011/03/Keep-Dead.zip

防范
~~~~~~~~~~~~~~~~~~~~~~~

server上游安装防火墙, 过滤Dos攻击

**SYN Flood攻击**

设置net.ipv4.tcp_syncookies = 1，就是给每一个请求连接的IP地址分配一个Cookie，如果短时间内连续受到某个IP的重复SYN报文，就认定是受到了攻击，以后从这个IP地址来的包会被一概丢弃。
