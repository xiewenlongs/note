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
    net.ipv4.tcp_keepalive_time                     **7200** 保活定时器的间隔, 时间约短，server回收资源越快
    net.ipv4.tcp_fin_timeout                        **15** 服务器一个socket 在FIN-WAIT2 状态维护的时间
    net.ipv4.tcp_max_tw_buckets                     **180000** 服务器TIME-WAIT状态套接字的数量限制，如果超过这个数量，
                                                    新来的TIME-WAIT套接字会直接释放 (过多的TIME-WAIT 套接字很影响服务器性能)
    net.ipv4.tcp_max_syn_backlog                    **181920** listen 函数的backlog参数
    net.core.netdev_max_backlog                     **262144** 当网卡接收包的速度大于内核处理速度，会有一个队列放这些包，
                                                    这个值是队列的最大值
    net.ipv4.ip_local_port_range                    **1024 65535** 定义了作为客户端， port的取值范围。 如果服务器要请求
                                                    其他server(作为客户端), 那么这个值还是有必要的
    net.ipv4.tcp_syncookies                         **1** 与性能无关，用于解决SYN攻击
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

**高并发配置模板**

    - :net.ipv4.tcp_no_metrics_save = 1
      默认情况下一个tcp连接关闭后,把这个连接曾经有的参数比如慢启动门限snd_sthresh,拥塞窗口snd_cwnd 还有srtt等信息保存到dst_entry中, 只要dst_entry 没有失效,下次新建立相同连接的时候就可以使用保存的参数来初始化这个连接.通常情况下是关闭的。

    - :net.unix.max_dgram_qlen = 1024
      unix socket 最大队列，默认为10, 明显不够

**其他**

    socket 连接中，打开TCP_DEFER_ACCEPT 选项
