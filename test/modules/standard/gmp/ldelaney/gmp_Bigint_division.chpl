use BigInteger;

// Tests the division functions
var a = new bigint(  8);
var b = new bigint( 10);
var c = new bigint(-27);
var d = new bigint();

d.div_q(c, a, Round.UP);
b.div_r(c, a, Round.UP);
writeln(d, " ", b);

d.div_q(c, a, Round.DOWN);
b.div_r(c, a, Round.DOWN);
writeln(d, " ", b);

d.div_q(c, a, Round.ZERO); // same as UP   for negative integers
b.div_r(c, a, Round.ZERO); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
d.div_qr(b, c, a, Round.UP);
writeln(d, " ", b);

d.div_qr(b, c, a, Round.DOWN);
writeln(d, " ", b);

d.div_qr(b, c, a, Round.ZERO); // same as DOWN for positive integers
writeln(d, " ", b);

writeln();

c.neg(c);
d.div_q(c, 8, Round.UP);
b.div_r(c, 8, Round.UP);
writeln(d, " ", b);

d.div_q(c, 8, Round.DOWN);
b.div_r(c, 8, Round.DOWN);
writeln(d, " ", b);

d.div_q(c, 8, Round.ZERO); // same as DOWN for positive integers
b.div_r(c, 8, Round.ZERO); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
d.div_qr(b, c, 8, Round.UP);
writeln(d, " ", b);

d.div_qr(b, c, 8, Round.DOWN);
writeln(d, " ", b);

d.div_qr(b, c, 8, Round.ZERO); // same as DOWN for positive integers
writeln(d, " ", b);

c.neg(c);
writeln();


// q = (n / 2^d)
d.div_q_2exp(c, 3, Round.UP);
b.div_r_2exp(c, 3, Round.UP);
writeln(d, " ", b);

d.div_q_2exp(c, 3, Round.DOWN);
b.div_r_2exp(c, 3, Round.DOWN);
writeln(d, " ", b);

d.div_q_2exp(c, 3, Round.ZERO);
b.div_r_2exp(c, 3, Round.ZERO);
writeln(d, " ", b);

writeln();

d.mod(c, a);
writeln(d);

var d2 : uint = 0;

d2 = d.mod(c, 8);
writeln(d2);

b.set(16);
d.divexact(b, a);
writeln(d);

d.divexact(b, 4);
writeln(d);

var d3 : int = b.divisible_p(a);
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
