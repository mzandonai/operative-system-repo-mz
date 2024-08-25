#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH_MAX 4096

int main(int argc, char *argv[])
{
    int n;
    pid_t pid;

    do
    {
        printf("Inserisci un valore positivo: ");
        scanf("%d", &n);
    } while (n <= 0);

    for (int i = 0; i < n; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork error");
            return 1;
        }
        if (pid == 0)
        {
            printf("(child) PID: %d, PPID: %d\n", getpid(), getppid());
            int exit_code = rand() % 256;
            exit(exit_code);
        }
        else
        {
            printf("(parent) PID: %d, PPID: %d\n", getpid(), getppid());
        }
    }
    return 0;
}