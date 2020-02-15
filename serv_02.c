#include "srv_h.h" 




 static int nchildren;
 static pid_t *pids;





 // if (argc == 3)
 // listenSocket = Tcp_listen(NULL, argv[1], &addrlen);
 // else if (argc == 4)
 // listenSocket = Tcp_listen(argv[1], argv[2], &addrlen);
 // else
 // err_quit("usage: serv02 [ <host> ] <port#> <#children>");
 // nchildren = atoi(argv[argc - 1]);

  int  main(int argc, char **argv)
 {


 	nchildren=3;

    int listenSocket, serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int k; 
    int one=1;
    socklen_t clilen;
    pid_t pid;


    int i;
  	socklen_t addrlen;



    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == -1)
    {
        printf("Socket creation failed");
        exit(1);
    }
    serverAddr.sin_family = AF_INET;

    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(7891);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  


    if (setsockopt(listenSocket, SOL_TCP, TCP_NODELAY, &one, sizeof(one)) < 0)
    {
        printf("setsockopt error");
    };

    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)
    {
        printf("setsockopt error");
    }
    /*---- Bind the address struct to the socket ----*/
    k = bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (k == -1)
    {
        printf("Binding error");
        exit(1);
    }
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if (listen(listenSocket, LISTENQ) == -1)
    {
        printf("Listening error\n");
        exit(1);
    }
    /*---- Accept call creates a new socket for the incoming connection ----*/
    /*---- Send message to the socket of the incoming connection ----*/
    /*----Koniec inicjalizacji, poczatek obslugi komend ----*/
    // daemon_init(argv[0], LOG_USER, 1000, listenSocket);
    // syslog (LOG_NOTICE, "Program started by User %d", getuid ());
    // syslog (LOG_INFO,"Waiting for clients ... ");
    printf("Serwer jest uruchomiony.\n");




 addrlen = sizeof(clientAddr);
 pids = calloc(nchildren, sizeof(pid_t));

 for (i = 0; i < nchildren; i++)
 pids[i] = child_make(i, listenSocket, addrlen); /* parent returns */

 // signal(SIGINT, sig_int);
 for ( ; ; )
 pause(); /* everything done by children */
 }