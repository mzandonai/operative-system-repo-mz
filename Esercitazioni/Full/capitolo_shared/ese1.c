#include <stdio.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <wait.h>

int main(int argc, char *argv[])
{
    int size = 5;
    int v1[] = {1, 2, 3, 4, 5};
    int v2[] = {9, 2, 5, 1, 4};

    key_t key = ftok(".", 'A');

    int shmid = shmget(key, size * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("smid error");
        return 1;
    }

    int *ptr = (int *)shmat(shmid, NULL, 0);
    printf("PTR: %p\n", ptr);

    for (int i = 0; i < size; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            ptr[i] = v1[i] * v2[i];
            printf("Child %d: computed %d * %d = %d\n", i, v1[i], v2[i], ptr[i]);
            exit(0);
        }
    }

    for (int i = 0; i < size; i++)
    {
        wait(NULL);
    }

    int res = 0;
    for (int i = 0; i < size; i++)
    {
        res = res + ptr[i];
    }

    printf("Final res: %d\n", res);

    return 0;
}
