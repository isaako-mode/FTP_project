CC = gcc -g
CFLAGS = -Wall -Iinclude -Wextra -pedantic -std=c11 -g 
TARGET = my_program
SRC = src/main.c src/network.c src/parser.c src/exec_cmd.c src/slice_lib.c src/build_output.c
all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)