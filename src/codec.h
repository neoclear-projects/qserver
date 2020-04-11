#pragma once

#include "util.h"
#include <string>

#define HEADER_NAME     0x1
#define HEADER_MSG      0x0
#define HEADER_PST_MSG  0x2
#define HEADER_SYNC     0x3
#define HEADER_USR_STS  0x4

// decode request data & database data
namespace decode {
    int request_type(const char *buffer);

    // name
    void get_name(char *name, const char *buffer);

    // message
    void get_message(char *msg, const char *buffer);

    // request latest message
    int get_past_msg_size(const char *buffer);

    // sync data
    int get_datetime(struct datetime *dt, const char *buffer);
    
}

namespace encode {
    void gen_msg(char *buffer, struct datetime dt, std::string sender, const char *msg);
    void gen_past_msg(char *buffer, struct datetime dt, std::string sender, const char *msg);
    void gen_sync_msg(char *buffer, struct datetime dt, std::string sender, const char *msg);
    void gen_login_dat(char *buffer, std::string user);
    void gen_logout_dat(char *buffer, std::string user);
    void gen_online_dat(char *buffer, std::string user);
    void gen_sync_end(char *buffer);
}
