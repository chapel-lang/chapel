use LinearAlgebra;
use TestUtils;
use IO;

/* LinearAlgebra correctness tests that do not depend on BLAS/LAPACK

   Any output denotes failure
*/

//
// Initializers
//

{
  const Dom = {1..10},
        MDom = {1..3, 1..3},
        MDom2 = {1..4, 1..6};

  //
  // Vectors
  //

  /* Dimensions */
  {
    var v = Vector(10);
    assertEqual(v.domain, Dom, "Vector(10)");
  }

  /* Range */
  {
    var v = Vector(1..10);
    assertEqual(v.domain, Dom, "Vector(1..10)");
  }
  {
    // 0-based
    var v = Vector(0..#10);
    assertEqual(v.domain, {0..#10}, "Vector(0..#10)");
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

  /* Elements */
  {
    const v1 = Vector(1);
    assertEqual(v1, [0.0], 'Vector(1)');
    const v12 = Vector(1, 2);
    assertEqual(v12, [1, 2], 'Vector(1, 2)');
    const v123 = Vector(1, 2, 3);
    assertEqual(v123, [1, 2, 3], 'Vector(1, 2, 3)');
  }

  //
  // Matrices
  //


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

  /* Range */
  {
    var M = Matrix(1..3);
    assertEqual(M.domain, MDom, "Matrix(1..3)");
  }

  /* Ranges */
  {
    var M = Matrix(1..4, 1..6);
    assertEqual(M.domain, MDom2, "Matrix(1..4, 1..6)");
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

  /* CSR Sparse array -> Dense array */
  {
    use LayoutCS;
    var spsD: sparse subdomain(MDom) dmapped CS(sortedIndices=false);
    var spsA: [spsD] real;
    spsD += (1,1);
    spsD += (2,2);
    spsA[1,1] = 1;
    spsA[2,2] = 2;
    var M = Matrix(spsA);
    assertEqual(M.domain, MDom, "Matrix(spsA)");
    assertEqual(spsA[1,1], M[1,1], "Matrix(spsA)");
    assertEqual(spsA[2,2], M[2,2], "Matrix(spsA)");
  }

  /* Sparse array -> Dense array */
  {
    var spsD: sparse subdomain(MDom);
    var spsA: [spsD] real;
    spsD += (1,1);
    spsD += (2,2);
    spsA[1,1] = 1;
    spsA[2,2] = 2;
    var M = Matrix(spsA);
    assertEqual(M.domain, MDom, "Matrix(spsA)");
    assertEqual(spsA[1,1], M[1,1], "Matrix(spsA)");
    assertEqual(spsA[2,2], M[2,2], "Matrix(spsA)");
  }

  /* Variadic arguments of vectors */
  {
    var M = Matrix([1,2,3],
                   [4,5,6],
                   [7,8,9]);
    assertEqual(M.domain, MDom, "Matrix([1,2,3], [4,5,6], [7,8,9]).domain");
    assertEqual(M[2,2], 5, "Matrix([1,2,3], [4,5,6], [7,8,9])[1,1]");
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
    for i in MDom.dim(0) do Identity[i, i] = 1.0;

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

  S[1, 1] =  inner(v13[1, ..], v31[.., 1]);

  // outer-product
  assertEqual(dot(v31, v13), M, "dot(Matrix(3, 1), Matrix(1, 3))");
  // inner-product
  assertEqual(dot(v13, v31), S, "dot(Matrix(1, 3), Matrix(3, 1))");
}

/* dot - rank-change slice */
{
  var M = Matrix(3, 3);
  M = 1;

  var slice = M[1..3, 1];

  var result = dot(slice, slice);
  assertEqual(result, 3, 'dot(M[1..3, 1], M[1..3, 1])');
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
  assertEqual(Mtrace, t, "trace(M)");
}

/* Rectangular trace */
{
  var Mrect = Matrix([1,2,3,4],
                 [4,5,6,7],
                 [7,8,9,8],
                 eltType=real);
  var Mtrace = 15.0;
  var t = trace(Mrect);
  assertEqual(Mtrace, t, 'trace(Mrect)');
}


/* diag */
{
  var M = Matrix([1,2,3],
                 [4,5,6],
                 [7,8,9],
                 eltType=real);
  var v = Vector(1,5,9);
  var vMat = Matrix([1,0,0],
                    [0,5,0],
                    [0,0,9],
                    eltType=real);
  var M1 = Matrix([1,2,3,4],
                  [5,6,7,8],
                  [9,0,1,2],
                  eltType=real);
  var v11 = Vector(1,6,1);
  var v12 = Vector(3,8);
  var v13 = Vector([9]);

  assertEqual(v,    diag(M),        "diag(M)");
  assertEqual(vMat, diag(v),        "diag(v)");
  assertEqual(v11,  diag(M1),       "diag(M1)");
  assertEqual(v12,  diag(M1,2),     "diag(M1,2)");
  assertEqual(v13,  diag(M1,-2),    "diag(M1,-2)");

  // Now let's try with offsets

  ref rM = M.reindex(1..3, 1..3);
  ref rv = v.reindex(1..3);
  ref rvMat = vMat.reindex(1..3, 1..3);
  ref rM1 = M1.reindex(1..3, 1..4);
  ref rv11 = v11.reindex(1..3);
  ref rv12 = v12.reindex(1..2);
  ref rv13 = v13.reindex(1..1);

  assertEqual(rv,   diag(rM),       "diag(M)");
  assertEqual(rvMat,diag(rv),       "diag(v)");
  assertEqual(rv11, diag(rM1),      "diag(M1)");
  assertEqual(rv12, diag(rM1,2),    "diag(M1,2)");
  assertEqual(rv13, diag(rM1,-2),   "diag(M1,-2)");
}

/* kron */
{
  var M1 = Matrix([1, 2, 3],
                  [4, 5, 6],
                  [7, 8, 9]);

  var M2 = Matrix([9, 8, 7],
                  [6, 5, 4],
                  [3, 2, 1]);

  var M3 = Matrix([9, 8, 7, 18, 16, 14, 27, 24, 21],
                  [6, 5, 4, 12, 10, 8, 18, 15, 12],
                  [3, 2, 1, 6, 4, 2, 9, 6, 3],
                  [36, 32, 28, 45, 40, 35, 54, 48, 42],
                  [24, 20, 16, 30, 25, 20, 36, 30, 24],
                  [12, 8, 4, 15, 10, 5, 18, 12, 6],
                  [63, 56, 49, 72, 64, 56, 81, 72, 63],
                  [42, 35, 28, 48, 40, 32, 54, 45, 36],
                  [21, 14, 7, 24, 16, 8, 27, 18, 9]);


  var I1 = eye(3, eltType=int(64));
  var I2 = eye(4, eltType=int(64));
  var I3 = eye(12, eltType=int(64));


  assertEqual(M3, kron(M1, M2), "kron(M1, M2)");
  assertEqual(I3, kron(I1, I2), "kron(I1, I2)");
}

/* tril & triu */
{
  var M = eye(5,5);
  M = 1;

  {
    var L = tril(M, -1);
    var U = triu(M);
    var LU = L.plus(U);
    assertEqual(M, LU, "tril(M, -1) + triu(M)");
    assertTrue(isTril(L, -1), "isTril(L, -1)");
    assertTrue(isTriu(U), "isTriu(U)");
    assertFalse(isTril(U, -1), "isTril(U, -1)");
    assertFalse(isTriu(L), "isTriu(L)");
  }
  {
    var L = tril(M);
    var U = triu(M, 1);
    var LU = L.plus(U);
    assertEqual(M, LU, "tril(M) + triu(M, 1)");
    assertTrue(isTril(L), "isTril(L)");
    assertTrue(isTriu(U, 1), "isTriu(U, 1)");
    assertFalse(isTril(U), "isTril(U)");
    assertFalse(isTriu(L, 1), "isTriu(L, 1)");
  }
  {
    var L = tril(M, 1);
    var U = triu(M, 2);
    var LU = L.plus(U);
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

  // rectangular
  var Irect = eye(3,5);
  assertTrue(isDiag(I), "isDiag(Irect)");

  var M = I;
  M[1,2] = 1;
  assertFalse(isDiag(M), "isDiag(M)");
}

/* isHermitian */
{
  var H = Matrix([2.0+0.0i, 2.0+1.0i, 4.0+0.0i],
                 [2.0-1.0i, 3.0+0.0i, 0.0+1.0i],
                 [4.0+0.0i, 0.0-1.0i, 1.0+0.0i],
                 eltType=complex);
  assertTrue(isHermitian(H), "isHermitian(H)");
  H[1,2] += -2i;
  assertFalse(isHermitian(H), "isHermitian(H')");

  // rectangular
  var Hrect = Matrix(3, 4, eltType=complex);
  assertFalse(isHermitian(Hrect), "isHermitian(Hrect)");

}

/* isSymmetric */
{
  var S = Matrix([2.0+0.0i, 2.0+1.0i, 4.0+0.0i],
                 [2.0+1.0i, 3.0+0.0i, 0.0+1.0i],
                 [4.0+0.0i, 0.0+1.0i, 1.0+0.0i],
                 eltType=complex);
  assertTrue(isSymmetric(S), "isSymmetric(S)");
  S[1, 2] += 2;
  assertFalse(isSymmetric(S), "isSymmetric(S')");
  S[1, 2] -= 2;
  S[2, 1] += 2;
  assertFalse(isSymmetric(S), "isSymmetric(S'')");
}


/* isSquare */
{
  var Square = Matrix(3, 3);
  var Rect = Matrix(4, 3);

  assertTrue(isSquare(Square), "isSquare(Square)");
  assertFalse(isSquare(Rect), "isSquare(Rect)");
}

//
// LinearAlgebra.Sparse
//

{
  use LinearAlgebra.Sparse;

  const parentDom = {1..3, 1..3},
        parentDom2 = {1..4, 1..6},
        tParentDom = {1..3, 1..5},
        tParentDomT = {1..5, 1..3};

  var   Dom: sparse subdomain(parentDom) dmapped CS(sortedIndices=false),
        Dom2: sparse subdomain(parentDom2) dmapped CS(sortedIndices=false),
        IDom: sparse subdomain (parentDom) dmapped CS(sortedIndices=false),
        tDom: sparse subdomain (tParentDom) dmapped CS(sortedIndices=false),
        tDomT: sparse subdomain (tParentDomT) dmapped CS(sortedIndices=false);


  // Identity sparse domain
  IDom += [(1,1), (2,2), (3,3)];
  tDom += [(1,1), (2,1), (3,1), (3,4), (3,5)];
  tDomT += [(1,1), (1,2), (1,3), (4,3), (5,3)];

  /* Rows */
  {
    var D = CSRDomain(3);
    assertEqual(D, Dom, "CSRDomain(3)");
  }

  /* Dimensions */
  {
    var D = CSRDomain(3, 3);
    assertEqual(D, Dom, "CSRDomain(3, 3)");
  }

  /* Range */
  {
    var D = CSRDomain(1..3);
    assertEqual(D, Dom, "CSRDomain(0..#3)");
  }

  /* Ranges */
  {
    var D = CSRDomain(1..4, 1..6);
    assertEqual(D, Dom2, "CSRDomain(1..4, 1..6)");
  }

  /* Domain - CSR */
  {
    var D = CSRDomain(Dom);
    assertEqual(D, Dom, "CSRDomain(Dom)");
  }

  /* Domain - Dense */
  {
    var D = CSRDomain(parentDom);
    assertEqual(D, Dom, "CSRDomain(parentDom)");
  }

  /* Array - Dense -> CSR */
  {
    var I = eye(3,3);
    var A = CSRMatrix(I);
    assertEqual(A.domain, IDom, "CSRMatrix(I)");
  }

  /* Array - CSR -> CSR */
  {
    var A: [Dom] real;
    var M = CSRMatrix(A);
    assertEqual(M.domain, Dom, "CSRDomain(A)");
  }

  /* Array - CSR real -> CSR int */
  {
    var A: [Dom] real;
    var M = CSRMatrix(A, eltType=int);
    assertEqual(M.domain, Dom, "CSRMatrix(A)");
    assertTrue(isIntType(M.eltType), "CSRMatrix(A, eltType=int)");
  }

  /* CSR Identity */
  {
    var A = eye(IDom);
    var B: [IDom] real = 1;

    assertEqual(A.domain, B.domain, 'LinearAlgebra.Sparse.eye(IDom) // domain');
    assertEqual(A, B, 'LinearAlgebra.Sparse.eye(IDom) // array');
  }

  //
  // Simple Ops
  //

  // plus, minus, times, elementDiv
  {
    // Dense
    var A = eye(5,3),
        B = eye(5,3),
        C = eye(5,3);

    B *= 3;

    // Ensure there are no zeros to avoid div-by-zero below
    C += 1;

    var ApB = A.plus(B),
        AmB = A.minus(B),
        AtB = A.times(B),
        AdC = A.elementDiv(C);

    // CSR

    var csrA = CSRMatrix(A),
        csrB = CSRMatrix(B),
        csrC = CSRMatrix(C);

    var csrApB = csrA.plus(csrB),
        csrAmB = csrA.minus(csrB),
        csrAtB = csrA.times(csrB),
        csrAdC = csrA.elementDiv(csrC);

    assertEqual(csrApB, CSRMatrix(ApB), "csrA.plus(csrB)");
    assertEqual(csrAmB, CSRMatrix(AmB), "csrA.minus(csrB)");
    assertEqual(csrAtB, CSRMatrix(AtB), "csrA.times(csrB)");
    assertEqual(Matrix(csrAdC), AdC, "csrA.elementDiv(csrC)");
  }

  //
  // dot
  //

  /* Helper: test A.dot(A.T) for CSR - expects A to be dense */
  proc test_CSRdot(A) {
    // Dense dot
    var AAT = A.dot(A.T);

    // Sparse dot
    var csrA = CSRMatrix(A);
    var csrAAT = csrA.dot(csrA.T);

    assertEqual(csrAAT, CSRMatrix(AAT), "csrA.dot(csrA.T)");
  }

  /* Helper: test A.dot(B) for CSR - expects A to be dense */
  proc test_CSRdot(A, B) {
    // Dense dot
    var AB = A.dot(B);

    // Sparse dot
    var csrA = CSRMatrix(A);
    var csrB = CSRMatrix(B);
    var csrAB = csrA.dot(csrB);

    assertEqual(csrAB, CSRMatrix(AB), "csrA.dot(csrB)");
  }

  /* dot - matrix-matrix (identity) */
  {
    var A: [Dom] real;
    var I: [IDom] real;
    var AI = dot(A, I);
    assertEqual(AI, A, "dot(A, I)");
  }


  /* dot - matrix-vector (identity) */
  {
    var Asps: [IDom] real = 1;
    var v: [1..3] real = 2;
    var Av: [1..3] real = 2;
    assertEqual(dot(Asps, v), Av, 'Asps.dot(v)');
  }

  /* dot - vector-matrix (identity) */
  {
    var Asps: [IDom] real = 1;
    var v: [1..3] real = 2;
    var Av: [1..3] real = 2;
    assertEqual(dot(v, Asps), Av, 'v.dot(Asps)');
  }

  /* dot - vector-matrix */
  {
    var A = Matrix([1,1,0],
                   [0,1,1]);
    var Asps = CSRMatrix(A);
    var v = Vector(2,3);
    const Av = Vector(2, 5, 3);
    assertEqual(v.dot(Asps), Av, 'v.dot(Asps)');
  }

  /* dot - matrix-vector */
  {
    var A = Matrix([1,1,0],
                   [0,1,1]);
    var Asps = CSRMatrix(A);
    var v = Vector(1,2,3);
    const Av = Vector(3, 5);
    assertEqual(Asps.dot(v), Av, 'Asps.dot(v)');
  }

  /* dot - matrix-scalar */
  {
    var A: [IDom] real = 1;
    var B = dot(A, 2);
    var C = dot(2, A);
    assertEqual(A.domain, B.domain, "dot(A, 2)");
    assertEqual(A.domain, C.domain, "dot(2, A)");
    assertEqual(B, C, "matrix-scalar");
    var A2: A.type = 2*A;
    assertEqual(A2, B, "dot(A, 2)");
    assertEqual(A2, C, "dot(2, A)");
  }


  /* dot - matrix-scalar */
  {
    var A: [IDom] real = 1;
    var B = A.dot(2);
    var C = dot(2, A);
    assertEqual(A.domain, B.domain, "A.dot(2)");
    assertEqual(A.domain, C.domain, "dot(2, A)");
    assertEqual(B, C, "matrix-scalar");
    var A2: A.type = 2*A;
    assertEqual(A2, B, "dot(A, 2)");
    assertEqual(A2, C, "dot(2, A)");
  }

  /* dot - Various matrix-matrix tests */
  {
    var A = eye(3,5);
    // Identity matrix (3x5)
    test_CSRdot(A);

    A[2,4] = 1.0;
    test_CSRdot(A);

    // Get some leading zeros in CSR data structure
    A[1,1] = 0.0;
    test_CSRdot(A);

    // Get some leading zeros in CSR data structure
    A[1,1] = 0.0;
    test_CSRdot(A);

    var B = Matrix(5, 4);
    B[.., 1] =  1.0;
    B[1, ..] =  1.0;
    test_CSRdot(A, B);

    // A bigger matrix
    var C = eye(100, 20);
    test_CSRdot(C);
  }

  /* transpose */
  {
    var A: [tDom] int = 1;
    var B = transpose(A);

    assertEqual(B.domain, tDomT, "transpose(A)");
    for i in A.domain.dim(0) {
      assertEqual(A[i,1], B[1, i], "transpose(A) values");
    }
  }

  /* _array.T */
  {
    var A: [tDom] int = 1;
    var B = A.T;

    assertEqual(B.domain, tDomT, "transpose(A)");
    for i in A.domain.dim(0) {
      assertEqual(A[i,1], B[1, i], "transpose(A) values");
    }
  }


  // matPow with sparse matrices
  {
    // Real domains
    var D = CSRDomain(3,3);
    for ii in 1..#3 do D += (ii,ii);

    var A = CSRMatrix(D, real);
    for ii in 1..#3 do A[ii,ii] = ii;
    var B = matPow(A, 3);
    for ii in 1..#3 do assertEqual(B[ii,ii],(ii**3),
                                   "Error in matPow with sparse matrices : real");
  }

  {
    // Int domains
    var D = CSRDomain(3,3);
    for ii in 1..#3 do D += (ii,ii);

    var A = CSRMatrix(D, int);
    for ii in 1..#3 do A[ii,ii] = ii;
    var B = matPow(A, 3);
    for ii in 1..#3 do assertEqual(B[ii,ii],ii**3,
                                   "Error in matPow with sparse matrices : int");
  }

  {
    // Preserve domains
    /*
    Sparse.dot() does not yet support offset domains
    const lo = 10;
    var D = CSRDomain({lo..#3,lo..#3});
    for ii in lo..#3 do D += (ii,ii);

    var A = CSRMatrix(D, real);
    for ii in lo..#3 do A[ii,ii] = ii-lo+1;
    var B = matPow(A, 3);
    */
    /*
    Domain preservation remains an open question here.
    for ii in lo..#3 do assertEqual(B[ii,ii],(ii-lo+1)**3,
                                   "Error in matPow with sparse matrices : non-standard domain");
     */
  }

  // Matrix Properties
  {
    // Create dense, COO, and CSR domains
    var domDense: domain(2) = {1..3, 1..3},
        domCOO: sparse subdomain(domDense),
        domCSR: sparse subdomain(domDense) dmapped CS();

    // Create dense, COO, and CSR matrices (arrays)
    var matDense: [domDense] complex,
        matCOO: [domCOO] complex,
        matCSR: [domCSR] complex;


    // Make matrices diagonal
    matDense[2,2] = 1.0;

    domCOO += (2,2);
    matCOO[2,2] = 1.0;

    domCSR += (2,2);
    matCSR[2,2] = 1.0;

    // Verify matrices are diagonal
    assertTrue(isDiag(matDense), 'isDiag(matDense)');
    assertTrue(isDiag(matCOO), 'isDiag(matCOO)');
    assertTrue(isDiag(matCSR), 'isDiag(matCSR)');


    // Make matrices non-symmetric and non-diagonal
    matDense[1,2] = 1.0i;

    domCOO += (1,2);
    matCOO[1,2] = 1.0i;

    domCSR += (1,2);
    matCSR[1,2] = 1.0i;

    // Verify matrices are not diagonal
    assertFalse(isDiag(matDense), 'isDiag(matDense)');
    assertFalse(isDiag(matCOO), 'isDiag(matCOO)');
    assertFalse(isDiag(matCSR), 'isDiag(matCSR)');

    // Verify matrices are not symmetric
    assertFalse(isSymmetric(matDense), 'isSymmetric(matDense)');
    assertFalse(isSymmetric(matCOO), 'isSymmetric(matCOO)');
    assertFalse(isSymmetric(matCSR), 'isSymmetric(matCSR)');

    // Verify matrices are not Hermitian
    assertFalse(isHermitian(matDense), 'isHermitian(matDense)');
    assertFalse(isHermitian(matCOO), 'isHermitian(matCOO)');
    assertFalse(isHermitian(matCSR), 'isHermitian(matCSR)');

    // Make matrices symmetric
    matDense[2,1] = 1.0i;

    domCOO += (2,1);
    matCOO[2,1] = 1.0i;

    domCSR += (2,1);
    matCSR[2,1] = 1.0i;

    // Verify matrices are symmetric
    assertTrue(isSymmetric(matDense), 'isSymmetric(matDense)');
    assertTrue(isSymmetric(matCOO), 'isSymmetric(matCOO)');
    assertTrue(isSymmetric(matCSR), 'isSymmetric(matCSR)');

    // Make matrices Hermitian
    matDense[2,1] = -1.0i;
    matCOO[2,1] = -1.0i;
    matCSR[2,1] = -1.0i;

    // Verify matrices are Hermitian
    assertTrue(isHermitian(matDense), 'isHermitian(matDense)');
    assertTrue(isHermitian(matCOO), 'isHermitian(matCOO)');
    assertTrue(isHermitian(matCSR), 'isHermitian(matCSR)');

  } // Matrix Properties
} // LinearAlgebra.Sparse
