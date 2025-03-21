use Time;

var a = new timeDelta(microseconds=10);
writeln(a.totalMicroseconds());

var b = new timeDelta(minutes=2, seconds=3);
writeln(b.totalMicroseconds());
