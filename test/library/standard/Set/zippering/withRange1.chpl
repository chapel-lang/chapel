use Set;

var s1: set(int);
var r1 = 1..10;

for i in 1..10 { s1.add(i); }

writeln("s1 = ", s1);
writeln("r1 = ", r1);

forall (i,j) in zip(r1, s1) do
  writeln(i, " ", j);

writeln("all done");
