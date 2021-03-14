// this test is the same as its sibling
// in Mary Beth's direetory, exccept that
// it uses an even seed to make sure that
// the module is rounding to an odd number
// as it should.

use Random;
config param useNPB = true;
config param rtype = if useNPB then RNG.NPB else RNG.PCG;

proc main() {
  var Vector1 = {1..20};
  var Vec1 : [Vector1] real;
  var Vec2 : [Vector1] real;
  var Vec3 : [Vector1] real;

  var rng = createRandomStream(real, 314159264, algorithm=rtype);

  rng.fillRandom(Vec1);

  fillRandom(Vec2,314159264, algorithm=rtype);
  fillRandom(Vec3,314159264, algorithm=rtype);

  for (a,b,c) in zip(Vec1, Vec2, Vec3) {
    assert( a == b );
    assert( b == c );
  }
}

