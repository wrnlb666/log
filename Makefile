CC = gcc
CFLAG = -Wall -Wextra -Os -std=gnu17 -pedantic# -fsanitize=leak,bounds,address
OBJ = log.o
LIB = 

.PHONY: all

all: log

log: log.c
	$(CC) $(CFLAG) $< -c $(LIB)
