#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH_MAX 4096

int main(int argc, char *argv[])
{
    char buf[PATH_MAX];
    char *user = getenv("USER");
    char *home = getenv("HOME");

    printf("%s\n", home);

    getcwd(buf, PATH_MAX);
    printf("%s\n", buf);

    if (strlen(buf) < strlen(home))
    {
        printf("The current dir is not a sub dir of HOME\n");
        if (chdir(home) != 0)
        {
            perror("chdir error");
            return 1;
        }
        else
        {
            printf("Current dir changed\n");
        }

        int fd = open("empy_fie.txt", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("open failed");
            return 1;
        }
    }
    else
    {
        printf("Current dir is a sub dir\n");
    }

    return 0;
}