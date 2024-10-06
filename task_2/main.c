#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "math.h"

struct Point {
    double x;
    double y;
};

int thread_count;
unsigned long point_count;
struct Point* result;
long y_count;
long x_count;
double* ys;
double* xs;

void* routine(void* rank){
	
	return NULL;
}

void init_y_array() {
    y_count = sqrt((double)2*point_count/3) + 1;
    double delta = (double) 2 / y_count;
    double y_cur = -1.0;

    ys = calloc(y_count, sizeof(double));
    for (size_t i = 0; i < y_count; i++) {
        ys[i] = y_cur;
        y_cur += delta;
    }
}

void init_x_array() {
    x_count = 1.5 * y_count + 1;
    double delta = (double) 3 / x_count;
    double x_cur = -2.0;

    xs = calloc(x_count, sizeof(double));
    for (size_t i = 0; i < x_count; i++) {
        xs[i] = x_cur;
        x_cur += delta;
    }
}

void complex_square(struct Point* z) {
    double x = z->x;
    double y = z->y;
    z->x = x*x - y*y;
    z->y = 2*x*y;
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

    init_y_array();
    init_x_array();

    printf("X array with size = %d :", x_count);
    for(int i=0; i<x_count; i++){
        printf("%f ", xs[i]);
    }
    printf("\n");

    printf("Y array with size = %d :", y_count);
    for(int i=0; i<y_count; i++){
        printf("%f ", ys[i]);
    }
    printf("\n");

    long cur_points_i = 0;
    for(long x_i=0; x_i<x_count; x_i++){
        if (cur_points_i == point_count) {
                break;
        }
        for(long y_i=0; y_i<y_count; y_i++){
            if (cur_points_i == point_count) {
                break;
            }
            struct Point z;
            z.x = 0;
            z.y = 0;
            char is_skip = 0;
            for(int i=0; i<4000; i++){
                complex_square(&z);
                z.x += xs[x_i];
                z.y += ys[y_i];
                if (z.x * z.x + z.y * z.y > 4) {
                    is_skip = 1;
                    break;
                }
            }

            if (is_skip == 0) {
                result[cur_points_i].x = xs[x_i];
                result[cur_points_i].y = ys[y_i];
                cur_points_i++;
            }

        }
    }

    double end_time;
    GET_TIME(end_time);

    for (size_t i = 0; i < point_count; i++)
    {
        printf("Result x=%f y=%f\n", result[i].x, result[i].y);
    }
    

    free(result);
    free(ys);
    free(xs);
	//free(threads);
	return 0;
}