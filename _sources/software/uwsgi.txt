===============================================
uwsgi
===============================================

uwsgi 是一种协议， uWSGI是一个纯c写的web容器。 uWSGI实现了wsgi协议和uwsgi协议, 并且它的速度很快.
uwsgi 默认是在当前python 下编译的，如果你有多个python版本环境， 有两种方式可以解决:

1. 在每个python下编译安装各自的uwsgi
2. 编译uwsgi用 ``Bonus: multiple Python versions for the same uWSGI binary``, 不绑定uwsgi到具体的python版本


nginx 和 uwsgi 应用交互
---------------------------------------

在nginx中配置uwsgi时，会有这么一行::

    include     uwsgi_params;


这里面其实定义了很多变量::

    uwsgi_param  QUERY_STRING       $query_string;
    uwsgi_param  REQUEST_METHOD     $request_method;
    uwsgi_param  CONTENT_TYPE       $content_type;
    uwsgi_param  CONTENT_LENGTH     $content_length;
    uwsgi_param  REQUEST_URI        $request_uri;
    uwsgi_param  PATH_INFO          $document_uri;
    uwsgi_param  DOCUMENT_ROOT      $document_root;
    uwsgi_param  SERVER_PROTOCOL    $server_protocol;
    uwsgi_param  HTTPS              $https if_not_empty;
    uwsgi_param  REMOTE_ADDR        $remote_addr;
    uwsgi_param  REMOTE_PORT        $remote_port;


也许就是利用环境变量传递这些值给uwsgi 应用的， uwsgi 会把这些变量转换成http header, 放在request里, 所以: ``如果有些值，
nginx 层有，但是django层看不到， 那么可以利用nginx 的uwsgi_param指令，把值传到django里``


uwsgi插件
---------------------------------------

uwsgi 默认编译了很多插件在里面， 可以用 ``uwsgi --plugins-list`` 命令来查看有哪些可以使用的插件


route-cache
~~~~~~~~~~~~~~~~~~~~~~~

conf::

    [uwsgi]
    chdir = .
    # plugins = router_cache
    module = mysite.wsgi:application
    master = True
    processes = 1
    stats = /tmp/test_stat.sock
    http = :9090
    harakiri = 8

    cpu_affinity = 1
    log-micros = true
    cache2 = name=mycache,items=300,hash=murmur2
    route = ^/feedback cache:key=${REQUEST_METHOD}${REQUEST_URI}${QUERY_STRING},name=mycache,content_type=application/json
    route = ^/feedback cachestore:key=${REQUEST_METHOD}${REQUEST_URI}${QUERY_STRING},name=mycache,expires=10
    route = ^/feedback log:hey i am printing $1


    # route = ^/$ cache:key=myhome_for_${cookie[PHPSESSID]},name=mycache
    # ; store each successfull request (200 http status code) for '/'
    # route = ^/$ cachestore:key=myhome_for_${cookie[PHPSESSID]},name=mycache
    autoload = true

alarm
~~~~~~~~~~~~~~~~~~~~~~~

uwsgi 可以设置报警， 就是if then, example::

   alarm-backlog = mailme cmd:mail -s 'uWSGI alarm' -a 'From: foobar@example.com' admin@example.com


有用的alarm条件有这几个:

- log-alarm: 用regex监控log
- larm-backlog: 如果backlog表满了
- alarm-segfault: uwsgi 段错误了

.. warning::
    注意，uwsgi缓存只会以文本缓存response的内容, 所以查询缓存并返回给client的时候，它默认是以text/html返回的。可以用
    content_type指令来指定


uwsgi 的缓存可以监控状态:: https://pypi.python.org/pypi/uwsgicachetop


其他插件
~~~~~~~~~~~~~~~~~~~~~~~

**Legion(军团系统)**

uwsgi的集群， 当不适用前端web server时， uwsgi也可以配高可用，就是这套集群系统。 但是还是nginx的upstream集比较安全可靠.


**cheaper**

根据uwsgi 负载, 动态资源分配(workprocess 数量\内存占用...)

交互式启动uwsgi
---------------------------------------

command::

    uwsgi --chdir . --file mysite/wsgi.py --callable application  --master --processes 1 --http :9090 --harakiri 10


常见问题
---------------------------------------


当设置了 ``max-requests`` 后，每个worker会在收到那么多请求后自动重启，会在uwsgi log 里写入这么几行::

    Fire in the hole
    Sat Jul 11 13:48:19 2015 - Fire in the hole !!! (60 seconds to detonation)
    Gracefully killing worker 1 (pid: 22813)...
    Sat Jul 11 13:48:19 2015 - stopping gevent signals watchers for worker 1 (pid: 22813)...
    Sat Jul 11 13:48:19 2015 - stopping gevent sockets watchers for worker 1 (pid: 22813)...
    Sat Jul 11 13:48:19 2015 - main gevent watchers stopped for worker 1 (pid: 22813)...
    Sat Jul 11 13:48:19 2015 - worker 1 (pid: 22813) core 93 is managing "GET /"
    Sat Jul 11 13:48:19 2015 - worker 1 (pid: 22813) core 95 is managing "GET /"



如果同时使用 ``gevent`` 和 ``cpu_affinity`` 时，有可能导致worker进程不能被有效回收，
导致可工作的worker越来越少，最终服务挂掉. 这个可以从uwsgi log里看到， respawn的worker id 越来越固定

未解决
~~~~~~~~~~~~~~~~~~~~~~~

调优: ``If you are running only a single application you can disable multiple interpreters.`` 为什么???



进阶
---------------------------------------

减负线程: https://uwsgi-docs.readthedocs.org/en/latest/OffloadSubsystem.html


Harakiri 模式
~~~~~~~~~~~~~~~~~~~~~~~

Harakiri有两种模式:

1. 每个worker在接受每个request前，调用alarm， 这种方式是不可靠的。因为handler里面可能调用alarm来取消当前设置的超时
2. master 进程创建一个shared memory zone, 每个worker处理请求之前，在pool中记录start时间， 结束后删除。另外有一个monitor进程，不停去检测pool
   中超时的worker, 找到后kill


标准配置
---------------------------------------

example::

    [uwsgi]
    chdir = .
    chmod-socket=664
    module = mysite.wsgi:application
    master = True
    processes = %(%k * 4)
    pidfile = /tmp/%n.pid
    uwsgi-socket = /tmp/%n.sock
    # http 监控(返回json)
    # stats = 127.0.0.1:1717
    stats = /tmp/%n_stat.sock
    # uwsgi 退出的时候，清理环境, 如unix文件/ pid文件h
    vacuum = True
    harakiri = 50
    harakiri-verbose = True
    single-interpreter = True
    memory-report = True
    max-requests = 10000
    listen = 8192

    log-truncate = True
    disable-logging = True
    # logformat = i am a logline reporting "%(method) %(uri) %(proto)" returning with status %(status)
    daemonize = /home/log/uwsgi/%n_uwsgi.log
    procname-master = uwsgi --ini %n.ini (master)
    procname = uwsgi --ini %n.ini

    need-app = True

    # where n is the number of cpus to dedicate per-worker
    # 和gevent配合使用，可能会导致worker不被回收， 可用的worker会慢慢减少，减到1个，server就会挂掉
    # cpu_affinity = 1

    # 只对tcp有效
    # so-keepalive = True

    # 设置worker一定时间自动重启(second), 默认不自动重启
    # max-worker-lifetime = 600
