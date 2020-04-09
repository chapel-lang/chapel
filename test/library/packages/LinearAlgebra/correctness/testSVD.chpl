use LinearAlgebra;
use TestUtils;

// TODO: AssertEqual -> AssertApproximate

{
  var A = Matrix([1.0, 2.0, 3.0],
                 [3.0, 2.0, 4.0],
                 [1.0, 2.0, 5.0], eltType=real(32));

  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt);
  assertAlmostEqual(A, A2, 'svd - real(32)');
}

{
  var A = Matrix([1.0, 2.0, 3.0],
                 [3.0, 2.0, 4.0],
                 [1.0, 2.0, 5.0], eltType=real(64));

  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt);
  assertAlmostEqual(A, A2, 'svd - real(64)');
}

{
  var A = Matrix([1.0+1.0i, 2.0+1.0i, 3.0+1.0i],
                 [3.0-2.0i, 2.0+1.0i, 4.0+1.0i],
                 [1.0+1.0i, 2.0+1.0i, 5.0+1.0i], eltType=complex(64));

  var (u, s, vt) = svd(A);

  var sComplex = s:complex(64);
  var A2 = u.dot(diag(sComplex)).dot(vt);
  assertAlmostEqual(A, A2, 'svd - complex(64)');
}

{
  var A = Matrix([1.0+1.0i, 2.0+1.0i, 3.0+1.0i],
                 [3.0-2.0i, 2.0+1.0i, 4.0+1.0i],
                 [1.0+1.0i, 2.0+1.0i, 5.0+1.0i], eltType=complex(128));

  var (u, s, vt) = svd(A);

  var sComplex = s:complex(128);
  var A2 = u.dot(diag(sComplex)).dot(vt);
  assertAlmostEqual(A, A2, 'svd - complex(128)');
}
