#include "srv_h.h" 


int send_file( char *fname, int c){


int ch;
   char toSEND[1];
   	   char *sname;
   char *nm = strtok (fname,"/");
    
      while (nm != 0){
        sname =nm;    
        nm = strtok (0,"/");
   }    printf("%s\n", sname );



send(c,sname,BUFFSIZE,0);


     FILE *file;
  file = fopen( fname, "rb" ); 
  if (!file)
  {
    printf(" No such file \n" );
    return -1;
  }
  fseek(file , 0 , SEEK_END);                          // устанавливаем позицию в конец файла
  int lSize = ftell(file);                            // получаем размер в байтах
  rewind (file);                                      // устанавливаем указатель в конец файла
char * buffer = malloc(lSize);
int b = 0;
printf("%d\n", lSize );
int *ptrvar;        
     ptrvar = &lSize;

 

int file_desc = fileno(file);
 
 // struct stat  obj;
 //  stat(fname, &obj);
 // int file_desc = open(fname, O_RDONLY| O_BINARY,3);   
   
 // int file_size = obj.st_size;
 // int file_size = file_desc.st_size;
// printf("%d\n", file_size );
 send(c, ptrvar, sizeof(int), 0);
 fread(buffer, lSize, 1, file);
 printf("send\n");
// fwrite(buffer,lSize, 1,file);
 // sendfile(c, file_desc, NULL, lSize);
 int nbytes = send(c, buffer, lSize, 0);

 printf("sended %d\n",nbytes );
// while((ch=getc(file))!=EOF){
//         toSEND[0] = ch;
//         send(c, toSEND, 1, 0);
        // printf("%s\n",toSEND );
 printf("close\n");
// }
      // if(feof(file)){
      // 	printf("feof");
      // 	// break;
      // }
fclose(file);
close(file_desc);
free(buffer);
return 0;
}






int recv_file(int new_s ){


  char *fname=(char *)malloc(BUFFSIZE);
	int s;

recv(new_s, fname, BUFFSIZE,0);
  
//sscanf(argv[2], "%s", fname);
printf("%s\n",fname );
FILE *file;
file = fopen( fname, "wb" );
  if (!file)
  {
    printf("Can't create a file \n" );
    return -1;
  }

rewind (file);

int *file_size;        // объявление указателя
int i_size;
file_size=&i_size;
recv(new_s, file_size, sizeof(int),0);  //size of recivied file

 //printf("size%d --- %d \n ",i_size, *file_size );
int ccc;
// char * buffer = (char*) malloc(sizeof(char) * i_size);
int nbytes=0;
int recivied=0;
char toRECV[1];
printf("size- %d\n",i_size );
// buffer[0]=0;


    char * buffer = malloc(i_size);
    // int file_desc = fileno(file);
    while(recivied<i_size){
    nbytes=recv(new_s, buffer, i_size, 0);
    printf("recivied %d\n",nbytes );
    recivied=nbytes+recivied;
    // write(file_desc, buffer, i_size);
    fwrite (buffer , nbytes , 1 , file);}

    fclose(file);
    free(buffer);
    free(fname);

    return 0;

}



struct sockaddr_in multicast_cli()
{   
   struct sockaddr_in ret_addr; 
   struct sockaddr_in addr;
   int addrlen, sock, cnt;
   struct ip_mreq mreq;
   char message[INET_ADDRSTRLEN];
   unsigned short port_n;
   /* set up socket */
   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) {
     perror("socket");
     exit(1);
   }
   bzero((char *)&addr, sizeof(addr));
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(MULTICAST_PORT);
   addrlen = sizeof(addr);


      /* receive */
      if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {        
         perror("bind");
     exit(1);
      }    
      mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);         
      mreq.imr_interface.s_addr = htonl(INADDR_ANY);         
      if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
             &mreq, sizeof(mreq)) < 0) {
     perror("setsockopt mreq");
     exit(1);
      }         
     
     cnt = recvfrom(sock, message, sizeof(message), 0, 
            (struct sockaddr *) &addr, &addrlen);

     cnt = recvfrom(sock, &port_n, sizeof(port_n), 0, 
            (struct sockaddr *) &addr, &addrlen);

     if (cnt < 0) {
        perror("recvfrom");
        exit(1);
     } 
     // printf("%s:%d\n",message ,port_n );
    ret_addr.sin_addr.s_addr = inet_addr(message);
    ret_addr.sin_port = htons(port_n);

    close(sock);

    return ret_addr;
 
    
}


void multicast_srv( struct sockaddr_in send_addr)    // e.g. htons(3490)
{  
   unsigned short port_n;
   // char port_buf[sizeof(unsigned short)];
   char buf[INET_ADDRSTRLEN]; 
   struct sockaddr_in addr;
   int addrlen, sock, cnt;
   struct ip_mreq mreq;
   char message[50];

   /* set up socket */


   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) {
     perror("socket");
     exit(1);
   }
   bzero((char *)&addr, sizeof(addr));
   addr.sin_family = AF_INET;
   // addr.sin_addr.s_addr = htonl(INADDR_ANY);

   addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP); 
   addr.sin_port = htons(MULTICAST_PORT);
   addrlen = sizeof(addr);
  

  
      /* send */
   // ntohs(addr.sin_port)  ntohs(port_n)
   
    // sprintf(port_buf, "%d", port_n);
    // memcpy(port_buf, &port_n, sizeof(unsigned short));
     port_n=ntohs(send_addr.sin_port);
     inet_ntop(AF_INET, &send_addr.sin_addr.s_addr, buf, sizeof(buf));
      // printf("inet addr: %s\n", buf);

      // if (inet_ntop(AF_INET, &addr.sin_port, buf, sizeof(buf)) != NULL)
      // printf("inet port: %d\n", port_n);

     
      
      while (1) {
     time_t t = time(0);
     // printf("sending: %s\n",buf);
     cnt = sendto(sock, buf, sizeof(buf), 0,
              (struct sockaddr *) &addr, addrlen);
     if (cnt < 0) {
        perror("sendto");
        exit(1);
     }

     // printf("sending: %d\n",port_n);
          cnt = sendto(sock, &port_n, sizeof(port_n), 0,
              (struct sockaddr *) &addr, addrlen);
     if (cnt < 0) {
        perror("sendto");
        exit(1);
}


     sleep(5);
      }

}