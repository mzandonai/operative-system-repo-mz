#include <sys/sem.h>
#include <errno.h>  // Necessario per errno
#include <string.h> // Necessario per strerror
#include <stdio.h>
#include "semaphore.h"
#include "errExit.h"

void semOp(int semid, unsigned short sem_num, short sem_op)
{
    struct sembuf sop = {
        .sem_num = sem_num, // Indice del semaforo nell'array
        .sem_op = sem_op,   // Operazione da eseguire (P = -1, V = +1)
        .sem_flg = 0        // Flag
    };

    // Esegui l'operazione sul semaforo
    if (semop(semid, &sop, 1) == -1)
    {
        // Stampa un errore più dettagliato
        fprintf(stderr, "semop failed: %s\n", strerror(errno));
        errExit("semop failed");
    }
}
