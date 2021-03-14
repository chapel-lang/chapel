var r = 2.0;

while !isinf(r*r) do r *= r;

var c1 = r + 0.0i;
var c2 = 0.0 + _r2i(r);

writeln("abs(", c1, ") = ", abs(c1));
writeln("abs(", c2, ") = ", abs(c2));
