#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"
#include "math.h"

double X_MIN = -2.0;
double X_MAX = 1.0;
double Y_MIN = -1.0;
double Y_MAX = 1.0;
long ITERATION_COUNT = 4000;

struct Point {
    double x;
    double y;
};

int thread_count;
unsigned long point_count;
unsigned long cur_point_count;
struct Point* result;
double* x_right_bounds;
pthread_mutex_t mutex;

void complex_square(struct Point* z) {
    double x = z->x;
    double y = z->y;
    z->x = x*x - y*y;
    z->y = 2*x*y;
}

double get_x_left_bound(long rank) {
    if (rank == 0) {
        return (double)X_MIN;
    }
    return x_right_bounds[rank - 1];
}

double random_range(double from, double to) {
    double range = to - from;
    double div = RAND_MAX / range;
    return (double) from + (rand() / div);
}

void* routine(void* rank){
	long my_rank = (long) rank;
    double x_left = get_x_left_bound(my_rank);
    double x_right = x_right_bounds[my_rank];

    while(cur_point_count < point_count) {
        double y = random_range(Y_MIN, Y_MAX);
        double x = random_range(x_left, x_right);

        struct Point z;
        z.x = 0;
        z.y = 0;
        char is_skip = 0;

        for(long i=0; i<ITERATION_COUNT; i++){
            if (cur_point_count >= point_count) {
                return NULL;
            }

            complex_square(&z);
            z.x += x;
            z.y += y;

            if (z.x * z.x + z.y * z.y > 4) {
                is_skip = 1;
                break;
            }
        }
        if (is_skip == 0) {
            pthread_mutex_lock(&mutex);
            if(cur_point_count >= point_count) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            result[cur_point_count].x = x;
            result[cur_point_count].y = y;
            cur_point_count++;
            pthread_mutex_unlock(&mutex);
        }
    }
	return NULL;
}



void init_x_array() {
    long x_count = thread_count;
    double delta = (double) 3 / x_count;
    double x_cur = (double) X_MIN;

    x_right_bounds = calloc(x_count, sizeof(double));
    for (size_t i = 0; i < x_count; i++) {
        x_cur += delta;
        x_right_bounds[i] = x_cur;
    }
}

void save_result_in_csv_file() {
    FILE *file;
    file = fopen("result.csv", "w+");
    fprintf(file, "X, Y\n");
    for(long i=0; i<point_count; i++) {
        fprintf(file, "%f, %f\n", result[i].x, result[i].y);
    }
    fclose(file);
    printf("Result saved in file 'result.csv'!\n");
}

int main(int argc, char const *argv[])
{
	thread_count = strtol(argv[1], NULL, 10);
	point_count = strtol(argv[2], NULL, 10);

    cur_point_count = 0;
	result = calloc(point_count, sizeof(struct Point));
	pthread_t* threads = malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    init_x_array();

    printf("Thread count: %d, point count: %d!\n", thread_count, point_count);

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

    printf("Time spent: %f\n", (end_time - start_time));
    
    save_result_in_csv_file();
    
    free(result);
    free(x_right_bounds);
    pthread_mutex_destroy(&mutex);
	free(threads);
	return 0;
}