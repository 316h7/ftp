#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "srv_h.h" 

// #define BUFFSIZE 80000//ZMNIEJSZYC JESLI BEDZIE CORE DUMPED (W SRV TEZ WTEDY)
//nie wiem dlaczego, ale jak dalem buffsize na 3mb, to niby sie wlaczalo,
//ale nie dzialalo wysylanie na serwer (odbieranie z niego dzialalo)
//najwieksza wartosc dla jakiej to wszystko smiga to 80000 (ale nie wiem dlaczego)

int main()
{
    char buffer[BUFFSIZE], kopia [BUFFSIZE], nazwaPliku[255], option[255];
    char command2[BUFFSIZE], command[BUFFSIZE];
    int clientSocket;
    size_t uchwyt=0;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;


        printf("\n\nCo chcesz zrobic? \nsend [file_name] - przeslij plik \nrecieve [file_name]-pobierz plik ");
        printf("\n?-przenies/zmien nazwe \n?-wyszukaj \n?-wylistuj pliki ");
        printf("\npwd-obecny folder \ncd [path] - zmien folder \nhelp - for help  \nexit - for exit \n");
  
        /*---- Create the socket. The three arguments are: ----*/
        /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
        clientSocket = socket(PF_INET, SOCK_STREAM, 0);

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
  

        // scanf("%s",&option);
        // gets(option); 
        printf("\n=> ");
        fgets( option, 255, stdin );
        // printf("%s\n",option );
        sscanf(option,"%s %s", command, command2);//WYBOR CZYNNOSCI W TAKI SPOSOB
        // printf("%s %s\n",option );
        // switch(command)
        // {
        if(!strcmp(command, "send"))//przeslij plik
        {
            // printf("Podaj nazwe pliku do wyslania: ");
            // scanf("%s",nazwaPliku);
            strcpy(buffer, "wyslij ");
            send(clientSocket, buffer, sizeof(buffer),0);

            send_file( command2, clientSocket);
            // else printf("Brak takiego pliku!\n");
            // close(uchwyt);
            // break;
        }

                if(!strcmp(command, "help"))//przeslij plik
        {
        printf("\n\nCo chcesz zrobic? \nsend [file_name] - przeslij plik \nrecieve [file_name]-pobierz plik ");
        printf("\n?-przenies/zmien nazwe \n?-wyszukaj \n?-wylistuj pliki ");
        printf("\npwd-obecny folder \ncd [path] - zmien folder \nhelp - for help  \nexit - for exit \n");
  
        }
        // case 2://pobierz plik
        // {
        //     printf("Podaj nazwe pliku do pobrania: ");
        //     scanf("%s",nazwaPliku);
        //     strcpy(buffer,"pobierz ");
        //     strcat(buffer, nazwaPliku);
        //     strcpy(kopia, buffer);//do sprawdzania, czy na serwerze istnieje plik

        //     send(clientSocket, buffer, BUFFSIZE,0);
        //     /*---- Read the message from the server into the buffer ----*/
        //     recv(clientSocket, buffer, BUFFSIZE, 0);

        //     if(strcmp(buffer, kopia))
        //     {
        //         FILE *file = fopen(nazwaPliku, "w");
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
        //     scanf("%s",nazwaPliku);
        //     strcpy(buffer,"nazwa ");
        //     strcat(buffer, nazwaPliku);
        //     strcat(buffer, " ");
        //     printf("Podaj nowa nazwe pliku: ");
        //     scanf("%s",nazwaPliku);
        //     strcat(buffer, nazwaPliku);
        //     break;
        // }
        // case 4://wyszukaj plik
        // {
        //     printf("Podaj fraze z nazwy pliku: ");
        //     scanf("%s",nazwaPliku);//np. wpisanie sr* wyswietli pliki (musi byc gwiazdka)
        //     strcpy(buffer,"szukaj \"*");
        //     strcat(buffer, nazwaPliku);
        //     strcat(buffer, "*\"*");
        //     break;
        // }
        if(!strcmp(command, "ls"))//wyswietl pliki
        {
            strcpy(buffer, "ls -l");

            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);


            // break;
        }
        if(!strcmp(command, "pwd"))
        {
            strcpy(buffer,"pwd");
            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);
            // break;
        }

        if(!strcmp(command, "cd"))//wyswietl pliki
        {
            strcpy(buffer,"cd ");
            strcat(buffer, command2);
            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);
        }


                if(!strcmp(command, "exit"))//wyswietl pliki
        {


                close(clientSocket);
                return 0;
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
            for (int a=0; a<BUFFSIZE; a++)
            buffer[a]=0;//czyszczenie bufora
    }

        
        
    return 0;
}
