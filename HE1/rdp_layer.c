#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define BUFFER_SIZE 1024


int find_connection_index(int sender_id)
{
    for (int index = 0; index < number_of_connections; index++) {
        if (active_connections[index]->connection_id == sender_id){
            return index;
        }
    }
    return -1;
}

void rdp_close(struct Packet *packet)
{
    (void)packet;
}


int rdp_write_server(int socket_fd, struct Packet *output)
{
    int rc = 0;
    printf("WRITING\n");
    output_buffer_c = my_packet_to_buffer(output);
    rc = sendto(socket_fd,                                  /*socket*/
                output_buffer_c,                             /*message*/
                sizeof(output_buffer_c),                          /*amount of bytes*/
                0,                                           /*flags*/
                (struct sockaddr *)&server_fd_global,       /*IP and port*/
                sizeof(struct sockaddr_in));                /*size of addres-struct */
    return rc;
}


int rdp_write(struct rdp_connection *client, struct Packet *output)
{
    int rc = 0;

    char *output_buffer_send = malloc (sizeof(struct Packet)+BUFFER_SIZE); 

    printf("WRITING\n");
    output_buffer_send = my_packet_to_buffer(output);

    /*Sending packet with file in it*/
    rc = sendto(client->client_socketFd, output_buffer_send,
        sizeof(struct Packet) + 1000,
        0, (struct sockaddr *)&client->ip_adress, sizeof(struct sockaddr_in));
        check_error(rc, "sendto");

       //else if the last packet it sent
        // send empty packe

    //free(output_buffer_send);
    //free(client->previous_packet_sent);
    //client->previous_packet_sent = output;
    return rc;
}

void rdp_send_file(struct Packet *input, struct rdp_connection *current_client)
{
    char *output_buffer = malloc(sizeof(char)*1000);
    struct Packet *output_packet = malloc(sizeof(struct Packet *));

    /*Checking if it is not the correct ACK sequence*/
    if (current_client->previous_packet_sent != NULL &&
         input->packet_seq < current_client->packet_seq ) {

        //sending the previously sent packet
        rdp_write(current_client, current_client->previous_packet_sent);
    }
    int read = current_client->bytes_read;
    int ack = input->packet_seq*999; // How many bytes we have already read
    int bytes_to_read = 999;
   
    if (read  + 999 > file_length){
        bytes_to_read = file_length - read;
    }

    printf(" bytes read  %d remaing bytes %d readingn from  %d\n", read, bytes_to_read, ack);
    memcpy(output_buffer, file_buffer+read, 999);
    if(bytes_to_read > 0) {
        fprintf(stdout,"[INFO] output_buffer paylaod is not empty\n");
        //puts(output_buffer);
    }
    printf("read %d bytes\n", bytes_to_read);
    current_client->bytes_read = read + bytes_to_read;
    output_packet = construct_packet(DATA_PACK, input->ack_seq, 
                        current_client->packet_seq, 0, input->sender_id, 
                        strlen(output_buffer), output_buffer);


    /*Sending what is in buffer*/
    current_client->previous_packet_sent = output_packet;
    rdp_write(current_client, output_packet);
    printf("Sent packet\n");
    //free(output_packet);
}

void rdp_read_from_client(char *input_buffer)
{   
    struct Packet *input = malloc(sizeof(struct Packet ) + 2000);
    buffer_to_packet(input_buffer, input);

    //2. Try to deliver the 'next' packages to all connected rdp-clients
        // We recevied an ACK and can start/proceed in sending file to client
    if (input->flag & ACK_PACK) {
        fprintf(stdout,"[INFO] Attempting to send file \n");

        // finding client we're sending file to
        struct rdp_connection *current_client;
        int index = find_connection_index(input->sender_id);
        current_client = active_connections[index];
        
        rdp_send_file(input, current_client);
    }

    //3. Check if an rdp-connection is closed
    else if(input->flag & CONNECT_TERMINATE) {
        // free rdp_connection
        fprintf(stdout, "[INFO] DISCONNECTED \n");
        rdp_close(input); // closes connection removes from array and frees space
    }
    else {
        fprintf(stdout,"[INFO] flag INVALID: %d\n", input->flag);
    }
    
}

//returns NULL if everything is handeled by the RDP layer, if not it return the packet to FSP_client to handle file transfer
struct Packet* rdp_read(char *input_buffer, struct Packet *ack_pack, int udpSocket_fd)
{   
    int rc = 0;
    struct Packet *input = malloc(sizeof(struct Packet ) + 2000);
    printf("INSIDE RDP_READ\n");
    buffer_to_packet(input_buffer, input);

    if (input->flag & CONNECTION_ACC) {
        fprintf(stdout, "[SUCCESS] CONNECTED TO SERVER ID: %d meta %d\n", input->recv_id , input->metadata);
        return NULL;
    }

    if(input->flag & CONNECTION_DEN) {
        fprintf(stderr,"[ERROR] NOT CONNECTED TO SERVER\n");
        return NULL;
    }

    if (input->flag & DATA_PACK) {
        // Correct data packet received
        if (input->packet_seq == ack_pack->packet_seq) {
            fprintf(stdout, "[INFO] Received data pcket: meta %d\n",  input->metadata);
            return input;
        }
        else { // Old packet, sending ack again
            rc = rdp_write_server(udpSocket_fd, ack_pack);
                check_error(rc, "sendto");
            return NULL;
        }

    } 
    return NULL;
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


struct rdp_connection* rdp_accept(int socket_fd, struct sockaddr_in addr_cli)
{
    /* Initializing variables */
    int rc = 0; 
    socklen_t sockaddr_size = sizeof(struct sockaddr);
    char input[sizeof(struct Packet)];

   /* Peeking with recvfrom */ 
    rc = recvfrom(socket_fd, input, sizeof(struct Packet),
            MSG_PEEK, (struct sockaddr *)&addr_cli, &sockaddr_size);

    //Creating packets reading buffer and deserializing
    struct Packet* client_con_packet =  malloc(sizeof(struct Packet)+BUFFER_SIZE);
    client_con_packet = construct_packet(ACK_PACK, 0, 0, 0, client_con_packet->sender_id, 0, 0);
    buffer_to_packet(input, client_con_packet);

    /* Checking if it is not a connection attempt*/
    if (!(client_con_packet->flag & CONNECT_REQ)) {
        printf("Not a connection request \n");
        return NULL;
    }

    // It is a CONNECTION_REQ 
    struct Packet *output_pack             = malloc(sizeof(struct Packet)+BUFFER_SIZE);
    output_pack            = construct_packet(CONNECTION_DEN, 0, 0, 0, client_con_packet->sender_id, 0, 0);

    /* Checking if ID is valid and if server takes more clients*/
    if (check_valid_id(client_con_packet->sender_id) || number_of_connections >= max_connections) {
        fprintf(stderr, "[ERROR] NOT CONENCTED %c\n", client_con_packet->flag);

        // Data structure for this client isnt created, so to avoid sending a lot of input to rdp_write I'm sending it from this function
        rc = sendto(socket_fd, output_pack, sizeof(struct Packet), 0, (struct sockaddr *)&addr_cli, sockaddr_size);
            check_error(rc, "sendto");
            return NULL;
    }

    fprintf(stdout, "[SUCCESS] CONNECTED %d %d\n", client_con_packet->sender_id, client_con_packet->recv_id);

    /*Allocating space for conneciton datastructure*/
    struct rdp_connection *client = malloc(sizeof(struct rdp_connection));

    // Initializing datastructure move this into a function
    active_connections[number_of_connections] = client; // Adding new connection to global array
    client->ip_adress = addr_cli;
    client->connection_id = client_con_packet->sender_id;
    client->packet_seq = 0;
    client->bytes_read = 0;
    client->client_socketFd = socket_fd;
    client->previous_packet_sent = NULL;
    fprintf(stdout,"[INFO] created rdp struct with id: %d\n", client->connection_id);

    // Sendign ack for pack
    output_pack->flag = CONNECTION_ACC; 
    rc = rdp_write(active_connections[number_of_connections], output_pack);
        number_of_connections++;

    rdp_send_file(client_con_packet, client);
    return client;
}
