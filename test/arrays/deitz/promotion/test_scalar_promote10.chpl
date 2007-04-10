var s = _seq(1, 2, 3, 4);
var p = _seq(2, 4, 1, 3);
var r = _seq(4, 3, 2, 1);

writeln(s);
writeln(p);
writeln(r);

s(p)(r) = 0;

writeln(s);
writeln(p);
writeln(r);
