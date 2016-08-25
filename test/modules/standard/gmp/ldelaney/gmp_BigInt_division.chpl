use BigIntegers;
use Round; // the rounding enum

// Tests the division functions

var a = new BigInt(8);
var b = new BigInt(10);
var c = new BigInt(-27);
var d = new BigInt();

d.div_q(UP, c, a);
b.div_r(UP, c, a);
writeln(d, " ", b);
d.div_q(DOWN, c, a);
b.div_r(DOWN, c, a);
writeln(d, " ", b);
d.div_q(ZERO, c, a); // same as UP for negative integers
b.div_r(ZERO, c, a); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
d.div_qr(UP, b, c, a);
writeln(d, " ", b);
d.div_qr(DOWN, b, c, a);
writeln(d, " ", b);
d.div_qr(ZERO, b, c, a); // same as DOWN for positive integers
writeln(d, " ", b);

writeln();

c.neg(c);
d.div_q(UP, c, 8);
b.div_r(UP, c, 8);
writeln(d, " ", b);
d.div_q(DOWN, c, 8);
b.div_r(DOWN, c, 8);
writeln(d, " ", b);
d.div_q(ZERO, c, 8); // same as DOWN for positive integers
b.div_r(ZERO, c, 8); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
d.div_qr(UP, b, c, 8);
writeln(d, " ", b);
d.div_qr(DOWN, b, c, 8);
writeln(d, " ", b);
d.div_qr(ZERO, b, c, 8); // same as DOWN for positive integers
writeln(d, " ", b);
c.neg(c);

writeln();
var d2 : uint;
d2 = div(UP, c, 8);
writeln(d2);
d2 = div(DOWN, c, 8);
writeln(d2);
d2 = div(ZERO, c, 8);
writeln(d2);

writeln();

// q = (n / 2^d)
d.div_q_2exp(UP, c, 3);
b.div_r_2exp(UP, c, 3);
writeln(d, " ", b);
d.div_q_2exp(DOWN, c, 3);
b.div_r_2exp(DOWN, c, 3);
writeln(d, " ", b);
d.div_q_2exp(ZERO, c, 3);
b.div_r_2exp(ZERO, c, 3);
writeln(d, " ", b);

writeln();
d.mod(c, a);
writeln(d);
d2 = d.mod(c, 8); // Note: this returns a value instead of modifying a BigInt
writeln(d2);

b.set(16);
d.divexact(b, a);
writeln(d);
d.divexact(b, 4);
writeln(d);

var d3 : int;
d3 = b.divisible_p(a);
writeln(d3);
d3 = b.divisible_p(3);
writeln(d3, " ");
writeln(b.divisible_2exp_p(3)); // divisble by 2^3
a.set(25);
b.set(20);
c.set(5);
d3 = a.congruent_p(c, b); // 25 congruent to 5 mod 20
writeln(d3);
d3 = a.congruent_p(10, 20); //25 not congruent to 10 mod 20
writeln(d3);
a.set(45);
d3 = a.congruent_2exp_p(c, 3); //45 congruent to 5 mod 8
writeln(d3);
