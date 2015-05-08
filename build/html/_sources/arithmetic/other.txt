===============================================
常见算法
===============================================


一个数组求最大最小值
~~~~~~~~~~~~~~~~~~~~~~~

1. Pick 2 elements(a, b), compare them. (say a > b)
2. Update min by comparing (min, b)
3. Update max by comparing (max, a)

This way you would do 3 comparisons for 2 elements, amounting to 3N/2 total comparisons for N elements.



找出海量数据最大的前10个数
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

使用最小堆， 遍历海量数组的同时，构造一个最小二叉树。这个二叉树有以下特点:

1. 根的数最小
2. 根左边的元素，全部比根小；根右边的元素，全部比根大


每次遍历到一个数， 先用它和根相比。如果比根小，pass. 如果比根大，那么把它加入根. 这样的话，越往后，遍历一个数越快(因为
最小堆越来越大)

|

详情见: http://www.benfrederickson.com/heap-visualization/
