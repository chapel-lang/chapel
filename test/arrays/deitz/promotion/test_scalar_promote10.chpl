var s = (/1, 2, 3, 4/);
var p = (/2, 4, 1, 3/);
var r = (/4, 3, 2, 1/);

writeln(s);
writeln(p);
writeln(r);

s(p)(r) = 0;

writeln(s);
writeln(p);
writeln(r);
