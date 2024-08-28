#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

struct Request
{
    char filepath[250];
    key_t shmkey;
};

int main()
{
    key_t key1 = ftok("shmfile1", 65);
    key_t key2 = ftok("shmfile2", 66);

    // Creazione del segmento SH1 per la richiesta
    int shmid1 = shmget(key1, sizeof(struct Request), 0666 | IPC_CREAT);
    if (shmid1 == -1)
    {
        perror("shmget failed");
        exit(1);
    }

    struct Request *req = (struct Request *)shmat(shmid1, NULL, 0);
    if (req == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    // Creazione del segmento SH2 per i primi 100 caratteri
    int shmid2 = shmget(key2, 100, 0666 | IPC_CREAT);
    if (shmid2 == -1)
    {
        perror("shmget failed");
        exit(1);
    }

    // Richiesta del file path all'utente
    printf("Client: enter the file path: ");
    fgets(req->filepath, 250, stdin);
    req->filepath[strcspn(req->filepath, "\n")] = '\0'; // Rimuove il newline

    req->shmkey = key2;

    // Segnalazione al server (qui simulata con sleep)
    sleep(1);

    // Attesa del server e lettura del contenuto di SH2
    char *shared_data = (char *)shmat(shmid2, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    sleep(1); // Attesa che il server termini

    if (strcmp(shared_data, "-1") == 0)
    {
        printf("Client: File does not exist or could not be read.\n");
    }
    else
    {
        printf("Client: First 100 characters of the file:\n%s\n", shared_data);
    }

    // Pulizia: rimozione SH2
    if (shmdt(shared_data) == -1)
    {
        perror("shmdt failed");
        exit(1);
    }

    if (shmctl(shmid2, IPC_RMID, NULL) == -1)
    {
        perror("shmctl failed");
        exit(1);
    }

    // Pulizia: rimozione SH1
    if (shmdt(req) == -1)
    {
        perror("shmdt failed");
        exit(1);
    }

    printf("Client: removed SH2 and exiting.\n");

    return 0;
}
