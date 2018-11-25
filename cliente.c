#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdint.h>
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char buffer[1024] = {0}; 
	int op, cliente, aux, rp, monto, opc;
	char ops[1], codigo[9], rps[2], montost[10];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convertimos las direcciones IPv4 y IPv6 a forma de texto binaria 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	do{
		printf("\nFavor de ingresar su ID de cliente de 9 dígitos:\n");
	    scanf("%d", &cliente);
	    sprintf(codigo, "%d", cliente);
	    send(sock,codigo,strlen(codigo),0);

		valread = read( sock , rps, 2); 
	    rp=atoi(rps);
	}while(rp == 0);
	
	printf("\n¡Bienvenid@! Por favor seleccione una de las siguientes opciones\n1) Consulta de saldo\n2) Retiro de efectivo\n3) Deposito\n0) Salir\n");
	do{
	fflush(stdin);
	scanf("%d", &op);
	aux = op;
	sprintf(ops,"%d",op); //CONVIERTE EL ENTERO A UN STRING
	send(sock,ops,strlen(ops),0);
	switch(aux){

		case 1: //CONSULTA DE SALDO
		valread = read(sock , montost, 10); 
	    monto=atoi(montost);
		printf("\nSaldo Disponible: $ %d.00\n", monto);
		break;

		case 2: //RETIRO DE SALDO


		break;

		case 3: //DEPOSITO
		printf("Ingrese el monto a depositar(deposito minimo: $100)\n$ ");
		fflush(stdin);
		scanf("%d",&monto);
		sprintf(montost,"%d",monto);
	    send(sock,montost,strlen(montost),0);
		printf("¡¡Monto depositado!!\n\n");
		break;
	}
	if(aux != 0){
		printf("\nQue mas desea hacer?\n1) Consulta de saldo\n2) Retiro de efectivo\n3) Deposito\n0) Salir\n");
	}
	}while(aux != 0);

	printf("usted ah salido del cajero\n");
	puts("\n");
/*	
	FUNCIÓN DE EJEMPLO PARA CONVERTIR DE INT A STRING
		int k=12345;
		char ks[12];
		sprintf(ks,"%d",k); //CONVIERTE EL ENTERO A UN STRING
		printf("%s\n", ks);
		send(sock, ks, strlen(ks),0);
		printf("valor sent\n"); 
*/
	return 0; 
} 
