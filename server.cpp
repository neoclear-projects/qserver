#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <thread>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <iostream>
#include "src/sock_pool.h"
#include "src/transmit.h"
#include "src/util.h"
#include "src/codec.h"
#include "src/io.h"
#include "src/handle.h"
#include "src/response.h"
#include "src/db_access.h"

#define MSG_SIZE    0x4000

using namespace std;

void connection(int socket) {
    cout << "Client " << socket << " online" << endl;

    // initialization
    sock_pool* pool = sock_pool::get_instance();
    pool->add_sock(socket);

    char *buffer = transmit::gen_buf();

    // If failed to request or request is not register name
    if (transmit::receive(socket, buffer) == COM_FAIL ||
        decode::request_type(buffer) != HEADER_NAME) {
        pool->delete_sock(socket);
    } else {
        // If successful
        handle::name_register(socket, buffer);
    }

    while (pool->is_alive(socket)) {
        transmit::clear_buf(buffer);

        // If failed to receive data
        if (transmit::receive(socket, buffer) == COM_FAIL) {
            pool->delete_sock(socket);
            continue;
        }
        switch (decode::request_type(buffer)) {
        case HEADER_NAME:
            // preserved
            break;
        case HEADER_MSG:
            handle::handle_message(socket, buffer);
            break;
        case HEADER_PST_MSG:
            // reserved
            break;
        case HEADER_SYNC:
            handle::handle_sync(socket, buffer);
            break;
        case HEADER_USR_STS:
            handle::handle_user_status(socket);
            break;
        }
    }

    string client = pool->get_sockname(socket);
    cout << "Client " << socket << " disconnected" << endl;
    transmit::free_buf(buffer);
    pool->destroy_sock(socket);
    response::logout(client);
}

int main(int argc, char *argv[])
{
    int ssock = -1, csock = -1;
    struct sockaddr_in saddr, caddr;
    socklen_t clen = sizeof(caddr);
 
    // 创建流套接字
    ssock = socket(AF_INET, SOCK_STREAM, 0);
 
    // 设置服务器接收的连接地址和监听的端口
    saddr.sin_family = AF_INET;                   // 指定网络套接字
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);    // 接受所有IP地址的连接
    saddr.sin_port = htons(9736);                 // 绑定到 9736 端口

    // 绑定（命名）套接字
    bind(ssock, (struct sockaddr *)&saddr, sizeof(saddr));

    // 创建套接字队列，监听套接字
    listen(ssock, 1024);
    
    db_access::get_instance()->connect("server.db");

    while (true) {
        // 接收连接，创建新的套接字
        csock = accept(ssock, (struct sockaddr *)&caddr, &clen);
        thread *t = new thread(connection, csock);
        t->detach();
    }
}
