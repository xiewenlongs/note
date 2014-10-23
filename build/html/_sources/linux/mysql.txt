
重置slave
=========================

1. >stop slave;
2. >change master_host='xx.xx.xx.xx', master_user='sns',master_bin_log='mysql_bin.00001',master_position=146;
3. >start slave;
4. >show slave status\G; 查看 slave_io_running 是否是yes
