F77 = pgf95
CC = pgcc
LFLAGS = -mcmodel=medium -tp amd64 

stream.out: stream.o timers.o
	$(F77) -o $@ $+

stream.o: stream.f
	$(F77) -c $(LFLAGS) $+

timers.o: timers.c
	$(CC) -c $+
