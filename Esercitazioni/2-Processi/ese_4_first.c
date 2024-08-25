#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int x = n * m;
    printf("Il prodotto per n e m è: %d\n", x);
    return 0;
}