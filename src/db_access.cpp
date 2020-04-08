#include "db_access.h"
#include <string.h>
#include <iostream>

std::atomic<db_access *> db_access::pinstance{ nullptr };
std::mutex db_access::_m;

db_access* db_access::get_instance() {
    if (pinstance == nullptr) {
        _m.lock();
        if (pinstance == nullptr)
            pinstance = new db_access();
        _m.unlock();
    }
    return pinstance;
}

db_access::db_access() {
    db_mtx = new std::mutex();
    query_mtx = new std::mutex();
    err_mtx = new std::mutex();

    err_msg = (char *)malloc(sizeof(char) * err_size);
}

void db_access::set_errmsg(std::string msg) {
    err_mtx->lock();
    strcpy(err_msg, msg.c_str());
    err_mtx->unlock();
}

void db_access::connect(const char *db_name) {
    db_mtx->lock();
    if (db == NULL && db_name != NULL) {
        int rc = sqlite3_open(db_name, &db);
        if (rc != SQLITE_OK) {
            close();
            set_errmsg("Unable to open database named: " + std::string(db_name));
        }
    }
    db_mtx->unlock();
}

bool db_access::is_ok() {
    return err_msg == NULL || strlen(err_msg) == 0;
}

void db_access::close() {
    db_mtx->lock();
    query_mtx->lock();
    if (db != NULL) {
        int rc = sqlite3_close(db);
        if (rc != SQLITE_OK)
            set_errmsg("Unable to close database");
        db = NULL;
    }
    query_mtx->unlock();
    db_mtx->unlock();
}

void db_access::reset_err() {
    err_mtx->lock();
    memset(err_msg, 0, sizeof(char) * err_size);
    err_mtx->unlock();
}

std::string db_access::get_errmsg() {
    err_mtx->lock();
    std::string res(err_msg);
    err_mtx->unlock();
    return res;
}

int gen_res(void *dat, int col_num, char **col_val, char **col_name) {
    auto res = (std::vector<std::unordered_map<std::string, std::string>> *)dat;
    res->push_back({});
    for (int i = 0; i < col_num; i++) {
        res->back()[std::string(col_name[i])] = std::string(col_val[i]);
    }
    return 0;
}

std::vector<std::unordered_map<std::string, std::string>> db_access::query(const char *stmt) {
    if (stmt == NULL || db == NULL)
        return {};

    char *tmp = (char *)malloc(1024);

    // std::cout << stmt << std::endl;
    query_mtx->lock();
    err_mtx->lock();
    std::vector<std::unordered_map<std::string, std::string>> result;
    int rc = sqlite3_exec(db, stmt, gen_res, (void *)&result, (char **)&tmp);
    if (rc != SQLITE_OK) {
        strcpy(err_msg, tmp);
        sqlite3_free((char *)tmp);
    }
    err_mtx->unlock();
    query_mtx->unlock();
    return result;
}
