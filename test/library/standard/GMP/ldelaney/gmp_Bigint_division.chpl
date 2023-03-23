use BigInteger;
import round; // avoid conflict with Math.round

// Tests the division functions
var a = new bigint(  8);
var b = new bigint( 10);
var c = new bigint(-27);
var d = new bigint();

divQ(d, c, a, round.up);
divR(b, c, a, round.up);
writeln(d, " ", b);

divQ(d, c, a, round.down);
divR(b, c, a, round.down);
writeln(d, " ", b);

divQ(d, c, a, round.zero); // same as up   for negative integers
divR(b, c, a, round.zero); // same as down for positive integers
writeln(d, " ", b);

neg(c, c);
divQR(d, b, c, a, round.up);
writeln(d, " ", b);

divQR(d, b, c, a, round.down);
writeln(d, " ", b);

divQR(d, b, c, a, round.zero); // same as down for positive integers
writeln(d, " ", b);

writeln();

neg(c, c);
divQ(d, c, 8, round.up);
divR(b, c, 8, round.up);
writeln(d, " ", b);

divQ(d, c, 8, round.down);
divR(b, c, 8, round.down);
writeln(d, " ", b);

divQ(d, c, 8, round.zero); // same as down for positive integers
divR(b, c, 8, round.zero); // same as down for positive integers
writeln(d, " ", b);

neg(c, c);
divQR(d, b, c, 8, round.up);
writeln(d, " ", b);

divQR(d, b, c, 8, round.down);
writeln(d, " ", b);

divQR(d, b, c, 8, round.zero); // same as down for positive integers
writeln(d, " ", b);

neg(c, c);
writeln();


// q = (n / 2^d)
divQ2Exp(d, c, 3, round.up);
divR2Exp(b, c, 3, round.up);
writeln(d, " ", b);

divQ2Exp(d, c, 3, round.down);
divR2Exp(b, c, 3, round.down);
writeln(d, " ", b);

divQ2Exp(, c, 3, round.zero);
divR2Exp(, c, 3, round.zero);
writeln(d, " ", b);

writeln();

mod(d, c, a);
writeln(d);

var d2 : int = 0;

d2 = mod(d, c, 8);
writeln(d2);

b.set(16);
divexact(d, b, a);
writeln(d);

divexact(d, b, 4);
writeln(d);

var d3 : bool = b.isDivisible(a);
writeln(d3);

d3 = b.isDivisible(3);
writeln(d3, " ");
writeln(b.isDivisibleBy2Pow(3)); // divisble by 2^3

a.set(25);
b.set(20);
c.set(5);

d3 = a.isCongruent(c, b); // 25 congruent to 5 mod 20
writeln(d3);

d3 = a.isCongruent(10, 20); //25 not congruent to 10 mod 20
writeln(d3);

a.set(45);
d3 = a.isCongruentBy2Pow(c, 3); //45 congruent to 5 mod 8
writeln(d3);
