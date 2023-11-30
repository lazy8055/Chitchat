#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/socket.h>
#include<errno.h>

#define PORT 8888
#define BUFFER 100

int createTcpIp4Socket();
struct sockaddr_in* setAddress(char *ip, int port);

int main()
{
    int client_socket;
    char *server_ip = NULL;
    size_t ip_len = 0;
    char send_buffer[BUFFER], recv_buffer[BUFFER];

    if((client_socket = createTcpIp4Socket()) == -1)
    {
        perror("Error occured while creating the socket!\nTry again later!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Enter the server ip: ");
    getline(&server_ip, &ip_len, stdin);
    


    struct sockaddr_in* address = setAddress(server_ip, PORT);

    if(connect(client_socket, (struct sockaddr*)address, sizeof(*address)) == -1)
    {
        perror("Can't able to connect to the server!\nTry Again!\n");
        free(address);
        free(server_ip);
        exit(EXIT_FAILURE);
    }
    do
    {
        printf("Enter a msg: ");
        fgets(send_buffer, BUFFER, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        send(client_socket,send_buffer, BUFFER,0);

        recv(client_socket, recv_buffer, BUFFER, 0);
        printf("%s\n", recv_buffer);
    }while(strcmp(send_buffer,"exit") != 0);

    free(address);
    free(server_ip);

    return 0;

}

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