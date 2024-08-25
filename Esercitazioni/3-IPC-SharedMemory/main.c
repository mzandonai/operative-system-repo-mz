#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{

    printf("Benvenuto...\n");

    printf("Shared memory configuration...\n");
    key_t key = ftok(".", 'A');
    size_t size = 1024;
    int shmid;

    shmid = shmget(key, size, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Shmid error");
    }
    else
    {
        printf("Shm done!\n");
    }

    printf("Try to attach the new shm...\n");
    char *str = (char *)shmat(shmid, NULL, 0);

    pid_t pid = fork();
    printf("PID: %d\n", pid);
    if (pid == 0)
    {
        sleep(1);
        printf("Siamo nel figlio...%s\n", str);
        shmdt(str);
    }
    else
    {
        printf("Siamo nel padre...\n");
        sprintf(str, "Ciao figlio mio!!!");
        wait(NULL);
        // Distacco del segmento di memoria condivisa
        shmdt(str);

        // Distrugge il segmento di memoria condivisa
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}