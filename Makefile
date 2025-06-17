CC = gcc
OUTPUT = -o passwordManager
SRC = passwordManager.c gui.c cliFunctions.c

GTK_CFLAGS = $(shell pkg-config --cflags gtk4)
GTK_LIBS = $(shell pkg-config --libs gtk4)

FLAGS = -lcrypto $(GTK_CFLAGS) $(GTK_LIBS)

all:
	$(CC) $(SRC) $(OUTPUT) $(FLAGS)

clean:
	rm -f passwordManager

.PHONY: all clean

