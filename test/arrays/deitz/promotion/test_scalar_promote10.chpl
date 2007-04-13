var s = makeList(1, 2, 3, 4);
var p = makeList(2, 4, 1, 3);
var r = makeList(4, 3, 2, 1);

writeln(s);
writeln(p);
writeln(r);

s(p)(r) = 0;

writeln(s);
writeln(p);
writeln(r);
