#include <stdio.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <wait.h>

struct Request
{
    char[250];
    key_t shmkey;
};

int main(int argc, char *argv[])
{

    key_t key = ftok(".", 'A');

    int sh1 = shmget(key, sizeof(struct Request), IPC_CREAT | 0666);
    if (sh1 == -1)
    {
        perror("sh1 error");
        return 1;
    }
    
    int fd = open("text.txt", O_CREAT | O_RDWR, 0666);

    char buf[100];
    ssize_t reader = read(fd, buf, sizeof(buf));

    int *ptr = (int *)shmat(sh1, NULL, 0);

    ssize_t writer = write(ptr, buf, sizeof(buf));

    pid_t pid = fork();
    if (pid == 0)
    {
        key_t key = ftok(".", 'B');
        int sh2 = shmget(key, 100, IPC_CREAT | 0666);
    }
}