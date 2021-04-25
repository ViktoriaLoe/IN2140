#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define PORT 
#define BUFFER_SIZE 1000

//An overview over all active connection to different clients, needs to be dynamically allocated
int socket_fd;



void check_error(int ret, char *msg) {
    if (ret == -1) {
        fprintf(stderr, "[ERROR] An error has occurred attempting function: %s\n", msg);
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void free_infrastructure()
{
    for (int i = 0; i < number_of_connections; i++){
        free(active_connections[i]);
    }
    free(active_connections);
}


int main(int argc, char const *argv[])
{
    /*Variables*/ 
    int port, rc;
    struct sockaddr_in addr_con, addr_cli;
    const char *filename;
    FILE *file;


    /*Check if right amount of arguments are provided */
    if (argc < 5)
    {
        fprintf(stderr, "[ERROR] Usage: %s <port-number> <filename> <number-of-transerfer> <loss-prob>\n", argv[0]);
        return EXIT_SUCCESS;
    }
        // Check for right format in arguments
    fprintf(stderr, "[INFO] server successfully received input\n");
   /* Socket to receive datagrams with IPv4*/
        // There needs to be multiple sockets later on, one for each client
        // The data needs to be fragmented, so it can send large files with UDP
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(socket_fd, "socket");        

    /*Assigning input variables, listening for all adresses*/
    socklen_t sock_addrsize = sizeof(struct sockaddr);
    port = atoi(argv[1]);
    
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = port;
    addr_con.sin_addr.s_addr = INADDR_ANY;

    filename = argv[2];
    max_connections = atoi(argv[3]);
    number_of_connections = 0;
    active_connections = malloc(sizeof(struct rdp_connection)*max_connections);
    set_loss_probability(atof(argv[4]));

        // Buffers for input/output
    char input_buffer[BUFFER_SIZE] = {0};
    char output_buffer[BUFFER_SIZE] = {0};


   /* Socket will listen for adress we assigned (local host)*/
    rc = bind(socket_fd, (struct sockaddr *)&addr_con, sizeof(struct sockaddr_in));
    check_error(rc, "bind");


    /* MAIN LOOP*/
    while (number_of_connections < max_connections+1)
    {
        int action = 0;
        fprintf(stderr,"[INFO] Trying to recevie data\n");
        /*Getting input and reformatting it*/
        rc = recvfrom(socket_fd, input_buffer, BUFFER_SIZE,
                0, (struct sockaddr *)&addr_cli, &sock_addrsize);
                // clients IP would be inet_ntoa(addr_cli.sin_addr) and port ntohn(addr_cli.sin_port)
        check_error(rc, "recvfrom");
        struct Packet *input = buffer_to_packet(input_buffer); //packet that holds input


        //1. Check if it was a new connect request  
        if (input->flag & 0x01) {
            if (number_of_connections < max_connections) {
                rdp_accept(input, addr_cli);
            } else {
                //send packet 0x20 == refuses connect request
                fprintf(stderr, "[ERROR] NOT CONENCTED %d %d\n", input->sender_id, input->recv_id);
            }
        }

        //2. Try to deliver the 'next' packages to all connected rdp-clients
        rdp_write();

        //3. Check if an rdp-connection is closed
        if(input->flag & 0x02) {
            // free rdp_connection
            fprintf(stderr, "[INFO] DISCONNECTED %d %d\n", input->sender_id, input->recv_id);
            rdp_close(input); // closes connection removes from array and frees space
        }

        //4. if none of this happened, wait ~ either wait 1s or use select() to wait
        if (!action)
        {
            //wait
        }

    }

    close(socket_fd);
    free_infrastructure();
    return EXIT_SUCCESS;

}
/* Questions:
    - Where does the ID gets set?
    - What isn't always included in packet?
    - When does server send ack
    - Where do I create the packets and how
    - How do I read the buffer 
    - Why is the buffer const char not char * what's the difference?ah
    

  Things to remember:
    - Check flags before trying to access payload etc. 
*/