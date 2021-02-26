use LinearAlgebra;
use Random;
use TestUtils;

// Basic test: m > n
{
  var y = Vector([0.3, 1.1, 1.5, 2.0, 3.2, 6.6, 8.6]);

  // Fitting quadratic: a + x**2
  var M = Matrix([  1.0  ,   1.0  ],
                 [  1.0  ,   6.25],
                 [  1.0  ,  12.25],
                 [  1.0  ,  16.0  ],
                 [  1.0  ,  25.0  ],
                 [  1.0  ,  49.0  ],
                 [  1.0  ,  72.25]);


  // Debug output
  if !TestUtils.correctness {
    writeln('M:');
    writeln(M);
    writeln('y:');
    writeln(y);
  }

  var (p, res, rnk, s) = leastSquares(M, y);
  var pSolved = directLeastSquares(M, y);

  assertAlmostEqual(p, pSolved,                  "leastSquares(M, y) (p)");
  assertAlmostEqual(res, 0.40826652374403327,    "leastSquares(M, y) (residues)");
  assertEqual(rnk, M.shape(1),                            "leastSquares(M, y) (rank)");
  assertAlmostEqual(s, [93.25228799, 1.7883749], "leastSquares(M, y) (s)");


  // Debug output
  if !TestUtils.correctness {
    writeln('p: ', p);
    writeln('res: ', res);
    writeln('rnk: ', rnk);
    writeln('s: ', s);
  }
}


// Basic test: m <= n
{

  var y = Vector(0.0, 1.0, 8.0);
  var x = Vector(1.0, 2.0, 3.0);
  var M = vander(x, 3);

  // Debug output
  if !TestUtils.correctness {
    writeln('M:');
    writeln(M);
    writeln('y:');
    writeln(y);
  }

  var (p, res, rnk, s) = leastSquares(M, y);

  // compute solutions
  var pSolved = directLeastSquares(M, y);
  var (u, sv, vt) = svd(M);

  assertAlmostEqual(p, pSolved,     "leastSquares(M, y) (p)");
  assertAlmostEqual(y, dot(M, p),   "leastSquares(M, y) (dot(M, p))");
  assertEqual(rnk, M.shape(1),      "leastSquares(M, y) (rank)");
  assertAlmostEqual(s, sv,          "leastSquares(M, y) (s)");


  // Debug output
  if !TestUtils.correctness {
    writeln('p: ', p);
    writeln('res: ', res);
    writeln('rnk: ', rnk);
    writeln('s: ', s);
  }
}

/* Compute least squares solution via solve() */
proc directLeastSquares(a: [], b: [], cmplx=false) {
  var at = a.T;
  if cmplx then at = conjg(at);
  var a1 = dot(at, a),
      b1 = dot(at, b);
  return solve(a1, b1);
}

