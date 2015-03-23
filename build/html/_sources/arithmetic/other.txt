===============================================
常见算法
===============================================


一个数组求最大最小值
~~~~~~~~~~~~~~~~~~~~~~~

1. Pick 2 elements(a, b), compare them. (say a > b)
2. Update min by comparing (min, b)
3. Update max by comparing (max, a)

This way you would do 3 comparisons for 2 elements, amounting to 3N/2 total comparisons for N elements.
