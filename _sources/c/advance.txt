高级
======

自实现锁
````````````````````

使用gcc 提供的 ``__sync_bool_compare_and_swap``, 可以实现自己的锁，该函数是一个
原子操作

    __sync_bool_compare_and_swap(lock, old, set)

    例子: src/c_self_lock.c
    相关链接:null
