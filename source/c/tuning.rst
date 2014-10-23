代码优化
===========

字符串
`````````
大小写转换

  #define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)

