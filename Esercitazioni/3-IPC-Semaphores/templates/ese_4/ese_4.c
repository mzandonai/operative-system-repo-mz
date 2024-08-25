#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>

void semOp(int semid, unsigned short sem_num, short sem_op)
{
    struct sembuf sop = {
        .sem_num = sem_num, // Indice del semaforo nell'array
        .sem_op = sem_op,   // Operazione da eseguire (P = -1, V = +1)
        .sem_flg = 0        // Flag
    };

    // Esegui l'operazione sul semaforo
    if (semop(semid, &sop, 1) == -1)
    {
        printf("semop failed\n");
    }
}

int main(int argc, char *argv[])
{
    key_t key = ftok(".", 'A');
    int semid = semget(key, 3, IPC_CREAT | 0666);

    semctl(semid, 0, SETVAL, 0);
    semctl(semid, 1, SETVAL, 0);
    semctl(semid, 2, SETVAL, 1);

    if (fork() == 0)
    {
        semOp(semid, 2, -1);
        printf("Uno - ");
        semOp(semid, 1, 1);
        exit(0);
    }

    if (fork() == 0)
    {
        semOp(semid, 1, -1);
        printf("Due - ");
        semOp(semid, 0, 1);
        exit(0);
    }

    if (fork() == 0)
    {
        semOp(semid, 0, -1);
        printf("Tre\n");
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    semctl(semid, 0, IPC_RMID);

    return 0;
}