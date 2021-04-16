#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 

struct RDP_connection {
    struct in_addr ip_adress;
    int connection_id;
} client;

//An overview over all active connection to different clients, needs to be dynamically allocated
struct RDP_connection **Active_connections;

int rdp_write()
{
    // When sending a packet, the sender has to be achknowledged

    // see RDP in HE1
}

struct RDP_connection rdp_accept()
{
    // Check if a new connection request has arrived

    //Accept it if there is,

    //and return malloc *RDP_connection 

    //else return NULL-pointer              How do I return NULL or struct

    
}

int main(int argc, char const *argv[])
{
    int socket_fd, N_of_files, port;
    struct sockaddr_in addr_con;
    FILE *file;


    if (argc < 5)
    {
        prinft("Usage: %s <port-number> <filename> <number-of-transerfer> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    N_of_files = argv[4];
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = argv[1];
    port = argv[1];

    //Handle loss probablity

    /* will receive some arguemtns from the commandline
        - UDP portnumber, that the application will receive packages on
        - filename of the file it will send to all connected clients
        - number N files that the server needs to send before it quits 
        - loss-probablity as a float
    */

   //socket?


   //bind?


   // send? file then an empty packet

   /* everytime a client connects to the server, the server respons with a package with flagg == 0x10 and prints the connection IDs
    format: CONNECTED <client-id> <server-id>
    if the server has already made N conenctiong and transfers, it will not accept and respond with a package flagg == 0x20 and prints
    format: NOT CONNECTED <client-id> <server-id>

    server quits after delivering N files
   */  
    //Establish client

    // MAIN LOOP

        //1. Check for new RDP connection requests


        //2. Try to deliver the 'next' packages to all connected RDP-clients


        //3. Check if an RDP-connection is closed???


        //4. if none of this happened, wait ~ either wait 1s or use select() to wait


    

}