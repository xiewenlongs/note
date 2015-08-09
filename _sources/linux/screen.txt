===============================================
screen
===============================================

screen 配置的不好，有可能导致vim花屏， screen 标准配置::

    bind c screen 1
    bind ^c screen 1
    bind 0 select 10
    escape ^Ee

    # the following two lines give a two-line status, with the current window highlighted
    hardstatus alwayslastline
    hardstatus string '%{= kG}[%{G}%H%? %1`%?%{g}][%= %{= kw}%-w%{+b yk} %n*%t%?(%u)%? %{-}%+w %=%{g}][%{B}%m/%d %{W}%C%A%{g}]'

    # # huge scrollback buffer
    defscrollback 5000

    # no welcome message
    startup_message off

    # 256 colors
    attrcolor b ".I"
    termcapinfo xterm 'Co#256:AB=\E[48;5;%dm:AF=\E[38;5;%dm'
    defbce on

    # mouse tracking allows to switch region focus by clicking
    # mousetrack on

    # default windows
    screen -t Shell1  1 bash

    select 0
    bind c screen 1 # window numbering starts at 1 not 0
    bind 0 select 10

