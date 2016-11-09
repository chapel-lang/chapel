use BigInteger;

// Tests the logical and bit manipulation functions

var a = new bigint( 8);
var b = new bigint(40);

a.and(a, b); // 8 & 40 = 8
writeln(a);

b.set(2);
b.ior(a, b);  // 8 | 2 = 10
writeln(b);

a.set(6);
a.xor(a, b); // 6 XOR 10 = 12
writeln(a);

a.set(129);
a.com(a); // one's complement of 129, aka bitwise negation
writeln(a);

a.set(13);
writeln(a, " has ", a.popcount(), " bits set");

b.set(7);
writeln("hamming distance between ", a, " and ", b, " is ", a.hamdist(b));

a.set(79);
writeln(a, "'s first 0 bit is in position ", a.scan0(0));

a.set(96);
writeln(a, "'s first 1 bit is in position ", a.scan1(0));

a.setbit(2);  // set bit 2 (if not set, adds 2^2)
writeln(a);

a.clrbit(6);  // clear bit 6 (if set, subtracts 2^6)
writeln(a);

a.combit(3);  // flip the bit signifying 2^3 (adds or subtracts 2^3)
writeln(a);
writeln("bit 3 of a is ", a.tstbit(3));
