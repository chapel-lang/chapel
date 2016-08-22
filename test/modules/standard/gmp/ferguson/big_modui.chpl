use GMP;

var x = new BigInt(20);
var y = new BigInt(0);

y.mod(x, 11);

writeln(y);
