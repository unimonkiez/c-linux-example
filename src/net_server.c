#include <sys/types.h>
#include <unistd.h>   
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>       
#define SIM_LENGTH 10 
#define PORT 1337 
#define SERVER_NAME "Noy_server"

void clean_up(int cond, int *sock)
{ printf("Exiting now.\n");
  close(*sock); 
  exit(cond);
} 
int main(void)
{ 
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  int len; 
  int count;
  int cc;
  char* clientName;
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);  
  if (sock < 0)
    { perror ("Error opening channel");
      clean_up(1, &sock);
    }
  
  bzero(&serv_name, sizeof(serv_name)); 
  serv_name.sin_family = AF_INET;                                   
  serv_name.sin_port = htons(PORT); 

  if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0)
    { perror ("Error naming channel");
      clean_up(1, &sock);
    }
      
  printf("Server is alive and waiting for socket connection from client.\n");
  //listen(sock, 1); 

  len = sizeof(serv_name);
  //connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);

  for (count = 1; count <= SIM_LENGTH; count++)
    { 
      cc = recvfrom(sock, &clientName, sizeof(clientName),  0, (struct sockaddr *)&serv_name, &len);
      write(connect_sock, &SERVER_NAME, sizeof(SERVER_NAME));
      printf("Server got - '%s' and written '%s' to socket.\n", clientName, SERVER_NAME);
    }

  close(connect_sock); 
  clean_up(0, &sock); 

}
