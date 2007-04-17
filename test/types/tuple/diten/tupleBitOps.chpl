var a = (1, 2, 3, 4);
var b = (-1, 3, 1, 7);
var biga = (1200, 344, 55, -2103);
var c = 5;
var bigc = 3104;

writeln("Not");
writeln(~a);

writeln();
writeln("And");
writeln(a & b);
writeln(a & c);
writeln(c & b);

writeln();
writeln("Or");
writeln(a | b);
writeln(a | c);
writeln(c | b);

writeln();
writeln("XOr");
writeln(a ^ b);
writeln(a ^ c);
writeln(c ^ b);

writeln();
writeln("Left Shift");
writeln(b << a);
writeln(a << c);
writeln(c << b);

writeln();
writeln("Right Shift");
writeln(biga >> a);
writeln(biga >> c);
writeln(bigc >> a);


