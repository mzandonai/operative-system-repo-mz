#include <pthread.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
    int *A;
    int *B;
    int *C;
    int size;
    int i;
    float media;
} data;

void *init_routine(void *arg)
{
    data *thread_arg = (data *)arg;

    srand(time(NULL));

    for (int i = 0; i < thread_arg->size; i++)
    {
        thread_arg->A[i] = rand() % 10;
        thread_arg->B[i] = rand() % 10;
    }
    printf("A: \n");
    for (int i = 0; i < thread_arg->size; i++)
    {
        printf("%d ", thread_arg->A[i]);
    }
    printf("\nB: \n");
    for (int i = 0; i < thread_arg->size; i++)
    {
        printf("%d ", thread_arg->B[i]);
    }

    return NULL;
}

void *calculate_routine(void *arg)
{
    data *thread_arg = (data *)arg;
    printf("\n");
    for (int i = 0; i < thread_arg->size; i++)
    {
        thread_arg->C[i] = thread_arg->A[i] * thread_arg->B[i];
        printf(" %d", thread_arg->C[i]);
    }

    return NULL;
}

void *media_calculator(void *arg)
{
    data *thread_arg = (data *)arg;

    int somma = 0;
    for (int i = 0; i < thread_arg->size; i++)
    {
        somma = somma + thread_arg->C[i];
    }

    thread_arg->media = somma / thread_arg->size;

    printf("\nMedia: %f\n", thread_arg->media);

    return NULL;
}

int main(int argc, char *argv[])
{
    // Tre vettori A, B, C 10 elementi
    //  1° thread --> init A e B con valori casuali da 0-9
    //  2° thread "op" deve fare C[i] = A[i] * B[i] i è un par della thread
    //  3° thread calcola valore medio dell'array C e lo inserisce in variabile M
    //  Main deve essere una thread, eseguire la prima thread e attendo il suo termine
    //  Eseguire 10 thread op, una per ogni posizione i, attendo che terminino

    data thread_arg;

    int size = 10;
    int A[size];
    int B[size];
    int C[size];
    thread_arg.A = A;
    thread_arg.B = B;
    thread_arg.C = C;
    thread_arg.size = size;

    pthread_t thread1;
    pthread_t thread2[size];
    pthread_t thread3;

    pthread_create(&thread1, NULL, init_routine, &thread_arg);
    pthread_join(thread1, NULL);

    for (int i = 0; i < size; i++)
    {
        pthread_create(&thread2[i], NULL, calculate_routine, &thread_arg); /* code */
    }

    for (int i = 0; i < size; i++)
    {
        pthread_join(thread2[i], NULL); /* code */
    }

    pthread_create(&thread3, NULL, media_calculator, &thread_arg);
    pthread_join(thread3, NULL);

    return 0;
}