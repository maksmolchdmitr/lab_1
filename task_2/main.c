#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

struct Point {
    double x;
    double y;
};

int thread_count;
unsigned long point_count;
struct Point* result;

void* routine(void* rank){
	
	return NULL;
}

int main(int argc, char const *argv[])
{
	thread_count = strtol(argv[1], NULL, 10);
	point_count = strtol(argv[2], NULL, 10);


	result = calloc(point_count, sizeof(struct Point));
	//pthread_t* threads = malloc(thread_count * sizeof(pthread_t));

    printf("Thread count: %d, point count: %d!\n", thread_count, point_count);

    double start_time;
    GET_TIME(start_time);

    

    double end_time;
    GET_TIME(end_time);

	printf("Result\n");
    

    free(result);
	//free(threads);
	return 0;
}