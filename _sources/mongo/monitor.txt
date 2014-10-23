issue about mongo performance
----------------------------------

- RAM
- locks time
- page fault
- connections number


性能调优
``````````````````````````````````
- 使用 ``覆盖索引``
- 最好确保 index 大小可以全部放入内存,否则连index都要 swap，就会比较慢

  使用 ``db.collection.totalIndexSize ()`` 来查看单个表 index 大小


性能分析
``````````````````
- 分析性能，使用 ``db.getProfilingStatus()`` 命令

- cursor.hint (index).explain ()  指定使用一个index,并查看它的查询消耗, 下面是一个返回结果:

    {
    "cursor" : "BasicCursor",               ==> 使用索引 (BasicCursor表示没有用索引，全表扫描)
    "isMultiKey" : false,                   ==> 是否使用了多个索引
    "n" : 4372,                             ==> 返回结果数
    "nscannedObjects" : 4372,               ==> 扫描条目
    "nscanned" : 4372,
    "nscannedObjectsAllPlans" : 4372,
    "nscannedAllPlans" : 4372,
    "scanAndOrder" : false,
    "indexOnly" : false,                    ==> 是否是 'cover query'
    "nYields" : 0,
    "nChunkSkips" : 0,
    "millis" : 11,                          ==> 时间
    "indexBounds" : {

    },
    "server" : "index:12346"
    }

性能分析工具
------------------

- mongostat
- mongotop
- db.serverStatus()
- db.stats()
- db.currentOp()
