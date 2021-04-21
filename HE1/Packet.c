#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include "send_packet.h"

int sequence_number = 100;

struct Packet
{
    unsigned char flag;
    unsigned char packet_seq;
    unsigned char ack_seq;
    //unused 
    int sender_id;
    int recv_id;
    int metadata; // if flag == 0c04 then metadata is length of packet + payload in bytes
    // if it's +x20 then metadata gives an itnerger value that indicates the reason for refusing the request
    char *payload; // metadata is size, only for flag == 0x04
};

void my_print_packet(struct Packet *p)
{
    unsigned char flag = p->flag;
    int sid = p->sender_id;
    int rid = p-> recv_id;

    if (flag & 0x01){ fprintf(stderr,"[INFO] This is a connect request, sender: %d receiver: %d\n", sid, rid);}
    if (flag & 0x02){ fprintf(stderr,"[INFO] This is a connection termination \n");}
    if (flag & 0x04){ fprintf(stderr,"[INFO] This packet contians data\n");}
    if (flag & 0x08){ fprintf(stderr,"[INFO] ACK from sender: %d\n", sid);}
    if (flag & 0x10){ fprintf(stderr,"[INFO] Accept connect request from sender: %d to: %d\n", sid, rid);}
    if (flag & 0x20){ fprintf(stderr,"[INFO] Denied connect request from sender: %d to: %d\n", sid, rid);} 
}


struct Packet* buffer_to_packet(char *buffer)
{
    //calls construct_packet to create packet out of input_buffer
}

char* my_packet_to_buffer(struct Packet *p)
{
    char *buffer;

    return buffer;
}

char *construct_packet(unsigned char flag, unsigned char pktseq, unsigned char ackseq, int sid, int rid, int meta, char *payload)
{
    //CHECK if packet is empty and handle this 
    // If we receive a packet where multiple bits in flags is one, discard it

    struct Packet *packet = malloc(sizeof(struct Packet));
    if (packet == NULL) { fprintf(stderr,"[ERROR] malloc failed\n");}

    packet->flag = flag;
    packet->packet_seq = pktseq;
    packet->ack_seq = ackseq;
    packet->sender_id = sid;
    packet->recv_id = rid;
    packet->metadata = meta;

    if (flag & 0x04) // Only case where payload is allowed
    {
        packet->payload = malloc(sizeof(char) * meta);
        packet->payload = payload;
    }

    char *packet_buffer = my_packet_to_buffer(packet);
    return packet_buffer;
}

