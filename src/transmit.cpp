#include "transmit.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

char *transmit::gen_buf() {
    return (char *)calloc(1, BUF_SIZE);
}

void transmit::free_buf(char *buf) {
    free(buf);
}

void transmit::clear_buf(char *buf) {
    memset(buf, 0, BUF_SIZE);
}

int transmit::dispatch(int socket, const char *msg) {
    int rc = send(socket, msg, BUF_SIZE, 0);
    if (rc <= 0 && errno != EINTR)
        return COM_FAIL;
    return COM_OK;
}

int transmit::receive(int socket, char *msg) {
    int rc = recv(socket, msg, BUF_SIZE, 0);
    if (rc <= 0 && errno != EINTR)
        return COM_FAIL;
    return COM_OK;
}
