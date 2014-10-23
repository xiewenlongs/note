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

