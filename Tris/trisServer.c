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
#include <stdbool.h>

// costanti da inizializzare
#define TIMEOUT 10
#define KEY_SHM 1234
#define KEY_SEM 5678
#define ROWS 3
#define COLS 3
#define SIZE sizeof(int) * (6 + ROWS * COLS)
#define END 0
#define WIN 1
#define PID_CLIENT1 2
#define PID_CLIENT2 3

// variabili globali
int timeout;
int i, j;
int semid;
int shmid;
int *shared_memory;
bool computer = false;
// numero di semafori creati dopo è 1
//{num del semaforo su cui eseguire l'op, numero da aggiungere, opzioni}
struct sembuf sop = {0, 0, 0};

// funzioni

void cleanup()
{
    // Deallocazione memoria condivisa
    if (shmdt(shared_memory) == -1)
    {
        perror("Errore durante la deallocazione");
    }
    // Rimuove area di memoria condivisa
    //(id memoria, flag per rimozione, opzioni)
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("Errore durante la rimozione");
    }

    // Deallocazione semaforo
    //(id semaforo, numero del semaforo su cui eseguire l'operazione, flag per rimozione)
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("Errore durante la rimozione del semaforo");
    }
}

void gestore_segnale(int segnale)
{
    ...
}

// Chiude i progessi figli quando sono diversi da 0
void sigHandler(int signum)
{
    printf("Ricevuto segnale di interrupt\n");
    cleanup();
    exit(0);
}

void controlli_iniziali(int argc, char *argv[])
{

    // controllo se il secondo argomento è un numero
    // in caso non viene inserito metto un timeout di default

    // controllo correttezza simboli

    // se viene inserito 0 non c'è timeout
    if (argc < 4)
    {
        printf("Uso: %s <timeout> <simbolo1> <simbolo2>\n", argv[0]);
        exit(1);
    }
    timeout = atoi(argv[1]);
    if (timeout <= 0)
    {
        timeout = TIMEOUT; // Imposta il timeout di default
    }
}

// funzione per stabilire se c'è un vincitore
bool vincitore(char *matrix, char player)
{
    // Controllo orizzontale
    for (int i = 0; i < ROWS; i++)
    {
        if (matrix[i * COLS] == player && matrix[i * COLS + 1] == player && matrix[i * COLS + 2] == player)
        {
            return true;
        }
    }
    // Controllo verticale
    for (int i = 0; i < COLS; i++)
    {
        if (matrix[i] == player && matrix[i + COLS] == player && matrix[i + 2 * COLS] == player)
        {
            return true;
        }
    }
    // Controllo diagonale
    if ((matrix[0] == player && matrix[4] == player && matrix[8] == player) ||
        (matrix[2] == player && matrix[4] == player && matrix[6] == player))
    {
        return true;
    }
    return false;
}

bool pareggio(char *matrix)
{
    // Conta il numero di celle occupate

    // Se tutte le celle sono occupate e nessuno ha vinto, c'è pareggio
    for (int i = 0; i < ROWS * COLS; i++)
    {
        if (matrix[i] == 0)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{

    // ripassa segnali
    signal(SIGINT, sigHandler);

    controlli_iniziali(argc, argv);

    // CREAZIONE AREA DI MEMORIA CONDIVISA
    //(chiave shm, dimesione, flag per creazione | permessi)
    shmid = shmget(KEY_SHM, SIZE, IPC_CREAT | 0660);

    // collega la memoria condivisa
    //(id mem, indirizzo scelto dal sistema, flag)
    // SHM_RND indirizzo è arrotondato verso il basso al limite di pagina più vicino
    shared_memory = (int *)shmat(shmid, NULL, SHM_RND);

    // prima di scriverci sopra inizializzo l'area di memoria condivisa a 0
    //(mem da settare, valore a cui viene assegnata la mem, byte che vuoi impostare a 0 nella shared_memory

    // inizializzazione delle prime celle della shmem
    shared_memory[0] = getpid();
    shared_memory[1] = *argv[i]; // simbolo1
    shared_memory[2] = *argv[j]; // simbolo2
    shared_memory[3] = 0;        // turno
    shared_memory[4] = timeout;  // tempo
    shared_memory[5] = 0;        // start gioco

    // CREAZIONE SEMAFORO PER ATTENDERE 2 GIOCATORI
    semid = semget(KEY_SEM, 1, IPC_CREAT | 0600);

    // si imposta il valore iniziale a -2 in modo che il server aspetta 2 incrementi prima
    // di procedere con la sua esecuzione

    // Aspetta che il valore del semaforo raggiunga 0 a partire da -2
    //(cioè due client hanno fatto l'operazione di incremento)

    if (shared_memory[END] == 1)
    {
        printf("I client hanno abbandonato.\n");
        cleanup();
        exit(0);
    }
    else
        printf("Entrambi i client connessi.\n");

    // START GIOCO
    shared_memory[5] = 1;

    char *matrix = (char *)&shared_memory[6];

    // il server attende fin quando la partita non si conclude con una vittoria o con un pareggio
    while (1)
    {
        // Attendi che il semaforo venga incrementato dal client
        sb.sem_op = -1;
        semop(semid, &sb, 1);

        if (shared_memory[END] != 0)
        {
            break; // Se il gioco è terminato
        }

        if (vincitore(matrix, shared_memory[1]))
        {
            printf("Giocatore 1 ha vinto!\n");
            shared_memory[END] = WIN;
            break;
        }
        else if (vincitore(matrix, shared_memory[2]))
        {
            printf("Giocatore 2 ha vinto!\n");
            shared_memory[END] = WIN;
            break;
        }
        else if (pareggio(matrix))
        {
            printf("Pareggio!\n");
            shared_memory[END] = 1;
            break;
        }

        // Alternare il turno tra i giocatori
        shared_memory[3] = 1 - shared_memory[3];
    }

    cleanup();
    return 0;
}
