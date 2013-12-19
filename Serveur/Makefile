CC=gcc
LIBS=-lreadline -lpthread
EXEC=serveur
all:$(EXEC)
CCFLAGS=-g -Wall

$(EXEC): serveur.o shell_fct_serv.o
	gcc $(CCFLAGS) -o  $(EXEC) serveur.o shell_fct_serv.o $(LIBS)

shell_fct_serv.o: shell_fct_serv.c
	$(CC)  $(CCFLAGS) -o shell_fct_serv.o -c shell_fct_serv.c

serveur.o: serveur.c
	$(CC)  $(CCFLAGS) -o serveur.o -c serveur.c
 
clean:
	rm -vf *.o
