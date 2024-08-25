#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>

#define N 10

int main(int argc, char *argv[])
{
    size_t memory = 3 * N * sizeof(int);

    key_t key = 1234;
    int shmid = shmget(key, memory, 0666);
    if (shmid == -1)
    {
        perror("Shared Memory error");
        return 1;
    }

    int *shared_memory = (int *)shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1)
    {
        perror("Shmat error");
        return 1;
    }

    int *arrA = shared_memory;
    int *arrB = shared_memory + N;
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        arrA[i] = rand() % 10;
        arrB[i] = rand() % 10;
    }

    shmdt(shared_memory);

    return 0;
}