var a: atomic int = 3;
var b: [1..2] atomic int = 5;
writeln(a.read());
writeln(b[1].read());
