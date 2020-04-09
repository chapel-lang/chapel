config var n : int = 12;

var d : domain(1) = {1..n};
var a : [d] int;

writeln(d);
writeln(a);

for i in d do
  writeln(a(i));
