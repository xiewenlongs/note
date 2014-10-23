性能调优
*************************




调试nginx
````````````````

- 方法1: 打开 --with-debug

- 方法2: Debugging nginx with DTrace pid provider
  http://nginx.org/en/docs/nginx_dtrace_pid_provider.html


负载均衡
`````````````````

- :DNS 轮循:
  由于存在缺点，所以一般用于可靠性要求不高的集群，如：图片服务器、纯静态网页服务器

  缺点:
        - 可靠性低
          假设一个域名DNS 轮循多台服务器，如果其中一台服务器发生故障，那么所有访问该
          服务器的请求都不会有所回应，这是任何人都不愿意看到的。即时从DNS中去掉该服务器
          的ＩＰ，　但是在Internet上，各地区电信的接入商，都会将DNS放入缓存。ＤＮＳ记录
          全部生效需要几个小时，甚至更久。

        - 负载分配不均匀
          虽然采用简单的轮循算法。但是由于各层缓存的问题，还是会出现某些服务器负载偏高，
          某些很低。


- :反向代理:



deferred
```````````````````````

    listen 80 deferred;

配置调优
````````````````````
    :timer_resolution 100ms;:

        默认情况下，每次内核事件调用，都要执行一次 gettimeofday, 来更新nginx中的缓存时钟

    :worker_cpu_affinity 1000 0100 0010 0001;:

        (绑定４CPU的例子)

    :sendfile on;:
        启用sendfile 系统调用，来减少static file 请求时，用户态和内核态的切换时间


