#pragma once
#include <string>
#include "util.h"

namespace response {
    void login(std::string name);
    void logout(std::string name);
    void send_message(std::string sender, struct datetime dt, std::string msg);
}
