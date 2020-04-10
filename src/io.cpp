#include "io.h"
#include "transmit.h"
#include <string>
#include <thread>
#include "sock_pool.h"

void io::dispatch_all(const char *buffer) {
    sock_pool* pool = sock_pool::get_instance();

    pool->for_each([&](int sock) {
        int rc = transmit::dispatch(sock, buffer);
        if (rc == COM_FAIL) {
            pool->delete_sock(sock);
        }
    });
}
