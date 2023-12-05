#include "helper.h"

void display_banner()
{
    //char banner[] = "   ____ _     _ _    ____ _           _   \n"
                    "  / ___| |__ (_) |_ / ___| |__   __ _| |_ \n"
                    " | |   | '_ \\| | __| |   | '_ \\ / _` | __|\n"
                    " | |___| | | | | |_| |___| | | | (_| | |_ \n"
                    "  \\____|_| |_|_|\\__|\\____|_| |_|\\__,_|\\__|\n";
    //const char *banner =
        "                  ________    _ __  ________          __ \n"
        "                 / ____/ /_  (_) /_/ ____/ /_  ____ _/ /_\n"
        "                / /   / __ \\/ / __/ /   / __ \\/ __ `/ __/\n"
        "               / /___/ / / / / /_/ /___/ / / / /_/ / /_  \n"
        "               \\____/_/ /_/ /\\__/\\____/_/ /_/\\__,_/\\__/  \n";

    const char *banner = 
        "      ██████╗██╗  ██╗██╗████████╗ ██████╗██╗  ██╗ █████╗ ████████╗\n"
        "     ██╔════╝██║  ██║██║╚══██╔══╝██╔════╝██║  ██║██╔══██╗╚══██╔══╝\n"
        "     ██║     ███████║██║   ██║   ██║     ███████║███████║   ██║   \n"
        "     ██║     ██╔══██║██║   ██║   ██║     ██╔══██║██╔══██║   ██║   \n"
        "     ╚██████╗██║  ██║██║   ██║   ╚██████╗██║  ██║██║  ██║   ██║   \n"
        "      ╚═════╝╚═╝  ╚═╝╚═╝   ╚═╝    ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   \n";

    char colour[6][8] = {"\x1B[31m", "\x1B[32m", "\x1B[33m", "\x1B[34m", "\x1B[35m", "\x1B[36m"};

    //set the seed of the rand() with the time()
    srand(time(NULL));

    system("clear");
    for(int i=0; i<75; i++)
    {
        printf("=");
    }
    printf("\n\n");

    printf("%s%s%s%s\n",ANSI_BOLD, colour[rand()%6], banner, ANSI_RESET);
    
    for(int i=0; i<75; i++)
    {
        printf("=");
    }
    printf("\n\n");
}

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

void Perror(char error_message[])
{
    printf("%s%s",ANSI_BOLD, ANSI_RED);
    perror(error_message);
    printf("%s",ANSI_RESET);
}

// Create socket for TCP connection with IPv4
int createTcpIp4Socket()
{
    
    return socket(AF_INET, SOCK_STREAM, 0);
    
}

// Set address in sockaddr_in structure
struct sockaddr_in* setAddress(char *ip, int port)
{
    struct sockaddr_in *address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if(ip[0]=='\0')
    {
        address->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        if(inet_pton(AF_INET, ip, &address->sin_addr.s_addr) < 0)
        {
            Perror("Error occured during convertion of ip to binary\n");
            free(address);
            exit(EXIT_FAILURE);
        }
    }
    return address;
    
}

