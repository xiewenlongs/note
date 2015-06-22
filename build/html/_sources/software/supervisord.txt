===============================================
supervisord
===============================================

进程守护管理. 用supervisord 可以管理非daemon的进程，可以自动crash重启, web监控.
supervisord分两个组件: ``supervisord`` 和 ``supervisorctl``

.. warning::
    一个supervisord 只能管理本地的subprocess, 如果有多台机器，每个机器上都有supervisord实例，这种还需要一个一个去管理.
    或者自己写程序来管理，毕竟每个instance的API都可以通过网络来访问

|

原理
---------------------------------------

supervisord 是通过fock的方式来执行 subprocess 的，所以它能准确的捕捉到子进程意外退出信号 CHILD, 也能拿到status. 但
这样做的缺点是: **一个supervisord只能管理一台机器**

client 通过conf里配置的方式跟server连接(unix domain 或者 tcp)

|

使用
---------------------------------------

配置一份conf, 里面有server 和 client的配置。 然后在server上启动supervisord::

    supervisord -c deploy/supervisor.conf


再在client上执行::

    > supervisorctl -c deploy/supervisor.conf status

    test                             RUNNING   pid 4753, uptime 13:54:04



启动停止subprocess::

    # 重启某个服务
    > supervisorctl -c deploy/supervisor.conf restart <subprocess>
    # 重启supervisord (它会重启所有subprocess)
    > supervisorctl -c deploy/supervisor.conf reload


client支持的命令有::

    add    clear  fg        open  quit    remove  restart   start   stop  update
    avail  exit   maintail  pid   reload  reread  shutdown  status  tail  version



.. warning::
    supervisord现在没法reload每个subprocess, 它现在的做法只能restart subprocess
