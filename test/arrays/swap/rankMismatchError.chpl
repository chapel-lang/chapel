use BlockDist;

var d1 = {0..9, 0..9} dmapped Block({0..9, 0..9});
var d2 = {0..99} dmapped Block({0..99});

var a1: [d1] int;
var a2: [d2] int;

a1 = 10;
a2 = 20;

a1 <=> a2;

writeln(a1);
writeln(a2);
