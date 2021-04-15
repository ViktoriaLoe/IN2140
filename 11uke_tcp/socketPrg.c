#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <string.h>
#include <sys/types.h>     
#include <netinet/in.h>
#include <netinet/ip.h>     
#include <sys/socket.h>

#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>




/*
Open two terminals 
./chat <myport> <target-port> <targetIP>
*/


void checkError(int res, char *msg)
 {
     if (res == -1) 
     {
        perror(msg);
        // Rydde?
        exit(EXIT_FAILURE);
     }
}

#define BUFFSIZE 325

int main(int argc, char const *argv[])
{
    int msgFD, rc;
    fd_set fds;
    struct sockaddr_in friends_addr, my_addr;
    char buf[BUFFSIZE];

    //riktig antall argumenter?
    if (argc < 4)
    {
        prinft("Usage: %s <myport> <friendport> <friendIP>\n", argv[0]);
        return EXIT_SUCCESS;
    }
    //UDP stream
    msgFD = socket(AF_INET, SOCK_DGRAM, 0);
    checkError(msgFD, "socket");

    //siden vi er mottaker og avsender trenger vi to adresser (dest og den vi lytter på)
    //lytte på adresse ved å binde adresse med en port
    
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1])); //burde sjekk eom det er gyldig port
    my_addr.sin_addr.s_addr = INADDR_ANY;

    rc = bind(msgFD, (struct sockaddr *)&my_addr, sizeof (struct sockaddr_in));
    checkError(rc, "bind");


    friends_addr.sin_family = AF_INET;
    friends_addr.sin_port = htons(atoi(argv[2]));

    rc = inet_pton(AF_INET, argv[3], &friends_addr.sin_addr.s_addr);  // look at return value
    checkError(rc, "inet_pton");
    if (!rc) {
        fprintf((stderr, "IP adress not valid: %s\n"), argv[3]);
        return EXIT_FAILURE;
    }

    /*
     to ting skal skje samtidig.
        - lytte til nettet, kommer det en melding fra friend?
        - lytte til tastaturet, kommer det en meldig?

        med read og fgets så MÅ den ene sende først og den andre bare venter, men med select kan hvem som helst initiere
    */


   //init fds til en tom liste
   FD_ZERO(&fds);

   FD_SET(msgFD, &fds);
   //FD_SET(STDIN_FILENO, &fds);

   rc = select(FD_SETSIZE, &fds, NULL, NULL, NULL);

   if (FD_ISSET(msgFD, &fds))
   {
       //mld fra nettet
        //lese mld inn i et buffer
        rc = read(msgFD, buf, BUFFSIZE -1);
        checkError(rc, "read");
        buf[rc] = 0; //legg til 0-byte

        //2 printe buffer
        printf("Vi mottok %s\n", buf);

   }

  /*  if (FD_ISSET(STDIN_FILENO, &fds))
    {
        //mld fra tastatur
            //lese inni buffer
         
            //2 sende mld over nett til friend
    }*/


    close(msgFD);
    return EXIT_SUCCESS;
}