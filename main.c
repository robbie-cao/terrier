#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <ctype.h>

#include <sys/select.h>
#include <sys/queue.h>
#include <sys/stat.h>

#include "log.h"


#define BUFFER_SIZE         256

static uint8_t buf_in[BUFFER_SIZE];
#if DEBUG
static uint8_t buf2[BUFFER_SIZE / 2];
#endif

#if DEBUG
/* Convert a hex char digit to its integer value. */
int hex2digit(char digit)
{
    // 0-9
    if ('0' <= digit && digit <= '9') {
        return (int)(digit - '0');
    }
    // a-f
    if ('a' <= digit && digit <= 'f') {
        return (int)(10 + digit - 'a');
    }
    // A-F
    if ('A' <= digit && digit <= 'F') {
        return (int)(10 + digit - 'A');
    }

    return 0;
}

/* Decode a hex string. */
int hex2data(unsigned char *data, const char *hexstring, unsigned int len)
{
    size_t count = 0;

    if (!hexstring || !data || !len) {
        return 0;
    }

    if (strlen(hexstring) == 0) {
        return 0;
    }

    for (count = 0; count < (len / 2); count++) {
        data[count] = hex2digit(hexstring[count * 2]);
        data[count] <<= 4;
        data[count] |= hex2digit(hexstring[count * 2 + 1]);
    }

    return count;
}

int data2hex(char *hexstring, const unsigned char *data, unsigned int len)
{
    size_t count = 0;

    if (!hexstring || !data || !len) {
        return 0;
    }

    for (count = 0; count < len; count++) {
        sprintf(hexstring, "%02x", data[count]);
    }
    hexstring[count * 2] = '\0';

    return strlen(hexstring);
}
#endif

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
            // Store input into buf_in, newline ('\n') as input done
            while ((c = fgetc(stdin)) != '\n') {
                buf_in[idx++] = c;
            }
            buf_in[idx] = '\0';

            LOGD(LOG_TAG, "%s\n", buf_in);
        }

        if (strcmp((char *)buf_in, "quit") == 0) {
            break;
        }

        printf("...\n");

        // Parse buffer and action accordingly
        // TODO
#if DEBUG
        hex2data(buf2, (char *)buf_in, strlen((char *)buf_in));
        LOGD(LOG_TAG, "%08X\n", *(uint32_t *)buf2);
#endif
    }

    return 0;
}
