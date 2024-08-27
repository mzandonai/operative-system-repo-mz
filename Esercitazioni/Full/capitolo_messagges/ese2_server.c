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

    int msgid = msgget(key, IPC_CREAT | 0666);

    struct message msg;
    while (msgrcv(msgid, &msg, sizeof(msg), 2, IPC_NOWAIT) != -1)
    {

        printf("%s", msg.sms);
    }

    while (msgrcv(msgid, &msg, sizeof(msg), 1, IPC_NOWAIT) != -1)
    {

        printf("%s", msg.sms);
    }
    
    return 0;
}