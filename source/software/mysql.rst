===============================================
mysql
===============================================

重置slave
=========================

1. >stop slave;
2. >change master_host='xx.xx.xx.xx', master_user='sns',master_bin_log='mysql_bin.00001',master_position=146;
3. >start slave;
4. >show slave status\G; 查看 slave_io_running 是否是yes


存储引擎
========================

MyISAM 和 InnoDB索引 这两种索引都是用B+数来存储。 不同在于:

- 对于MyISAM存储，索引和数据分离， 索引使用B+树建立，叶子节点只保存磁盘上真正数据的地址
- 而对于InnoDB不同， 它分两种情况: 主键索引 和 辅助索引

  - 主键索引B+树的叶子节点保存整条记录的完整信息
  - 辅助索引的叶子节点，保存的是相应记录对应主键的值。

  所以Innodb使用辅助索引查找时，会查询两遍。 但是用主键索引的查询，非常快。


.. warning::
    Innodb的主键长度不要太长，因为每个辅助索引，叶子节点都会把主键存一遍


Auto Increment ID
---------------------------------------

InnoDB引擎的 Auto Increment ID 可能不连续，导致不连续有两种情况:

- 删除记录
- rollback 会话

