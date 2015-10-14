===============================================
bash
===============================================


PS1 命令行提示符
---------------------------------------

在~/.bashrc中加入::

    export PS1='\n\e[1;37m[\e[m\e[1;32m\u\e[m\e[1;33m@\e[m\e[1;35m\H\e[m \e[4m`pwd`\e[m\e[1;37m]\e[m\e[1;36m\e[m\n\$'


history 在多终端下的表现
---------------------------------------

- There is the history in the history file.
- There is the history in the memory of a bash process.
- The history in the memory of one bash process is not synced with the history in the memory of any other bash process.
- The history in the memory of a bash process is not synced with the history in the file, unless explicitly asked to or during some specific event (see below).
- During startup bash will read the history file. The content of the history file is now in the memory of the bash process.
- During normal use only the history in memory is manipulated.
- During shutdown the history in memory is written to the history file, overwriting any previous content of the history file.
- 可以在 ~/.bashrc 里，加入 ``shopt -s histappend``, 来防止多terminator 相互覆盖.bash_history， 但是时间戳会乱


history 设置
---------------------------------------
- HISTFILESIZE=2000
- HISTSIZE=2000
- HISTTIMEFORMAT="%Y%m%d-%H:%M:%S: "   每个操作，加上时间戳
- export HISTTIMEFORMAT
- shopt -s histappend



scripts example
---------------------------------------

log 跨机器传输(retry): :download:`example </_src/decrypt_nginx_log.sh>`


计算字符串的md5值
---------------------------------------

在终端输入::

    echo -n "863224020062594" | md5 | awk '{print $1}'


终端发送邮件
---------------------------------------

1. 安装sendmail包
2. 配置sendmail /etc/mail.rc::

    set from=<from@com> smtp=<stmp.com>
    set smtp-auth-user=<from@com> smtp-auth-password=<password> smtp-auth=login


3. 启动sendmail服务::

    service sendmail start


4. mail -s 'this is test' yourmail@com


tips
---------------------------------------

- 要在远端最快启动一个静态文件服务器，用以下命令::

    python -m SimpleHTTPServer 7777


- tcpdump 过滤抓包::

    sudo tcpdump -t -XX -i eth0 src host 10.32.30.138  and port 80
