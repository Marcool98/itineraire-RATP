CC = gcc
CFLAGS = -Wall

all : projet.exe 
projet.exe : projet.o 
	$(CC) -o projet projet.o $(CFLAGS)

projet.o : projet.c projet.h 
	$(CC) -o projet.o -c projet.c $(CFLAGS)

clean : 
	rm -rf *.o