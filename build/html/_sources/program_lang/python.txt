python
================================================



元编程
---------------------------------------


在python 中，普通Object 是类的实例， 类是元类的实例. 如下::

    class A:
        pass

    a = A()

定义了一个类A, 定义了一个实例a, 其中 a是类A的一个实例, 但其实类A也是实例( ``类也是实例`` ), 它
是谁的实例呢，是元类的实例(默认所有类的元类是type)。 当解析器解析到 class A 时，就在内存中创建了
一个对象 A, 只不过这个对象是 ``类对象``, 它可以创造实例



定义一个元类
~~~~~~~~~~~~~~~~~~~~~~~

默认所有类的元类是 type, 要定义一个元类，必须继承自type, 并重写__new__方法, __new__方法在解析到classs
字段的时候会调用，也就是创建一个类的时候，会调用__new__::

    class Metaclass(type):
        def __new__(meta, name, bases, dct):
            """
            它有四个参数:
            meta: 元类，这里是 MetaClass
            name: <要使用此元类的类>的类名
            bases: <要使用此元类的类>的基类
            dct: <要使用此元类的类>的属性, 包括函数，类变量, 如: __init__函数
            """

            dct['add_attr'] = 'haha'

            # 调用type(name, bases, dct) 来创建类
            return super(Base, meta).__new__(meta, name, bases, dct)


使用元类
~~~~~~~~~~~~~~~~~~~~~~~

这样就定义了一个元类, 要使用这个元类， 在普通类中增加 __metaclass__变量， 指向元类::

    class A:
        __metaclass__ = Metaclass
        pass

    a = A()
    print a.add_attr


注意
~~~~~~~~~~~~~~~~~~~~~~~

元类可以被继承, 如果类A的__metaclass__为MetaClass, 类B继承自类A, 则创建类B的时候，
也会用MetaClass类


相关链接:

http://stackoverflow.com/questions/100003/what-is-a-metaclass-in-python


magic 方法
---------------------------------------


__get__
~~~~~~~~~~~~~~~~~~~~~~~


定义了__get__ 方法的类(object子类)的实例，作为另一个类的类变量时，方位这个变量时
会调用__get__方法::

    class attrCls(object):

        default = list

        def __get__(self, instance, owner):
            """
            这个方法称为Descriptor
            @instance: 调用这个属性的实例， 如果是通过类名调的， 这个值为None
            @owner: 调用这个属性的类
            """
            return self.default()

    class Cls:
        objects = attrCls()

    print type(Cls.objects)  ==> list
    Cls.objects.reverse()


并且__get__方法会被继承, 如何attrCls的子类，也会有同样的效果

自实现python版的http框架
---------------------------------------

示例:  :download:`example </_src/py_http_srv/server.py>`



常见问题
---------------------------------------

如何生成跨平台不乱码的csv
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

csv很容易在多个平台之间乱码，使用如下代码可以减少乱码情况::

    fp = codecs.open(dump_file, 'w', encoding='utf16')
    title = u'性名\t平台\tid\t数量\n'
    fp.write(title)

    for items in result:
        try:
            line = '%s\t%s\t%s\t%s\n' % (items[0], items[1], item[2], item[3])
            fp.write(line)
        except Exception as E:
            pass
    fp.close()


unicode转码
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

普通字符串怎么变成unicode::

    > s = u'\u4f60\u597d'
    > print s
    你好
    > s = '\u4f60\u597d'
    > print s
    '\u4f60\u597d'
    > print s.encode('utf-8')
    '\u4f60\u597d'
    > print s.decode('utf-8')
    '\u4f60\u597d'
    > print s.decode('unicode-escape')
    你好
