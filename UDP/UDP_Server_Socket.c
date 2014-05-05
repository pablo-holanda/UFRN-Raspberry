// Programa Servidor7.cpp - Servidor que lê e escreve em um vetor de caracter
// Protocolo UDP

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>


int main( )
{
   int i;
   int server_sockfd, client_sockfd;
   size_t server_len;
   socklen_t client_len;
   struct sockaddr_in server_address;
   struct sockaddr_in client_address;
   struct timeval t1,t2;
	
   int porta = 9734;
   char resposta;

      
   while((resposta!='y')&&(resposta!='n')&&(resposta!= 'Y')&&(resposta!= 'N')){
	   
		printf("\n\nUtilizar porta padrão (%d) ?  (y/n)",porta);
		scanf(" %c",&resposta);
   }
   
   
   if((resposta=='N')||(resposta=='n')){
	   printf("\n\nDigite a porta: ");
	   scanf("%d",&porta);
   }
   

   unlink("server_socket");  // remocao de socket antigo
   
   //protocol (Terceiro argumento de socket): 0 - IP, 1 - ICMP, 6 - TCP, 17 - UDP
   if ( (server_sockfd = socket(AF_INET, SOCK_DGRAM, 0) )  < 0  )  // cria um novo socket
     {
       printf(" Houve erro na ebertura do socket ");
       exit(1);
     }
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(porta);
 

   server_len = sizeof(server_address);
  
   if(  bind(server_sockfd, (struct sockaddr *) &server_address, server_len) < 0 )
    {
      perror("\nHouve error no Bind\n");
      exit(1);
    }
   
 
   //listen(server_sockfd, 5);
   
   while(1){
	   
      char vetor_ch[50];
      int tamanho_String = sizeof(vetor_ch);

      printf("Servidor esperando ...\n");
      
      //client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
      client_len = sizeof(client_address);
      
      
      
      
      
	  gettimeofday(&t1,NULL);
	  
	  
	  
	  
      if(recvfrom(server_sockfd, vetor_ch, sizeof(vetor_ch),0,(struct sockaddr *) &client_address, &client_len) < 0 ){
          perror(" erro no RECVFROM( )");
          exit(1);
        } 
       printf("Recebimento feito com sucesso ...\n");
        
        
       gettimeofday(&t2,NULL);
	   //~ float time = (t2.tv_sec + t2.tv_usec*0.000001) - (t1.tv_sec + t1.tv_usec*0.000001);
	   //~ printf("\n\nTEMPO: %f segundos\n\n",time );
        
        
        
        

      
      sendto(server_sockfd, vetor_ch, sizeof(vetor_ch),0,(struct sockaddr *) &client_address,sizeof(struct sockaddr));
      printf("Envio feito com sucesso ...\n");
      // close(server_sockfd);
   
   }
}
