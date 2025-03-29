# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Target executables
TARGET = app
EVAL_TARGET = cache_eval

# Source files and corresponding objects
SRCS = main.c message.c cache.c
OBJS = $(SRCS:.c=.o)

# Evaluation source and object files
EVAL_SRCS = cache_evaluation.c message.c cache.c
EVAL_OBJS = $(EVAL_SRCS:.c=.o)

.PHONY: all clean evaluation

all: $(TARGET)

evaluation: $(EVAL_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(EVAL_TARGET): $(EVAL_OBJS)
	$(CC) $(CFLAGS) -o $(EVAL_TARGET) $(EVAL_OBJS)

main.o: main.c message.h cache.h
	$(CC) $(CFLAGS) -c $< -o $@

message.o: message.c message.h
	$(CC) $(CFLAGS) -c $< -o $@

cache.o: cache.c cache.h message.h
	$(CC) $(CFLAGS) -c $< -o $@

cache_evaluation.o: cache_evaluation.c cache.h message.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EVAL_OBJS) $(TARGET) $(EVAL_TARGET) *.o