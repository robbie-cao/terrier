#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

#include <sys/select.h>
#include <sys/queue.h>
#include <sys/stat.h>

#include "log.h"


#define BUFFER_SIZE         256

static uint8_t buf[BUFFER_SIZE];

int main (void)
{
    const char *LOG_TAG = "MAIN";
    fd_set readfds;
    int idx = 0;

    fprintf(stdout, "Welcome - %s %s\n", __DATE__, __TIME__);

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    while (1) {
        // Wait for input from stdin
        if (select(1, &readfds, NULL, NULL, NULL)) {
            int c;

            idx = 0;
            // Store input into buf, newline ('\n') as input done
            while ((c = fgetc(stdin)) != '\n') {
                buf[idx++] = c;
            }
            buf[idx] = '\0';

            LOGD(LOG_TAG, "%s\n", buf);
        }

        if (strcmp((char *)buf, "quit") == 0) {
            break;
        }

        printf("...\n");

        // Parse buffer and action accordingly
        // TODO
    }

    return 0;
}
