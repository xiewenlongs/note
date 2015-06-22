===============================================
linux 调优
===============================================


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

|

.. _http_core_tuning:

http server tuning
~~~~~~~~~~~~~~~~~~~~~~~


.. cssclass:: table-bordered
.. table::

    ================================                ==========================================================================
    内核参数                                        推荐值
    ================================                ==========================================================================
    fs.file-max                                     **4872754**  一个进程可以打开的最大文件句柄数， 这个值直接影响最大并发量
    net.ipv4.tcp_tw_reuse                           **1** TIME-WAIT 状态的套接字重用， http服务器会有大量TIME-WAIT状态socket
    net.core.somaxconn                              **262144** listen 函数的backlog参数上限
    net.ipv4.tcp_synack_retries                     **2** 为了打开对端的连接，内核需要发送一个SYN并附带一个回应前面一个SYN的
                                                    ACK。也就是所谓三次握手中的第二次握手。这个设置决定了内核放弃连接之前发
                                                    送SYN+ACK包的数量。
    net.ipv4.tcp_tw_recycle                         **0** 启动 timewait 快速回收, 这个值打开会有一定的危险，还是关闭比较好,
                                                    而且处于timewait状态的socket占用内存很少，现在机器不差这点内存。并且有
                                                    tcp_max_tw_buckets 来限制总timewait数，唯一的缺点就是log里报:
                                                    time wait bucket table overflow
    net.ipv4.tcp_syn_retries                        **2** 在内核放弃建立连接之前发送SYN包的数量。
    net.ipv4.tcp_fin_timeout                        **15** 服务器一个socket 在FIN-WAIT2 状态维护的时间
    net.ipv4.tcp_max_tw_buckets                     **18000** 服务器TIME-WAIT状态套接字的数量限制，如果超过这个数量，
                                                    新来的TIME-WAIT套接字会直接释放 (过多的TIME-WAIT 套接字很影响服务器性能),
                                                    默认是180000, 太大了.过多的timewait 是浪费资源
    net.ipv4.tcp_max_syn_backlog                    **181920** listen 函数的backlog参数
    net.core.netdev_max_backlog                     **262144** 当网卡接收包的速度大于内核处理速度，会有一个队列放这些包，
                                                    这个值是队列的最大值
    net.ipv4.ip_local_port_range                    **1024 65535** 定义了作为客户端， port的取值范围。 如果服务器要请求
                                                    其他server(作为客户端), 那么这个值还是有必要的
    net.ipv4.tcp_syncookies                         **1** 与性能无关，用于解决SYN攻击, 给每一个请求连接的IP地址分配一个
                                                    Cookie，如果短时间内连续受到某个IP的重复SYN报文，就认定是受到了攻击,
                                                    以后从这个IP地址来的包会被一概丢弃。
    net.ipv4.tcp_keepalive_time                     **600** 保活定时器的间隔, 时间约短，server回收资源越快, 每个alive最大
                                                    探测时间= (tcp_keepalive_intvl * tcp_keepalive_probes)
    net.ipv4.tcp_keepalive_intvl                    **30** 
    net.ipv4.tcp_keepalive_probes                   **3** 如果HTTP服务开启了keepalive, 这个参数就起作用了. server每两小时发送
                                                    一次 探测包，如果这个包没有回应，这个参数指定重试多少次, 默认是9 (太多)
    net.core.rmem_max                               这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    net.core.rmem_default                           这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    net.ipv4.tcp_rmem                               这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    net.ipv4.udp_rmem_min                           这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    net.core.wmem_max                               这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    net.core.wmem_default                           这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    net.ipv4.tcp_wmem                               这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    net.ipv4.udp_wmem_min                           这个值在一定程度上影响了并发量，但需要根据具体业务来调整
    ================================                ==========================================================================





其他
~~~~~~~~~~~~~~~~~~~~~~~


.. cssclass:: table-bordered
.. table::

    ================================                ==========================================================================
    内核参数                                        推荐值
    ================================                ==========================================================================
    net.ipv4.tcp_abort_on_overflow                  这个值决定了系统调用listen中backlog参数的作用。默认这个值为0，所以当
                                                    backlog队列已满时，新来的SYN请求， server不予理会，那么client会重发SYN,
                                                    那时backlog队列也许已经恢复了。 如果这个值设为1, 那么当backlog满的时候
                                                    新来的SYN, 服务器会直接返回RST, 导致"Connection reset by peer"
    ================================                ==========================================================================


**高并发配置模板**

    - :net.ipv4.tcp_no_metrics_save = 1
      默认情况下一个tcp连接关闭后,把这个连接曾经有的参数比如慢启动门限snd_sthresh,拥塞窗口snd_cwnd 还有srtt等信息保存到dst_entry中, 只要dst_entry 没有失效,下次新建立相同连接的时候就可以使用保存的参数来初始化这个连接.通常情况下是关闭的。

    - :net.unix.max_dgram_qlen = 1024
      unix socket 最大队列，默认为10, 明显不够

**其他**

    socket 连接中，打开TCP_DEFER_ACCEPT 选项
