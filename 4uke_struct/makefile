CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  --malloc-fill=0x40 --free-fill=0x23 ./$(BIN)
BIN = cbra oppgave1 input struct

all: $(BIN)

cbra: cbra.c
	gcc -g $@ -o $^

oppgave1: oppgave1.c
	gcc -g -o $@ $^
	
input: input.c
	gcc -g $(CFLAGS) -o $@ $^

struct: struct.c
	gcc -g $(CFLAGS) -o $@ $^

run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)


clean:
	rm -f ./$(BIN)


