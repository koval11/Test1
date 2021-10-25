#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sting.h>          // for strncmp()
#include <unistd.h>         // for close()

// Address structure

#define MAX_LEN 1024
#define PORT 22110
int main(){

    printf("Brian's Net Listen Test on UPD port %d:\n", PORT);
    printf("Connect using: \n");
    printf("    netcat -u 127.0.0.1 %d\n", PORT);


    struct sockaddr_in sin; //_in means internet; sin is socket for internet (local variable on stack)
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET; // Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    // htonl = host to network long; htons = host to network short


    // Create and bind to socket

    int socketDescriptor = socket(PF_INET, SOCK_DGRAM); // SOCK_GRAM means its UPD 
    bind(socketDescriptor, (struct sockaddr*)&sin, sizeof(sin)); // opens socket


    // Receive Data

    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);
    char messageRx[MAX_LEN];                            // Buffer Rx
    int bytesRx = recvfrom(socketDescriptor,
        messageRx, MAX_LEN, 0,                          // Client's data written into messageRx string
        (struct sockaddr*) &sinRemote, &sin_len);        // sinRemote is output parameterl; sinLen is in/out parameter. passed as pointer   sinRemote will be filled with sender's IP addr


    // Null terminated (sting)

    int terminateldx =                                  //
        (bytesRx < MAX_LEN) ? bytesRx : MAX_LEN - 1;    // This 3 lines (32-33) reminate NULL sting
    messageRx[terminateldx] = 0;                        //
    printf ("Message received (%d bytes): '%s'\n",
        bytesRx, messageRx);


    // Create Reply
        // Watch for buffer overflow!

    char messageTx[MAX_LEN];                            // Populate return message;
    sprintf(messageTx, "hello %d\n", 42);               // messageTx is sring = array of char

    // Send Reply

    sin_len = sizeof(sinRemote);
    sendto(socketDescriptor,
        messageTx, strlen(messageTx),                   // message
        0,
        (stuct sockaddr *)&sinRemote, sin_len);         // where to?; Here client's IP address and port from receiving the message.



    // Close socket (when done)

    close(socketDescriptor);                            // may take a few seconds for OS to sinish closing. or you may endup with memeory leak!!!

}vvvv