#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    char n = rand() % 10;
    char m = rand() % 10;

    char n_str[10];
    char m_str[10];

    sprintf(n_str, "%d", n);
    sprintf(m_str, "%d", m);

    char *args[] = {"./ese4_prodotto", m_str, n_str, NULL};
    if (execv("./ese4_prodotto", args) == -1)
    {
        perror("exec error");
        exit(EXIT_FAILURE);
    }

    return 0;
}