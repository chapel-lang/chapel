use BigInteger;

var a: bigint;
a.set(79);
writeln(a, "'s first 0 bit is in position ", a.scan0(starting_bit=0));

a.set(96);
writeln(a, "'s first 1 bit is in position ", a.scan1(starting_bit=0));
