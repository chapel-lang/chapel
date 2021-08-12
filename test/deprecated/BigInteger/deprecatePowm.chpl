use BigInteger;

// Tests the power functions

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
