use LinearAlgebra, Random, TestUtils;

/*
  This test checks the correctness of the `solve` method (implicitly `lu`).
  Different offsets and array initializations/sizes are considered for robustness.
*/

const offsets = [0, 11, -7]; // tested offsets
const eps = 1e-7; // error tolerance

var res = true;

/*
  Series 1: explicit initialization of size 3 x 3.

  Expected results (manually computed): 5.0 -2.0 3.0
*/
{
  const n = 3;

  var A:  [0..#n, 0..#n] real;
  var At: [0..#n, 0..#n] real;
  var b:  [0..#n] real;

  A[0, ..] = [ 2.0, 1.0, 3.0];
  A[1, ..] = [ 4.0,-1.0, 3.0];
  A[2, ..] = [-2.0, 5.0, 5.0];

  b = [17.0, 31.0, -5.0];

  for offset in offsets {
    // reindex
    var Ao  = A.reindex(offset..#n, offset..#n);
    var bo  = b.reindex(offset..#n);

    // solve
    const x = solve(Ao, bo);
    assertAlmostEqual(b, bo, "solve() modified its argument");

    // check results
    if !almostEquals(dot(Ao, x), bo) {
      res = false;
      writeln("test with offset ", offset, " in serie 1 failed");
    }
  }
}

/*
  Series 2: random initialization of size 20 x 20.
*/
{
  const n = 20;

  var A:  [0..#n, 0..#n] real;
  var b:  [0..#n] real;

  fillRandom(A, min=-100.0, max=100.0);
  fillRandom(b, min=-100.0, max=100.0);

  for offset in offsets {
    // reindex
    var Ao  = A.reindex(offset..#n, offset..#n);
    var bo  = b.reindex(offset..#n);

    // solve
    const x = solve(Ao, bo);
    bo = b; // `solve` overrites bo

    // check result
    if !almostEquals(dot(Ao, x), bo) {
      res = false;
      writeln("test with offset ", offset, " in serie 2 failed");
    }
  }
}

if res then writeln("All tests passed");
else writeln("Some tests failed");
