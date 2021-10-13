use BigInteger, Map;

var m: map(bigint, string);

var i: bigint = 123;

m.add(i, "hello");

writeln(m);
writeln(m[123:bigint]);
