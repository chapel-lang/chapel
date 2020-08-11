use LinearAlgebra;
use Random;
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
  var x: [1..7] real = [1.0, 2.5, 3.5, 4.0, 5.0, 7.0, 8.5];
  var y: [1..7] real = [0.3, 1.1, 1.5, 2.0, 3.2, 6.6, 8.6];

  var M: [1..7, 1..2] real;

  M = Matrix([  1.0  ,   1.0  ],
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

/*
// Fitting a polynomial to some data
{
  var rng = createRandomStream(real);
  const nx = 10; // x data points
  const ny = 10; // y data points
  const m = nx*ny;
  const n = 10; // Order of polynomial to fit

  var A:[0..#m, 0..#n] real;
  var b: [0..#m] real;
  var c = [1.5, 2.1,-2.7, -3.1,4.5,-6.7, 0.8,-0.7,4.1,-3.7];

  var k = 0;
  // Generate some data
  for i in 0..#nx {
    var x = (i+1)*0.01;
    for j in 0..#ny {
      var y = (j+1)*0.02;
      A[i, 0] = 1;
      A[i, 1] = x;
      A[i, 2] = y;
      A[i, 3] = x*x;
      A[i, 4] = x*y;
      A[i, 5] = y*y;
      A[i, 6] = x*x*x;
      A[i, 7] = x*x*y;
      A[i, 8] = x*y*y;
      A[i, 9] = y*y*y;
      b[i]=(c[0]+
            c[1]*x     + c[2]*y     +
            c[3]*x*x   + c[4]*x*y   + c[5]*y*y   +
            c[6]*x*x*x + c[7]*x*x*y + c[8]*x*y*y + c[9]*y*y*y
            ) * (0.9995+0.001*rng.getNext());  // random noise added
      k += 1;
    }
  }

  writeln('A:');
  writeln(A);

  writeln('B:');
  writeln(b);

  var (x1, residue, rank, s) = leastSquares(A, b);
  writeln(x1);
}
 */

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

