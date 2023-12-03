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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8888               // Port of server
#define BUFFER 1024             // Buffer for every input and output
#define BACKLOG 10              // Backlog of the server
#define TERMINAL_EMULATOR "gnome-terminal"    // Command to open terminal "gnome-terminal" in ubuntu


int createTcpIp4Socket();
struct sockaddr_in* setAddress(char *ip, int port);

#endif
