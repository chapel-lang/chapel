var t : (int, (real, string));

t = (2, (3.14, "hello world"));

writeln(t(0));
writeln(t(1)(0));
writeln(t(1)(1));

var x : int, y : real, z : string;

(x, (y, z)) = t;

writeln(x);
writeln(y);
writeln(z);
