CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  --malloc-fill=0x40 --free-fill=0x23 ./$(BIN)
BIN = NewFSP_client NewFSP_server RDP_protocol

all: $(BIN)

NewFSP_client : NewFSP_client.c
	gcc -g $(CFLAGS) -o $@ $^
	
NewFSP_server : NewFSP_server.c
	gcc -g $(CFLAGS) -o $@ $^

RDP_protocol : RDP_protocol.c
	gcc -g $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)

clean:
	rm -f ./$(BIN)


