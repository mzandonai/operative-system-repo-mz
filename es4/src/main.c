#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define DIM 100
#include "errExit.h"
char buffer[DIM + 1];
int main(int argc, char *argv[])
{
    int source;
    int destination;
    ssize_t reader;
    if (argc == 3)
    {
        printf("Caso senza flag\n");
        source = open(argv[1], O_RDWR);
        if (source == -1)
        {
            errExit("open failed");
        }

        if (access(argv[2], F_OK) == 0)
        {
            printf("Il file destination esiste!\n");
            return 0;
        }

        destination = open(argv[2], O_RDWR | O_CREAT);
        if (destination == -1)
        {
            errExit("open failed");
        }

        do
        {
            reader = read(source, buffer, sizeof(buffer));
            if (reader == -1)
                errExit("read failed");
            if (reader > 0 && write(destination, buffer, reader) != reader)
                errExit("write failed");
        } while (reader > 0);

        close(source);
        close(destination);
    }
    else if (argc == 4)
    {
        if (strcmp(argv[1], "-s") == 0)
        {
            printf("Caso -s");
            source = open(argv[2], O_RDWR);
            if (source == -1)
            {
                errExit("Open failed");
            }

            if (access(argv[3], F_OK) != 0)
            {
                printf("Il file destination non esiste\n");
                return 0;
            }
            else
            {
                destination = open(argv[3], O_RDWR | O_TRUNC);
                if (destination == -1)
                {
                    errExit("Open failed");
                }
                do
                {
                    reader = read(source, buffer, sizeof(buffer));
                    if (reader == -1)
                        errExit("read failed");
                    if (reader > 0 && write(destination, buffer, reader) != reader)
                        errExit("write failed");
                } while (reader > 0);
            }
        }
        else if (strcmp(argv[1], "-a") == 0)
        {
            printf("Caso -a");
            source = open(argv[2], O_RDWR);
            if (source == -1)
            {
                errExit("Open failed");
            }

            if (access(argv[3], F_OK) != 0)
            {
                printf("Il file destination non esiste\n");
                return 0;
            }
            else
            {
                destination = open(argv[3], O_RDWR);
                if (destination == -1)
                {
                    errExit("Open failed");
                }
                if (lseek(destination, -1, SEEK_END) == -1)
                {
                    errExit("lseek failed");
                }

                do
                {
                    reader = read(source, buffer, sizeof(buffer));
                    if (reader == -1)
                        errExit("read failed");
                    if (reader > 0 && write(destination, buffer, reader) != reader)
                        errExit("write failed");
                } while (reader > 0);
            }
        }
        else
        {
            printf("Flag errato\n");
            return 0;
        }
    }

    return 0;
}