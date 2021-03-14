use BitOps;

var x: uint = 0xFF773311FF773311;
writeln(popcount(x));
var x2: uint(32) = 0xF731F731;
writeln(popcount(x2));
var x3: uint(16) = 0xF731;
writeln(popcount(x3));
var x4: uint(8) = 0xF1;
writeln(popcount(x4));
