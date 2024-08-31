#include <stdio.h>
#include <sys/ipc.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define MAX 3

void sig_alarm_handler(int sig)
{
    char message[] = {"Timer scaduto!\n"};

    if (sig == SIGALRM)
    {
        write(STDOUT_FILENO, message, strlen(message));
        exit(1);
    }
}

void sig_alarm_ctrl(int sig)
{
    char message[] = {"Non puoi fermarmi in questo modo!\n"};

    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, message, strlen(message));
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int pipefd[2];
    int pipeid = pipe(pipefd);

    char password[50];
    int timer = 10;

    int try = 0;
    int done = 0;

    while (try < MAX && done != 1)
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
            // Processo figlio
            signal(SIGINT, sig_alarm_ctrl);
            signal(SIGALRM, sig_alarm_handler);

            char richiesta[] = {"Inserisci password: "};
            write(STDOUT_FILENO, richiesta, sizeof(richiesta) - 1); // Correzione: sizeof(richiesta) - 1

            alarm(timer);

            ssize_t reader = read(STDIN_FILENO, password, sizeof(password) - 1); // Correzione: sizeof(password) - 1
            if (reader > 0)
            {
                // Rimuove il carattere di newline '\n' dalla password
                if (password[reader - 1] == '\n')
                {
                    password[reader - 1] = '\0';
                }
                else
                {
                    password[reader] = '\0'; // Aggiunge terminatore di stringa
                }
            }

            if (reader < 0)
            {
                perror("read error");
                return 1;
            }
            else
            {
                ssize_t writer = write(pipefd[1], password, strlen(password) + 1); // Scrive la password compreso il terminatore di stringa
                alarm(0);
                exit(0);
            }
        }
        else
        {
            // Processo padre
            int status;
            wait(&status);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
            {
                char error[] = "CHILD RETURN: error terminated\n";
                write(STDOUT_FILENO, error, strlen(error));
            }
            else
            {
                char pwd_esatta[] = {"ciao"};
                char buffer[50];

                ssize_t reader = read(pipefd[0], buffer, sizeof(buffer) - 1); // Correzione: sizeof(buffer) - 1
                if (reader > 0)
                {
                    buffer[reader] = '\0'; // Aggiunge terminatore di stringa

                    if (strcmp(pwd_esatta, buffer) == 0)
                    {
                        char corretto[] = {"PWD ESATTA\n"};
                        write(STDOUT_FILENO, corretto, strlen(corretto));
                        done = 1;
                        exit(0);
                    }
                    else
                    {
                        char errato[] = {"PWD ERRATA\n"};
                        write(STDOUT_FILENO, errato, strlen(errato));
                    }
                }
            }
        }
    }

    return 0;
}
