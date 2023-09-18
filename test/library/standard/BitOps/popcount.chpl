use BitOps;

var x: uint = 0xFF773311FF773311;
writeln(popCount(x));
var x2: uint(32) = 0xF731F731;
writeln(popCount(x2));
var x3: uint(16) = 0xF731;
writeln(popCount(x3));
var x4: uint(8) = 0xF1;
writeln(popCount(x4));
