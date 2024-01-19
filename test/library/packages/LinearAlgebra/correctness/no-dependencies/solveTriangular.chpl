use LinearAlgebra, Random, TestUtils;

/*
  This test checks the correctness of the `solve_[tril/triu]` methods.
  Different offsets and array initializations/sizes are considered for robustness.
*/

const offsets = [0, 1, 11, -7]; // tested offsets
const eps = 1e-7; // error tolerance

var res = true;

/*
  Series 1: explicit initialization, with non-unitary diagonal, of size 3 x 3.

  Expected results (manually computed):
    solve_tril(A,            b) =  0.5 5.5 5.5
    solve_triu(transpose(A), b) = 15.6 4.4 1.2
*/
{
  const n = 3;

  var A:  [0..#n, 0..#n] real;
  var At: [0..#n, 0..#n] real;
  var b:  [0..#n] real;

  A[0, ..] = [ 2.0, 0.0, 0.0];
  A[1, ..] = [-7.0, 1.0, 0.0];
  A[2, ..] = [ 0.5,-2.0, 2.5];

  At = transpose(A);

  b = [1.0, 2.0, 3.0];

  for offset in offsets {
    // reindex
    const Ao  = A.reindex(offset..#n, offset..#n);
    const Ato = At.reindex(offset..#n, offset..#n);
    const bo  = b.reindex(offset..#n);

    // solve
    const xl = solve_tril(Ao, bo, false);
    const xu = solve_triu(Ato, bo);

    // check result
    if !almostEquals(dot(Ao, xl), bo) || !almostEquals(dot(Ato, xu), bo) {
      res = false;
      writeln("test with offset ", offset, " in series 1 failed");
    }
  }
}

/*
  Series 2: explicit initialization, with unitary diagonal, of size 5 x 5.

  Expected results (manually computed):
    solve_tril(A,            b) = 1.0    0.0  -0.5 -0.75 0.625
    solve_triu(transpose(A), b) = 0.625 -0.75 -0.5  0.0  1.0
*/
{
  const n = 5;

  var A:  [0..#n, 0..#n] real;
  var At: [0..#n, 0..#n] real;
  var b:  [0..#n] real;

  A[0,..] = [ 1.0, 0.0, 0.0, 0.0, 0.0];
  A[1,..] = [-0.5, 1.0, 0.0, 0.0, 0.0];
  A[2,..] = [ 0.0,-0.5, 1.0, 0.0, 0.0];
  A[3,..] = [ 0.0, 0.0,-0.5, 1.0, 0.0];
  A[4,..] = [ 0.0, 0.0, 0.0,-0.5, 1.0];

  At = transpose(A);

  b = [1.0, -0.5, -0.5, -0.5, 1.0];

  for offset in offsets {
    // reindex
    const Ao  = A.reindex(offset..#n, offset..#n);
    const Ato = At.reindex(offset..#n, offset..#n);
    const bo  = b.reindex(offset..#n);

    // solve
    const xl = solve_tril(Ao, bo, true);
    const xu = solve_triu(Ato, bo);

    // check result
    if !almostEquals(dot(Ao, xl), bo) || !almostEquals(dot(Ato, xu), bo) {
      res = false;
      writeln("test with offset ", offset, " in series 2 failed");
    }
  }
}

/*
  Series 3: random initialization of size 20 x 20.
*/
{
  const n = 20;

  var A:  [0..#n, 0..#n] real;
  var At: [0..#n, 0..#n] real;
  var b:  [0..#n] real;

  fillRandom(A, min=-100.0, max=100.0, seed=314159);
  for i in 0..#n {
    for j in i+1..<n {
      A(i,j) = 0.0;
    }
  }
  At = transpose(A);
  fillRandom(b, min=-100.0, max=100.0, seed=2718281);

  for offset in offsets {
    // reindex
    const Ao  = A.reindex(offset..#n, offset..#n);
    const Ato = At.reindex(offset..#n, offset..#n);
    const bo  = b.reindex(offset..#n);

    // solve
    const xl = solve_tril(Ao, bo, false);
    const xu = solve_triu(Ato, bo);

    // check result
    if !almostEquals(dot(Ao, xl), bo) || !almostEquals(dot(Ato, xu), bo) {
      res = false;
      writeln("test with offset ", offset, " in series 3 failed");
    }
  }
}

if res then writeln("All tests passed");
else writeln("Some tests failed");
