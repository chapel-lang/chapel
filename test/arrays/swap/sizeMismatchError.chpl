use BlockDist;

var d1 = {0..9 by 2} dmapped Block({1..10});
var d2 = {0..9} dmapped Block({1..10});

var a1: [d1] int;
var a2: [d2] int;

a1 = 10;
a2 = 20;

a1 <=> a2;

writeln(a1);
writeln(a2);
