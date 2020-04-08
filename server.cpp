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

#define MSG_SIZE 1024

using namespace std;

void send_msg(int sender, char *msg) {
    sock_pool* pool = sock_pool::get_instance();

    char dat[MSG_SIZE];
    sprintf(dat, "%s: %s", pool->get_client_name(sender).c_str(), msg);

    pool->for_each([&](int sock, string client) {
        if (send(sock, dat, MSG_SIZE, 0) <= 0 && errno != EINTR) {
            pool->delete_sock(sock);
        }
    });
}

void handle(int socket) {
    char msg[MSG_SIZE];

    sock_pool* pool = sock_pool::get_instance();

    pool->add_sock(socket, "NeoClear");
    cout << pool->get_client_name(socket) << endl;

    while (1) {
        if (!pool->is_alive(socket)) {
            printf("%s Disconnected\n", pool->get_death_name(socket).c_str());
            pool->destroy_sock(socket);
            return;
        }
        
        memset(msg, 0, sizeof(char) * MSG_SIZE);

        if (recv(socket, msg, MSG_SIZE, 0) <= 0 && errno != EINTR) {
            pool->delete_sock(socket);
            continue;
        }

        printf("%s: %s\n", pool->get_client_name(socket).c_str(), msg);
        send_msg(socket, msg);
    }
}

 
int main(int argc, char *argv[])
{
    int ssock = -1;
    int csock = -1;
    socklen_t clen = 0;
 
    struct sockaddr_in saddr;
    struct sockaddr_in caddr;
 
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
 
    char *msg = (char *)malloc(sizeof(char) * 1024);

    while (1) {
        // 接收连接，创建新的套接字
        csock = accept(ssock, (struct sockaddr *)&caddr, &clen);

        clen = sizeof(caddr);
        
        thread *t = new thread(handle, csock);
        t->detach();
    }
}
