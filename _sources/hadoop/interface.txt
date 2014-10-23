


HDFS 接口
-----------------------------------

实例代码:

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
