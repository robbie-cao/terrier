#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "cmd.h"
#include "stm8_cmd.h"

/**
 * Parse cmd string into cmd array
 *
 * input: cmd array, number of cmds
 * output: 0 for success, other for fail
 */
int cmd_parse(const uint8_t *cmd_bytes, unsigned int len, cmd_callback_t cb)
{
    int pos = 0;
    uint16_t cmd_size;
    const uint8_t *p = cmd_bytes;

    if (!cmd_bytes || !len) {
        return -1;
    }

    // Skip bytes until leading sign
    while (p[pos] != 0x55) {
        pos += 1;
    }

    /**
     * byte 0  : 0x55
     * byte 1-2: size
     */
    if (pos >= len - 3) {
        return -1;
    }

    cmd_size = (p[pos + 1] << 8) | p[pos + 2];
    pos += 3;

    if (pos + cmd_size > len) {
        return -1;
    }

    struct stm8_cmd *scmd = (struct stm8_cmd *)(p + pos);
    if (cb) {
        cb(scmd, 1);
    }
#if 0
    cmd = p[pos];
    pos += 1;
    while (pos < len) {
        struct stm8_cmd *scmd = (struct stm8_cmd *)(p + pos);
        if (cb) {
            cb(scmd, 1);
        }
        switch (cmd) {
            case LED_GET:
                pos += 2;
                break;
            case BUTTON_GET:
                pos += 2;
                break;
            case VOL_GET:
                pos += 1;
                break;
            case NFC_CARD_INFO:
            case NFC_READ_CARD:
                // TODO
            default:
                pos += 1;
                break;
        }
    }
#endif

    return 0;
}
