enum color {red=42, orange=56};
var r1 = color.red..color.orange;
var r2 = r1: range(int);
writeln(r2);
var r3 = r2: range(color);
writeln(r3);
var r3a = (0..1): range(color);
writeln(r3a);
