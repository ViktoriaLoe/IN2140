CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  --malloc-fill=0x40 --free-fill=0x23 ./$(BIN)
BIN =  bitOp printbits signed

all: $(BIN)

bitOp: bitOp.c
	gcc -g $(CFLAGS) -o $@ $^
	
signed: signed.c
	gcc -g $(CFLAGS) -o $@ $^

printbits: printbits.c
	gcc -g $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)


clean:
	rm -f ./$(BIN)


