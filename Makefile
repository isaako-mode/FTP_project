CC = gcc
CFLAGS = -Wall
TARGET = my_program
SRC = src/main.c

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)