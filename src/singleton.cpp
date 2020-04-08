#include "singleton.h"

std::atomic<singleton *> singleton::pinstance{ nullptr };
std::mutex singleton::_m;

singleton* singleton::get_instance() {
    if (pinstance == nullptr) {
        _m.lock();
        if (pinstance == nullptr)
            pinstance = new singleton();
    }
    return pinstance;
}
