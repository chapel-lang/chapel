use BigInteger;

var a = new bigint(0);
var b = new bigint(5);
var c = new bigint(2);

var status = a.invert(b, c);
writeln("Non-zero status: ", status != 0);
