-----------------
通用调优
-----------------

.. role:: bluetext

ulimit 限制
===================

   一般一台机器，先用 `ulimit -a` 看一下，系统是否有限制资源使用, 例如::

    > ulimit

    core file size          (blocks, -c) 0
    data seg size           (kbytes, -d) unlimited
    scheduling priority             (-e) 0
    file size               (blocks, -f) unlimited
    pending signals                 (-i) 93592
    max locked memory       (kbytes, -l) 64
    max memory size         (kbytes, -m) unlimited
    open files                      (-n) 1024
    pipe size            (512 bytes, -p) 8
    POSIX message queues     (bytes, -q) 819200
    real-time priority              (-r) 0
    stack size              (kbytes, -s) 8192
    cpu time               (seconds, -t) unlimited
    max user processes              (-u) 1024
    virtual memory          (kbytes, -v) unlimited
    file locks                      (-x) unlimited


查看系统当前的并发连接数
============================

    使用netstat 命令查看连接数::

        > netstat -n | awk '/^tcp/ {++state[$NF]} END {for(key in state) print key,"t",state[key]}'

            TIME_WAIT t 2727
            CLOSE_WAIT t 8
            SYN_SENT t 3
            FIN_WAIT1 t 56
            FIN_WAIT2 t 5915
            ESTABLISHED t 6813
            SYN_RECV t 87
            CLOSING t 6
            LAST_ACK t 130

    查看网络状态：sar -n DEV 1
    查看IO状态：iostat 1
    查看cpu状态：mpstat -P ALL


内核调优
==========================


    **高并发配置模板**

    - :fs.file_max = 9999999:
      最大的打开文件数

    - :net.ipv4.tcp_tw_reuse = 1:

      设置为１，　表示允许TIME_WAIT 状态的socket可以重用， 这对于服务器来说很有意义，
      因为服务器上总会有大量的TIME_WAIT状态的连接

    - :net.ipv4.tcp_keepalive_time = 600:

      默认２小时，设置小一点，可以更快清理无效连接

    - :net.ipv4.tcp_fin_timeout = 30:

      当服务器主动关闭连接时，　socket 保持在　FIN_WAIT状态的时间

    - :net.ipv4.tcp_max_tw_buckets = 5000:

      这个参数表示操作系统允许TIME_WAIT 套接字数量的最大值，如果超过这个数字，TIME_WAIT套接字将立刻被清除并
      打印警告信息。该参数默认为180000, 过多的TIME_WAIT套接字会使服务器变慢。

    - :net.ipv4.ip_local_port_range = 1024:

      本地端口的取值范围

    - :net.ipv4.tcp_rmem = 4096 32768 262142:

      TCP接收缓存的最大值，默认值，最小值

    - :net.ipv4.tcp_wmem = 4096 32768 262142:

      TCP接收发送的最大值，默认值，最小值

    - :net.ipv4.tcp_syncookies = 1:

        该参数与性能无关，用于解决TCP的SYN攻击

    - :net.ipv4.tcp_max_syn.backlog = 1024:

        TCP三次握手建立阶段接收SYN请求队列的最大长度，默认为1024, 设置的大一些，可以使nginx繁忙时也不至于丢失
        客户端发起的连接请求

    - :net.core.netdev_max_backlog = 8096:

        当网卡接收数据包的速度大于内核处理的速度时，会有一个参数表示该队列的最大值

    - :net.core.rmem_default = 262144:

        内核套接字接收缓冲区的默认大小

    - :net.core.wmem_default = 262144:

        内核套接字发送缓冲区的默认大小

    - :net.core.rmem_max = 2097152:

    - :net.core.wmem_max = 2097152:

    - :net.ipv4.tcp_no_metrics_save = 1
      默认情况下一个tcp连接关闭后,把这个连接曾经有的参数比如慢启动门限snd_sthresh,拥塞窗口snd_cwnd 还有srtt等信息保存到dst_entry中, 只要dst_entry 没有失效,下次新建立相同连接的时候就可以使用保存的参数来初始化这个连接.通常情况下是关闭的。
    - :net.unix.max_dgram_qlen = 1024
      unix socket 最大队列，默认为10, 明显不够

    **其他**

        socket 连接中，打开TCP_DEFER_ACCEPT 选项
