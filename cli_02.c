#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "srv_h.h" 







void print_help(){

        printf("\n********************************** \nsend [file_name] - przeslij plik \nrecieve [file_name] - pobierz plik ");
        printf("\nrename [old_file_name] [new_file_name] - zmien nazwe \nfind [file_name] - wyszukaj \nls - wylistuj pliki ");
        printf("\npwd - obecny folder \ncd [path] - zmien folder \nhelp - for help  \nexit - for exit \n********************************** \n");



};
int main(int argc, char const *argv[])
{
    char    a_str[INET_ADDRSTRLEN];
    char buffer[BUFFSIZE], option[BUFFSIZE];
    char command3[BUFFSIZE], command2[BUFFSIZE], command1[BUFFSIZE];
    int clientSocket;

    struct sockaddr_in serverAddr,mcast_serverAddr;
    socklen_t addr_size;


        
  
        /*---- Create the socket. The three arguments are: ----*/
        /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
        clientSocket = socket(PF_INET, SOCK_STREAM, 0);

int one=1;
    if (setsockopt(clientSocket, SOL_TCP, TCP_NODELAY, &one, sizeof(one)) < 0)
    {
        printf("setsockopt error");
    };



        /*---- Configure settings of the server address struct ----*/
        /* Address family = Internet */
       
        /* Set port number, using htons function to use proper byte order */
        // serverAddr.sin_port = htons(7891);
        /* Set IP address to localhost */
        // serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        /* Set all bits of the padding field to 0 */
        



    // if (argc == 2 ){ //& argv[1] == "-m"
    // if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) != 1){

    //     printf("@ incorrect IP\n" );
    //      return 0;
    // }}

     serverAddr.sin_family = AF_INET;
     memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);


    if( argc == 1 ){
    printf("usage : %s mcast (Find server address) \n",argv[0] );
    printf("      : %s [ <host> | <port#> ] \n",argv[0]);
    return 0;

    } 
    else if( argc == 2 & ( (strcmp(argv[1], "mcast") == 0))){     // get multicast
        mcast_serverAddr = multicast_cli();
        serverAddr.sin_port = mcast_serverAddr.sin_port;
        serverAddr.sin_addr.s_addr =mcast_serverAddr.sin_addr.s_addr;

    }

    else if (argc == 3){

    if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) != 1){

        printf("@ incorrect IP\n" );
         return 0;
    }
       serverAddr.sin_port = htons(atoi(argv[2]));
    }

    else{
    printf("usage : %s mcast (Find server address) \n",argv[0] );
    printf("      : %s [ <host> | <port#> ] \n",argv[0]);
    return 0;}

   




        /*---- Connect the socket to the server using the address struct ----*/
        addr_size = sizeof serverAddr;
      if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)< 0){

        fprintf(stderr,"connect error : %s \n", strerror(errno));
        return 1;
    }
     

     inet_ntop(AF_INET, &serverAddr.sin_addr.s_addr, a_str, sizeof(a_str)); 
     printf("\n @ connected to: %s:%d",a_str ,ntohs(serverAddr.sin_port));
     // printf("%s: message = \"%s\"\n", inet_ntoa(addr.sin_addr), message);



 print_help();

  while(1)
    {
  


        printf("\n=> ");
        fgets( option, BUFFSIZE, stdin );

        sscanf(option,"%s %s %s", command1, command2, command3);//WYBOR CZYNNOSCI W TAKI SPOSOB

        if(!strcmp(command1, "send"))//przeslij plik
        {
            // printf("Podaj nazwe pliku do wyslania: ");
            // scanf("%s",filename);
            // strcpy(buffer, "wyslij");
            if (command2[0] == '\0'){

                printf("@ missing filename\n" );

            }
            else if( access( command2, F_OK ) != -1 ) {   // file exists
            send(clientSocket, command1, BUFFSIZE,0);
            // strcpy(filename,command2);
            send_file(command2, clientSocket);}
  
            else // file doesn't exist
            printf("@ file doesn't exist"); 
                         


        }

        else  if(!strcmp(command1, "recv"))//przeslij plik
        {

            if (command2[0] == '\0'){

                printf("@ missing filename\n" );

            }
            else{

            strcat(command1, " ");  
            strcat(command1, command2); 
            printf("%s\n",command1 );
            printf("%d\n",sizeof(command1) );
            send(clientSocket, command1, BUFFSIZE,0);

                recv(clientSocket, buffer, BUFFSIZE, 0);
                if(!strcmp(buffer, "F_OK"))
                    recv_file(clientSocket);
                else
                    printf("@ file doesn't exist"); 
            }
        }


               else if(!strcmp(command1, "help"))//przeslij plik
        {
             print_help();
        }
   
        else if(!strcmp(command1, "ls"))//wyswietl pliki
        {
            strcpy(buffer, "ls -l");

            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);
}

       else if(!strcmp(command1, "find"))
        {
        
            strcpy(buffer,"find \"*");
            strcat(buffer, command2);
            strcat(buffer, "*\"*");

            send(clientSocket, buffer, BUFFSIZE, 0);
            recv(clientSocket, buffer, BUFFSIZE, 0);
            printf("%s", buffer);
        }


               else if(!strcmp(command1, "rename"))
        {

            strcpy(buffer,command1);
            strcat(buffer, " ");
            strcat(buffer, command2);
            strcat(buffer, " ");
            strcat(buffer, command3);
            send(clientSocket, buffer, BUFFSIZE, 0);
            recv(clientSocket, buffer, BUFFSIZE, 0);
            printf("%s", buffer);

        }


        
        else if(!strcmp(command1, "pwd"))
        {
            strcpy(buffer,"pwd");
            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);
            // break;
        }

        else if(!strcmp(command1, "cd"))//wyswietl pliki
        {
            strcpy(buffer,"cd ");
            strcat(buffer, command2);
            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);
        }


        else if(!strcmp(command1, "exit"))//wyswietl pliki
        {


                close(clientSocket);
                return 0;
        }

        else {

            printf("@ command-not-found\n"); 
        }




            bzero(buffer,BUFFSIZE);
            bzero(command1,BUFFSIZE);
            bzero(command2,BUFFSIZE);
            bzero(option,BUFFSIZE);
          
    }

        
        
    return 0;
}
