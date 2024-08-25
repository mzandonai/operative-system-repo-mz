#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Too few arguments");
        exit(EXIT_FAILURE);
    }

    int number = atoi(argv[1]);
    if (number <= 0)
    {
        perror("Number should be positive");
        exit(EXIT_FAILURE);
    }

    pid_t lastpid;

    for (int i = 0; i < number; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            // Errore nella fork
            perror("Errore nella fork");
            exit(1);
        }
        else if (pid == 0)
        {
            // Codice eseguito dal processo figlio
            srand(time(NULL) ^ (getpid() << 16)); // Re-inizializza
            int exit_status = rand() % 256;       // Genera un numero casuale tra 0 e 255
            printf("Figlio %d: PID = %d, PID del padre = %d, valore di uscita = %d\n",
                   i + 1, getpid(), getppid(), exit_status);
            exit(exit_status); // Termina il figlio con il valore casuale
        }
        else
        {
            lastpid = pid;
        }
    }

    int status;
    if (waitpid(lastpid, &status, 0) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Ultimo figlio con %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}