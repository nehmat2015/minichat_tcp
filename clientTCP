 #include <stdio.h>  
 #include <stdlib.h>        
 #include <string.h>            
 #include <unistd.h>            
 #include <sys/types.h>            
 #include <sys/socket.h>           
 #include <netinet/in.h>            
 #include <pthread.h>            
 #include <signal.h>            
 #define MYPORT 8080     
 #define MAXDATALEN 256  
 int      sockfd;  
 int      n,x;                          
 struct sockaddr_in serv_addr;            
 char      buffer[MAXDATALEN];  
 char      buf[10];            
 void *quitproc();  
 void* chat_write(int);  
 void* chat_read(int);  
 void *zzz();  
  
 int main(int argc, char *argv[]){  
  pthread_t thr1,thr2;          
     if( argc != 2 ){       
       printf("S V P saisir le ip de serveur\n");  
       exit(0);  
       }  
      //creation de socket
      sockfd = socket(AF_INET, SOCK_STREAM, 0);  
      if (sockfd == -1)  
        printf ("erreur lors de creation de socket\n");  
      else       
        printf("socket\t\tcree avec succe\n");  
        
      bzero((char *) &serv_addr, sizeof(serv_addr));  
      serv_addr.sin_family = AF_INET;  
      serv_addr.sin_port = htons(MYPORT);  
      serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  
      //nom utilisateur
      bzero(buf,10);  
      printf("\nsaisir votre nom::");  
      fgets(buf,10,stdin); 
       
      __fpurge(stdin);       
      buf[strlen(buf)-1]=':';  
      //connection de client au serveur 
      if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))==-1)  
      {  
        printf("erreur lors de connection de client\n");  
        exit(0);  
      }  
      else  
        printf("%s connection avec succe\n",buf);  
      printf("\rtu es connecte  a - %s",buffer-1);  
      send(sockfd,buf,strlen(buf),0);  
      pthread_create(&thr2,NULL,(void *)chat_write,(void *)sockfd);       
      pthread_create(&thr1,NULL,(void *)chat_read,(void *)sockfd);       
      pthread_join(thr2,NULL);  
      pthread_join(thr1,NULL);  
      return 0;  
 }  
 //lecture avec boucle 
 void* chat_read(int sockfd)  
 {  
      if (signal(SIGINT,(void *)quitproc)==0)  
      if(signal(SIGTSTP, (void *)zzz)==0)  
      while(1)  
      {  
        n=recv(sockfd,buffer,MAXDATALEN-1,0);  
         if(n==0){   
            printf("\nle serveur est arrete \n\n");  
             exit(0);  
             }  
         if(n>0){  
              printf("\n%s ",buffer);  
            bzero(buffer,MAXDATALEN);  
             }  
      }//fin boucle while
 }  
 //ecriture dans le socket 
 void* chat_write(int sockfd)  
 {  
      while(1)  
      {  
        printf("%s",buf);  
          fgets(buffer,MAXDATALEN-1,stdin);  
         if(strlen(buffer)-1>sizeof(buffer)){  
             printf("la taille de buffer est execede ,full\t par  %d caracteres\n",sizeof(buffer));  
             bzero(buffer,MAXDATALEN);  
             __fpurge(stdin);  
             }  
        n=send(sockfd,buffer,strlen(buffer),0);  
         if(strncmp(buffer,"quit",4)==0)  
             exit(0);  
         bzero(buffer,MAXDATALEN);  
      }//fin boucle while 
 }  
  
 void *quitproc(){             
    printf("\rS V P saisir 'quit' pour quiter\n");  
 } 
//handel signal ctrl + z 
 void *zzz(){                
    printf("\rS V P saisir  'quit' pour quiter\n");  
 }  

