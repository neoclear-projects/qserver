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

void sock_pool::add_sock(int socket) {
    sock_mtx.lock();
    socks.insert(socket);
    sock_mtx.unlock();
}

// delete sock from active sock pool & put it to remove pool
void sock_pool::delete_sock(int socket) {
    sock_mtx.lock();
    socks.erase(socket);
    sock_mtx.unlock();
}

// stop connection of this socket
void sock_pool::destroy_sock(int socket) {
    name_mtx.lock();
    sock_name.erase(socket);
    name_mtx.unlock();
    close(socket);
}

// check if a socket is waiting to be terminated
bool sock_pool::is_alive(int socket) {
    return socks.count(socket);
}

void sock_pool::for_each(std::function<void(int)> capture) {
    for (const auto& it : socks)
        capture(it);
}

void sock_pool::register_sockname(int socket, std::string name) {
    name_mtx.lock();
    sock_name[socket] = name;
    name_mtx.unlock();
}

bool sock_pool::has_name(int socket) {
    return sock_name.count(socket);
}

std::string sock_pool::get_sockname(int socket) {
    return sock_name[socket];
}
