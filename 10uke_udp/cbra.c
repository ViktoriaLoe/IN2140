#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>

#define SERVER_PORT 80

#define BUFFER 4096
#define SA struct sockaddr



int main (int argc, char *argv )
{
    int socketFd, sendbytes;

    if (argc != 2) {
        printf("Useage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //creating stream socket, not datagram, TCP not UDP
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit(EXIT_FAILURE);


    //specify family and port 
    //convert to network 

    //translate text ip to binary with inet_pton


    //connect to server with connect 

    
}