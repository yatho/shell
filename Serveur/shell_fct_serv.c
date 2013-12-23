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

int exec_commande(char ** args, int * tube) {
	int status;

	pipe(tube);
	numProcessCmd = fork();
	if (numProcessCmd == 0) {
		close(1);
		dup(tube[1]);
		execvp(args[0], args);
		close(tube[1]);
		exit(0);
	}
	else {
		signal(SIGALRM, timeOut);
		alarm(10);
		waitpid(numProcessCmd, &status, 0);
		close(tube[1]);
	}
	return 0;
}
