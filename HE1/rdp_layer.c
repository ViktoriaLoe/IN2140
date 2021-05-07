#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "send_packet.h"

#define BUFFER_SIZE 1016


int find_connection_index(int sender_id)
{
    for (int index = 0; index < number_of_connections; index++) {
        if (active_connections[index]->connection_id == sender_id){
            return index;
        }
    }
    return -1;
}
void free_packet(struct Packet *pack)
{
    if (pack->payload != 0) {
        //free(pack->payload);
    }
    free(pack);
}

void rdp_close()
{
    for (int i = 0; i < number_of_connections; i++){
        free(active_connections[i]);
    }
    free(active_connections);

}


int rdp_write_server(int socket_fd, struct Packet *output)
{
    int rc = 0;
    char *output_buffer_c;
    printf("WRITING\n");
    output_buffer_c = my_packet_to_buffer(output);
    rc = send_packet(socket_fd,                                  /*socket*/
                output_buffer_c,                             /*message*/
                sizeof(output_buffer_c),                          /*amount of bytes*/
                0,                                           /*flags*/
                (struct sockaddr *)&server_fd_global,       /*IP and port*/
                sizeof(struct sockaddr_in));                /*size of addres-struct */
    
    free(output_buffer_c);
    return rc;
}


int rdp_write(struct rdp_connection *client, struct Packet *output)
{
    int rc = 0;

    printf("WRITING\n");
    print_packet(output);

    output_buffer = my_packet_to_buffer(output);

    /*Sending packet with file in it*/
    rc = send_packet(client->client_socketFd, output_buffer,
        BUFFER_SIZE,
        0, (struct sockaddr *)&client->ip_adress, sizeof(struct sockaddr_in));
        check_error(rc, "sendto");

       //else if the last packet it sent
        // send empty packe
    //free(output->payload);
   // free(output);
    free(output_buffer);
    return rc;
}

void rdp_send_file(struct Packet *input, struct rdp_connection *current_client)
{
    int rc = 0;
    char *output_buffer = calloc(1000, sizeof(char)); // this is free at end of 
    struct Packet *output_packet; //this is freeed in rdp_write

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

    printf(" file length %d bytes read  %d remaing bytes %d readingn from  %d\n",file_length, read, bytes_to_read, ack);
    //memcpy(output_buffer, file_buffer, bytes_to_read);

    rc = fread(output_buffer, sizeof(char), bytes_to_read, output_file);
        check_error(rc, "fread");

    if(bytes_to_read > 0) {
        fprintf(stdout,"[INFO] output_buffer paylaod is not empty \n");
        //uts(output_buffer);
    }

    output_buffer[rc] = '\0';
    current_client->bytes_read = read + bytes_to_read;

    output_packet = construct_packet(DATA_PACK, input->packet_seq, //freed in rdp_write
                        input->ack_seq, 0, ntohs(input->sender_id), 
                        bytes_to_read, output_buffer);

    /*Sending what is in buffer*/
    //free(current_client->previous_packet_sent->payload);
    current_client->previous_packet_sent = output_packet;
    rdp_write(current_client, output_packet);
    printf("Sent packet\n");
    free(output_packet);
    //free(input->payload);
    //free(input);
    //free(output_buffer);
}

int rdp_read_from_client(char *input_buffer)
{   
    struct Packet *input = malloc(sizeof(struct Packet ) + 2000); //freed at at end of this func or in rdp_send_file
    buffer_to_packet(input_buffer, input);
    print_packet(input);

    //2. Try to deliver the 'next' packages to all connected rdp-clients
        // We recevied an ACK and can start/proceed in sending file to client
    if (input->flag & ACK_PACK) {
        fprintf(stdout,"[INFO] Attempting to send file \n");

        // finding client we're sending file to
        struct rdp_connection *current_client;
        int index = find_connection_index(ntohs(input->sender_id));
        current_client = active_connections[index];
        current_client->packet_seq++; 
        rdp_send_file(input, current_client);
    }

    //3. Check if an rdp-connection is closed
    else if(input->flag & CONNECT_TERMINATE) {
        
        fprintf(stdout, "[INFO] DISCONNECTED \n");
        free_packet(input);
        rewind(output_file);
        return 1;
    }
    else {
        fprintf(stdout,"[INFO] flag INVALID: %d\n", input->flag);
    }
    free(input->payload);
    free(input);
    return 0;
}

//returns NULL if everything is handeled by the RDP layer, if not it return the packet to FSP_client to handle file transfer
struct Packet* rdp_read(char *input_buffer, struct Packet *ack_pack, int udpSocket_fd)
{   
    int rc = 0;
    struct Packet *input = malloc(sizeof(struct Packet)+1000);
    printf("INSIDE RDP_READ\n");
    buffer_to_packet(input_buffer, input);
    print_packet(input);

    if (input->flag & CONNECTION_ACC) {
        fprintf(stdout, "[SUCCESS] CONNECTED TO SERVER ID: %d meta %d\n", ntohs(input->recv_id) , input->metadata);
        free(input);
        return NULL;
    }

    if(input->flag & CONNECTION_DEN) {
        fprintf(stderr,"[ERROR] NOT CONNECTED TO SERVER\n");
        free(input);
        return NULL;
    }

    if (input->flag & DATA_PACK) {
        // Correct data packet received
        if (input->packet_seq == ack_pack->packet_seq) {
            fprintf(stdout, "[INFO] Received data pcket: meta %d\n",  input->metadata);
            return input;
        }
        else { // Old packet, sending ack again
        fprintf(stdout,"[INFO: WE GOT OLD DATA EW SENDING ACK AGAIn %d\n");
            rc = rdp_write_server(udpSocket_fd, ack_pack);
                check_error(rc, "sendto");
            free(input);
            return NULL;
        }

    } 
    // free(input);
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
    char input[BUFFER_SIZE];

   /* Peeking with recvfrom */ 
    rc = recvfrom(socket_fd, input, sizeof(struct Packet),
            MSG_PEEK, (struct sockaddr *)&addr_cli, &sockaddr_size);

    //Creating packets reading buffer and deserializing
    struct Packet *client_con_packet;
    client_con_packet = construct_packet(ACK_PACK, 0, 0, 0, ntohs(client_con_packet->sender_id), 0, 0);
    buffer_to_packet(input, client_con_packet);

    /* Checking if it is not a connection attempt*/
    if (!(client_con_packet->flag & CONNECT_REQ)) {
        printf("Not a connection request \n");
        free(client_con_packet->payload);
        free(client_con_packet);
        return NULL;
    }

    // It is a CONNECTION_REQ 
    struct Packet *output_pack;
    output_pack            = construct_packet(CONNECTION_DEN, 0, 0, 0, ntohs(client_con_packet->sender_id), 0, 0);

    /* Checking if ID is valid and if server takes more clients*/
    if (check_valid_id(ntohs(client_con_packet->sender_id)) || number_of_connections >= max_connections) {
        fprintf(stderr, "[ERROR] NOT CONENCTED %c\n", client_con_packet->flag);

        // Data structure for this client isnt created, so to avoid sending a lot of input to rdp_write I'm sending it from this function
        rc = send_packet(socket_fd, output_pack, sizeof(struct Packet), 0, (struct sockaddr *)&addr_cli, sockaddr_size);
            check_error(rc, "sendto");
            free_packet(output_pack);
            free_packet(client_con_packet);
            return NULL;
    }

    fprintf(stdout, "[SUCCESS] CONNECTED %d %d\n", (client_con_packet->sender_id), client_con_packet->recv_id);

    /*Allocating space for conneciton datastructure*/
    struct rdp_connection *client = malloc(sizeof(struct rdp_connection));

    // Initializing datastructure move this into a function
    active_connections[number_of_connections] = client; // Adding new connection to global array
    client->ip_adress = addr_cli;
    int id = ntohs(client_con_packet->sender_id);
    client->connection_id = id; 
    client->packet_seq = 0;
    client->bytes_read = 0;
    client->client_socketFd = socket_fd;
    client->previous_packet_sent = client_con_packet;
    fprintf(stdout,"[INFO] created rdp struct with id: %d %d\n", client->connection_id, id);

    // Sendign ack for pack
    output_pack->flag = CONNECTION_ACC; 
    rc = rdp_write(active_connections[number_of_connections], output_pack);
        number_of_connections++;

    rdp_send_file(client_con_packet, client);
    //free(output_pack);
    //free(client_con_packet->payload);
    free(client_con_packet);
    return client;
}
