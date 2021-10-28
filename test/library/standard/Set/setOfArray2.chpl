use Set;

var s = new set([0..1] int);

s.add([1, 2]);

writeln(s);
writeln(s.contains([1, 2]));
writeln(s.contains([2, 1]));
writeln(s.contains([1, 2, 3]));
s.remove([1, 2]);
writeln(s);
