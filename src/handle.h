#pragma once

namespace handle {
    void name_register(int socket, const char *buffer);
    void handle_message(int socket, const char *buffer);
    void handle_user_status(int socket);
    void handle_sync(int socket, const char *buffer);
}
