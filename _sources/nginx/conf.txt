

accept_mutex
````````````````
两个作用:

    * **避免惊群效果** (每个 accept 上一把锁)
    * **负载平衡** (如果当前worker有任务，则不参与竞争，直接返回处理当前任务)

worker_connections      
``````````````````````
    每个worker 的连接池大小。 所以nginx能接收的总的连接数 = worker_connections * worker_processes


unknown
``````````````````
- **worker_rlimit_nofile**    ?
- **keepalive_timeout**
