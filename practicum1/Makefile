# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Target executable
TARGET = app

# Source files and corresponding objects
SRCS = main.c message.c cache.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c message.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)