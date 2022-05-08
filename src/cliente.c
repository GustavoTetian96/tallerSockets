#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h> //man 3 inet_addr
#include <string.h>

void valida_error(int res, int sockfd,char *msg);
void valida_socket(int sockfd, char *msg);
int main(int argc, char **argv){
	//puerto a usar
	if(argc<4){
		printf("falta argumentos\nEjemplo de implementacion: ./bin/ejecutable 127.0.0.1 hola\n");
		exit(1);
	}
	unsigned short puerto=(unsigned short)atoi(argv[2]);
	//socket
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	valida_socket(sockfd,"falla en sockfd\n");
	printf("sockfd=%d\n",sockfd);
	//esctructura de direccion
	struct sockaddr_in direccion;
	direccion.sin_family=AF_INET;
	direccion.sin_port=htons(puerto);
	direccion.sin_addr.s_addr=inet_addr(argv[1]);
	//bind
	int connectfd=connect(sockfd, (struct sockaddr *)&direccion, sizeof(direccion));
	valida_error(connectfd,sockfd,"Falla de connect\n");
	//enviar y recibir informacion
	char *s=argv[3];
	//enviar la cadena s al servidor
	connectfd=write(sockfd,s, strlen(s));
	valida_error(connectfd,sockfd,"Falla en connectfd para write\n");
	//leer respuesta de servidor
	char bufc[100]={0};
	connectfd=read(sockfd,bufc,100);
	valida_error(connectfd,sockfd,"falla en connectfd para read");
	printf("%s\n",bufc);
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
