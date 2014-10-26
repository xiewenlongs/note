#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define ngx_atomic_cmp_set(lock, old, set)                                    \
    __sync_bool_compare_and_swap(lock, old, set)

#define ngx_trylock(lock)  (*(lock) == 0 && ngx_atomic_cmp_set(lock, 0, 1))
#define ngx_unlock(lock) *(lock) = 0

//typedef AO_t uint_t;
//typedef volatile uint_t atomic_t;

int num = 0;
pthread_mutex_t lock;
int aot_lock;


void * th1 (void *argv)
{
    int i, a;
    for (i = 0; i < 3; i++) {
        //pthread_mutex_lock (&lock);
        while (ngx_trylock (&aot_lock) == false);
        printf ("+++++++++\n");
        a = num;
        a--;
        usleep (100000);
        num = a;
        ngx_unlock (&aot_lock);
        //pthread_mutex_unlock (&lock);
        //sleep (0.1);
    }
}

void * th2 (void *argv)
{
    int i, a;
    for (i = 0; i < 3; i++) {
        while (ngx_trylock (&aot_lock) == false);
        //ngx_trylock (&aot_lock);
        //pthread_mutex_lock (&lock);
        printf ("-----------\n");
        a = num;
        a++;
        usleep (100000);
        num = a;
        //pthread_mutex_unlock (&lock);
        ngx_unlock (&aot_lock);
        //sleep (0.1);
    }
}



int main (int argc, const char *argv[])
{
    pthread_t t1, t2;

    pthread_mutex_init (&lock, NULL);

    printf ("start num = %d\n", num);
    pthread_create (&t1, NULL, th1, NULL);
    pthread_create (&t2, NULL, th2, NULL);
    pthread_join (t1, NULL);
    pthread_join (t2, NULL);
    printf ("end num = %d\n", num);
    pthread_mutex_destroy (&lock);
    return 0;
}
