#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define BUFLEN 1016

//An overview over all active connection to different clients, needs to be dynamically allocated
int socket_fd;



void check_error(int ret, char *msg) {
    if (ret == -1) {
        fprintf(stderr, "[ERROR] An error has occurred attempting function: %s\n", msg);
        perror(msg);
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char const *argv[])
{
    /*Check if right amount of arguments are provided */
    if (argc < 5)   {
        fprintf(stderr, "[ERROR] Usage: %s <port-number> <filename> <number-of-transerfer> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }
        // Check for right format in arguments

    /*Variables*/ 
    fd_set readFD;
    int port, rc;
    struct sockaddr_in addr_con;

    /*Assigning input variables, listening for all adresses*/
    socklen_t sock_addrsize = sizeof(struct sockaddr);
    port = htons(atoi(argv[1]));


    output_file = fopen(argv[2], "rb");
    fseek(output_file, 0L, SEEK_END);
    file_length = ftell(output_file);
    rewind(output_file);
    file_buffer = calloc(file_length, sizeof(char));
    rc = fread(file_buffer, sizeof(char), file_length, output_file);
        check_error(rc, "fread");
    rewind(output_file);
    output_buffer = calloc(BUFFER_SIZE, sizeof(char));
    

    fprintf(stdout,"[INFO] output_buffer: %s\n", output_buffer);
    max_connections = atoi(argv[3]);
    number_of_connections = 0;
    active_connections = malloc(sizeof(struct rdp_connection)*max_connections);
    set_loss_probability(atof(argv[4]));

    //Setting up client connection 
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = port;
    addr_con.sin_addr.s_addr = INADDR_ANY;

//Socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(socket_fd, "socket");        

   /* Socket will listen for adress we assigned (local host)*/
    rc = bind(socket_fd, (struct sockaddr *)&addr_con, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    /* MAIN LOOP*/
    char *input = calloc(BUFFER_SIZE, sizeof(char));
    struct rdp_connection *new_connection;
    int done = 0;
    sleep(5);
    do
    {
        FD_ZERO(&readFD); //clear the read fd
        FD_SET(socket_fd, &readFD);
        fprintf(stdout,"\n\n[INFO] Using select and waiting\n");

        //listening to FD for incoming packet
        struct timeval tv = {1, 0};
        select(FD_SETSIZE , &readFD, NULL, NULL, &tv); 

        /*Getting input and reformatting it*/
        if (FD_ISSET(socket_fd, &readFD)) {

            fprintf(stdout,"[INFO] Checking if there are new connections to accept\n");
            
            new_connection = rdp_accept(socket_fd, addr_con);

            rc = recvfrom(socket_fd, input, sizeof(struct Packet),
                    0, (struct sockaddr *)&addr_con, &sock_addrsize);
                    check_error(rc, "recvfrom");
                    fprintf(stdout,"[INFO] Recevied data bytes from \n");
                // fprintf(stderr,"[INFO] Buffer contains flag: %d id %d\n", input->flag, input->sender_id);

            if (new_connection != NULL) {
                //free(new_connection);
                continue;
            }

            // reading input to see if it was an ACK or CONNECTION termination
            done += rdp_read_from_client(input);
            if (done == max_connections) {
                break;
            }

        }
        else { //timed out sending again
            fprintf(stdout,"[INFO] Waited too long!\n");
            if (number_of_connections == 0) {
                fprintf(stdout,"[INFO] Nothing to do, number_of_connections: %d\n", number_of_connections);
                continue;
            }
            if (new_connection != NULL && 
            done < number_of_connections &&
            new_connection->previous_packet_sent != NULL) {
                fprintf(stdout,"[INFO] Sending previous packet again\n");
                rdp_write(new_connection, new_connection->previous_packet_sent);
            }
        }
    } while (1);
    printf("end of main \n");
    //rdp_close();
    close(socket_fd);
    free(new_connection);
    free(file_buffer);
    free(output_buffer);
    return EXIT_SUCCESS;


}

/* Questions:
    - Where does the ID gets set?
    - What isn't always included in packet?
    - When does server send ack
    - Where do I create the packets and how
    - How do I read the buffer 
    - Why is the buffer const char not char * what's the difference?ah
    - Do we need to send ACK for conenction denied?
    
    // https://github.com/hzxie/Multiplex-Socket/blob/master/server.c see line 264

  Things to remember:
    - Check flags before trying to access payload etc. 
*/