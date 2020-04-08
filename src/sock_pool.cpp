#include "sock_pool.h"
#include <unistd.h>

std::atomic<sock_pool *> sock_pool::pinstance{ nullptr };
std::mutex sock_pool::_m;

sock_pool* sock_pool::get_instance() {
    if (pinstance == nullptr) {
        _m.lock();
        if (pinstance == nullptr)
            pinstance = new sock_pool();
        _m.unlock();
    }
    return pinstance;
}

void sock_pool::add_sock(int socket, std::string client) {
    sock_mtx.lock();
    socks[socket] = client;
    sock_mtx.unlock();
}

std::string sock_pool::get_client_name(int socket) {
    return socks[socket];
}

// delete sock from active sock pool & put it to remove pool
void sock_pool::delete_sock(int socket) {
    rm_mtx.lock();
    rms[socket] = socks[socket];
    rm_mtx.unlock();

    sock_mtx.lock();
    socks.erase(socket);
    sock_mtx.unlock();
}

// stop connection of this socket
void sock_pool::destroy_sock(int socket) {
    close(socket);
}

// check if a socket is waiting to be terminated
bool sock_pool::is_alive(int socket) {
    return !rms.count(socket);
}

// get the value of dead socket if it is waiting to be terminated
std::string sock_pool::get_death_name(int socket) {
    return rms[socket];
}

void sock_pool::for_each(std::function<void(int, std::string)> capture) {
    for (const auto& it : socks)
        capture(it.first, it.second);
}
