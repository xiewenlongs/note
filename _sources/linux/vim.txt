vim 乱码
*************************

vim does't support unicode
-----------------------------------

查看vim 的version ,是否有 +multi_type 选项，如果没有，说明编译vim的时候，没有加
入 --with-features=big



文件编码转换
*************************

其他编码转换utf-8
------------------------------------
    :set encoding=utf-8
    :w!
