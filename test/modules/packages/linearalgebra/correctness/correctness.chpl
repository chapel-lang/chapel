use LinearAlgebra;

/* LinearAlgebra correctness tests */

/* Nothing prints if tests are successful */

//
// Initializers
//

{
  const Dom = {0..#10};
  const MDom = {0..#3, 0..#3};

  /* Dimensions */
  {
    var v = Vector(10);
    assertEqual(v.domain, Dom, "Vector(10)");
  }

  /* Domain */
  {
    var v = Vector(Dom);
    assertEqual(v.domain, Dom, "Vector(Dom)");
  }

  /* Array */
  {
    var A: [Dom] real;
    var v = Vector(A);
    assertEqual(v.domain, Dom, "Vector(A)");
  }

  /* Dimensions */
  {
    var M = Matrix(3, 3);
    assertEqual(M.domain, MDom, "Matrix(3, 3)");
  }

  /* Domain */
  {
    var M = Matrix(MDom);
    assertEqual(M.domain, MDom, "Matrix(MDom)");
  }

  /* Array */
  {
    var A: [MDom] real;
    var M = Matrix(A);
    assertEqual(M.domain, MDom, "Matrix(A)");
  }

  /* Variadic arguments of vectors */
  {
    var M = Matrix([1,2,3],
                   [4,5,6],
                   [7,8,9]);
    assertEqual(M.domain, MDom, "Matrix([1,2,3], [4,5,6], [7,8,9]).domain");
    assertEqual(M[1,1], 5, "Matrix([1,2,3], [4,5,6], [7,8,9])[1,1]");
  }

  /* Identity - Dimensions */
  {
    proc checkIdentity(m, n, type t = real) {
      var Identity: [0..#m, 0..#n] t;
      const count = min(m, n);
      for i in 0..#count {
        Identity[i, i] = 1: t;
      }

      var I = eye(m, n, eltType=t);
      assertEqual(I, Identity,
          "checkIdentity(%i, %i, %s)".format(m, n, t:string));
    }

    checkIdentity(3, 3);
    checkIdentity(3, 4);
    checkIdentity(4, 3);
    checkIdentity(3, 3, complex);
  }

  {
    var Identity: [MDom] real;
    for i in MDom.dim(1) do Identity[i, i] = 1.0;

    var I = eye(MDom);
    assertEqual(I, Identity, "eye(D)");
  }
}

//
// Matrix operations
//


{
  /* transpose - vector */
  {
    var v = Vector([1,2,3]);
    var vT = transpose(v);

    assertEqual(v, vT, "transpose(v)");
    assertEqual(v.shape, vT.shape, "transpose(v).shape");
    assertEqual(v.T, vT, "v.T");
  }

  /* transpose - square matrix */
  {

    var A =  Matrix([1.0,2.0,3.0],
                    [4.0,5.0,6.0],
                    [7.0,8.0,9.0]);
    var AT = Matrix([1.0,4.0,7.0],
                    [2.0,5.0,8.0],
                    [3.0,6.0,9.0]);

    assertEqual(transpose(A).shape, AT.shape, "transpose(A).shape");
    assertEqual(transpose(A), AT, "transpose(A)");
    assertEqual(A.T, AT, "A.T");
  }

  /* transpose - non-square matrix */
  {
    var A =  Matrix([1,2,3],
                    [4,5,6]);

    var AT = Matrix([1,4],
                    [2,5],
                    [3,6]);

    assertEqual(transpose(A).shape, AT.shape, "transpose(A).shape");
    assertEqual(transpose(A), AT, "transpose(A)");
    assertEqual(A.T, AT, "A.T");
  }
}

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
  }

  test_dot(real);
  test_dot(real(32));
  test_dot(complex);
  test_dot(complex(128));
  test_dot(int(32));
  test_dot(int(64));
}

/* outer */
{
  // TODO
}

/* matPow scalar-matrix */
{
  var A = Matrix([1,2,3,4],[4,3,2,1], eltType=real);

  var B = matPow(2, A);
  var R: [A.domain] real = 2**A;

  assertEqual(B, R, "matPow(2, A)");
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
}

/* matSqrt */

/* matExp */
{
  var A = Matrix([1,2,3,4],[4,3,2,1], eltType=real);

  var B = matExp(A);
  var R: [A.domain] real = e**A;

  assertEqual(B, R, "matExp(2, A)");
}

/* cross */
{
  // TODO
}

/* trace */
{
  var M = Matrix([1,2,3],
                 [4,5,6],
                 [7,8,9],
                 eltType=real);
  var Mtrace = 15.0;
  var t = trace(M);
  assertEqual(Mtrace, t);

  var v = Vector([1,2,3], eltType=real);
  var vtrace = 6.0;
  t = trace(v);
  assertEqual(vtrace, t);
}

/* tril & triu */
{
  var M = eye(5,5);
  M = 1;

  {
    var L = tril(M, -1);
    var U = triu(M);
    var LU = matPlus(L, U);
    assertEqual(M, LU, "tril(M, -1) + triu(M)");
  }
  {
    var L = tril(M);
    var U = triu(M, 1);
    var LU = matPlus(L, U);
    assertEqual(M, LU, "tril(M) + triu(M, 1)");
  }
  {
    var L = tril(M, 1);
    var U = triu(M, 2);
    var LU = matPlus(L, U);
    assertEqual(M, LU, "tril(M, 1) + triu(M, 2)");
  }
}

//
// Helpers
//

proc assertEqual(X, Y, msg="") {
  if X != Y {
    writeln("Test Failed: ", msg);
    writeln(X, ' != ', Y);
  }
}

proc assertEqual(X: [], Y: [], msg="") {
  if X.shape != Y.shape {
    writeln("Test Failed: ", msg);
    writeln(X, '\n!=\n', Y);
    return;
  } else if !X.equals(Y) {
    writeln("Test Failed: ", msg);
    writeln(X, '\n!=\n', Y);
    return;
  }
}

