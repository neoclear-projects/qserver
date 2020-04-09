#pragma once

#define BUF_SIZE    0x4000
#define COM_OK      0x0
#define COM_FAIL    0x1

namespace transmit {
    char *gen_buf();
    void free_buf(char *buf);
    void clear_buf(char *buf);
    int receive(int socket, char *msg);
    int dispatch(int socket, const char *msg);
}
