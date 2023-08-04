// deprecated in 1.32 by Jade
use BigInteger;

var a: bigint = 3;
var b: bigint = 4;
var res: bigint = 11;

addmul(res, a, b);
writeln(res);
addmul(res, a, 7:int);
writeln(res);
addmul(res, a, 8:uint);
writeln(res);

submul(res, a, b);
writeln(res);
submul(res, a, 7:int);
writeln(res);
submul(res, a, 8:uint);
writeln(res);
