CC=gcc
CFLAGS=-O3

.PHONY: debug clean

all: fib fib2

fib: fib.c
	$(CC) -o $@ $< $(CFLAGS) -L../../.libs -lmyth-compat

fib2: fib2.c
	$(CC) -o $@ $< $(CFLAGS) -L../../.libs -lmyth-compat

debug:
	$(CC) -o fib fib.c -pthread -ggdb $(CFLAGS)
	$(CC) -o fib2 fib2.c -pthread -ggdb $(CFLAGS)

clean: 
	rm -f fib fib2
