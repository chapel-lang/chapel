CC = cc

CFLAGS=-O3
LDFLAGS=-lm

ra: rand.o ra.o args.o timer.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o ra rand.o ra.o args.o timer.o

testrand: rand.o testrand.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o testrand rand.o testrand.o

helloworld: helloworld.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o helloworld helloworld.o

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o ra testrand helloworld
