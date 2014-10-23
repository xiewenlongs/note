web服务日志分析工具
========================


使用方法
````````````````

#. 书写 ~/.goaccessrc 文件，在其中配置日志的格式

    color_scheme 0

    date_format %a %b %d %H:%M:%S %Y

    log_format {%^} {%^} [%^] %h %^[%d] %^ %r %^(%^ %s) %^

#. 使用命令

    > goaccess -f 日志路径 > result.html
