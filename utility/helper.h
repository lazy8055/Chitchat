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
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdbool.h>
#include <regex.h>
#include <time.h>

#define PORT 8888                   // Port of server
#define BUFFER 1024                 // Buffer for every input and output
#define BACKLOG 10                  // Backlog of the server
#define TERMINAL_EMULATOR "gnome-terminal"    // Command to open terminal "gnome-terminal" in ubuntu
#define FLAG_TO_EXECUTE "--"        // Flag to execute the file in corresponding terminal emulator
#define COMMAND_LEN 100             // Length of the command to open terminal

// For coloured output
#define ANSI_RESET   "\x1B[0m"
#define ANSI_BOLD    "\x1B[1m"
#define ANSI_RED     "\x1B[31m"
#define ANSI_GREEN   "\x1B[32m"
#define ANSI_YELLOW  "\x1B[33m"
#define ANSI_BLUE    "\x1B[34m"
#define ANSI_MAGENTA "\x1B[35m"
#define ANSI_CYAN    "\x1B[36m"



int createTcpIp4Socket();
struct sockaddr_in* setAddress(char *ip, int port);
void display_banner();

#endif
