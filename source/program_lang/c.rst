C语言
=============

自实现锁
~~~~~~~~~~~~~

使用gcc 提供的 **__sync_bool_compare_and_swap**, 可以实现自己的锁，该函数是一个
原子操作:

.. code-block:: c

   // 如果 \*lock 的值等于 old, 那么就把set 写入*lock, 否则不写
   __sync_bool_compare_and_swap(lock, old, set)

用法示例:  :download:`example </_src/c_self_lock.c>`


.. _prog_c_pool:

内存池
~~~~~~~~~~~~~~~

nginx 的内存池实现

用法示例:  :download:`example </_src/mempool/palloc.c>`

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


内存覆盖bug
~~~~~~~~~~~~~~~~~~~~~~~

strcpy函数并不会检测参数范围，如下

.. code-block:: c
    :linenos:

    include <stdio.h>
    int main ()
    {
        int flag;
        char buf[10];

        strcpy (buf, argv[1]);

        if (strcmp (“abc123”, buf) == 0) {
            flag = 1;
        } else
            flag = 0;

        if (flag) {
            printf (“successful”);
        }
    }


这是一个验证密码的程序, 有办法让你不知道密码，也能登陆。只要输入的字符串足够长就行, 原理: **strcpy 不会考虑第一个参数
的内存大小，如果 argv[1]足够大 导致buf已经复制不下，那么会 溢出复制。就是往内存中后续的 地址复制**。 内存中存储可能如下::

    --------------
    | --内存中---|
    |____________|
    | buf 变量   |
    |____________|
    | flag 变量  |
    --------------

当输入足够长， 利用strcpy函数不保证边界检测的特性， 后续的字符会覆盖下面的变量flag


牛逼宏定义
~~~~~~~~~~~~~


大小写转换:

.. code-block:: c

   #define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
