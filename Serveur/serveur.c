#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include "shell_fct_serv.h"

void free_args(char ** args, int nombre)
{
	int i;
	for(i=0; i< nombre;i++)
		free(args[i]);
	free(args);
}


//Fonction thread
static void * thread_start(void * socketClient) {
	//allocation du nombre d'arguments
	char ** args;
	int tailleArgs, i, nombre;
	int * fdSocket = (int *)socketClient;
	int tube[2];
	char bufferPipe[PIPE_BUF];

	if(recv(*fdSocket, &nombre, sizeof(int),0) == -1) {
		perror("Erreur dans recv()");
		exit(-1);
	}

	args = (char**) malloc(sizeof(char*)*(nombre+1));
	if (args == NULL) {
		perror("Erreur d'allocation");
		exit(-1);
	}		

	for(i=0;i<nombre;i++) {
		if (recv(*fdSocket, &tailleArgs, sizeof(int),0) == -1) {
			perror("Erreur dans recv()");
			exit(-1);
		}

		args[i] = (char*)malloc(sizeof(char)*(tailleArgs+1));
		if(args[i] == NULL)
		{
			perror("Erreur d'allocation");
			exit(-1);
		}
		if (recv(*fdSocket, (char*)args[i], tailleArgs,0) == -1) {
			perror("Erreur dans recv()");
			exit(-1);
		}
		args[i][tailleArgs] = '\0';
	}
	args[nombre] = NULL;
	exec_commande(args, tube);

//Renvoyer le contenu du tube - faire un renvoi de flux pour toutes tailles de "buffer"
	memset(bufferPipe, '\0', PIPE_BUF);
	i = 0;
	while (read(tube[0], &(bufferPipe[i]), 1)) {
		i++;
	}
	close(tube[0]);

	if (send(*fdSocket, (char *)bufferPipe, sizeof(char) * PIPE_BUF, 0) == -1) {
		perror("Erreur dans send()");
		exit(-1);
	}
	free_args(args, nombre);

	return NULL;
}



int main(int argc, char * argv[]) {
	struct sockaddr_in SockAdr;
	int idSocket;
	int fdSocket;
	int taille;
	int status;
	pthread_t thread_id;
	pthread_attr_t attr;
	void * res;

	if (argc < 2) {
		perror("Erreur dans le nombre d'arguments.");
		exit(-1);
	}

	idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (idSocket == -1) {
		perror("Erreur dans socket()");
		exit(-1);
	}

	SockAdr.sin_family = AF_INET;
	SockAdr.sin_port = htons(atoi(argv[1]));// numéro de port d'accès du serveur
	SockAdr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(idSocket, (struct sockaddr *)&SockAdr, sizeof(SockAdr)) == -1) {
		perror("Erreur dans bind()");
		exit(-1);
	}

	if (listen(idSocket, 10) == -1) {
		perror("Erreur dans listen()");
		exit(-1);
	}


	while (1) {
		taille = sizeof(struct sockaddr_in);
		fdSocket = accept(idSocket, (struct sockaddr *)&SockAdr, (socklen_t *)&taille);
		
		status = pthread_attr_init(&attr);
		status = pthread_create(&thread_id, &attr, &thread_start, &fdSocket);
		status = pthread_detach(thread_id);
	}

	close(idSocket);
	return 0;
}
