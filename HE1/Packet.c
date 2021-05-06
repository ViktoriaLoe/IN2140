#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include "send_packet.h"
#define BUFSIZE 1000

int sequence_number = 100;


//prints content of packet
void print_packet(struct Packet *packet)
{   
    fprintf(stdout,"[INFO] \n |    id:%d %d\n |    pkt_s: %d\n |    packet flag: %d\n |    payload size %d \n", packet->recv_id, packet->sender_id,packet->packet_seq, packet->flag, packet->metadata);
    if (packet->flag  & DATA_PACK) {
        fprintf(stdout,"[INFO] packet: %s\n", packet->payload);
    }
}
//serializing
void buffer_to_packet(char *final_buffer, struct Packet *p)
{
    char buf[1000];
    memcpy(p, final_buffer,                          sizeof(struct Packet));
    if (p->flag & DATA_PACK) {
        memcpy(buf, final_buffer + sizeof(struct Packet),  p->metadata);
        p->payload = buf;
    }
    print_packet(p);
}

// Deseriaizing
char * my_packet_to_buffer(struct Packet *p )
{
    //change into htons
    char *final_buffer;

    if (p !=NULL && p->flag & DATA_PACK) {
        if (p->payload == NULL) {
            final_buffer = malloc(sizeof (struct Packet) + BUFFER_SIZE);
            memcpy(final_buffer, p, sizeof(struct Packet));
        }
        else {
            final_buffer = malloc(p->metadata + sizeof(struct Packet) + BUFFER_SIZE);
            memcpy(final_buffer, p,                              sizeof(struct Packet));
            memcpy(final_buffer + sizeof(struct Packet), p->payload,  p->metadata);
        }
    }
    else {
        if (p == NULL) {
            printf("P IS NUKLLL OG NOOOo\n");
        }
        final_buffer = malloc(sizeof(struct Packet));
        memcpy(final_buffer, p, sizeof(struct Packet));
    }
    fprintf(stdout,"\n[INFO] Sending packet! sending:\n");
    print_packet(p);
    return final_buffer;
}

struct Packet* construct_packet(unsigned char flag, unsigned char pktseq, unsigned char ackseq, int sid, int rid, int meta, char *payload)
{
    //CHECK if packet is empty and handle this 
    // If we receive a packet where multiple bits in flags is one, discard it

    struct Packet *packet = malloc(sizeof(struct Packet));
    if (packet == NULL) { fprintf(stderr,"[ERROR] malloc failed\n");}

    packet->flag = flag;
    packet->packet_seq = pktseq;
    packet->ack_seq = ackseq;
    packet->unused = 0;
    packet->sender_id = sid;
    packet->recv_id = rid;
    packet->metadata = meta;

    if (flag & DATA_PACK) // Only case where payload is allowed
    {
        packet->payload = malloc(sizeof(char) * meta);
        packet->payload = payload;
        fprintf(stdout,"[INFO] constructing packet! meta %d payload: %s\n",packet->metadata, packet->payload);
    }

    return packet;
}

