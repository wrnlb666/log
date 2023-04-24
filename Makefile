CC = gcc
CFLAG = -Wall -Wextra -g -std=gnu17 -pedantic# -fsanitize=leak,bounds,address
OBJ = log.o
LIB = 

.PHONY: all

all: log main

log: log.c
	$(CC) $(CFLAG) $< -c $(LIB)

main: main.c log.o
	$(CC) $(CFLAG) $(OBJ) $< -o $@