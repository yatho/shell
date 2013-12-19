#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"

/*Fonction pour les membres*/
void parse_membres(char *chaine,cmd *ma_cmd) {
	char * decoupChaine, ** tampon;

	/*Initialisation*/
	ma_cmd->cmd_initiale = (char *)malloc((strlen(chaine) + 1) * sizeof(char));
	if (ma_cmd->cmd_initiale == NULL) {
		exit(-1);
	}

	strcpy(ma_cmd->cmd_initiale, chaine);
	ma_cmd->cmd_membres = NULL;
	ma_cmd->nb_cmd_membres = 0;
	/*Fin de l'initialisation*/

	decoupChaine = strtok(chaine, "|");
	while (decoupChaine != NULL) {
		ma_cmd->nb_cmd_membres++;
		tampon = (char **)realloc(ma_cmd->cmd_membres, ma_cmd->nb_cmd_membres * sizeof(char *));
		if (tampon == NULL) {
			exit(-1);
		}

		ma_cmd->cmd_membres = tampon;
		ma_cmd->cmd_membres[ma_cmd->nb_cmd_membres - 1] = (char *)malloc((strlen(decoupChaine) + 1) * sizeof(char));
		if (ma_cmd->cmd_membres[ma_cmd->nb_cmd_membres - 1] == NULL) {
			exit(-1);
		}

		strcpy(ma_cmd->cmd_membres[ma_cmd->nb_cmd_membres - 1],decoupChaine);
		decoupChaine = strtok(NULL, "|");
	}	
}


void aff_membres(cmd *ma_cmd) {
	int i;

	printf("%d membres :\n", ma_cmd->nb_cmd_membres);
	for (i = 0; i < ma_cmd->nb_cmd_membres; i++) {
		printf("%s\n", ma_cmd->cmd_membres[i]);
	}
}


void free_membres(cmd *ma_cmd) {
	int i;

	for (i = 0; i < ma_cmd->nb_cmd_membres; i++) {
		free(ma_cmd->cmd_membres[i]);
	}

	free(ma_cmd->cmd_membres);
	free(ma_cmd->cmd_initiale);
}



/*Fonctions pour les arguments*/
void parse_args(cmd * c) {
	char * decoupChaine, * tamponMembre, *chaineTemp, *finChaine; 
	char ** tampon;
	int i;

	/*Initialisation*/
	init_serveurs(c);

	c->cmd_args = (char ***)malloc(c->nb_cmd_membres * sizeof(char **));
	if (c->cmd_args == NULL) {
		exit(-1);
	}

	c->nb_args = (unsigned int *)malloc(c->nb_cmd_membres * sizeof(unsigned int));
	if (c->nb_args == NULL) {
		exit(-1);
	}

	for (i = 0; i < c->nb_cmd_membres; i++) {
		c->cmd_args[i] = NULL;
		c->nb_args[i] = 0;
	}
	/*Fin de l'initialisation */


	for (i = 0; i < c->nb_cmd_membres; i++) {
		tamponMembre = (char *)malloc((strlen(c->cmd_membres[i]) + 1) * sizeof(char));
		if (tamponMembre == NULL) {
			exit(-1);
		}

		strcpy(tamponMembre, c->cmd_membres[i]);
		decoupChaine = strtok(tamponMembre, " ");

		while ((decoupChaine != NULL) && (strchr(decoupChaine, '<') != decoupChaine) && (strchr(decoupChaine, '>') != decoupChaine) && (strstr(decoupChaine, "2>") != decoupChaine)) {
			
			if (strstr(decoupChaine, "s:") != NULL) {
				parse_serveurs(c, i, decoupChaine);	
			}

			else {
				tampon = (char **)realloc(c->cmd_args[i], (c->nb_args[i] + 1) * sizeof(char *));
				if (tampon == NULL) {
					exit(-1);
				}

				c->cmd_args[i] = tampon;

				if ((finChaine = strchr(decoupChaine, '<')) != NULL) {
					chaineTemp = (char *)malloc((finChaine - decoupChaine + 1) * sizeof(char));
					strncpy(chaineTemp, decoupChaine, (finChaine - decoupChaine));
					chaineTemp[finChaine - decoupChaine] = '\0';

					c->cmd_args[i][c->nb_args[i]] = (char *)malloc((strlen(chaineTemp) + 1) * sizeof(char));
					if (c->cmd_args[i][c->nb_args[i]] == NULL) {
						exit(-1);
					}
			
					strcpy(c->cmd_args[i][c->nb_args[i]], chaineTemp);
					free(chaineTemp);
				}
			
				else if ((finChaine = strchr(decoupChaine, '>')) != NULL) {
					chaineTemp = (char *)malloc((finChaine - decoupChaine + 1) * sizeof(char));
					strncpy(chaineTemp, decoupChaine, (finChaine - decoupChaine));
					chaineTemp[finChaine - decoupChaine] = '\0';

					c->cmd_args[i][c->nb_args[i]] = (char *)malloc((strlen(chaineTemp) + 1) * sizeof(char));
					if (c->cmd_args[i][c->nb_args[i]] == NULL) {
						exit(-1);
					}
			
					strcpy(c->cmd_args[i][c->nb_args[i]], chaineTemp);
					free(chaineTemp);
				}

				else if ((finChaine = strstr(decoupChaine, "2>")) != NULL) {
					chaineTemp = (char *)malloc((finChaine - decoupChaine + 1) * sizeof(char));
					strncpy(chaineTemp, decoupChaine, (finChaine - decoupChaine));
					chaineTemp[finChaine - decoupChaine] = '\0';

					c->cmd_args[i][c->nb_args[i]] = (char *)malloc((strlen(chaineTemp) + 1) * sizeof(char));
					if (c->cmd_args[i][c->nb_args[i]] == NULL) {
						exit(-1);
					}
			
					strcpy(c->cmd_args[i][c->nb_args[i]], chaineTemp);
					free(chaineTemp);
				}

				else {			
					c->cmd_args[i][c->nb_args[i]] = (char *)malloc((strlen(decoupChaine) + 1) * sizeof(char));
					if (c->cmd_args[i][c->nb_args[i]] == NULL) {
						exit(-1);
					}
			
					strcpy(c->cmd_args[i][c->nb_args[i]], decoupChaine);
				}

				c->nb_args[i]++;
			}

			decoupChaine = strtok(NULL, " ");
		}

		tampon = (char **)realloc(c->cmd_args[i], (c->nb_args[i] + 1) * sizeof(char *));
		if (tampon == NULL) {
			exit(-1);
		}

		c->cmd_args[i] = tampon;
		c->cmd_args[i][c->nb_args[i]] = NULL;
	}
}


void aff_args(cmd * c) {
	int i, j;

	printf("Arguments :\n");
	for (i = 0; i < c->nb_cmd_membres; i++) {
		printf("Membre %d : %d arguments\n", i+1, c->nb_args[i]);

		for (j = 0; j <= c->nb_args[i]; j++) {
			printf("\t%s\n", c->cmd_args[i][j]);
		}
	}
}


void free_args(cmd * c) {
	int i, j;
	
	for (i = 0; i < c->nb_cmd_membres; i++) {
		for (j = 0; j < c->nb_args[i]; j++) {
			if (c->cmd_args[i][j] != NULL)
				free(c->cmd_args[i][j]);
		}
		
		free(c->cmd_args[i]);
	}

	free(c->cmd_args);
	free(c->nb_args);
	free_serveurs(c);
}

/*Fonctions pour les redirections*/
void parse_redirection(cmd *c) {
	char * decoupChaine, * tampon;
	int i;

	/*Initialisation des variables redirection et type_redirection*/
	c->redirection = NULL;
	c->type_redirection = NULL;

	c->redirection = (char ***)malloc(c->nb_cmd_membres * sizeof(char **));
	if (c->redirection == NULL) {
		exit(-1);
	}

	c->type_redirection = (int **)malloc(c->nb_cmd_membres * sizeof(int *));
	if (c->type_redirection == NULL) {
		exit(-1);
	}

	for (i = 0; i < c->nb_cmd_membres; i++) {
		c->redirection[i] = (char **)malloc(3 * sizeof(char *));
		c->type_redirection[i] = (int *)malloc(3 * sizeof(int));
		if ((c->redirection[i] == NULL) || (c->type_redirection[i] == NULL)) {
			exit(-1);
		}

		c->redirection[i][STDIN] = NULL;
		c->redirection[i][STDOUT] = NULL;
		c->redirection[i][STDERR] = NULL;

		c->type_redirection[i][STDIN] = 0;
		c->type_redirection[i][STDOUT] = 0;
		c->type_redirection[i][STDERR] = 0;
	}
	/*Fin d'initialisation*/

	for (i = 0; i < c->nb_cmd_membres; i++) {
		if ((tampon = strchr(c->cmd_membres[i], '<')) != NULL) {
			decoupChaine = strtok(tampon, " <");

			c->redirection[i][STDIN] = (char *)malloc((strlen(decoupChaine) + 1) * sizeof(char));
			if (c->redirection[i][STDIN] == NULL) {
				exit(-1);
			}

			strcpy(c->redirection[i][STDIN], decoupChaine);
		}

		else if ((tampon = strstr(c->cmd_membres[i], "2>>")) != NULL) {
			c->type_redirection[i][STDERR] = APPEND;
			decoupChaine = strtok(++tampon, " >");

			c->redirection[i][STDERR] = (char *)malloc((strlen(decoupChaine) + 1) * sizeof(char));
			if (c->redirection[i][STDERR] == NULL) {
				exit(-1);
			}

			strcpy(c->redirection[i][STDERR], decoupChaine);
		}

		else if ((tampon = strstr(c->cmd_membres[i], "2>")) != NULL) {
			c->type_redirection[i][STDERR] = OVERWRITE;
			decoupChaine = strtok(++tampon, " >");

			c->redirection[i][STDERR] = (char *)malloc((strlen(decoupChaine) + 1) * sizeof(char));
			if (c->redirection[i][STDERR] == NULL) {
				exit(-1);
			}

			strcpy(c->redirection[i][STDERR], decoupChaine);
		}

		else if ((tampon = strstr(c->cmd_membres[i], ">>")) != NULL) {
			c->type_redirection[i][STDOUT] = APPEND;
			decoupChaine = strtok(tampon, " >");

			c->redirection[i][STDOUT] = (char *)malloc((strlen(decoupChaine) + 1) * sizeof(char));
			if (c->redirection[i][STDOUT] == NULL) {
				exit(-1);
			}

			strcpy(c->redirection[i][STDOUT], decoupChaine);
		}

		else if ((tampon = strchr(c->cmd_membres[i], '>')) != NULL) {
			c->type_redirection[i][STDOUT] = OVERWRITE;
			decoupChaine = strtok(tampon, " >");

			c->redirection[i][STDOUT] = (char *)malloc((strlen(decoupChaine) + 1) * sizeof(char));
			if (c->redirection[i][STDOUT] == NULL) {
				exit(-1);
			}

			strcpy(c->redirection[i][STDOUT], decoupChaine);
		}
	}
}


void free_redirection(cmd *c) {
	int i, j;
	
	for (i = 0; i < c->nb_cmd_membres; i++) {
		for (j = 0; j < 3; j++) {
			if (c->redirection[i][j] != NULL) {
				free(c->redirection[i][j]);
			}
		}

		free(c->redirection[i]);
		free(c->type_redirection[i]);
	}

	free(c->redirection);
	free(c->type_redirection);
}


void aff_redirection(cmd * c) {
	int i;

	printf("Redirections :\n");
	for (i = 0; i < c->nb_cmd_membres; i++) {
		printf("Commande %d :\n", i+1);
		printf("\t%s\n", c->redirection[i][STDIN]);

		printf("\t%s", c->redirection[i][STDOUT]);
		if (c->type_redirection[i][STDOUT] == APPEND) {
			printf(", type : append");
		}
		else if (c->type_redirection[i][STDOUT] == OVERWRITE) {
			printf(", type : overwrite");
		}
		printf("\n");

		printf("\t%s", c->redirection[i][STDERR]);
		if (c->type_redirection[i][STDERR] == APPEND) {
			printf(", type : append");
		}
		else if (c->type_redirection[i][STDERR] == OVERWRITE) {
			printf(", type : overwrite");
		}
		printf("\n");
	}
}


/*Fonctions pour le serveur*/
void init_serveurs(cmd * c) {
	unsigned int i;

	c->liste_serveurs = (serveur *)malloc(c->nb_cmd_membres * sizeof(serveur));
	if (c->liste_serveurs == NULL) {
		exit(-1);
	}

	for (i = 0; i < c->nb_cmd_membres; i++) {
		(c->liste_serveurs[i]).adresseIP = NULL;
		(c->liste_serveurs[i]).numPort = -1;
	}
}


void parse_serveurs(cmd * c, unsigned int position, char * chaine) {
	char * position1, * position2;

	position1 = strchr(chaine, ':');
	position2 = strrchr(chaine, ':');
	position1++;
	
	(c->liste_serveurs[position]).adresseIP = (char *)malloc((position2 - position1 + 1) * sizeof(char));
	if ((c->liste_serveurs[position]).adresseIP == NULL) {
		exit(-1);
	}

	strncpy((c->liste_serveurs[position]).adresseIP, position1, (position2 - position1));
	(c->liste_serveurs[position]).adresseIP[position2 - position1] = '\0';

	position2++;
	(c->liste_serveurs[position]).numPort = atoi(position2);
}


void free_serveurs(cmd * c) {
	int i;
	
	for (i = 0; i < c->nb_cmd_membres; i++) {
		free((c->liste_serveurs[i]).adresseIP);
	}

	free(c->liste_serveurs);
}
