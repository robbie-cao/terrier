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
#include "hex.h"
#include "cmd.h"


#define BUFFER_SIZE         256

static uint8_t buf_in[BUFFER_SIZE];
#if DEBUG
static uint8_t buf2[BUFFER_SIZE / 2];
#endif

static int cmd_callback(const struct stm8_cmd *pcmd, uint8_t cmd_size)
{
    uint8_t i;
    uint8_t cmd;

    if (!pcmd || !cmd_size) {
        return -1;
    }

    cmd = pcmd->cmd_code;
    printf("CMD: %d\n", pcmd->cmd_code);

    switch (cmd) {
        case LED_GET:
            for (i = 0; i < cmd_size; i += sizeof(struct led_cmd)) {
                printf("LED %d - %d\n", pcmd->data[i], pcmd->data[i + 1]);
            }
            break;
        case BUTTON_GET:
            for (i = 0; i < cmd_size; i += sizeof(struct button_cmd)) {
                printf("BTN %d - %d\n", pcmd->data[i], pcmd->data[i + 1]);
            }
            break;
        case VOL_GET:
            for (i = 0; i < cmd_size; i += sizeof(struct vol_cmd)) {
                printf("VOL %d\n", pcmd->data[i]);
            }
            break;
        case NFC_CARD_INFO:
        case NFC_READ_CARD:
            // TODO
        default:
            break;
    }

    return 0;
}

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
        int len = hex2data(buf2, (char *)buf_in, strlen((char *)buf_in));
        hexdump(buf2, len);
        cmd_parse(buf2, len, cmd_callback);
#endif
    }

    return 0;
}
