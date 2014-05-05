
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
   int sockfd;
   int length;
   socklen_t len_recv;
   struct sockaddr_in address;
   int result;
   char vetor_ch[] = "Mensagem a ser enviada.\n";
   struct timeval t1,t2;

   unsigned short porta = 9734;

   sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    char *ip_address = NULL;
    ssize_t read;
    size_t len;
	printf("Digite o endereco ip:(XXX.XXX.XXX.XXX) \n ");
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
   
   printf( "\n\nString Original = %s\n",vetor_ch);
	
   gettimeofday(&t1,NULL);
   sendto(sockfd,  vetor_ch,sizeof(vetor_ch),0,(struct sockaddr *) &address, length);
   recvfrom(sockfd, vetor_ch,sizeof(vetor_ch),0,(struct sockaddr *) &address,&len_recv);
   gettimeofday(&t2,NULL);
  // send(sockfd,  vetor_ch,sizeof(vetor_ch),0);
  // recv(sockfd, vetor_ch,sizeof(vetor_ch),0);
  //write(sockfd,  vetor_ch,sizeof(vetor_ch));
  //read(sockfd, vetor_ch,sizeof(vetor_ch));

  //~ printf( "Caracter vindo do servidor = %c %c %c %c\n",vetor_ch[0],vetor_ch[1],vetor_ch[2],vetor_ch[3]);
  printf( "String vinda do servidor = %s\n\n",vetor_ch);
  float time = (t2.tv_sec + t2.tv_usec*0.000001) - (t1.tv_sec + t1.tv_usec*0.000001);
  printf("\n\nTempo gasto (Enviar+Receber+Percurso): %f segundos\n\n\n",time );
  
  
  close(sockfd);
  exit(0);
}
