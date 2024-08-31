#include <stdio.h>
#include <time.h>
#include <sys/shm.h>
#include <wait.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    key_t key = ftok("svr.c", 65);

    int shmid_1 = shmget(key, 10 * sizeof(int), 0666);
    int shmid_2 = shmget(key + 1, 10 * sizeof(int), 0666);

    int *A = (int *)shmat(shmid_1, NULL, 0);
    int *B = (int *)shmat(shmid_2, NULL, 0);

    printf("A: ");
    for (int i = 0; i < 10; i++)
    {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d ", A[i]);
    }

    printf("B: ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", B[i]);
    }
    printf("\n");
    shmdt(A);
    shmdt(B);

    return 0;
}