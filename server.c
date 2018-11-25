#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdint.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

#define PORT 8080 

struct accban{
	int id_cliente;
	int nip;
	int saldo;
}account;

FILE *cuenta;

void iniciarcuenta(){
	account.id_cliente = 201629946;
	account.nip = 29946;
	account.saldo = 0;
}

void createfile(){
	cuenta=fopen("cuentabancaria.txt","a");
	if(cuenta == NULL){
		perror("error al crear el archivo\n");
	}
	fwrite(&account,sizeof(account), 1, cuenta);
	puts("cuenta creada\n");
	fclose(cuenta);
}

void writefile(int abono){
	cuenta=fopen("cuentabancaria.txt","w");
	if(cuenta == NULL){
		perror("error al escribir el archivo\n");
	}

	account.saldo = account.saldo + abono; //SUMA DINERO AL SALDO

	fwrite(&account,sizeof(account), 1, cuenta);
	puts("datos sobreescritos\n");
	fclose(cuenta);
}

void readfile(){
	cuenta=fopen("cuentabancaria.txt","r");
	if(cuenta == NULL){
		perror("error al leer el archivo\n");
	}
	fread(&account,sizeof(account), 1, cuenta);
	puts("Datos leidos\n");
	fclose(cuenta);
}

void imprimircuenta(){
	printf("id cliente: %d\n",account.id_cliente);
	printf("NIP cliente: %d\n",account.nip);
	printf("saldo: %d\n",account.saldo);
}

int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0};
	char bufop[5] = {0};
	char salds[10] = {0};
	char rps[2];
	char montost[10];
	int cliente, num, monto, rp, aux;

	//***IMPORTATE***  en caso de primera ejecucion, habilitar las funciones comentadas

/*	iniciarcuenta();
	createfile();
	writefile(0);
	imprimircuenta();*/
	readfile();
	

	//mensaje de numeros
	int32_t vs = htonl(228);
	char *vals = (char*)&vs;

	int32_t vr;
	char *valr = (char*)&vr;
	
	// Creamos el socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//adjuntamos el socket al puerto 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 

	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}

	puts("Esperando solicitud del cliente\n");
	
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	}
	do{
		valread = read( new_socket , buffer, 1024); 
	    cliente=atoi(buffer); //CONVIERTE EL STRING RECIBIDO EN BUFFER A INT
		if(cliente != account.id_cliente){
			printf("Cliente desconocido\n");
			rp = 0;
			sprintf(rps, "%d", rp);
			send(new_socket, rps, strlen(rps),0);
		}else{
			rp = 1;
		    sprintf(rps, "%d", rp);
		    send(new_socket, rps, strlen(rps),0);
		}
	}while(cliente != account.id_cliente);

	printf("Cliente: %d\n", account.id_cliente);

	do{
	valread = read( new_socket , bufop, 5); 
	num=atoi(bufop);
	switch(num)
	{
		case 1: 
		    printf("CONSULTA DE SALDO\n");
			readfile();
			sprintf(salds, "%d", account.saldo);
		    send(new_socket, salds, strlen(salds),0);
			break;

		case 2: 
		    printf("RETIRO DE SALDO\n");
			break;

		case 3: 
		    printf("ABONO DE DINERO\n");
			valread = read( new_socket , montost, 10); 
	        monto = atoi(montost);
			writefile(monto);
			puts("Deposito exitoso");
			break;
	}

	}while(num != 0);
	printf("Salio\n");

	return 0; 
} 
