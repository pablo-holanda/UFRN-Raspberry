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
   int repeticoes_loop_back = 10000;
   float tempo_medio;

      
   while((resposta!='y')&&(resposta!='n')&&(resposta!= 'Y')&&(resposta!= 'N')){
	   
		printf("\n\nUtilizar porta padrão (%d) ?  (y/n): ",porta);
		scanf(" %c",&resposta);
   }
   
   
   if((resposta=='N')||(resposta=='n')){
	   printf("\n\nDigite a porta: ");
	   scanf("%d",&porta);
   }
   

   unlink("server_socket");  // remocao de socket antigo
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
      perror("Houve error no Bind");
      exit(1);
    }
   
 
   //listen(server_sockfd, 5);
   
   tempo_medio=0;
   for(i=0;i<repeticoes_loop_back;i++){
	   
      char vetor_ch[50] ="Matheus Fernandes Torquato";
      int tamanho_String = sizeof(vetor_ch);

      
      //client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);
      client_len = sizeof(client_address);
      
      
      
	  //~ printf( "\nString Enviada = %s\n",vetor_ch);
	  gettimeofday(&t1,NULL);
	  
	  sendto(server_sockfd, vetor_ch, sizeof(vetor_ch),0,(struct sockaddr *) &server_address,sizeof(struct sockaddr));
	  
      if(recvfrom(server_sockfd, vetor_ch, sizeof(vetor_ch),0,(struct sockaddr *) &server_address, &client_len) < 0 ){
          perror(" erro no RECVFROM( )");
          exit(1);
        }
         
       gettimeofday(&t2,NULL);
       
       //~ printf( "\nString recebida = %s\n\n",vetor_ch);
       
       
	   float time = (t2.tv_sec + t2.tv_usec*0.000001) - (t1.tv_sec + t1.tv_usec*0.000001);
	   //~ printf("\n\nTEMPO: %f segundos\n\n",time );
        //~ if((i>((repeticoes_loop_back-10)))||(i<10)){
			//~ printf("\n\nTEMPO: %f segundos\n\n",time);//Imprime os 10 primeiros e os 10 ultimos tempos
		//~ }
        
        tempo_medio+=time;
        
      

      // close(server_sockfd);
   }
   printf("\n\nTotal de experimentos (Loopback's) realizados: %d\n\n",repeticoes_loop_back);
   printf("\nTempo total da execução: %f segundos\n\n",tempo_medio);
   printf("\nTempo médio do loopback  (Send e Recv):  %f/%d =  %f segundos\n\n",tempo_medio,repeticoes_loop_back,tempo_medio/repeticoes_loop_back);
   printf("\nTempo médio do loopback/2  (Send ou Recv): %f segundos\n\n\n",(tempo_medio/repeticoes_loop_back)/2);
   
}
