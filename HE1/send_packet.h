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



/*PACKET FILES*/
typedef struct Packet
{
    unsigned char flag, packet_seq, ack_seq;
    //unused 
    int sender_id, recv_id, metadata;
    // if flag == 0c04 then metadata is length of packet + payload in bytes
    // if it's +x20 then metadata gives an itnerger value that indicates the reason for refusing the request
    char *payload; // metadata is size, only for flag == 0x04
} Packet;

//Prints out meaning of files in buffer
void            my_print_packet(struct Packet *p);

//Calls construct_packet to create packet out of input_buffer
struct Packet*  buffer_to_packet(char *buffer);

//Creates sendable packet to output_buffer
char*           my_packet_to_buffer(struct Packet *p);

//Constructs packet struct from arguements
struct Packet*           construct_packet(unsigned char flag, unsigned char pktseq, unsigned char ackseq, int sid, int rid, int meta, char *payload);



/* RDP FILES*/
struct rdp_connection { //add to header potentially 
    struct sockaddr_in ip_adress;   // what socket will bind to 
    int connection_id;              // used to identify client
    unsigned char pktseq;           // bytes sent 
    unsigned char status;           // used to identify if last action was ACKed
    int client_socketFd;
};

struct rdp_connection **active_connections;
int number_of_connections;
int max_connections; 

// accepts incoming network requests 
struct rdp_connection*  rdp_accept(struct Packet *packet, struct sockaddr_in addr_cli);

// writes to receiver 
int                     rdp_write();

void                    rdp_read();

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
