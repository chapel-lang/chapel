use Set;

var s1, s2: set(int);

for i in 1..10 { s1.add(i); s2.add(i+10); }

writeln("s1 = ", s1);
writeln("s2 = ", s2);

forall (i,j) in zip(s1, s2) do writeln(i, " ", j);

writeln("all done");
