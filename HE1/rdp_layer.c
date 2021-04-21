#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define BUFFER_SIZE 1000


void rdp_close()
{

}

void rdp_read()
{

}

int rdp_write()
{
    // When sending a packet, the sender has to be achknowledged

    // see document for further instructions
    return 0;
}


struct rdp_connection* rdp_accept(struct Packet *packet, struct sockaddr_in addr_cli)
{
    // https://github.com/hzxie/Multiplex-Socket/blob/master/server.c see line 264
    // https://github.com/hzxie/Multiplex-Socket/blob/master/udp-client.c for client

    fprintf(stderr, "[INFO] CONNECTED %d %d\n", packet->sender_id, packet->recv_id);
    int rc; // does this need to be an argument from the recvfrom?
    char inputBuffer[BUFFER_SIZE] = {0};
    char outputBudder[BUFFER_SIZE] = {0};

    /* CLient socket*/
    socklen_t sockaddr_size = sizeof(struct sockaddr);
    struct sockaddr_in clientSocket_addr;
        //int client_fd[max_connections] = {0}; or use socket that i already bound
        //int *clientSocketFDs list of fds, all connections should have own fd

    /*Allocating space for conneciton datastructure*/
    struct rdp_connection *client = malloc(sizeof(struct rdp_connection));
        // Initializing datastructure
    active_connections[number_of_connections] = client; // Adding new connection to global array
    number_of_connections++;
    client->ip_adress = addr_cli;
        //check if id is valid?
    client->connection_id = packet->sender_id;

        // check number of sent/sending files

        //and return malloc *rdp_connection with random ID

    // send packet 0x10 == accept connect request

    //else return NULL-pointer              How do I return NULL or struct

    //return NULL-poitner
    return;
}

int main(void)
{

}