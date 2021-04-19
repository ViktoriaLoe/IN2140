CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  
BIN = NewFSP_client NewFSP_server 

all: $(BIN)

NewFSP_client : NewFSP_client.c send_packet.c
	gcc -g $(CFLAGS) -o $@ $^
	
NewFSP_server : NewFSP_server.c send_packet.c
	gcc -g $(CFLAGS) -o $@ $^


run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)

clean:
	rm -f ./$(BIN)


