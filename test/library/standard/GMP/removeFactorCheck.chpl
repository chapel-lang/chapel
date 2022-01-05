use BigInteger;

var a = new bigint(2);
var b = new bigint(-2);
var z = new bigint(0);
var c = new bigint();
var d = new bigint();

c=d.removeFactor(a, z); 
writeln("0 as a factor of 2 is undefined, so returning ", c," & ",d);

c=d.removeFactor(b, z);
writeln("0 as a factor of -2 is undefined, so returning ", c," & ",d);

c=d.removeFactor(z, z); 
writeln("0 as a factor of 0 is undefined, so returning ", c," & ",d);
