#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "errExit.h"

#define BUFFER_SZ 4096

int main(int argc, char *argv[])
{
    char *stringSearch, *stringReplace;
    char *fileSource, *fileDestination;

    if (argc != 5)
    {
        printf("Usage: %s <sourceFile> <destinationFile> <string search> <string replace>\n", argv[0]);
        return 1;
    }
    else
    {
        fileSource = argv[1];
        fileDestination = argv[2];
        stringSearch = argv[3];
        stringReplace = argv[4];
    }

    int fileS = open(fileSource, O_RDONLY);
    if (fileS == -1)
    {
        printf("File %s does not exist\n", fileSource);
        errExit("source open failed");
    }

    int fileD = -1;
    if (access(fileDestination, F_OK) == 0)
    {
        printf("File %s already exists!\n", fileDestination);
        return 1;
    }

    fileD = open(fileDestination, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fileD == -1)
        errExit("open failed");

    char buffer[BUFFER_SZ];
    ssize_t bytesRead;
    ssize_t searchLen = strlen(stringSearch);
    
    while ((bytesRead = read(fileS, buffer, sizeof(buffer))) > 0)
    {
        for (ssize_t i = 0; i < bytesRead; i++)
        {
            if (memcmp(&buffer[i], stringSearch, searchLen) == 0)
            {
                // Found a match, write the replacement string
                if (write(fileD, stringReplace, strlen(stringReplace)) == -1)
                    errExit("write failed");
                i += searchLen - 1;  // Skip the characters already replaced
            }
            else
            {
                // No match, write the original character
                if (write(fileD, &buffer[i], 1) == -1)
                    errExit("write failed");
            }
        }
    }

    if (bytesRead == -1)
        errExit("read failed");

    if (close(fileS) == -1 || close(fileD) == -1)
        errExit("close failed");

    return 0;
}
