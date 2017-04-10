use LinearAlgebra;

/* LinearAlgebra correctness tests

   Any output denotes failure

   Many of these tests are trivial and can be expanded upon in the future.
*/

config const verbose=false;

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


  /* Rows */
  {
    var M = Matrix(3);
    assertEqual(M.domain, MDom, "Matrix(3)");
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
  {
    var A: [MDom] real;
    var M = Matrix(A, eltType=int);
    assertEqual(M.domain, MDom, "Matrix(A)");
    assertTrue(isIntType(M.eltType), "Matrix(A, eltType=int)");
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

  /* transpose - Nx1 , 1xN vectors */
  {
    var v31 = Matrix(3, 1);
    var v13 = Matrix(1, 3);

    v31 = 1;
    v13 = 1;

    assertEqual(v13, v31.T, "Matrix(3, 1).T");
    assertEqual(v31, v13.T, "Matrix(1, 3).T");
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
}


/* cross */
{
  var v1 = Vector(1,2,3);
  var v2 = Vector(4,5,6);
  var C = Vector(-3, 6, -3);
  var C12 = cross(v1, v2);
  var C21 = cross(v2, v1);

  assertEqual(C, C12, "cross(v1, v2)");
  C = -C;
  assertEqual(C, C21, "cross(v2, v1)");
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
    assertTrue(isTril(L, -1), "isTril(L, -1)");
    assertTrue(isTriu(U), "isTriu(U)");
    assertFalse(isTril(U, -1), "isTril(U, -1)");
    assertFalse(isTriu(L), "isTriu(L)");
  }
  {
    var L = tril(M);
    var U = triu(M, 1);
    var LU = matPlus(L, U);
    assertEqual(M, LU, "tril(M) + triu(M, 1)");
    assertTrue(isTril(L), "isTril(L)");
    assertTrue(isTriu(U, 1), "isTriu(U, 1)");
    assertFalse(isTril(U), "isTril(U)");
    assertFalse(isTriu(L, 1), "isTriu(L, 1)");
  }
  {
    var L = tril(M, 1);
    var U = triu(M, 2);
    var LU = matPlus(L, U);
    assertEqual(M, LU, "tril(M, 1) + triu(M, 2)");
    assertTrue(isTril(L, 1), "isTril(L, 1)");
    assertTrue(isTriu(U, 2), "isTriu(U, 2)");
    assertFalse(isTril(U, 1), "isTril(U, 1)");
    assertFalse(isTriu(L, 2), "isTriu(L, 2)");
  }
}

/* isDiag */
{
  var I = eye(3,3);
  assertTrue(isDiag(I), "isDiag(I)");

  var M = I;
  M[0,2] = 1;
  assertFalse(isDiag(M), "isDiag(M)");
}

/* isHermitian */
{
  var H = Matrix([2.0+0.0i, 2.0+1.0i, 4.0+0.0i],
                 [2.0-1.0i, 3.0+0.0i, 0.0+1.0i],
                 [4.0+0.0i, 0.0-1.0i, 1.0+0.0i],
                 eltType=complex);
  assertTrue(isHermitian(H), "isHermitian(H)");
  H[0,1] += -2i;
  assertFalse(isHermitian(H), "isHermitian(H')");
}

/* isSymmetric */
{
  var S = Matrix([2.0+0.0i, 2.0+1.0i, 4.0+0.0i],
                 [2.0+1.0i, 3.0+0.0i, 0.0+1.0i],
                 [4.0+0.0i, 0.0+1.0i, 1.0+0.0i],
                 eltType=complex);
  assertTrue(isSymmetric(S), "isSymmetric(S)");
  S[0, 1] += 2;
  assertFalse(isSymmetric(S), "isSymmetric(S')");
}


/* isSquare */
{
  var Square = Matrix(3, 3);
  var Rect = Matrix(4, 3);

  assertTrue(isSquare(Square), "isSquare(Square)");
  assertFalse(isSquare(Rect), "isSquare(Rect)");
}


//
// Helpers
//

proc assertEqual(X, Y, msg="") {
  if verbose then writeln(msg);
  if X != Y {
    writeln("Test Failed: ", msg);
    writeln(X, ' != ', Y);
  }
}


proc assertEqual(X: [], Y: [], msg="") where isArrayValue(X) && isArrayValue(Y)
{
  if verbose then writeln(msg);
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


proc assertEqual(X: _tuple, Y: _tuple, msg="") {
  if verbose then writeln(msg);
  if X.size != Y.size {
    writeln("Test Failed: ", msg);
    writeln(X, '\n!=\n', Y);
    return;
  } else {
    for (x, y) in zip(X, Y) {
      if x != y {
        writeln("Test Failed: ", msg);
        writeln(X, '\n!=\n', Y);
        return;
      }
    }
  }
}

proc assertTrue(x: bool, msg="") {
  if verbose then writeln(msg);
  if !x {
    writeln("Test Failed: ", msg);
    writeln("boolean is false");
  }
}

proc assertFalse(x: bool, msg="") {
  if verbose then writeln(msg);
  if x {
    writeln("Test Failed: ", msg);
    writeln("boolean is true");
  }
}
