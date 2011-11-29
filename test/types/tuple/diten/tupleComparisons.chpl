var a = (1, 2.2, 3.14);
var b = (1, 2.2, 3.14);
var c = (1, 2.5);
var d = (1, 2.2, 3.14, 2.718i);
var e = (1, 1.1, 3.14);

writeln("Equal");
writeln(a == b);
writeln(c == b);
writeln(a == d);
writeln(a == e);

writeln();
writeln("Not equal");
writeln(a != b);
writeln(c != b);
writeln(a != d);
writeln(a != e);

writeln();
writeln("Greater than or equal");
writeln(a >= b);
writeln(a >= e);

writeln();
writeln("Greater than");
writeln(a > b);
writeln(a > e);

writeln();
writeln("Less than or equal");
writeln(a <= b);
writeln(a <= e);

writeln();
writeln("Less than");
writeln(a < b);
writeln(a < e);

