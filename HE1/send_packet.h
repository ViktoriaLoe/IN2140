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

char *packet_to_buffer(struct Packet *packet)
{

}

char * construct_packet()
{
    struct Packet buffer;
    char *packet_buffer = packet_to_buffer(&buffer);


    return packet_buffer;
}

typedef struct Packet
{
    unsigned char flag;
    unsigned char packet_seq;
    unsigned char ack_seq;
    //unused 
    int sender_id;
    int recv_id;
    int metadata; // if flag == 0c04 then metadata is length of packet + payload in bytes
    // if it's +x20 then metadata gives an itnerger value that indicates the reason for refusing the request
    char payload[1000]; // metadata is size, only for flag == 0x04
} Packet;

/* This function is used to set the probability (a value between 0 and 1) for
 * dropping a packet in the send_packet function. You call set_loss_probability
 * once in your program, and send_packet will drop packets after that.
 */
void set_loss_probability( float x );

/* This is a lossy replacement for the sendto function. It uses a random
 * number generator to drop packets with the probability chosen with
 * set_loss_probability. If it doesn't drop the packet, it calls sendto.
 */
ssize_t send_packet( int sock, const char* buffer, size_t size, int flags, const struct sockaddr* addr, socklen_t addrlen );

#endif /* SEND_PACKET_H */
