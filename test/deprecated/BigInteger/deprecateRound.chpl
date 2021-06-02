use BigInteger;

param down = Round.DOWN; // Should trigger the warning
param zero = Round.ZERO; // Should trigger the warning
param up = Round.UP;     // Should trigger the warning

// Every non-writeln function call after this point should trigger the warning
var a = new bigint(  8);
var b = new bigint( 10);
var c = new bigint(-27);
var d = new bigint();

d.div_q(c, a, up);
b.div_r(c, a, up);
writeln(d, " ", b);

d.div_q(c, a, down);
b.div_r(c, a, down);
writeln(d, " ", b);

d.div_q(c, a, zero); // same as UP   for negative integers
b.div_r(c, a, zero); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
d.div_qr(b, c, a, up);
writeln(d, " ", b);

d.div_qr(b, c, a, down);
writeln(d, " ", b);

d.div_qr(b, c, a, zero); // same as DOWN for positive integers
writeln(d, " ", b);

writeln();

c.neg(c);
d.div_q(c, 8, up);
b.div_r(c, 8, up);
writeln(d, " ", b);

d.div_q(c, 8, down);
b.div_r(c, 8, down);
writeln(d, " ", b);

d.div_q(c, 8, zero); // same as DOWN for positive integers
b.div_r(c, 8, zero); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
d.div_qr(b, c, 8, up);
writeln(d, " ", b);

d.div_qr(b, c, 8, down);
writeln(d, " ", b);

d.div_qr(b, c, 8, zero); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
writeln();


// q = (n / 2^d)
d.div_q_2exp(c, 3, up);
b.div_r_2exp(c, 3, up);
writeln(d, " ", b);

d.div_q_2exp(c, 3, down);
b.div_r_2exp(c, 3, down);
writeln(d, " ", b);

d.div_q_2exp(c, 3, zero);
b.div_r_2exp(c, 3, zero);
writeln(d, " ", b);
