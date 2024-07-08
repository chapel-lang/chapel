use BigInteger;

var a = new bigint(2);
var b = new bigint(4);
var d = new bigint();

pow(d , a, -1); // d = a^-1
writeln("2^-1 = ", d);

pow(d , a, -2); // d = a^-2
writeln("2^-2 = ", d);

pow(d , b, -1); // d = b^-1
writeln("4^-1 = ", d);

pow(d , b, -2); // d = b^-2
writeln("4^-2 = ", d);

pow(d , 1, -1); // d = 1^-1
writeln("1^-1 = ", d);

pow(d , 1, -2); // d = 1^-2
writeln("1^-2 = ", d);

pow(d , -1, -1); // d = -1^-1
writeln("-1^-1 = ", d);

pow(d , -1, -2); // d = -1^-2
writeln("-1^-2 = ", d);
