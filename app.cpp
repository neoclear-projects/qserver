#include <bits/stdc++.h>
#include "src/sqlite3.h"
#include "src/db_access.h"
#include <thread>

using namespace std;

void insert(int i) {
    db_access* access = db_access::get_instance();
    access->query("insert into Message values(\"fdas\", \"What the hell\", CURRENT_TIMESTAMP)");
}

void drop(int i) {
    db_access* access = db_access::get_instance();
    // access->query("delete from Message");
}

vector<thread *> pool;

int main()
{
    db_access::get_instance()->connect("server.db");
    int cc = 10000, dc = 98;
    
    while (cc--) {
        pool.emplace_back(new thread(insert, cc));
    }
    for (const auto& it : pool) {
        it->join();
    }
    while (!pool.empty())
        pool.pop_back();

    while (dc--) {
        pool.emplace_back(new thread(drop, dc));
    }
    for (const auto& it : pool) {
        it->join();
    }

    db_access::get_instance()->close();
    return 0;
}
