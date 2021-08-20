use BigInteger;

// Tests the sizeinbase deprecation message
var a = new bigint(-65536);
var b = new bigint(129);

writeln(a, " is ", a.sizeinbase(10), " digits in base 10");
writeln(a, " is ", a.sizeinbase(2), " digits in base 2");
writeln(b, " is ", b.sizeinbase(10), " digits in base 10");
writeln(b, " is ", b.sizeinbase(2), " digits in base 2");
