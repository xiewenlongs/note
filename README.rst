个人笔记
===============

http://xiewenlongs.github.io/note


更新
---------

1. 安装相应的工具::

    pip install -r requirement.text


2. 部署到github::

    make update


3. 如果遇到 'gh-pages -> gh-pages (non-fast-forward)' 这样的错， 先把远端gh-pages删掉，再push 一遍::

    # 删除远端gh-pages 分支
    git push origin :gh-pages

    # 把 build/html 目录推送到远端
    ghp-import -p build/html
