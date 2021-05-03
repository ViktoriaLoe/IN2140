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
    (void)packet;
}

void rdp_read()
{
    
}

int rdp_write_server(int socket_fd, char *output)
{
    int rc = 0;
    rc = sendto(socket_fd,                           /*socket*/
                output,                             /*message*/
                sizeof(output),                     /*amount of bytes*/
                0,                                  /*flags*/
                (struct sockaddr *)&server_fd_global,       /*IP and port*/
                sizeof(struct sockaddr_in));        /*size of addres-struct */

    return rc;
}

int rdp_write(struct rdp_connection *client, struct Packet *output)
{
    int rc = 0;
    fprintf(stdout,"[INFO] Attempting to write to client\n payload: %s\n", output->payload);

    char *output_buffer = malloc(sizeof(struct Packet));
    my_packet_to_buffer(output, output_buffer);

    /*Sending packet with file in it*/
    rc = sendto(client->client_socketFd, output_buffer,
        sizeof(output_buffer),
        0, (struct sockaddr *)&client->ip_adress, sizeof(struct sockaddr_in));
    check_error(rc, "sendto");

       //else if the last packet it sent
        // send empty packe
    free(client->previous_packet_sent);
    client->previous_packet_sent = output;
    return 0;
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

    int rc; // does this need to be an argument from the recvfrom?

    socklen_t sockaddr_size = sizeof(struct sockaddr);

    struct Packet output_packet = {CONNECTION_DEN, 0, 0, 0, client_con_packet->sender_id, 0, 0};
    /* Checking if ID is valid*/
    if (check_valid_id(client_con_packet->sender_id)) {
        fprintf(stderr, "[ERROR] NOT CONENCTED %c\n", client_con_packet->flag);
        rc = sendto(fd, &output_packet, sizeof(struct Packet), 0, (struct sockaddr *)&addr_cli, sockaddr_size);
        check_error(rc, "sendto");
        return NULL;
    }

    fprintf(stdout, "[SUCCESS] CONNECTED %d %d\n", client_con_packet->sender_id, client_con_packet->recv_id);
    output_packet.flag = CONNECTION_ACC; 

    // Sendign ack for pack
    rc = sendto(fd, &output_packet, sizeof(struct Packet), 0, (struct sockaddr *)&addr_cli, sockaddr_size);
    check_error(rc, "sendto");

    /*Allocating space for conneciton datastructure*/
    struct rdp_connection *client = malloc(sizeof(struct rdp_connection));
    // Initializing datastructure
    active_connections[number_of_connections] = client; // Adding new connection to global array
    client->ip_adress = addr_cli;
    client->connection_id = client_con_packet->sender_id;
    client->packet_seq = 0;
    client->client_socketFd = 0;
    client->previous_packet_sent = NULL;
    number_of_connections++;
    fprintf(stdout,"[INFO] created rdp struct with id: %d\n", client->connection_id);


    return client;
}
