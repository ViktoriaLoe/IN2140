#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include "send_packet.h"

int sequence_number = 100;

void my_print_packet(struct Packet *packet)
{
    unsigned char flag = packet->flag;
    int sid = packet->sender_id;
    int rid = packet-> recv_id;

    if (flag & 0x01){ fprintf(stderr,"[INFO] This is a connect request, sender: %d receiver: %d\n", sid, rid);}
    if (flag & 0x02){ fprintf(stderr,"[INFO] This is a connection termination \n");}
    if (flag & 0x04){ fprintf(stderr,"[INFO] This packet contians data\n");}
    if (flag & 0x08){ fprintf(stderr,"[INFO] ACK from sender: %d\n", sid);}
    if (flag & 0x10){ fprintf(stderr,"[INFO] Accept connect request from sender: %d to: %d\n", sid, rid);}
    if (flag & 0x20){ fprintf(stderr,"[INFO] Denied connect request from sender: %d to: %d\n", sid, rid);} 
}


void buffer_to_packet(char *buffer)
{
    //calls construct_packet to create packet out of input_buffer
}

char* my_packet_to_buffer(struct Packet *packet)
{
    char *buffer;

    return buffer;
}

char *construct_packet(unsigned char flag, unsigned char pktseq, unsigned char ackseq, int sid, int rid, int meta, char *payload)
{
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

    char *packet_buffer = packet_to_buffer(packet);
    return packet_buffer;
}

