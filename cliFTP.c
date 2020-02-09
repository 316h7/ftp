#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFSIZE 80000//ZMNIEJSZYC JESLI BEDZIE CORE DUMPED (W SRV TEZ WTEDY)
//nie wiem dlaczego, ale jak dalem buffsize na 3mb, to niby sie wlaczalo,
//ale nie dzialalo wysylanie na serwer (odbieranie z niego dzialalo)
//najwieksza wartosc dla jakiej to wszystko smiga to 80000 (ale nie wiem dlaczego)

int main()
{
    char buffer[BUFFSIZE], kopia [BUFFSIZE], nazwaPliku[255];
    int option=0, clientSocket;
    size_t uchwyt=0;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
  
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
        printf("\n\nCo chcesz zrobic? (1-przeslij plik, 2-pobierz plik, ");
        printf("3-przenies/zmien nazwe, 4-wyszukaj, 5-wylistuj pliki, ");
        printf("6-obecny folder, 7-zmien folder,  inne-zakoncz):\n");
        scanf("%d",&option);
        switch(option)
        {
        case 1://przeslij plik
        {
            printf("Podaj nazwe pliku do wyslania: ");
            scanf("%s",nazwaPliku);
            strcpy(buffer, "wyslij ");
            strcat(buffer, nazwaPliku);
            send(clientSocket, buffer, BUFFSIZE,0);
            uchwyt = open(nazwaPliku, O_RDONLY);
            if (uchwyt != -1)
            {
                sendfile(clientSocket, uchwyt, NULL, BUFFSIZE);
                printf("Wyslano plik!\n");
            }
            else printf("Brak takiego pliku!\n");
            close(uchwyt);
            break;
        }
        case 2://pobierz plik
        {
            printf("Podaj nazwe pliku do pobrania: ");
            scanf("%s",nazwaPliku);
            strcpy(buffer,"pobierz ");
            strcat(buffer, nazwaPliku);
            strcpy(kopia, buffer);//do sprawdzania, czy na serwerze istnieje plik

            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);

            if(strcmp(buffer, kopia))
            {
                FILE *file = fopen(nazwaPliku, "w");
                fputs(buffer, file);
                fclose(file);
                printf("Pobrano plik.\n");
            }
            else printf("Taki plik nie istnieje.\n");
            break;
        }
        case 3://zmien nazwe
        {
            printf("Podaj dotychczasowa nazwe pliku: ");
            scanf("%s",nazwaPliku);
            strcpy(buffer,"nazwa ");
            strcat(buffer, nazwaPliku);
            strcat(buffer, " ");
            printf("Podaj nowa nazwe pliku: ");
            scanf("%s",nazwaPliku);
            strcat(buffer, nazwaPliku);
            break;
        }
        case 4://wyszukaj plik
        {
            printf("Podaj fraze z nazwy pliku: ");
            scanf("%s",nazwaPliku);//np. wpisanie sr* wyswietli pliki (musi byc gwiazdka)
            strcpy(buffer,"szukaj \"*");
            strcat(buffer, nazwaPliku);
            strcat(buffer, "*\"*");
            break;
        }
        case 5://wyswietl pliki
        {
            strcpy(buffer, "ls -l");
            break;
        }
        case 6: //obecny folder
        {
            strcpy(buffer,"pwd");
            break;
        }

           case 7: //obecny folder
        {
            strcpy(buffer,"cd ");
            scanf("%s",nazwaPliku);//np. wpisanie sr* wyswietli pliki (musi byc gwiazdka)
            // strcpy(buffer,"szukaj \"*");
            strcat(buffer, nazwaPliku);
            break;
        }

        default:
             return 0;
              // break;
        }
        if (option!=2 && option!=1)//opcje 1 i 2 maja juz odpowiednie funkcje
        {
            send(clientSocket, buffer, BUFFSIZE,0);
            /*---- Read the message from the server into the buffer ----*/
            recv(clientSocket, buffer, BUFFSIZE, 0);
            /*---- Print the received message ----*/
            printf("%s", buffer);
        }
        for (int a=0; a<BUFFSIZE; a++)
            buffer[a]=0;//czyszczenie bufora
    }
    return 0;
}
