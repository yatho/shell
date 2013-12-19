#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char * argv[]) {
	struct sockaddr_in SockAdr;
	int idSocket;
	int fdSocket;
	int taille;
	int nombre;//nombre d'arguments à envoyer
	int i;
	int tailleArgs;
	char ** args;

//Partie correct
	idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (idSocket == -1) {
		perror("Erreur dans socket()");
		exit(-1);
	}
//Partie correct
	SockAdr.sin_family = AF_INET;
	SockAdr.sin_port = htons(8080);// numéro de port d'accès du serveur
	SockAdr.sin_addr.s_addr = htonl(INADDR_ANY);
//Partie correct
	if (bind(idSocket, (struct sockaddr *)&SockAdr, sizeof(SockAdr)) == -1) {
		perror("Erreur dans bind()");
		exit(-1);
	}
//Partie correct
	if (listen(idSocket, 10) == -1) {
		perror("Erreur dans listen()");
		exit(-1);
	}
//Partie correct
	while (1) {
		taille = sizeof(struct sockaddr_in);
		fdSocket = accept(idSocket, (struct sockaddr *)&SockAdr, (socklen_t *)&taille);//Connexion fonctionne
		if(recv(fdSocket, &nombre, sizeof(int),0) == -1) {
			perror("Erreur dans send()");
			exit(-1);
		}
		printf("nombre == %d \n",nombre);

		//allocation du nombre d'arguments
		args = (char**) malloc(sizeof(char*)*(nombre+1));
//récupération des arguments a effectuer
		for(i=0;i<nombre;i++) {
			printf("i = %d\n",i);
			if (recv(fdSocket, &tailleArgs, sizeof(int),0) == -1) {
				perror("Erreur dans send()");
				exit(-1);
			}
			printf("Taille = %d\t",tailleArgs);
			args[i] = (char*)malloc(sizeof(char)*(tailleArgs+1));
			if(args[i] == NULL)
			{
				perror("Erreur d'allocation");
				exit(-1);
			}
			if (recv(fdSocket, (char*)args[i], tailleArgs,0) == -1) {
				perror("Erreur dans send()");
				exit(-1);
			}
			args[i][tailleArgs] = '\0';
			printf("args[%d] = %s\n", i, args[i]);
		}
		args[nombre] = NULL;
	}

	close(idSocket);
	return 0;
}
