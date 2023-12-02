#include "../utility/helper.h"
//client need to send username after connecting
//It recieves msg with username at first
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
    printf("Enter username:");
    fgets(send_buffer, BUFFER, stdin);
    send_buffer[strcspn(send_buffer, "\n")] = '\0';
    send(client_socket,send_buffer, BUFFER,0);


    /*do
    {
        printf("Enter a msg: ");
        fgets(send_buffer, BUFFER, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        send(client_socket,send_buffer, BUFFER,0);

        recv(client_socket, recv_buffer, BUFFER, 0);
        printf("%s\n", recv_buffer);
    }while(strcmp(send_buffer,"exit") != 0);*/
    fd_set read_fds;
    int ready_select, max_fd = client_socket;
    if((ready_select < 0) && (errno!=EINTR))
    {
        perror("Error occoured during checking for ready fds to read!\n");
        exit(EXIT_FAILURE);
    }
    do
    {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        FD_SET(client_socket, &read_fds);
        ready_select = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if(FD_ISSET(0, &read_fds))
        {
            fgets(send_buffer, BUFFER, stdin);
            send_buffer[strcspn(send_buffer, "\n")] = '\0';
            send(client_socket,send_buffer, BUFFER,0);
        }
        if(FD_ISSET(client_socket, &read_fds))
        {
            recv(client_socket, recv_buffer, BUFFER, 0);
            printf("%s\n", recv_buffer);
        }
    }while(strcmp(send_buffer, "exit") != 0);


    free(address);
    free(server_ip);
    close(client_socket);

    return 0;

}

