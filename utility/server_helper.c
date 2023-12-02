#include "server_helper.h"


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

sockfd_node* remove_client_by_soc(sockfd_node* server_node, int socket_val)
{
    if(socket_val == server_node->socket_val)
    {
        perror("Admin can't be removed!\n");
        return server_node;
    }
    sockfd_node *temp = server_node->next;
    char remove_msg[] = "Kicked Out By Admin!";
    while(temp != server_node)
    {
        if(temp->socket_val == socket_val)
        {
            (temp->pre)->next = temp->next;
            temp->next->pre = temp->pre;
            send(temp->socket_val, remove_msg, sizeof(remove_msg), 0);
            close(temp->socket_val);
            free(temp);
            break;
        }
        temp = temp->next;
    }
    return server_node;
}

sockfd_node* remove_client_by_name(sockfd_node* server_node, char *user_name)
{
    if(strcmp("Admin(*)", user_name)==0)
    {
        perror("Admin can't be removed!\n");
        return server_node;
    }
    sockfd_node *temp = server_node->next;
    char remove_msg[] = "Kicked Out By Admin!";
    while(temp != server_node)
    {
        if(strcmp(temp->user_name, user_name)==0)
        {
            (temp->pre)->next = temp->next;
            temp->next->pre = temp->pre;
            send(temp->socket_val, remove_msg, sizeof(remove_msg), 0);
            close(temp->socket_val);
            free(temp);
            break;
        }
        temp = temp->next;
    }
}

sockfd_node* remove_all_nodes(sockfd_node* server_node)
{
    sockfd_node *pre=NULL, *temp=server_node->next;
    char remove_msg[] = "Kicked Out By Admin!";
    while(temp!=server_node)
    {
        pre = temp;
        temp = temp->next;
        send(temp->socket_val, remove_msg, sizeof(remove_msg), 0);
        close(pre->socket_val);
        free(pre);
    }
    server_node->pre = server_node; 
    server_node->next = server_node;
}

void clean_up(sockfd_node* server_node)
{
    remove_all_nodes(server_node);
    close(server_node->socket_val);
    free(server_node);
}

