#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define N 10

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        perror("Too few arguments\n");
        return 1;
    }

    int position = atoi(argv[1]);
    if (position < 0 || position >= N)
    {
        perror("Invalid position");
        return 1;
    }

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
    int *arrC = shared_memory + 2 * N;

    arrC[position] = arrA[position] * arrB[position];
    printf("CLI2: Array C: position %d value %d\n", position, arrC[position]);

    shmdt(shared_memory);

    return 0;
}