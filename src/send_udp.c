#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  // Define varibales
  int socket_fd;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  struct { char head; u_long body; char tail; } msgbuf;

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
  msgbuf.head = '<';
  msgbuf.body = htonl(getpid()); /* IMPORTANT! */
  msgbuf.tail = '>';

  // Send the message to the socket 
  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));

  return 0;
}
