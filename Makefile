all: client/client server/server

client/client: 
	gcc -Wall -o client.o client/client.c client/ui.c -lncurses

server/server:
	gcc -Wall -o server.o server/server.c

