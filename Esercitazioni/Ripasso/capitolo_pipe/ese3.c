#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    int pipefd_2[2];
    int pipeid = pipe(pipefd);
    int pipeid_2 = pipe(pipefd_2);

    if (pipeid == -1)
    {
        perror("pipeid error");
        return 1;
    }

    if (pipeid_2 == -1)
    {
        perror("pipeid error");
        return 1;
    }

    char message[] = "This is the message";
    char message_reader[50];

    ssize_t writer = write(pipefd[1], message, strlen(message));
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
        char child_response[] = "and this is the child message";

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

        ssize_t child_writing = write(pipefd_2[1], child_response, strlen(child_response) + 1);
        if (child_writing == -1)
        {
            perror("writing error");
            return 1;
        }
    }
    else
    {
        wait(NULL);

        char dad_buffer[50];
        ssize_t dad_reader = read(pipefd_2[0], dad_buffer, sizeof(dad_buffer));
        if (dad_reader == -1)
        {
            perror("read error");
            return 1;
        }
        else
        {
            printf("Child message: %s\n", dad_buffer);
        }
    }

    return 0;
}