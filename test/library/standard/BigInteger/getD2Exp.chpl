use BigInteger;

var a: bigint = 64;
var (d, exp) = a.getD2Exp();
writeln(d); // expect .5
writeln(exp); // expect 7 (.5 * 2^7 = 64)
