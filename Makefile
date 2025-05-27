CC = gcc -g
CFLAGS = -Wall -Iinclude
TARGET = my_program
SRC = src/main.c src/network.c src/parser.c src/exec_cmd.c src/slice_lib.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)