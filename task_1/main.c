#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int thread_count;
int m;
int n;
int* matrix;
int* vector;
int* result;

void* routine(void* rank){
	long my_rank = (long)rank;
	int my_rows_count = m / thread_count;
	int my_first_row = my_rank * my_rows_count;
	int my_last_row = (my_rank+1) * my_rows_count;

	for (long i = my_first_row; i < my_last_row; ++i)
	{
		for (long j = 0; j < n; ++j)
		{
			result[i] += matrix[i*n+j] * vector[j];
		}
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	printf("Hello from main thread!\n");

	thread_count = strtol(argv[1], NULL, 10);
	m = strtol(argv[2], NULL, 10);
	n = strtol(argv[3], NULL, 10);

	pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

	matrix = malloc(n*m*sizeof(int));
	vector = malloc(n*sizeof(int));
	result = calloc(m, sizeof(int));

	for (long i = 0; i < n*m; ++i)
	{
		matrix[i] = rand() % 10;
	}

	printf("Matrix\n");
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%d ", matrix[i*n+j]);
		}
		printf("\n");
	}
	printf("\n");

	for (long i = 0; i < n; ++i)
	{
		vector[i] = rand() % 10;
	}

	printf("Vector\n");
	for (int i = 0; i < n; ++i)
	{
		printf("%d ", vector[i]);
	}
	printf("\n");

	for(long i = 0; i < thread_count; i++){
		pthread_create(&thread_handles[i], NULL, routine, (void*)i);
	}

	for (long i = 0; i < thread_count; ++i)
	{
		pthread_join(thread_handles[i], NULL);
		// pthread_detach(thread_handles[i]);
	}

	printf("Result\n");
	for (int i = 0; i < m; ++i)
	{
		printf("%d ", result[i]);
	}

	free(thread_handles);
	free(result);
	free(matrix);
	return 0;
}