#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LEN 3000

int C, N, NRT, max_dif = -1, nr1 = -1, nr2 = -1, plin = 0, fd;
int numere[MAX_LEN];

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_p = PTHREAD_COND_INITIALIZER;

void* citire(void* arg) {
    pthread_mutex_lock(&mtx);
    while (plin == 1)
        pthread_cond_wait(&cond_c, &mtx);
    for (int i = 0; i < MAX_LEN; i++)
        read(fd, &numere[i], 2);
    plin = 1;
    pthread_cond_signal(&cond_p);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void* procesare(void* arg) {
    pthread_mutex_lock(&mtx);

    while (plin == 0)
        pthread_cond_wait(&cond_p, &mtx);
    for (int i = 0; i < (MAX_LEN - 1); i++)
        if (numere[i] % 10 == C && numere[i + 1] % 10 == C) {
            int dif = abs(numere[i] - numere[i + 1]);
            if (dif > max_dif) {
                max_dif = dif;
                nr1 = numere[i];
                nr2 = numere[i + 1];
            }
        }
    plin = 0;
    pthread_cond_signal(&cond_c);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main(int argc, char** argv) {
    printf("C: ");
    scanf("%d", &C);
    printf("N: ");
    scanf("%d", &N);

    NRT = 1;
    fd = open("file1", O_RDONLY);

    pthread_t tid_c[NRT], tid_p[NRT];
    for (int i = 0; i < NRT; i++) {
        pthread_create(&tid_c[i], NULL, citire, NULL);
        pthread_create(&tid_p[i], NULL, procesare, NULL);
    }
    for (int i = 0; i < NRT; i++) {
        pthread_join(tid_c[i], NULL);
        pthread_join(tid_p[i], NULL);
    }
    close(fd);
    printf("nr1: %d\n", nr1);
    printf("nr2: %d\n", nr2);
    printf("dif: %d\n", max_dif);

    return 0;
}