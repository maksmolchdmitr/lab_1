#include "my_rwlock.h"

int my_rwlock_init (my_rwlock* lock) {
    if (pthread_cond_init(&(lock->cond), NULL) != 0) {
        return -1;
    }

    if (pthread_mutex_init(&(lock->mutex), NULL) != 0) {
        return -1;
    }

    lock->num_readers_active = 0;
    lock->num_writers_waiting = 0;
    lock->writer_active = 0;

    return 0;
}

int my_rwlock_destroy (my_rwlock* lock /*out*/){
    if (pthread_mutex_destroy(&(lock->mutex)) != 0)  {
        return -1;
    }
    
    if (pthread_cond_destroy(&(lock->cond)) != 0)  {
        return -1;
    }
    
    free(lock);

    return 0;
}

int my_rwlock_rdlock (my_rwlock* lock /*in/out*/){
    return 0;
}

int my_rwlock_wrlock (my_rwlock* lock /*in/out*/){
    pthread_mutex_lock(&lock->mutex);
    lock->num_writers_waiting++;
    while (lock->num_readers_active > 0 || lock->writer_active==1){
        pthread_cond_wait(&lock->cond, &lock->mutex);
    }
    lock->num_writers_waiting--;
    lock->writer_active = 1;
    pthread_mutex_unlock(&lock->mutex);
    return 0;
}

int my_rwlock_unlock (my_rwlock* lock /*in/out*/){
    pthread_mutex_lock(&lock->mutex);
    if(lock->num_readers_active > 0) {
        lock->num_readers_active--;
    }
    lock->writer_active = 0;
    pthread_cond_signal(&lock->cond);
    pthread_mutex_unlock(&lock->mutex);
    return 0;
}