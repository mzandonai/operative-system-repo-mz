#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10

int main(int argc, char *argv[])
{
    size_t memory = 3 * N * sizeof(int);

    key_t key = 1234;
    int shmid = shmget(key, memory, IPC_CREAT | 0666);
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

    printf("Child generating...\n");
    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        perror("Fork error");
        return 1;
    }
    else if (pid == 0)
    {
        printf("We are inside the child\n");
        execl("./cli1", "./cli1", NULL);

        perror("execl");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
        printf("Generating 10 child...\n");
        for (int i = 0; i < 10; i++)
        {
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("Fork error");
                return 1;
            }
            else if (pid == 0)
            {
                char position[N];
                snprintf(position, sizeof(position), "%d", i);
                execl("./cli2", "./cli2", position, NULL);
            }
        }

        for (int i = 0; i < N; i++)
        {
            wait(NULL);
        }

        int sum = 0;
        for (int i = 0; i < N; i++)
        {
            sum += arrC[i];
        }
        printf("Averace C: %f\n", (float)sum / N);

        if (shmdt(shared_memory) == -1)
        {
            perror("shmdt error");
            return 1;
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("shmctl error");
            return 1;
        }
    }

    return 0;
}