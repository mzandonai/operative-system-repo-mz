
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH_MAX 100
int main(int argc, char *argv[])
{
    char *home = getenv("HOME");
    printf("Variabile %s\n", home);

    char *user = getenv("USER");
    printf("Variabile: %s\n", user);

    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    printf("Current: %s\n", buf);

    int check = strncmp(buf, home, strlen(home));
    if (check == 0)
    {
        printf("Ciao USER, sono già nel posto giusto!\n");
    }
    else
    {
        printf("Current dir not sub dir\n");
        int changing = chdir(home);
        if (changing == -1)
        {
            printf("Not changed");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Changed");
        }

        int fd = open("prova.txt", O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("Open failed");
            exit(EXIT_FAILURE);
        }

        if (close(fd) == -1)
        {
            perror("close failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}