CC = cc

ra: rand.o ra.o args.o
	$(CC) -lm -o ra rand.o ra.o args.o

testrand: rand.o testrand.o
	$(CC) -o testrand rand.o testrand.o

helloworld: helloworld.o
	$(CC) -o helloworld helloworld.o

rand.o: rand.c
	$(CC) -o rand.o -c rand.c

args.o: args.c args.h ra.h
	$(CC) -o args.o -c args.c

testrand.o: testrand.c
	$(CC) -o testrand.o -c testrand.c

ra.o: ra.c
	$(CC) -o ra.o -c ra.c

helloworld.o: helloworld.c
	$(CC) -o helloworld.o -c helloworld.c

clean:
	rm -f *.o ra testrand helloworld
