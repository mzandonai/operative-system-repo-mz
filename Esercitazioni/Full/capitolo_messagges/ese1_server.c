#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>

#define QUEUE_KEY 1234 // Chiave per la coda di messaggi

struct order
{
    long mtype;
    unsigned int code;
    char description[50]; // Ridotto a 50 caratteri
    unsigned int quantity;
    char email[50]; // Ridotto a 50 caratteri
};

int msgid; // ID della coda di messaggi

void cleanup(int signum)
{
    // Rimuovi la coda di messaggi
    msgctl(msgid, IPC_RMID, NULL);
    printf("Coda di messaggi rimossa. Server terminato.\n");
    exit(0);
}

int main()
{
    // Crea la coda di messaggi
    msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("Errore nella creazione della coda di messaggi");
        exit(1);
    }

    // Imposta il gestore del segnale per SIGTERM
    signal(SIGTERM, cleanup);

    printf("Server in attesa di messaggi...\n");

    struct order received_order;

    // Prima fase: Ricevi e stampa tutti i messaggi di tipo 2
    while (1)
    {
        if (msgrcv(msgid, &received_order, sizeof(received_order), 2, IPC_NOWAIT) != -1)
        {
            // Stampa i dettagli dell'ordine ricevuto di tipo 2
            printf("Ordine di tipo 2 ricevuto:\n");
            printf("Codice: %u\n", received_order.code);
            printf("Descrizione: %s\n", received_order.description);
            printf("Quantità: %u\n", received_order.quantity);
            printf("Email: %s\n", received_order.email);
        }
        else
        {
            break; // Esci dal ciclo se non ci sono più messaggi di tipo 2
        }
    }

    // Seconda fase: Ricevi e stampa tutti i messaggi di tipo 1
    while (1)
    {
        if (msgrcv(msgid, &received_order, sizeof(received_order), 1, 0) != -1)
        {
            // Stampa i dettagli dell'ordine ricevuto di tipo 1
            printf("Ordine di tipo 1 ricevuto:\n");
            printf("Codice: %u\n", received_order.code);
            printf("Descrizione: %s\n", received_order.description);
            printf("Quantità: %u\n", received_order.quantity);
            printf("Email: %s\n", received_order.email);
        }
    }

    return 0;
}
