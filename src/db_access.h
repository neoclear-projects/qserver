#pragma once

#include "sqlite3.h"
#include <vector>
#include <unordered_map>
#include <atomic>
#include <mutex>

class db_access {
private:
    static std::atomic<db_access *> pinstance;
    static std::mutex _m;
    db_access();

    static constexpr int err_size = 1024;
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    void set_errmsg(std::string msg);

    std::mutex *db_mtx;
    std::mutex *query_mtx;
    std::mutex *err_mtx;

public:
    static db_access* get_instance();

    void connect(const char *db_name);
    std::vector<std::unordered_map<std::string, std::string>> query(const char *stmt);
    bool is_ok();
    std::string get_errmsg();
    void close();
    void reset_err();
};
