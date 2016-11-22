use BigInteger;

// Tests the power and root functions

// powers
var a = new bigint( 2);
var b = new bigint( 4);
var c = new bigint(10);
var d = new bigint();

a.powm(a, b, c);          // a = a^b mod c
writeln("2^4 mod 10 = ", a);

a.set(2);
a.powm(a, 3, c);          // a = a^3 mod c
writeln("2^3 mod 10 = ", a);

a.set(2);
d.pow(a, 5);
writeln(d);

d.pow(3, 5);
writeln(d);

// roots
a.root(d, 5); // fifth root of d = 3 // d^(1/5) = 3
writeln(a);

d.add(d, 10);
a.rootrem(b, d, 5);
writeln(a, ", remainder ", b);

b.set(17);
d.sqrt(b);
writeln("sqrt ", b, " = ", d);

d.sqrtrem(a, b);
writeln("sqrt ", b, " = ", d, " remainder ", a);

c.set(81);

var d2 = c.perfect_power_p();
writeln(d2);

d2 = b.perfect_power_p();
writeln(d2);

d2 = c.perfect_square_p();
writeln(d2);

d2 = b.perfect_square_p();
writeln(d2);
