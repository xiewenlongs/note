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


---------------------------------------
其他
---------------------------------------


信号
~~~~~~~~~~~~~~~~~~~~~~~

django 的信号，其实就是定义一个全局类， 这个类的作用就是观察者， 可以注册
一堆callback, 最后通过send()方法来调用每个signal 上的callback. **它的作用就是解藕, 无其他作用**


.. image:: ../_static/django-signal.png
   :align: center

第一步, **注册 signal**,  这步建议把代码写在每个app下的signal.py 文件中::

    import django.dispatch
    delete_done = django.dispatch.Signal(providing_args=['obj'])


第二步, **关联对应的listener**, 建议把这行代码写在 models.py 中::

    class Article:
        pass

    def callback(sender, **kwargs):
        pass

    signals.delete_done.connect(callback, sender=Article)


第三步, **触发信号**::

    signals.delete_done.send(sender=Article, obj=self)


.. code-block:: console

    $ pip install -U mongoengine
