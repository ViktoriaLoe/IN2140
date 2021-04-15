#include <stdio.h>


struct RDP_connection {
    int connection_id;
};

//An overview over all active connection to different klients, needs to be dynamically allocated
struct RDP_connection **Active_connections;


int main(void)
{


    /* will receive some arguemtns from the commandline
        - UDP portnumber, that the application will receive packages on
        - filename of the file it will send to all connected clients
        - number N files that the server needs to send before it quits 
        - loss-probablity as a float
    */


   /* everytime a client connects to the server, the server respons with a package with flagg == 0x10 and prints the connection IDs
    format: CONNECTED <client-id> <server-id>
    if the server has already made N conenctiong and transfers, it will not accept and respond with a package flagg == 0x20 and prints
    format: NOT CONNECTED <client-id> <server-id>

    server quits after delivering N files
   */  
    //Establish client

    // MAIN LOOP

        //1. Check for new RDP connection requests


        //2. Try to deliver the 'next' packages to all connected RDP-clients


        //3. Check if an RDP-connection is closed???


        //4. if none of this happened, wait ~ either wait 1s or use select() to wait


    

}