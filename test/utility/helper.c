#include "helper.h"

int createTcpIp4Socket()
{
    
    return socket(AF_INET, SOCK_STREAM, 0);
    
}

struct sockaddr_in* setAddress(char *ip, int port)
{
    struct sockaddr_in *address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if(ip[0]=='\0')
    {
        address->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if(inet_pton(AF_INET, ip, &address->sin_addr.s_addr) < 0)
        {
            perror("Error occured during convertion of ip to binary\n");
            free(address);
            exit(EXIT_FAILURE);
        }
    }
    return address;
    
}