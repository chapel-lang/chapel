use BigInteger, Map;

var m: map(bigint, string);

var i = "123456789123456789123456789123456789": bigint;

m.add(i, "hello");

writeln(m);
writeln(m["123456789123456789123456789123456789": bigint]);
