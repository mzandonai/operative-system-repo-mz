#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "consumer.h"
#include "errExit.h"

#define MSG_BYTES 100

void consumer (int *pipeFD) {
    // close pipe's write end
    ...

    ssize_t rB = -1;
    char buffer[MSG_BYTES + 1];
    do {
        // read max MSG_BYTES chars from the pipe
        ...
    } while (rB > 0);

    // close pipe's read end
    ...
}
