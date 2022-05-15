#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#define TAM 1000
//./cliente ip puerto
//
void valida_error(int res, int sockfd,char *msg);
void valida_socket(int sockfd, char *msg);
int menu();
void leer_lista(int sockfd);
char *nombre_arch(char *name);
//void descargar_archivo(char *nombre,int sockfd, char *buff,int completar);
int main(int argc, char **argv){
	if(argc<3){
		printf("no se ingresan el numero de argumentos necesarios\n");
		exit(1);
	}
	unsigned short puerto=(unsigned short)atoi(argv[2]);
	//socket
	
	//printf("sockfd=%d\n",sockfd);
	//esctructura de direccion
	struct sockaddr_in direccion;
	direccion.sin_family=AF_INET;
	direccion.sin_port=htons(puerto);
	direccion.sin_addr.s_addr=inet_addr(argv[1]);
	
	//bind
	
	char nombre[50]={0};
	int opcion;
	while(1){
		opcion=menu();
		switch(opcion){
			case 1:{
					int sockfd1=socket(AF_INET,SOCK_STREAM,0);
					valida_socket(sockfd1,"falla en sockfd\n");
					int connectfd=connect(sockfd1, (struct sockaddr *)&direccion, sizeof(direccion));
					valida_error(connectfd,sockfd1,"Falla de connect\n");
					if((write(sockfd1,"_listar_",8))>0){
						leer_lista(sockfd1);
					}else{
						perror("falla en enviar _lista_\n");
						exit(1);
					}
					close(sockfd1);
					break;	
			}
			case 2:{
					int sockfd2=socket(AF_INET,SOCK_STREAM,0);
					valida_socket(sockfd2,"falla en sockfd\n");
					int connectfd=connect(sockfd2, (struct sockaddr *)&direccion, sizeof(direccion));
					valida_error(connectfd,sockfd2,"Falla de connect\n");
					nombre_arch(nombre);
					char resp[TAM]={0}; int sol;
					if((sol=write(sockfd2,nombre,strlen(nombre)))>0){
						
						connectfd=read(sockfd2,resp,TAM); //escribir estos bytes en caso que sea correctop el nombre
						valida_socket(connectfd,"falla en leer connectfd");
						//printf("archivo->%s\n",resp);
						if(strcmp(resp,"no_existe")==0){
							printf("nombre ingresado no valido\n");
							exit(1);
						}else{
							umask(0);
							int fdo=open(nombre,O_CREAT | O_TRUNC | O_RDWR, 0666),fdw,conteo=0;
							valida_socket(fdo,"falla fdo");
							printf("Descargando ..\n");
							fdw=write(fdo,resp,connectfd);
							valida_socket(fdw,"falla en fdw inicial");
							conteo+=fdw;
							do{
								connectfd=read(sockfd2,resp,TAM);
								valida_error(connectfd,sockfd2,"falla en connectfd caso2");
								fdw=write(fdo,resp,connectfd);
								valida_socket(fdw,"falla en fdw");
								conteo+=fdw;
								lseek(fdo,conteo,SEEK_SET);
								memset(resp,0,TAM);
							}while(connectfd!=0);
							close(fdo);
							sleep(1);
							printf("Archivo %s descargado (%d bytes)\n",nombre,conteo);
						}
					}else{
						valida_error(sol,sockfd2,"fallo sol en opcion 2, se procedio a cerrar sockfd");
					}
					close(sockfd2);
					break;
			       }
			case 3:
			        return 0;

		}

	}
	

}
void valida_error(int res, int sockfd, char *msg){
	if(res<0){
		perror("err-->");
		printf("%s\n",msg);
		exit(1);
	}
}
void valida_socket(int sockfd, char *msg){
	if(sockfd<0){
		perror("err->");
		printf("%s\n",msg);
	}
}
void leer_lista(int sockfd){
	int fdread;
	char n_archivo[20]={0};
	while((fdread=read(sockfd,n_archivo,20))!=0){
		valida_socket(fdread,"falla en fdread para leer_lista");
		printf("\t%s\n",n_archivo);
		memset(n_archivo,0,20);
	}
	sleep(2);
}
char *nombre_arch(char *name){
	printf("Ingrese nombre de archivo: ");
	scanf("%s",name);
	return name;
}
/*void descargar_archivo(char *nombre,int sockfd, char *buff,int completar){
		umask(0);
		int numbytes_leidos=0,fdopen,fdread,fdwrite,escr=0;
		printf("Descargando archivo ..\n");
		fdopen=open(nombre,O_CREAT | O_TRUNC | O_RDWR, 0666);
		valida_socket(fdopen,"falla en fdopen para descargar_archivo");
		fdwrite=write(fdopen,buff,completar);
		valida_socket(fdwrite,"falla en completar");
		memset(buff,0,TAM);
		while((fdread=read(sockfd,buff,TAM))!=0){
			valida_error(fdread,sockfd,"se cerro el sockfd por falla en read para descargar_archivo ");
			fdwrite=write(fdopen,buff,fdread);
			valida_socket(fdwrite,"fallo en fdwrite para crear archivo");
			numbytes_leidos+=fdread;
			escr+=fdwrite;
			lseek(fdopen,numbytes_leidos,SEEK_SET);
			memset(buff,0,TAM);
		}
		printf("escritos=%d\nrebibidos=%d\n",escr,numbytes_leidos);
		close(fdopen);
}*/
int menu(){
	int opcion;
	printf("\n\t********MENU*******\n");
	printf("\n\t1. Listar archivos a descargar");
	printf("\n\t2. Descargar archivo");
	printf("\n\t3. salir");
	printf("\n\n\topcion:\t");
	scanf("%d",&opcion);
	printf("\n");
	if((opcion != 1) && (opcion != 2 )&& (opcion != 3)){
		printf("opcion no valida\n");
		exit(1);
	}
	return opcion;
}