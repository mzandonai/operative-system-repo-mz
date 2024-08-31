#include <stdio.h>
#include <time.h>
#include <sys/shm.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i = atoi(argv[1]);
    printf("i: %d", i);
    printf("\n");

    key_t key = ftok("svr.c", 65);

    int shmid_1 = shmget(key, 10 * sizeof(int), 0666);
    int shmid_2 = shmget(key + 1, 10 * sizeof(int), 0666);
    int shmid_3 = shmget(key + 2, 10 * sizeof(int), 0666);

    int *A = (int *)shmat(shmid_1, NULL, 0);
    int *B = (int *)shmat(shmid_2, NULL, 0);
    int *C = (int *)shmat(shmid_2, NULL, 0);

    C[i] = A[i] * B[i];

    printf("C: \n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", C[i]);
    }
    printf("\n");

    shmdt(A);
    shmdt(B);
    shmdt(C);

    return 0;
}