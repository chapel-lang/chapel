use BitOps;

var x: uint(64) = 0xFF773311FF773311;
writeln(popCount(x));
var x2: uint(32) = 0xF731F731;
writeln(popCount(x2));
var x3: uint(16) = 0xF731;
writeln(popCount(x3));
var x4: uint(8) = 0xF1;
writeln(popCount(x4));

var y: int(64) = 0x0F773311FF773311;
writeln(popCount(y));
var y2: int(32) = 0x0731F731;
writeln(popCount(y2));
var y3: int(16) = 0x0731;
writeln(popCount(y3));
var y4: int(8) = 0x71;
writeln(popCount(y4));
