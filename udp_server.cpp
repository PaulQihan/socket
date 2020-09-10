#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <iostream>
#define PORT 8000
#define BUFFER_SIZE 1024

int main()
{
    int ss = socket(AF_INET, SOCK_STREAM, 0);//若成功则返回一个sockfd（套接字描述符）
    struct sockaddr_in server_sockaddr;

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(PORT);//将一个无符号短整型数值转换为网络字节序，即大端模式(big-endian)　
    //printf("%d\n",INADDR_ANY);
    //INADDR_ANY就是指定地址为0.0.0.0的地址，这个地址事实上表示不确定地址，或“所有地址”、“任意地址”。
    //一般来说，在各个系统中均定义成为0值。
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//将主机的无符号长整形数转换成网络字节顺序。
    int len = sizeof(server_sockaddr);
    if(bind(ss, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    struct sockaddr_in client_sockaddr;

    while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        printf("server wait:\n");
        int recv_num =recvfrom(ss, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&client_sockaddr, (socklen_t *)&len);

        if(recv_num < 0)
        {
            perror("recv_error");
            exit(1);            
        }

        recvbuf[recv_num] = '\0';
        printf("server receive %d bytes: %s\n", recv_num, recvbuf);  
        
        int send_num = sendto(ss, sendbuf, recv_num, 0, (struct sockaddr *)&client_sockaddr, (socklen_t *)&len);
        if(send_num < 0)
        {
            perror("send_error");
            exit(1);            
        }
    }
    close(ss);//关闭socket套接字描述符
    return 0;
}