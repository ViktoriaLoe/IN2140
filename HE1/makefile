CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  
BIN = rdp_client rdp_server 

all: $(BIN)

rdp_client : rdp_client.c send_packet.c Packet.c
	gcc -g $(CFLAGS) -o $@ $^
	
rdp_server : rdp_server.c send_packet.c Packet.c
	gcc -g $(CFLAGS) -o $@ $^


run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)

clean:
	rm -f ./$(BIN)


