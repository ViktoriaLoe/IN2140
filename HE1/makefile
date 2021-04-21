CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  
BIN = fsp_client_program fsp_server_program 

all: $(BIN)

fsp_client_program : fsp_client.o Packet.o send_packet.o
	gcc -g $(CFLAGS) $^ -o $@ 
	
fsp_server_program : fsp_server.o Packet.o send_packet.o
	gcc -g $(CFLAGS) $^ -o $@ 


fsp_client.o 	: fsp_client.c 
	gcc -g $(CFLAGS) $^ -c

fsp_server.o 	: fsp_server.c 
	gcc -g $(CFLAGS) $^ -c

Packet.o 		: Packet.c
	gcc -g $(CFLAGS) $^ -c

send_packet.o 	: send_packet.o
	gcc -g $(CFLAGS) $^ -c


run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)

clean:
	rm -f *.o ./$(BIN)


