#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell_fct.h"

pid_t numProcessCmd;

void timeOut(int sigNum) {
	kill(numProcessCmd, SIGKILL);
}


static void exec_commande_serveur(cmd * c, unsigned int position, int * tube) {
	int sockFd, taille, i, nbOctets, long_Chaine;
	char buffer[1024];
	struct sockaddr_in adresse;

	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockFd == -1) {
		perror("Erreur dans socket()");
		exit(-1);
	}

	adresse.sin_family = AF_INET;
	inet_aton((c->liste_serveurs[position]).adresseIP, (struct in_addr *)&(adresse.sin_addr.s_addr));
	adresse.sin_port = htons((c->liste_serveurs[position]).numPort);

	taille = sizeof(adresse);
	
	if (connect(sockFd, (struct sockaddr *)&adresse, taille) == -1) {
		perror("Erreur dans connect()");
		exit(-1);
	}

	//Faire travail de la socket
	//envoie du nombre d'arguments de la commande a effectuer sur le serveur.
	if (send(sockFd, (int *)(&(c->nb_args[position])), sizeof(int), 0) == -1) {
		perror("Erreur dans send()");
		exit(-1);
	}

	for (i = 0; c->cmd_args[position][i] != NULL; i++) {
		memset(buffer, '\0', 1024);
		strcpy(buffer, c->cmd_args[position][i]);
		printf("%s\n", buffer);

		long_Chaine = strlen(c->cmd_args[position][i]);
		if (send(sockFd, (int *)&long_Chaine, sizeof(int), 0) == -1) {
			perror("Erreur dans send()");
			exit(-1);
		}

		if ((nbOctets = send(sockFd, (char *)buffer, strlen(c->cmd_args[position][i]), 0)) == -1) {
			perror("Erreur dans send()");
			exit(-1);
		}
		
		printf("%d\n", nbOctets);
	}

	//Récupérer les données finales
//	if (recv(sockFd, (char*)buf, , 0)) 


	close(sockFd);
}


int exec_commande(cmd* ma_cmd) {
	int status, i;
	int fdIn, fdOut, fdErr;
	unsigned int position = ma_cmd->nb_cmd_membres - 1;
	pid_t numPidCmd;
	int tabTube[2][2];

	numProcessCmd = fork();
	if (numProcessCmd == 0) {

		for (i = 0; i < ma_cmd->nb_cmd_membres; i++) {
			switch (position) {
				case 0 :
					break;


				default :
					if (i % 2 == 0) {
						pipe(tabTube[0]);
					}

					if (i % 2 == 1) {
						pipe(tabTube[1]);
					}
					break;
			}


			numPidCmd = fork();

			if (numPidCmd == 0) {
				if (ma_cmd->redirection[i][STDERR] != NULL) {
					if (ma_cmd->type_redirection[i][STDERR] == APPEND) {
						fdErr = open(ma_cmd->redirection[i][STDERR], O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
					}
					else if (ma_cmd->type_redirection[i][STDERR] == OVERWRITE) {
						fdErr = open(ma_cmd->redirection[i][STDERR], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
					}
					close(2);
					dup(fdErr);
				}

				if (i > 0) {
					close(0);

					if (i % 2 == 1) {
						dup(tabTube[0][0]);
						close(tabTube[1][0]);
					}
					else {
						dup(tabTube[1][0]);
						close(tabTube[0][0]);
					}

				}
				else if (ma_cmd->redirection[i][STDIN] != NULL) {
					fdIn = open(ma_cmd->redirection[i][STDIN], O_RDONLY);
					close(0);
					dup(fdIn);
				}

				if (i < position) {
					close(1);

					if (i % 2 == 0) {
						dup(tabTube[0][1]);
						close(tabTube[1][1]);
					}
					else {
						dup(tabTube[1][1]);
						close(tabTube[0][1]);
					}

				}
				else if (ma_cmd->redirection[i][STDOUT] != NULL) {
					if (ma_cmd->type_redirection[i][STDOUT] == APPEND) {
						fdOut = open(ma_cmd->redirection[i][STDOUT], O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
					}
					else if (ma_cmd->type_redirection[i][STDOUT] == OVERWRITE) {
						fdOut = open(ma_cmd->redirection[i][STDOUT], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
					}

					close(1);
					dup(fdOut);
				}

				if ((ma_cmd->liste_serveurs[i]).adresseIP != NULL) {
					exec_commande_serveur(ma_cmd, i, tabTube[0]);
				}
				else {
					execvp(ma_cmd->cmd_args[i][0], ma_cmd->cmd_args[i]);
				}

				if (i > 0) {
					if (i % 2 == 1) {
						close(tabTube[0][0]);
					}
					else {
						close(tabTube[1][0]);
					}
				}
				else if (ma_cmd->redirection[i][STDIN] != NULL) {
					close(fdIn);
				}

				if (i < position) {
					if (i % 2 == 0) {
						close(tabTube[0][1]);
					}
					else {
						close(tabTube[1][1]);
					}

				}
				else if (ma_cmd->redirection[i][STDOUT] != NULL) {
					close(fdOut);
				}

				if (ma_cmd->redirection[i][STDERR] != NULL) {
					close(fdErr);
				}

				exit(0);
			}

			waitpid(numPidCmd, &status, 0);

			if (i > 0) {
				if (i % 2 == 1) {
					close(tabTube[0][0]);
				}
				else {
					close(tabTube[1][0]);
				}
			}

			if (i < position) {
				if (i % 2 == 0) {
					close(tabTube[0][1]);
				}
				else {
					close(tabTube[1][1]);
				}
			}
		}
		exit(0);
	}
	else {
		signal(SIGALRM, timeOut);
		alarm(10);
		waitpid(numProcessCmd, &status, 0);
	}
	
	return 0;
}





