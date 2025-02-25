CC = gcc
CFLAGS = -Wall -pthread
TARGET = multithread

all: $(TARGET)

$(TARGET): multithread.c
	$(CC) $(CFLAGS) multithread.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
