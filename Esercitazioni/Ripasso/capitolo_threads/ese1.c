#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void *thread_function(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        printf("Message from thread %d\n", *((int *)arg));
        sleep(5);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread1;
    pthread_t thread2;
    int id1 = 1, id2 = 2;

    int thread_id1 = pthread_create(&thread1, NULL, thread_function, (void *)&id1);
    int thread_id2 = pthread_create(&thread1, NULL, thread_function, (void *)&id2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}