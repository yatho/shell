#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char * argv[]) {
	struct sockaddr_in SockAdr;
	int idSocket;
	int fdSocket;
	int taille;

	idSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (idSocket == -1) {
		perror("Erreur dans socket()");
		exit(-1);
	}

	SockAdr.sin_family = AF_UNIX;
	SockAdr.sin_port = htons(1234);
	SockAdr.sin_add_.s_addr = htonl(INADDR_ANY);

	if (bind(idSocket, &SockAdr, sizeof(SockAdr)) == -1) {
		perror("Erreur dans bind()");
		exit(-1);
	}

	if (listen(idSocket, 10) == -1) {
		perror("Erreur dans listen()");
		exit(-1);
	}

	while (1) {
		taille = sizeof(struct sockaddr_in);
		fdSocket = accept(idSocket, &SockAdr, &taille);
	}

	close(idSocket);
	return 0;
}
