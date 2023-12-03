#include "../utility/helper.h"
//client need to send username after connecting
//It recieves msg with username at first
int main()
{
    int client_socket;
    int server_status;
    char server_ip[BUFFER];
    //size_t ip_len = 0;
    char send_buffer[BUFFER], recv_buffer[BUFFER];
    char fifo_file[BUFFER];
    int fifo_fd;
    printf("Give a unique name for your TypeSpace:");
    fgets(fifo_file, sizeof(fifo_file)-1, stdin);
    fifo_file[strcspn(fifo_file, "\n")] = '\0';
    if (access(fifo_file, F_OK) == -1) 
    {
        if(mkfifo(fifo_file, S_IRUSR | S_IWUSR) != 0)
        {
            perror("Error creating named TypeSpace");
            exit(EXIT_FAILURE);
        }
        //printf("Fifo file\n");
    }
    
    pid_t process_id = fork();
    //printf("Fork\n");
    if(process_id < 0)
    {
        perror("Error happened during fork()!\n");
        exit(EXIT_FAILURE);
    }
    else if(process_id == 0)
    {
        printf("New process\n");
        system(TERMINAL_EMULATOR " -- ./type_space");
    }
    else
    {
    //printf("%d\n",process_id);
    //fifo_fd = open(fifo_file, O_RDONLY );
    fifo_fd = open(fifo_file, O_RDONLY);
    printf("Connected to the TypeSpace!\n");
    fd_set read_fds;
    int ready_select,max_fd;
    max_fd = fifo_fd;
    fflush(stdout);
    //printf("dsfsdg");
    //fflush(stdout);
    
    //read(fifo_fd, server_ip, BUFFER);
    
    
    
    if((client_socket = createTcpIp4Socket()) == -1)
    {
        perror("Error occured while creating the socket!\nTry again later!\n");
        exit(EXIT_FAILURE);
    }
    fflush(stdout);
    printf("Enter the server ip: ");
    fflush(stdout);
    read(fifo_fd, server_ip, BUFFER);
    printf("%s\n", server_ip);
    fflush(stdout);
    /*while(1)
    {
        FD_ZERO(&read_fds);
        FD_SET(fifo_fd, &read_fds);
        //FD_SET(client_socket, &read_fds);
        ready_select = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if((ready_select < 0) && (errno!=EINTR))
        {
            perror("Error occoured during checking for ready fds to read!\n");
            exit(EXIT_FAILURE);
        }
        if(FD_ISSET(fifo_fd, &read_fds))
        {
            read(fifo_fd, server_ip, BUFFER);
            
            break;
        }
    }*/
    


    struct sockaddr_in* address = setAddress(server_ip, PORT);

    if(connect(client_socket, (struct sockaddr*)address, sizeof(*address)) == -1)
    {
        perror("Can't able to connect to the server!\nTry Again!\n");
        free(address);
        
        exit(EXIT_FAILURE);
    }
    //printf("Enter username:");
    //read(fifo_fd, send_buffer, BUFFER);
    
    //send(client_socket,send_buffer, BUFFER,0);
    
    printf("Enter the username: ");
    fflush(stdout);
    read(fifo_fd, send_buffer, BUFFER);
    send(client_socket,send_buffer, BUFFER,0);
    printf("%s\n",send_buffer);
    fflush(stdout);
    /*while(1)
    {
        FD_ZERO(&read_fds);
        FD_SET(fifo_fd, &read_fds);
        //FD_SET(client_socket, &read_fds);
        ready_select = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if((ready_select < 0) && (errno!=EINTR))
        {
            perror("Error occoured during checking for ready fds to read!\n");
            exit(EXIT_FAILURE);
        }
        if(FD_ISSET(fifo_fd, &read_fds))
        {
            read(fifo_fd, send_buffer, BUFFER);
            send(client_socket,send_buffer, BUFFER,0);
            break;
        }
    }*/
    if(client_socket > fifo_fd) max_fd = client_socket;
    else max_fd = fifo_fd;
    do
    {
        FD_ZERO(&read_fds);
        FD_SET(fifo_fd, &read_fds);
        FD_SET(client_socket, &read_fds);
        ready_select = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        if((ready_select < 0) && (errno!=EINTR))
        {
            perror("Error occoured during checking for ready fds to read!\n");
            exit(EXIT_FAILURE);
        }
        if(FD_ISSET(fifo_fd, &read_fds))
        {
            read(fifo_fd, send_buffer, BUFFER);
            send(client_socket,send_buffer, BUFFER,0);
            fflush(stdout);
            printf("You : %s\n",send_buffer);
            fflush(stdout);
        }
        if(FD_ISSET(client_socket, &read_fds))
        {
            /*server_status = recv(client_socket, NULL, 0, MSG_PEEK);
            if(server_status == 0) 
            {
                break;
            }*/
            recv(client_socket, recv_buffer, BUFFER, 0);
            fflush(stdout);
            printf("%s\n", recv_buffer);
            fflush(stdout);
            
            
        }
    }while(strcmp(send_buffer, "exit") != 0 && strcmp(recv_buffer,"ChatRoom is closed!") != 0);

    close(fifo_fd);
    free(address);
    //free(server_ip);
    close(client_socket);
    }
    return 0;

}

