config param n1 = 100;

use BlockDist;

const Dom1D: domain(1) = [1..n1];
const BlockDom1D = Dom1D distributed new dist(new Block(1, bbox=[1..n1]));

var A1D: [Dom1D] int;
A1D = 1;
var TA1D => A1D;
TA1D = 2;
var BlockA1D: [BlockDom1D] int;
BlockA1D = 3;
TA1D => BlockA1D;
TA1D = 4;

writeln("FAIL: We should not get here!");
writeln(A1D);
writeln(BlockA1D);
writeln(TA1D);
