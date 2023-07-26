use BigInteger;

param down = Round.DOWN; // Should trigger the warning
param zero = Round.ZERO; // Should trigger the warning
param up = Round.UP;     // Should trigger the warning

// Every non-writeln function call after this point should trigger the warning
var a = new bigint(  8);
var c = new bigint(-27);
var d = new bigint();

d.div_q(c, a, up);
writeln(d);

d.div_q(c, a, down);
writeln(d);

d.div_q(c, a, zero); // same as UP   for negative integers
writeln(d);

writeln();

neg(c, c);
d.div_q(c, 8, up);
writeln(d);

d.div_q(c, 8, down);
writeln(d);

d.div_q(c, 8, zero); // same as DOWN for positive integers
writeln(d);

writeln();
// Check default arguments, too
d.div_q(c, a); // same as UP   for negative integers
writeln(d);
