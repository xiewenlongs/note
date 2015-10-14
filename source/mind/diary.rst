感悟
===============================================


用户反馈时间戳错乱
~~~~~~~~~~~~~~~~~~~~~~~

尝试使用mongoengine 的 singal 机制::

    @classmethod
    def pre_save(cls, sender, document, **kwargs):
        if not '_id' in document or document._created:
            timestamp = UTILS.current()
            today = UTILS.today()
            document.created_at = timestamp
            document.updated_at = timestamp
            document.date = today
            if document.content:
                document.content = document.content.strip()


对mongoengine document对象的_created 变量没有过多了解，只是按字面意思理解, 导致
所有save() 操作都会更新时间戳. 感悟::
1. 事情对一个变量不了解，那么它很可能引发大得BUG, 不管这种可能性有多小, 它总会发生, 而且在最糟糕的时候发生
2. 数据一定要备份

