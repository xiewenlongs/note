===============================================
maven
===============================================


archetype
~~~~~~~~~~~~~~~~~~~~~~~


archetype 简单来说，就是maven预先定义好的一些java项目模板(有j2ee的helloworld, 有webapp的helloword 等), 也可以自己定义
自己的模板, 举个例子::

    > mvn archetype:generate -DgroupId=com.mycompany.app -DartifactId=my-app -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false



注意, ``archetype:generate`` 是生成项目的命令，用 ``-DarchetypeArtifactId`` 来选择模板，这里选择的是 ``maven-archetype-quickstart``

quick-start
~~~~~~~~~~~~~~~~~~~~~~~

**用maven创建一个java项目**::

    > mvn archetype:generate -DgroupId=com.mycompany.app -DartifactId=my-app -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false

创建完会生成一个新项目目录，目录结构比较固定, 如下::

    .
    ├── pom.xml
    └── src
        ├── main
        │   └── java
        │       └── com
        │           └── mycompany
        │               └── app
        │                   └── App.java
        └── test
            └── java
                └── com
                    └── mycompany
                        └── app
                            └── AppTest.java



这个pom.xwl 是这个新项目的主要maven配置, maven会生成一个简单的hello world程序. 接着执行打包::

    > mvn package



他会把项目打成jar包，放在最外面的target目录下, target/my-app-1.0-SNAPSHOT.jar, 接着执行这个jar包试试::


    > java -cp target/my-app-1.0-SNAPSHOT.jar com.mycompany.app.App
    > Hello World!



