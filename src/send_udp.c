#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#define NAME 12345

int main(int argc, char *argv[])
{
  // Define varibales
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; u_long  body; char tail;} out_msg, in_msg;

  // Create socket for communication
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */

  // Get the host by the host name 
  hostptr = gethostbyname(argv[1]);

  // Define the address used by the socket
  dest.sin_family = (short) AF_INET;
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  dest.sin_port = htons((u_short)0x3333);

  // Create the message
  out_msg.head = '<';
  out_msg.body = htonl(NAME);
  out_msg.tail = '>';

  // Send the message to the socket 
  sendto(socket_fd,&out_msg,sizeof(out_msg),0,(struct sockaddr *)&dest,
                  sizeof(dest));
  printf("Client sent data ::%c%ld%c\n",out_msg.head, (long) ntohl(out_msg.body), out_msg.tail); 
  // Receives a message and store it into in_msg varibale
  recvfrom(socket_fd,&in_msg,sizeof(in_msg),0,(struct sockaddr *)&dest, sizeof(dest));

  // Prints the message
  printf("Client got data ::%c%ld%c\n",in_msg.head,(long) ntohl(in_msg.body),in_msg.tail); 

  return 0;
}
