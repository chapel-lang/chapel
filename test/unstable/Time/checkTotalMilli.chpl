use Time;

var a = new timeDelta(milliseconds=10);
writeln(isClose(a.totalMilliseconds(), 10.0));

var b = new timeDelta(minutes=2, seconds=3);
writeln(isClose(b.totalMilliseconds(), 123000.0));
