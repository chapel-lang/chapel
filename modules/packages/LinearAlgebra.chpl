/*
 * Copyright 2004-2017 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* TODO
    - implementations
    - testing
    - documentation
      - document types
    - performance testing
*/

/* Linear Algebra Module

A high-level interface to linear algebra operations and procedures.

.. note::
  This is

Compiling with Linear Algebra
-----------------------------

The linear algebra module uses both `BLAS` and `LAPACK` for
many routines. In order to compile a Chapel program with this module, be sure
to have BLAS and LAPACK implementations available on your system. See the BLAS
and LAPACK documentation for further details.

.. comment:
  - native implementations some day

Linear Algebra API
------------------

**Matrix and Vector representation**

Matrices and vectors are represented as Chapel arrays, which means the
convenience constructors provided in this module are not necessary to use this
module's functions. This also means that matrices (2D arrays) and vectors (1D
arrays) will work with any other Chapel library works with arrays.

.. note::

  This documentation uses the terms `matrix` to refer to `2D arrays`, and
  `vector` to refer to `1D arrays`.

**Indexing**

All functions that return new arrays will return arrays over 0-based indices,
unless otherwise specified.

**Matrix multiplication**

``dot()`` is the general matrix multiplication function provided in this module.
This function supports any combination of scalars, vectors (1D arrays), and
matrices (2D arrays). See the :proc:`dot` documentation for more information.

If you find this convenient notation confusing, you can opt to calling the
underlying functions: :proc:`matMult` and `inner`.

The :proc:`dot` function, along with others may be given a matrix-specific
operator in future releases.

.. comment:
  It is also less verbose than matrixMultiply, matMult, or mMult.

**Row vs Column vectors**

Row and column vectors are both represented as 1D arrays and are
indistinguishable in Chapel. In the :proc:`dot` function, matrix-vector
multiplication assumes a column vector, vector-matrix multiplcation assumes a
row vector, vector-vector multiplication is always treated as an inner-product,
as the function name implies.
An outer product can be computed with the :proc:`outer` function.

**Promotion flattening**

``TODO``

Discuss promotion flattening, and how to get around it, providing examples
and helper functions like :proc:`matPlus`.

.. note::
    Promotion flattening is not an intended feature, and should be resolved in
    future releases of Chapel.

.. Examples:

.. Features:

.. Future directions:
  - Domain maps
    - Distributed array support
      - PBLAS
    - Sparse support
      - suiteSparse
    - GPU support
      - CuBLAS
  - Support fully native implementations
    - Only use the BLAS/LAPACK version when the libraries are installed
  - Provide MKL BLAS/LAPACK with Chapel installation
    - install with something like: CHPL_EXTRAS=MKL

.. Reviewer Notes:
 - naming scheme OK?
 - Should we support the promotion-helpers?
    - matPlus, matMinus
 - Should we provide matMult, or just dot?
  - dot as a method?

.. LinearAlgebra Module TODOs
  - Support non-matching domains and eltTypes in matOperations
    - check that eltTypes are coercible
    - check domain shapes are equal
*/

module LinearAlgebra {

 use Norm; // TODO -- merge Norm into LinearAlgebra
 use BLAS;
// use LAPACK; // TODO -- Use LAPACK routines


//
// Matrix and Vector Initializers
//

/* Return a vector (1D array) over domain ``{0..#length}``*/
proc Vector(length, type eltType=real) {
  var V: [0..#length] eltType;
  return V;
}


/* Return a vector (1D array) over domain ``Dom`` */
proc Vector(Dom: domain(1), type eltType=real) {
  var V: [Dom] eltType;
  return V;
}


/* Return a vector (1D array) with domain and values of ``A`` */
proc Vector(A: [?Dom] ?Atype, type eltType=Atype ) {
  var V: [Dom] eltType = A;
  return V;
}


pragma "no doc"
proc Vector(x: ?t, Scalars...?n)  where isNumericType(t) {
  type eltType = Scalars(1).type;
  return Vector((...Scalars), eltType=eltType);
}


/* Return a vector (1D array), given 2 or more numeric values

   If `type` is omitted, it will be inferred from the first argument
*/
proc Vector(x: ?t, Scalars...?n, type eltType) where isNumericType(t) {

  if !isNumeric(Scalars(1)) then
      compilerError("Vector() expected numeric arguments");

  var V: [0..n] eltType;

  V[0] = x: eltType;

  for i in 1..n do V[i] = Scalars[i]: eltType;

  return V;
}


/* Return a matrix (2D array) over domain ``{0..#rows, 0..#cols}``*/
proc Matrix(rows, cols, type eltType=real) where isIntegral(rows) && isIntegral(cols) {
  var M: [0..#rows, 0..#cols] eltType;
  return M;
}


/* Return a matrix (2D array) over domain ``Dom`` */
proc Matrix(Dom, type eltType=real) where Dom.rank == 2 {
  var M: [Dom] eltType;
  return M;
}


/* Return a matrix (2D array) with domain and values of ``A`` */
proc Matrix(A: [?Dom] ?eltType) where Dom.rank == 2 {
  var M: [Dom] eltType = A;
  return M;
}


pragma "no doc"
proc Matrix(Arrays...?n) {
  type eltType = Arrays(1).eltType;
  return Matrix((...Arrays), eltType=eltType);
}


/*
    Return a matrix (2D array), given 2 or more vectors. Vectors are
    concatenated such that the ``ith`` vector corresponds to the matrix slice:
    ``A[i, ..]``

    If `type` is omitted, it will be inferred from the first argument
*/
proc Matrix(Arrays...?n, type eltType) {
  // TODO -- assert all array domains are same length
  //         Can this be done via type query?

  if Arrays(1).domain.rank != 1 then compilerError("Matrix() expected 1D arrays");

  const dim2 = 0..#Arrays(1).domain.dim(1).length,
        dim1 = 0..#n;

  var M: [{dim1, dim2}] eltType;

  for i in dim1 do
    M[i, ..] = Arrays(i+1)[..]: eltType;

  return M;
}


/* Return an identity matrix over domain ``{0..#m, 0..#n}`` */
proc eye(m, n, type eltType=real) {
  var A: [{0..#m, 0..#n}] eltType;
  const diagonal = min(m, n);
  for i in 0..#diagonal do A[i, i] = 1: eltType;
  return A;
}


/* Return an identity matrix over domain ``Dom`` */
proc eye(Dom: domain(2), type eltType=real) {
  var A: [Dom] eltType;
  const diagonal = min(Dom.shape(1), Dom.shape(2));
  return eye(Dom.shape(1), Dom.shape(2), eltType=eltType);
}


//
// Matrix Operations
//

pragma "no doc"
/* Transpose vector (no-op) */
inline proc transpose(A: [?Dom]) where Dom.rank == 1 {
  return A;
}


pragma "no doc"
/* Syntactic sugar for transpose(Vector) */
proc _array.T where this.domain.rank == 1 { return transpose(this); }


/* Transpose vector or matrix.

   .. note::

      Since row vectors and columns vectors are indistinguishable, passing
      a vector to this function will return that vector unchanged

*/
proc transpose(A: [?Dom] ?eltType) where Dom.rank == 2 {
  if isIntegralType(eltType) then return _transpose(A);
  else {
    var B: [Dom] eltType = eye(Dom, eltType=eltType);
    var C: [{Dom.dim(2), Dom.dim(1)}] eltType;
    // TODO -- more efficient algorithm (BLAS not necessary)
    gemm(A, B, C, 1:eltType, 0:eltType, opA=Op.T);
    return C;
  }
}


/* Transpose vector or matrix */
proc _array.T where this.domain.rank == 2 { return transpose(this); }


/* Transpose matrix with native implementation */
pragma "no doc"
private proc _transpose(A: [?Dom] ?eltType) {
  var C: [{Dom.dim(2), Dom.dim(1)}] eltType;

  // naive algorithm
  forall (i, j) in Dom do
    C[j, i] = A[i, j];

  return C;
}


/* Add matrices, maintaining dimensions */
proc matPlus(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.size != Bdom.size then halt('Unmatched array size');
  var C: [Adom] eltType = A + B;
  return C;
}


/* Subtract matrices, maintaining dimensions */
proc matMinus(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.size != Bdom.size then halt('Unmatched array size');
  var C: [Adom] eltType = A - B;
  return C;
}

/*
    Generic matrix multiplication, ``A`` and ``B`` can be a matrix, vector, or
    scalar
*/
proc dot(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  // vector-vector
  if Adom.rank == 1 && Bdom.rank == 1 then
    return inner(A, B);
  // matrix-(vector|matrix)
  else
    return matMult(A, B);
}


pragma "no doc"
/* Element-wise scalar multiplication */
proc dot(A: [?Adom] ?eltType, b) where isNumeric(b) {
  var C: [Adom] eltType = A*b;
}


pragma "no doc"
/* Element-wise scalar multiplication */
proc dot(a, B: []) where isNumeric(a) {
  return dot(B, a);
}


pragma "no doc"
/* Explicit matrix-(matrix|vector) multiplication */
proc matMult(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  // matrix-vector
  if Adom.rank == 2 && Bdom.rank == 1 then
    // TODO -- assert shapes are correct
    return _matvecMult(A, B);
  // vector-matrix
  else if Adom.rank == 1 && Bdom.rank == 2 then
    // TODO -- assert shapes are correct
    return _matvecMult(B, A);
  // matrix-matrix
  else if Adom.rank == 2 && Bdom.rank == 2 then
    // TODO -- assert shapes are correct
    return _matmatMult(A, B);
  else
    compilerError("Rank sizes are not 1 or 2");
}


pragma "no doc"
/* matrix-vector multiplication */
proc _matvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType)
  where isBLASType(eltType)
{
  if Adom.rank != 2 || Xdom.rank != 1 then
    compilerError("Rank sizes are not 2 and 1");

  var Y: [Xdom] eltType;
  gemv(A, X, Y, 1:eltType, 0:eltType);
  return Y;
}


pragma "no doc"
/* matrix-matrix multiplication */
proc _matmatMult(A: [?Adom] ?eltType, B: [?Bdom] eltType)
  where isBLASType(eltType)
{
  if Adom.rank != 2 || Bdom.rank != 2 then
    compilerError("Rank sizes are not 2");

  var C: [Adom.dim(1), Bdom.dim(2)] eltType;
  gemm(A, B, C, 1:eltType, 0:eltType);
  return C;
}


/* Inner product of 2 vectors */
proc inner(A: [?Adom], B: [?Bdom]) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Rank sizes are not 1");

  return + reduce(A[..], B[..]);
}


/* Outer product of 2 vectors */
proc outer(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Rank sizes are not 1");

  var C: eltType [Adom.dim(1), Bdom.dim(2)];
  forall (i,j) in C.domain do
    C[i, j] = A[i]*B[j];
}


pragma "no doc"
/* Generic matrix-vector multiplication */
proc _matvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType)
  where !isBLASType(eltType)
{
  if Adom.rank != 2 || Xdom.rank != 1 then
    compilerError("Rank sizes are not 2 and 1");

  var C: [Xdom] eltType;

  // naive algorithm
  forall i in Xdom do
    C[i] = + reduce (A[i,..]*X[..]);

  return C;
}

pragma "no doc"
/* Generic matrix-matrix multiplication */
proc _matmatMult(A: [?Adom] ?eltType, B: [?Bdom] eltType)
  where !isBLASType(eltType)
{
  if Adom.rank != 2 || Bdom.rank != 2 then
    compilerError("Rank sizes are not 2 and 1");

  var C: [Adom.dim(1), Bdom.dim(2)] eltType;

  // naive algorithm
  forall (i,j) in C.domain do
    C[i,j] = + reduce (A[i,..]*B[..,j]);

  return C;
}


/* Return the matrix ``A`` to the ``bth`` power, where ``b`` is a positive
   integral type. */
proc matPow(A: [], b) where isNumeric(b) {
  if !isIntegral(b) then
    // TODO -- support all reals with Sylvester's formula
    compilerError("matPow only support powers of integers");
  if !isSquare(A) then
    halt("Array not square");

  return _expBySquaring(A, b);
}


pragma "no doc"
/* Exponentiate by squaring recursively */
private proc _expBySquaring(x: ?t, n): t {
    // TODO -- _expBySquaring(pinv(x), -n);
    if n < 0  then halt("Negative powers not yet supported");
    else if n == 0  then return eye(x.domain);
    else if n == 1  then return x;
    else if n%2 == 0  then return _expBySquaring(dot(x, x), n / 2);
    else return dot(x, _expBySquaring(dot(x, x), (n - 1) / 2));
}


/* Return element-wise power: ``a**B`` */
proc matPow(a, B: [?Bdom] ?eltType) where isNumeric(a) {
  var C: [Bdom] eltType = a**B;
  return C;
}


/* Return element-wise exponential: ``e**A[i,j]`` */
proc matExp(A: [?Dom] ?eltType) {
  var B: [Dom] eltType = e**A;
  return B;
}


pragma "no doc"
/* Integer overload */
proc matExp(A: [?Dom] ?eltType) where isIntegralType(eltType) {
  var B: [Dom] real = e**A;
  return B;
}


/* Return cross-product of vectors ``A`` and ``B`` with domains of size ``3`` */
proc cross(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Rank sizes are not 1");
  if Adom.size != 3 || Bdom.size != 3 then
    halt("cross() expects arrays of 3 elements");

  var cross = Vector(Adom.size, eltType);

  cross[0] = A[1]*B[2] - A[2]*B[1];
  cross[1] = A[2]*B[0] - A[0]*B[2];
  cross[2] = A[0]*B[1] - A[1]*B[0];

  return cross;
}


//
// Matrix Structure
//

/* Return lower triangular part of matrix, below the diagonal + ``k`` */
proc tril(A: [?D] ?eltType, k=0) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  var L = Matrix(A);
  forall (i, j) in D do
    if (i < j-k) then L[i, j] = 0: eltType;
  return L;
}


/* Return upper triangular part of matrix, above the diagonal + ``k`` */
proc triu(A: [?D] ?eltType, k=0) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  var U = Matrix(A);
  forall (i, j) in D do
    if (i > j-k) then U[i, j] = 0: eltType;
  return U;
}


/* Return `true` if matrix is diagonal */
proc isDiag(A: [?D] ?eltType) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");

  const zero = 0: eltType;
  for (i, j) in D do
    if i != j && D[i, j] != zero return false
  return true;
}


/* Return `true` if matrix is Hermitian */
proc isHermitian(A: [?D]) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  for (i, j) in D {
    if i > j {
      if D[i, j] != conjg(D[j, i]) then return false;
    }
  }
  return true;
}


/* Return `true` if matrix is symmetric */
proc isSymmetric(A: [?D]) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  for (i, j) in D {
    if i > j {
      if D[i, j] != D[j, i] then return false;
    }
  }
  return true;
}


/* Return `true` if matrix is lower triangular below the diagonal + ``k`` */
proc isTril(A: [?D], k=0) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  const zero = 0: eltType;
  for (i, j) in D do
    if (i < j-k) && (A[i, j] != zero) then
      return false;
  return true;
}


/* Return `true` if matrix is upper triangular */
proc isTriu(A: [?D] ?eltType, k=0) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  const zero = 0: eltType;
  for (i, j) in D do
    if (i > j-k) && (A[i, j] != zero) then
      return false;
  return true;
}


/* Return `true` if matrix is square */
proc isSquare(A: [?D]) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  const (M, N) = A.shape;
  return M == N;
}


/* Return the determinant of square matrix ``A`` */
proc det(A: [?D] ?eltType) {
  // Naive algorithm -
  //    Better solution uses LU decomp: det M = det LU = det L * det U
  if D.rank != 2 then
    compilerError("Rank size not 2");

  // Handle odd domains inefficiently:
  if D.stridable {
    var Atmp = Matrix(A);
    return det(Atmp);
  }
  if D.first != 0 || D.last != D.shape(1) {
    var Atmp = Matrix(A);
    return det(Atmp);
  }
  // Assume 0-based non-strided domain below this point

  if !isSquare(A) then
    halt('non-square array passed to determinant. Aborting');

  const n = D.shape(1);
  if n == 1 then return A[0];

  var d = 0: eltType;
  forall j in 0..#n with (+ reduce d) do
    d += (((-1)**(j))*(A[0,j])*det(_minor(A, j))): eltType;

  return d;
}


pragma "no doc"
/* Helper function for determinant */
private proc _minor(A: [?Adom] ?eltType, j) {
  const n = Adom.dim(1).size;
  var a : [0..#n-1, 0..#n-1] eltType;
  var K = 0,
      L = 0,
      i = 0;

  for k in 0..#n {
    for l in 0..#n  {
      // K
      if k == i || l == j then continue;
      if k < i then K = k;
      else K = k-1;
      // L
      if l < j then L = l;
      else L = l-1;
      // a[K, L]
      a[K,L] = A[k,l];
    }
  }
  return a;
}


/* Return the trace of ``A`` */
proc trace(A: [?D] ?eltType) {
  if D.rank == 1 then return + reduce A[..];
  else if D.rank == 2 {
    var trace = 0: eltType;
    forall (i, j) in zip(D.dim(1), D.dim(2)) with (+ reduce trace) do
      trace += A[i, j];
    return trace;
  } else {
    compilerError("Rank size not 1 or 2");
  }
}

} // module LinearAlgebra
