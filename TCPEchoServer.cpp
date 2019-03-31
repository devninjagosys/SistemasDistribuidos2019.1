/* The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#define PORT 8080
#define SIZE_BUFFER 256

using namespace std;

int error(const char *msg)
{
    perror(msg);
    return EXIT_FAILURE;
}

int isPrime(int number)
{   for(int i = 2; i <= number/2; ++i)
    {
        if(number % i == 0)
        {
            return 0;
        }
    }
    return 1;
};

int main(int argc, char *argv[])
{
  int socketFileDescript, newsocketFileDescript, portNumber, dataMsg , isPrimeProduction ,numberOfMessages = 0;
  socklen_t clientLength;
  char charDataMsg[SIZE_BUFFER];
  struct sockaddr_in serverAddress, clientAddress;
  int statusSocket;
  if (argc < 2) 
  {
    fprintf(stderr,"ERROR, Not Enought Parameters Received\n");
    exit(1);
  }
  // Creating a New Socket
  // Socket File descriptor = socket(int domain, int type, int protocol)
  socketFileDescript =  socket(AF_INET, SOCK_STREAM, 0);
  if (socketFileDescript < 0) 
  {
    error("ERROR Opening Socket file Descriptor");
  }
  // clear address structure
  bzero((char *) &serverAddress, sizeof(serverAddress));
  portNumber = atoi(argv[1]);
  /* setup the host_addr structure for use in bind call */
  // server byte order
  serverAddress.sin_family = AF_INET;  
  // automatically be filled with current host's IP address
  serverAddress.sin_addr.s_addr = INADDR_ANY;  
  // convert short integer value for port must be converted into network byte order
  //serverAddress.sin_port = htons(portNumber);
  serverAddress.sin_port = htons(portNumber);
  // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
  // bind() passes file descriptor, the address structure, 
  // and the length of the address structure
  // This bind() call will bind  the socket to the current IP address on port, portNumber
  if (bind(socketFileDescript, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) 
      error("ERROR on binding");
  printf("The Server Has born - LOOK TO THE POWER !\n");
  // This listen() call tells the socket to listen to the incoming connections.
  // The listen() function places all incoming connection into a backlog queue
  // until accept() call accepts the connection.
  // Here, we set the maximum size for the backlog queue to 5.
  listen(socketFileDescript,5);
  // The accept() call actually accepts an incoming connection
  clientLength = sizeof(clientAddress);
  // This accept() function will write the connecting client's address info 
  // into the the address structure and the size of that structure is clientLength.
  // The accept() returns a new socket file descriptor for the accepted connection.
  // So, the original socket file descriptor can continue to be used 
  // for accepting new connections while the new socker file descriptor is used for
  // communicating with the connected client.
  newsocketFileDescript = accept(socketFileDescript,(struct sockaddr *) &clientAddress, &clientLength);
  if (newsocketFileDescript < 0) 
          error("ERROR on accept");
  printf("server: got connection from %s port %d\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
  // This send() function sends the 13 bytes of the string to the new socket
  while(true)
  {
    statusSocket = read(newsocketFileDescript,charDataMsg,SIZE_BUFFER);
    if (statusSocket < 0)
    {
      error("ERROR Sending Message to Socket file Descriptor");
      break;
    }
    dataMsg =  atoi(charDataMsg);
    if (dataMsg == 0)
    { 
      cout << "The Producer has finished the process" << endl;
      cout << "The Message received was :" << charDataMsg << endl;
      break;
    }
    cout << "The Consumer Received from the Producer: " << dataMsg << endl;
    cout << "Now the Consumer checks will check if is a Prime Number " << endl;
    isPrimeProduction = isPrime(dataMsg);
    sprintf (charDataMsg, "%d", isPrimeProduction);
    if (isPrimeProduction == 0 )
    {
      cout << "Consumer is about to send to producer: " << isPrimeProduction << endl;
      cout << "This means that is not a Prime number! " << endl;
    }
    else
    {
      cout << "Consumer is about to send to producer: " << isPrimeProduction << endl;
      cout << "This means that is a Prime number! " << endl;
    }
    statusSocket = send(newsocketFileDescript,charDataMsg, SIZE_BUFFER, 0);
    if (statusSocket < 0)
    {
      error("ERROR Sending Message to Socket file Descriptor");
    }
    bzero(charDataMsg,SIZE_BUFFER);
    numberOfMessages+=1;


  }
  printf("The Communication has Finished!\n");
  printf("Number of messages sent + received: %d\n",numberOfMessages*2);
  close(newsocketFileDescript);
  close(socketFileDescript);
  return 0; 
}