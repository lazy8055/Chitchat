#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, char const *argv[])
{
    if(argc == 2)
    {
        // Execute Client file
        if(strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--client") == 0)
        {
            system("./client");
        }
        // Execute Server file
        else if(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--server") == 0)
        {
            system("./server");
        }
        else
        {
            perror("Invalid Argument passed!\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        perror("Invalid Argument passed!\n");
            exit(EXIT_FAILURE);
    }
    return 0;
}