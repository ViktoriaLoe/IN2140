#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define BUFFER_SIZE 1024



void rdp_close(struct Packet *packet)
{
    (void)packet;
}

void rdp_read()
{
    
}

int rdp_write_server(int socket_fd, struct Packet *output)
{
    int rc = 0;
    output_buffer_c = my_packet_to_buffer(output);
    rc = sendto(socket_fd,                                  /*socket*/
                output_buffer_c,                             /*message*/
                sizeof(output_buffer),                          /*amount of bytes*/
                0,                                           /*flags*/
                (struct sockaddr *)&server_fd_global,       /*IP and port*/
                sizeof(struct sockaddr_in));                /*size of addres-struct */
    return rc;
}

int rdp_write(struct rdp_connection *client, struct Packet *output)
{
    int rc = 0;

    output_buffer = my_packet_to_buffer(output);

    /*Sending packet with file in it*/
    rc = sendto(client->client_socketFd, output_buffer,
        sizeof(struct Packet) + 1000,
        0, (struct sockaddr *)&client->ip_adress, sizeof(struct sockaddr_in));
        check_error(rc, "sendto");

       //else if the last packet it sent
        // send empty packe

    //free(client->previous_packet_sent);
    //client->previous_packet_sent = output;
    return rc;
}


int check_valid_id(int id)
{
    for (int i = 0; i < number_of_connections; i++)
    {
        if (active_connections[i]->connection_id == id) {

            fprintf(stderr, "[ERROR] ID TAKEN NOT CONNECTED %d \n", id);
            return 1;
        }
    }
    return 0;
}

struct rdp_connection* rdp_accept(struct Packet *client_con_packet, struct sockaddr_in addr_cli, int fd)
{

    int rc = 0; // does this need to be an argument from the recvfrom?
    socklen_t sockaddr_size = sizeof(struct sockaddr);

    /* Checking if ID is valid and if server takes more clients*/
    struct Packet output_packet = {CONNECTION_DEN, 0, 0, 0, 0, client_con_packet->sender_id, 50, 0};

    if (check_valid_id(client_con_packet->sender_id) || number_of_connections >= max_connections) {
        fprintf(stderr, "[ERROR] NOT CONENCTED %c\n", client_con_packet->flag);

        // data structure for this client isnt created, so to avoid sending a lot of input to rdp_write I'm sending it from this function
        rc = sendto(fd, &output_packet, sizeof(struct Packet), 0, (struct sockaddr *)&addr_cli, sockaddr_size);
            check_error(rc, "sendto");
        return NULL;
    }

    fprintf(stdout, "[SUCCESS] CONNECTED %d %d\n", client_con_packet->sender_id, client_con_packet->recv_id);
    output_packet.flag = CONNECTION_ACC; 

    /*Allocating space for conneciton datastructure*/
    struct rdp_connection *client = malloc(sizeof(struct rdp_connection));

    // Initializing datastructure
    active_connections[number_of_connections] = client; // Adding new connection to global array
    client->ip_adress = addr_cli;
    client->connection_id = client_con_packet->sender_id;
    client->packet_seq = 0;
    client->client_socketFd = fd;
    client->previous_packet_sent = NULL;
    fprintf(stdout,"[INFO] created rdp struct with id: %d\n", client->connection_id);

    // Sendign ack for pack
    rc = rdp_write(active_connections[number_of_connections], &output_packet);
        check_error(rc, "sendto");
        number_of_connections++;


    return client;
}
