CC = gcc
CFLAGS = -Wall -Iinclude
TARGET = my_program
SRC = src/main.c src/network.c src/parser.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)