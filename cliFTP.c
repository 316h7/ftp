#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "srv_h.h" 


int main()
{
    char buffer[BUFFSIZE], option[BUFFSIZE];
    char command2[BUFFSIZE], command1[BUFFSIZE];
    int clientSocket;

    struct sockaddr_in serverAddr;
    socklen_t addr_size;


        printf("\n\nCo chcesz zrobic? \nsend [file_name] - przeslij plik \nrecieve [file_name]-pobierz plik ");
        printf("\n?-przenies/zmien nazwe \n?-wyszukaj \n?-wylistuj pliki ");
        printf("\npwd-obecny folder \ncd [path] - zmien folder \nhelp - for help  \nexit - for exit \n");
  
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
        serverAddr.sin_family = AF_INET;
        /* Set port number, using htons function to use proper byte order */
        serverAddr.sin_port = htons(7891);
        /* Set IP address to localhost */
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        /* Set all bits of the padding field to 0 */
        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);



        /*---- Connect the socket to the server using the address struct ----*/
        addr_size = sizeof serverAddr;
        connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);


  while(1)
    {
  


        printf("\n=> ");
        fgets( option, BUFFSIZE, stdin );

        sscanf(option,"%s %s", command1, command2);//WYBOR CZYNNOSCI W TAKI SPOSOB

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
            // strcpy(buffer, command1);
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
        printf("\n\nCo chcesz zrobic? \nsend [file_name] - przeslij plik \nrecieve [file_name]-pobierz plik ");
        printf("\n?-przenies/zmien nazwe \n?-wyszukaj \n?-wylistuj pliki ");
        printf("\npwd-obecny folder \ncd [path] - zmien folder \nhelp - for help  \nexit - for exit \n");
  
        }
        // case 2://pobierz plik
        // {
        //     printf("Podaj nazwe pliku do pobrania: ");
        //     scanf("%s",filename);
        //     strcpy(buffer,"pobierz ");
        //     strcat(buffer, filename);
        //     strcpy(kopia, buffer);//do sprawdzania, czy na serwerze istnieje plik

        //     send(clientSocket, buffer, BUFFSIZE,0);
        //     /*---- Read the message from the server into the buffer ----*/
        //     recv(clientSocket, buffer, BUFFSIZE, 0);

        //     if(strcmp(buffer, kopia))
        //     {
        //         FILE *file = fopen(filename, "w");
        //         fputs(buffer, file);
        //         fclose(file);
        //         printf("Pobrano plik.\n");
        //     }
        //     else printf("Taki plik nie istnieje.\n");
        //     break;
        // }
        // case 3://zmien nazwe
        // {
        //     printf("Podaj dotychczasowa nazwe pliku: ");
        //     scanf("%s",filename);
        //     strcpy(buffer,"nazwa ");
        //     strcat(buffer, filename);
        //     strcat(buffer, " ");
        //     printf("Podaj nowa nazwe pliku: ");
        //     scanf("%s",filename);
        //     strcat(buffer, filename);
        //     break;
        // }
        // case 4://wyszukaj plik
        // {
        //     printf("Podaj fraze z nazwy pliku: ");
        //     scanf("%s",filename);//np. wpisanie sr* wyswietli pliki (musi byc gwiazdka)
        //     strcpy(buffer,"szukaj \"*");
        //     strcat(buffer, filename);
        //     strcat(buffer, "*\"*");
        //     break;
        // }
        else if(!strcmp(command1, "ls"))//wyswietl pliki
        {
            strcpy(buffer, "ls -l");

            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);


            // break;
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


        // default:
        //      return 0;
        //       // break;

        // if (option!=2 && option!=1)//opcje 1 i 2 maja juz odpowiednie funkcje
        // {
        //     send(clientSocket, buffer, BUFFSIZE,0);
        //     ---- Read the message from the server into the buffer ----
        //     recv(clientSocket, buffer, BUFFSIZE, 0);
        //     /*---- Print the received message ----*/
        //     printf("%s", buffer);
        // }

            bzero(buffer,BUFFSIZE);
            bzero(command1,BUFFSIZE);
            bzero(command2,BUFFSIZE);
            bzero(option,BUFFSIZE);
          
    }

        
        
    return 0;
}
