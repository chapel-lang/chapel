config const n = 5;
config type myIdxType = int;
var p: [1..n] myIdxType;

writeln("p = ", p);
writeln("numBits(myIdxType) = ", numBits(myIdxType));
