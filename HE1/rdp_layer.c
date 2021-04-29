#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define BUFFER_SIZE 1000



void rdp_close(struct Packet *packet)
{

}

void rdp_read()
{
    
}
int rdp_write_server(struct sockaddr_in client_fd, char *output)
{

}

int rdp_write(struct sockaddr_in client_fd, char *output)
{
    // what should be sent and where as input
        //while (rc = fread())


       //else if the last packet it sent
        // send empty packe

    return 0;
}

int check_valid_id(int id)
{
    for (int i = 0; i > number_of_connections; i++)
    {
        if (active_connections[i]->connection_id == id) {

            fprintf(stderr, "[ERROR] ID TAKEN NOT CONNECTED %d %d\n", id);
            return 1;
        }
    }
    return 0;
}

struct rdp_connection* rdp_accept(struct Packet *client_con_packet, struct sockaddr_in addr_cli, int fd)
{

    int rc; // does this need to be an argument from the recvfrom?

    socklen_t sockaddr_size = sizeof(struct sockaddr);
    struct Packet *output_ack = malloc(sizeof(struct Packet *));

    /* Checking if ID is valid*/
    if (check_valid_id(client_con_packet->sender_id)) {
        output_ack = construct_packet(CONNECTION_DEN, 0, 0, 0, client_con_packet->sender_id, 0, 0);
        fprintf(stderr, "[ERROR] NOT CONENCTED %c\n", output_ack->flag);
        rc = sendto(fd, output_ack, sizeof(struct Packet), 0, (struct sockaddr *)&addr_cli, sockaddr_size);
        check_error(rc, "sendto");
        return NULL;
    }

    fprintf(stdout, "[INFO] CONNECTED %d %d\n", client_con_packet->sender_id, client_con_packet->recv_id);
    output_ack = construct_packet(CONNECTION_ACC, 0, 0, 0, client_con_packet->sender_id, 0, 0);

    // Sendign ack for pack
    rc = sendto(fd, output_ack, sizeof(struct Packet), 0, (struct sockaddr *)&addr_cli, sockaddr_size);
    check_error(rc, "sendto");

    /*Allocating space for conneciton datastructure*/
    struct rdp_connection *client = malloc(sizeof(struct rdp_connection));
    // Initializing datastructure
    active_connections[number_of_connections] = client; // Adding new connection to global array
    client->ip_adress = addr_cli;
    client->connection_id = client_con_packet->sender_id;
    number_of_connections++;
    fprintf(stdout,"[INFO] created rdp struct with id: %d\n", client->connection_id);

    return client;
}
