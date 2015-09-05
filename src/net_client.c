
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define SIM_LENGTH 10 
#define PORT 1337 

int main(int argc, char* argv[])
{ 
  // Defien variables
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 

  // Check if hostname was sent
  if (argc != 2) {
    perror("Usage: net_client.o <hostname>\n");
    exit(1);
  }

  // Get hostname
  hostname = argv[1];
  
  // Get address by hostname string
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  // Resolve host address as string
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);

  printf("Client is alive and establishing socket connection.\n");
  
  
  // Create a socket of stream type
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      

  // Define properties required to connect to the socket
  bzero(&cli_name, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); 
  cli_name.sin_port = htons(PORT);


  // Connect to the socket, if return 0 then succeeded, if not, print error
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }


  // Loop 10 times
  for (count = 1; count <= SIM_LENGTH; count++)
    { 
      // Get values from the socket into value variable
      read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");
  // Close the scoekt
  close(sock); 
  exit(0); 

} 
