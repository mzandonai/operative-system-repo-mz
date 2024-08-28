#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

int main()
{
    const char *fifo_name = "/tmp/myfifo";

    // Creazione della FIFO
    if (mkfifo(fifo_name, 0666) == -1)
    {
        perror("mkfifo");
        return 1;
    }

    printf("FIFO creata con successo: %s\n", fifo_name);

    int fd = open(fifo_name, O_WRONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    return 0;
}
