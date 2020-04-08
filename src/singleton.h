#pragma once

#include <atomic>
#include <mutex>

class singleton {
private:
    singleton() {}
    static std::atomic<singleton *> pinstance;
    static std::mutex _m;

public:
    static singleton* get_instance();
};
