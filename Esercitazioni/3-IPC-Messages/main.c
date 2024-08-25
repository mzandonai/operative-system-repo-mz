#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>

int main(int argc, char *argv[])
{
    struct message
    {
        long mtype;
        char mtext[100];
    } m;

    m.mtype = 1;
    char *text = "Ciao amico";
    memcpy(m.mtext, text, strlen(text) + 1);
    size_t mSize = sizeof(struct message) - sizeof(long);

    key_t key = ftok(".", 'A');

    // Creo una coda da una chiave
    int msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        printf("Errore nella creazione\n");
        return 0;
    }
    else
    {
        printf("Coda di messaggi creata\n");
    }

    // Scrivo un messaggio
    if (msgsnd(msgid, &m, mSize, 0) == -1)
    {
        printf("Msgsnd() error\n");
        return 0;
    }
    else
    {
        printf("Msgsnd done\n");
    }

    printf("Lettura in corso...\n");
    sleep(3);
    if (msgrcv(msgid, &m, mSize, 1, 0) == -1)
    {
        perror("Reading error");
        return 0;
    }

    printf("Il messaggio letto: %s\n", m.mtext);

    printf("\n");
    printf("Rimuovo la coda...\n");

    struct msqid_ds ds;
    if (msgctl(msgid, IPC_RMID, &ds) == -1)
    {
        perror("Errore nella rimozione\n");
        return 0;
    }
    else
    {
        printf("---DONE---\n");
    }

    return 0;
}