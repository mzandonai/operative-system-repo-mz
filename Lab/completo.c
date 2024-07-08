#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 10

// Struttura per passare i vettori al thread
typedef struct
{
    int *A;
    int *B;
} Vectors;

// Funzione che il thread eseguirà
void *initialize_vectors(void *arg)
{
    Vectors *vec = (Vectors *)arg;
    srand(time(NULL)); // Inizializza il generatore di numeri casuali

    for (int i = 0; i < SIZE; i++)
    {
        vec->A[i] = rand() % 10;
        vec->B[i] = rand() % 10;
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t thread;
    int A[SIZE], B[SIZE];
    Vectors vec = {A, B};

    // Creare il thread per inizializzare i vettori
    if (pthread_create(&thread, NULL, initialize_vectors, (void *)&vec) != 0)
    {
        fprintf(stderr, "Errore nella creazione del thread\n");
        return 1;
    }

    // Attendere che il thread termini
    if (pthread_join(thread, NULL) != 0)
    {
        fprintf(stderr, "Errore nel join del thread\n");
        return 2;
    }

    // Stampare i vettori
    printf("Vettore A: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("Vettore B: ");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", B[i]);
    }
    printf("\n");

    return 0;
}
