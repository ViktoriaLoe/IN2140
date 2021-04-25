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
    /* DESCRIPTION
    * It's the client that always initiates the connection 
    * we have to create multiple clients that all send packets to the server
    * The client then communicate with the server through packets, and act accordingly 
    * If a client sends 0x01, a new RDP connection needs to be established
    * On top of the existing UDP connection 
    */

    /*Variables*/ 
    int udpSocket_fd, rc;
    struct sockaddr_in addr_con;
    FILE *file;

    if (argc < 4)
    {
        fprintf(stderr, "[ERROR]Usage: %s <server-ip> <port-number> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }
    //check if input is in valid format

    /*Assigning input variables*/ 
    socklen_t sockaddr_size = sizeof(struct sockaddr);
    addr_con.sin_family = AF_INET; //can it send to IPv6 as well? does it have to
    addr_con.sin_port = atoi(argv[2]);
    addr_con.sin_addr.s_addr = atoi(argv[1]);
    set_loss_probability(atof(argv[3]));

    char input_buffer[BUFFER_SIZE] = {0};
    char output_buffer[BUFFER_SIZE] = {0};

    /* Socket to send datagrams with IPv4*/
    udpSocket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(udpSocket_fd, "socket");
    //client tries to connect to the server, if it doesnt respond within a minute it will exit and write an error.

    /*Making packets to send*/
    struct Packet *connection_attempt = malloc(sizeof(struct Packet));
    connection_attempt = construct_packet(0x01, 1, 1, 100, 200, 0, 0);
    my_print_packet(connection_attempt);

    /* Sending packet*/
    const char *packet_request = my_packet_to_buffer(connection_attempt);
    fprintf(stderr,"[INFO] Buffer: %s\n", packet_request);

    rc = send_packet(udpSocket_fd,                      /*socket*/
                    packet_request,                     /*message*/
                    sizeof(unsigned char),              /*amount of bytes*/
                    0,                                  /*flags*/
                    (struct sockaddr *)&addr_con,       /*IP and port*/
                    sizeof(struct sockaddr_in));        /*size of addres-struct */
    
     
    check_error(rc, "send_packet");
        //when a connection is established, client prints connection-ID to both server and client
    fprintf(stderr, "[INFO] Sent %d bytes\n", rc);
    
    // CHECK; if server takes more than a second to answer, terminate.
    /* Receive packets with recvfrom*/
    rc = recvfrom(udpSocket_fd, input_buffer, BUFFER_SIZE, 
                0, (struct sockaddr*)&addr_con, &sockaddr_size);
    check_error(rc, "recvfrom");
    struct Packet *response = buffer_to_packet(input_buffer);
    // check if it was a connection accpet
    fprintf(stderr, "[INFO] CONNECTED TO SERVER  id: %d client_id: %d\nReceived from server %s\n", response->sender_id, response->recv_id, input_buffer);


    // reveive and store complete file

        //when package is recieved and saved -> the RDP-connection is closed and it prints the name of the file and quits
    



    close(udpSocket_fd);
    return EXIT_SUCCESS;
}