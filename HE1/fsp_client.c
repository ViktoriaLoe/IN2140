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
    connection_attempt  = construct_packet(0x01, 1, 1, rand() % 1000, 200, 0, 0);
    ack_pack            = construct_packet(0x08, 0, 0, rand() % 1000, 200, 0, 0);
    my_print_packet(connection_attempt);

    fprintf(stderr,"[INFO] Packet contains flag: %d id: %d\n", connection_attempt->flag, connection_attempt->sender_id);

    /* Sending packet*/
    char *buffer = malloc(sizeof(struct Packet)+10);
    my_packet_to_buffer(connection_attempt, buffer);

    //Test that i goes from buffer back to packet
    struct Packet *packetRe = malloc(sizeof(struct Packet *));
    memcpy(&packetRe, buffer, sizeof(struct Packet));

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
    
    // CHECK; if server takes more than a second to answer, terminate.
    /* Receive packets with recvfrom*/
    struct Packet *input = malloc(sizeof(struct Packet *));
    rc = recvfrom(udpSocket_fd, input, BUFFER_SIZE, 
                0, (struct sockaddr*)&addr_con, &sockaddr_size);
    check_error(rc, "recvfrom");
    struct Packet *response = malloc(sizeof(struct Packet *));
    // check if it was a connection accpet
    if (input->flag & CONNECTION_ACC) { 
        fprintf(stdout, "[INFO] CONNECTED TO SERVER  id: %d client_id: %d\n", response->sender_id, response->recv_id);
    }
    else if (input->flag & CONNECTION_DEN) {
        fprintf(stderr," [ERROR] NOT CONNECTED TO SERVER\n");
    }
    /* Main loop for file receiving (select timeout if nothing is recevied ) */
    do {
        // check if its an empty packet
            // send 0x02 terminate
            // save file etc

        //check if it's the correct packet
            // send ack again

    // reveive and store complete file

        //when package is recieved and saved -> the RDP-connection is closed and it prints the name of the file and quits

    } while (1);
    



    close(udpSocket_fd);
    return EXIT_SUCCESS;
}