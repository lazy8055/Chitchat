#include "../utility/helper.h"


int main()
{
    char fifo_file[BUFFER];
    char send_buffer[BUFFER];
    printf("Enter the name of your TypeSpace:");
    fgets(fifo_file, sizeof(fifo_file)-1, stdin);
    fifo_file[strcspn(fifo_file, "\n")] = '\0';
    int fifo_fd = open(fifo_file, O_WRONLY );
    //write(fifo_fd, send_buffer, BUFFER);
    if (fifo_fd == -1) 
    {
        perror("Error occured during accessing TypeSpace.\nTerminate the main program and run it again!\n");
        exit(EXIT_FAILURE);
    }

    while(strcmp(send_buffer, "exit") != 0)
    {
        printf("Type here >>> ");
        fgets(send_buffer, BUFFER, stdin);
        send_buffer[strcspn(send_buffer, "\n")] = '\0';
        write(fifo_fd, send_buffer, BUFFER);
    }
    close(fifo_fd);
    return 0;
}