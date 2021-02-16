CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  --malloc-fill=0x40 --free-fill=0x23 ./$(BIN)
BIN = cbra

all: $(BIN)

cbra: cbra.c
	gcc -g cbra.c -o cbra

test: test.c
	gcc -g test.c -o test

run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)


clean:
	rm -f ./$(BIN)


