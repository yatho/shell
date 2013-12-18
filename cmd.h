#ifndef CMD_H
#define CMD_H


#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define APPEND 3
#define OVERWRITE 4

typedef struct commande {
	char * cmd_initiale;				/* La chaine initiale tapee par l'utilisateur */
	char ** cmd_membres;				/* a chaque case i chaine du membre i */
	unsigned int nb_cmd_membres;	/* Nombre de membres */
	char *** cmd_args;				/* cmd_args[i][j] contient le jieme mot du ieme membre */
	unsigned int * nb_args;			/* Nombre d'arguments par membres */
	char *** redirection;			/* Pour stocker le chemin vers le fichier de redirection */
	int ** type_redirection;			/* Pour stocker le type de redirection */
} cmd;

void aff_args(cmd *c);  //Affichage des arguments
void free_args(cmd *c);  //Libération des arguments
void parse_args(cmd *c);  //Remplit les champs cmd_args et nb_args_membres 
void parse_membres(char *chaine,cmd *ma_cmd);  //Remplit les champs initial_cmd, membres_cmd et nb_membres
void aff_membres(cmd *ma_cmd);  //Affichage des membres 
void free_membres(cmd *ma_cmd); 
void parse_redirection(cmd *c);  //Remplit les champs redirection et type_redirection du membre i
void free_redirection(cmd *c);
void aff_redirection(cmd * c);

#endif
