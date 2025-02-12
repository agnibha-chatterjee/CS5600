CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = queuetest
OBJ = queue.o queuetest.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c

queuetest.o: queuetest.c queue.h
	$(CC) $(CFLAGS) -c queuetest.c

clean:
	rm -f $(OBJ) $(TARGET) 