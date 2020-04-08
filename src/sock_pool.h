#pragma once

#include <atomic>
#include <mutex>
#include <functional>
#include <unordered_map>

class sock_pool {
private:
    static std::atomic<sock_pool *> pinstance;
    static std::mutex _m;
    sock_pool() {}

    std::mutex sock_mtx;
    std::unordered_map<int, std::string> socks;

    std::mutex rm_mtx;
    std::unordered_map<int, std::string> rms;

public:
    static sock_pool* get_instance();

    void add_sock(int socket, std::string client);
    std::string get_client_name(int socket);

    void delete_sock(int socket);

    void destroy_sock(int socket);

    bool is_alive(int socket);
    std::string get_death_name(int socket);

    void for_each(std::function<void(int, std::string)> capture);
};
