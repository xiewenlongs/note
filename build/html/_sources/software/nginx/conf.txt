===============================================
demo config
===============================================


This is a max-optimizal nginx config::


    #####################
    # global config
    #####################

    # daemon on;
    # master_process on;
    # timer_resolution 100ms;
    # lock_file /var/run/nginx.lock;
    # worker_priority -10;

    user admin admin;
    pid /var/run/nginx.pid;
    worker_processes auto;
    worker_rlimit_nofile 102400;
    error_log /var/log/nginx/error.log;

    #####################
    # event config
    #####################

    events {
        use epoll;
        worker_connections 51200;
        multi_accept off;
    }

    #####################
    # http config
    #####################

    http {
        server_tokens off;
        sendfile on;
        tcp_nopush on;
        tcp_nodelay on;


        keepalive_timeout 10;
        client_header_timeout 10;
        client_body_timeout 10;
        reset_timedout_connection on;
        send_timeout 10;

        # 限制同一IP并发连接数
        limit_conn_zone $binary_remote_addr zone=manyconns:5m;
        limit_conn_status 405;
        limit_conn manyconns 100;

        # 限制同一IP请求量, 每秒不超过2个req, 峰值为10, 那么客户端以5req/s
        # 的速度，每5秒发一次，平均下来，不超过rate限制，不会报错
        limit_req_zone $binary_remote_addr zone=manyreqs:10m rate=2r/s;
        limit_req_status 404;
        limit_req zone=manyreqs burst=10 nodelay;

        include /etc/nginx/mime.types;
        default_type text/html;
        charset UTF-8;


        gzip on;
        gzip_disable "msie[1-6]";   # 为IE6关闭gzip, 兼容老浏览器
        gzip_min_length  1100;
        gzip_comp_level   6;
        gzip_buffers     16 8k;
        gzip_types  text/plain text/css application/json application/x-javascript text/xml application/xml application/xml+rss text/javascript image/png image/gif image/jpeg;
        gzip_proxied any;

        ##
        #client_body,client_header config
        ##

        client_body_temp_path   /dev/shm/nginx/client_body_temp 1 2;
        client_body_timeout     60s;
        client_header_timeout   60s;
        client_max_body_size    1m;

        ##
        #request state
        ##
        req_status_zone server "$host,$server_addr:$server_port" 10M;

        ##
        #uwsgi global
        ##

        uwsgi_temp_path             /dev/shm/nginx/uwsgi_temp 2 2;
        uwsgi_max_temp_file_size    1024m;
        uwsgi_temp_file_write_size  8k;
        uwsgi_buffering             on;
        uwsgi_ignore_client_abort   on;
        uwsgi_buffer_size           8k;
        uwsgi_buffers               24 8k;
        uwsgi_busy_buffers_size     8k;
        uwsgi_connect_timeout       60s;
        uwsgi_send_timeout          60s;
        uwsgi_read_timeout          60s;


        # uwsgi cache
        # 配置uwsgi cache 后， nginx 会多启动两个进程 (cache manager process) 和 (cache loader process)
        uwsgi_cache_path /tmp/uwsgi_cache levels=1:2 keys_zone=CACHE:10m inactive=10s;

        ##
        #proxy config
        ##

        proxy_buffering             on;
        proxy_buffers               8 4k;

        proxy_temp_path             /dev/shm/nginx/proxy_temp 1 2;
        proxy_cache_path            /dev/shm/nginx/proxy_cache  levels=1:2 keys_zone=cache:128m inactive=1d max_size=1g;
        proxy_connect_timeout       60s;
        proxy_read_timeout          60s;
        proxy_send_timeout          60s;

        #####################
        # http config
        #####################

        upstream mysite
        {
            # ip_hash;
            # keepalive;
            # least_conn;
            server unix:/tmp/mysite.sock weight=10 max_fails=1 fail_timeout=10;
            server 192.168.1.1:8080 weight=20 backup; # 标示这个server不会做load balancer, 除非其他server挂了
            server 192.168.1.1:8080 weight=20 down;  # 标示这个server不可用
        }

        server {
            # listen 80 backlog=2048 deferred keepalive=on fastopen=100;
            listen 80 backlog=1024 deferred;
            server_name     localhost;

            location / {
                uwsgi_pass  mysite;
            }

            location /cached {
                uwsgi_cache CACHE;
                uwsgi_buffering on;
                uwsgi_cache_key $scheme$request_method$host$request_uri$args;
                uwsgi_pass  mysite;
                uwsgi_cache_valid 200 20s;

                uwsgi_pass  mysite;
            }

            location /nginx_status {

                limit_except GET {
                    allow 192.168.1.0/32;
                    deny  all;
                }

                access_log off;
                stub_status on;
                access_log off;
                allow 127.0.0.1;
            }

            location /reqstat {
                access_log off;
                allow 127.0.0.1;
                deny all;
                req_status_show;
            }
        }

        include /usr/local/nginx/conf/sites-enabled/*.conf;
    }
