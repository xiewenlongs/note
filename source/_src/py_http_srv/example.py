#!/usr/bin/env python
# -*- coding: utf8 -*-


def tt(request, response_head):
    # print request.form
    # print request.getdic
    # print request.postdic
    # itemname = request.form["file"].filename
    # value = request.form["file"].file.read()
    return "ccb"+request.path
