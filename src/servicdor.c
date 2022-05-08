#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> //man 3 inet_addr
#include <string.h>
#define TAM_COLA 4

void valida_error(int res, int sockfd,char *msg);
void valida_socket(int sockfd, char *msg);
int main(int argc, char **argv){
	//puerto a usar
	unsigned short puerto=(unsigned short)atoi(argv[2]);
	//socket
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	valida_socket(sockfd,"falla en sockfd\n");
	printf("sockfd=%d\n",sockfd);
	char *mensj="conexion exitosa, no hay saludo";
	//esctructura de direccion
	struct sockaddr_in direccion;
	direccion.sin_family=AF_INET;
	direccion.sin_port=htons(puerto);
	direccion.sin_addr.s_addr=inet_addr(argv[1]);
	//bind
	int bindfd=bind(sockfd, (struct sockaddr *)&direccion, sizeof(direccion));
	valida_error(bindfd,sockfd,"Falla de bind\n");
	//listen
	bindfd=listen(sockfd, TAM_COLA);
	valida_error(bindfd,sockfd,"Falla en listen\n");
	while(1){
		int sockfd_conectado=accept(sockfd, NULL, 0);
		valida_error(sockfd_conectado,sockfd,"falla en sockfd_conectado para accept\n");
		char buf[100]={0};
		bindfd=read(sockfd_conectado,buf,100);
		valida_error(bindfd,sockfd_conectado,"fallo en read\n");
		int fdwrite;
		if(strcmp(buf,"hola servidor saludame")==0){
			//printf("%s\n",buf);
			fdwrite=write(sockfd_conectado,"te saludo bien",14);
			valida_socket(fdwrite,"fallo en write\n");
		}else{
			fdwrite=write(sockfd_conectado,mensj,strlen(mensj));
		}
		close(sockfd_conectado);
	}
	close(sockfd);
}
void valida_error(int res, int sockfd,char *msg){
	if(res<0){
		perror("err->");
		printf("%s\n",msg);
		close(sockfd);
		exit(1);
	}
}
void valida_socket(int sockfd, char *msg){
	if(sockfd<0){
		perror("err->");
		printf("%s\n",msg);
		exit(1);
	}
}
