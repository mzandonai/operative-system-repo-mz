#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <wait.h>
#include <fcntl.h>

void sem_op(int semid, int semnum, int sem_operation)
{
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = sem_operation;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

int main(int argc, char *argv[])
{
    int n = 2;

    key_t key = ftok(".", 'A');
    int semid = semget(key, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 0);

    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Prima stampa\n");
        sem_op(semid, 0, 1);
        exit(0);
    }

    pid = fork();
    if (pid == 0)
    {
        printf("Seconda stampa\n");
        sem_op(semid, 0, -1);
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}