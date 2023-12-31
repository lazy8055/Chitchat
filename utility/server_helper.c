#include "server_helper.h"



// ### Implementation of circular doubly linked list ###

// Initialize the linked list and set server as head_node
sockfd_node* initialize(int server_socket, struct sockaddr_in server_address)
{
    sockfd_node* node = (sockfd_node*)malloc(sizeof(sockfd_node));

    node->socket_val = server_socket;
    strcpy(node->user_name, "Admin(*)");
    node->sock_address = server_address;
    node->pre = node;
    node->next = node;
    return node;
}

// Insert a client node before the server_node
sockfd_node* insert_client(sockfd_node* server_node, int client_socket,char* user_name, struct sockaddr_in client_address)
{
    sockfd_node* node = (sockfd_node*)malloc(sizeof(sockfd_node));

    node->socket_val = client_socket;
    strncpy(node->user_name, user_name, sizeof(node->user_name) - 1);
    node->user_name[sizeof(node->user_name) - 1] = '\0';
    node->sock_address = client_address;
    node->pre = server_node->pre;
    (server_node->pre)->next = node;
    node->next = server_node;
    server_node->pre = node;
    
    return server_node;
}

// Send the given message to all clients except the sender
void send_to_all(sockfd_node *server_node, sockfd_node *sender, char message[])
{
    char send_buffer[BUFFER];
    sockfd_node *temp = server_node->next;
    strcpy(send_buffer, ANSI_BOLD ANSI_BLUE);
    strcat(send_buffer, sender->user_name);
    strcat(send_buffer, ANSI_RESET);
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

// Accepts new clients and insert them in the linked list
void accept_client(sockfd_node *server_node)
{
    int client_socket;
    char user_name[BUFFER], send_buffer[BUFFER], intro_massage[] = " joined the Chatroom!";;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    //printf("Server: %d\n", server_node->socket_val);
    client_socket = accept(server_node->socket_val, (struct sockaddr*)&client_address, &client_address_len);
    if(client_socket < 0)
    {
        Perror("Error Occured During accepting the new client!\n");
        return;
    }
    //printf("User:%s\n",user_name);
    // Get username
    while(1)
    {
        recv(client_socket, user_name, BUFFER, 0);
        if(is_username_already_used(server_node, user_name))
        {
            send(client_socket, "0", 2, 0);
        }
        else
        {
            send(client_socket, "1", 2, 0);
            break;
        }
    }
    
    //printf("%d User:%s\n",client_socket, user_name);

    server_node = insert_client(server_node, client_socket,user_name, client_address);

    strcpy(send_buffer, ANSI_BOLD ANSI_CYAN);
    strcat(send_buffer,user_name);
    strcat(send_buffer, intro_massage);
    strcat(send_buffer, ANSI_RESET);
    

    // Send intro message to every clients
    send_to_all(server_node, server_node, send_buffer);
    
}

int is_username_already_used(sockfd_node* server_node, char username[])
{
    sockfd_node* temp = server_node->next;

    while(temp != server_node)
    {
        if(strcmp(temp->user_name, username) == 0) return 1;
        temp = temp->next;
    }
    return 0;
}

// Remove client node using socket value
sockfd_node* remove_client_by_soc(sockfd_node* server_node, int socket_val)
{
    if(socket_val == server_node->socket_val)
    {
        Perror("Admin can't be removed!\n");
        return server_node;
    }

    //char remove_msg[] = "Kicked Out By Admin!";
    sockfd_node *temp = server_node->next;
    
    while(temp != server_node)
    {
        if(temp->socket_val == socket_val)
        {
            (temp->pre)->next = temp->next;
            temp->next->pre = temp->pre;
            //send(temp->socket_val, remove_msg, sizeof(remove_msg), 0);
            close(temp->socket_val);
            free(temp);
            break;
        }
        temp = temp->next;
    }
    return server_node;
}

// Remove client node using user name
sockfd_node* remove_client_by_name(sockfd_node* server_node, char *user_name)
{
    if(strcmp("Admin(*)", user_name)==0)
    {
        Perror("Admin can't be removed!\n");
        return server_node;
    }
    char send_buffer[BUFFER];
    char remove_msg[BUFFER] = ANSI_BOLD ANSI_RED "Kicked Out By Admin!" ANSI_RESET;
    sockfd_node *temp = server_node->next;
    
    while(temp != server_node)
    {
        if(strcmp(temp->user_name, user_name)==0)
        {
            strcpy(send_buffer, ANSI_BOLD ANSI_RED);
            strcat(send_buffer, temp->user_name);
            strcat(send_buffer, " ");
            strcat(send_buffer, remove_msg);
            
            

            (temp->pre)->next = temp->next;
            temp->next->pre = temp->pre;
            send(temp->socket_val, remove_msg, sizeof(remove_msg), 0);
            close(temp->socket_val);
            free(temp);

            send_to_all(server_node, server_node, send_buffer);

            break;
        }
        temp = temp->next;
    }
}

// Remove all client nodes
sockfd_node* remove_all_nodes(sockfd_node* server_node)
{
    char remove_msg[] = ANSI_BOLD ANSI_RED "ChatRoom is closed!" ANSI_RESET;
    
    sockfd_node *pre=NULL, *temp=server_node->next;
    
    
    while(temp!=server_node)
    {
        pre = temp;
        temp = temp->next;
        send(pre->socket_val, remove_msg, sizeof(remove_msg), 0);
        close(pre->socket_val);
        free(pre);
    }
    server_node->pre = server_node; 
    server_node->next = server_node;

    printf("%s%s%s%s\n", ANSI_BOLD, ANSI_RED, "Removed all users!", ANSI_RESET);
}

// Remove all client nodes and server node
void clean_up(sockfd_node* server_node)
{
    remove_all_nodes(server_node);
    close(server_node->socket_val);
    free(server_node);
}

