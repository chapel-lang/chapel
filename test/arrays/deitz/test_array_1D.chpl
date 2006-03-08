config var n : integer = 12;

var d : domain(1) = [1..n];
var a : [d] integer;

writeln(d);
writeln(a);

for i in d do
  writeln(a(i));
