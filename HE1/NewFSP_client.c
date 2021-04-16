#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, char const *argv[])
{

    int socket_fd, rc;
    struct sockaddr_in addr_con;
    FILE *file;

    if (argc < 4)
    {
        prinft("Usage: %s <server-ip> <port-number> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    addr_con.sin_family = AF_INET; //can it send to IPv6 as well? does it have to
    addr_con.sin_port = argv[3];
    addr_con.sin_addr.s_addr = argv[1];


    // will receive some arguemtns from the commandline
     //     - IP-adresser to the machine the applikation is running
     //     - UDP-port that's being used by the server
     //     - lossprobability, float between 0 and 1 (1 is always loss)

    //client calls set_loss_probablity(prob) at the start of the program

    /* It's the client that always initiates the connection 
    * we have to create multiple clients that all send packets to the server
    * The client then communicate with the server through packets, and act accordingly 
    * If a client sends 0x01, a new RDP connection needs to be established
    * On top of the existing UDP connection 
    */

    //client tries to connect to the server, if it doesnt respond within a minute it will exit and write an error.
    

    //when a connection is established, client prints connection-ID to both server and client
    
    

     //when package is recieved and saved -> the RDP-connection is closed and it prints the name of the file and quits
}