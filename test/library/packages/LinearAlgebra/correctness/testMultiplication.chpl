use LinearAlgebra;
use TestUtils;

/* LinearAlgebra correctness tests for multiplication routines that depend on
   BLAS

   Any output denotes failure
*/

/* dot - calls matMult && inner*/

{
  proc test_dot(type t) {
    var M = Matrix([1,2,3],
                   [4,5,6],
                   [7,8,9],
                   eltType=t);
    var I = eye(M.domain, eltType=t);

    var v = Vector(1,2,3, eltType=t);

    // matrix-matrix
    var MI = dot(M, I);
    assertEqual(MI, M, "dot(M, I)");

    // vector-matrix && matrix-vector
    var Iv = dot(I, v);

    assertEqual(Iv, v, "dot(I, v)");

    var vI = dot(I, v);

    assertEqual(vI, v, "dot(v, I)");

    // vector-vector (inner-product)
    var vv = dot(v, v);
    var R = + reduce(v[..]*v[..]);
    assertEqual(vI, v, "dot(v, I)");


    var M34 = Matrix([1, 2, 3, 4],
                     [5, 6, 7, 8],
                     [9, 10,11,12],
                     eltType=t);
    var v4 = Vector([1,2,3,4], eltType=t);

    var v3 = Vector([1,2,3], eltType=t);

    // Sanity check
    assertEqual(M34.shape, (3, 4), 'M34.shape');
    assertEqual(v4.shape, (4,), 'v4.shape');
    assertEqual(v3.shape, (3,), 'v3.shape');

    assertEqual(dot(v3, M34).shape, (4,), 'dot(v3, M34).shape');
    assertEqual(dot(M34.T, v3).shape, (4,), 'dot(M34.T, v3).shape');

    assertEqual(dot(M34, v4).shape, (3,),'dot(M34, v4).shape');
    assertEqual(dot(v4, M34.T).shape, (3,), 'dot(v4, M34.T)');

    assertEqual(dot(M34, M34.T).shape, (3,3), 'dot(M34, M34.T)');
    assertEqual(dot(M34.T, M34).shape, (4,4), 'dot(M34.T, M34)');
  }

  test_dot(real);
  test_dot(real(32));
  test_dot(complex);
  test_dot(complex(128));
  test_dot(int(32));
  test_dot(int(64));

}

{
  var M = Matrix([1,2,3],
                 [4,5,6],
                 [7,8,9], eltType=int);
  var I = eye(M.domain, eltType=int);

  // matrix-matrix
  var MI = M.dot(I);
  assertEqual(MI, M, "M.dot(I)");
}

/* dot scalars */

{
  var I = eye(3, 3);
  var I2: [I.domain] real = 2*I;

  assertEqual(I2, dot(2, I), "dot(2, I)");
  assertEqual(I2, dot(I, 2), "dot(I, 2)");

  var v = Vector(3);
  var v2 = 2*v;

  assertEqual(v2, dot(2, v), "dot(2, v)");
  assertEqual(v2, dot(v, 2), "dot(v, 2)");
}


/* dot - Nx1 , 1xN vectors */
{
  var v31 = Matrix(3, 1);
  var v13 = Matrix(1, 3);
  var M = Matrix(3, 3);
  var S = Matrix(1, 1);

  v31 = 1;
  v13 = 1;
  M = 1;

  S[0, 0] =  inner(v13[0, ..], v31[.., 0]);

  // outer-product
  assertEqual(dot(v31, v13), M, "dot(Matrix(3, 1), Matrix(1, 3))");
  // inner-product
  assertEqual(dot(v13, v31), S, "dot(Matrix(1, 3), Matrix(3, 1))");
}

/* outer */
{
  var v = Vector(3);
  var M = Matrix(3, 3);

  v = 1;
  M = 1;

  assertEqual(outer(v, v), M, "outer(Vector(3), Vector(3))");
}


/* matPow matrix-scalar */
{
  const v1 = [1,2,3];
  var A = Matrix(v1,v1,v1, eltType=real);

  const v2 = [6, 12, 18];
  const A2 = Matrix(v2,v2,v2, eltType=real);

  const v3 = [36, 72, 108];
  const A3 = Matrix(v3,v3,v3, eltType=real);

  var B2 = matPow(A, 2);
  var B3 = matPow(A, 3);

  assertEqual(A2, B2, "matPow(A, 2)");
  assertEqual(A3, B3, "matPow(A, 3)");

  var A0 = matPow(A, 0);
  var A1 = matPow(A, 1);
  var I3 = eye(3, eltType=real);

  assertEqual(A0, I3, "matPow(A, 0)");
  assertEqual(A1, A,  "matPow(A, 1)");
}
