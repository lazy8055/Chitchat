#include "../utility/server_helper.h"



void send_to_all(sockfd_node *server_node, sockfd_node *sender, char message[])
{
    sockfd_node *temp = server_node->next;
    char send_buffer[BUFFER];
    strcpy(send_buffer, sender->user_name);
    strcat(send_buffer, ": ");
    strcat(send_buffer, message);
    printf("%s\n", send_buffer);
    do
    {
        if(temp->socket_val != sender->socket_val)
        {
            send(temp->socket_val, send_buffer, BUFFER, 0);
        }
        temp = temp->next;
    }while(temp != server_node);
}

void accept_client(sockfd_node *server_node)
{
    struct sockaddr_in client_address;
    socklen_t client_address_len;
    int client_socket = accept(server_node->socket_val, (struct sockaddr*)&client_address, &client_address_len);
    char recv_buffer[BUFFER];
    recv(client_socket, recv_buffer, BUFFER, 0);
    server_node = insert_client(server_node, client_socket,recv_buffer, client_address);
    char intro_massage[] = " joined the Chatroom!";
    //int result_len = strlen(intro_massage) + strlen(recv_buffer) + 1;
    char send_buffer[BUFFER];
    strcpy(send_buffer,recv_buffer);
    strcat(send_buffer, intro_massage);
    send_to_all(server_node, server_node, send_buffer);
    
}

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
    

    /*int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
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
    }while(strcmp(recv_buffer, "exit") != 0);*/
    sockfd_node* head_node = initialize(server_socket, *server_address), *temp=NULL;
    int active_connections = 1, max_fd;
    char recv_buffer[BUFFER], send_buffer[BUFFER];
    fd_set read_fds;
    while(1)
    {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        max_fd = server_socket;
        temp = head_node->next;
        //printf("while adding\n");
        while(temp != head_node)
        {
            if(temp->socket_val != 0)
            {
                FD_SET(temp->socket_val, &read_fds);
            }
            if(temp->socket_val > max_fd) max_fd = temp->socket_val;
            //printf("%d\n", temp->socket_val);
            temp = temp->next;
        }

        int ready_select = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if((ready_select < 0) && (errno!=EINTR))
        {
            perror("Error occoured during checking for ready fds to read!\n");
            exit(EXIT_FAILURE);
        }

        temp = head_node;
        do
        {
            if(FD_ISSET(temp->socket_val, &read_fds))
            {
                //printf("Set : %d\n", temp->socket_val);
                if(temp->socket_val == server_socket)
                {
                    accept_client(head_node);
                }
                else
                {
                    //What to do if client socket is ready to read
                    recv(temp->socket_val, recv_buffer, BUFFER, 0);
                    //printf("Msg recieved!\n");

                    if(strcmp(recv_buffer, "exit") == 0)
                    {
                        
                        strcpy(send_buffer, temp->user_name);
                        strcat(send_buffer, " exit the chatroom");
                        sockfd_node *next_node = temp->next;
                        head_node = remove_client_by_soc(head_node, temp->socket_val);
                        send_to_all(head_node, head_node, send_buffer);
                        recv_buffer[0] = '\0';
                        temp = next_node;
                        continue;
                    }
                    
                    send_to_all(head_node, temp, recv_buffer);

                }
            }
            temp = temp->next;
        }while(temp != head_node);
        

    }
    clean_up(head_node);

    free(server_address);
    //close(client_socket);
    //close(server_socket);
    return 0;
    
}



