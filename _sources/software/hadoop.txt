===============================================
hadoop
===============================================


1. start hadoop
    sh bin/start-all.sh

2. exec wordcount

    a. put input into hdfs
        ./bin/hadoop dfs -put input/a input
    b. exec
        ./bin/hadoop jar example.jar wordcount input output
    c. get output from hdfs
        ./bin/hadoop dfs -get output/part-r-00000 .


Web 管理窗口
```````````````````````````````

    - HDFS 管理窗口
      localhost:50070

    - JobTracker 管理窗口
      localhost:50030




HDFS 接口
-----------------------------------

实例代码::

    import org.apache.hadoop.fs.*;
    import org.apache.hadoop.conf.*;
    class Person
    {
        public String talk() {
        return "Person —— >> talk()" ;
        }
    }

    public class a
    {
        public static void main(String[] args) throws Exception
        {
            Configuration conf = new Configuration ();
            FileSystem hdfs = FileSystem.get (conf);
            FileSystem local = FileSystem.getLocal (conf);

            Path inputDir = new Path (args[0]);
            Path hdfsFile = new Path (args[1]);

            FileStatus[] inputFiles = local.listStatus (inputDir);
            FSDataOutputStream out = hdfs.create (hdfsFile);

            for (int i = 0; i < inputFiles.length; i++) {
                FSDataInputStream in = local.open (inputFiles[i].getPath ());
                byte buffer[] = new byte [256];
                int bytesRead = 0;
                while ((bytesRead = in.read (buffer)) > 0) {
                    out.write (buffer, 0, bytesRead);
                }
                in.close ();
                System.out.println ("aaaa:" + inputFiles[i].getPath ());
            }
            out.close ();
            System.out.println(new Person().talk()) ;
        }
    }



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

this is a test
============================

部署新版本

    * 10.18.10.20 为线上服务器，负责为sdk端提供服务。
    * 10.18.10.96 为离线服务器，负责离线脚本及队列workers的运行。


umengplus的nginx 在 10.18.10.20 上，其配置在 /etc/nginx/sites-enabled/all 中::

    class SimpleTest(TestCase):

        def test_add(self):
            self.assertEqual(1+1, 2)

        def test_bar(self):
            json_obj = self.get('/bar/get/%s/' % TCT.ak)
            self.assertEqual(json_obj['st'], 200)
