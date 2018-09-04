use LinearAlgebra;
use TestUtils;

// TODO: real(32), real(64), complex(64)
// TODO: AssertEqual -> AssertApproximate

{
  var A = Matrix([1.0, 2.0, 3.0],
                 [3.0, 2.0, 4.0],
                 [1.0, 2.0, 5.0], eltType=real(32));

  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt);
  assertEqual(A, A2, 's');
}

{
  var A = Matrix([1.0, 2.0, 3.0],
                 [3.0, 2.0, 4.0],
                 [1.0, 2.0, 5.0], eltType=real(64));

  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt);
  assertEqual(A, A2, 's');
}

{
  var A = Matrix([1.0+1.0i, 2.0+1.0i, 3.0+1.0i],
                 [3.0-2.0i, 2.0+1.0i, 4.0+1.0i],
                 [1.0+1.0i, 2.0+1.0i, 5.0+1.0i], eltType=complex(64));

  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt);
  assertEqual(A, A2, 's');
}

{
  var A = Matrix([1.0+1.0i, 2.0+1.0i, 3.0+1.0i],
                 [3.0-2.0i, 2.0+1.0i, 4.0+1.0i],
                 [1.0+1.0i, 2.0+1.0i, 5.0+1.0i], eltType=complex(128));

  var (u, s, vt) = svd(A);

  var A2 = u.dot(diag(s)).dot(vt);
  assertEqual(A, A2, 's');
}
