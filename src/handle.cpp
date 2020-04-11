#include "handle.h"
#include "sock_pool.h"
#include <iostream>
#include "codec.h"
#include "response.h"
#include "util.h"
#include "db_access.h"
#include <string>
#include "transmit.h"
#include <unordered_map>
#include <vector>
#include <string.h>

// name_register could only happen once through the life cycle of a connection
void handle::name_register(int socket, const char *buffer) {
    sock_pool *pool = sock_pool::get_instance();

    // register names
    char *name = util::gen_name_buf();
    decode::get_name(name, buffer);
    pool->register_sockname(socket, std::string(name));
    std::cout << "Client " << socket << " registered name: " << name << std::endl;

    // send response to users
    response::login(std::string(name));
    util::free_name_buf(name);
}

void handle::handle_message(int socket, const char *buffer) {
    char *msg = util::gen_msg_buf();

    decode::get_message(msg, buffer);

    if (strlen(msg) == 0) {
        util::free_msg_buf(msg);
        return;
    }

    std::string name = sock_pool::get_instance()->get_sockname(socket);

    struct datetime cur_time = util::get_current_time();
    // store it in db
    db_access *access = db_access::get_instance();
    std::string stmt = "insert into Message values (\"" +
                       name + "\", \"" +
                       std::string(msg) + "\", \""
                       + util::datetime_to_string(cur_time) + "\")";
    std::cout << stmt << std::endl;
    access->query(stmt.c_str());

    // response to client machine
    response::send_message(name, cur_time, std::string(msg));

    util::free_msg_buf(msg);
}

void handle::handle_user_status(int socket) {
    char *buffer = transmit::gen_buf();

    sock_pool *pool = sock_pool::get_instance();

    pool->for_each([&](int sock) {
        transmit::clear_buf(buffer);
        encode::gen_online_dat(buffer, pool->get_sockname(sock));
        transmit::dispatch(sock, buffer);
    });

    transmit::free_buf(buffer);
}

void handle::handle_sync(int socket, const char *buffer) {
    struct datetime dt;
    decode::get_datetime(&dt, buffer);

    db_access *access = db_access::get_instance();
    std::string stmt = "SELECT * FROM Message WHERE time >= \"" +
                       util::datetime_to_string(dt) + "\" ORDER BY time ASC";
    
    char *buf = transmit::gen_buf();

    std::vector<std::unordered_map<std::string, std::string>> result = access->query(stmt.c_str());

    for (std::unordered_map<std::string, std::string>& it : result) {
        transmit::clear_buf(buf);
        encode::gen_sync_msg(buf,
                             util::string_to_datetime(it["time"]),
                             it["username"],
                             it["content"].c_str());
        transmit::dispatch(socket, buf);
    }

    // Send end of sync data to client
    transmit::clear_buf(buf);
    encode::gen_sync_end(buf);
    transmit::dispatch(socket, buf);

    transmit::free_buf(buf);
}
