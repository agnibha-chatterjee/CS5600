# Makefile for CS5600 Week 2 Assignment
CC = gcc
CFLAGS = -Wall -Wextra -O2 # -Wall: show all warnings, -Wextra: show extra warnings, -O2: optimize code

# Targets
TARGETS = learnc arger

# Object files
LEARN_OBJS = learnc.o mpg2km.o
ARGER_OBJS = arger.o

all: $(TARGETS)

# Build learnc
learnc: $(LEARN_OBJS)
	$(CC) $(CFLAGS) -o learnc $(LEARN_OBJS)

# Build arger
arger: $(ARGER_OBJS)
	$(CC) $(CFLAGS) -o arger $(ARGER_OBJS)

# Compile individual source files
learnc.o: learnc.c mpg2km.h
	$(CC) $(CFLAGS) -c learnc.c

mpg2km.o: mpg2km.c mpg2km.h
	$(CC) $(CFLAGS) -c mpg2km.c

arger.o: arger.c
	$(CC) $(CFLAGS) -c arger.c

clean:
	rm -f $(LEARN_OBJS) $(ARGER_OBJS) $(TARGETS)

.PHONY: all clean 