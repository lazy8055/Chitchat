#include "../utility/helper.h"

void print_help(char role[])
{
    if(strcmp(role, "client") == 0)
    {
        printf("      help - To display the help menu\n");
        printf("      exit - To exit from the chatroom\n");
    }
    else if(strcmp(role, "server") == 0)
    {
        printf("      remove user_name - To remove a particular user from the chatroom\n");
        printf("      remove all       - To remove all users from the chatroom\n");
        printf("      help             - To display the help menu\n");
        printf("      exit             - To close the chatroom\n");
    }
}

int main(int argc, char const *argv[])
{
    int fifo_fd;
    char send_buffer[BUFFER], fifo_file[BUFFER], role[BUFFER] ;
    strcpy(fifo_file, argv[2]);
    strcpy(role, argv[1]);

    // Open fifo file created by server/client program to write only
    fifo_fd = open(fifo_file, O_WRONLY );
    
    if (fifo_fd == -1) 
    {
        perror("Error occured during accessing TypeSpace.\nTerminate the main program and run it again!\n");
        exit(EXIT_FAILURE);
    }

    // Get message from user and write to fifo file
    while(strcmp(send_buffer, "exit") != 0)
    {
        printf("Type here >>> ");
        fgets(send_buffer, BUFFER, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';

        if(strcmp(send_buffer, "help") == 0)
        {
            print_help(role);
            continue;
        }

        write(fifo_fd, send_buffer, BUFFER);
    }

    //close the fifo file descriptor and delete the file
    close(fifo_fd);
    remove(fifo_file);
    
    return 0;
}