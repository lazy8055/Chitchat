#include "../utility/helper.h"


int main(int argc, char const *argv[])
{
    int fifo_fd;
    char send_buffer[BUFFER], fifo_file[BUFFER] ;
    strcpy(fifo_file, argv[1]);

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

        write(fifo_fd, send_buffer, BUFFER);
    }

    //close the fifo file descriptor and delete the file
    close(fifo_fd);
    remove(fifo_file);
    
    return 0;
}