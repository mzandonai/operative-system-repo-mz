#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define CHUNK_SIZE 100 // Dimensione massima di un chunk

void producer(int pipe_fd[2], const char *file_path)
{
    close(pipe_fd[0]); // Chiude il lato di lettura della PIPE

    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        perror("Errore nell'apertura del file");
        exit(1);
    }

    char buffer[CHUNK_SIZE];
    ssize_t bytes_read;

    // Legge il file in chunk e scrive ogni chunk nella PIPE
    while ((bytes_read = read(file_fd, buffer, CHUNK_SIZE)) > 0)
    {
        if (write(pipe_fd[1], buffer, bytes_read) == -1)
        {
            perror("Errore nella scrittura sulla PIPE");
            close(file_fd);
            exit(1);
        }
    }

    close(file_fd);    // Chiude il file
    close(pipe_fd[1]); // Chiude il lato di scrittura della PIPE
    exit(0);
}

void consumer(int pipe_fd[2])
{
    close(pipe_fd[1]); // Chiude il lato di scrittura della PIPE

    char buffer[CHUNK_SIZE];
    ssize_t bytes_read;

    // Legge dalla PIPE e stampa il contenuto ricevuto
    while ((bytes_read = read(pipe_fd[0], buffer, CHUNK_SIZE)) > 0)
    {
        if (write(STDOUT_FILENO, buffer, bytes_read) == -1)
        {
            perror("Errore nella scrittura su stdout");
            exit(1);
        }
    }

    close(pipe_fd[0]); // Chiude il lato di lettura della PIPE
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Utilizzo: %s <pathname del file>\n", argv[0]);
        exit(1);
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("Errore nella creazione della PIPE");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Errore nella fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // Processo figlio: Produttore
        producer(pipe_fd, argv[1]);
    }
    else
    {
        // Processo padre: Consumatore
        consumer(pipe_fd);

        // Attende la terminazione del processo figlio
        wait(NULL);
    }

    return 0;
}
