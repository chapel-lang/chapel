config param n1 = 100;

use BlockDist;

const BDom1D = [1..n1] distributed new dist(new Block(1, bbox=[1..n1]));
const B2Dom1D = BDom1D distributed new dist(new Block(1, bbox=[n1+1..n1*2]));

var BA1D: [BDom1D] int;
BA1D = 1;
var TA1D => BA1D;
TA1D = 2;
var B2A1D: [B2Dom1D] int;
B2A1D = 3;
TA1D => B2A1D;
TA1D = 4;

writeln("FAIL: We should not get here!"); // for multiple locales only?
writeln(BA1D);
writeln(B2A1D);
writeln(TA1D);
