===============================================
sentry
===============================================



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
