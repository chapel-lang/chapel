use BlockDist;

var D1 = {1..10, 1..10} dmapped Block({1..10, 1..10});
var D2 = {1..10, 1..10} dmapped Block({1..1000, 1..1000});
var a: [D1] int = 1;
var b: [D2] int = 2;
a <=> b;
writeln(a);
writeln(b);
