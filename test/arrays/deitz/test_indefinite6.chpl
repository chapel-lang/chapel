var d : domain(int);
var a : [d] int;

d += 2;
d += 0;

a(2) = 7;
a(0) = 4;

writeln(d);
writeln(a);

for i in d do
  writeln(i, " -> ", a(i));
