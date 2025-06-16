CC = gcc
OUTPUT = -o passwordManager
SRC = passwordManager.c gui.c

GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

FLAGS = -lcrypto $(GTK_CFLAGS) $(GTK_LIBS)

all:
	$(CC) $(SRC) $(OUTPUT) $(FLAGS)

clean:
	rm -f passwordManager

.PHONY: all clean

