===============================================
flask
===============================================


flask 是一个web框架，相当于 ``flask = Werkzeug + jinja2 + threadlocal``


模板
===============================================

flask 的模板默认是使用jinja2, 你也可以使用其他模板渲染


注册Filters
---------------------------------------

修改 app.jinja_env 就可以



Context Processors
---------------------------------------

你可以为模板渲染注入新的变量， 通过Context Processors, 如下, 就是为模板增加了一个user变量::

    @app.context_processor
    def inject_user():
        return dict(user=g.user)

