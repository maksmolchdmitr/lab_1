#ifndef _MY_RWLOCK_H_
#define _MY_RWLOCK_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct my_rwlock
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int num_readers_active;
    int num_writers_waiting;
    char writer_active;
};



int my_rwlock_init (pthread_rwlock_t* lock , /*out*/ pthread_rwlockattr_t attr /*in*/); // никита
int my_rwlock_destroy (pthread_rwlock_t* lock /*out*/);  // макс
int my_rwlock_rdlock ( pthread_rwlock_t* lock /*in/out*/); //никита
int my_rwlock_wrlock ( pthread_rwlock_t* lock /*in/out*/); //макс
int my_rwlock_unlock ( pthread_rwlock_t* lock /*in/out*/);

#endif