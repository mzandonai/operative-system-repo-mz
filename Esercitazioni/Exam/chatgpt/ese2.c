#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <wait.h>
#include <pthread.h>

typedef struct
{
    char pathname[250];
    int id;
    off_t last_size;
} monitoring;

void *routine_1(void *arg)
{
    monitoring *monitor = (monitoring *)arg;
    struct stat file_stat;

    while (1)
    {
        if (stat(monitor->pathname, &file_stat) == 0)
        {
            if (file_stat.st_size != monitor->last_size)
            {
                printf("Nuova dim\n");
                monitor->last_size = file_stat.st_size;
            }
        }
        else
        {
            perror("stat error");
        }
        sleep(5);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    monitoring monitor[3] = {
        {"file1.txt", 1, 0},
        {"file2.txt", 2, 0},
        {"file3.txt", 3, 0},
    };

    pthread_t main;
    pthread_t second;
    pthread_t third;

    pthread_create(&main, NULL, routine_1, (void *)&monitor[0]);
    pthread_create(&second, NULL, routine_1, (void *)&monitor[1]);
    pthread_create(&third, NULL, routine_1, (void *)&monitor[2]);

    pthread_join(main, NULL);
    pthread_join(second, NULL);
    pthread_join(third, NULL);

    return 0;
}