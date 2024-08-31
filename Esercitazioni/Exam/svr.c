#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <sys/shm.h>
#include <wait.h>
#include <unistd.h>

#define SIZE 10

int main(int argc, char *argv[])
{
    // 3 programmi
    // SVR --> init shm condivisa in grado di cont 3 array interi 10 elementi
    // genera 1 figlio, che esegue il programma CLI1 e termina il figlio.
    // padre terminato f1, genera f..f10, tutti i figli eseguono CLI2
    // passando un parametro da 0-9 che indica la posizione ove CLI2 opera
    // CLI1 -->
    // CLI2
    key_t key = ftok("svr.c", 65);

    int shmid_1 = shmget(key, 10 * sizeof(int), 0666 | IPC_CREAT);
    int shmid_2 = shmget(key + 1, 10 * sizeof(int), 0666 | IPC_CREAT);
    int shmid_3 = shmget(key + 2, 10 * sizeof(int), 0666 | IPC_CREAT);

    int *A = (int *)shmat(shmid_1, NULL, 0);
    int *B = (int *)shmat(shmid_2, NULL, 0);
    int *C = (int *)shmat(shmid_3, NULL, 0);

    // child
    pid_t f1 = fork();
    if (f1 < 0)
    {
        perror("fork error");
        return 1;
    }
    else if (f1 == 0)
    {
        execl("./cli1", "./cli1", NULL);
    }
    else
    {
        wait(NULL);

        // 10 child generation
        for (int i = 0; i < 10; i++)
        {
            printf("i: %d", i);
            printf("\n");

            pid_t pid = fork();
            if (pid < 0)
            {
                perror("fork error child");
                return 1;
            }
            else if (pid == 0)
            {
                char arg[5];
                sprintf(arg, "%d", i);
                execl("./cli2", "./cli2", arg, NULL);
            }
        }

        for (int i = 0; i < 10; i++)
        {
            wait(NULL);
        }

        float somma = 0.0;
        float media = 0.0;

        for (int i = 0; i < 10; i++)
        {
            somma += C[i];
        }

        media = somma / 10;
        printf("Media: %f", media);

        shmdt(A);
        shmdt(B);
        shmdt(C);

        shmctl(shmid_1, IPC_RMID, NULL);
        shmctl(shmid_2, IPC_RMID, NULL);
        shmctl(shmid_3, IPC_RMID, NULL);
    }

    return 0;
}