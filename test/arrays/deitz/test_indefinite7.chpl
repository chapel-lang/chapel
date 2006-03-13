var d : domain(string);
var a : [d] integer;

d.add("two");
d.add("zero");

a("two") = 7;
a("zero") = 4;

writeln(d);
writeln(a);

for i in d do
  writeln(i, " -> ", a(i));
