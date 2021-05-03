#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "send_packet.h"
#include <time.h>


#define BUFFER_SIZE 1000



void check_error(int ret, char *msg) {
    if (ret == -1 ) {
        fprintf(stderr, "[ERROR] An error has occurred attempting function: %s\n", msg);
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    /*Variables*/ 
    int udpSocket_fd, rc;
    fd_set server_fd_set;
    struct sockaddr_in server_fd;
    int id;
    char filename[20];
    FILE *filep;


    if (argc < 4)
    {
        fprintf(stderr, "[ERROR]Usage: %s <server-ip> <port-number> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }


    /*Assigning input variables*/ 
    socklen_t sockaddr_size = sizeof(struct sockaddr);
    server_fd.sin_family = AF_INET; //can it send to IPv6 as well? does it have to
    server_fd.sin_port = htons(atoi(argv[2]));
    server_fd.sin_addr.s_addr = inet_addr(argv[1]);
    //set_loss_probability(atof(argv[3]));
    server_fd_global = server_fd;
    srand(time(0));
    id = rand() % 1000;

    /* Socket to send datagrams with IPv4*/
    udpSocket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(udpSocket_fd, "socket");

    /*Making packets to send*/
    struct Packet *connection_attempt   = malloc(sizeof(struct Packet *));
    connection_attempt  = construct_packet(CONNECT_REQ, 1, 1, id, 200, 0, 0);

    /* Sending packet using rdp_write*/
    char *buffer = malloc(sizeof(struct Packet)+10);
    my_packet_to_buffer(connection_attempt, buffer);
    
    rc = rdp_write_server(udpSocket_fd, buffer);
    check_error(rc, "sendto");
    fprintf(stderr, "[INFO] Sent %d bytes\n", rc);
    
    /* Receive answer for connection request */
    struct Packet *input = malloc(sizeof(struct Packet *));
    rc = recvfrom(udpSocket_fd, input, BUFFER_SIZE, 
                0, (struct sockaddr*)&server_fd, &sockaddr_size);
        check_error(rc, "recvfrom");


    //Creating ack_packet
    struct Packet *ack_pack             = malloc(sizeof(struct Packet *));
    ack_pack            = construct_packet(ACK_PACK, 0, 0, id, 200, 0, 0);
    char *buffer_ack = malloc(sizeof(struct Packet));
    my_packet_to_buffer(ack_pack, buffer_ack);

    //Received accept, send ack
    if (input->flag & CONNECTION_ACC) { 
        fprintf(stdout, "[SUCCESS] CONNECTED TO SERVER ID: %d. sending ack\n", id);

    // dont think I need to send this ack!
        rc = rdp_write_server(udpSocket_fd, buffer_ack);
        check_error(rc, "sendto");
    }
    else {
        fprintf(stderr," [ERROR]: NOT CONNECTED id: %d\n", id);
        return EXIT_FAILURE;
    }
    /*Opening file in write-mode*/
    snprintf(filename, 28, "kernal_file_%d.txt", id %20);
    filep = fopen(filename, "w+");
    //check_error(filep, "fopen");

    /* Main loop for file receiving (select timeout if nothing is recevied ) */
    do {
        FD_ZERO(&server_fd_set);
        FD_SET(udpSocket_fd, &server_fd_set);

        struct timeval tv = {100, 0};
        select(FD_SETSIZE + 1, &server_fd_set, NULL, NULL, &tv);

        if (FD_ISSET(udpSocket_fd, &server_fd_set)) {
            // FD is used, recevie input buffer
            rc = recvfrom(udpSocket_fd, input, BUFFER_SIZE, 0, 
                    (struct sockaddr*)&server_fd, &sockaddr_size);
            check_error(rc, "recvfrom");
            
            fprintf(stdout,"[INFO] input->flag: %d meta: %d payload %s\n", input->flag, input->metadata, input->payload);
            if(input->flag & CONNECT_TERMINATE) {
                fprintf(stderr,"[ERROR] NOT CONNECTED TO SERVER\n");
                return EXIT_FAILURE;
            }

            // check if data pack was the one we expected
            else if (input->packet_seq > ack_pack->packet_seq) {
                ack_pack->packet_seq = input->packet_seq;
                ack_pack->ack_seq++;
                fprintf(stdout,"[INFO] We received pauload %s\n", input->payload);
                fputs(input->payload, filep); //writes buffer into filep
                // update seqence in ack_pack
                // send ack on recevied data 
                //rc = sendto()
            }
            else {
                // wasnt the right data pack!?
                // send ack agaig
            }

            // check if its an empty packet
            if (input->flag == DATA_PACK && sizeof(input->payload) < 1){
                // send 0x02 terminate
                // save file etc
            }

        }
        else { // we waited too long and recevied nothing. Send packet again
            fprintf(stdout,"[INFO] We waited too long, sending ACK agian\n");
            //rc = sendto(udpSocket_fd, my_packet_to_buffer(ack_pack, buffer))
        }
        

    } while (1);
    



    close(udpSocket_fd);
    return EXIT_SUCCESS;
}