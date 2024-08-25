#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        perror("too few arguments");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork error");
        return 1;
    }
    else if (pid == 0)
    {
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        int fd = open("test.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
        if (fd == -1)
        {
            perror("open error");
            return 1;
        }

        dup(fd);
        fflush(stdout);

        if (execvp(argv[1], &argv[1]) == -1)
        {
            perror("exec error");
            return 1;
        }
    }
    else
    {
        int status;
        if (wait(&status) == -1)
        {
            perror("wait err");
            return 1;
        }

        printf("Command %s exited with status %d\n", argv[1], WEXITSTATUS(status));
    }

    return 0;
}
