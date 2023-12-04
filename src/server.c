#include "../utility/server_helper.h"



int main() 
  
{
    int server_socket, fifo_fd, max_fd, ready_select, addr_reuse_opt = 1;
    char recv_buffer[BUFFER], send_buffer[BUFFER], fifo_file[BUFFER];
    char command[COMMAND_LEN];
    struct sockaddr_in* server_address = NULL;
    sockfd_node *head_node = NULL, *temp=NULL;
    pid_t process_id;
    fd_set read_fds;
    regex_t regex;
    const char *pattern = "^remove [[:alnum:]_]+$";

    display_banner();

    // Compile the regex pattern
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        perror("Failed to compile regular expression\n");
        exit(EXIT_FAILURE);
    }

    // ### Create a new terminal for typing messages ###
    printf("Give a unique name for your TypeSpace:");
    fgets(fifo_file, sizeof(fifo_file)-1, stdin);
    fifo_file[strcspn(fifo_file, "\n")] = '\0';

    // Check if the file already exists
    if (access(fifo_file, F_OK) == -1) 
    {
        //If not make a new file giving read and write permissions to the user
        if(mkfifo(fifo_file, S_IRUSR | S_IWUSR) != 0)
        {
            perror("Error creating named TypeSpace");
            exit(EXIT_FAILURE);
        }
    }

    // ### Create a child process with process_id 0 to open a terminal without blocking the program ###
    process_id = fork();

    if(process_id<0)
    {
        perror("Error occured during fork()!\n");
        exit(EXIT_FAILURE);
    }

    else if(process_id == 0)
    {
        // ### Clild Process ###
        // Execute the type_space program in another terminal 
        strcpy(command, TERMINAL_EMULATOR );
        strcat(command, " ");
        strcat(command, FLAG_TO_EXECUTE);
        strcat(command, " ./type_space server ");
        strcat(command, fifo_file);
        system(command);
    }
    else
    {
        // ### Parent process ###
        // Open the fifo file
        if ((fifo_fd = open(fifo_file, O_RDONLY)) == -1) 
        {
            perror("Error occured during accessing TypeSpace.\nTerminate the TypeSpace program and run it again!\n");
            exit(EXIT_FAILURE);
        }
        printf("Connected to the TypeSpace!\n");

        // Create a socket to listen for clients
        if((server_socket = createTcpIp4Socket()) == -1)
        {
            perror("Error occured while creating the socket!\nTry again later!\n");
            exit(EXIT_FAILURE);
        }

        // Set the socket option to reuse the port
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (void *) &addr_reuse_opt, sizeof(addr_reuse_opt));

        // Create a address and bind to it to listen for clients
        server_address = setAddress("", PORT);
        if(bind(server_socket, (struct sockaddr*)server_address, sizeof(*server_address)) == -1)
        {
            perror("Error occured while binding the port!\nTry again later!\n");
            exit(EXIT_FAILURE);
        }

        listen(server_socket, BACKLOG);
    

        head_node = initialize(server_socket, *server_address); 
        

        // ### Use select() to forward the messages from TypeSpace and clients to every other clients ###
        while(1)
        {
            // Clear the read_fds set
            FD_ZERO(&read_fds);

            // Set fifo_fd and server_socket in read_fds
            FD_SET(server_socket, &read_fds);
            FD_SET(fifo_fd, &read_fds);

            // Set max value of file descriptor
            if(server_socket > fifo_fd) max_fd = server_socket;
            else max_fd = fifo_fd;
            temp = head_node->next;
        
            // Iterate every nodes to add the client's socket to the fd_set and update the max_fd
            while(temp != head_node)
            {
                if(temp->socket_val != 0)
                {
                    FD_SET(temp->socket_val, &read_fds);
                }
                if(temp->socket_val > max_fd) max_fd = temp->socket_val;
                temp = temp->next;
            }

            // select() removed the fd that are not ready to read from fd_set and return the status value
            ready_select = select(max_fd+1, &read_fds, NULL, NULL, NULL);

            if((ready_select < 0) && (errno!=EINTR))
            {
                perror("Error occoured during checking for ready fds to read!\n");
                exit(EXIT_FAILURE);
            }

            // If fifo file is ready to read, recieve from it and forward to all
            if(FD_ISSET(fifo_fd, &read_fds))
            {
                read(fifo_fd, send_buffer, BUFFER);
            
                if(strcmp(send_buffer, "exit") == 0) break;
                else if(strcmp(send_buffer, "remove all") == 0)
                {
                    remove_all_nodes(head_node);
                }
                else if(regexec(&regex, send_buffer, 0, NULL, 0) == 0)
                {
                    remove_client_by_name(head_node, strchr(send_buffer,' ')+1);
                }
                else send_to_all(head_node, head_node, send_buffer);
            
            }

            // Iterate all nodes to check if any client is sending messages and handle it
            temp = head_node;
            do
            {
                if(FD_ISSET(temp->socket_val, &read_fds))
                {
                    // Accepts new clients
                    if(temp->socket_val == server_socket)
                    {
                        accept_client(head_node);
                    }
                    else
                    {
                        recv(temp->socket_val, recv_buffer, BUFFER, 0);
                    
                        // Remove the client node if it want to exit and anounced in chatroom
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
        // Remove all nodes including server node
        clean_up(head_node);
        free(server_address);
        close(fifo_fd);
    }
    return 0;
    
}



