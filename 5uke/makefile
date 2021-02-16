CFLAGS = -std=gnu11 -g -Wall -Wextra
VFLAGS = --leak-check=full --show-leak-kinds=all  --malloc-fill=0x40 --free-fill=0x23 ./$(BIN)
BIN = files

all: $(BIN)

files: files.c
	gcc -g files.c -o files

test: test.c
	gcc -g test.c -o test

run: $(BIN)
	./$(BIN)

check: $(BIN)
	valgrind $(VFLAGS) ./$(BIN)


clean:
	rm -f ./$(BIN)


