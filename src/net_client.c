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
#define IP_ADDRESS "127.0.0.1" 
#define PORT 1337 
#define CLIENT_NAME "Noy_client"

int main(void)
{ 
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 
  char* serverName;

  printf("Client is alive and establishing socket connection.\n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      

  bzero(&cli_name, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
  cli_name.sin_port = htons(PORT);


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)
    { 
      sendto(sock,&CLIENT_NAME,sizeof(CLIENT_NAME),0,(struct sockaddr *)&cli_name,
                  sizeof(cli_name));
      read(sock, &serverName, sizeof(serverName));
      printf("Client has received %s from socket.\n", serverName);
    }

  printf("Exiting now.\n");

  close(sock); 
  exit(0); 

} 
