F77 = pgf77
CC = pgcc

stream.out: stream.o timers.o
	$(F77) -o $@ $+

stream.o: stream.f
	$(F77) -c $+

timers.o: timers.c
	$(CC) -c $+
