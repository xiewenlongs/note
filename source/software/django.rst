===============================================
django
===============================================


---------------------------------------
测试
---------------------------------------

测试类

- django.utils.unittest.TestCase  基本测试类


- django.test.TransactionTestCase 事务测试类::

    每个test_函数执行过后，都会恢复数据库到初始状态。

- django.test.LiveServerTestCase 活服务测试类::

    每次测试的时候，会在后台模拟启动一个服务端(python manager.py runserver), 这就可以用 Selenium 来自动化测试



---------------------------------------
manager.py 工具
---------------------------------------


diffsettings
~~~~~~~~~~~~~~~~~~~~~~~

显示setting.py 文件与初始 setting.py 的不同
