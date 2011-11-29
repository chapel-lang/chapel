var s: [1..4] int = (1, 2, 3, 4);
var p: [1..4] int = (2, 4, 1, 3);
var r: [1..4] int = (4, 3, 2, 1);

writeln(s);
writeln(p);
writeln(r);

s(p)(r) = 0;

writeln(s);
writeln(p);
writeln(r);
