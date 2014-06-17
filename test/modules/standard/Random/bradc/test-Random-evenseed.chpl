// this test is the same as its sibling
// in Mary Beth's direetory, exccept that
// it uses an even seed to make sure that
// the module is rounding to an odd number
// as it should.

proc main() {

use Random;

var Vector1 = {1..20};
var Vec1 : [Vector1] real;

var rng = new RandomStream(314159264);

rng.fillRandom(Vec1);
writeln(Vec1);

fillRandom(Vec1,314159264);
writeln(Vec1);
fillRandom(Vec1,314159264);
writeln(Vec1);

delete rng;
}

