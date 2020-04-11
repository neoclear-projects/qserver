#include "codec.h"
#include <string.h>

int decode::request_type(const char *buffer) {
    return (int)(*buffer);
}

void decode::get_name(char *name, const char *buffer) {
    strcpy(name, buffer + 1);
}

void decode::get_message(char *msg, const char *buffer) {
    strcpy(msg, buffer + 1);
}

int decode::get_past_msg_size(const char *buffer) {
    return *(int *)(buffer + 1);
}

int decode::get_datetime(struct datetime *dt, const char *buffer) {
    buffer++;
    dt->year = *(int *)(buffer);
    buffer += 4;
    dt->month = (int)(*(buffer++));
    dt->day = (int)(*(buffer++));
    dt->hour = (int)(*(buffer++));
    dt->minute = (int)(*(buffer++));
    dt->second = (int)(*(buffer++));
}

void fill_datetime(char *buffer, struct datetime dt) {
    *((int *)buffer) = dt.year;
    buffer += 4;

    *(buffer++) = (char)dt.month;
    *(buffer++) = (char)dt.day;
    *(buffer++) = (char)dt.hour;
    *(buffer++) = (char)dt.minute;
    *(buffer++) = (char)dt.second;
}

void encode::gen_msg(char *buffer, struct datetime dt, std::string sender, const char *msg) {
    *(buffer++) = HEADER_MSG;
    fill_datetime(buffer, dt);
    buffer += 9;

    strcpy(buffer, sender.c_str());
    buffer += NAME_SIZE;
    strcpy(buffer, msg);
}

void encode::gen_past_msg(char *buffer, struct datetime dt, std::string sender, const char *msg) {
    *(buffer++) = HEADER_PST_MSG;
    fill_datetime(buffer, dt);
    buffer += 9;

    strcpy(buffer, sender.c_str());
    buffer += NAME_SIZE;
    strcpy(buffer, msg);
}

void encode::gen_sync_msg(char *buffer, struct datetime dt, std::string sender, const char *msg) {
    *(buffer++) = HEADER_SYNC;
    fill_datetime(buffer, dt);
    buffer += 9;

    strcpy(buffer, sender.c_str());
    buffer += NAME_SIZE;
    strcpy(buffer, msg);
}

void encode::gen_login_dat(char *buffer, std::string user) {
    *(buffer++) = 0x5;
    strcpy(buffer, user.c_str());
}

void encode::gen_logout_dat(char *buffer, std::string user) {
    *(buffer++) = 0x6;
    strcpy(buffer, user.c_str());
}

void encode::gen_online_dat(char *buffer, std::string user) {
    *(buffer++) = 0x4;
    strcpy(buffer, user.c_str());
}

void encode::gen_sync_end(char *buffer) {
    *buffer = 0x7;
}
