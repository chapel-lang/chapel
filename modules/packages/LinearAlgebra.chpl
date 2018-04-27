/*
 * Copyright 2004-2018 Cray Inc.
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

.. _LinearAlgebraInterface:

Linear Algebra Interface
------------------------

**Matrix and Vector representation**

Matrices and vectors are represented as Chapel arrays, which means the
convenience constructors provided in this module are not necessary to use this
module's functions. This also means that matrices (2D arrays) and vectors (1D
arrays) will work with any other Chapel library that works with arrays.

.. note::

  This documentation uses the terms `matrix` to refer to `2D arrays`, and
  `vector` to refer to `1D arrays`.

**Indexing**

All functions that return new arrays will return arrays over 1-based indices,
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

**Domain maps**

All of the functions in this module only support
``DefaultRectangular`` arrays (the default domain map), unless explicitly
specified in the function's documentation. Other domain maps
are supported through submodules, such ``LinearAlgebra.Sparse`` for the
``CS`` layout.

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

To avoid this, you can avoid relying on inferred-types for new arrays:

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

*/

module LinearAlgebra {

use Norm; // TODO -- merge Norm into LinearAlgebra
use BLAS;
use LAPACK;

// TODO: compilerError if matrices are distributed

//
// Matrix and Vector Initializers
//

/* Return a vector (1D array) over domain ``{1..length}``*/
proc Vector(length, type eltType=real) {
  if (length <= 0) then halt("Vector length must be > 0");
  return Vector(1..length, eltType);
}


/* Return a vector (1D array) over domain ``{space}`` */
proc Vector(space: range, type eltType=real) {
  return Vector({space}, eltType);
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
/* Vector(Scalars...) overload where eltType is inferred */
proc Vector(x: ?t, Scalars...?n)  where isNumericType(t) {
  type eltType = x.type;
  return Vector(x, (...Scalars), eltType=eltType);
}


/* Return a vector (1D array), given 2 or more numeric values

   If `type` is omitted, it will be inferred from the first argument
*/
proc Vector(x: ?t, Scalars...?n, type eltType) where isNumericType(t) {

  if !isNumeric(Scalars(1)) then
      compilerError("Vector() expected numeric arguments");

  // First element is x, and remaining elements are Scalars
  var V: [1..n+1] eltType;

  V[1] = x: eltType;

  forall i in 2..n+1 {
    V[i] = Scalars[i-1]: eltType;
  }

  return V;
}


/* Return a square matrix (2D array) over domain ``{1..rows, 1..rows}``*/
proc Matrix(rows, type eltType=real) where isIntegral(rows) {
  if rows <= 0 then halt("Matrix dimensions must be > 0");
  return Matrix(1..rows, 1..rows, eltType);
}


/* Return a matrix (2D array) over domain ``{1..rows, 1..cols}``*/
proc Matrix(rows, cols, type eltType=real) where isIntegral(rows) && isIntegral(cols) {
  if rows <= 0 || cols <= 0 then halt("Matrix dimensions must be > 0");
  return Matrix(1..rows, 1..cols, eltType);
}


/* Return a square matrix (2D array) over domain ``{space, space}`` */
proc Matrix(space: range, type eltType=real) {
  return Matrix({space, space}, eltType);
}


/* Return a matrix (2D array) over domain ``{rowSpace, colSpace}`` */
proc Matrix(rowSpace: range, colSpace: range, type eltType=real) {
  return Matrix({rowSpace, colSpace}, eltType);
}


/* Return a matrix (2D array) over domain ``Dom`` */
proc Matrix(Dom: domain, type eltType=real) where Dom.rank == 2 {
  var A: [Dom] eltType;
  return A;
}


/* Return a matrix (2D array) with domain and values of ``A``.

   ``A`` can be sparse (CS) or dense.
*/
proc Matrix(A: [?Dom] ?Atype, type eltType=Atype)
  where Dom.rank == 2 && isDefaultRectangularArr(A)
{
  var M: [Dom] eltType = A: eltType;
  return M;
}

pragma "no doc"
/* Return a matrix (2D array) with domain and values of ``A`` - sparse case */
proc Matrix(A: [?Dom] ?Atype, type eltType=Atype)
  where Dom.rank == 2 && Sparse.isCSArr(A)
{
  var M: [Dom._value.parentDom] eltType;

  forall (i,j) in Dom {
    M[i, j] = A[i, j]: eltType;
  }

  return M;
}

pragma "no doc"
/* Return a matrix (2D array) with domain and values of ``A`` - sparse case */
proc Matrix(A: [?Dom] ?Atype, type eltType=Atype)
  where Dom.rank == 2 && isDefaultSparseArr(A)
{
  var M: [Dom._value.parentDom] eltType;
  forall (i,j) in Dom {
    M[i,j] = A[i,j]: eltType;
  }
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

  const dim2 = 1..Arrays(1).domain.dim(1).size,
        dim1 = 1..n;

  var M: [{dim1, dim2}] eltType;

  for i in dim1 do
    M[i, ..] = Arrays(i)[..]: eltType;

  return M;
}


/* Helper function for setting diagonal to 1. ``idx`` is the minimum */
private proc _eyeDiagonal(ref A: [?Dom] ?eltType) {
  const (m, n) = A.shape;
  const idx = if m <= n then 1 else 2;
  for i in Dom.dim(idx) do A[i, i] = 1: eltType;
}

/* Return a square identity matrix over domain ``{1..m, 1..m}`` */
proc eye(m, type eltType=real) {
  var A: [{1..m, 1..m}] eltType;
  _eyeDiagonal(A);
  return A;
}


/* Return an identity matrix over domain ``{1..m, 1..n}`` */
proc eye(m, n, type eltType=real) {
  var A: [{1..m, 1..n}] eltType;
  _eyeDiagonal(A);
  return A;
}


/* Return an identity matrix over domain ``Dom`` */
proc eye(Dom: domain(2), type eltType=real) {
  var A: [Dom] eltType;
  _eyeDiagonal(A);
  return A;
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
proc transpose(A: [?Dom] ?eltType) where isDefaultRectangularArr(A) && Dom.rank == 2 {
  if Dom.shape(1) == 1 then
    return reshape(A, transpose(Dom));
  else if Dom.shape(2) == 1 then
    return reshape(A, transpose(Dom));
  else {
    const rDom = {Dom.dim(2), Dom.dim(1)};
    var C: [rDom] eltType;

    [(i, j) in Dom] C[j, i] = A[i, j];

    return C;
  }
}

/* Transpose vector or matrix */
proc _array.T where isDefaultRectangularArr(this) && this.domain.rank == 2
{
  return transpose(this);
}

/* Add matrices, maintaining dimensions, deprecated for ``_array.plus`` */
proc matPlus(A: [?Adom] ?eltType, B: [?Bdom] eltType) where isDefaultRectangularArr(A) && isDefaultRectangularArr(B) {
  compilerWarning('matPlus has been deprecated for _array.plus, ' +
                  'try: A.plus(B)');
  return A.plus(B);
}

/* Add matrices, maintaining dimensions */
proc _array.plus(A: [?Adom]) where isDefaultRectangularArr(A) && isDefaultRectangularArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this + A;
  return C;
}

/* Subtract matrices, maintaining dimensions, deprecated for ``_array.minus``*/
proc matMinus(A: [?Adom] ?eltType, B: [?Bdom] eltType) where isDefaultRectangularArr(A) && isDefaultRectangularArr(B) {
  compilerWarning('matMinus has been deprecated for _array.plus, ' +
                  'try: A.minus(B)');
  return A.minus(B);
}

/* Subtract matrices, maintaining dimensions */
proc _array.minus(A: [?Adom]) where isDefaultRectangularArr(A) && isDefaultRectangularArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this - A;
  return C;
}

/* Element-wise multiplication, maintaining dimensions */
proc _array.times(A: [?Adom]) where isDefaultRectangularArr(A) && isDefaultRectangularArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this * A;
  return C;
}

/* Element-wise division, maintaining dimensions */
proc _array.elementDiv(A: [?Adom]) where isDefaultRectangularArr(A) && isDefaultRectangularArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this / A;
  return C;
}

/*
    Generic matrix multiplication, ``A`` and ``B`` can be a matrix, vector, or
    scalar.

    When ``A`` is a vector and ``B`` is a matrix, this function implicitly
    computes ``dot(transpose(A), B)``, which may not be as efficient as
    passing ``A`` and ``B`` in the reverse order.
*/
proc dot(A: [?Adom] ?eltType, B: [?Bdom] eltType) where isDefaultRectangularArr(A) && isDefaultRectangularArr(B) {
  // vector-vector
  if Adom.rank == 1 && Bdom.rank == 1 then
    return inner(A, B);
  // matrix-(vector|matrix)
  else
    return matMult(A, B);
}

/* Compute the dot-product */
proc _array.dot(A: []) where isDefaultRectangularArr(this) && isDefaultRectangularArr(A) {
  return LinearAlgebra.dot(this, A);
}


pragma "no doc"
/* Element-wise scalar multiplication */
proc dot(A: [?Adom] ?eltType, b) where isNumeric(b) {
  var C: A.type = A * b;
  return C;
}


pragma "no doc"
/* Element-wise scalar multiplication */
proc dot(a, B: []) where isNumeric(a) {
  var C: B.type = B * a ;
  return C;
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
    else if n == 0  then return eye(x.domain, x.eltType);
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

  // Reindex to ensure 1-based indices
  const A1Dom = {1..Adom.size};
  ref A1 = A.reindex(A1Dom),
      B1 = B.reindex(A1Dom),
      C1 = C.reindex(A1Dom);

  C1[1] = A1[2]*B1[3] - A1[3]*B1[2];
  C1[2] = A1[3]*B1[1] - A1[1]*B1[3];
  C1[3] = A1[1]*B1[2] - A1[2]*B1[1];

  return C;
}


//
// Matrix Structure
//


/*
   Return a Vector containing the diagonal elements of ``A`` if the argument ``A`` is of rank 2.
   Return a diagonal Matrix whose diagonal contains elements of ``A`` if argument ``A`` is of rank 1.
 */
proc diag(A: [?Adom] ?eltType, k=0) {
  if (Adom.rank == 2) {
    if (k == 0) then
      return _diag_vec(A);
    else
      return _diag_vec(A, k);
  }
  else if (Adom.rank == 1) then
    return _diag_mat(A);
  else compilerError("A must have rank 2 or less");
}

private proc _diag_vec(A:[?Adom] ?eltType) {
  const (m, n) = Adom.shape;
  const d = if m < n then 1 else 2;
  const dim = Adom.dim(d);

  var diagonal = Vector(dim, eltType);

  forall i in dim do
    diagonal[i] = A[i,i];

  return diagonal;
}

private proc _diag_vec(A:[?Adom] ?eltType, k) {
  const (m, n) = Adom.shape;
  const d = if m < n then 1 else 2;
  const dim = Adom.dim(d);

  if k > 0 {
    // Upper diagonal
    if m < k then halt("k is out of range");

    var length = min(m, n - k);
    const space = dim.first..#length;
    var diagonal = Vector(space, eltType);

    forall i in space do
      diagonal[i] = A[i, i+k];

    return diagonal;
  }
  else{
    // Lower diagonal
    const K = abs(k);
    if m < K then halt("k is out of range");

    var length = min(n, m - K);
    const space = dim.first..#length;
    var diagonal = Vector(space, eltType);

    forall i in space do
      diagonal[i] = A[i+K, i];

    return diagonal;
  }
}

private proc _diag_mat(A:[?Adom] ?eltType){
  var diagonal = Matrix(Adom.dim(1), eltType);

  forall i in Adom.dim(1) do
    diagonal[i, i] = A[i];

  return diagonal;
}


/*
   Return lower triangular part of matrix, above the diagonal + ``k``,
   where ``k = 0`` includes the diagonal, and ``k = -1`` does *not* include
   the diagonal. For example:

   .. code-block:: chapel

    var A = Matrix(4, 4, eltType=int);
    A = 1;

    tril(A);
    /* Returns:

        1    0    0    0
        1    1    0    0
        1    1    1    0
        1    1    1    1
    */

    tril(A, 1);
    /* Returns:

        1    1    0    0
        1    1    1    0
        1    1    1    1
        1    1    1    1
    */

    tril(A, -1);
    /* Returns:

        0    0    0    0
        1    0    0    0
        1    1    0    0
        1    1    1    0
    */
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
   where ``k = 0`` includes the diagonal, and ``k = 1`` does *not* include
   the diagonal. For example:

   .. code-block:: chapel

      var A = Matrix(4, 4, eltType=int);
      A = 1;

      triu(A);
      /* Returns:

          1    1    1    1
          0    1    1    1
          0    0    1    1
          0    0    0    1
      */

      triu(A, 1);
      /* Returns:

          0    1    1    1
          0    0    1    1
          0    0    0    1
          0    0    0    0
      */

      triu(A, -1);
      /* Returns:

          1    1    1    1
          1    1    1    1
          0    1    1    1
          0    0    1    1
      */
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

  // Check if any element not along the diagonal is nonzero
  for (i, j) in D {
    if i != j && A[i, j] != zero then return false;
  }
  return true;
}


/* Return `true` if matrix is Hermitian */
proc isHermitian(A: [?D]) {
  if D.rank != 2 then
    compilerError("Rank size is not 2");
  if !isSquare(A) then
    return false;

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
  if !isSquare(A) then
    return false;

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


/* Return the trace (sum of diagonal elements) of ``A`` */
proc trace(A: [?D] ?eltType) {
  if D.rank != 2 then compilerError("Rank size not 2");

  const (m, n) = A.shape;
  const d = if m < n then 1 else 2;

  var trace = 0: eltType;
  forall i in D.dim(d) with (+ reduce trace) {
    trace += A[i, i];
  }
  return trace;
}


/* Perform a Cholesky factorization on matrix ``A``.  ``A`` must be square.
   Argument ``lower`` indicates whether to return the lower or upper
   triangular factor.  Matrix ``A`` is not modified.  Returns an array with
   the same shape as argument ``A`` with the lower or upper triangular
   Cholesky factorization of ``A``.
 */
proc cholesky(A: [] ?t, lower = true) where A.rank == 2 &&
                                            (isRealType(t) ||
                                             isComplexType(t)) {
  if !isSquare(A) then
    halt("Matrix passed to cholesky must be square");

  var copy = A;
  const uploStr = if lower then "L" else "U";
  potrf(lapack_memory_order.row_major, uploStr, copy);

  // tril and triu make/return an extra copy.  Should we zero the unused
  // triangle of the array manually instead to avoid the copy?
  return if lower then tril(copy) else triu(copy);
}


/* Find the eigenvalues and eigenvectors of matrix ``A``. ``A`` must be square.

   * If ``left`` is ``true`` then the "left" eigenvectors are computed. The
     return value is a tuple with two elements:
     ``(eigenvalues, leftEigenvectors)``

   * If ``right`` is ``true`` then the "right" eigenvectors are computed. The
     return value is a tuple with two elements:
     ``(eigenvalues, rightEigenvectors)``

   * If ``left`` and ``right`` are both ``true`` then both eigenvectors are
     computed.  The return value is a tuple with three elements:
     ``(eigenvalues, leftEigenvectors, rightEigenvectors)``

   * If ``left`` and ``right`` are both ``false`` only the eigenvalues are
     computed, and returned as a single array.

 */
proc eigvals(A: [] ?t, param left = false, param right = false)
  where isRealType(t) && A.domain.rank == 2 {

  proc convertToCplx(wr: [] t, wi: [] t) {
    const n = wi.numElements;
    var eigVals: [1..n] complex(numBits(t)*2);
    forall (rv, re, im) in zip(eigVals, wr, wi) {
      rv = (re, im): complex(numBits(t)*2);
    }
    return eigVals;
  }

  proc flattenCplxEigenVecs(wi: [] t, vec: [] t) {
    const n = wi.numElements;
    var cplx: [1..n, 1..n] complex(numBits(t)*2);

    var skipNext = false;
    for j in 1..n {
      if skipNext {
        skipNext = false;
        continue;
      }

      // if the imaginary part of the eigenvalue is zero, column j of
      // vec stores the corresponding eigenvector.  If it is not zero
      // then column j stores the real part and j+1 stores the imaginary
      // part.  In this case, the next (j+1) eigenvector is the complex
      // conjugate of the current (j) eigenvector, so compute it now and
      // skip the next loop iteration.
      if wi[j] == 0.0 {
        cplx[.., j] = vec[.., j];
      } else {
        skipNext = true;
        cplx[.., j  ] = vec[.., j] + 1.0i*vec[.., j+1];
        cplx[.., j+1] = vec[.., j] - 1.0i*vec[.., j+1];
      }
    }
    return cplx;
  }

  const n = A.domain.dim(1).length;
  if !isSquare(A) then
    halt("Matrix passed to eigvals must be square");
  var copy = A;
  var wr, wi: [1..n] t;

  if !left && !right {
    var vl, vr: [1..1, 1..n] t;
    geev(lapack_memory_order.row_major, 'N', 'N', copy, wr, wi, vl, vr);
    var eigVals = convertToCplx(wr, wi);
    return eigVals;
  } else if left && !right {
    var vl: [1..n, 1..n] t;
    var vr: [1..1, 1..n] t;
    geev(lapack_memory_order.row_major, 'V', 'N', copy, wr, wi, vl, vr);

    var eigVals = convertToCplx(wr, wi);
    var vlcplx = flattenCplxEigenVecs(wi, vl);

    return (eigVals, vlcplx);
  } else if right && !left {
    var vl: [1..1, 1..n] t;
    var vr: [1..n, 1..n] t;
    geev(lapack_memory_order.row_major, 'N', 'V', copy, wr, wi, vl, vr);

    var eigVals = convertToCplx(wr, wi);
    var vrcplx = flattenCplxEigenVecs(wi, vr);

    return (eigVals, vrcplx);
  } else {
    // left && right
    var vl: [1..n, 1..n] t;
    var vr: [1..n, 1..n] t;
    geev(lapack_memory_order.row_major, 'V', 'V', copy, wr, wi, vl, vr);

    var eigVals = convertToCplx(wr, wi);
    var vlcplx = flattenCplxEigenVecs(wi, vl);
    var vrcplx = flattenCplxEigenVecs(wi, vr);

    return (eigVals, vlcplx, vrcplx);
  }
}


/* Return the Kronecker Product of matrix ``A`` and matrix ``B``.
   If the size of A is ``x * y`` and of B is ``a * b`` then size of the resulting
   matrix will be ``(x * a) * (y * b)`` */
proc kron(A: [?ADom] ?eltType, B: [?BDom] eltType) {
  if ADom.rank != 2 || BDom.rank != 2 then compilerError("Rank size not 2");

  const (rowA, colA) = A.shape;
  const (rowB, colB) = B.shape;

  const A1Dom = {1..rowA, 1..colA},
        B1Dom = {1..rowB, 1..colB};

  // Reindex to ensure 1-based indices
  ref A1 = A.reindex(A1Dom),
      B1 = B.reindex(B1Dom);

  var C = Matrix(rowA*rowB, colA*colB, eltType=eltType);

  forall (i, j) in A1Dom {
    const stR = (i-1)*rowB,
          stC = (j-1)*colB;
    for (k, l) in B1Dom {
      C[stR+k, stC+l] = A1[i, j]*B1[k, l];
    }
  }
  return C;
}


//
// Type helpers
//
private proc isDefaultRectangularDom (D: domain) param where _to_borrowed(D._value): DefaultRectangularDom { return true; }
private proc isDefaultRectangularDom (D: domain) param { return false; }
private proc isDefaultRectangularArr (A: []) param { return isDefaultRectangularDom(A.domain); }

private proc isDefaultSparseDom(D: domain) param where _to_borrowed(D._value): DefaultSparseDom { return true; }
private proc isDefaultSparseDom(D: domain) param { return false; }
private proc isDefaultSparseArr(A: []) param { return isDefaultSparseDom(A.domain); }



/* Linear Algebra Sparse Submodule

A high-level interface to linear algebra operations and procedures for sparse
matrices (2D arrays).

Sparse matrices are represented as 2D arrays domain-mapped to a sparse *layout*.
Only the ``CS(compressRows=true)`` (CSR) layout of the
:mod:`LayoutCS` layout module is currently supported.

See the :ref:`Sparse Primer <primers-sparse>` for more information about working
with sparse domains and arrays in Chapel.

Sparse Linear Algebra Interface
-------------------------------

``LinearAlgebra.Sparse`` follows the same conventions and interface choices
as the parent module, ``LinearAlgebra``, with few exceptions. These
exceptions are detailed below.


**Sparse Domains**

In Chapel, changes to the index set of a sparse array must be made
directly on the sparse domain. When working with sparse arrays that will
require index modification, it is necessary to maintain access to their sparse
domains as well. As a result of this, the sparse linear algebra interface
provides helper functions for creating both sparse domains and sparse arrays.

A common usage of this interface might look like this:

.. code-block:: chapel

  // Create an empty 3x3 CSR domain
  var D = CSRDomain(3,3);

  // Create a CSR matrix over this domain
  var A = CSRMatrix(D, int);
  // The above is equivalent to:
  // var A: [D] int;

  // Add indices to the sparse domain along the diagonal
  D += (0,0);
  D += (1,1);
  D += (2,2);

  // Set all nonzero indices to the value of 1
  A = 1;

  // A is now a 3x3 sparse identity matrix
  writeln(A);


.. note::
  This is an early prototype package submodule. As a result, interfaces may
  change over the next release.

*/
module Sparse {

  use LayoutCS;

  /* Return an empty CSR domain over parent domain:
     ``{1..rows, 1..rows}``
   */
  proc CSRDomain(rows) where isIntegral(rows) {
    if rows <= 0 then halt("Matrix dimensions must be > 0");
    return CSRDomain(1..rows, 1..rows);
  }


  /* Return an empty CSR domain  over parent domain: ``{1..rows, 1..cols}``*/
  proc CSRDomain(rows, cols) where isIntegral(rows) && isIntegral(cols) {
    if rows <= 0 || cols <= 0 then halt("Matrix dimensions must be > 0");
    return CSRDomain(1..rows, 1..cols);
  }


  /* Return an empty CSR domain over parent domain: ``{space, space}`` */
  proc CSRDomain(space: range) {
    return CSRDomain({space, space});
  }


  /* Return an empty CSR domain over parent domain: ``{rowSpace, colSpace}`` */
  proc CSRDomain(rowSpace: range, colSpace: range) {
    return CSRDomain({rowSpace, colSpace});
  }

  /* Return a CSR domain based on domain: ``Dom``

    If ``Dom`` is dense, it will be interpreted as the parent domain, and the
    domain returned will be empty.

    If ``Dom`` is sparse (CSR), the domain returned will contain the same
    nonzeros as ``Dom``
  */
  proc CSRDomain(Dom: domain) where Dom.rank == 2 && isCSDom(Dom) {
    var csrDom: sparse subdomain(Dom._value.parentDom) dmapped CS();
    csrDom += Dom;
    return csrDom;
  }

  pragma "no doc"
  /* Return a CSR domain based on domain: ``Dom`` - Dense case */
  proc CSRDomain(Dom: domain(2)) where Dom.rank == 2 {
    var csrDom: sparse subdomain(Dom) dmapped CS();
    return csrDom;
  }


  /* Return a CSR matrix over domain: ``Dom``

    If ``Dom`` is dense, it will be interpreted as the parent domain, and the
    matrix returned will be empty.

    If ``Dom`` is sparse (CSR), the matrix returned will contain the same
    nonzeros as ``Dom``
  */
  proc CSRMatrix(Dom: domain, type eltType=real) where Dom.rank == 2 && isCSDom(Dom) {
    var M: [Dom] eltType;
    return M;
  }

  pragma "no doc"
  /* Return a CSR matrix over domain: ``Dom`` - Dense case */
  proc CSRMatrix(Dom: domain, type eltType=real) where Dom.rank == 2 && isDefaultRectangularDom(Dom) {
    var csrDom = CSRDomain(Dom);
    var M: [csrDom] eltType;
    return M;
  }


  /* Return a CSR matrix with domain and values of ``A``

    If ``A`` is dense, only the indices holding nonzero elements are added
    to the sparse matrix returned.

    If ``A`` is sparse (CSR), the returned sparse matrix will be a copy of ``A``
    casted to ``eltType``
   */
  proc CSRMatrix(A: [?Dom] ?Atype, type eltType=Atype) where Dom.rank == 2 && isCSArr(A) {
    var M: [Dom] eltType = A: eltType;
    return M;
  }

  pragma "no doc"
  /* Return a CSR matrix with domain and values of ``A`` - Dense case */
  proc CSRMatrix(A: [?Dom] ?Atype, type eltType=Atype) where Dom.rank == 2 && isDefaultRectangularArr(A) {
    var D = CSRDomain(Dom);
    var M: [D] eltType;

    const zero = 0: Atype;
    for (i,j) in Dom {
      if A[i,j] != zero {
        D += (i,j);
        M[i,j] += A[i,j];
      }
    }

    return M;
  }

  /* Return a CSR matrix constructed from internal representation:

    - ``shape``: bounding box dimensions
    - ``data``: non-zero element values
    - ``indices``: non-zero row pointers
    - ``indptr``: index pointers

  */
  proc CSRMatrix(shape: 2*int, data: [?nnzDom] ?eltType, indices: [nnzDom], indptr: [?indDom])
    where indDom.rank == 1 && nnzDom.rank == 1 {
    var ADom = CSRDomain(shape, indices, indptr);
    var A: [ADom] eltType;
    A.data = data;
    return A;
  }

  /* Return a CSR domain constructed from internal representation */
  proc CSRDomain(shape: 2*int, indices: [?nnzDom], indptr: [?indDom])
    where indDom.rank == 1 && nnzDom.rank == 1 {
    const (M, N) = shape;
    const D = {1..M, 1..N};
    var ADom: sparse subdomain(D) dmapped CS();

    ADom.startIdxDom = {1..indptr.size};
    ADom.startIdx = indptr;
    const (hasZero, zeroIndex) = indices.find(0);
    if hasZero {
      ADom.nnz = zeroIndex-1;
    } else {
      ADom.nnz = indices.size;
    }
    ADom.nnzDom = {1..indices.size};
    ADom.idx = indices;

    return ADom;
  }

  /*
      Generic matrix multiplication, ``A`` and ``B`` can be a scalar, dense
      vector, or sparse matrix.

      When ``A`` is a vector and ``B`` is a matrix, this function implicitly
      computes ``dot(transpose(A), B)``, which may not be as efficient as
      passing ``A`` and ``B`` in the reverse order.

  */
  proc dot(A: [?Adom] ?eltType, B: [?Bdom] eltType) where isSparseArr(B) || isSparseArr(A) {
    // Assumes matrix-(vector|matrix) case
    return matMult(A, B);
  }

  /* CSR matrix-(matrix|vector) multiplication */
  private proc matMult(A: [?Adom] ?eltType, B: [?Bdom] eltType) where (isSparseArr(A) || isSparseArr(B)) {
    // matrix-vector
    if Adom.rank == 2 && Bdom.rank == 1 {
      if !isCSArr(A) then
        halt("Only CSR format is supported for sparse multiplication");
      return _csrmatvecMult(A, B);
    }
    // vector-matrix
    else if Adom.rank == 1 && Bdom.rank == 2 {
      if !isCSArr(B) then
        halt("Only CSR format is supported for sparse multiplication");
      return _csrmatvecMult(B, A, trans=true);
    }
    // matrix-matrix
    else if Adom.rank == 2 && Bdom.rank == 2 {
      if !isCSArr(A) || !isCSArr(B) then
        halt("Only CSR format is supported for sparse multiplication");
      return _csrmatmatMult(A, B);
    }
    else {
      compilerError("Rank sizes are not 1 or 2");
    }
  }

  /* Compute the dot-product */
  proc _array.dot(A: []) where isCSArr(A) || isCSArr(this) {
    return LinearAlgebra.Sparse.dot(this, A);
  }

  /* Compute the dot-product */
  proc _array.dot(a) where isNumeric(a) && isCSArr(this) {
    return LinearAlgebra.dot(this, a);
  }


  /* CSR Matrix-vector multiplication */
  private proc _csrmatvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType,
                              trans=false) where isCSArr(A)
  {

    if Adom.rank != 2 || Xdom.rank != 1 then
      compilerError("Rank sizes are not 2 and 1");

    const Ydom = if trans then {Adom.dim(2)}
                    else {Adom.dim(1)};
    var Y: [Ydom] eltType;

    if !trans {
      if Adom.shape(2) != Xdom.shape(1) then
        halt("Mismatched shape in matrix-vector multiplication");
        // TODO: Loop over non-zero rows only
        forall i in Adom.dim(1) {
          for j in Adom.dimIter(2, i) {
            Y[i] += A[i, j] * X[j];
          }
        }
    } else {
      if Adom.shape(1) != Xdom.shape(1) then
        halt("Mismatched shape in matrix-vector multiplication");

      // Ensure same domain indices
      ref X2 = X.reindex(Adom.dim(1));

      forall i in Adom.dim(1) with (+ reduce Y) {
        for j in Adom.dimIter(2, i) {
          Y[j] += A[i, j] * X2[i];
        }
      }
    }
    return Y;
  }

  pragma "no doc"
  /* Sparse matrix-matrix multiplication.

     Does not assume sorted indices, but preserves sorted indices.

     Implementation derived from the SMMP algorithm:

      "Sparse Matrix Multiplication Package (SMMP)"
        Randolph E. Bank and Craig C. Douglas

      https://link.springer.com/article/10.1007/BF02070824

  */
  proc _csrmatmatMult(A: [?ADom] ?eltType, B: [?BDom] eltType) where isCSArr(A) && isCSArr(B) {
    type idxType = ADom.idxType;

    const (M, K1) = A.shape,
          (K2, N) = B.shape;

    // major axis
    var indPtr: [1..M+1] idxType;

    pass1(A, B, indPtr);

    const nnz = indPtr[indPtr.domain.last];
    var indices: [1..nnz] idxType;
    var data: [1..nnz] eltType;

    pass2(A, B, indPtr, indices, data);

    var C = CSRMatrix((M, N), data, indices, indPtr);

    // TODO: Check if array is not using sorted indices, when that's possible
    sortIndices(C);

    return C;
  }


  /* Populate indPtr and total nnz (last element of indPtr) */
  proc pass1(ref A: [?ADom] ?eltType, ref B: [?BDom] eltType, ref indPtr) {
    // TODO: Parallelize - mask -> atomic ints,
    //                   - Write a scan to compute idxPtr in O(log(n))

    /* Aliases for readability */
    proc _array.indPtr ref return this.dom.startIdx;
    proc _array.indices ref return this.dom.idx;

    const (M, K1) = A.shape,
          (K2, N) = B.shape;
    type idxType = ADom.idxType;
    var mask: [1..N] idxType;
    indPtr[1] = 1;
    var nnz = 1: idxType;

    // Rows of C
    for i in 1..M {
      var row_nnz = 0;
      const Arange = A.indPtr[i]..A.indPtr[i+1]-1;
      // Row pointers of A
      for jj in Arange {
        // Column index of A
        const j = A.indices[jj];
        const Brange = B.indPtr[j]..B.indPtr[j+1]-1;
        // Row pointers of B
        for kk in Brange {
          // Column index of B
          var k = B.indices[kk];
          if mask[k] != i {
            mask[k] = i;
            row_nnz += 1;
          }
        }
      }

      nnz = nnz + row_nnz;
      indPtr[i+1] = nnz;
    }
  }

  /* Populate indices and data */
  proc pass2(ref A: [?ADom] ?eltType, ref B: [?BDom] eltType, ref indPtr, ref indices, ref data) {
    // TODO: Parallelize - next, sums -> task-private stacks

    /* Aliases for readability */
    proc _array.indPtr ref return this.dom.startIdx;
    proc _array.indices ref return this.dom.idx;

    type idxType = ADom.idxType;

    const (M, K1) = A.shape,
          (K2, N) = B.shape;

    const cols = {1..N};

    var next: [cols] idxType = -1,
        sums: [cols] eltType;

    var nnz = 1;

    for i in 1..M {
      var head = 0:idxType,
          length = 0:idxType;

      // Maps row index (i) -> nnz index of A
      const Arange = A.indPtr[i]..A.indPtr[i+1]-1;
      for jj in Arange {
        // Non-zero column index of A for row i
        const j = A.indices[jj];
        const v = A.data[jj];

        // Maps row index (j) -> nnz index of B
        const Brange = B.indPtr[j]..B.indPtr[j+1]-1;
        for kk in Brange {
          // Non-zero column index of B for row j
          const k = B.indices[kk];

          sums[k] += v*B.data[kk];

          // push k to stack
          if next[k] == -1 {
            next[k] = head;
            head = k;
            length += 1;
          }
        }
      }

      // Recounting is faster than accessing 'nnz in indPtr[i]..indPtr[i+1]-1'
      for 1..length {
        indices[nnz] = head;
        data[nnz] = sums[head];

        nnz += 1;

        // pop next k off stack
        const temp = head;
        head = next[head];

        // clear stack as we traverse
        next[temp] = -1;
        sums[temp] = 0;
      }
    }
  }


  /* Sort CS array indices */
  private proc sortIndices(ref A: [?Dom] ?eltType) where isCSArr(A) {
    use Sort;

    const (M, N) = A.shape;

    proc _array.indPtr ref return this.dom.startIdx;
    proc _array.indices ref return this.dom.idx;
    type idxType = A.indices.eltType;

    var temp: [1..A.indices.size] (idxType, eltType);
    temp = for (idx, datum) in zip(A.indices, A.data) do (idx, datum);

    for i in 1..M {
      const rowStart = A.indPtr[i],
            rowEnd = A.indPtr[i+1]-1;
      if rowEnd - rowStart > 0 {
        insertionSort(temp[rowStart..rowEnd]);
      }
    }

    for i in temp.domain {
      (A.indices[i], A.data[i]) = temp[i];
    }
  }

  /* Transpose CSR domain */
  proc transpose(D: domain) where isCSDom(D) {
    var indices: [1..0] 2*D.idxType;
    for i in D.dim(1) {
      for j in D.dimIter(2, i) {
        indices.push_back((j, i));
      }
    }

    const parentDT = transpose(D._value.parentDom);
    var Dom: sparse subdomain(parentDT) dmapped CS();
    Dom += indices;
    return Dom;
  }

  /* Transpose CSR matrix */
  proc transpose(A: [?Adom] ?eltType) where isCSArr(A) {
    var Dom = transpose(Adom);
    var B: [Dom] eltType;

    forall i in Adom.dim(1) {
      for j in Adom.dimIter(2, i) {
        B[j, i] = A[i, j];
      }
    }
    return B;
  }

  /* Transpose CSR matrix */
  proc _array.T where isCSArr(this) { return transpose(this); }

  /* Element-wise addition */
  proc _array.plus(A) where isCSArr(this) && isCSArr(A) {
    if this.domain._value.parentDom != A.domain._value.parentDom then
      halt('Cannot add sparse arrays with non-matching parent domains');

    // Create copy of 'this'
    var BDom = this.domain;
    var B: [BDom] this.eltType;
    forall (i,j) in B.domain do B[i,j] = this[i,j];

    // If domain indices do not match, bulk add A's indices to B
    if this.domain != A.domain {
      BDom += A.domain;
    }

    // Do in-place addition of A into B
    forall (i,j) in A.domain do B[i,j] += A[i,j];

    return B;
  }

  /* Element-wise subtraction */
  proc _array.minus(A) where isCSArr(this) && isCSArr(A) {
    if this.domain._value.parentDom != A.domain._value.parentDom then
      halt('Cannot add sparse arrays with non-matching parent domains');

    // Create copy of 'this'
    var BDom = this.domain;
    var B: [BDom] this.eltType;
    forall (i,j) in B.domain do B[i,j] = this[i,j];

    // If domain indices do not match, bulk add A's indices to B
    if this.domain != A.domain {
      BDom += A.domain;
    }

    // Do in-place addition of A into B
    forall (i,j) in A.domain do B[i,j] -= A[i,j];

    return B;
  }

  /* Element-wise multiplication */
  proc _array.times(A) where isCSArr(this) && isCSArr(A) {
    if this.domain._value.parentDom != A.domain._value.parentDom then
      halt('Cannot subtract sparse arrays with non-matching parent domains');

    // Create copy of 'this'
    var BDom = this.domain;
    var B: [BDom] this.eltType;
    forall (i,j) in B.domain do B[i,j] = this[i,j];

    // If domain indices do not match, bulk add A's indices to B
    if this.domain != A.domain {
      BDom += A.domain;
    }

    // Do in-place addition of A into B
    forall (i,j) in A.domain do B[i,j] *= A[i,j];

    return B;
  }

  /* Element-wise division */
  proc _array.elementDiv(A) where isCSArr(this) && isCSArr(A) {
    if this.domain._value.parentDom != A.domain._value.parentDom then
      halt('Cannot element-wise divide sparse arrays with non-matching parent domains');

    // Create copy of 'this'
    var BDom = this.domain;
    var B: [BDom] this.eltType;
    forall (i,j) in B.domain do B[i,j] = this[i,j];

    // If domain indices do not match, bulk add A's indices to B
    if this.domain != A.domain {
      BDom += A.domain;
    }

    // Do in-place addition of A into B
    forall (i,j) in A.domain do B[i,j] /= A[i,j];

    return B;
  }

  /* Matrix division (solve) */
  pragma "no doc"
  proc _array.div(A) where isCSArr(this) && isCSArr(A) {
    compilerError("Matrix division not yet supported for sparse matrices */");
  }

  /* Return an identity matrix over sparse domain ``Dom`` */
  proc eye(Dom: domain, type eltType=real) where isCSDom(Dom) {
    const (m,n) = Dom.shape;
    var D = CSRDomain(Dom.parentDom);
    const idx = if m <= n then 1 else 2;
    for i in Dom.parentDom.dim(idx) {
      D += (i,i);
    }
    var A = CSRMatrix(D, eltType);
    for i in Dom.parentDom.dim(idx) {
      A[i,i] = 1 : eltType;
    }
    return A;
  }

  //
  // Type helpers
  //
  pragma "no doc"
  proc isCSArr(A: []) param { return isCSType(A.domain._value.dist.type); }
  pragma "no doc"
  proc isCSDom(D: domain) param { return isCSType(D._value.dist.type); }

} // submodule LinearAlgebra.Sparse


} // module LinearAlgebra
