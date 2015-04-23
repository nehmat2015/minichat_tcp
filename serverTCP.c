#include <stdio.h>            
 #include <stdlib.h>            
 #include <sys/socket.h>         
 #include <sys/types.h>            
 #include <string.h>           
 #include <netinet/in.h>            
 #include <pthread.h>           
 #include <arpa/inet.h>          
 #include <unistd.h>            
 #include <signal.h>            
 #define BACKLOG 100          
 #define MAXDATALEN 256            
 #define PORT 8080          
 //structure de donnee liste lineaire chaine pour y sauvegarder les clients
   struct Node                         
     {  
       int port;  
        char nomutilisateur[10];  
       struct Node *next;  
     };  
   typedef struct Node *ptrtonode;  
   typedef ptrtonode head;  
   typedef ptrtonode addr; 
      head viderliste( head h );         
      //la methode sendtoclients() sert  a envoyer un message pour tous les clients connectes  
      void sendtousclients(char *,int new_fd);     
      //la methode quitterAll() envoit un message pour tous le client si le serveur quite
      void quitterAll( );   
      //si un client quite on efface son nom utilisateur par cette methode supprimerclient()             
      void supprimerclient( int port, head h );     /*supprimerclient client values on client exit*/  
      //si un client est connecte on insere son nom utilistaeur par cette methode insererclient()
      void insererclient(int port,char*,head h,addr a); 
      void clearlist( head h );         
      void Display( const head h );         
      void *Quitproc( );                
           //instance pour chaque client connecte 
      void *server(void * arg);          
      void estime();  
      char      nomutilisateur[10];           
      int sf2;  
      head h;                    
      char     buffer[MAXDATALEN];  
      
  int main(int argc, char *argv[]) {  
      int      sockfd,new_fd;                 
      int      portnum;                 
      int      taille_client,z;                
      pthread_t      thr; 
      struct sockaddr_in      server_addr;      
      struct sockaddr_in      client_addr;                 
      int      yes=1;  
       addr a;                           
      printf("le serveur a commence\n");  
      //si l'utilisateur a saisit le numero de port sur la ligne de commande alors on le prend dans argv[1]
  if( argc == 2 )       
       portnum = atoi(argv[1]);  
  else 
      // si l'utilisateur n'a pas saist le numero de port sur la ligne de commande , on prend un numero par defaut soit  8080  
      portnum = PORT; 
      printf("PORT NO.:\t%d\n",portnum);  
      h = viderliste( NULL );          //frees the list  
      /*=set info of server =*/  
      server_addr.sin_family=AF_INET;          /* set family to Internet   */  
      server_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* set IP address */  
      server_addr.sin_port=htons(portnum);  
printf("IP ADDRESS:\t%s\n",inet_ntoa(server_addr.sin_addr));         
//printf("IP ADDRESS:\t%s\n",ntoa(server_addr.sin_addr));  
      /*=creating socket=*/  
      sockfd = socket(AF_INET, SOCK_STREAM, 0);  
   if(sockfd == -1){  
      printf("server- socket() error");      
      exit(1);  
   }else  
      printf("socket\t\tcreated.\n");  
   if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {  
      printf("setsockopt error");     
      exit(1);  
   }else printf("reusing\t\tport\n");  
       
   if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))==-1){  
           printf("binding failed\n");     
      exit(1);}  
   else   
      printf("binding\t\tsuccess.\n\n");  
      printf("\t\tPRESS CTRL+z TO VIEW ONLINE CLIENTS\n\n");  
      listen(sockfd, BACKLOG);  
      printf("waiting for clients......\n");  
   if (signal(SIGINT,(void *)Quitproc)==0)      
   if(signal(SIGTSTP, estime)==0)               
   while(1){
       //taille_client est un argument pour la creation de thread
             taille_client=sizeof(struct sockaddr_in); 
// accepte de la connexion d'un client 
             new_fd = accept(sockfd, (struct sockaddr *)&client_addr,&taille_client); 
             a =h ;  
           //le client fait sign in avec un nom utilisateur
           bzero(nomutilisateur,10);            
           if(recv(new_fd,nomutilisateur,sizeof(nomutilisateur),0)>0);  
             nomutilisateur[strlen(nomutilisateur)-1]=':';  
             printf("\t%d->%s jointure de chatroom\n",new_fd,nomutilisateur);  
             sprintf(buffer,"%s est ONLINE\n",nomutilisateur);  
            insererclient( new_fd,nomutilisateur, h, a );       
            //insertion d'un nouveau client socked fd dans la liste lineaire chainee
            a = a->next;  
            
          //notification a tous les clients qui sont online d'un nouveau client   
           a = h ;  
           do{  
             a = a->next;  
             sf2 = a->port;  
           if(sf2!=new_fd)  
           send(sf2,buffer ,sizeof(buffer),0);  
           } while( a->next != NULL );  
              printf("le serveur est en connexion %s & %d\n\n",inet_ntoa(client_addr.sin_addr),new_fd);   
            struct Node args;                          
             args.port=new_fd;  
             strcpy(args.nomutilisateur,nomutilisateur);
         //creation d'un pthread pour tous client connecte  
            pthread_create(&thr,NULL,server,(void*)&args);      
            pthread_detach(thr);  
       } //end pour la boucle while 
   //quand le serveur quite , effacer tous les clients de la liste  
  clearlist(h);                           
  close(sockfd);  
 }
//les fonctions de chaque client sont
 void *server(void * arguments){  
  struct Node *args=arguments;  
  char     buffer[MAXDATALEN],ubuf[50],uname[10];        
  char *strp;            
  char      *msg = (char *) malloc(MAXDATALEN);  
  int      ts_fd,x,y;  
  int      sfd,msglen;  
  ts_fd = args->port;      
  strcpy(uname,args->nomutilisateur);   
  addr      a;  
       //la liste des clients qui sont on line 
       a =h ;  
           do{  
           a = a->next;  
           sprintf( ubuf," %s est online\n",a->nomutilisateur );  
           send(ts_fd,ubuf,strlen(ubuf),0);  
           } while( a->next != NULL );  
      //debut chat  
   while(1){  
        bzero(buffer,256);  
        y=recv(ts_fd,buffer,MAXDATALEN,0);  
       if (y==0)   
       goto jmp;  
        //si un client quite
       if ( strncmp( buffer, "quit", 4) == 0 ){  
      jmp:     printf("%d ->%squite le chat , suppression de la list \n",ts_fd,uname);  
           sprintf(buffer,"%s a quite le chat\n",uname);  
           addr a = h ;  
           do{  
                a = a->next;  
                sfd = a->port;  
                if(sfd == ts_fd)   
                 supprimerclient( sfd, h );  
                if(sfd != ts_fd)   
                 send(sfd,buffer,MAXDATALEN,0);  
            }while ( a->next != NULL );  
                Display( h );  
                close(ts_fd);  
                free(msg);  
             break;  
            }  
      //envoie d'un message a tous les clients 
      printf("%s %s\n",uname,buffer);  
      strcpy(msg,uname);  
      x=strlen(msg);  
      strp = msg;  
      strp+= x;  
      strcat(strp,buffer);  
      msglen=strlen(msg);  
        addr a = h ;  
      do{  
       a = a->next;  
       sfd = a->port;  
      if(sfd != ts_fd)   
        send(sfd,msg,msglen,0);  
      } while( a->next != NULL );  
       Display( h );  
       bzero(msg,MAXDATALEN);  
   }//fin de la boucle while 
    return 0;  
 }//fin de la fonction server  
 //suprimer les clients de la liste
 head viderliste( head h )  
      {  
      if( h != NULL )  
           clearlist( h );  
           h = malloc( sizeof( struct Node ) );  
      if( h == NULL )  
           printf( "Out of memoire!" );  
           h->next = NULL;  
      return h;  
      }  
 
 void clearlist( head h )  
      {  
      addr a, Tmp;  
      a = h->next;   
      h->next = NULL;  
           while( a != NULL )  
           {  
           Tmp = a->next;  
           free( a );  
           a = Tmp;  
           }  
      }  
 
 void insererclient( int port,char *nomutilisateur, head h, addr a )  
      {  
      addr TmpCell;  
      TmpCell = malloc( sizeof( struct Node ) );  
           if( TmpCell == NULL )  
           printf( "Out of space!!!" );  
      TmpCell->port = port;  
      strcpy(TmpCell->nomutilisateur,nomutilisateur);  
      TmpCell->next = a->next;  
      a->next = TmpCell;  
      }  
 //affichage de noms des clients da la liste 
 void Display( const head h )  
      {  
        addr a =h ;  
        if( h->next == NULL )  
      printf( "pas de clients qui sont online\n" );  
        else  
        {  
           do  
           {  
             a = a->next;  
             printf( "%d->%s \t", a->port,a->nomutilisateur );  
           } while( a->next != NULL );  
           printf( "\n" );  
        }  
      }  
 
 void supprimerclient( int port, head h ){  
        addr a, TmpCell;  
        a = h;  
    while( a->next != NULL && a->next->port != port )  
       a = a->next;  
   if( a->next != NULL ){             
      TmpCell = a->next;  
      a->next = TmpCell->next;   
      free( TmpCell );  
   }  
  }  
  
 void *Quitproc(){        
      printf("\n\nSERVER SHUTDOWN\n");  
     quitterAll( );  
     exit(0);  
  }  
 //notification que le serveur a quite  
 void quitterAll(){  
   int sfd;  
   addr a = h ;  
   int i=0;  
    if( h->next == NULL ) {  
     printf( "au revoir pas de clients \n\n" );  
      exit(0);  
     } else {  
        do{  
          i++;  
         a = a->next;  
         sfd = a->port;  
         send(sfd,"server down",13,0);  
         } while( a->next != NULL );  
       printf("%d le client a ferme\n\n",i);         
       }  
  }  
 void estime(){  
   printf("\raffichage des clients online\n\n");  
   Display(h);  
  }  

