#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY 1234 // Chiave per la coda di messaggi

struct order
{
    long mtype;
    unsigned int code;
    char description[50]; // Ridotto a 50 caratteri
    unsigned int quantity;
    char email[50]; // Ridotto a 50 caratteri
};

int main()
{
    int msgid = msgget(QUEUE_KEY, 0666);
    if (msgid == -1)
    {
        perror("Errore nel collegamento alla coda di messaggi");
        exit(1);
    }

    struct order new_order;

    int prime = 2;
    int normale = 1;

    printf("Inserisci il tipo di cliente: ");
    scanf("%ld", &new_order.mtype);
    getchar();

    // Raccoglie i dati dall'utente
    printf("Inserisci il codice dell'ordine: ");
    scanf("%u", &new_order.code);
    getchar(); // Per consumare il newline rimasto dal precedente input

    printf("Inserisci la descrizione (max 50 caratteri): ");
    fgets(new_order.description, sizeof(new_order.description), stdin);
    new_order.description[strcspn(new_order.description, "\n")] = '\0'; // Rimuove newline

    printf("Inserisci la quantità: ");
    scanf("%u", &new_order.quantity);
    getchar(); // Per consumare il newline rimasto dal precedente input

    printf("Inserisci l'email (max 50 caratteri): ");
    fgets(new_order.email, sizeof(new_order.email), stdin);
    new_order.email[strcspn(new_order.email, "\n")] = '\0'; // Rimuove newline

    // Invia il messaggio al server
    if (msgsnd(msgid, &new_order, sizeof(new_order), 0) == -1)
    {
        perror("Errore nell'invio del messaggio");
        exit(1);
    }

    printf("Ordine inviato al server.\n");

    return 0;
}
