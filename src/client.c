#include "../utility/helper.h"

int main()
{
    int client_socket, fifo_fd, ready_select, max_fd;
    char send_buffer[BUFFER], recv_buffer[BUFFER], server_ip[BUFFER], fifo_file[BUFFER];
    pid_t process_id;
    fd_set read_fds;
    struct sockaddr_in* address = NULL;
    
    // ### Create a new terminal for typing messages ###
    printf("Give a unique name for your TypeSpace:");
    fgets(fifo_file, sizeof(fifo_file), stdin);
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
    
    if(process_id < 0)
    {
        perror("Error happened during fork()!\n");
        exit(EXIT_FAILURE);
    }

    else if(process_id == 0)
    {
        // Execute the type_space program in another terminal
        system(TERMINAL_EMULATOR " -- ./type_space");
    }

    else
    {
        // Open the fifo file
        if ((fifo_fd = open(fifo_file, O_RDONLY)) == -1) 
        {
            perror("Error occured during accessing TypeSpace.\nTerminate the TypeSpace program and run it again!\n");
            exit(EXIT_FAILURE);
        }
        printf("Connected to the TypeSpace!\n");


        
        // Create a socket to connect to the server
        if((client_socket = createTcpIp4Socket()) == -1)
        {
            perror("Error occured while creating the socket!\nTry again later!\n");
            exit(EXIT_FAILURE);
        }
        
        printf("Enter the server ip: ");
        fflush(stdout);
        read(fifo_fd, server_ip, BUFFER);
        printf("%s\n", server_ip);
        fflush(stdout);
    
        // Set server address
        address = setAddress(server_ip, PORT);

        // Connect to the server
        if(connect(client_socket, (struct sockaddr*)address, sizeof(*address)) == -1)
        {
            perror("Can't able to connect to the server!\nTry Again!\n");
            free(address);

            exit(EXIT_FAILURE);
        }
    
        // Get username and send to server
        printf("Enter the username: ");
        fflush(stdout);
        read(fifo_fd, send_buffer, BUFFER);
        send(client_socket,send_buffer, BUFFER,0);
        printf("%s\n",send_buffer);
        fflush(stdout);
    
        if(client_socket > fifo_fd) max_fd = client_socket;
        else max_fd = fifo_fd;

        // ### Use select() to forward the messages from TypeSpace terminal to the server ###
        do
        {
            // Clear the read_fds set
            FD_ZERO(&read_fds);

            // Set fifo_fd and client socket in read_fds
            FD_SET(fifo_fd, &read_fds);
            FD_SET(client_socket, &read_fds);

            // select() removed the fd that are not ready to read from fd_set and return the status value
            ready_select = select(max_fd+1, &read_fds, NULL, NULL, NULL);

            if((ready_select < 0) && (errno!=EINTR))
            {
                perror("Error occoured during checking for ready fds to read!\n");
                exit(EXIT_FAILURE);
            }

            // Check if fifo_file is ready to read (Use send the message)
            if(FD_ISSET(fifo_fd, &read_fds))
            {
                read(fifo_fd, send_buffer, BUFFER);
                send(client_socket,send_buffer, BUFFER,0);
                //fflush(stdout);
                printf("You : %s\n",send_buffer);
                fflush(stdout);
            }

            // Check if the server sending any messages
            if(FD_ISSET(client_socket, &read_fds))
            {
                recv(client_socket, recv_buffer, BUFFER, 0);
                //fflush(stdout);
                printf("%s\n", recv_buffer);
                fflush(stdout);
            }

        }while(strcmp(send_buffer, "exit") != 0 && strcmp(recv_buffer,"ChatRoom is closed!") != 0);

        // Close the opened file descriptors and free dynamically allocated memory
        close(fifo_fd);
        close(client_socket);
        free(address);
    }
    return 0;

}

