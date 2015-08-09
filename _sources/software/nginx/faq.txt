===============================================
FAQ
===============================================


压缩指令对老IE浏览器支持的问题
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

老IE浏览器对 http Gzip 压缩功能的支持不太好, 所以为了避免怪异的问题，最好关掉对IE老浏览器的gzip功能::

      gzip_disable "MSIE [1-6]\.";

IE老浏览器对gzip 支持不好，可能会体现出一下几个问题

* 当页面中存在多个iframe, 并且这些iframe独立使用相同的javaScript脚本时，可能会出现脚本不运行或者运行不正常的情况. 这个
  问题的发生与IE6在请求javascript脚本时共享资源有关

* IE6通过各种方式从一个页面跳转到另一个页面，并且跳转页响应数据头部的cache-control头域设置了no-cache指令， 这也可能造成
  javascript脚本不运行或者运行不正常. 该问题与IE6处理缓存数据有关

* JavaScript 脚本中的汉字没有被正确编码导致解析失败, 因而造成脚本不运行或者运行不正常. 这种问题在首次加载页面运行脚本时
  不会发生，因为IE6可以使用正确的编码对脚本中的汉字进行解析，使脚本正常，但当使用刷新功能时，由于IE6不会再记录上次使用
  的编码规则，使得脚本中的汉字被解析成乱码




使用nginx防盗链
~~~~~~~~~~~~~~~~~~~~~~~

使用rewrite 和 valid_referers 指令，可以完成基本的防盗链::

    location ~* ^.+\.(gif|jpg|png|swf|flv)$ {

        # 指定哪些referer被认为是valid的
        valid_referers blocked server_names *.abc.com;

        if ($invalid_referer) {
            # 禁止盗图
            rewrite ^/ http://abc.com/forbidden.jpg permanent;
        }
    }


rewrite VS 重定向
~~~~~~~~~~~~~~~~~~~~~~~

nginx 的rewrite 指令，就是返回一个重定向。所以对于客户端，访问rewrite的url, 会导致两次请求。


在log中记录header
~~~~~~~~~~~~~~~~~~~~~~~

记录requests 的header使用 ``$http_<name>`` 变量, 记录response的header使用 ``$sent_http_<name>`` 变量, 如下::

    underscores_in_headers on;
    log_format STAT '[$time_local]`$http_x_up_calling_line_id`"$request"`"$http_user_agent"`$status`[$remote_addr]`$http_range`"$http_referer"`$request_time`$body_bytes_sent`$http_deviceid`$http_x_forwarded_for`$host`$http_cookie |token=`$http_HTTP_AUTHORIZATION`';
