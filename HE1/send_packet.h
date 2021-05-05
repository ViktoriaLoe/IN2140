#ifndef SEND_PACKET_H
#define SEND_PACKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <signal.h>
#include <poll.h>



#define BUFFER_SIZE         1024
#define CONNECT_REQ         0x01
#define CONNECT_TERMINATE   0x02
#define DATA_PACK           0x04
#define ACK_PACK            0x08
#define CONNECTION_ACC      0x10
#define CONNECTION_DEN      0x20

/*PACKET FILES*/
struct Packet
{
    unsigned char flag, packet_seq, ack_seq, unused;
    int sender_id, recv_id, metadata;
    // if flag == 0c04 then metadata is length of packet + payload in bytes
    // if it's +x20 then metadata gives an itnerger value that indicates the reason for refusing the request
    char *payload; // metadata is size, only for flag == 0x04
};


void print_packet(struct Packet *packet);
//Calls construct_packet to create packet out of input_buffer

void buffer_to_packet(char *buffer, struct Packet *p);
//Creates sendable packet to output_buffer
char *             my_packet_to_buffer(struct Packet *p );

//Constructs packet struct from arguements
struct Packet*          construct_packet(unsigned char flag, unsigned char pktseq, unsigned char ackseq, int sid, int rid, int meta, char *payload);



/* RDP FILES*/
struct rdp_connection { //add to header potentially 
    struct sockaddr_in ip_adress;   // what socket will bind to 
    int connection_id, packet_seq;              // used to identify client
    int client_socketFd;
    struct Packet *previous_packet_sent; // last packet sent to client, used when no ack is receieved in time 
};

void check_error(int ret, char *msg);

// Global variables
struct rdp_connection **active_connections;
struct sockaddr_in server_fd_global;
int number_of_connections;
int max_connections; 
char *output_buffer; 
char *output_buffer_c;

// accepts incoming network requests 
struct rdp_connection*  rdp_accept(struct Packet *packet, struct sockaddr_in addr_cli, int fd);

// converts packet into buffer and sends it to server 
int                     rdp_write_server(int socket_fd, struct Packet *output);

// converts packet into buffer and sends it to client
int rdp_write(struct rdp_connection *client_fd, struct Packet *output);

//
void                    rdp_read();

//
void                    rdp_close(struct Packet *packet);


// Extra functions
/* This function is used to set the probability (a value between 0 and 1) for
 * dropping a packet in the send_packet function. You call set_loss_probability
 * once in your program, and send_packet will drop packets after that.
 */
void        set_loss_probability( float x );

/* This is a lossy replacement for the sendto function. It uses a random
 * number generator to drop packets with the probability chosen with
 * set_loss_probability. If it doesn't drop the packet, it calls sendto.
 */
ssize_t     send_packet( int sock, const char* buffer, size_t size, int flags, const struct sockaddr* addr, socklen_t addrlen );

#endif /* SEND_PACKET_H */
