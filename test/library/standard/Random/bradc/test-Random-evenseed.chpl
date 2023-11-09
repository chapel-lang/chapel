// this test is the same as its sibling
// in Mary Beth's direetory, exccept that
// it uses an even seed to make sure that
// the module is rounding to an odd number
// as it should.

use Random;

proc main() {
  var Vector1 = {1..20};
  var Vec1 : [Vector1] real;
  var Vec2 : [Vector1] real;
  var Vec3 : [Vector1] real;

  var rng = new randomStream(real, 314159264);

  rng.fill(Vec1);

  fillRandom(Vec2, 314159264);
  fillRandom(Vec3, 314159264);

  for (a,b,c) in zip(Vec1, Vec2, Vec3) {
    assert( a == b );
    assert( b == c );
  }
}
