===============================================
demo config
===============================================


This is a max-optimizal nginx config::


    #####################
    # global config
    #####################

    user admin admin;
    pid /var/run/nginx.pid;
    worker_processes auto;
    worker_rlimit_nofile 102400;

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

        limit_conn_zone $binary_remote_addr zone=addr:5m;
        limit_conn addr 100;

        include /etc/nginx/mime.types;
        default_type text/html;
        charset UTF-8;


        gzip on;
        gzip_disable "msie6";       # 为IE6关闭gzip, 兼容老浏览器
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

        ##
        #proxy config
        ##
        proxy_temp_path             /dev/shm/nginx/proxy_temp 1 2;
        proxy_cache_path            /dev/shm/nginx/proxy_cache  levels=1:2 keys_zone=cache:128m inactive=1d max_size=1g;
        proxy_connect_timeout       60s;
        proxy_read_timeout          60s;
        proxy_send_timeout          60s;

        #####################
        # http config
        #####################

        server {
            listen 80;
            server_name     localhost;

            location / {
                deny all;
            }

            location /nginx_status {
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
