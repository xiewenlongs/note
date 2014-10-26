===============================================
防火墙
===============================================


开一个端口
~~~~~~~~~~~~~~~~~~~~~~~

注意要用 -I, 因为如果 iptable 中设置了 reject ALL, 那么如果用-A 的话， reject 的优先级会在新的append之前::

    $> sudo iptables -I INPUT -p tcp -m state --state NEW -m tcp --dport 9003 -j ACCEPT



保存配置
~~~~~~~~~~~~~~~~~~~~~~~

sudo iptables-save > /etc/iptables.up.rule




