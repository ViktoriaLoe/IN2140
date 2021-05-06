#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "send_packet.h"
#include <time.h>


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
    char input_buffer[1024];
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
    output_buffer_c = malloc(sizeof(struct Packet)+ BUFFER_SIZE); 
    server_fd_global = server_fd;

    srand(time(0));
    id = rand() % 1000;

    /* Socket to send datagrams with IPv4*/
    udpSocket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(udpSocket_fd, "socket");

    /*Making packets to send*/
    struct Packet *connection_attempt   = malloc(sizeof(struct Packet));
    connection_attempt  = construct_packet(CONNECT_REQ, 0, 0, id, 0, 0, 0);

    /* Sending packet using rdp_write*/
    rc = rdp_write_server(udpSocket_fd, connection_attempt);
        check_error(rc, "sendto");
        fprintf(stderr, "[INFO] Sent %d bytes\n", rc);
    
    /*Opening file in write-mode*/
    snprintf(filename, 28, "kernal_file_%d.txt", id %20);
    filep = fopen(filename, "w+");
    //check_error(filep, "fopen");

    // input packet and ack_packet
    struct Packet *input = malloc(sizeof(struct Packet ) + 2000);
    struct Packet *ack_pack             = malloc(sizeof(struct Packet)+BUFFER_SIZE);
    ack_pack            = construct_packet(ACK_PACK, 0, 0, id, 200, 0, 0);



    /* Main loop for file receiving (select timeout if nothing is recevied ) */
    do {
        FD_ZERO(&server_fd_set);
        FD_SET(udpSocket_fd, &server_fd_set);

        struct timeval tv = {100, 0};
        select(FD_SETSIZE + 1, &server_fd_set, NULL, NULL, &tv);

        if (FD_ISSET(udpSocket_fd, &server_fd_set)) {
            // FD is used, recevie input buffer
            printf("\nReceving input\n");
            rc = recvfrom(udpSocket_fd, input_buffer, sizeof(struct Packet)+ 1024, 0,
                    (struct sockaddr*)&server_fd, &sockaddr_size);
                    check_error(rc, "recvfrom");

            input = rdp_read(input_buffer, ack_pack, udpSocket_fd); 

            // If we got the expected datapacket
            if (input != NULL) {
            
                // Data pack is empty
                if (input->metadata == 0) {
                    printf("We got an Empty packet! DONE\n");
                    connection_attempt->flag = CONNECT_TERMINATE;
                    rc = rdp_write_server(udpSocket_fd, connection_attempt);
                        check_error(rc, "sento");
                    break;
                }
                // We received data
                ack_pack->packet_seq++;
                fputs(input->payload, filep); //writes buffer into filep

                rc = rdp_write_server(udpSocket_fd, ack_pack);
                check_error(rc, "sendto");
                ack_pack->ack_seq++;
                continue;
            }
            else {
                // Everything is handled
            }

        }
        else { // we waited too long and recevied nothing. Send packet again
            fprintf(stdout,"[INFO] We waited too long\n");
            rc = rdp_write_server(udpSocket_fd, ack_pack);
                check_error(rc, "sendto");
        }
        
    } while (1);
    

    printf("End of main\n");
    free(connection_attempt);
    free(input);
    free(ack_pack);
    close(udpSocket_fd);
    fclose(filep);
    return EXIT_SUCCESS;
}