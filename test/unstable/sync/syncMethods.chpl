var s: sync int;

s.writeEF(1);
writeln(s.readFF());

s.writeFF(2);
writeln(s.readXX());

s.writeXF(3);
writeln(s.readFE());

writeln(s.isFull);
s.reset();
