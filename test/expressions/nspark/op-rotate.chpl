use Random.PCGRandom;
use BitOps;

config const numTests = 1000;

proc test_rotate(type intType) where isIntegralType(intType) {
  param bits = numBits(intType);
  var R = new RandomStream(intType);
  for 1..numTests {
    var X = R.getNext();
    // Left rotate
    for N in 0..(bits-1) {
      var a = rotl(X, N);
      var b = X <<< N;
      if a != b then
        halt("rotl(%xu, %i) = %xu  !=  (%xu <<< %i) = %xu".format(
               X, N, a, X, N, b));
    }
    // Right rotate
    for N in 0..(bits-1) {
      var a = rotr(X, N);
      var b = X >>> N;
      if a != b then
        halt("rotr(%xu, %i) = %xu  !=  (%xu >>> %i) = %xu".format(
               X, N, a, X, N, b));
    }
  }
  delete R;
}

test_rotate(uint( 8));
test_rotate(uint(16));
test_rotate(uint(32));
test_rotate(uint(64));

test_rotate( int( 8));
test_rotate( int(16));
test_rotate( int(32));
test_rotate( int(64));
