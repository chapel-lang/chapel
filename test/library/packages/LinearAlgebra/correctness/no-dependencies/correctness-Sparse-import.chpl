import LinearAlgebra;
use TestUtils;
use IO;

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
    var I = LinearAlgebra.eye(3,3);
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
    var A = LinearAlgebra.eye(5,3),
        B = LinearAlgebra.eye(5,3),
        C = LinearAlgebra.eye(5,3);

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
    assertEqual(LinearAlgebra.Matrix(csrAdC), AdC, "csrA.elementDiv(csrC)");
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
    var A = LinearAlgebra.Matrix([1,1,0],
                   [0,1,1]);
    var Asps = CSRMatrix(A);
    var v = LinearAlgebra.Vector(2,3);
    const Av = LinearAlgebra.Vector(2, 5, 3);
    assertEqual(v.dot(Asps), Av, 'v.dot(Asps)');
  }

  /* dot - matrix-vector */
  {
    var A = LinearAlgebra.Matrix([1,1,0],
                   [0,1,1]);
    var Asps = CSRMatrix(A);
    var v = LinearAlgebra.Vector(1,2,3);
    const Av = LinearAlgebra.Vector(3, 5);
    assertEqual(Asps.dot(v), Av, 'Asps.dot(v)');
  }

  /* dot - matrix-scalar */
  {
    var A: [IDom] real = 1;
    var B = LinearAlgebra.dot(A, 2);
    var C = LinearAlgebra.dot(2, A);
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
    var C = LinearAlgebra.dot(2, A);
    assertEqual(A.domain, B.domain, "A.dot(2)");
    assertEqual(A.domain, C.domain, "dot(2, A)");
    assertEqual(B, C, "matrix-scalar");
    var A2: A.type = 2*A;
    assertEqual(A2, B, "dot(A, 2)");
    assertEqual(A2, C, "dot(2, A)");
  }

  /* dot - Various matrix-matrix tests */
  {
    var A = LinearAlgebra.eye(3,5);
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

    var B = LinearAlgebra.Matrix(5, 4);
    B[.., 1] =  1.0;
    B[1, ..] =  1.0;
    test_CSRdot(A, B);

    // A bigger matrix
    var C = LinearAlgebra.eye(100, 20);
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
    var B = LinearAlgebra.matPow(A, 3);
    for ii in 1..#3 do assertEqual(B[ii,ii],(ii**3),
                                   "Error in matPow with sparse matrices : real");
  }

  {
    // Int domains
    var D = CSRDomain(3,3);
    for ii in 1..#3 do D += (ii,ii);

    var A = CSRMatrix(D, int);
    for ii in 1..#3 do A[ii,ii] = ii;
    var B = LinearAlgebra.matPow(A, 3);
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
    var B = LinearAlgebra.matPow(A, 3);
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
    assertTrue(LinearAlgebra.isDiag(matDense), 'isDiag(matDense)');
    assertTrue(isDiag(matCOO), 'isDiag(matCOO)');
    assertTrue(isDiag(matCSR), 'isDiag(matCSR)');


    // Make matrices non-symmetric and non-diagonal
    matDense[1,2] = 1.0i;

    domCOO += (1,2);
    matCOO[1,2] = 1.0i;

    domCSR += (1,2);
    matCSR[1,2] = 1.0i;

    // Verify matrices are not diagonal
    assertFalse(LinearAlgebra.isDiag(matDense), 'isDiag(matDense)');
    assertFalse(isDiag(matCOO), 'isDiag(matCOO)');
    assertFalse(isDiag(matCSR), 'isDiag(matCSR)');

    // Verify matrices are not symmetric
    assertFalse(LinearAlgebra.isSymmetric(matDense), 'isSymmetric(matDense)');
    assertFalse(isSymmetric(matCOO), 'isSymmetric(matCOO)');
    assertFalse(isSymmetric(matCSR), 'isSymmetric(matCSR)');

    // Verify matrices are not Hermitian
    assertFalse(LinearAlgebra.isHermitian(matDense), 'isHermitian(matDense)');
    assertFalse(isHermitian(matCOO), 'isHermitian(matCOO)');
    assertFalse(isHermitian(matCSR), 'isHermitian(matCSR)');

    // Make matrices symmetric
    matDense[2,1] = 1.0i;

    domCOO += (2,1);
    matCOO[2,1] = 1.0i;

    domCSR += (2,1);
    matCSR[2,1] = 1.0i;

    // Verify matrices are symmetric
    assertTrue(LinearAlgebra.isSymmetric(matDense), 'isSymmetric(matDense)');
    assertTrue(isSymmetric(matCOO), 'isSymmetric(matCOO)');
    assertTrue(isSymmetric(matCSR), 'isSymmetric(matCSR)');

    // Make matrices Hermitian
    matDense[2,1] = -1.0i;
    matCOO[2,1] = -1.0i;
    matCSR[2,1] = -1.0i;

    // Verify matrices are Hermitian
    assertTrue(LinearAlgebra.isHermitian(matDense), 'isHermitian(matDense)');
    assertTrue(isHermitian(matCOO), 'isHermitian(matCOO)');
    assertTrue(isHermitian(matCSR), 'isHermitian(matCSR)');

  } // Matrix Properties
} // LinearAlgebra.Sparse
