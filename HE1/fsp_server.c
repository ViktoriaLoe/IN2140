#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "send_packet.h"

#define PORT 

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
int find_connection_index(int sender_id)
{
    for (int index = 0; index < number_of_connections; index++) {
        if (active_connections[index]->connection_id == sender_id){
            return index;
        }
    }
    return -1;
}

void send_file(struct Packet *input)
{
    // pakke og 
    char *output_buffer;
    //goes through all connected clients and checks if more needs to be sent

    /* Reveice a message ffrom client*/
    fprintf(stdout,"[INFO] Buffer contains flag: %d id %d\n", input->flag, input->sender_id);

    FILE *outpufile;
    //check valid file etc
    int index = find_connection_index(input->sender_id);
    struct rdp_connection *current_client = active_connections[index];
    struct sockaddr_in client_ip = current_client->ip_adress;
    struct Packet *output_packet = malloc(sizeof(struct Packet *));
    output_packet = construct_packet(DATA_PACK, input->ack_seq, input->packet_seq)

    int rc = 0;
    //write to output_buffer from FILE


    /*Sending what is in buffer*/
    rdp_write(client_ip, output_buffer);

}


int main(int argc, char const *argv[])
{
    /*Variables*/ 
    fd_set readFD;
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
    fprintf(stdout, "[INFO] server successfully received input\n");
   /* Socket to receive datagrams with IPv4*/
        // There needs to be multiple sockets later on, one for each client
        // The data needs to be fragmented, so it can send large files with UDP

    /*Assigning input variables, listening for all adresses*/
    socklen_t sock_addrsize = sizeof(struct sockaddr);
    port = htons(atoi(argv[1]));
    
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

    //Socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(socket_fd, "socket");        

   /* Socket will listen for adress we assigned (local host)*/
    rc = bind(socket_fd, (struct sockaddr *)&addr_con, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    struct pollfd poll_list[max_connections];

    /* MAIN LOOP*/
    do
    {
        FD_ZERO(&readFD); //clea the read fd
        FD_SET(socket_fd, &readFD); //add fd to set


        struct Packet *input = malloc(sizeof(struct Packet *));

        struct timeval tv = {60, 0};
        //listening to FD for incoming packet
        if (select(1, &readFD, NULL, NULL, &tv) < 0) {

            /*Getting input and reformatting it*/
            if (FD_ISSET(socket_fd, &readFD)) {

                fprintf(stdout,"[INFO] Trying to recevie data\n");
                rc = recvfrom(socket_fd, input, sizeof(struct Packet),
                        0, (struct sockaddr *)&addr_con, &sock_addrsize);
                        // clients IP would be inet_ntoa(addr_cli.sin_addr) and port ntohn(addr_cli.sin_port)
                    check_error(rc, "recvfrom");
                    fprintf(stdout,"[INFO] Recevied data %d bytes\n", rc);

                    fprintf(stderr,"[INFO] Buffer contains flag: %d id %d\n", input->flag, input->sender_id);

                //1. Check if it was a new connect request 
                if (input->flag & CONNECT_REQ) {
                    if (number_of_connections < max_connections) {
                        fprintf(stdout,"[INFO] Attempting to accept %d\n", input->sender_id);
                        rdp_accept(input, addr_con, socket_fd);
                    } else {
                        //send packet 0x20 == refuses connect request
                        fprintf(stderr, "[ERROR] NOT CONENCTED %d %d\n", input->sender_id, input->recv_id);
                    }
                }

                //2. Try to deliver the 'next' packages to all connected rdp-clients
                    // We recevied an ACK and can start/proceed in sending file to client
                if (input->flag & ACK_PACK) {
                    fprintf(stdout,"[INFO] Attempting to send file %d\n");
                    send_file(input, addr_con);
                }

                //3. Check if an rdp-connection is closed
                if(input->flag & CONNECT_TERMINATE) {
                    // free rdp_connection
                    fprintf(stdout, "[INFO] DISCONNECTED %d %d\n", input->sender_id, input->recv_id);
                    rdp_close(input); // closes connection removes from array and frees space
                }

                //4. if none of this happened, wait ~ either wait 1s or use select() to wait
                if (0)
                {
                    //wait
                }
            }
        } else {
            // send again
    }
    } while (1);

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
    
    // https://github.com/hzxie/Multiplex-Socket/blob/master/server.c see line 264

  Things to remember:
    - Check flags before trying to access payload etc. 
*/