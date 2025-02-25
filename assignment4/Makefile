CC = gcc
CFLAGS = -Wall -Wextra -g -c


OBJS = encrypt.o readfile.o queue.o

all: encrypt cipher genrand

encrypt: $(OBJS)
	$(CC) -o $@ $(OBJS)

encrypt.o: encrypt.c readfile.h queue.h
	$(CC) $(CFLAGS) -c encrypt.c -o encrypt.o

readfile.o: readfile.c readfile.h queue.h
	$(CC) $(CFLAGS) -c readfile.c -o readfile.o

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c queue.c -o queue.o

readfile_demo:
	$(CC) $(CFLAGS) -DDEMO_READFILE_MAIN readfile.c queue.o -o readfile_demo

genrand: genrand.o
	$(CC) -o genrand genrand.o

genrand.o: genrand.c
	$(CC) $(CFLAGS) -c genrand.c -o genrand.o

cipher: cipher.o polybius.o
	$(CC) -o cipher cipher.o polybius.o

cipher.o: cipher.c polybius.h
	$(CC) $(CFLAGS) -c cipher.c -o cipher.o

polybius.o: polybius.c polybius.h
	$(CC) $(CFLAGS) -c polybius.c -o polybius.o

clean:
	rm -f *.o *.txt encrypt readfile_demo genrand cipher output/* && rmdir output