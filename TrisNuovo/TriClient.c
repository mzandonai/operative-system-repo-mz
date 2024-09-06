#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define KEY_MEM 1234
#define KEY_SEM 5678

struct game
{
    char board[3][3];
    int turn;
    int gameOver;
    int players_connected;
    int timeout;
};

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

// Funzione P (wait)
void sem_wait(int semid)
{
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = -1; // Decrementa
    sem_op.sem_flg = 0;
    semop(semid, &sem_op, 1);
}

// Funzione V (signal)
void sem_signal(int semid)
{
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = 1; // Incrementa
    sem_op.sem_flg = 0;
    semop(semid, &sem_op, 1);
}

int main(int argc, char *argv[])
{
    int shmid, semid;
    struct game *g;
    int player_num;
    char player_symbol;

    // Collegamento alla memoria condivisa
    shmid = shmget(KEY_MEM, sizeof(struct game), 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    g = (struct game *)shmat(shmid, NULL, 0);
    if (g == (struct game *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Collegamento ai semafori
    semid = semget(KEY_SEM, 1, 0666);
    if (semid < 0)
    {
        perror("semget");
        exit(1);
    }

    // Incrementare il numero di giocatori connessi
    g->players_connected++;

    // Determinare se il giocatore è 1 o 2 (basato sul turno)
    if (g->players_connected == 1)
    {
        player_num = 1;
        player_symbol = 'O'; // Può essere cambiato dal server
    }
    else
    {
        player_num = 2;
        player_symbol = 'X'; // Può essere cambiato dal server
    }

    printf("Player %d: Your symbol is %c\n", player_num, player_symbol);

    while (!g->gameOver)
    {
        // Attendere il proprio turno usando i semafori
        sem_wait(semid);

        // Verifica se il gioco è finito
        if (g->gameOver)
        {
            sem_signal(semid);
            break;
        }

        // Controllare se è il turno del giocatore corrente
        if (g->turn == player_num - 1)
        {
            print_board(g);

            // Richiedere la mossa al giocatore
            int row, col;
            printf("Enter your move (row col): ");
            scanf("%d %d", &row, &col);

            // Controllo sulla validità della mossa
            if (row < 0 || row > 2 || col < 0 || col > 2 || g->board[row][col] != '-')
            {
                printf("Invalid move. Try again.\n");
            }
            else
            {
                // Eseguire la mossa
                g->board[row][col] = player_symbol;

                // Passare il turno all'altro giocatore
                g->turn = (g->turn + 1) % 2;
            }
        }
        else
        {
            printf("Not your turn, waiting...\n");
        }

        // Segnalare che il turno è finito
        sem_signal(semid);

        // Verifica se il gioco è terminato
        if (g->gameOver)
        {
            break;
        }
    }

    printf("Game over! Closing the client.\n");

    // Scollegarsi dalla memoria condivisa
    shmdt(g);

    return 0;
}
