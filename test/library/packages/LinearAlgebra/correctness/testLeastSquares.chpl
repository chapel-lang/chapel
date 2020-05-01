use LinearAlgebra;
use TestUtils;


// Basic test: M < N
{
  var x = Vector([1.0, 2.5, 3.5, 4.0, 5.0, 7.0, 8.5]);
  var y = Vector([0.3, 1.1, 1.5, 2.0, 3.2, 6.6, 8.6]);

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

  assertAlmostEqual(p, pSolved,                  "leastSquares(M, y) (x)");
  assertAlmostEqual(res, 0.40826652374403327,    "leastSquares(M, y) (residues)");
  assertEqual(rnk, 2,                            "leastSquares(M, y) (rank)");
  assertAlmostEqual(s, [93.25228799, 1.7883749], "leastSquares(M, y) (s)");


  // Debug output
  if !TestUtils.correctness {
    writeln('p: ', p);
    writeln('res: ', res);
    writeln('rnk: ', rnk);
    writeln('s: ', s);
  }
}


// Basic test: M < N
{
  var x = Vector([1.0, 2.5, 3.5, 4.0, 5.0, 7.0, 8.5]);
  var y = Vector([0.3, 1.1, 1.5, 2.0, 3.2, 6.6, 8.6]);

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

  assertAlmostEqual(p, pSolved,                  "leastSquares(M, y) (x)");
  assertAlmostEqual(res, 0.40826652374403327,    "leastSquares(M, y) (residues)");
  assertEqual(rnk, 2,                            "leastSquares(M, y) (rank)");
  assertAlmostEqual(s, [93.25228799, 1.7883749], "leastSquares(M, y) (s)");


  // Debug output
  if !TestUtils.correctness {
    writeln('p: ', p);
    writeln('res: ', res);
    writeln('rnk: ', rnk);
    writeln('s: ', s);
  }
}


                              //            x, direct_lstsq(a, b, cmplx=0),
                              //            rtol=25 * _eps_cast(a1.dtype),
                              //            atol=25 * _eps_cast(a1.dtype),
                              //            err_msg="driver: %s" % lapack_driver)

/* Compute least squares solution via solve() */
proc directLeastSquares(a: [], b: [], cmplx=false) {
  var at = a.T;
  if cmplx then at = conjg(at);
  var a1 = dot(at, a),
      b1 = dot(at, b);
  return solve(a1, b1);
}

