#include "palloc.h"

#define NGX_POOL_ALIGNMENT       16
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)
#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT   sizeof(unsigned long)    /* platform word */
#endif

#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define ngx_free          free


ngx_uint_t  ngx_pagesize = 4096;

void *
ngx_alloc(size_t size)
{
    void  *p;

    p = malloc(size);
    if (p == NULL) {
        fprintf (stderr, "malloc(%uz) failed", size);
    }

    return p;
}

void *
ngx_memalign(size_t alignment, size_t size)
{
    void  *p;
    int    err;

    err = posix_memalign(&p, alignment, size);  // malloc 的对齐版

    if (err) {
        fprintf (stderr, "posix_memalign(%uz, %uz) failed", alignment, size);
        p = NULL;
    }

    return p;
}

ngx_pool_t *
ngx_create_pool(size_t size)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size);
    if (p == NULL) {
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
 //   p->large = NULL;
    p->cleanup = NULL;

    return p;
}

static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new, *current;

    printf ("===== alloc new block\n");
    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize);
    if (m == NULL) {
        return NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new->d.last = m + size;

    current = pool->current;

    for (p = current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            current = p->d.next;
        }
    }

    p->d.next = new;

    pool->current = current ? current : new;

    return m;
}

void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max) {        // 如果要分配内存小于 pool 最大

        p = pool->current;

        do {
            m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);

            if ((size_t) (p->d.end - m) >= size) {  // 在 pool 链表里面找一个 pool, 剩余内存能放的下 size
                printf ("===== normal \n");
                p->d.last = m + size;
                return m;
            }
            p = p->d.next;
        } while (p);
        // 如果 pool 链表里面没有一个pool 能装下 size, 开辟一个新的 pool
        return ngx_palloc_block(pool, size);
    }

    return ngx_palloc_large(pool, size);
}

static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    printf ("===== large\n");
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size);
    if (p == NULL) {
        return NULL;
    }

    n = 0;

    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
    if (c == NULL) {
        return NULL;
    }

    if (size) {
        c->data = ngx_palloc(p, size);
        if (c->data == NULL) {
            return NULL;
        }

    } else {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    return c;
}

void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {

        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);
        if (n == NULL) {
            break;
        }
    }
}
