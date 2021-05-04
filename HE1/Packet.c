#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include "send_packet.h"
#define BUFSIZE 1000

int sequence_number = 100;



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

    fprintf(stderr,"[INFO] pktseq %d, ackseq %d, sid %d, rid %d\n", p->packet_seq,p->ack_seq, p->sender_id, p->recv_id );
}
void print_packet(struct Packet *packet)
{   
    char buf[1000];
    fprintf(stdout,"[INFO] \n   id:%d %d\n   packet flag: %d \n  payload size %d payload %s\n", packet->recv_id, packet->sender_id, packet->flag, packet->metadata, packet->payload);
    memcpy(buf, packet + sizeof(struct Packet) - 8, packet->metadata);
    fprintf(stdout,"[INFO] packet: %s\n", buf);
    packet->payload = buf;
}

void buffer_to_packet(char *buffer, struct Packet *p)
{
    char buf[1000];
    printf("buffer %s\n", buffer + sizeof(struct Packet)-sizeof(char*));
    memcpy(buf, buffer + sizeof(struct Packet) - sizeof(char *), 1000);
    printf("buf %s\n", buf);
}

// Change to return buffer
void my_packet_to_buffer(struct Packet *p, char *buffer)
{
    fprintf(stdout,"[INFO] Attempting to write to client id: %d payload %s\n", p->recv_id, p->payload);
    //change into htons
    memcpy(buffer, p, sizeof(char)*4);
    // memcpy(buffer + 4, p->sender_id, sizeof(int));
    // memcpy(buffer + 8, p->recv_id, sizeof(int));
    // memcpy(buffer + 16, p->metadata, sizeof(int));
    memcpy(buffer + 4, p + 4, sizeof(int)*3);

    if (p->flag & DATA_PACK) {
        memcpy(buffer + sizeof(struct Packet) - 8, p->payload, BUFFER_SIZE);
    }
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

