
机制
``````````````````

    master 把接收的请求，写入 oplog 表里。 oplog 表记录了所有的操作，也就是说，master
和 slave 之间传递的是 oplog里的记录
    oplog 是一个 capped 表，它的大小，可以配置的，可以用 ``db.printReplicationInfo()`` 查看




步骤
````````````````````

#. 有两天机器pc1, pc2, 保证两台机器的互通

#. 分别在pc1, pc2 上启动mongod

    pc1 > mongod --port 12345 --dbpath /srv/mongodb/rs0-0 --replSet rs0 --smallfiles --oplogSize 128
    pc2 > mongod --port 12346 --dbpath /srv/mongodb/rs0-0 --replSet rs0 --smallfiles --oplogSize 128

#. 在primary 上配置rs0

    pc1 > mongo --port 12345
    mongo> rsconf = {
                    _id: "rs0",
                    members: [
                                {
                                _id: 0,
                                host: "<hostname>:27017"        ===> 别用 localhost ,用对外ip
                                }
                        ]
                    }

    pc1 > rs.initiate (rsconf)

    查看rs状态

    pc1 > rs.status ()

#. 添加别的机器(pc2) 到 rs0

    pc1 > mongo --port 12345
    mongo > rs.add ("<pc2的ip>:port")


#. 到此完成了，还可以添加 Arbiter 到 master 和 slave


#. slave 不支持一般 mongo 操作，除非在slave上使用 ``rs.slaveOk ()`` 才可以




