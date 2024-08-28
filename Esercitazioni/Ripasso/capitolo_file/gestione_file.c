#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    printf("---GESTIONE FILE---\n");

    int fd;
    fd = open("test.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1)
    {
        perror("open error");
        exit(EXIT_FAILURE);
    }

    const char *text = "Hello, world!\n";
    if (write(fd, text, 14) < 0)
    {
        perror("write error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("String to write: %s", text);

    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    printf("Reading...\n");
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Done!\n");
    }

    buffer[bytes_read] = '\0';
    printf("String after read: %s", buffer);

    printf("Let's unlink the file...\n");
    if (unlink("test.txt") == -1)
    {
        perror("unlink error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Done!\n");
    }

    printf("Renaming new_dir...\n");
    if (rename("./new_dir", "./renamed") == -1)
    {
        perror("rename dir error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Done!\n");
    }

    printf("Chmodding new_dir...\n");
    if (chmod("./renamed", S_IRUSR) == -1)
    {
        perror("chmod error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Done!\n");
    }

    struct stat fileStat;
    printf("Lstat dir...\n");
    if (stat("./renamed", &fileStat) == -1)
    {
        perror("chmod error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Done!\n");
    }

    return 0;
}