// deprecated in 1.32 by Jade
use BigInteger;

var a: bigint = 0b010101010101;
var idx: uint;

idx = a.scan0(2);
writeln(idx);

idx = a.scan1(3);
writeln(idx);
