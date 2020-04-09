#pragma once

#include <atomic>
#include <mutex>
#include <functional>
#include <unordered_set>
#include <unordered_map>

class sock_pool {
private:
    static std::atomic<sock_pool *> pinstance;
    static std::mutex _m;
    sock_pool() {}

    std::mutex sock_mtx;
    std::unordered_set<int> socks;

    std::mutex name_mtx;
    std::unordered_map<int, std::string> sock_name;

public:
    static sock_pool* get_instance();

    void add_sock(int socket);

    // put sock to disconnect queue
    void delete_sock(int socket);

    // disconnect sock & free all space
    void destroy_sock(int socket);

    bool is_alive(int socket);

    void for_each(std::function<void(int)> capture);

    void register_sockname(int socket, std::string name);
    bool has_name(int socket);
    std::string get_sockname(int socket);
};
