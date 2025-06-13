CC=gcc
OUTPUT=-o passwordManager
FLAGS=-lcrypto
SRC=passwordManager.c terminalUI.c

all:
	$(CC) $(SRC) $(OUTPUT) $(FLAGS)

clean:
	rm -f passwordManager

.PHONY: all clean