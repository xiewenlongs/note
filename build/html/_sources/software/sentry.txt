===============================================
sentry
===============================================

|

配置
---------------------------------------

安装
~~~~~~~~~~~~~~~~~~~~~~~

安装sentry前，需要先安装mysql 和redis. sentry可以用pip 安装::

    > pip install sentry

启动
~~~~~~~~~~~~~~~~~~~~~~~

先启动mysql::

    > /etc/init.d/mysqld restart

再创建一份sentry新配置文件::

    > sentry init <配置文件名>

修改配置文件，配置database信息，如下::

    DATABASES = {
            'default': {
                # You can swap out the engine for MySQL easily by changing this value
                # to ``django.db.backends.mysql`` or to PostgreSQL with
                # ``django.db.backends.postgresql_psycopg2``

                # If you change this, you'll also need to install the appropriate python
                # package: psycopg2 (Postgres) or mysql-python

                'ENGINE': 'django.db.backends.mysql',
                'NAME': '数据库名',
                'USER': '账号名',
                'PASSWORD': 'msyql密码',
                'HOST': 'localhost',
                'PORT': '3307',
                }
    }

接下来需要先搞好mysql::

    # 创建数据库
    mysql> create database 数据库

    # 创建账号
    mysql> create user 用户@主机 IDENTIFIED by '密码'

    # 给这个用户授权

    mysql> grant all on sentry.* to user@'localhost'


接下来搞好redis, sentry 内部用redis是用来做缓存的，提高性能. Redis启动略过，sentry里redis配置如下::

    SENTRY_BUFFER = 'sentry.buffer.redis.RedisBuffer'
    SENTRY_REDIS_OPTIONS = {
        'hosts': {
            0: {
                'host': '127.0.0.1',
                'port': 6379,
            }
        }
    }



接下来在启动sentry 之前， 可以选择两种worker处理方式:

    1. 一种是celery队列异步处理。这种方式除了启动sentry web 外，还要启动多个celery worker进程， 注意，这些celery worker
       进程可能会crash掉，所以需要用supervisor 来监控
    2. 另一种是不用celery队列，来了一个event事件，同步处理，性能可能差些。但是不用起celery worker进程了，不用担心celery
       crash 掉

如果是用celery队列，那么先启动celery worker::

    # 起3个worker
    > sentry --config=./sentry.settings.py celery worker -B &
    > sentry --config=./sentry.settings.py celery worker -B &
    > sentry --config=./sentry.settings.py celery worker -B &


接下来就是启动main server::

    > sentry --config=./sentry.settings.py start


然后创建superuser::

    > sentry --config=senry.settings.py createsuperuser


整个过程完成，现在可以用web登陆sentry了

|

报警邮件
~~~~~~~~~~~~~~~~~~~~~~~

configure as this::

    EMAIL_BACKEND = 'django.core.mail.backends.smtp.EmailBackend'
    EMAIL_HOST = 'smtp-inc.alibaba-inc.com'
    EMAIL_HOST_PASSWORD = 'yourpassword'
    EMAIL_HOST_USER = 'service@umeng.com'
    EMAIL_PORT = your_port
    EMAIL_USE_TLS = True
    SERVER_EMAIL = 'service@umeng.com'


**Cannot receive email notifytion After configure email**:

Enter email test web(http://localhost:9000/manage/status/mail/) and type [send a test email]. If it report
``STARTTLS extension not supported by server`` ,  you must set ``EMAIL_USE_TLS = False`` in your sentry configure

|

问题
---------------------------------------


**启动时报错: Transaction managed block ended with pending COMMIT/ROLLBACK**::


    $> sentry start

    Performing upgrade before service startup...
    Syncing...
    Creating tables ...
    Installing custom SQL ...
    Installing indexes ...
    Installed 0 object(s) from 0 fixture(s)
    Migrating...
    Running migrations for djcelery:
       - Migrating forwards to 0004_v30_changes.
          > djcelery:0001_initial
          TransactionManagementError: Transaction managed block ended with pending COMMIT/ROLLBACK


这是因为权限问题, sentry配置文件里的user, 没有足够的权限操作sentry数据库, 用超级账号调用::

    mysql> grant all on sentry.* to user@'localhost'

**sentry After start sentry web, cannot mark event to reslove**:

This maybe cause by SENTRY_URL_PREFIX configure. If your add resversal proxy in front of sentry, you must set
``SENTRY_URL_PREFIX`` to the IP of resversal proxy


