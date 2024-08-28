#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sem.h>
#include <unistd.h>

void semOp(int semid, unsigned short sem_num, short sem_op)
{
    struct sembuf sop =
        {
            .sem_num = sem_num,
            .sem_op = sem_op,
            .sem_flg = 0};

    if (semop(semid, &sop, 1) == -1)
    {
        printf("semop failed");
        exit(0);
    }

    semop()
}

void *thread_function(void *arg)
{
    int semid = *((int *)arg);

    for (int i = 0; i < 5; i++)
    {
        semOp(semid, 0, -1);
        printf("Message from thread %ld\n", pthread_self());
        sleep(1);
        semOp(semid, 0, 1);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    key_t key = ftok(".", 'A');
    int semid = semget(key, 1, IPC_CREAT | 0666);

    semctl(semid, 0, SETVAL, 1);

    pthread_t thread1;
    pthread_t thread2;
    int id1 = 1, id2 = 2;

    int thread_id1 = pthread_create(&thread1, NULL, thread_function, (void *)&semid);
    int thread_id2 = pthread_create(&thread1, NULL, thread_function, (void *)&semid);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    semctl(semid, 0, IPC_RMID);
    return 0;
}