#include <fcntl.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void sem_op(int semid, int semnum, int sem_operation)
{
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = sem_operation;
    op.sem_flg = 0;

    if (semop(semid, &op, 1) == -1)
    {
        perror("semop");
        exit(1);
    }
}

int main()
{
    char *words[4] = {"Operativi\n", "Sistemi", "di", "Corso"};

    key_t key = ftok(".", 'A');
    if (key == -1)
    {
        perror("ftok error");
        return 1;
    }

    int semid = semget(key, 4, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget error");
        return 1;
    }

    semctl(semid, 0, SETVAL, 1); // Sblocca inizialmente il semaforo 0
    semctl(semid, 1, SETVAL, 0);
    semctl(semid, 2, SETVAL, 0);
    semctl(semid, 3, SETVAL, 0);

    for (int i = 0; i < 4; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork error");
            return 1;
        }
        else if (pid == 0)
        {
            // Ogni figlio attende il proprio semaforo, stampa, e poi sblocca il semaforo successivo
            sem_op(semid, i, -1);
            printf("%s", words[i]);
            fflush(stdout);

            if (i < 3)
            {
                sem_op(semid, i + 1, 1); // Sblocca il semaforo successivo
            }
            exit(0);
        }
    }

    // Il processo padre attende la terminazione di tutti i figli
    for (int i = 0; i < 4; i++)
    {
        wait(NULL);
    }

    // Rimozione del set di semafori
    semctl(semid, 0, IPC_RMID);

    return 0;
}
