#include <sys/ipc.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int pipe_fd[2];

void sigHandler(int sig)
{
    printf("Time is up\n");
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    exit(1);
}

int main(int argc, char *argv[])
{
    printf("This the ESE3...\n");

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    printf("Genero un figlio: \n");

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Fork error");
        return 1;
    }
    else if (pid == 0)
    {
        printf("This is the child\n");
        if (signal(SIGALRM, sigHandler) == SIG_ERR)
        {
            perror("Signal error");
            return 1;
        }

        int timer = 10;
        alarm(timer);

        char password[4];
        printf("Inserisci una password: ");
        scanf("%c", password);
    }

    return 0;
}