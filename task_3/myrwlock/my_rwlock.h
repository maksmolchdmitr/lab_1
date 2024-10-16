#ifndef _MY_RWLOCK_H_
#define _MY_RWLOCK_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct my_rwlock
{
    pthread_mutex_t mutex;
    pthread_cond_t read_cond;
    pthread_cond_t write_cond;
    int num_readers_waiting;
    int num_writers_waiting;
    int num_readers_active;
    char writer_active;
} my_rwlock;



int my_rwlock_init (my_rwlock* lock);
int my_rwlock_destroy (my_rwlock* lock); 
int my_rwlock_rdlock (my_rwlock* lock);
int my_rwlock_wrlock (my_rwlock* lock);
int my_rwlock_unlock (my_rwlock* lock);

#endif