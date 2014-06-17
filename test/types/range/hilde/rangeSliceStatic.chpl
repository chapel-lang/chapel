// This is the Mark Potts bug:
// Range slicing does not seem to work using statically-typed procedure-call syntax.

var k = 1..6;
var l = 3..;

writeln("int:     ", k, "[", l, "] = ", k[l]);
writeln("1..6 (3..) = ", 1..6 (3..));
writeln("1..6 [3..] = ", 1..6 [3..]);
