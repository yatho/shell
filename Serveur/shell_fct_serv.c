#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell_fct_serv.h"

pid_t numProcessCmd;

void timeOut(int sigNum) {
	kill(numProcessCmd, SIGKILL);
}
/*A modifier*/
int exec_commande(char ** args) {
	int status;

	numProcessCmd = fork();
	if (numProcessCmd == 0) {
		execvp(args[0], args);
		exit(0);
	}
	else {
		signal(SIGALRM, timeOut);
		alarm(10);
		waitpid(numProcessCmd, &status, 0);
	}
	return 0;
}
