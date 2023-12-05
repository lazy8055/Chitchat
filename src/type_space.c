#include "../utility/helper.h"
#include <signal.h>

int fifo_fd;
char fifo_file[BUFFER];

void dispose(int signal)
{
    //close the fifo file descriptor and delete the file
    char remove_command[COMMAND_LEN] = "rm ";
    strcat(remove_command, fifo_file);
    system(remove_command);
    close(fifo_fd);
}

int main(int argc, char const *argv[])
{
    char send_buffer[BUFFER], role[BUFFER] ;
    strcpy(fifo_file, argv[2]);
    strcpy(role, argv[1]);

    signal(SIGPIPE, dispose);

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

    dispose(0);
    
    return 0;
}