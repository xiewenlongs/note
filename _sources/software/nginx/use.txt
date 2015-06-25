===============================================
use
===============================================

---------------------------------------
简介
---------------------------------------

nginx 是开源、多平台的http 服务器，以下是它的特性:

|

.. image:: ../../_static/s_nginx_feature.png


|

nginx 虽然是开源的，但Igor Sysoev 已为nginx 成立公司，提供商业级的产品与支持


---------------------------------------
安装
---------------------------------------

依赖
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ===============         =============================            =============
    依赖库                  作用                                     备注
    ===============         =============================            =============
    PCRE(required)          正则匹配                                 无
    zlib(required)          gzip 压缩使用
    openssl                 https 需要
    ===============         =============================            =============


编译
~~~~~~~~~~~~~~~~~~~~~~~

|

configure选项

.. cssclass:: table-bordered
.. table::

    =======================================             =============================================================================
    选项(默认安装)                                      作用
    =======================================             =============================================================================
    --with-rtsig-module (no)                            使用rtsig module 处理事件驱动. 默认情况下， nginx 不安装rtsig module
    --with-select_module (no)                           使用select module 处理事件驱动. 如果nginx 没有找到epoll, 该模块将会被安装
    --without-select_module (no)                        不使用select module 处理事件驱动
    --with-poll_module (no)                             与select 模块类似，默认不会安装
    --without-poll_module (no)                          不使用poll module 处理事件驱动
    --with-aio_module (no)                              使用AIO方式处理文件访问
    --without-http_charset_module (yes)                 这个模块可以降服务器发出的http响应重新编码
    --without-http_gzip_module (yes)
    --without-http-ssi_module (yes)                     该模块可以在Response 中加入特定的内容，如html固定页头/页尾
    --without-http_userid_module (yes)                  这个模块可以通过HTTP请求头，认证用户信息，以确定请求是否合法
    --without-http_access_module (yes)                  这个模块可以限制客户端IP
    --without-http_auth_basic_module                    这个模块可以提供最简单的用户密码验证
    --without-http_autoindex_module                     目录浏览功能
    --without-http_geo_module                           这个模块可以定义一些变量，这些变量的值与客户端IP地址关联. 例如: 这样Nginx
                                                        针对不同地域显示不同的语言
    --without-http_map_module                           这个模块可以建立一个key/value映射表, 这样可以针对不同的url做特殊处理
    =======================================             =============================================================================

|

configure 脚本的主要目的是 ``检测编译依赖库是否存在`` , ``生成ngx_modules.c文件``

|


信号操作
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ===================             =====================================================
    信号                            作用
    ===================             =====================================================
    nginx -s stop                   停止服务
    nginx -s quit                   优雅的停止服务
    nginx -s reload                 重载配置
    nginx -s reopen                 日志文件回滚
    kill -s SIGUSR2                 平滑升级nginx
    ===================             =====================================================



---------------------------------------
配置
---------------------------------------

core模块
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ================================    ==============================================================================
    选项                                作用
    ================================    ==============================================================================
    daemon <on>                         是否以daemon方式运行。默认是on, 提供off的目的是为了开发调试
    master_process <on>                 是否启动worker进程。 同上，提供off只是为了调试
    timer_resolution <time>             By default, gettimeofday() is called each time a kernel event is received.
                                        With reduced resolution, gettimeofday() is only called once per specified this.
    pid <path>                          pid文件路径，默认在<prefix/logs/nginx.pid>
    lock_file <path>                    nginx 多个进程访问共享内存时，需要用lock来同步，但现在的nginx都用的是atomic,
                                        所以这个指令相当于已经废弃
    user <user>                         worker进程的owner
    worker_processes <num>              指定worker进程个数, 可以设置为``auto``, nginx自动检测CPU核心数
    worker_rlimit_nofile <int>          **102400**  设置一个worker进程可以打开的最大文件句柄数
    worker_priority <num>               nginx进程优先级, 默认-10. 值越低, 优先级越高。 注意优先级不要设置太高，否则
                                        系统调用的优先级会被比下去
    worker_cpu_affinity <...>           CPU亲和性
    worker_rlimit_core                  coredump 文件的最大大小
    worker_rlimit_sigpending            只对RTSIG 系统有用。On systems that support rtsig connection processing
                                        method, changes the limit on the number of signals that may be queued
                                        (RLIMIT_SIGPENDING) for worker processes. Used to increase the limit without
                                        restarting the main process
    working_directory <path>            Defines the current working directory for a worker process. It is primarily
                                        used when writing a core-file
    env <...>                           默认的，nginx的worker进程会清除从parent继承来的所有环境变量, 这个指令可以保留
                                        部分变量
    ================================    ==============================================================================


event模块
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ================================   =================    ==============================================================================
    选项                               模块                 作用
    ================================   =================    ==============================================================================
    worker_connections <num>           event_core           一个worker进程可以建立的最大连接总数, 它不能超过 worker_rlimit_nofile, 否则
                                                            没有意义. 默认512
    connections <num>                  event_core           貌似被废弃了, nginx文档里没有这个指令, 从代码看默认512
    use <event>                        event_core           使用那种connection processing method, nginx默认会选择最高效的方式
    multi_accept <on>                  event_core           当事件模型通知有新请求时，尽可能对本次调度中客户端的所有TCP请求都建立连接,
                                                            默认off. 如果同一时间过来的请求量太大，一个worker进程会花费很多在accept上，
                                                            所以这时应该关闭
    accept_mutex <on>                  event_core           1. ``避免惊群效果`` (每个 accept 上一把锁);
                                                            2. ``负载平衡`` (如果当前worker的请求量已达到worker_connections的7/8，
                                                            则这个worker 不参与竞争新来的request) ,默认是on
    accept_mutex_delay                 event_core           和accept_mutex 配合用, 如果一个worker进程未拥有accept mutex，它至少延迟这么
                                                            长时间之后再尝试抢夺, 默认500ms
    epoll_events <num>                 epoll                ??????, 默认512
    worker_aio_requests <num>          epoll                ??????, 默认32
    ================================   =================    ==============================================================================

http模块
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ================================   =================    ==============================================================================
    选项                               模块                 作用
    ================================   =================    ==============================================================================
    error_log                          errlog               改变error log 路径, 默认是 /prefix/logs/error.log
    client_header_timeout              http_core            收到一个请求后,nginx 建立与客户端的TCP链接, 链接建立后开始read
                                                            客户端发送的数据。如果这个客户端是个hacker, 它hung住来浪费nginx
                                                            资源，那么nginx在这段时间内会一直维持着这个连接。此参数设置
                                                            nginx等待时间，如果超过时间，nginx直接返回408错误, 默认60s
    client_body_timeout <time>         http_core            同上，请求体超时

    connection_pool_size               http_core            nginx为一个连接能开辟内存的上限, 默认256. 这个值对性能调优效果很小，不应该设置
    request_pool_size                  http_core            nginx为一个请求能开辟的内存上限，默认4k, 这个值对性能调优效果很小，不应该设置
    client_header_buffer_size          http_core            处理一个请求头最大开辟的内存大小, 默认1k
    large_client_header_buffers        http_core            处理一个请求头，如果请求过大，超过client_header_buffer_size指定缓存，那么使用
                                                            这个设置项
    client_max_body_size <size>        http_core            根据请求头的content-Length, 来限制请求. 若超出限制，nginx 返回413
    client_body_buffer_size <num>      http_core            一个请求的请求体需要存在内存中， 这个值指定了buffer大小, 如果超过这个值，
                                                            nginx 会把请求体写入磁盘
    client_body_in_file_only <on>      http_core            对于一个请求，如果请求体过大，超过client_body_buffer_size大小
    client_body_temp_path              http_core            同上，如果请求过大，nginx会把请求存在临时文件中
    sendfile <on>                      http_core            启用sendfile 系统调用，来减少static file 请求时，用户态和内核态的切换时间
    aio                                http_core            使用AIO方式来访问静态文件服务, 和sendfile互斥。两者谁好谁坏很难定论, nginx
                                                            论坛上，建议使用AIO, 因为sendfile对预读的控制不好. 应使用aio + directio
    tcp_nodelay <on>                   http_core            对keepalive连接是否使用TCP_NODELAY选项, 与NOPUSH选项互斥， 其实是关闭
                                                            Nagle's 算法， Nagle 是避免传输数据小，网络带宽无法有效利用，将多个数据组
                                                            包一起发送, 但是HTTP的数据更多是偏向流处理，而不是类似telnet,等待用户输入
                                                            数据, 所以默认是TCP_NODELAY 开启
    send_timeout                       http_core            nginx 返回response 给客户端的时候，每发送一个packet， 都希望收到ack, 如果在
                                                            这个指定的时间内，没有收到ack, 说明client可能掉了，那么关闭连接
    keepalive_requests <num>           http_core            nginx 建立一个keep alive 连接后，在这个连接上处理了<num>个请求后，就关闭连接
    keepalive_disable <...>            http_core            nginx 可以设置对哪些浏览器不keep alive, 可选的有: msie6, safari
    satisfy <...>                      http_core            nginx 有多种限制access的方式，如ip, base_auth, 这个指令是决定把所有限制<与>
                                                            还是<或>
    log_not_found <on>                 http_core            访问不存在的文件， 这种请求是否记录在error log里, 默认记录<on>
    server_tokens <on>                 http_core            在错误response是否显示nginx版本, 默认on
    error_page                         http_core            error_page 500 502 503 504 /50x.html;
    try_files                          http_core            try file
    error_log                          http_core            指定error log路径
    open_file_cache <max> <inact>      http_core            **max=102400 inactive=20s**  open_file_cache 会告诉 Nginx 去缓存打开的文件，
                                                            “未找到”的错误，有关文件的元 数据和他们的权限，等等。这样做的好处是，一个高
                                                            需求的文件要求时，Nginx 的可以 立即开始发送数据；而且也知道立即发送一个 404,
                                                            但是，有一个不太理想的缺点：如果磁盘上有变化，服务器不会立即作出反应。 最多
                                                            缓存<max>个， 非活跃的缓存 <inact>秒后从缓存剔除
    open_file_cache_valid <time>       http_core            **30s** 而活动（最近要求的文件）每<time>秒重新验证一次。
    open_file_cache_min_uses           http_core            如果定义活跃的item, 这个指令就是定义: 在<inact>时间内至少访问过 <time>次
    open_file_cache_errors             http_core            缓存404
    resolver                           http_core            配置nginx 内部的DNS服务器
    resolver_timeout                   http_core            配置DNS查询超时
    read_ahead <size>                  http_core            ???
    lingering_close <on>               http_core            ???
    lingering_time <on>                http_core            ???
    lingering_timeout <on>             http_core            ???
    ================================   =================    ==============================================================================


其他模块
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ================================   =================    ==============================================================================
    选项                               模块                 作用
    ================================   =================    ==============================================================================
    error_log                          errlog               改变error log 路径, 默认是 /prefix/logs/error.log
    ================================   =================    ==============================================================================


基本配置
~~~~~~~~~~~~~~~~~~~~~~~


.. cssclass:: table-bordered
.. table::

    ================================    ==============================================================================
    选项                                作用
    ================================    ==============================================================================
    user <用户名>                       worker进程的执行用户
    use <事件模型>                      选择事件模型(如: epoll)
    listen <num...>                     监听端口, 它有以下参数:

                                        **default_server**: 默认server, nginx 可能有多个server配置，设置这个后当前
                                        server就成为默认server(server_name没有匹配)

                                        **bind**: 当设置 listen xx.xx.xx.xx:80 时，默认nginx 不会限制IP, 就是不会绑定
                                        xx.xx.xx.xx这个IP接口， 设置bind则绑定

                                        **fastopen**: 打开TCP fastopen选项, TCP fastopen特性只有kernel 版本大于3才支持
                                        这个参数水很深，不要随便设置

                                        **backlog <num>**: 默认512, 修改完net.core.somaxconn之后一定要修改这个，否则
                                        等于没改

                                        **deferred**: 默认新来一个TCP连接，三次握手后master进程就唤醒worker进程来
                                        接待。 设置这个参数后，三次握手完成master并不立 刻唤醒worker, 而是这个连接
                                        上真来了数据，才唤醒worker, 它减轻了worker的负担。降低服务端进行
                                        epoll_ctl、epoll_wait（linux下）的次数（系统调用）和降低服务端保持的连接句柄数
                                        ``需要根据业务特征来决定``
    server_name <...>                   虚拟主机配置, nginx 检测request头的HOST字段，拿来匹配server, 按以下顺序匹配:

                                        1. 字符串完全匹配, 如: www.test.com

                                        2. 通配符前匹配, 如: \*.test.com

                                        3. 通配符后匹配, 如: www.test.\*

                                        4. 正则匹配, 如: ~^(?<user>.+)\.example\.net$; (注意，正则表达式前要增加~)

                                        5. 都没有匹配， 使用default_server

                                        server_name 是忽略大小写的，因为它是把server_name转成小写

    server_names_hash_bucket_size       为了提高快速找到server_name的能力， nginx使用了散列桶， 这个参数指定散列桶的
                                        大小， 越大越占内存，但速度越快. 默认32|64|128
    server_names_hash_max_size          效果同上. 默认512
    server_name_in_redirect <yes>       重定向的时候，把原请求里的HOST, 换成server_name写的第一个主机名
    location <...>                      用请求中的url来匹配, 见 :ref:`location <nginx_location>`
    alias <path>                        指定文件路径
    root <path>                         指定文件路径(和alias 互为两种方式)
    index <path>                        指定主页的html文件, 默认为(index.html)
    error_page <code> <url>             错误重定向, 出现<code>对应的错误response时，nginx 把结果重定向到url
    resursive_error_pages <on>          是否打开"错误重定向"的递归
    try_files <path1> <path2> <url>     按顺序尝试每一个path
    limit_except {...}                  按http方法, 限制客户端请求种类, 见 :ref:`limit_except <nginx_limit_except>`
    limit_rate <num>                    对每一个TCP连接限速
    ignore_invalid_headers <on>         如果出现不合法的HTTP头部时， nginx 会忽略错误继续处理。但如果这个选项被off
                                        nginx 会直接返回400
    underscores_in_headers <on>         http头部是否允许带下划线
    log_not_found <on>                  404是否记录日志
    merge_slashes <on>                  是否合并url中相邻的/, 如: //test//a.txt 会变成 /test/a.txt
    resolver <ip>                       设置DNS服务器地址
    resolver_timeout <time>             DNS解析超时时间, 默认30s
    server_tokens <on>                  返回错误页面时，是否在server中注明nginx版本, 默认on
    ================================    ==============================================================================

|

内部资源分配
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ================================    ==========================================================================
    选项                                作用
    ================================    ==========================================================================
    connnection_pool_size <num>         每个TCP连接分配的内存池初始大小, 默认256, 如果这个值太大，内存占用会很多，
                                        如果很小，造成分配次数增多
    send_timeout <time>                 nginx 向客户端发送了数据，但客户端超过这么长时间都没有去接收数据，那么
                                        nginx 会关闭这个连接
    reset_timeout_connection            ??? 向客户端发送RST来关闭连接， 减少服务端的FIN-WAIT状态套接字
    lingering_close                     ???
    lingering_time                      ???
    lingering_timeout                   ???
    keepalive_disable <...>             对某些浏览器禁用 keepalive 功能
    keepalive_timeout <time>            keepalive 超时时间, 每个http1.1的连接， 默认nginx都是长连接,超时时间由
                                        这个值来定
    keepalive_requests <num>            一个keepalive 连接上默认最多能发送的request 个数
    tcp_nopush <on>                     是否开启FreeDSB的TCP_NOPUSH 或Linux 的TCP_CORK功能
    proxy_buffering <on>                proxy服务器接收后端upstream响应的时候，是否使用buffer先把response缓存起
                                        来，等upstream处理完了，再全部send到客户端. 打开的好处是， 只要代理nginx
                                        和后端nginx交互完后，就可以关闭这条链接，减少upstream服务器开销，这对于
                                        客户端响应很慢的情况来说，是很有利的，后端之间的链接，不用等客户端响应
    ================================    ==========================================================================

|

文件访问
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ================================    ==========================================================================
    选项                                作用
    ================================    ==========================================================================
    open_file_cache                     ???
    open_file_cache_errors              ???
    open_file_cache_min_uses            ???
    open_file_cache_valid               ???
    ================================    ==========================================================================

|

MIME
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ================================    ==========================================================================
    选项                                作用
    ================================    ==========================================================================
    type {...}                          配置文件扩展名与mime映射
    default_type <...>                  默认MIME类型
    types_hash_bucket_size              上面映射的散列桶大小
    types_hash_max_size                 上面散列桶的个数
    ================================    ==========================================================================

|

性能调优
~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered
.. table::

    ============================    ==========================================================================
    选项                            作用
    ============================    ==========================================================================
    worker_cpu_affinity <...>       worker和CPU绑定 (仅对Linux系统起作用， 内部调用sched_setaffinity()来实现,
                                    示例: worker_cpu_affinity 1000 0100 0010 0001
    ssl_engine <device>             ssl硬件加速。如果服务器上有SSL硬件加速设备，就可以用这个指令配置
    timer_resolution <time>         默认情况下，每次内核事件调用，都要执行一次 gettimeofday
    worker_prority <int>            在Linux系统中，每个进程都有优先级，范围为[-19,+20], -19优先级最高,
                                    如果想让分配CPU资源的时候，多分给nginx, 可以增加nginx优先级，默认nginx 值
                                    为0 (不建议低于-5, 内核进程的优先级)
    accept_mutex <on>               1. ``避免惊群效果`` (每个 accept 上一把锁);
                                    2. ``负载平衡`` (如果当前worker的请求量已达到worker_connections的7/8，
                                       则这个worker 不参与竞争新来的request)
    multi_accept <on>               当事件模型通知有新请求时，尽可能对本次调度中客户端的所有TCP请求都建立连接
    worker_connections              每个worker 的连接池大小。 所以:
                                    ``nginx能接收的总的连接数 = worker_connections * worker_processes``
    ============================    ==========================================================================


---------------------------------------
反向代理
---------------------------------------

.. image:: ../../_static/s_nginx_rproxy.png

Nginx代理和squid代理机制不太一样， 客户端发送请求， nginx代理服务器接收完整个请求，才向upstream转发请求， 这样做的目的
主要是 ``降低upstream`` 的压力. 因为客户端到nginx代理之间一般式走外网， 速度较慢。 而代理和upstream之间一般式走内网，
速度很快


buffer
~~~~~~~~~~~~~~~~~~~~~~~

Nginx proxy use buffer default. Without buffers, data is sent from the proxied server and immediately begins to be
transmitted to the client. If the clients are assumed to be fast, buffering can be turned off in order to get the
data to the client as soon as possible. With buffers, the Nginx proxy will temporarily store the backend's response
and then feed this data to the client. If the client is slow, this allows the Nginx server to close the connection
to the backend sooner. It can then handle distributing the data to the client at whatever pace is possible.


If you want turn it off, you should set ``proxy_buffering`` to off


refer: https://www.digitalocean.com/community/tutorials/understanding-nginx-http-proxying-load-balancing-buffering-and-caching


负载均衡
~~~~~~~~~~~~~~~~~~~~~~~

upstream负载均衡有两种机制: ``ip_hash`` 和 ``weight``

ip_hash 是nginx按客户端ip, 自动的把请求打在上游集群中特定一台, 配置如下::

    upstream backend {
        ip_hash
        server backend1.example.com;
        server backend2.example.com;
        server backend3.example.com;
    }

weight 是按权重来分流量， 配置如下::

    upstream backend {
        server backend1.example.com weight=2 max_fails=3 fail_timeout=30s;
        server backend2.example.com weight=2 max_fails=2 fail_timeout=20s;
        server backend3.example.com weight=1;
    }


一份反向代理配置如下::

    upstream real.sites {

        server 123.123.123.123;

        // 用keepalive保存长连接，降低频繁创建连接的开销
        keepalive 16;
    }

    proxy_cache_path /path/to/cache levels=1:2 keys_zone=static_cache:100m;

    server {
        server_name     www.example.com;

        // 把真正的IP地址放到header的X-Forwarded-For里面
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

        proxy_http_version 1.1;

        proxy_set_header Connection "";

        # 当某一个上游返回503错误时， nginx 继续换一个上游转发
        # 默认一个上游返回错误时，nginx是不会换一个上游转发的
        proxy_next_upstream http_503;

        // 把静态资源缓存起来，减少服务器间数据传输
        location ~ \.(css|js|jpg|png|gif|ico)$ {

        proxy_cache static_cache;

        proxy_pass http://real.sites;
        }

        location / {
            proxy_pass http://real.sites;
        }
    }

|

---------------------------------------
性能调优
---------------------------------------

内核参数调优
~~~~~~~~~~~~~~~~~~~~~~~
见 :ref:`内核调优<http_core_tuning>`


debug nginx
~~~~~~~~~~~~~~~~~~~~~~~

方法1: 打开 --with-debug

方法2: Debugging nginx with DTrace pid provider

相关链接: http://nginx.org/en/docs/nginx_dtrace_pid_provider.html


|

---------------------------------------
其他
---------------------------------------

.. _nginx_location:

location 配置
~~~~~~~~~~~~~~~~~~~~~~~


``=`` 匹配符， 完全匹配才处理， e.g::

   location = / {
        # 完全匹配才处理, 只处理url/
        ...
   }

``~`` 匹配符， 表示执行一个正则匹配, 大小写敏感

``~*`` 匹配符， 表示执行一个正则匹配, 大小写不敏感

``^~`` 表示普通字符匹配，如果该选项匹配，只匹配该选项，不匹配别的选项，一般用来匹配目录

可以使用正则, e.g::

   location  = / {
        # 只匹配"/".
        [ configuration A ]
   }

   location  / {
        # 匹配任何请求，因为所有请求都是以"/"开始
        # 但是更长字符匹配或者正则表达式匹配会优先匹配
        [ configuration B ]
   }

   location ^~ /images/ {
        # 匹配任何以 /images/ 开始的请求，并停止匹配 其它location
        [ configuration C ]
   }

   location ~* \.(gif|jpg|jpeg)$ {
        # 匹配以 gif, jpg, or jpeg结尾的请求.
        # 但是所有 /images/ 目录的请求将由 [Configuration C]处理.
        [ configuration D ]
   }


log 配置
~~~~~~~~~~~~~~~~~~~~~~~

log配置有四个指令::

    error_log logs/error.log warn;
    log_format gzip '$remote_addr - $remote_user [$time_local] '
                    '"$request" $status $bytes_sent '
                    '"$http_referer" "$http_user_agent" "$gzip_ratio"';

    access_log /spool/logs/nginx-access.log gzip buffer=32k;
    open_log_file_cache max=1000 inactive=20s valid=1m min_uses=2;


默认的error日志，在logs/error.log. 默认的access日志， 在logs/access.log. 可以通过 ``log_format`` 指令来定义format, 然后
在access_log指令里用.

.. warning::
    尽量不要在log 路径里写变量， 否则nginx 每写一条日志，都要打开关闭一次文件描述符, 虽然可以用open_log_file_cache 指令
    来优化这点，但还是很影响性能

|

写log时默认使用同level的log配置, 如果当前level没有log配置, 就继承上层配置。如果当前level有多个log配置，那么每个配置都写
一遍



Don't use If
~~~~~~~~~~~~~~~~~~~~~~~

IfIsEvil: http://wiki.nginx.org/IfIsEvil


.. _nginx_limit_except:

限制请求方法
~~~~~~~~~~~~~~~~~~~~~~~

配置::

    limit_except GET POST DELETE {
        deny all;
    }

意思是， 除了GET POST DELETE 方法外，其他一切请求都deny


---------------------------------------
扩展
---------------------------------------

.. cssclass:: table-bordered
.. table::

    ================================    ==========================================================================
    模块                                作用
    ================================    ==========================================================================
    nginx_upstream_check_module         upstream 集群健康监测模块
    ================================    ==========================================================================




---------------------------------------
问题
---------------------------------------


