CC = gcc
CFLAGS = -Wall -pthread
TARGET = multithread
PQUEUE_TARGET = pqueuepmain

all: $(TARGET) $(PQUEUE_TARGET)

$(TARGET): multithread.c
	$(CC) $(CFLAGS) multithread.c -o $(TARGET)

$(PQUEUE_TARGET): pqueuepmain.c
	$(CC) $(CFLAGS) pqueuepmain.c -o $(PQUEUE_TARGET)

clean:
	rm -f $(TARGET) $(PQUEUE_TARGET)