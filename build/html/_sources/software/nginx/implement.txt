===============================================
implement
===============================================


configure script
---------------------------------------

The configure script is a bash script which in charge of generating 3 important file:

- ngx_auto_headers.h
- ngx_auto_config.h
- Makefile

Those files are used in compiled. The configure script generating code by checking compile environment and parsing
configure arguments. For example::

    ngx_feature="/dev/poll"
    ngx_feature_name="NGX_HAVE_DEVPOLL"
    ngx_feature_run=no
    ngx_feature_incs="#include <sys/devpoll.h>"
    ngx_feature_path=
    ngx_feature_libs=
    ngx_feature_test="int  n, dp; struct dvpoll  dvp;
                     dp = 0;
                     dvp.dp_fds = NULL;
                     dvp.dp_nfds = 0;
                     dvp.dp_timeout = 0;
                     n = ioctl(dp, DP_POLL, &dvp);
                     if (n == -1) return 1"
    . auto/feature


this pice checking ``/dev/poll`` feature. It generate a test.c file and compile it. If successed, it echo one preprocess
macro into auto_headers.h


modules
---------------------------------------

There are 5 type of modules:

* NGX_CORE_MODULE

* NGX_HTTP_MODULE

* NGX_CONF_MODULE

* NGX_EVENT_MODULE

* NGX_MAIL_MODULE


.. cssclass:: table-bordered
.. table::

    =========================   =======  === ==== ===========  =========== ============ =========== =========== ============ ===========
    模块                        类型     idx cmd  init_master  init_module init_process init_thread exit_thread exit_process exit_master
    =========================   =======  === ==== ===========  =========== ============ =========== =========== ============ ===========
    core                        CORE     0   Y
    errlog                      CORE     1   Y
    conf                        CONF     2   Y                                                                  Y
    events                      CORE     3   Y
    event_core                  EVENT    4   Y                 Y           Y
    epoll                       EVENT    5   Y
    regex                       CORE     6   Y                 Y
    http                        CORE     7   Y
    http_core                   HTTP     8   Y
    http_log                    HTTP     9   Y
    http_upstream               HTTP     10  Y
    http_static                 HTTP     11
    http_gzip_static            HTTP     12  Y
    http_autoindex              HTTP     13  Y
    http_index                  HTTP     14  Y
    http_auth_basic             HTTP     15  Y
    http_access                 HTTP     16  Y
    http_limit_conn             HTTP     17  Y
    http_limit_req              HTTP     18  Y
    http_geo                    HTTP     19  Y
    http_map                    HTTP     20  Y
    http_split_clients          HTTP     21  Y
    http_referer                HTTP     22  Y
    http_rewrite                HTTP     23  Y
    http_proxy                  HTTP     24  Y
    http_fastcgi                HTTP     25  Y
    http_uwsgi                  HTTP     26  Y
    http_scgi                   HTTP     27  Y
    http_memcached              HTTP     28  Y
    http_empty_gif              HTTP     29  Y
    http_browser                HTTP     30  Y
    http_upstream_ip_hash       HTTP     31  Y
    http_upstream_least_conn    HTTP     32  Y
    http_upstream_keepalive     HTTP     33  Y
    http_stub_status            HTTP     34  Y
    http_write_filter           HTTP     35
    http_header_filter          HTTP     36
    http_chunked_filter         HTTP     37
    http_range_header_filter    HTTP     38
    http_gzip_filter            HTTP     39  Y
    http_postpone_filter        HTTP     40
    http_ssi_filter             HTTP     41  Y
    http_charset_filter         HTTP     42  Y
    http_userid_filter          HTTP     43  Y                             Y
    http_headers_filter         HTTP     44  Y
    http_copy_filter            HTTP     45  Y
    http_range_body_filter      HTTP     46
    http_not_modified_filter    HTTP     47
    =========================   =======  === ==== ===========  =========== ============ =========== =========== ============ ===========
