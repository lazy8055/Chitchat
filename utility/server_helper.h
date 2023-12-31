#ifndef SERVER_HELPER_H
#define SERVER_HELPER_H

#include "helper.h"

typedef struct sockfd_node sockfd_node;

// ### Implementation of circular doubly linked list ###

// Structure to hold socket value, username and address details of the clients
struct sockfd_node {
    int socket_val;
    char user_name[25];
    struct sockaddr_in sock_address;
    sockfd_node *pre;
    sockfd_node *next;
};


sockfd_node* initialize(int server_socket, struct sockaddr_in server_address);
sockfd_node* insert_client(sockfd_node* server_node, int client_socket,char* user_name, struct sockaddr_in client_address);
void send_to_all(sockfd_node *server_node, sockfd_node *sender, char message[]);
void accept_client(sockfd_node *server_node);
int is_username_already_used(sockfd_node *server_node, char username[]);
sockfd_node* remove_client_by_soc(sockfd_node* server_node, int socket_val);
sockfd_node* remove_client_by_name(sockfd_node* server_node, char *user_name);
sockfd_node* remove_all_nodes(sockfd_node* server_node);
void clean_up(sockfd_node* server_node);

#endif