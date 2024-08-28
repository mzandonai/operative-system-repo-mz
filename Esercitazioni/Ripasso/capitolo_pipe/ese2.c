#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    int pipeid = pipe(pipefd);
    if (pipeid == -1)
    {
        perror("pipeid error");
        return 1;
    }

    char message[] = "This is the message";
    char message_reader[50];

    ssize_t writer = write(pipefd[1], message, strlen(message) + 1);
    if (writer == -1)
    {
        perror("writing error");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
    }
    else if (pid == 0)
    {

        ssize_t reader = read(pipefd[0], message_reader, sizeof(message_reader));
        if (reader == -1)
        {
            perror("read error");
            return 1;
        }
        else
        {
            printf("Message: %s\n", message_reader);
        }
    }
    else
    {
        wait(NULL);
    }

    return 0;
}