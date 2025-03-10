#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <pthread.h>

#define THR_NUM 5
#define VAL_NUM 5

int values[VAL_NUM];

pthread_barrier_t barr;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void list_values(int tid) {
    pthread_mutex_lock(&mtx);
    int i;
    printf("[Tread %2d] The values are: ", tid);
    for (i = 0; i < VAL_NUM; i++)
        printf("%d ", values[i]);
    printf("\n");
    pthread_mutex_unlock(&mtx);
}

void* f(void* a)
{
    int tid = *(int*)a;
    free(a);

    sleep(2);

    pthread_mutex_lock(&mtx);

    printf("[Thread %2d] Enter a value: ", tid);
    scanf("%d", &values[tid]);

    pthread_mutex_unlock(&mtx);

    pthread_barrier_wait(&barr);
    list_values(tid);

    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_barrier_init(&barr, NULL, THR_NUM);                             // init the barrier

    pthread_t thr[THR_NUM];
    for (int i = 0; i < THR_NUM; i++)                                               // create the threads
    {
        int* tid = (int*)malloc(sizeof(int));
        *tid = i;
        int rc = pthread_create(&thr[i], NULL, f, tid);
        if (rc != 0)
            printf("Unable to create thread %d\n", tid);
    }

    printf("The threads were created.\n");
    for (int i = 0; i < THR_NUM; i++)
    {
        pthread_join(thr[i], NULL);                                                     // wait for each thread to finish
    }

    pthread_barrier_destroy(&barr);                                                 // destroy the barrier

    return 0;
}