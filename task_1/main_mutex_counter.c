#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

int thread_count;
unsigned long trials_count;
unsigned long trials_on_thread;
unsigned long result;
pthread_mutex_t mutex;

double randPoint(){
    double max = 1.0;
    double min = -1.0;
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void* routine(void* rank){
	long my_rank = (long)rank;
    for(long i = 0; i < trials_on_thread; i++){
        double x = randPoint();
        double y = randPoint();
        if(x * x + y * y < 1){
            pthread_mutex_lock(&mutex);
            result++;
            pthread_mutex_unlock(&mutex);
        }
    }
	return NULL;
}

int main(int argc, char const *argv[])
{
	printf("Hello from main thread!\n");
	thread_count = strtol(argv[1], NULL, 10);
	trials_count = strtol(argv[2], NULL, 10);
    trials_on_thread = trials_count/thread_count;
	result = 0l;
	pthread_t* threads = malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    printf("Thread count: %d, tries count: %d!\n", thread_count, trials_count);

    double start_time;
    srand(NULL);
    GET_TIME(start_time);
	for(long i = 0; i < thread_count; i++){
		pthread_create(&threads[i], NULL, routine, (void*)i);
	}

	for (long i = 0; i < thread_count; ++i)
	{
		pthread_join(threads[i], NULL);
	}

    double end_time;
    GET_TIME(end_time);

	printf("Result\n");
    double pi = 0;
    pi = 4 * result / (double) trials_count;
    printf("PI = %f, time start: %f, end: %f, spent: %f", pi, start_time, end_time, (end_time - start_time));

    pthread_mutex_destroy(&mutex);
	free(threads);
	return 0;
}