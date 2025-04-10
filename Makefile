CFLAGS = -Wall -Wextra
CC = clang
SRC = main

build:$(SRC).c
	$(CC) $(CFLAGS) $(SRC).c -o $(SRC)
run:build
	./$(SRC)

