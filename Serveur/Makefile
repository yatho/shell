CC=gcc
LIBS=-lreadline -lpthread
EXEC=serveur
all:$(EXEC)
CCFLAGS=-g -Wall

$(EXEC): serveur.o
	gcc $(CCFLAGS) -o  $(EXEC) serveur.o $(LIBS)

serveur.o: serveur.c
	$(CC)  $(CCFLAGS) -o serveur.o -c serveur.c
 
clean:
	rm -vf *.o
