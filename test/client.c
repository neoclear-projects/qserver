#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
 
int main(int argc, char **argv)
{
    int sockfd = -1;
    int len = 0;
    struct sockaddr_in address;
    int result;
 
    // 创建流套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
 
    // 设置要连接的服务器的信息
    address.sin_family = AF_INET;                       // 使用网络套接字
    address.sin_addr.s_addr = inet_addr("127.0.0.1");   // 服务器地址
    address.sin_port = htons(9736);                     // 服务器所监听的端口
    len = sizeof(address);

    char *msg = (char *)malloc(sizeof(char) * 0x4000);

    // 连接到服务器
    result = connect(sockfd, (struct sockaddr *)&address, (socklen_t)len);

    if (result == -1) {
        perror("ops: client\n");
        exit(1);
    }
    char name[1024];
    printf("Enter your username: ");
    scanf("%s", name + 1); getchar();
    name[0] = 0x1;
    send(sockfd, name, 1024, 0);

    while (1) {
        printf(">>> ");
        scanf("%[^\n]", msg + 1); getchar();
        msg[0] = 0x0;
        if (strcmp(msg + 1, ".exit") == 0)
            exit(0);
        if (send(sockfd, msg, 0x4000, 0) <= 0 && errno != EINTR) {
            printf("Server down");
            exit(0);
        }
        // write(sockfd, msg, 1024);
        if (recv(sockfd, msg, 0x4000, 0) <= 0 && errno != EINTR) {
            printf("Server down");
            exit(0);
        }
        printf("%s: %s\n", msg + 10, msg + 1034);
        // printf("Server: %s\n", msg);
        
    }
    close(sockfd);
    // while (strcmp((msg = readline(">>> ")), ".exit") != 0) {
    //     write(sockfd, msg, strlen(msg) + 1);
    //     free(msg)
    //     msg = (char *)malloc(sizeof(char) * 1024);
    //     read(sockfd, msg, 1024);
    //     printf("Server: %s\n", msg);
    //     free(msg);
    // }
    exit(0);
}
