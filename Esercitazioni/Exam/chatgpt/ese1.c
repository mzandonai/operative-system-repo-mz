#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <wait.h>

int main(int argc, char *argv[])
{
    // PIPE
    int pipefd[2];
    int pipeid = pipe(pipefd);

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        return 1;
    }
    else if (pid == 0)
    {
        char file[100];
        ssize_t first_reader = read(STDIN_FILENO, file, sizeof(file));
        if (first_reader < 0)
        {
            perror("reading error");
            return 1;
        }
        else if (first_reader > 0)
        {
            // Rimuove il carattere di newline '\n' dalla password
            if (file[first_reader - 1] == '\n')
            {
                file[first_reader - 1] = '\0';
            }
            else
            {
                file[first_reader] = '\0'; // Aggiunge terminatore di stringa
            }
        }

        int fd1 = open(file, O_CREAT | O_TRUNC | O_EXCL | O_RDWR, S_IRWXU);
        if (fd1 == -1)
        {
            perror("open error");
            return 1;
        }

        char text_to_write[] = {"Questo è il testo per f2"};
        ssize_t writer = write(fd1, text_to_write, strlen(text_to_write));
        if (writer == -1)
        {
            perror("writing error");
            close(fd1);
            return 1;
        }

        lseek(fd1, 0, SEEK_SET);

        char text_to_send[100];
        ssize_t content_reader = read(fd1, text_to_send, sizeof(text_to_send));
        if (content_reader < 0)
        {
            perror("reading error");
            close(fd1);
            return 1;
        }
        else if (content_reader > 0)
        {
            // Rimuove il carattere di newline '\n' dalla password
            if (text_to_send[content_reader - 1] == '\n')
            {
                text_to_send[content_reader - 1] = '\0';
            }
            else
            {
                text_to_send[content_reader] = '\0'; // Aggiunge terminatore di stringa
            }
        }

        close(fd1);

        ssize_t pipe_writer = write(pipefd[1], text_to_send, strlen(text_to_send));
        if (pipe_writer == -1)
        {
            perror("pipe writing error");
            close(pipefd[1]);
            return 1;
        }

        close(pipefd[1]);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    pid_t pid2 = fork();
    if (pid2 < 0)
    {
        perror("fork error");
        return 1;
    }
    else if (pid2 == 0)
    {
        close(pipefd[1]);

        char content[100];
        ssize_t cont_reader = read(pipefd[0], content, sizeof(content));
        if (cont_reader < 0)
        {
            perror("reading error");
            return 1;
        }
        else if (cont_reader > 0)
        {
            // Rimuove il carattere di newline '\n' dalla password
            if (content[cont_reader - 1] == '\n')
            {
                content[cont_reader - 1] = '\0';
            }
            else
            {
                content[cont_reader] = '\0'; // Aggiunge terminatore di stringa
            }
        }

        close(pipefd[0]);

        char file2[100];
        ssize_t reader = read(STDIN_FILENO, file2, sizeof(file2));
        if (reader < 0)
        {
            perror("reading error");
            return 1;
        }
        else if (reader > 0)
        {
            // Rimuove il carattere di newline '\n' dalla password
            if (file2[reader - 1] == '\n')
            {
                file2[reader - 1] = '\0';
            }
            else
            {
                file2[reader] = '\0'; // Aggiunge terminatore di stringa
            }
        }

        int fd2 = open(file2, O_CREAT | O_TRUNC | O_EXCL | O_RDWR, S_IRWXU);
        if (fd2 == -1)
        {
            perror("open error");
            return 1;
        }

        ssize_t final_writer = write(fd2, content, strlen(content));
        if (final_writer == -1)
        {
            perror("final write error");
            close(fd2);
            return 1;
        }

        close(fd2);
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    return 0;
}