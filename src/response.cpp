#include "response.h"
#include "codec.h"
#include "transmit.h"
#include "io.h"

void response::login(std::string name) {
    char *buffer = transmit::gen_buf();

    encode::gen_login_dat(buffer, name);
    io::dispatch_all(buffer);

    transmit::free_buf(buffer);
}

void response::logout(std::string name) {
    char *buffer = transmit::gen_buf();

    encode::gen_logout_dat(buffer, name);
    io::dispatch_all(buffer);

    transmit::free_buf(buffer);
}

void response::send_message(std::string sender, struct datetime dt, std::string msg) {
    char *buffer = transmit::gen_buf();

    encode::gen_msg(buffer, dt, sender.c_str(), msg.c_str());
    io::dispatch_all(buffer);

    transmit::free_buf(buffer);
}
