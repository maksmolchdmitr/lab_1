#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

int thread_count;
int trials_count;
int trials_on_thread;
int* results;

double randPoint(){
    double max = 1.0;
    double min = -1.0;
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void* routine(void* rank){
	long my_rank = (long)rank;
    for(int i = 0; i < trials_on_thread; i++){
        double x = randPoint();
        double y = randPoint();
        if(x * x + y * y < 1){
            results[my_rank]++;
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
	results = calloc(thread_count, sizeof(int));
	pthread_t* threads = malloc(thread_count * sizeof(pthread_t));

    double start_time;
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
	for (int i = 0; i < thread_count; ++i)
	{
        pi += results[i];
	}
    pi /= trials_count;
    pi *= 4;
    printf("PI = %f, time start: %f, end: %f, spent: %f", pi, start_time, end_time, (end_time - start_time));

	free(threads);
	free(results);
	return 0;
}