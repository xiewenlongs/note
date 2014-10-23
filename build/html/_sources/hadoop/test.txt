this is a test
======

部署新版本

    * 10.18.10.20 为线上服务器，负责为sdk端提供服务。
    * 10.18.10.96 为离线服务器，负责离线脚本及队列workers的运行。


umengplus的nginx 在 10.18.10.20 上，其配置在 /etc/nginx/sites-enabled/all 中:

class SimpleTest(TestCase):

    def test_add(self):
        self.assertEqual(1+1, 2)

    def test_bar(self):
        json_obj = self.get('/bar/get/%s/' % TCT.ak)
        self.assertEqual(json_obj['st'], 200)
