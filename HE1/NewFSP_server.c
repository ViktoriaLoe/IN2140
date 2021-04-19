#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define PORT 
#define BUFFER_SIZE 1000

//An overview over all active connection to different clients, needs to be dynamically allocated
struct rdp_connection **Active_connections;
int max_connections; 
int socket_fd;


struct rdp_connection { //add to header potentially 
    struct sockaddr ip_adress; // what socket will bind to 
    int connection_id;
    //current position within file
    unsigned char pktseq;
    int client_socketFd;
} client;

int rdp_write()
{
    // When sending a packet, the sender has to be achknowledged

    // see document for further instructions
    return 0;
}


struct rdp_connection rdp_accept()
{
    // https://github.com/hzxie/Multiplex-Socket/blob/master/server.c see line 264
    // https://github.com/hzxie/Multiplex-Socket/blob/master/udp-client.c for client
    int rc;
    char inputBuffer[BUFFER_SIZE] = {0};
    char outputBudder[BUFFER_SIZE] = {0};

   /* everytime a client connects to the server, 
    the server respons with a package with flagg == 0x10 and prints the connection IDs
        format: CONNECTED <client-id> <server-id>
    if the server has already made N conenctiong and transfers,
    it will not accept and respond with a package flagg == 0x20 and prints
        format: NOT CONNECTED <client-id> <server-id>
   */  

    /* CLient socket*/
    socklen_t sockaddr_size = sizeof(struct sockaddr);
    struct sockaddr_in clientSocket_addr;
    //int client_fd[max_connections] = {0}; or use socket that i already bound
    //int *clientSocketFDs list of fds, all connections should have own fd

    // Check if a new connection request has arrived

        //If there is; 
          // read packet with recvfrom

            //create connection with id

                // check if ID is valid/taken
        
            // check number of sent/sending files

            //create datastructure

            //and return malloc *rdp_connection with random ID

        //else return NULL-pointer              How do I return NULL or struct

    //return NULL-poitner
    return;
}

void checkError(int res, char *msg)
 {
     if (res == -1) 
     {
        perror(msg);
        // Rydde?
        exit(EXIT_FAILURE);
     }
}

int main(int argc, char const *argv[])
{
    /*Variables*/ 
    int port, rc;
    struct sockaddr_in addr_con;
    const char *filename;
    FILE *file;

    /*Check if right amount of arguments are provided */
    if (argc < 5)
    {
        printf("Usage: %s <port-number> <filename> <number-of-transerfer> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }
    // Check for right format in arguments

   /* Socket to receive datagrams with IPv4*/
        // There needs to be multiple sockets later on, one for each client
        // The data needs to be fragmented, so it can send large files with UDP
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    checkError(socket_fd, "socket");        

    /*Assigning input variables, listening for all adresses*/
    port = atoi(argv[1]);
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = port;
    addr_con.sin_addr.s_addr = INADDR_ANY;

    filename = argv[2];
    max_connections = atoi(argv[3]);
    set_loss_probability(atof(argv[4]));


   /* Socket will listen for adress we assigned (local host)*/
    rc = bind(socket_fd, (struct sockaddr *)&addr_con, sizeof(struct sockaddr_in));
    checkError(rc, "bind");

    //Establish client

    // MAIN LOOP
    while (0)
    {
        int action = 0;
        //1. Check for new rdp connection requests
        rdp_accept();

        //2. Try to deliver the 'next' packages to all connected rdp-clients
        rdp_write();

        //3. Check if an rdp-connection is closed???
        //listen

        //4. if none of this happened, wait ~ either wait 1s or use select() to wait
        if (!action)
        {
            //wait
        }

    }

    close(socket_fd);
    return EXIT_SUCCESS;

}