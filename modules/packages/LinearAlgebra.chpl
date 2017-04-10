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

/* Linear Algebra Module

A high-level interface to linear algebra operations and procedures.

.. note::
  This is an early prototype package module. As a result, interfaces may change
  over the next release.

Compiling with Linear Algebra
-----------------------------

The linear algebra module uses the :mod:`BLAS` module.
In order to compile a Chapel program with this module, be sure
to have a BLAS implementation available on your system. See the :mod:`BLAS`
documentation for further details.


Linear Algebra API
------------------

**Matrix and Vector representation**

Matrices and vectors are represented as Chapel arrays, which means the
convenience constructors provided in this module are not necessary to use this
module's functions. This also means that matrices (2D arrays) and vectors (1D
arrays) will work with any other Chapel library that works with arrays.

.. note::

  This documentation uses the terms `matrix` to refer to `2D arrays`, and
  `vector` to refer to `1D arrays`.

**Indexing**

All functions that return new arrays will return arrays over 0-based indices,
unless otherwise specified.

**Matrix multiplication**

:proc:`dot` is the general matrix multiplication function provided in this module.
This function supports any combination of scalars, vectors (1D arrays), and
matrices (2D arrays). See the :proc:`dot` documentation for more information.

The :proc:`dot` function, along with others may be given a matrix-specific
operator in future releases.

**Row vs Column vectors**

Row and column vectors are both represented as 1D arrays and are
indistinguishable in Chapel. In the :proc:`dot` function, matrix-vector
multiplication assumes a column vector, vector-matrix multiplication assumes a
row vector, vector-vector multiplication is always treated as an inner-product,
as the function name implies.
An outer product can be computed with the :proc:`outer` function.

**Promotion flattening**

Promotion flattening is an unintended consequence of Chapel's
:ref:`promotion feature <ug-promotion>`, when a multi-dimensional array assignment
gets type-inferred as a 1-dimensional array of the same size, effectively
flattening the array dimensionality. This can result in unexpected behavior in
programs such as this:

.. code-block:: chapel

  var A = Matrix(4, 4),
      B = Matrix(4, 4);
  // A + B is a promoted operation, and C becomes a 1D array
  var C = A + B;
  // This code would then result in an error due to rank mismatch:
  C += A;

To get avoid this, you can avoid relying on inferred-types for new arrays:

.. code-block:: chapel

  var A = Matrix(4, 4),
      B = Matrix(4, 4);
  // C's type is not inferred and promotion flattening is not a problem
  var C: [A.domain] A.eltType = A + B;
  // Works as expected
  C += A;

Alternatively, you can use the LinearAlgebra helper routines, which create
and return a new array:

.. code-block:: chapel

  var A = Matrix(4, 4),
      B = Matrix(4, 4);
  // matPlus will create a new array with an explicit type
  var C = matPlus(A, B);
  // Works as expected
  C += A;

Promotion flattening is not expected to be an issue in future releases.


.. LinearAlgebra Module Future TODOs:
  - Support non-matching domains and eltTypes in matOperations
    - check that eltTypes are coercible
    - check domain shapes are equal
  - Add LinearAlgebra primer
  - Add feature table, comparing to numpy and matlab
  - performance testing
  - Domain maps
    - Distributed array support
    - Sparse support
    - GPU support
  - Support fully native implementations
    - Only use the BLAS/LAPACK version when the libraries are installed
  - Provide MKL BLAS/LAPACK with Chapel installation
    - install with something like: CHPL_EXTRAS=MKL

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
  if (length <= 0) then halt("Vector length must be > 0");
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
  var V: [Dom] eltType = A: eltType;
  return V;
}


pragma "no doc"
proc Vector(x: ?t, Scalars...?n)  where isNumericType(t) {
  type eltType = Scalars(1).type;
  return Vector(x, (...Scalars), eltType=eltType);
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


/* Return a square matrix (2D array) over domain ``{0..#rows, 0..#rows}``*/
proc Matrix(rows, type eltType=real) where isIntegral(rows) {
  if rows <= 0 then halt("Matrix dimensions must be > 0");
  var M: [0..#rows, 0..#rows] eltType;
  return M;
}


/* Return a matrix (2D array) over domain ``{0..#rows, 0..#cols}``*/
proc Matrix(rows, cols, type eltType=real) where isIntegral(rows) && isIntegral(cols) {
  if rows <= 0 || cols <= 0 then halt("Matrix dimensions must be > 0");
  var M: [0..#rows, 0..#cols] eltType;
  return M;
}


/* Return a matrix (2D array) over domain ``Dom`` */
proc Matrix(Dom: domain(2), type eltType=real) where Dom.rank == 2 {
  var M: [Dom] eltType;
  return M;
}


/* Return a matrix (2D array) with domain and values of ``A`` */
proc Matrix(A: [?Dom] ?Atype, type eltType=Atype) where Dom.rank == 2 {
  var M: [Dom] eltType = A: eltType;
  return M;
}


pragma "no doc"
proc Matrix(Arrays...?n) {
  type eltType = Arrays(1).eltType;
  return Matrix((...Arrays), eltType=eltType);
}


/*
    Return a matrix (2D array), given 2 or more vectors, such that the vectors
    form the rows of the matrix. In other words, the vectors are
    concatenated such that the ``ith`` vector corresponds to the matrix slice:
    ``A[i, ..]``

    If `type` is omitted, it will be inferred from the first array.

    For example:

    .. code-block:: chapel

        var A = Matrix([1, 2, 3],
                       [4, 5, 6],
                       [7, 8, 9]);
        /* Produces the 3x3 matrix of integers:
             1 2 3
             4 5 6
             7 8 9
         */

*/
proc Matrix(const Arrays...?n, type eltType) {
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


/* Return a square identity matrix over domain ``{0..#m, 0..m}`` */
proc eye(m, type eltType=real) {
  var A: [{0..#m, 0..#m}] eltType;
  for i in 0..#m do A[i, i] = 1: eltType;
  return A;
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
inline proc transpose(D: domain(1)) {
  return D;
}

pragma "no doc"
inline proc transpose(D: domain(2)) {
  return {D.dim(2), D.dim(1)};
}


pragma "no doc"
/* Transpose vector (no-op) */
inline proc transpose(A: [?Dom]) where Dom.rank == 1 {
  return A;
}


pragma "no doc"
/* Syntactic sugar for transpose(Vector) */
proc _array.T where this.domain.rank == 1 { return transpose(this); }


/* Transpose vector, matrix, or domain.

   .. note::

      Since row vectors and columns vectors are indistinguishable, passing
      a vector to this function will return that vector unchanged

*/
proc transpose(A: [?Dom] ?eltType) where Dom.rank == 2 {
  if !isBLASType(eltType) then
    return _transpose(A);
  else if Dom.shape(1) == 1 then
    return reshape(A, transpose(Dom));
  else if Dom.shape(2) == 1 then
    return reshape(A, transpose(Dom));
  else {
    var B: [Dom] eltType = eye(Dom, eltType=eltType);
    var C: [transpose(Dom)] eltType;
    // TODO -- use native algorithm (BLAS not necessary or memory-efficient)
    gemm(A, B, C, 1:eltType, 0:eltType, opA=Op.T);
    return C;
  }
}


/* Transpose vector or matrix */
proc _array.T where this.domain.rank == 2 { return transpose(this); }


/* Transpose matrix with native implementation */
pragma "no doc"
private proc _transpose(A: [?Dom] ?eltType) {
  var C: [transpose(Dom)] eltType;

  // naive algorithm
  forall (i, j) in Dom do
    C[j, i] = A[i, j];

  return C;
}


/* Add matrices, maintaining dimensions */
proc matPlus(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != Bdom.rank then compilerError("Unmatched ranks");
  if Adom.shape != Bdom.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = A + B;
  return C;
}


/* Subtract matrices, maintaining dimensions */
proc matMinus(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != Bdom.rank then compilerError("Unmatched ranks");
  if Adom.shape != Bdom.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = A - B;
  return C;
}

/*
    Generic matrix multiplication, ``A`` and ``B`` can be a matrix, vector, or
    scalar.

    When ``A`` is a vector and ``B`` is a matrix, this function implicitly
    computes ``dot(transpose(A), B)``, which may not be as efficient as
    passing ``A`` and ``B`` in the reverse order.
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
  return C;
}


pragma "no doc"
/* Element-wise scalar multiplication */
proc dot(a, B: []) where isNumeric(a) {
  return dot(B, a);
}


/* Explicit matrix-(matrix|vector) multiplication */
private proc matMult(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  // matrix-vector
  if Adom.rank == 2 && Bdom.rank == 1 then
    return _matvecMult(A, B);
  // vector-matrix
  else if Adom.rank == 1 && Bdom.rank == 2 then
    return _matvecMult(B, A, trans=true);
  // matrix-matrix
  else if Adom.rank == 2 && Bdom.rank == 2 then
    return _matmatMult(A, B);
  else
    compilerError("Rank sizes are not 1 or 2");
}


pragma "no doc"
/* matrix-vector multiplication */
private proc _matvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType, trans=false)
  where isBLASType(eltType)
{
  if Adom.rank != 2 || Xdom.rank != 1 then
    compilerError("Rank sizes are not 2 and 1");
  if !trans {
    if Adom.shape(2) != Xdom.shape(1) then
      halt("Mismatched shape in matrix-vector multiplication");
  } else {
    if Adom.shape(1) != Xdom.shape(1) then
      halt("Mismatched shape in matrix-vector multiplication");
  }


  var op = if trans then BLAS.Op.T
           else BLAS.Op.N;

  var Ydom = if trans then {Adom.dim(2)}
             else {Adom.dim(1)};

  var Y: [Ydom] eltType;
  gemv(A, X, Y, 1:eltType, 0:eltType, opA=op);
  return Y;
}


pragma "no doc"
/* matrix-matrix multiplication */
private proc _matmatMult(A: [?Adom] ?eltType, B: [?Bdom] eltType)
  where isBLASType(eltType)
{
  if Adom.rank != 2 || Bdom.rank != 2 then
    compilerError("Rank sizes are not 2");
  if Adom.shape(2) != Bdom.shape(1) then
    halt("Mismatched shape in matrix-matrix multiplication");

  var C: [Adom.dim(1), Bdom.dim(2)] eltType;
  gemm(A, B, C, 1:eltType, 0:eltType);
  return C;
}


/* Inner product of 2 vectors */
proc inner(A: [?Adom], B: [?Bdom]) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Rank sizes are not 1");
  if Adom.size != Bdom.size then
    halt("Mismatched size in inner multiplication");

  return + reduce(A[..]*B[..]);
}


/* Outer product of 2 vectors */
proc outer(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Rank sizes are not 1");

  var C: [{Adom.dim(1), Bdom.dim(1)}] eltType;
  forall (i,j) in C.domain do
    C[i, j] = A[i]*B[j];
  return C;
}


pragma "no doc"
/* Generic matrix-vector multiplication */
proc _matvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType, trans=false)
  where !isBLASType(eltType)
{
  if Adom.rank != 2 || Xdom.rank != 1 then
    compilerError("Rank sizes are not 2 and 1");

  var Ydom = if trans then {Adom.dim(2)}
             else {Adom.dim(1)};

  var Y: [Ydom] eltType;

  // naive algorithm
  if !trans {
    if Adom.shape(2) != Xdom.shape(1) then
      halt("Mismatched shape in matrix-vector multiplication");
    forall i in Ydom do
      Y[i] = + reduce (A[i,..]*X[..]);
  } else {
    if Adom.shape(1) != Xdom.shape(1) then
      halt("Mismatched shape in matrix-vector multiplication");
    forall i in Ydom do
      Y[i] = + reduce (A[.., i]*X[..]);
  }

  return Y;
}


pragma "no doc"
/* Generic matrix-matrix multiplication */
proc _matmatMult(A: [?Adom] ?eltType, B: [?Bdom] eltType)
  where !isBLASType(eltType)
{
  if Adom.rank != 2 || Bdom.rank != 2 then
    compilerError("Rank sizes are not 2 and 2");

  var C: [Adom.dim(1), Bdom.dim(2)] eltType;

  // naive algorithm
  forall (i,j) in C.domain do
    C[i,j] = + reduce (A[i,..]*B[..,j]);

  return C;
}


/* Return the matrix ``A`` to the ``bth`` power, where ``b`` is a positive
   integral type. */
proc matPow(A: [], b) where isNumeric(b) {
  // TODO -- flatten recursion into while-loop
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


/* Return cross-product of 3-element vectors ``A`` and ``B`` with domain of
  ``A`` */
proc cross(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Rank sizes are not 1");
  if Adom.size != 3 || Bdom.size != 3 then
    halt("cross() expects arrays of 3 elements");

  var C = Vector(Adom, eltType);

  // TODO -- a better way to do 0-based indexing...
  C[_raw(Adom, 0)] = A[_raw(Adom, 1)]*B[_raw(Bdom, 2)] -
                     A[_raw(Adom, 2)]*B[_raw(Bdom, 1)];

  C[_raw(Adom, 1)] = A[_raw(Adom, 2)]*B[_raw(Bdom, 0)] -
                     A[_raw(Adom, 0)]*B[_raw(Bdom, 2)];

  C[_raw(Adom, 2)] = A[_raw(Adom, 0)]*B[_raw(Bdom, 1)] -
                     A[_raw(Adom, 1)]*B[_raw(Bdom, 0)];

  return C;
}


pragma "no doc"
/* Compute 0-based index */
private inline proc _raw(D: domain(1), i) {
  return D.dim(1).orderToIndex(i);
}


//
// Matrix Structure
//

/*
   Return lower triangular part of matrix, below the diagonal + ``k``,
   where ``k = 0`` does *not* include the diagonal, and ``k = 1`` includes the
   diagonal
 */
proc tril(A: [?D] ?eltType, k=0) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  var L = Matrix(A);
  forall (i, j) in D do
    if (i < j-k) then L[i, j] = 0: eltType;
  return L;
}


/*
   Return upper triangular part of matrix, above the diagonal + ``k``,
   where ``k = 0`` does *not* include the diagonal, and ``k = -1`` includes the
   diagonal
 */
proc triu(A: [?D] ?eltType, k=0) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  var U = Matrix(A);
  const zero = 0: eltType;
  forall (i, j) in D do
    if (i > j-k) then U[i, j] = zero;
  return U;
}


/* Return `true` if matrix is diagonal */
proc isDiag(A: [?D] ?eltType) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");

  const zero = 0: eltType;
  for (i, j) in D {
    if i != j && A[i, j] != zero then return false;
  }
  return true;
}


/* Return `true` if matrix is Hermitian */
proc isHermitian(A: [?D]) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  for (i, j) in D {
    if i > j {
      if A[i, j] != conjg(A[j, i]) then return false;
    }
  }
  return true;
}


/* Return `true` if matrix is symmetric */
proc isSymmetric(A: [?D]) : bool {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  for (i, j) in D {
    if i > j {
      if A[i, j] != A[j, i] then return false;
    }
  }
  return true;
}


/*
   Return `true` if matrix is lower triangular below the diagonal + ``k``,
   where ``k = 0`` does *not* include the diagonal, and ``k = 1`` includes the
   diagonal
 */
proc isTril(A: [?D] ?eltType, k=0) : bool {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  const zero = 0: eltType;
  for (i, j) in D do
    if (i < j-k) && (A[i, j] != zero) then
      return false;
  return true;
}


/* Return `true` if matrix is upper triangular above the diagonal + ``k``,
   where ``k = 0`` does *not* include the diagonal, and ``k = -1`` includes the
   diagonal
 */
proc isTriu(A: [?D] ?eltType, k=0) : bool {
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


/* Return the trace of square matrix ``A`` */
proc trace(A: [?D] ?eltType) {
  if D.rank != 2 then compilerError("Rank size not 2");
  else
    if !isSquare(A) then halt("Trace only supports square matrices");
    var trace = 0: eltType;
    forall (i, j) in zip(D.dim(1), D.dim(2)) with (+ reduce trace) do
      trace += A[i, j];
    return trace;
}

} // module LinearAlgebra
