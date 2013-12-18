#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include "shell_fct.h"
#include <readline/readline.h> 
#include <readline/history.h>
 

int main(int argc, char** argv)
{
	//..........
	int ret = MYSHELL_CMD_OK;
	cmd mycmd;
	char* readlineptr, * tampon;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];

	using_history();

	//..........
	while(ret != MYSHELL_FCT_EXIT)
	{
		infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);

		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);

		if (strstr(readlineptr, "exit") != NULL) {
			ret = MYSHELL_FCT_EXIT;
		}
		else {

			tampon = (char *)malloc((strlen(readlineptr) + 1) * sizeof(char));
			if (tampon == NULL) {
				exit(-1);
			}

			strcpy(tampon, readlineptr);
			add_history(tampon);

			//..........
			parse_membres(readlineptr, &mycmd); 
			parse_args(&mycmd);
			parse_redirection(&mycmd);
			exec_commande(&mycmd);
			//.......... 

			//aff_membres(&mycmd);
			//aff_args(&mycmd);
			//aff_redirection(&mycmd);

		
			free(readlineptr);
			free_membres(&mycmd);
			free_args(&mycmd);
			free_redirection(&mycmd);
		
			//..........
		}
		
	}
	clear_history();
	//..........
	return 0;
}
