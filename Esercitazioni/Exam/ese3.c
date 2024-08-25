#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <wait.h>

void handle_signal(int sig)
{
    printf("CTRL not permissed\n");
}

int main(int argc, char *argv[])
{
    int pipefd[2];
    signal(SIGINT, handle_signal);

    if (pipe(pipefd) == -1)
    {
        perror("pipe error");
        return 1;
    }

    int try = 0;
    int max_try = 3;
    int timeout = 10;
    char password[20];
    char read_it[20];

    while (try < max_try)
    {
        try++;
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork error");
            return 1;
        }
        else if (pid == 0)
        {
            alarm(timeout);
            printf("Insert password: ");
            scanf("%s", password);

            write(pipefd[1], password, strlen(password) + 1);
            close(pipefd[1]);
            exit(0);
        }
        else
        {
            char password[] = "prova";
            close(pipefd[1]);
            wait(NULL);
            read(pipefd[0], read_it, sizeof(read_it));

            if (strcmp(password, read_it) == 0)
            {
                printf("PWD ESATTA\n");
                break;
            }
            else
            {
                printf("PWD ERRATA\n");
                if (try >= max_try)
                {
                    printf("Max try reached\n");
                }
            }
        }
    }

    return 0;
}