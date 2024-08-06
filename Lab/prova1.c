#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#define PATH_MAX 50

int main(int argc, int *argv[])
{
    /*
        PID --> Process Identifier
    */

    pid_t pid1 = getpid(); // PID del processo

    pid_t pid2 = getuid();  // ID raele dell'utente
    pid_t pid3 = geteuid(); // ID effettivo dell'utente
    pid_t pid4 = getgid();  // ID reale del gruppo
    pid_t pid5 = getegid(); // ID effettivo del gruppo

    printf("PID: %d\n", pid1);
    printf("PID: %d\n", pid2);
    printf("PID: %d\n", pid3);
    printf("PID: %d\n", pid4);
    printf("PID: %d\n", pid5);

    /*
        Dopo l'esecuzione gli ID:
        - 68505
        - 1000
        - 1000
        - 1000
        - 1000
    */
    // --------------------------------------------------

    /* ENVIRONMENT
        Ogni processo possiede un array di stringhe chiamato
        environment. Per ciascun campo della lista è associato
        un valore.

        Il processo figlio eredita una copia dell'environment del padre
    */

    char *path = getenv("PATH");
    if (path != NULL)
    {
        printf("Il percorso: %s\n", path);
    }
    else
    {
        printf("Variabile PATH\n");
    }

    setenv("MY VAR", "Hello World", 1) == 0;
    printf("MY_VAR è stata impostata a %s\n", getenv("MY_VAR"));

    if (unsetenv("MY_VAR") == 0)
    {
        printf("MY_VAR è stato rimossa \n");
    }

    if (getenv("MY_VAR") == NULL)
    {
        printf("MY_VAR non esiste più \n");
    }

    /*

        Process Attributes

    */

    char buf[PATH_MAX];
    int fd = open(".", O_RDONLY);
    getcwd(buf, PATH_MAX);
    printf("1) Current dir: \n\t%s\n", buf);

    chdir("/tmp");
    getcwd(buf, PATH_MAX);
    printf("2) Current dir: \n\t%s\n", buf);

    fchdir(fd);
    getcwd(buf, PATH_MAX);
    printf("3) Current dir: \n\t%s\n", buf);

    return 0;
}