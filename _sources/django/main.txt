




测试
=======================

    from django.utils import unittest
    from django.test.client import Client

    class SimpleTest(unittest.TestCase):

        def setUp(self):
            self.client = Client()

        def test_details(self):
            response = self.client.get(’/customer/details/’)
            self.assertEqual(response.status_code, 200)
            self.assertEqual(len(response.context[’customers’]), 5)




测试类
```````````````

  - django.utils.unittest.TestCase  基本测试类


  - django.test.TransactionTestCase 事务测试类

    每个test_函数执行过后，都会恢复数据库到初始状态。

  - django.test.LiveServerTestCase 活服务测试类
    每次测试的时候，会在后台模拟启动一个服务端(python manager.py runserver), 这就可以用 Selenium 来自动化测试




