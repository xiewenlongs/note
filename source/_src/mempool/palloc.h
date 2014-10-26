#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"


#define NGX_POOL_ALIGNMENT       16

typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

typedef struct ngx_pool_large_s  ngx_pool_large_t;

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};


typedef struct {
    u_char               *last;         // 本 pool 的最大ptr
    u_char               *end;          // 本 pool 已使用到的 ptr
    ngx_pool_t           *next;         // 本 pool 如果用完，下一个 pool的 ptr
    ngx_uint_t            failed;       // 开辟失败的次数
} ngx_pool_data_t;

struct ngx_pool_s {
    ngx_pool_data_t       d;
    size_t                max;
    ngx_pool_t           *current;
    //ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
};


struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};

ngx_pool_t *
ngx_create_pool(size_t size);

static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size);

void *
ngx_palloc(ngx_pool_t *pool, size_t size);

static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size);

ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);

void
ngx_destroy_pool(ngx_pool_t *pool);
