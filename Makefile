BIN = test
CC = gcc -std=c99
SRC = main.c
CFLAGS = -O3 -Wall -Wextra -Wpedantic -fsanitize=address

all:
	@ clear
	@ $(CC) $(SRC) $(CFLAGS) -o $(BIN)
	@ ./$(BIN) 10000000
	@ rm -f ./$(BIN)
