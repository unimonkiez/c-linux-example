#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 
#define PORT 1337 


/**
  This method is used by the server to clean up before existing, 
  only prints and closes the socket 
*/
void clean_up(int cond, int *sock)
{ printf("Exiting now.\n");
  close(*sock); 
  exit(cond);
} 

int main(void)
{ 
  // Define the variables
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  int len; 
  int count;
  
  // Create a socket of stream type
  sock = socket(AF_INET, SOCK_STREAM, 0);  
  if (sock < 0)
    { perror ("Error opening channel");
      clean_up(1, &sock);
    }
  
  bzero(&serv_name, sizeof(serv_name));

  // Define socket's properties so the socket could listen 
  serv_name.sin_family = AF_INET;                                   
  serv_name.sin_port = htons(PORT); 

  // Bind that socket to the address given
  if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0)
    { perror ("Error naming channel");
      clean_up(1, &sock);
    }
      
  printf("Server is alive and waiting for socket connection from client.\n");
  // Listen to the socket and accept messages
  listen(sock, 1); 
  len = sizeof(serv_name);
  // Get a socket to send responses to
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);

  // Loop 10 times
  for (count = 1; count <= SIM_LENGTH; count++)
    { 
      // Write a response on the connected socket
      write(connect_sock, &count, 4);
      printf("Server has written %d to socket.\n", count);
    }

  // Close the connected socket
  close(connect_sock); 
  // Perform a clean up before termination
  clean_up(0, &sock); 

}
