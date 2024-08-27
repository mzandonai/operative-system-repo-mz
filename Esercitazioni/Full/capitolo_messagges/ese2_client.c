#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>

struct message
{
    long mtype;
    char sms[100];
};

int main(int argc, char *argv[])
{
    key_t key = ftok(".", 65);

    int msgid = msgget(key, 0666);

    struct message msg1;
    struct message msg2;
    struct message msg3;

    msg1.mtype = 1;
    msg2.mtype = 2;
    msg3.mtype = 1;

    printf("Insert a message 1: ");
    strcpy(msg1.sms, "Questo è un messaggio di tipo 1.");
    printf("\n");
    printf("Insert a message 2: ");
    strcpy(msg2.sms, "Questo è un messaggio di tipo 2.");
    printf("\n");
    printf("Insert a message 3: ");
    strcpy(msg3.sms, "Questo è un messaggio di tipo 1.");
    printf("\n");

    if (msgsnd(msgid, &msg1, sizeof(msg1), 0) == -1)
    {
        perror("msgsnd error");
        return 1;
    }

    if (msgsnd(msgid, &msg2, sizeof(msg2), 0) == -1)
    {
        perror("msgsnd error");
        return 1;
    }

    if (msgsnd(msgid, &msg3, sizeof(msg3), 0) == -1)
    {
        perror("msgsnd error");
        return 1;
    }

    return 0;
}