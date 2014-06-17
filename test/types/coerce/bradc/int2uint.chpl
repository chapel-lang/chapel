// Test that coercions from ints to uints and back again work

var u: uint = 0;
var i: int = 1;

u = i;
writeln("u is: ", u);

u += 1;
i = u;
writeln("i is: ", i);
