#ifndef HELPER_H
#define HELPER_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<errno.h>

#define PORT 8888
#define BUFFER 1024
#define BACKLOG 10

int createTcpIp4Socket();
struct sockaddr_in* setAddress(char *ip, int port);

#endif
