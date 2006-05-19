var t : (int, (float, string));

t = (2, (3.14, "hello world"));

writeln(t(1));
writeln(t(2)(1));
writeln(t(2)(2));

var x : int, y : float, z : string;

(x, (y, z)) = t;

writeln(x);
writeln(y);
writeln(z);
