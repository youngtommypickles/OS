#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NUMBER_ERROR -1
#define THREAD_CREATE_ERROR -2
#define THREAD_JOIN_ERROR -3

pthread_mutex_t mutex;

typedef struct{
	int* first;
	int* second;
	int* result;
	int n, m, lhs, rhs;
}Data;

void* thread_func(void* arg){
	Data* data = (Data*) arg;
	int result[data->n + data->m - 1];
	for (int i = data->lhs; i < data->rhs && i < data->n; ++i) {
		for (int j = 0; j < data->m; ++j) {
			result[i + j] += data->first[i] * data->second[j];
		}
	}

	pthread_mutex_lock(&mutex);
	for(int i = 0; i < data->n+data->m - 1; ++i){
		data->result[i] += result[i];
	}
	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main(int argc, char* argv[]){
	if (argc<2) {
        printf("You must use ./... number of threads \n");
        return 0;
    }
	int thread_number = atoi(argv[1]);
	if(!thread_number){
		printf("n must be > 0\n");
		return 0;
	}
	int n, m;
	printf("Thread number: %d\n", thread_number);
	printf("Enter the number of degree of 1 polynomial: ");
	scanf("%d", &n);
	if(!n){
		printf("n must be > 0\n");
		return 0;
	}
	int* first = malloc(sizeof(int) * n);
	for(int i = n-1; i >= 0; --i){
		scanf("%d", &first[i]);
	}
	printf("Enter the number of degree of 2 polynomial: ");
	scanf("%d", &m);
	if(!m){
		printf("n must be > 0\n");
		return 0;
	}
	int* second = malloc(sizeof(int) * m);
	for(int i = m-1; i >= 0; --i){
		scanf("%d", &second[i]);
	}
	int* result = malloc(sizeof(int) * (n+m));
	for(int i = 0; i < n+m; ++i){
		result[i] = 0;
	}
	pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t) * thread_number);
	pthread_mutex_init(&mutex, NULL);
	Data* data = (Data*)malloc(sizeof(Data) * thread_number);
	int k = (n + thread_number - 1) / thread_number;
	for (int i = 0; i < thread_number; ++i) {
		data[i].first = first;
		data[i].second = second;
		data[i].result = result;
		data[i].lhs = i * k;
		data[i].rhs = (i + 1) * k;
		data[i].n = n;
		data[i].m = m;
	}
	double t0 = clock();
	for (int i = 0; i < thread_number; ++i) {
		if (pthread_create(&thread[i], NULL, thread_func, &data[i])) {
			printf("Error creating thread!\n");
			return THREAD_CREATE_ERROR;
		}
	}
	for (int i = 0; i < thread_number; ++i) {
		if (pthread_join(thread[i], NULL)) {
			printf("Error executing thread!\n");
			return THREAD_JOIN_ERROR;
		}
	}
	double t1 = clock();
	printf("Result: ");
	for (int i = n + m - 2; i >= 0; --i) {
		printf("%d ", result[i]);
	}
	printf("\n");
	printf("Execution time %lf ms\n", (t1 - t0) / 1000.0);
	free(first);
	free(second);
	free(result);
	free(thread);
	free(data);
	pthread_mutex_destroy(&mutex);
	return 0;
}
