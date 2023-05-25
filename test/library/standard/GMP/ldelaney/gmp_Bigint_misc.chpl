use BigInteger;

// Tests the miscellaneous functions
var a = new bigint(-65536);
var b = new bigint(129);

if a.isOdd() then writeln("a is odd");
else if a.isEven() then writeln("a is even");

if b.isOdd() then writeln("b is odd");
else if b.isEven() then writeln("b is even");

writeln(a, " is ", a.sizeInBase(10), " digits in base 10");
writeln(a, " is ", a.sizeInBase(2), " digits in base 2");
writeln(b, " is ", b.sizeInBase(10), " digits in base 10");
writeln(b, " is ", b.sizeInBase(2), " digits in base 2");

a.set(35);
b.set(9);

writeln(jacobi(b, a));
b.set(7);
writeln(jacobi(b, a));

a.set(11);
writeln(legendre(b, a));
b.set(9);
writeln(legendre(b, a));

a.set(14);
b.set(11);
writeln(kronecker(b, a));
writeln(kronecker(b, 35));
writeln(kronecker(b, 25));
writeln(kronecker(29, a));
writeln(kronecker(5, a));
