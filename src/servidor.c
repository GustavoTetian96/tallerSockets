#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#define TAM_BUF 1000
#define TAM_RUTA 1000
#define TAM_COLA 1000
//./servidor ip puerto directorio_de_archivos
//

void valida_error(int res, int sockfd,char *msg);
void valida_socket(int sockfd, char *msg);
void enviar_archivo(int sockfd_conectado,char *ruta,char *buf);
void listar_directorio(int sockfd_conectado,char *rutadir);
int Acepta_Conexion_Cliente (int Descriptor);
int main(int argc, char **argv){
	//puerto a usar
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
	char bufarc[1000]={0};
	//bind
	int bindfd=bind(sockfd, (struct sockaddr *)&direccion, sizeof(direccion));
	valida_error(bindfd,sockfd,"Falla de bind\n");
	//listen
	bindfd=listen(sockfd, TAM_COLA);
	valida_error(bindfd,sockfd,"Falla en listen\n");
	while(1){
		int sockfd_conectado=Acepta_Conexion_Cliente (sockfd);
		valida_error(sockfd_conectado,sockfd,"falla en sockfd_conectado para accept\n");
		char buf[100]={0};
		char ruta[TAM_RUTA]={0};
		bindfd=read(sockfd_conectado,buf,100);
		valida_error(bindfd,sockfd_conectado,"fallo en read\n");
		if(strncmp(buf,"_listar_",8)==0){
			listar_directorio(sockfd_conectado,argv[3]);
		}else{
			strcat(ruta,argv[3]);
			strcat(ruta,buf);
			enviar_archivo(sockfd_conectado,ruta,bufarc);
		}
		printf("solicitud: %s\n",buf);
		memset(buf,0,100);
		close(sockfd_conectado);
	}
	close(sockfd);
	printf("sockfd fue cerrardo");
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
void enviar_archivo(int sockfd_conectado,char *ruta,char *buf){
	printf("enviando desde: %s\n",ruta);
	int numbytes_leidos=0,env=0;
	size_t cuenta=TAM_BUF;
	char *aviso="no_existe";
	int fdwrite,fdread;
	int fdopen=open(ruta,O_RDONLY);
		if(fdopen==-1){
			perror("Problema al abrir el archivo o no existe ese archivo\n");
			write(sockfd_conectado,aviso,strlen(aviso));
			exit(1);
		}
		do{
			fdread=read(fdopen,buf,cuenta);
			fdwrite=write(sockfd_conectado,buf,fdread);
			valida_socket(fdwrite,"fallo write para enviar cliente");
			numbytes_leidos+=fdread,env+=fdwrite;
			lseek(fdopen,numbytes_leidos,SEEK_SET);
			memset(buf,0,1000);
		}while(fdread!=0);
		printf("enviados %d bytes\n",env);
		close(fdopen);
}
void listar_directorio(int sockfd_conectado,char *rutadir){
	DIR *d;
	struct dirent *dir;
	d=opendir(rutadir);
	char buf[100]={0};
	int fdwrite;
	if(d){
		while((dir = readdir(d))!=NULL){
			if(!(strncmp(dir->d_name,".",1)==0) && !(strcmp(dir->d_name,"..")==0)){
				strcat(buf,dir->d_name);
				printf("%s\n",buf);
				fdwrite=write(sockfd_conectado,buf,strlen(buf));
				valida_socket(fdwrite,"falla en fdwrite en listar_directorio\n");
				memset(buf,0,100);
			}
		}
	}
	fdwrite=write(sockfd_conectado,"_fin",4);
	valida_socket(fdwrite,"falla en fdwrite en _fin");
	closedir(d);
}
int Acepta_Conexion_Cliente (int Descriptor)
{
	socklen_t Longitud_Cliente;
	struct sockaddr Cliente;
	int Hijo;
	Longitud_Cliente = sizeof (Cliente);
	Hijo = accept (Descriptor, &Cliente, &Longitud_Cliente);
	if (Hijo == -1)
		return -1;
	return Hijo;
}