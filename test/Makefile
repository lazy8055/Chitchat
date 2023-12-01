
CC = gcc
CFLAGS = -I utility

all: client server

helper.o: utility/helper.c utility/helper.h
	$(CC) -c utility/helper.c $(CFLAGS) -o helper.o

client.o: client.c 
	$(CC) -c client.c -o client.o

client: client.o helper.o
	$(CC) client.o helper.o -o client

server.o: server.c 
	$(CC) -c server.c -o server.o

server: server.o helper.o
	$(CC) server.o helper.o -o server

clean:
	rm -f *.o client server
