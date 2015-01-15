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
