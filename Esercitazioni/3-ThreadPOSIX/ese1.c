#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 16

typedef struct
{
    int *array;
    int start;
    int end;
    int sum;
} ThreadData;

void *partial_sum(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->sum = 0;
    for (int i = data->start; i < data->end; i++)
    {
        data->sum = data->array[i];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Inizializza l'array
    pthread_t thread[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int array[ARRAY_SIZE];
    int sum = 0;

    // Divide l'array e crea i thread
    int segment_size = ARRAY_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].array = array;
        thread_data[i].start = i * segment_size;
        thread_data[i].end = (i + 1) * segment_size;
        if (pthread_create(&thread[i], NULL, partial_sum, &thread_data[i]) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
        sum = sum + thread_data[i].sum;
    }

    printf("Total sum: %d\n", sum);

    printf("Thread done!\n");
    return 0;
}