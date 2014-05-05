
// programa cliente7.cpp --> ler um vetor de caracter. Protocolo UDP


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int main( )
{
   int sockfd , i;
   int length;
   socklen_t len_recv;
   struct sockaddr_in address;
   int result;
   char vetor_ch[] = "Mensagem a ser enviada\n";
   struct timeval t1,t2;
   int repeticoes_ping_pong;
   float tempo_medio;
   unsigned short porta = 9734;

   sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket

   printf("Digite o numero de repeticoes:");
   scanf("%i", &repeticoes_ping_pong);
   
   
   //recebendo o IP
    char *ip_address = NULL;
    ssize_t read;
    size_t len=0;
	printf("Digite o endereço ip:(XXX.XXX.XXX.XXX) \n ");
    read = getline(&ip_address, &len, stdin);
    if (-1 != read)
        puts(ip_address);
    else
        printf("Endereco nao lido...\n");
   
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr(ip_address);
   address.sin_port = htons(porta);
  
   length = sizeof(address);
   len_recv = sizeof(address);
   
   //~ printf( "\n\nString Original = %s\n",vetor_ch);
	
   tempo_medio=0;
   for(i=0;i<repeticoes_ping_pong;i++){
		
	   gettimeofday(&t1,NULL);
	   sendto(sockfd,  vetor_ch,sizeof(vetor_ch),0,(struct sockaddr *) &address, length);
	   recvfrom(sockfd, vetor_ch,sizeof(vetor_ch),0,(struct sockaddr *) &address,&len_recv);
	   gettimeofday(&t2,NULL);
	
	
	   //~ printf( "String vinda do servidor = %s\n\n",vetor_ch);
	   float time = (t2.tv_sec + t2.tv_usec*0.000001) - (t1.tv_sec + t1.tv_usec*0.000001);
	   //~ printf("\n\nTempo gasto (Enviar+Receber+Percurso): %f segundos\n\n\n",time );
	  
	  tempo_medio+=time;
	}
	
   printf("\n\nTotal de experimentos (Ping-Pong) realizados: %d\n\n",repeticoes_ping_pong);
   printf("\nTempo total da execução: %f segundos\n\n",tempo_medio);
   printf("\nTempo médio do Ping-Pong  (Send e Recv):  %f/%d =  %f segundos\n\n",tempo_medio,repeticoes_ping_pong,tempo_medio/repeticoes_ping_pong);
   printf("\nTempo médio do Envio  (Send): (%f)/2  =%f segundos\n\n\n",tempo_medio/repeticoes_ping_pong,(tempo_medio/repeticoes_ping_pong)/2);
  
  free(ip_address);
  close(sockfd);
  exit(0);
  
}
