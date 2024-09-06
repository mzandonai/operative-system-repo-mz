#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define KEY_MEM 1234 // Chiave per la memoria condivisa
#define KEY_SEM 5678 // Chiave per i semafori
#define SIZE 9       // Dimensione della matrice 3x3

// Struttura per la memoria condivisa
struct game
{
    char board[3][3];      // Matrice di gioco
    int turn;              // Indica a chi tocca (0 -> Giocatore 1, 1 -> Giocatore 2)
    int gameOver;          // Indica se il gioco è finito
    int players_connected; // Numero di giocatori collegati
    int timeout;           // Timeout per la mossa
};

int shmid, semid; // Variabili globali per memoria condivisa e semafori
struct game *g;   // Puntatore alla memoria condivisa

// Funzione per pulire semafori e memoria condivisa
void cleanup()
{
    if (shmid >= 0)
    {
        // Rimuovi memoria condivisa
        shmctl(shmid, IPC_RMID, NULL);
    }
    if (semid >= 0)
    {
        // Rimuovi semaforo
        semctl(semid, 0, IPC_RMID);
    }
}

// Funzione che viene chiamata alla terminazione del programma
void terminate_handler(int signum)
{
    printf("Terminazione del server...\n");
    g->gameOver = 1;
    cleanup(); // Pulizia di memoria condivisa e semafori
    exit(0);   // Termina il programma
}

// Funzione per stampare la matrice
void print_board(struct game *g)
{
    printf("Current Board:\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%c ", g->board[i][j]);
        }
        printf("\n");
    }
}

// Funzione per controllare se un giocatore ha vinto
int check_win(struct game *g)
{
    // Controllo righe
    for (int i = 0; i < 3; i++)
    {
        if (g->board[i][0] == g->board[i][1] && g->board[i][1] == g->board[i][2] && g->board[i][0] != '-')
        {
            return 1;
        }
    }

    // Controllo colonne
    for (int i = 0; i < 3; i++)
    {
        if (g->board[0][i] == g->board[1][i] && g->board[1][i] == g->board[2][i] && g->board[0][i] != '-')
        {
            return 1;
        }
    }

    // Controllo diagonali
    if (g->board[0][0] == g->board[1][1] && g->board[1][1] == g->board[2][2] && g->board[0][0] != '-')
    {
        return 1;
    }

    if (g->board[0][2] == g->board[1][1] && g->board[1][1] == g->board[2][0] && g->board[0][2] != '-')
    {
        return 1;
    }

    return 0;
}

// Funzione per controllare se la matrice è piena
int check_draw(struct game *g)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (g->board[i][j] == '-')
            {
                return 0; // C'è ancora spazio
            }
        }
    }
    return 1; // La matrice è piena
}

// Signal handler per il timeout
void timeout_handler(int signum)
{
    printf("Timeout! Il giocatore ha impiegato troppo tempo. Il gioco termina.\n");
    g->gameOver = 1; // Imposta che il gioco è finito
    cleanup();       // Pulizia di memoria condivisa e semafori
    exit(0);         // Termina il server
}

int main(int argc, char *argv[])
{
    // Collegare signal handler per la terminazione pulita
    signal(SIGINT, terminate_handler);  // Ctrl+C termina il server
    signal(SIGTERM, terminate_handler); // Signal per terminazione del server

    // Pulisci eventuali risorse IPC rimaste da esecuzioni precedenti
    cleanup();

    // Creazione della memoria condivisa
    shmid = shmget(KEY_MEM, sizeof(struct game), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    // Collegamento alla memoria condivisa
    g = (struct game *)shmat(shmid, NULL, 0);
    if (g == (struct game *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Inizializzazione della matrice di gioco
    memset(g->board, '-', sizeof(g->board));
    g->turn = 0;
    g->gameOver = 0;
    g->players_connected = 0; // Nessun giocatore inizialmente

    // Timeout per le mosse (definito dall'utente o impostato di default a 10 secondi)
    g->timeout = (argc > 1) ? atoi(argv[1]) : 10;

    // Creazione dei semafori
    semid = semget(KEY_SEM, 1, IPC_CREAT | 0666);
    if (semid < 0)
    {
        perror("semget");
        cleanup();
        exit(1);
    }

    // Inizializzare il semaforo a 1 (per il primo giocatore)
    semctl(semid, 0, SETVAL, 1);

    printf("Server inizializzato. In attesa dei giocatori...\n");

    // Attendere che entrambi i giocatori si connettano
    while (g->players_connected < 2)
    {
        sleep(1);
    }

    printf("Entrambi i giocatori sono connessi. Inizio del gioco!\n");

    // Impostare l'handler per il timeout
    signal(SIGALRM, timeout_handler);

    // Ciclo principale del server
    while (!g->gameOver)
    {
        // Impostare il timeout per il turno del giocatore
        alarm(g->timeout);

        // Verificare se qualcuno ha vinto o se la partita è finita in pareggio
        if (check_win(g))
        {
            printf("Player %d wins!\n", g->turn == 0 ? 2 : 1);
            g->gameOver = 1; // Segnala che il gioco è finito
        }
        else if (check_draw(g))
        {
            printf("It's a draw!\n");
            g->gameOver = 1; // Segnala che il gioco è finito
        }

        // Se il gioco non è finito, passa il turno all'altro giocatore
        if (!g->gameOver)
        {
            g->turn = (g->turn + 1) % 2;
        }
    }

    printf("Game over! Il server sta chiudendo.\n");

    // Pulizia di memoria condivisa e semafori
    cleanup();

    return 0;
}
