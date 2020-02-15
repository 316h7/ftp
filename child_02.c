#include "srv_h.h" 




char* read_command_output(const char* command1)
{
    const int CHUNK = BUFFSIZE;

    FILE* pipe = popen(command1, "r");
    if (pipe == NULL)
    {
        puts("popen failed");
        exit(1);
    }
    int capacity = CHUNK;
    int position = 0;
    char* buffer = malloc(capacity);
    if (buffer == NULL)
    {
        puts("malloc failed");
        exit(1);
    }
    for (;;)
    {
        if (position + CHUNK > capacity)
        {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
            if (buffer == NULL)
            {
                puts("realloc failed");
                exit(1);
            }
        }
        int bytes_read = fread(buffer + position, 1, CHUNK, pipe);
        position += bytes_read;
        if (bytes_read < CHUNK)
        {
            break;
        }
    }
    buffer = realloc(buffer, position + 1);
    if (buffer == NULL)
    {
        puts("realloc failed");
        exit(1);
    }
    buffer[position] = '\0';

    pclose(pipe);
    return buffer;
}





 void cmd_child(int serverSocket){


	int  zmienNazwe=0;
    int k, sndbuf;
    char* komenda;
	char buf[BUFFSIZE], command1[BUFFSIZE];
    char command2[BUFFSIZE], command3[BUFFSIZE];

/*----pobranie komendy----*/
			// chroot("~/"); 
             // chdir("~/");
             // chroot("~/ftp"); 
again:

           recv(serverSocket, buf, BUFFSIZE, 0);// > 0//  if ( ) {
            //Rozpoznanie komendy

            sscanf(buf, "%s %s %s", command1, command2, command3);//WYBOR CZYNNOSCI W TAKI SPOSOB
            if(!strcmp(command1, "ls"))//TAK SPRAWDZAMY CO MA SIE STAC, nie musi byc to komenda
            {
                strcat(command1," ");
                strcat(command1, command2);
                komenda=read_command_output(command1);
                strcpy(buf, komenda);
                send(serverSocket, buf, BUFFSIZE,0);
            }
            else if(!strcmp(command1, "szukaj"))  //TAK SPRAWDZAMY, nie musi byc to komenda
            {
                strcpy(command1,"find -name ");
                strcat(command1,command2);
                komenda=read_command_output(command1);
                sprintf(buf, "%s", komenda);
                send(serverSocket, buf, BUFFSIZE,0);
            }
            else if(!strcmp(command1, "nazwa"))
            {
                zmienNazwe=rename(command2, command3);
                if (zmienNazwe==-1)
                    strcpy(buf,"Nie ma takiego pliku lub inny blad.\n");
                else strcpy(buf,"Sukces!\n");
                send(serverSocket, buf, BUFFSIZE,0);
            }
            else if(!strcmp(command1, "pwd"))
            {
            	// chroot("~/"); 
				// chdir("~/");
            	getcwd(cwd, sizeof(cwd));
                // komenda=read_command_output(command1);

                strcpy(buf, cwd);
                send(serverSocket, buf, BUFFSIZE,0);
            }
            else if(!strcmp(command1, "recv"))
            {


            if ( access( command2, F_OK ) != -1 ){   // file exists
                strcpy(buf, "F_OK");
                send(serverSocket, buf, BUFFSIZE,0);    
            
            printf("Sending... \n" );
            send_file(command2, serverSocket);
            printf("OK \n" );}
  
            else // file doesn't exist
            {
                strcpy(buf, "@ no such file or directory");
                send(serverSocket, buf, BUFFSIZE,0);

            } 

            }




            else if(!strcmp(command1, "send"))
            {
            	printf("Sending... \n" );
                /*---- Read the message from the server into the buffer ----*/
                recv_file( serverSocket );
                printf("OK \n" );
           			 // }
}
            else if(!strcmp(command1, "cd"))
            {
            	// chroot(command2); 
				if (chdir(command2) == -1){

                strcpy(cwd, "@ no such file or directory");
                // strcpy(buf, cwd);
                send(serverSocket, cwd, BUFFSIZE,0);
                }
                else{
            	getcwd(cwd, sizeof(cwd));
                // komenda=read_command_output(command1);

                strcpy(buf, cwd);
                send(serverSocket, buf, BUFFSIZE,0);}
            }

            
            bzero(buf,BUFFSIZE);
            bzero(command1,BUFFSIZE);
            bzero(command2,BUFFSIZE);
            bzero(command3,BUFFSIZE);
            


  goto again;
 };


 void child_main(int i, int listenfd, int addrlen){
 int serverSocket;

 socklen_t clilen;
 struct sockaddr *cliaddr;
 cliaddr = malloc(addrlen);
 // printf("child %ld starting\n", (long) getpid());
 	for ( ; ; ) {
 	clilen = addrlen;
 	serverSocket = accept(listenfd, cliaddr, &clilen);
 	cmd_child(serverSocket); /* process the request */
 	close(serverSocket);
 }
 }


 pid_t child_make(int i, int listenfd, int addrlen){
 pid_t pid;

 void child_main(int, int, int);
 
 if ( (pid = fork()) > 0)
 	return (pid); /* parent */

 child_main(i, listenfd, addrlen);}  /* never returns */




int daemon_init(const char *pname, int facility, uid_t uid, int socket)
{
    int     i, p;
    pid_t   pid;

    if ( (pid = fork()) < 0)
        return (-1);
    else if (pid)
        exit(0);            /* parent terminates */

    /* child 1 continues... */

    if (setsid() < 0)           /* become session leader */
        return (-1);

    signal(SIGHUP, SIG_IGN);
    if ( (pid = fork()) < 0)
        return (-1);
    else if (pid)
        exit(0);            /* child 1 terminates */

    /* child 2 continues... */

    // chdir("/tmp");              /* change working directory  or chroot()*/
 chroot(getenv("HOME"));
chdir(getenv("HOME"));
    /* close off file descriptors */
    for (i = 0; i < MAXFD; i++){
        if(socket != i )
            close(i);
    }

    /* redirect stdin, stdout, and stderr to /dev/null */
    p= open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    openlog(pname, LOG_PID, facility);
    
        syslog(LOG_ERR," STDIN =   %i\n", p);
    setuid(uid); /* change user */
    
    return (0);             /* success */
}


