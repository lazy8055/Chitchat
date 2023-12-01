#include "utility/helper.h"



int main(int argc, char const *argv[]) 
  
{
    int server_socket;
    if((server_socket = createTcpIp4Socket()) == -1)
    {
        perror("Error occured while creating the socket!\nTry again later!\n");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (void *) &opt, sizeof(opt));

    struct sockaddr_in* server_address = setAddress("", PORT);
    if(bind(server_socket, (struct sockaddr*)server_address, sizeof(*server_address)) == -1)
    {
        perror("Error occured while binding the port!\nTry again later!\n");
        exit(EXIT_FAILURE);
    }
    listen(server_socket, BACKLOG);
    struct sockaddr_in client_address;
    socklen_t client_address_len;
    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
    char recv_buffer[BUFFER], send_buffer[BUFFER];
    printf("Client connected\n");
    do
    {
        recv(client_socket, recv_buffer, BUFFER, 0);
        printf("%s\n", recv_buffer);
        printf("Reply:");
        fgets(send_buffer, BUFFER, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        send(client_socket, send_buffer, BUFFER, 0);
    }while(strcmp(recv_buffer, "exit") != 0);
    free(server_address);
    close(client_socket);
    close(server_socket);
    return 0;
    
}

