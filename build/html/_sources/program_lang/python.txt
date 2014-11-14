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
