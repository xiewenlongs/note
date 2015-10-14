===============================================
scrapy
===============================================


feature
---------------------------------------

scrapy 有以下特性:

- 内建的支持数据提取（css selector  和 xpath expressions)
- 内建的交互shell, 供debug xpath和css表达式
- 编码检测，自动识别
- 扩展性强(signals, middlewares, extensions, pipeline)
- 很多内建的扩展
- cookies and session handling
- HTTP features like compression, authentication, caching
- user-agent spoofing
- robots.txt
- crawl depth restriction
- and more
- 内建一个telnet控制台，用来查看running中的scrapy进程状态
- 内建一些直接能用的类，如Sitemap Spider, 如自动下载图片的spider


深度 or 广度
---------------------------------------

scrapy 默认是深度遍历， 如果想改成广度遍历，添加::

    DEPTH_PRIORITY = 1
    SCHEDULER_DISK_QUEUE = 'scrapy.squeues.PickleFifoDiskQueue'
    SCHEDULER_MEMORY_QUEUE = 'scrapy.squeues.FifoMemoryQueue


爬虫封禁
---------------------------------------

爬虫可能被网站封掉，最终极的方案是“商业合作”， 在这之前，有一些技巧可以做:

- 收集一个UA池来轮询发请求
- 禁用cookies (某些网站使用cookie来检测爬虫行为)
- 使用delay来防止IP封禁(见DOWNLOAD_DELAY)
- 使用代理IP池， 免费的有TOR, 收费的有http://proxymesh.com/




问题
---------------------------------------

模拟用户登录
http://doc.scrapy.org/en/latest/topics/request-response.html#topics-request-response-ref-request-userlogin
