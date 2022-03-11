use BigInteger;

var a: bigint = 64;
var (exp, d) = a.get_d_2exp();
writeln(d); // expect .5
writeln(exp); // expect 7 (.5 * 2^7 = 64)
