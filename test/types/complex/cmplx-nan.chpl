use IO;

var z = 0.0/0.0;
writeln(z);

var c = z + z*1.0i;
writeln(c);

var s = c: string;
writeln(s);

var b = c: bytes;
writeln(b);

var c2 = s: complex;
writeln(c2);

var c3 = b: complex;
writeln(c3);

var c4 = stdin.read(complex);
writeln(c4);
