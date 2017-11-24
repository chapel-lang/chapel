use Random;

config param useNPB = true;
config param rtype = if useNPB then RNG.NPB else RNG.PCG;

config var n = 100;

var D = [1..n];

var A8: [D] int(8);
var A16: [D] int(16);
var A32: [D] int(32);
var A64: [D] int(64);

var randStr1 = new RandomStream(314159265);

randStr1.fillRandom(A8);
randStr1.fillRandom(A16);
randStr1.fillRandom(A32);
randStr1.fillRandom(A64);

writeln("A8 is: ", A8, "\n");
writeln("A16 is: ", A16, "\n");
writeln("A32 is: ", A32, "\n");
writeln("A64 is: ", A64, "\n");
writeln();

randStr1.fillRandom(A8, minval=0);
randStr1.fillRandom(A16, minval=0);
randStr1.fillRandom(A32, minval=0);
randStr1.fillRandom(A64, minval=0);

writeln("A8 is: ", A8, "\n");
writeln("A16 is: ", A16, "\n");
writeln("A32 is: ", A32, "\n");
writeln("A64 is: ", A64, "\n");
writeln();

randStr1.fillRandom(A8, minval=1, maxval=6);
randStr1.fillRandom(A16, minval=1, maxval=8);
randStr1.fillRandom(A32, minval=1, maxval=12);
randStr1.fillRandom(A64, minval=1, maxval=20);

writeln("A8 is: ", A8, "\n");
writeln("A16 is: ", A16, "\n");
writeln("A32 is: ", A32, "\n");
writeln("A64 is: ", A64, "\n");
writeln();
