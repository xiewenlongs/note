===============================================
nginx
===============================================

---------------------------------------
简介
---------------------------------------

nginx 是开源、多平台的http 服务器，以下是它的特性:

|

.. image:: ../_static/s_nginx_feature.png


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
    --with-aio_module (no)                              使用AIO方式处理事件驱动， 这里的 aio module 只能与FreeBSD系统上的kqueue模块
                                                        合作， Linux上无法使用
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


---------------------------------------
配置
---------------------------------------


基本配置
~~~~~~~~~~~~~~~~~~~~~~~


.. cssclass:: table-bordered
.. table::

    ====================    ==========================================================================          ============
    选项                    作用                                                                                推荐值
    ====================    ==========================================================================          ============
    accept_mutex            1. ``避免惊群效果`` (每个 accept 上一把锁);                                         true
                            2. ``负载平衡`` (如果当前worker有任务，则不参与竞争，直接返回处理当前任务)
    worker_connections      每个worker 的连接池大小。 所以:
                            ``nginx能接收的总的连接数 = worker_connections * worker_processes``
    timer_resolution        默认情况下，每次内核事件调用，都要执行一次 gettimeofday,                            100ms
                            来更新nginx中的缓存时钟, 这个选项控制执行间隔
    worker_cpu_affinity     绑定CPU
    sendfile on             启用sendfile 系统调用，来减少static file 请求时，用户态和内核态的切换时间
    worker_rlimit_nofile    unknown
    keepalive_timeout       unknown
    ====================    ==========================================================================          ============




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


负载均衡
~~~~~~~~~~~~~~~~~~~~~~~

nginx 使用upstream 分流

deferred
~~~~~~~~~~~~~~~~~~~~~~~

listen 80 deferred;
