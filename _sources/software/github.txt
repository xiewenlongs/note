===============================================
github
===============================================




---------------------------------------
FAQ
---------------------------------------

1. ``github Key is already in use``

   出现这个的原因是你在以前也用过GitHub, 并且提交了你的密钥. 这个时候你可以通过在命令行里输入::

        ssh -T -i ~/.ssh/id_rsa git@github.com

   来查看到底是哪一个账户在使用此密钥，会出现如下提示::

        Hi <XXX>! You've successfully authenticated, but GitHub does not provide shell access.

   就是这个XXX账号， 占用了当前sshkey, 登陆这个账号，删除掉sshkey就行了
