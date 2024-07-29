#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>

// costanti da inizializzare
#define TIMEOUT
#define KEY_SHM
#define KEY_SEM
#define SIZE
#define ROWS 3
#define COLS 3
#define END
#define WIN
#define PID_CLIENT1
#define PID_CLIENT2
// Variabili globali
int sem_id;
int shm_id;
int sem_val;
int *shared_memory;
bool sig_count = false;
char simbolo;
int player;
int timeout;
bool t_scaduto;
bool gioco_finito = false;
bool asterisco = false;
bool sono_CPU = false;
struct sembuf sb = {0, 0, 0};

// Funzioni

void cleanup_client()
{
    if (shmdt(shared_memory) {
        ...
    }
}

void sigHandler(int sig)
{
    // attraverso SIGINT gestisce la doppia pressione del ctrl+C
}

void sigAlarm(int sig)
{
    // gestione del timer per la mossa del giocatore
}

void sigClose(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("\nServer disconnesso.\n");
        shared_memory[END] = 3;
        cleanup_client();
        exit(0);
    }
    if (sig == SIGUSR2)
    {
        printf("\nL'altro giocatore ha abbandonato la partita.\n");
        exit(0);
    }
}

void stampa_matrice(char *matrix)
{
    // stampa matrice di gioco
}

void mossa_valida(char *matrix, char simbolo)
{
    int r, c, k = 0;
    int random, indice;
    int empty[ROWS * COLS];
    srand(time(NULL));

    if (sono_CPU)
    {
        for (int i = 0; i < ROWS * COLS; i++)
        {
            if (matrix[i] == 0)
            {
                empty[k++] = i;
            }
        }
        if (k > 0)
        {
            // gioco random
        }
        else
        {
            while (1)
            {
                printf("Inserisci una mossa, <riga> <colonna>: ");
                alarm(timeout);
                scanf("%d %d", &r, &c);
                // inserimento mossa nella matrice
            }
            printf("Hai inserito: %d %d\n", r, c);
            shared_memory[3] = (shared_memory[3] == 0) ? 1 : 0;
        }
    }

    void controlli_iniziali(int argc, char *argv[])
    {
        // controlla parametri
    }

    int main(int argc, char *argv[])
    {
        controlli_iniziali(argc, argv);

        signal(SIGINT, sigHandler);
        signal(SIGALRM, sigAlarm);
        signal(SIGUSR1, sigClose);
        signal(SIGUSR2, sigClose);

        shm_id = shmget(KEY_SHM, SIZE, 0600);

        shared_memory = (int *)shmat(shm_id, NULL, SHM_RND);

        sem_id = semget(KEY_SEM, 1, 0600);

        sem_val = semctl(sem_id, 0, GETVAL);

        // COLLEGAMENTO AL SEMAFORO
        // NB: semaforo inizializzato lato server a -2
        sem_id = semget(KEY_SEM, 1, 0600);

        sem_val = semctl(sem_id, 0, GETVAL);

        // GIOCO IN COPPIA
        // ogni client che viene eseguito fa un'operazione di incremento sul semaforo semid,
        // quando vengono eseguiti 2 client allora il semaforo torna a 0 e il server continua
        if (asterisco == false && sono_CPU == false)
        {
            ...
                // primo client connesso
                if (sem_val == 1)
            {

                printf("In attesa di un secondo client...\n");
                ... while (sem_val != 0)
                {
                    sem_val = semctl(sem_id, 0, GETVAL);
                }
            }
            else
            {
                shared_memory[PID_CLIENT2] = getpid();
                printf("Sei il secondo giocatore, la partita inizierà a breve.\n");
                simbolo = shared_memory[2];
                player = 1;
            }
            printf("Il tuo simbolo per questa partita è %c\n", simbolo);
        }
        // GIOCO SINGOLO
        else
        {
            // giocatore vero, è il primo a giocare
            if (!sono_CPU)
            {
                simbolo = shared_memory[1];
                player = 0;
                printf("Stai giocando in SINGOLO:");
                printf("Il tuo simbolo per questa partita è %c\n", simbolo);
            }
            // giocatore automatico
            else
            {
                simbolo = shared_memory[2];
                player = 1;
            }
            // sblocco il server incrementando il semaforo di 2
            sb.sem_op = 2;
            semop(sem_id, &sb, 1);
        }

        timeout = shared_memory[4];

        char *matrix = (char *)&shared_memory[6];

        printf("Player: %d\n", player);

        // while controlla dice chi ha vinto o se c'è un pareggio
        while (shared_memory[END] == 0)
        {
            ...
        }
        cleanup_client();
        return 0;
    }