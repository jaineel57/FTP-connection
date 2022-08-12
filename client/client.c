// Importing all the required libraries for the file
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define PORT 12345

// Defining the main function for client side
int main(){
  // Defining the scoker descriptor
  int socket_desc;
  // Getting the value for the socket descriptor by mentioning domain, type and protocol
  socket_desc = socket(AF_INET,SOCK_STREAM,0);
  // Using the giving structure to bind the socket with desired address so that server can losten to client
  struct sockaddr_in server_address;
  // Using the bzero function to erase any intial memory and replace it with /0 for new data
  bzero(&server_address,sizeof(server_address));
  
  // Using the if else condition to verify that an integer value was stored in socket descriptor defined above
  if(socket_desc<0){
    printf(" Our connection has faced an error \n");
    exit(0);
  }
  else{
    printf("Socket has been created successfully \n");
  }

  // Defining the three main components of the sockaddr_in structure used above
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(PORT);

  // Using the if else condition to verify the status of the connection that has been created
  if(connect(socket_desc,(struct sockadd *)&server_address,sizeof(server_address)) < 0){
    printf(" Our connection has faced an error \n");
  }
  else {
    printf(" Connection has been established successfully");
  }

  char buff[10000] = "";
  char file_name[50];
  while(1){
    bzero(buff,sizeof(buff));
    printf("\n >>>>>>\t");
    scanf("%[^\n]%*c",buff);
    write(socket_desc,buff,strlen(buff));
    
    printf("\n Client sends the following message\t:\t%s\n",buff);

    if(strlen(buff) > 7){
      int length;
      length = strlen(buff) - 5;
      strncpy(file_name,buff + 5,length);
      file_name[length] = '\0';
    }
    read(socket_desc,buff,sizeof(buff));
    if(strcmp(buff,"\nConnection Termination\n") == 0){
      printf("\n The connection has been terminated \n");
      break;
    }

    if(strcmp(buff,"\n225 Data connection open; no transfer in progress.\n") == 0){
      printf("%s",buff);

      printf("\n Sending the file\n");
      sleep(2);

      FILE *file_ptr;
      file_ptr = fopen(file_name,"r");

      if(file_ptr == NULL){
        printf("Not able to open the file\t:\t%s \n",file_name);
      }   
      else{
        char c;
        bzero(buff,sizeof(buff));
        int x =0;

        while(1){
          while((strlen(buff)-1 != sizeof(buff)) && ((c = fgetc(file_ptr)) != EOF)){
            buff[x] = c;
            x = x + 1;
          }

          buff[x] = "\0";
          if(c == EOF){
            write(socket_desc,buff,strlen(buff));
            break;
          }
          if(strlen(buff) == sizeof(buff)){
            write(socket_desc,buff,strlen(buff));
            bzero(buff,sizeof(buff));
            x = 0;
          } 
        }
        printf("\nSuccessfully sent the file\n");
        fclose(file_ptr);
      }
      } 
      else if(strcmp(buff,"\n226 Data connection open; no tranfer in progress.\n") == 0){
          printf("%s",buff);

          FILE *file_ptr;
          file_ptr = fopen(file_name,"w");

          if(file_ptr == NULL){
            printf("Not able to open the file\t:\t%s \n",file_name);
          } 
          else{
            bzero(buff,sizeof(buff));
            while(1){
              read(socket_desc,buff,sizeof(buff));
              fputs(buff,file_ptr);

              if(strlen(buff) != sizeof(buff)){
                break;
              }
              bzero(buff,sizeof(buff));
            }
            printf("\nRecieved file successfully\n");
            fclose(file_ptr);
        }
      }
      else{
        printf("%s",buff);
      }
      }
      close(socket_desc);

      return 0;
}
