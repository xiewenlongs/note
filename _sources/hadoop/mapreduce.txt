
mapper
-------------------------

- 一个类要作为mapper, 需继承MapReduceBase 基类, 并实现 Mapper 接口

  void configure (JobConfjob)

  void close ()

  void map (K1 key,
            V1 value,
            OutputCollector <K2, V2> output,
            Reporter reporter) throws IOException


partitioner
--------------------------

- 一个定制的patitioner, 只需要实现 configure () 和 getPartition () 函数

  configure: 将hadoop对作业的配置应用到patitioner 上

  getPartition: 返回一个介于0和reduce任务数之间的整数，指向将要发送到的reducer


reduce
--------------------------

- 需要继承 MapReduceBase 基类

  void reducer (K1 key,
                V1 value,
                OutputCollector <K2, V2> output,
                Reporter reporter) throws IOException




运行
------------------

- 启动mapreduce

  > hadoop jar <jar包> <主类> <输入路径> <输出路径>

  如果只运行map, 可以跟参数 -D mapred.reduce.tasks=0
