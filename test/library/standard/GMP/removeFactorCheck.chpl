use BigInteger;

var a = new bigint(2);
var b = new bigint(-2);
var z = new bigint(0);
var c = new bigint();
var d = new bigint();

c = removeFactor(d, a, z); 
writeln("0 as a factor of 2 is undefined, so returning ", c," & ",d);

c = removeFactor(d, b, z);
writeln("0 as a factor of -2 is undefined, so returning ", c," & ",d);

c = removeFactor(d, z, z); 
writeln("0 as a factor of 0 is undefined, so returning ", c," & ",d);
