#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "send_packet.h"

#define BUFFER_SIZE 1000



void check_error(int ret, char *msg) {
    if (ret == -1) {
        fprintf(stderr, "[ERROR] An error has occurred attempting function: %s\n", msg);
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[])
{
    /*Variables*/ 
    int udpSocket_fd, rc;
    struct sockaddr_in addr_con;
    FILE *file;
    fd_set server_fd;


    if (argc < 4)
    {
        fprintf(stderr, "[ERROR]Usage: %s <server-ip> <port-number> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }


    /*Assigning input variables*/ 
    socklen_t sockaddr_size = sizeof(struct sockaddr);
    addr_con.sin_family = AF_INET; //can it send to IPv6 as well? does it have to
    addr_con.sin_port = htons(atoi(argv[2]));
    addr_con.sin_addr.s_addr = inet_addr(argv[1]);
    //set_loss_probability(atof(argv[3]));

    /* Socket to send datagrams with IPv4*/
    udpSocket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(udpSocket_fd, "socket");

    /*Making packets to send*/
    struct Packet *connection_attempt   = malloc(sizeof(struct Packet *));
    struct Packet *ack_pack             = malloc(sizeof(struct Packet *));
    connection_attempt  = construct_packet(CONNECT_REQ, 1, 1, rand() % 1000, 200, 0, 0);
    ack_pack            = construct_packet(ACK_PACK, 0, 0, rand() % 1000, 200, 0, 0);
    my_print_packet(connection_attempt);

    fprintf(stderr,"[INFO] Packet contains flag: %d id: %d\n", connection_attempt->flag, connection_attempt->sender_id);

    /* Sending packet*/
    char *buffer = malloc(sizeof(struct Packet)+10);
    my_packet_to_buffer(connection_attempt, buffer);

   char *buffer_to_send = buffer;
    rc = sendto(udpSocket_fd,                           /*socket*/
                    buffer_to_send,                     /*message*/
                    sizeof(buffer),              /*amount of bytes*/
                    0,                                  /*flags*/
                    (struct sockaddr *)&addr_con,       /*IP and port*/
                    sizeof(struct sockaddr_in));        /*size of addres-struct */
    
     
    check_error(rc, "sendto");
        //when a connection is established, client prints connection-ID to both server and client
    fprintf(stderr, "[INFO] Sent %d bytes\n", rc);
    
    /* Receive answer for connection request */
    struct Packet *input = malloc(sizeof(struct Packet *));
    rc = recvfrom(udpSocket_fd, input, BUFFER_SIZE, 
                0, (struct sockaddr*)&addr_con, &sockaddr_size);
    check_error(rc, "recvfrom");
    struct Packet *response = malloc(sizeof(struct Packet *));

    //Received accept, send ack
    if (input->flag & CONNECTION_ACC) { 
        fprintf(stdout, "[INFO] CONNECTED TO SERVER\n");

        char *buffer_ack = malloc(sizeof(struct Packet));
        my_packet_to_buffer(ack_pack, buffer_ack);

        rc = sendto(udpSocket_fd, buffer_ack, sizeof(buffer_ack), 
            0, (struct sockaddr *)&addr_con, sizeof(struct sockaddr_in));
            check_error(rc, "sendto");
    }

    else if (input->flag & CONNECTION_DEN) {
        fprintf(stderr,"[ERROR] NOT CONNECTED TO SERVER\n");
        return EXIT_FAILURE;
    }

    /* Main loop for file receiving (select timeout if nothing is recevied ) */
    do {
        FD_ZERO(&server_fd);
        FD_SET(udpSocket_fd, &server_fd);

        struct timeval tv = {60, 0};
        if (select(1, &server_fd, NULL, NULL, &tv) < 0) {
            rc = recvfrom(udpSocket_fd, input, BUFFER_SIZE, 0, 
                    (struct sockaddr*)&addr_con, &sockaddr_size);
            check_error(rc, "recvfrom");
            
            if (input->flag & CONNECTION_ACC ){
                //send ack_pack again 
                
            }
            else if(input->flag & CONNECT_TERMINATE) {
                fprintf(stderr,"[ERROR] NOT CONNECTED TO SERVER\n");
                return EXIT_FAILURE;
            }

            // check if data pack was the one we expected
            else if (input->packet_seq > ack_pack->packet_seq) {
                ack_pack->packet_seq = input->packet_seq;
                ack_pack->ack_seq++;
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

            // reveive and store complete file

            //when package is recieved and saved -> the RDP-connection is closed and it prints the name of the file and quits

        }
        else { // we waited too long, send packet again
            //rc = sendto(udpSocket_fd, my_packet_to_buffer(ack_pack, buffer))
        }
        

    } while (1);
    



    close(udpSocket_fd);
    return EXIT_SUCCESS;
}