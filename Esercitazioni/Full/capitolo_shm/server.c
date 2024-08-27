#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <string.h>

struct Request
{
    char path[250]; // pathname
    key_t shmkey;
};

int main(int argc, char *argv[])
{
    key_t key1 = ftok(".", 65);
    key_t key2 = ftok(".", 66);
    struct Request req;

    int sh1 = shmget(key1, sizeof(req), IPC_CREAT | 0666);
    int sh2 = shmget(key2, 100 * sizeof(char), 0666);

    char *shm1 = (char *)shmat(sh1, (void *), 0, 0);
    char *shm2 = (char *)shmat(sh2, (void *), 0, 0);

    int fd = open(req.path, O_RDWR | O_EXCL, S_IRWXU);
    if (fd == -1)
    {
        perror("open error");
        shm2[] = -1;
    }

    int size = 100;
    char buffer[size];
    ssize_t reader = read(fd, buffer, 100);
    if (reader == -1)
    {
        perror("reading error");
        return 1;
    }
    buffer[reader] = '\0';
    close(fd);

    strncpy(shm2, buffer, 100 * sizeof(char) - 1);
    shm2[100 * sizeof(char) - 1] = '\0';

    printf("Server: data writing done\n");

    shmdt(shm1);
    shmctl(sh1, IPC_RMID, NULL);
    printf("Server: memory deleted\n");

    return 0;
}