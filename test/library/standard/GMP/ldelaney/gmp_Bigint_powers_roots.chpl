use BigInteger;

// Tests the power and root functions

// powers
var a = new bigint( 2);
var b = new bigint( 4);
var c = new bigint(10);
var d = new bigint();

powMod(a, a, b, c);          // a = a^b mod c
writeln("2^4 mod 10 = ", a);

a.set(2);
powMod(a, a, 3, c);          // a = a^3 mod c
writeln("2^3 mod 10 = ", a);

a.set(2);
pow(d, a, 5);
writeln(d);

pow(d, 3, 5);
writeln(d);

// roots
root(a, d, 5); // fifth root of d = 3 // d^(1/5) = 3
writeln(a);

add(d, d, 10);
rootrem(a, b, d, 5);
writeln(a, ", remainder ", b);

b.set(17);
sqrt(d, b);
writeln("sqrt ", b, " = ", d);

sqrtrem(d, a, b);
writeln("sqrt ", b, " = ", d, " remainder ", a);

c.set(81);

var d2 = c.isPerfectPower();
writeln(d2);

d2 = b.isPerfectPower();
writeln(d2);

d2 = c.isPerfectSquare();
writeln(d2);

d2 = b.isPerfectSquare();
writeln(d2);
