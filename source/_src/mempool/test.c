#include <stdio.h>
#include "palloc.h"

void fun(void *p)
{
    printf("call fun\n");
}

int main (int argc, char *const *argv)
{

    ngx_pool_t    *t;
    ngx_pool_cleanup_t *clt;

    void *p1,*p2,*p3;

    t=ngx_create_pool(512); /*创建pool*/

    printf("\n");
    p1=ngx_palloc(t,416); /*分配内存，在可分配内*/
    printf("\n");
    p2=ngx_palloc(t,216);/*分配内存，在可分配内，但pool中没有足够的内存空间*/
    printf("\n");
    p3=ngx_palloc(t,624);/*分配大内存*/
    printf("\n");
    clt=ngx_pool_cleanup_add(t, 816);/*添加释放特殊结构体的函数*/
    clt->handler=&fun;
    ngx_destroy_pool(t);/*pool池注销*/
    printf("\n");
    return 0;
}

