#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int socketFileDescript, portNumber, status;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portNumber = atoi(argv[2]);
    socketFileDescript = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescript < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    //Line to understand
    bcopy((char *)server->h_addr,(char *)&serverAddress.sin_addr.s_addr,server->h_length);
    serverAddress.sin_port = htons(portNumber);
    if (connect(socketFileDescript, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    status = write(socketFileDescript, buffer, strlen(buffer));
    if (status < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    status = read(socketFileDescript, buffer, 255);
    if (status < 0) 
         error("ERROR reading from socket");
    printf("%s\n", buffer);
    close(socketFileDescript);
    return 0;
}