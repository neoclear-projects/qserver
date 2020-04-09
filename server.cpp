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
#include "src/scdt.h"
#include "src/codec.h"

#define MSG_SIZE    0x4000

using namespace std;

void send_msg(int sender, char *msg) {
    sock_pool* pool = sock_pool::get_instance();

    // delete later
    struct datetime dt;

    char *buffer = transmit::gen_buf();
    encode::gen_msg(buffer,
                    dt,
                    pool->has_name(sender) ? pool->get_sockname(sender) : "Anonymous",
                    msg);

    pool->for_each([&](int sock) {
        int rc = transmit::dispatch(sock, buffer);
        if (rc == COM_FAIL)
            pool->delete_sock(sock);
    });

    transmit::free_buf(buffer);
}

void handle(int socket) {
    cout << "Client " << socket << " online" << endl;

    sock_pool* pool = sock_pool::get_instance();
    
    pool->add_sock(socket);

    char *buffer = transmit::gen_buf();
    char *name = (char *)malloc(NAME_SIZE);
    char *msg = (char *)malloc(MSG_SIZE);
    struct datetime dt;

    while (pool->is_alive(socket)) {
        // cout << "@@@" << endl;
        transmit::clear_buf(buffer);
        // memset(msg, 0, MSG_SIZE);

        int rc = transmit::receive(socket, buffer);
        if (rc == COM_FAIL) {
            pool->delete_sock(socket);
            continue;
        }

        switch (decode::request_type(buffer)) {
        case HEADER_NAME:
            decode::get_name(name, buffer);
            pool->register_sockname(socket, string(name));
            cout << "Client " << socket << " registered name: " << name << endl;
            break;
        case HEADER_MSG:
            decode::get_message(msg, buffer);
            cout << "Client " << socket << " sent " << msg << endl;
            send_msg(socket, msg);
            break;
        }

        // cout << "Client " << socket << " sent: " << 
        // printf("%s: %s\n", pool->get_client_name(socket).c_str(), buffer);
        // send_msg(socket, buffer);
    }

    cout << "Client " << socket << " disconnected" << endl;
    transmit::free_buf(buffer);
    free(name);
    pool->destroy_sock(socket);
}

 
int main(int argc, char *argv[])
{
    int ssock = -1, csock = -1;
    socklen_t clen = 0;
 
    struct sockaddr_in saddr, caddr;
 
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
 
    while (true) {
        // 接收连接，创建新的套接字
        csock = accept(ssock, (struct sockaddr *)&caddr, &clen);
        clen = sizeof(caddr);
        
        thread *t = new thread(handle, csock);
        t->detach();
    }
}
