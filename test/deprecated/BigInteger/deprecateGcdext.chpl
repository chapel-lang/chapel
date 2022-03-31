use BigInteger;

var a   = new bigint(27);
var b   = new bigint(19);
var c   = new bigint(720);
var d   = new bigint(1000);
var e   = new bigint(375);

a.gcdext(e, b, c, d);

writeln("gcd(", c, ", ", d, ") = ", a);
writeln(c, " * ", e, " + ", d, " * ", b, " = ", a);
