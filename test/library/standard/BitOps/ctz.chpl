use BitOps;

var x: uint(64) = 0x0004000000000000;
writeln(ctz(x));
var x2: uint(32) = 0x00040000;
writeln(ctz(x2));
var x3: uint(16) = 0x0040;
writeln(ctz(x3));
var x4: uint(8) = 0x04;
writeln(ctz(x4));

var x5: int(64) = 0x0004000000000000;
writeln(ctz(x5));
var x6: int(32) = 0x00040000;
writeln(ctz(x6));
var x7: int(16) = 0x0040;
writeln(ctz(x7));
var x8: int(8) = 0x04;
writeln(ctz(x8));
