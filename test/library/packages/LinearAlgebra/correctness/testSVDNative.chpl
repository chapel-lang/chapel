use LinearAlgebra;
use TestUtils;

// TODO: AssertEqual -> AssertApproximate

{
  var A = Matrix([1.0, 2.0, 3.0],
                 [3.0, 2.0, 4.0],
                 [1.0, 2.0, 5.0], eltType=real(32));
  var Acopy = A;
  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt.T);
  assertAlmostEqual(Acopy, A2, 'svd - real(32)');
}

{
  var A = Matrix([1.0, 2.0, 3.0],
                 [3.0, 2.0, 4.0],
                 [1.0, 2.0, 5.0], eltType=real(64));
  var Acopy = A;
  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt.T);
  assertAlmostEqual(Acopy, A2, 'svd - real(64)');
}
