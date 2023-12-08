#include "../utility/helper.h"
#include <ctype.h>

int validate_username(char username[]);

int main()
{
    int client_socket, fifo_fd, ready_select, max_fd;
    char send_buffer[BUFFER], recv_buffer[BUFFER], server_ip[BUFFER], fifo_file[BUFFER];
    char command[COMMAND_LEN];
    bool exit_condition = false;
    pid_t process_id;
    fd_set read_fds;
    struct sockaddr_in* address = NULL;
    
    display_banner();

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
            Perror("Error creating named TypeSpace");
            exit(EXIT_FAILURE);
        }
    }
    
    // ### Create a child process with process_id 0 to open a terminal without blocking the program ###
    process_id = fork();
    
    if(process_id < 0)
    {
        Perror("Error happened during fork()!\n");
        exit(EXIT_FAILURE);
    }

    else if(process_id == 0)
    {
        // Execute the type_space program in another terminal
        strcpy(command, TERMINAL_EMULATOR );
        strcat(command, " ");
        strcat(command, FLAG_TO_EXECUTE);
        strcat(command, " ./type_space client ");
        strcat(command, fifo_file);
        system(command);
    }

    else
    {
        // Open the fifo file
        if ((fifo_fd = open(fifo_file, O_RDONLY)) == -1) 
        {
            Perror("Error occured during accessing TypeSpace.\nTerminate the TypeSpace program and run it again!\n");
            exit(EXIT_FAILURE);
        }
        printf("Connected to the TypeSpace!\n");


        
        // Create a socket to connect to the server
        if((client_socket = createTcpIp4Socket()) == -1)
        {
            Perror("Error occured while creating the socket!\nTry again later!\n");
            exit(EXIT_FAILURE);
        }
        
        printf("Enter the server ip: ");
        fflush(stdout);
        read(fifo_fd, server_ip, BUFFER);
        printf("%s%s%s%s\n",ANSI_BOLD ,ANSI_GREEN, server_ip, ANSI_RESET);
        fflush(stdout);
    
        // Set server address
        address = setAddress(server_ip, PORT);

        // Connect to the server
        if(connect(client_socket, (struct sockaddr*)address, sizeof(*address)) == -1)
        {
            Perror("Can't able to connect to the server!\nTry Again!\n");
            free(address);

            exit(EXIT_FAILURE);
        }
    
        // Get username and send to server
        do
        {
            do
            {
                printf("Enter the username: ");
                fflush(stdout);
                read(fifo_fd, send_buffer, BUFFER);
            }while(validate_username(send_buffer));
            send(client_socket,send_buffer, BUFFER,0);
            recv(client_socket,recv_buffer, 2, 0);
            if(strcmp(recv_buffer, "0") == 0)
            {
                printf("%s%sUsername already taken!\n%s", ANSI_BOLD, ANSI_RED, ANSI_RESET);
            }
            else break;
        }while(1);
        
        


        
        printf("%s%s%s%s\n\n",ANSI_BOLD ,ANSI_GREEN, send_buffer, ANSI_RESET);
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
                Perror("Error occoured during checking for ready fds to read!\n");
                exit(EXIT_FAILURE);
            }

            // Check if fifo_file is ready to read (Use send the message)
            if(FD_ISSET(fifo_fd, &read_fds))
            {
                read(fifo_fd, send_buffer, BUFFER);
                send(client_socket,send_buffer, BUFFER,0);
                //fflush(stdout);
                printf("%s%sYou %s: %s\n",ANSI_BOLD, ANSI_GREEN, ANSI_RESET, send_buffer);
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
            sleep(1);
            exit_condition = (strcmp(send_buffer, "exit") != 0 && strcmp(recv_buffer,ANSI_BOLD ANSI_RED "ChatRoom is closed!" ANSI_RESET) != 0 
            && strcmp(recv_buffer, ANSI_BOLD ANSI_RED "Kicked Out By Admin!"  ANSI_RESET) != 0);

        }while(exit_condition);

        // Close the opened file descriptors and free dynamically allocated memory
        close(fifo_fd);
        close(client_socket);
        free(address);
    }
    return 0;

}

// To validate the username if it only contains alphabets and numbers
int validate_username(char username[])
{
    int len = strlen(username);
    for(int i=0; i<len; i++)
    {
        if(!isalnum(username[i]))
        {
            printf("%s%s%s\nInvalid Username!\n%s", ANSI_BOLD, ANSI_RED,username, ANSI_RESET);
            return 1;
        }
    }
    return 0;
}

