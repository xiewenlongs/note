C语言
=============


------------
高级
------------

自实现锁
~~~~~~~~~~~~~

使用gcc 提供的 ``__sync_bool_compare_and_swap``, 可以实现自己的锁，该函数是一个
原子操作:

.. code-block:: c

   // 如果 \*lock 的值等于 old, 那么就把set 写入*lock, 否则不写
   __sync_bool_compare_and_swap(lock, old, set)

示例:  :download:`example </_src/c_self_lock.c>`


.. _prog_c_pool:

内存池
~~~~~~~~~~~~~~~

nginx 的内存池实现

示例:  :download:`example </_src/mempool/palloc.c>`

相关链接: http://www.alidata.org/archives/1390

相关链接: http://www.cnblogs.com/jzhlin/archive/2012/06/06/2537710.html


哈希表
~~~~~~~~~~~~~

glib 库(gnome基本库) 提供了一套 哈希表

示例:  :download:`example </_src/gnome_hash_table.c>`

相关链接:  https://developer.gnome.org/glib/2.30/glib-Hash-Tables.html#g-hash-table-new


终端下UI库
~~~~~~~~~~~~~

curses 库

示例:  :download:`example </_src/c_term_graph_lib.c>`



正则表达式
~~~~~~~~~~~~~

PCRE:  很多开源项目都用它来支持正则，如: nginx, apache

示例:  :download:`example </_src/c_pcre_lib.c>`

相关链接:  http://www.pcre.org/pcre.txt


-----------------------------------
代码优化
-----------------------------------


牛逼宏定义
~~~~~~~~~~~~~


大小写转换:

.. code-block:: c

   #define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
