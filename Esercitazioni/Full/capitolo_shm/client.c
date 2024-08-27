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

    int sh1 = shmget(key1, sizeof(req), | 0666);
    int sh2 = shmget(key2, 100, IPC_CREAT | 0666);

    

    if (sh2 == -1)
    {
        perror("sh2 error");
        return 1;
    }

    printf("Inserisci un pathname: ");
    scanf("%s", req.path);
    printf("\n");

    printf("Path inserito: %s\n", req.path);

    // deposito
    

    shmdt()

    return 0;
}