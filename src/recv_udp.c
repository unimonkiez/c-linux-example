#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#define NAME 54321


void printsin(struct sockaddr_in *s, char *str1, char *str2) {

  printf("%s\n", str1);
  printf("%s ", str2);
  printf("ip= %s , port= %d", inet_ntoa(s->sin_addr), s->sin_port);
  printf("\n");
}


int main(int argc, char *argv[])
{
  // Define varibales
  int socket_fd, cc, fsize;
  struct sockaddr_in  s_in, from;
  struct { char head; u_long  body; char tail;} out_msg, in_msg;


  // Create socket for communication
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

  // Define the address used by the socket
  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  s_in.sin_port = htons((u_short)0x3333);

  //printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout);

  // Binds the unbound socket to the address given
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

  // Loops infinitly
  for(;;) {
    fsize = sizeof(from);

    // Receives a message and store it into in_msg varibale
    cc = recvfrom(socket_fd,&in_msg,sizeof(in_msg),0,(struct sockaddr *)&from,&fsize);
    printsin( &from, "recv_udp: ", "Packet from:");

    // Prints the message
    printf("Server got data ::%c%ld%c\n",in_msg.head,(long) ntohl(in_msg.body),in_msg.tail); 
    fflush(stdout);

    // Create the message
    out_msg.head = '<';
    out_msg.body = htonl(NAME);
    out_msg.tail = '>';

    // Send the message to the socket 
    sendto(socket_fd,&out_msg,sizeof(out_msg),0,(struct sockaddr *)&from, &fsize);
    printf("Server sent data ::%c%ld%c\n",out_msg.head, (long) ntohl(out_msg.body), out_msg.tail); 
    fflush(stdout);
  }
  
  return 0;
}

