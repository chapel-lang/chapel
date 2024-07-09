use BigInteger;

// Tests the division functions
var a = new bigint(  8);
var b = new bigint( 10);
var c = new bigint(-27);
var d = new bigint();

div(d, c, a, roundingMode.up);
rem(b, c, a, roundingMode.up);
writeln(d, " ", b);

div(d, c, a, roundingMode.down);
rem(b, c, a, roundingMode.down);
writeln(d, " ", b);

div(d, c, a, roundingMode.zero); // same as up   for negative integers
rem(b, c, a, roundingMode.zero); // same as down for positive integers
writeln(d, " ", b);

neg(c, c);
divRem(d, b, c, a, roundingMode.up);
writeln(d, " ", b);

divRem(d, b, c, a, roundingMode.down);
writeln(d, " ", b);

divRem(d, b, c, a, roundingMode.zero); // same as down for positive integers
writeln(d, " ", b);

writeln();

neg(c, c);
div(d, c, 8, roundingMode.up);
rem(b, c, 8, roundingMode.up);
writeln(d, " ", b);

div(d, c, 8, roundingMode.down);
rem(b, c, 8, roundingMode.down);
writeln(d, " ", b);

div(d, c, 8, roundingMode.zero); // same as down for positive integers
rem(b, c, 8, roundingMode.zero); // same as down for positive integers
writeln(d, " ", b);

neg(c, c);
divRem(d, b, c, 8, roundingMode.up);
writeln(d, " ", b);

divRem(d, b, c, 8, roundingMode.down);
writeln(d, " ", b);

divRem(d, b, c, 8, roundingMode.zero); // same as down for positive integers
writeln(d, " ", b);

neg(c, c);
writeln();


// q = (n / 2^d)
div2Exp(d, c, 3, roundingMode.up);
rem2Exp(b, c, 3, roundingMode.up);
writeln(d, " ", b);

div2Exp(d, c, 3, roundingMode.down);
rem2Exp(b, c, 3, roundingMode.down);
writeln(d, " ", b);

div2Exp(d, c, 3, roundingMode.zero);
rem2Exp(b, c, 3, roundingMode.zero);
writeln(d, " ", b);

writeln();

mod(d, c, a);
writeln(d);

var d2 : int = 0;

d2 = mod(d, c, 8);
writeln(d2);

b.set(16);
divExact(d, b, a);
writeln(d);

divExact(d, b, 4);
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
