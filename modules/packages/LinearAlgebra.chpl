/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

/*

A high-level interface to linear algebra operations and procedures.

Compiling with Linear Algebra
-----------------------------

Programs using the :mod:`LinearAlgebra` module can be built with no additional
dependencies `if` they do not use any procedures that rely on :mod:`BLAS` or
:mod:`LAPACK`. Procedure dependencies are specified in procedure documentation
below.

If a program calls a procedure that depends on :mod:`BLAS` or :mod:`LAPACK`, the
headers and library will need to be available during compilation, typically
through compiler flags and/or environment variables.

Some procedures have implementations both with `and` without dependencies. By
default, the implementation with dependencies will be selected. Users can
explicitly opt out of using the :mod:`BLAS` and :mod:`LAPACK` dependent
implementations by setting the ``blasImpl`` and ``lapackImpl`` flags to ``off``.

**Building programs with no dependencies**

.. code-block:: chpl

  // example1.chpl
  var A = Matrix([0.0, 1.0, 1.0],
                 [1.0, 0.0, 1.0],
                 [1.0, 1.0, 0.0]);
  var I = eye(3,3);
  var B = A + I;

The program above has no dependencies and can therefore be compiled without
the ``BLAS`` or ``LAPACK`` headers and libraries available:

.. code-block:: bash

  chpl example1.chpl

If this program had used a procedure with a dependency such as
:proc:`cholesky` (depends on ``LAPACK``), compilation without ``LAPACK``
headers and libraries available would result in a compilation error.

**Building programs with dependencies**

.. code-block:: chpl

  // example2.chpl
  var A = Matrix([2.0, 1.0],
                 [1.0, 2.0]);
  var (eigenvalues, eigenvectors) = eig(A, right=true);

The program above uses :proc:`eig`, which depends on :mod:`LAPACK`.
Compilation without ``LAPACK`` headers and libraries available would result in
a compilation error.

Following the instructions from the :mod:`LAPACK` module
documentation, the above program could be compiled if ``LAPACK`` is available
on the system and specified with the following compilation flags:

.. code-block:: bash

  # Building with LAPACK dependency
  chpl -I$PATH_TO_LAPACKE_INCLUDE_DIR \
       -L$PATH_TO_LIBGFORTRAN -lgfortran \
       -L$PATH_TO_LAPACK_BINARIES -llapacke -llapack -lrefblas \
       example2.chpl

**Building programs with optional dependencies**

.. code-block:: chpl

  // example3.chpl
  var A = Matrix(3,5);
  A = 2;
  var AA = A.dot(A.T);


The program above uses :proc:`dot`, which has two available implementations:
one that depends on  :mod:`BLAS` and one that is written in Chapel.  The
program will default to using the more performant ``BLAS`` implementation
of matrix-matrix multiplication.

Following the instructions from the
:mod:`BLAS` module documentation, the above program could be compiled if
``BLAS`` is available on the system and specified with the following
compilation flags:

.. code-block:: bash

  # Building with BLAS dependency
  chpl -I$PATH_TO_CBLAS_DIR \
       -L$PATH_TO_BLAS_LIBS -lblas \
       example3.chpl

.. note::

  Users can set environment variables like ``LDFLAGS`` for ``-L`` arguments and
  ``CFLAGS`` for ``-I`` arguments, to avoid throwing these flags every time.

  Additionally, the required linker flags (``-l``) may vary depending on the
  ``BLAS`` and ``LAPACK`` implementations being used.


To opt out of using the ``BLAS`` implementation, users can add the ``--set
blasImpl=off`` flag, so that ``BLAS`` is no longer a dependency:

.. code-block:: bash

  # Building with BLAS dependency explicitly disabled
  chpl --set blasImpl=off example3.chpl

Similarly, users can opt out of of ``LAPACK`` implementations with the ``--set
lapackImpl=off`` flag. Setting both flags to ``off`` will always choose the
Chapel implementation when available, and will emit a compiler error
when no native implementation is available:

.. code-block:: bash

  # Building with all dependencies explicitly disabled
  chpl --set lapackImpl=off --set blasImpl=off example3.chpl

See the documentation of :mod:`BLAS` or :mod:`LAPACK` for
more details on these flags.

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

**Domain offsets**

All functions that return arrays will inherit their domains from the input
array if possible.  Otherwise they will return arrays with 0-based indices.

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

*/

module LinearAlgebra {

import BLAS;
use LAPACK only lapack_memory_order, isLAPACKType;

/* Determines if using native Chapel implementations */
private param usingBLAS = BLAS.header != '';
private param usingLAPACK = LAPACK.header != '';

// TODO: compilerError if matrices are distributed

//
// Error hierarchy
//

/* Base ``Error`` type for ``LinearAlgebra`` errors. */
class LinearAlgebraError : Error {
    /* Stores message to be emitted upon uncaught throw */
    var info: string;

    pragma "no doc"
    proc init() { }

    pragma "no doc"
    proc init(info: string) {
      this.info = info;
    }

    pragma "no doc"
    override proc message() {
      if info.isEmpty() then
        return "LinearAlgebra error";
      else
        return "LinearAlgebra error : " + info;
    }
}

//
// Matrix and Vector Initializers
//

/* Return a vector (1D array) over domain ``{0..<length}``*/
proc Vector(length, type eltType=real) {
  if (length <= 0) then halt("Vector length must be > 0");
  return Vector(0..<length, eltType);
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

  if !isNumeric(Scalars(0)) then
      compilerError("Vector() expected numeric arguments");

  // First element is x, and remaining elements are Scalars
  var V: [0..n] eltType;

  V[0] = x: eltType;

  forall i in 1..n {
    V[i] = Scalars[i-1]: eltType;
  }

  return V;
}


/* Return a square matrix (2D array) over domain ``{0..<rows, 0..<rows}``*/
proc Matrix(rows, type eltType=real) where isIntegral(rows) {
  if rows <= 0 then halt("Matrix dimensions must be > 0");
  return Matrix(0..<rows, 0..<rows, eltType);
}


/* Return a matrix (2D array) over domain ``{0..<rows, 0..<cols}``*/
proc Matrix(rows, cols, type eltType=real) where isIntegral(rows) && isIntegral(cols) {
  if rows <= 0 || cols <= 0 then halt("Matrix dimensions must be > 0");
  return Matrix(0..<rows, 0..<cols, eltType);
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
  where isDenseMatrix(A)
{
  var M: [Dom] eltType = A: eltType;
  return M;
}

pragma "no doc"
/* Return a matrix (2D array) with domain and values of ``A`` - sparse case */
proc Matrix(A: [?Dom] ?Atype, type eltType=Atype)
  where Dom.rank == 2 && Sparse.isCSArr(A)
{
  var M: [Dom.parentDom] eltType;

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
  var M: [Dom.parentDom] eltType;
  forall (i,j) in Dom {
    M[i,j] = A[i,j]: eltType;
  }
  return M;
}

pragma "no doc"
proc Matrix(const Arrays: ?t  ...?n) where isArrayType(t) && t.rank == 1 {
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
proc Matrix(const Arrays: ?t ...?n, type eltType) where isArrayType(t) && t.rank == 1 {

  if Arrays(0).domain.rank != 1 then compilerError("Matrix() expected 1D arrays");

  const dim2 = 0..<Arrays(0).domain.dim(0).size,
        dim1 = 0..<n;

  var M: [{dim1, dim2}] eltType;

  forall i in dim1 do {
    if Arrays(i).size != Arrays(0).size then halt("Matrix() expected arrays of equal length");
    M[i, ..] = Arrays(i): eltType;
  }

  return M;
}


/* Helper function for setting diagonal to 1. ``idx`` is the minimum */
private proc _eyeDiagonal(ref A: [?Dom] ?eltType) {
  const (m, n) = A.shape;
  const idx = if m <= n then 0 else 1;
  for i in Dom.dim(idx) do A[i, i] = 1: eltType;
}

/* Return a square identity matrix over domain ``{0..<m, 0..<m}`` */
proc eye(m: integral, type eltType=real) {
  var A: [{0..<m, 0..<m}] eltType;
  _eyeDiagonal(A);
  return A;
}


/* Return an identity matrix over domain ``{0..<m, 0..<n}`` */
proc eye(m: integral, n: integral, type eltType=real) {
  var A: [{0..<m, 0..<n}] eltType;
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


/* Sets the value of a diagonal in a matrix in-place. If the matrix is sparse,
    indices on the diagonal will be added to its domain

    ``k > 0``, represents an upper diagonal starting
    from the ``k``th column, ``k == 0`` represents the main
    diagonal, ``k < 0`` represents a lower diagonal starting
    from the ``-k``th row. ``k`` is 0-indexed.
*/
proc setDiag (ref X: [?D] ?eltType, in k: int = 0, val: eltType = 0)
              where isDenseMatrix(X)
{
  // Switch indexing to 0-based if necessary
  ref Xref = X.reindex(0..<D.shape(0), 0..<D.shape(1));

  var start, end = 0;
  if (k >= 0) { // upper or main diagonal
    start = 0;
    end = D.shape(0) - k;
  }
  else { // lower diagonal
    start = -k;
    end = D.shape(0);
  }
  forall row in start..<end {
    Xref[row, row+k] = val;
  }
}

pragma "no doc"
inline proc transpose(D: domain(1)) {
  return D;
}

pragma "no doc"
inline proc transpose(D: domain(2)) {
  return {D.dim(1), D.dim(0)};
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
proc transpose(A: [?Dom] ?eltType) where isDenseMatrix(A) {
  if Dom.shape(0) == 1 then
    return reshape(A, transpose(Dom));
  else {
    const rDom = {Dom.dim(1), Dom.dim(0)};
    var C: [rDom] eltType;

    [(i, j) in Dom] C[j, i] = A[i, j];

    return C;
  }
}

/* Transpose vector or matrix */
proc _array.T where isDenseMatrix(this)
{
  return transpose(this);
}

/* Element-wise addition. Same as ``A + B``. */
proc _array.plus(A: [?Adom] ?eltType) where isDenseArr(A) && isDenseArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this + A;
  return C;
}

/* Element-wise subtraction. Same as ``A - B``. */
proc _array.minus(A: [?Adom] ?eltType) where isDenseArr(A) && isDenseArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this - A;
  return C;
}

/* Element-wise multiplication. Same as ``A * B``. */
proc _array.times(A: [?Adom]) where isDenseArr(A) && isDenseArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this * A;
  return C;
}

/* Element-wise division. Same as ``A / B``. */
proc _array.elementDiv(A: [?Adom]) where isDenseArr(A) && isDenseArr(this) {
  if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
  if Adom.shape != this.domain.shape then halt("Unmatched shapes");
  var C: [Adom] eltType = this / A;
  return C;
}

/*
    Generic matrix multiplication, ``A`` and ``B`` can be a matrix, vector, or
    scalar.

    .. note::

      When ``A`` is a vector and ``B`` is a matrix, this function implicitly
      computes ``dot(transpose(A), B)``, which may not be as efficient as
      passing ``A`` and ``B`` in the reverse order.

    .. note::

      Dense matrix-matrix and matrix-vector multiplication will utilize the
      :mod:`BLAS` module for improved performance, if available. Compile with
      ``--set blasImpl=off`` to opt out of the :mod:`BLAS` implementation.
*/
proc dot(A: [?Adom] ?eltType, B: [?Bdom] eltType) where isDenseArr(A) && isDenseArr(B) {
  // vector-vector
  if Adom.rank == 1 && Bdom.rank == 1 then
    return inner(A, B);
  // matrix-(vector|matrix)
  else
    return matMult(A, B);
}

/* Compute the dot-product

  .. note::

    Dense matrix-matrix and matrix-vector multiplication will utilize the
    :mod:`BLAS` module for improved performance, if available. Compile with
    ``--set blasImpl=off`` to opt out of the :mod:`BLAS` implementation.

*/
proc _array.dot(A: []) where isDenseArr(this) && isDenseArr(A) {
  import LinearAlgebra;
  return LinearAlgebra.dot(this, A);
}


pragma "no doc"
/* Element-wise scalar multiplication. */
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
    compilerError("Ranks are not 1 or 2");
}


pragma "no doc"
/* matrix-vector multiplication */
private proc _matvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType, trans=false)
  where BLAS.isBLASType(eltType) && usingBLAS
{
  if Adom.rank != 2 || Xdom.rank != 1 then
    compilerError("Ranks are not 2 and 1");
  if !trans {
    if Adom.shape(1) != Xdom.shape(0) then
      halt("Mismatched shape in matrix-vector multiplication");
  } else {
    if Adom.shape(0) != Xdom.shape(0) then
      halt("Mismatched shape in matrix-vector multiplication");
  }


  var op = if trans then BLAS.Op.T
           else BLAS.Op.N;

  var Ydom = if trans then {Adom.dim(1)}
             else {Adom.dim(0)};

  var Y: [Ydom] eltType;
  BLAS.gemv(A, X, Y, 1:eltType, 0:eltType, opA=op);
  return Y;
}


pragma "no doc"
/* matrix-matrix multiplication */
private proc _matmatMult(A: [?Adom] ?eltType, B: [?Bdom] eltType)
  where BLAS.isBLASType(eltType) && usingBLAS
{
  if Adom.rank != 2 || Bdom.rank != 2 then
    compilerError("Ranks are not 2");
  if Adom.shape(1) != Bdom.shape(0) then
    halt("Mismatched shape in matrix-matrix multiplication");

  var C: [Adom.dim(0), Bdom.dim(1)] eltType;
  BLAS.gemm(A, B, C, 1:eltType, 0:eltType);
  return C;
}

pragma "no doc"
/*
   Returns ``true`` if the domain is distributed

   This is currently public only for unit testing purposes.
*/
proc isDistributed(a) param {
  if chpl__isArrayView(a) {
    // NOTE that this'll return true even if `a` is a slice of a distributed
    // array that falls entirely in a single locale
    return !chpl__getActualArray(a).isDefaultRectangular();
  }
  else if isSparseDom(a.domain) {
    // TODO: is there a better way to check for distributed sparse domains?
    use BlockDist;
    return isSubtype(a.domain.dist.type, Block);
  }
  else {
    return !isSubtype(a.domain.dist.type, DefaultDist);
  }
}

/* Inner product of 2 vectors. */
proc inner(const ref A: [?Adom] ?eltType, const ref B: [?Bdom]) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Ranks are not 1");
  if Adom.size != Bdom.size then
    halt("Mismatched size in inner multiplication");

  var result: eltType = 0;

  if !isDistributed(A) {
    result = + reduce (A*B);
  }
  else {
    // Replaces `+ reduce (A*B)` for improved distributed performance

    var localResults: [Locales.domain] eltType = 0;

    coforall l in Locales do on l {
      const maxThreads = if dataParTasksPerLocale==0
                         then here.maxTaskPar else dataParTasksPerLocale;
      const localDomain = A.localSubdomain();
      const iterPerThread = divceil(localDomain.size, maxThreads);
      var localResult: eltType = 0;
      var threadResults: [0..#maxThreads] eltType = 0;

      coforall tid in 0..#maxThreads {
        const startid = localDomain.low + tid * iterPerThread;
        const temp_endid = startid + iterPerThread - 1;
        const endid = if localDomain.high < temp_endid
                      then  localDomain.high else temp_endid;
        var myResult: eltType = 0;
        for ind in startid..endid {
          myResult += A.localAccess(ind) * B.localAccess(ind);
        }
        threadResults[tid] = myResult;
      }

      for tr in threadResults {
        localResult += tr;
      }
      localResults[here.id] = localResult;
    }

    for r in localResults {
      result += r;
    }
  }

  return result;
}


/* Outer product of 2 vectors. */
proc outer(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Ranks are not 1");

  var C: [{Adom.dim(0), Bdom.dim(0)}] eltType;
  forall (i,j) in C.domain do
    C[i, j] = A[i]*B[j];
  return C;
}


pragma "no doc"
/* Generic matrix-vector multiplication. */
proc _matvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType, trans=false)
  where !usingBLAS || !BLAS.isBLASType(eltType)
{
  if Adom.rank != 2 || Xdom.rank != 1 then
    compilerError("Ranks are not 2 and 1");

  var Ydom = if trans then {Adom.dim(1)}
             else {Adom.dim(0)};

  var Y: [Ydom] eltType;

  // naive algorithm
  if !trans {
    if Adom.shape(1) != Xdom.shape(0) then
      halt("Mismatched shape in matrix-vector multiplication");
    forall i in Ydom do
      Y[i] = + reduce (A[i,..]*X[..]);
  } else {
    if Adom.shape(0) != Xdom.shape(0) then
      halt("Mismatched shape in matrix-vector multiplication");
    forall i in Ydom do
      Y[i] = + reduce (A[.., i]*X[..]);
  }

  return Y;
}


pragma "no doc"
/* Generic matrix-matrix multiplication */
proc _matmatMult(A: [?Adom] ?eltType, B: [?Bdom] eltType)
  where !usingBLAS || !BLAS.isBLASType(eltType)
{
  if Adom.rank != 2 || Bdom.rank != 2 then
    compilerError("Ranks are not 2 and 2");
  if Adom.shape(1) != Bdom.shape(0) then
    halt("Mismatched shape in matrix-matrix multiplication");

  private use RangeChunk;

  var C: [Adom.dim(0), Bdom.dim(1)] eltType;

  if hasNonStridedIndices(Adom) {
    if hasNonStridedIndices(Bdom) then
      _matmatMultHelper(A, B, C);
    else
      _matmatMultHelper(A,
                        B.reindex(0..#Bdom.shape(0), 0..#Bdom.shape(1)),
                        C.reindex(0..#Adom.shape(0), 0..#Bdom.shape(1)));
  } else {
    if hasNonStridedIndices(Bdom) then
      _matmatMultHelper(A.reindex(0..#Adom.shape(0), 0..#Adom.shape(1)),
                        B,
                        C.reindex(0..#Adom.shape(0), 0..#Bdom.shape(1)));
    else
      _matmatMultHelper(A.reindex(0..#Adom.shape(0), 0..#Adom.shape(1)),
                        B.reindex(0..#Bdom.shape(0), 0..#Bdom.shape(1)),
                        C.reindex(0..#Adom.shape(0), 0..#Bdom.shape(1)));
  }
  return C;
}

pragma "no doc"
/* Helper for Generic matrix-matrix multiplication */
proc _matmatMultHelper(ref AMat: [?Adom] ?eltType,
                       ref BMat : [?Bdom] eltType,
                       ref CMat : [] eltType)
{
  // TODO - Add logic to calculate blockSize
  // based on eltType and L1 cache size
  const blockSize = 32;
  const bVecRange = 0..#blockSize;
  const blockDom = {bVecRange, bVecRange};
  const (Adim0, Adim1) = Adom.dims();
  const (Bdim0, Bdim1) = Bdom.dims();

  const numTasks = min(here.maxTaskPar, Bdom.shape(1));
  coforall tid in 0..#numTasks {
    const myChunk = chunk(Bdim1, numTasks, tid);

    var AA: [blockDom] eltType,
        BB: [blockDom] eltType,
        CC: [blockDom] eltType;

    for (jj,kk) in {myChunk by blockSize, Bdim0 by blockSize} {
      const jMax = min(jj+blockSize-1, myChunk.high);
      const kMax = min(kk+blockSize-1, Bdim0.high);
      const jRange = 0..jMax-jj;
      const kRange = 0..kMax-kk;

      for (jB, j) in zip(jj..jMax, 0..) do
        for (kB, k) in zip(kk..kMax, 0..) do
          BB[j,k] = BMat[kB,jB];

      for ii in Adim0 by blockSize {
        const iMax = min(ii+blockSize-1, Adim0.high);
        const iRange = 0..iMax-ii;

        for (iB, i) in zip(ii..iMax, 0..) do
          for (kB, k) in zip(kk..kMax, 0..) do
            AA[i,k] = AMat[iB, kB];

        for cc in CC do
          cc = 0;

        for (k,j,i) in {kRange, jRange, iRange} do
          CC[i,j] += AA[i,k] * BB[j,k];

        for (iB, i) in zip(ii..iMax, 0..) do
          for (jB, j) in zip(jj..jMax, 0..) do
            CMat[iB,jB] += CC[i,j];
      }
    }
  }
}

pragma "no doc"
private inline proc hasNonStridedIndices(Adom : domain(2)) {
  return (if Adom.stridable
          then Adom.dim(0).stride == 1 && Adom.dim(1).stride == 1
          else true);
}

/*
  Returns the inverse of ``A`` square matrix A.


    .. note::

      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``off``.
*/
proc inv(ref A: [?Adom] ?eltType, overwrite=false) where usingLAPACK {
  if isDistributed(A) then
    compilerError("inv does not support distributed vectors/matrices");

  use SysCTypes;
  if Adom.rank != 2 then
    halt("Wrong rank for matrix inverse");

  if !isSquare(A) then
    halt("Matrix inverse only supports square matrices");

  const n = Adom.shape(0);
  var ipiv : [0..<n] c_int;

  if (!overwrite) {
    var A_clone = A;
    LAPACK.getrf(lapack_memory_order.row_major, A_clone, ipiv);
    LAPACK.getri(lapack_memory_order.row_major, A_clone, ipiv);
    return A_clone;
  }

  LAPACK.getrf(lapack_memory_order.row_major, A, ipiv);
  LAPACK.getri(lapack_memory_order.row_major, A, ipiv);

  return A;
}

/*
  Return the matrix ``A`` to the ``bth`` power, where ``b`` is a positive
  integral type.

  .. note::

    ``matPow`` will utilize the :mod:`BLAS` module for improved performance, if
    available. Compile with ``--set blasImpl=off`` to opt out of the
    :mod:`BLAS` implementation.
*/
proc matPow(A: [], b) where isNumeric(b) {
  // TODO -- flatten recursion into while-loop
  if isDistributed(A) && A.rank == 2 then
    compilerError("matPow does not support distributed matrices");
  if !isIntegral(b) then
    // TODO -- support all reals with Sylvester's formula
    compilerError("matPow only support powers of integers");
  if !isSquare(A) then
    halt("Array not square");

  return _expBySquaring(A, b).value;
}

pragma "no doc"
// This is a workaround for undesired use
// of runtime-type of the input array x below
// in the return type. See also issue #9438.
record _wrap {
  var value;
}

pragma "no doc"
/* Exponentiate by squaring recursively */
private proc _expBySquaring(x: ?t, n): _wrap(t) {
  // TODO -- _expBySquaring(pinv(x), -n);
  if n < 0  then halt("Negative powers not yet supported");
  else if n == 0  then return new _wrap(eye(x.domain, x.eltType));
  else if n == 1  then return new _wrap(x);
  else if n%2 == 0  then return _expBySquaring(dot(x, x), n / 2);
  else return new _wrap(dot(x, _expBySquaring(dot(x, x), (n - 1) / 2).value));
}

/* Return cross-product of 3-element vectors ``A`` and ``B`` with domain of
  ``A``. */
proc cross(A: [?Adom] ?eltType, B: [?Bdom] eltType) {
  if Adom.rank != 1 || Bdom.rank != 1 then
    compilerError("Ranks are not 1");
  if Adom.size != 3 || Bdom.size != 3 then
    halt("cross() expects arrays of 3 elements");

  var C = Vector(Adom, eltType);

  // Reindex to ensure 0-based indices
  const A1Dom = {0..<Adom.size};
  ref A1 = A.reindex(A1Dom),
      B1 = B.reindex(A1Dom),
      C1 = C.reindex(A1Dom);

  C1[0] = A1[1]*B1[2] - A1[2]*B1[1];
  C1[1] = A1[2]*B1[0] - A1[0]*B1[2];
  C1[2] = A1[0]*B1[1] - A1[1]*B1[0];

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
  const diagSize = min(m, n);

  var diagonal : [0..#diagSize] eltType;
  forall (i, j, diagInd) in zip (Adom.dim(0)#diagSize,
                                 Adom.dim(1)#diagSize,
                                 0..) do
    diagonal[diagInd] = A[i,j];

  return diagonal;
}

private proc _diag_vec(A:[?Adom] ?eltType, k) {
  const (m, n) = Adom.shape;

  if k > 0 {
    // Upper diagonal
    if m < k then halt("k is out of range");

    var length = min(m, n - k);
    var diagonal = Vector(0..#length, eltType);
    const offset = Adom.dim(1).stride * k;

    forall (i, j, diagInd) in zip(Adom.dim(0)#length,
                                  Adom.dim(1)#length,
                                  0..) do
      diagonal[diagInd] = A[i, j+offset];

    return diagonal;
  }
  else{
    // Lower diagonal
    const K = abs(k);
    if m < K then halt("k is out of range");

    var length = min(n, m - K);
    var diagonal = Vector(0..#length, eltType);
    const offset = Adom.dim(0).stride * K;

    forall (i, j, diagInd) in zip(Adom.dim(0)#length,
                                  Adom.dim(1)#length,
                                  0..) do
      diagonal[diagInd] = A[i+offset, j];

    return diagonal;
  }
}

private proc _diag_mat(A:[?Adom] ?eltType){
  var diagonal = Matrix(Adom.dim(0), eltType);

  forall i in Adom.dim(0) do
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
    compilerError("Rank is not 2");
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
    compilerError("Rank is not 2");
  var U = Matrix(A);
  forall (i, j) in D do
    if (i > j-k) then U[i, j] = 0;
  return U;
}



/* Return `true` if matrix is diagonal. */
proc isDiag(A: [?D] ?eltType) where isDenseMatrix(A) {
  return _isDiag(A);
}

private proc _isDiag(A: [?D] ?eltType) {
  if D.rank != 2 then
    compilerError("Rank is not 2");

  // Check if any element not along the diagonal is nonzero
  for (i, j) in D {
    if i != j && A[i, j] != 0 then return false;
  }
  return true;
}


/* Return `true` if matrix is Hermitian */
proc isHermitian(A: [?D]) where isDenseMatrix(A) {
  if D.rank != 2 then
    compilerError("Rank is not 2");
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
proc isSymmetric(A: [?D]) where isDenseMatrix(A) {
  if D.rank != 2 then
    compilerError("Rank is not 2");
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
    compilerError("Rank is not 2");
  for (i, j) in D do
    if (i < j-k) && (A[i, j] != 0) then
      return false;
  return true;
}


/* Return `true` if matrix is upper triangular above the diagonal + ``k``,
   where ``k = 0`` does *not* include the diagonal, and ``k = -1`` includes the
   diagonal
 */
proc isTriu(A: [?D] ?eltType, k=0) : bool {
  if D.rank != 2 then
    compilerError("Rank is not 2");
  for (i, j) in D do
    if (i > j-k) && (A[i, j] != 0) then
      return false;
  return true;
}


/* Return `true` if matrix is square */
proc isSquare(A: [?D]) {
  if D.rank != 2 then
    compilerError("Rank is not 2");
  const (M, N) = A.shape;
  return M == N;
}


/* Return the trace (sum of diagonal elements) of ``A`` */
proc trace(A: [?D] ?eltType) {
  if D.rank != 2 then compilerError("Ranks not 2");

  const (m, n) = A.shape;
  const d = if m < n then 0 else 1;

  var trace = 0: eltType;
  forall i in D.dim(d) with (+ reduce trace) {
    trace += A[i, i];
  }
  return trace;
}

/* LU helper function */
private proc _lu(in A: [?Adom] ?eltType) {
  const n = Adom.shape(0);
  const dim = 0..<n;
  const LUDom = {dim, dim};

  // TODO: Reduce memory usage
  var L, U, LU: [LUDom] eltType;

  var ipiv: [dim] int = dim;

  var numSwap: int = 0;

  for i in dim {
    var max = A[i,i], swaprow = i;
    for row in (i+1)..<n {
      if (abs(A[row,i]) > abs(max)) {
        max = A[row,i];
        swaprow = row;
      }
    }
    if (swaprow != i) {
      A[i,..] <=> A[swaprow,..];
      L[i,..] <=> L[swaprow,..];
      ipiv[i] <=> ipiv[swaprow];
      numSwap += 1;
    }

    forall k in i..<n {
      const sum = + reduce (L[i,..] * U[..,k]);
      U[i,k] = A[i,k] - sum;
    }

    L[i,i] = 1;

    forall k in (i+1)..<n {
      const sum = + reduce (L[k,..] * U[..,i]);
      L[k,i] = (A[k,i] - sum) / U[i,i];
    }
  }

  LU = L + U;
  forall i in dim {
    LU(i,i) = U(i,i);
  }

  return (LU, ipiv, numSwap);
}

/*
  Compute an LU factorization of square matrix `A`
  using partial pivoting, such that `A = P * L * U` where P
  is a permutation matrix. Return a tuple of size 2 `(LU, ipiv)`.

  `L` and `U` are stored in the same matrix `LU` where
  the unit diagonal elements of L are not stored.

  `ipiv` contains the pivot indices such that row i of `A`
  was interchanged with row `ipiv(i)`.
*/
proc lu(A: [?Adom] ?eltType) {
  if Adom.rank != 2 then
    halt("Wrong rank for LU factorization");

  if Adom.shape(0) != Adom.shape(1) then
    halt("LU factorization only supports square matrices");

  var (LU, ipiv, numSwap) = _lu(A);
  return (LU,ipiv);
}

/* Return a new array as the permuted form of `A` according to
    permutation array `ipiv`.*/
private proc permute(ipiv: [] int, A: [?Adom] ?eltType, transpose=false) {
  const n = Adom.shape(0);
  const dim = 0..<n;
  var B: [Adom] eltType;

  if Adom.rank == 1 {
    if transpose {
      forall (i,pi) in zip(dim, ipiv) {
        B[i] = A[pi];
      }
    }
    else {
      forall (i,pi) in zip(dim, ipiv) {
        B[pi] = A[i];
      }
    }
  }
  else if Adom.rank == 2 {
    if transpose {
      forall (i,pi) in zip(dim, ipiv) {
        B[i, ..] = A[pi, ..];
      }
    }
    else {
      forall (i,pi) in zip(dim, ipiv) {
        B[pi, ..] = A[i, ..];
      }
    }
  }
  return B;
}

/* Return the determinant of a square matrix.

    .. note::

      This procedure performs LU factorization to compute the
      determinant. In certain cases, e.g. having a lower/upper
      triangular matrix, it is more desirable to compute the
      determinant manually.
*/

proc det(A: [?Adom] ?eltType) {
  if Adom.rank != 2 then
    halt("Wrong rank for computing determinant");

  if Adom.shape(0) != Adom.shape(1) then
    halt("Determinant can only be computed from square matrices");

  var (LU,ipiv,numSwap) = _lu(A);
  const pdet = if numSwap % 2 == 0 then 1 else -1;

  // L[i,i] always = 1, so we only need to take the
  // diagonal product of U

  return (* reduce [i in Adom.dim(0)] LU[i,i]) * pdet;
}

/*
  Compute the default norm on `x`.

  For a 1D array this is the 2-norm, for a 2D array, this is the Frobenius
  norm.

  :rtype: x.eltType
*/
proc norm(x: [], param p = normType.default) {
  if x.rank > 2 {
    compilerError("Norms not implemented for array ranks > 2D");
  }

  if p == normType.default {
    param defaultType: normType = if x.rank == 1 then normType.norm2 else normType.normFrob;
    return norm(x, defaultType);
  } else {
    return _norm(x, p);
  }
}

/*
  Indicates the different types of norms supported by :proc:`norm`:

    * Default - depends on array dimensions. See :proc:`norm` for details.
    * 1-norm
    * 2-norm
    * Infinity norm
    * Frobenius norm
 */
enum normType {
  default,
  norm1,
  norm2,
  normInf,
  normFrob
};

/*
  Compute the norm indicated by `p` on the 1D array `x`.

  :rtype: x.eltType
 */
private proc _norm(x: [], param p: normType) where x.rank == 1 {
  select (p) {
  when normType.norm1 do return + reduce abs(x);
  when normType.norm2 do return sqrt(+ reduce (abs(x)*abs(x)));
  when normType.normInf do return max reduce abs(x);
  when normType.normFrob do return sqrt(+ reduce (abs(x)*abs(x)));
  otherwise halt("Unexpected norm type");
  }
}

/*
  Compute the norm indicated by `p` on the 2D array `x`.

  `p` cannot be `normType.norm2`.

  :rtype: x.eltType
 */
proc _norm(x: [?D], param p: normType) where x.rank == 2 {
  select (p) {
  when normType.norm1 do
    return max reduce forall j in D.dim(1) do (+ reduce abs(x[D.dim(0), j..j]));

  when normType.norm2 {
    compilerError("2-norm for 2D arrays are not yet implemented");
    // TODO: Add implementation:
    //var (U, s, Vh) = svd(x);
    //return max(s)
  }

  when normType.normInf do
    return max reduce forall i in D.dim(0) do (+ reduce abs(x[i..i, D.dim(1)]));

  when normType.normFrob do return sqrt(+ reduce (abs(x)*abs(x)));

  otherwise halt("Unexpected norm type");
  }
}

/* Return the solution ``x`` to the linear system `` L * x = b ``
    where ``L`` is a lower triangular matrix. Setting `unit_diag` to true
    will assume the diagonal elements as `1` and will not be referenced
    within this procedure.
*/
proc solve_tril(const ref L: [?Ldom] ?eltType, const ref b: [?bdom] eltType,
                  unit_diag = true)
{
  const n = Ldom.shape(0);
  var y = b;

  for i in 0..<n {
    const sol = if unit_diag then y(i) else y(i) / L(i,i);
    y(i) = sol;

    if (i < n - 1) {
      forall j in (i+1)..<n {
        y(j) -= L(j,i) * sol;
      }
    }
  }

  return y;
}

/* Return the solution ``x`` to the linear system `` U * x = b ``
    where ``U`` is an upper triangular matrix.
*/
proc solve_triu(const ref U: [?Udom] ?eltType, const ref b: [?bdom] eltType) {
  const n = Udom.shape(0);
  var y = b;

  for i in 0..<n by -1 {
    const sol = y(i) / U(i,i);
    y(i) = sol;

    if (i > 0) {
      forall j in 0..<i by -1 {
        y(j) -= U(j,i) * sol;
      }
    }
  }

  return y;
}

/* Return the solution ``x`` to the linear system ``A * x = b``.
*/
proc solve(A: [?Adom] ?eltType, b: [?bdom] eltType) {
  var (LU, ipiv) = lu(A);
  b = permute (ipiv, b, true);
  var z = solve_tril(LU, b);
  var x = solve_triu(LU, z);
  return x;
}

/* Compute least-squares solution to ``A * x = b``.
   Compute a vector ``x`` such that the 2-norm ``|b - A x|`` is minimized.

   ``cond`` is the cut-off threshold such that singular values will be
   considered 0.0. If ``cond < 0.0`` (defaults to ``-1.0``), the threshold will
   be set to ``max((...A.shape)) * epsilon``, where ``epsilon`` is the machine
   precision for ``A.eltType``.

   Returns a tuple of ``(x, residues, rank, s)``, where:

    - ``x`` is the the least-squares solution with shape of ``b``

    - ``residues`` is:

        + the square of the 2-norm for each column in ``b - a x`` if ``M > N`` and ``A.rank == n``.
        + a scalar if ``b`` is 1-D

    - ``rank`` is the effective rank of ``A``

    - ``s`` is the singular values of ``a``

   .. note::

     This procedure depends on the :mod:`LAPACK` module, and will generate a
     compiler error if ``lapackImpl`` is ``none``.
*/
proc leastSquares(A: [] ?t, b: [] t, cond = -1.0) throws
  where usingLAPACK && isLAPACKType(t)
{
  use SysCTypes;
  use IO; // for string.format
  import LAPACK;
  require LAPACK.header;

  if A.rank != 2 then
    compilerError('leastSquares requires A.rank == 2');
  // TODO: Support b.rank == 2  -- (m,k)
  //       update this error message and documentation
  if b.rank != 1 then
    compilerError('leastSquares requires b.rank == 1');



  if A.shape[0] != b.shape[0] {
    throw new LinearAlgebraError('leastSquares(): A.shape[0] != b.shape[0]: %i != %i'.format(A.shape[0], b.shape[0]));
  }
  if A.size == 0 || b.size == 0 {
    throw new LinearAlgebraError('leastSquares(): A and b cannot be empty');
  }

  const (m, n) = A.shape;

  // TODO: Support overwrite=true/false
  var workA = A;
  const bdim1 = b.domain.dim(0);
  const bdim2 = if b.rank == 1 then 0..0
                else b.domain.dim(1);

  var workB: [0..<b.size, 0..<bdim2.size] real;
  if b.rank == 1 then
    workB[.., 0] = b;
  else
    workB = b;

  const rcond = if cond == -1.0 then max((...A.shape))*epsilon(t) else cond;

  var matrix_order = lapack_memory_order.row_major;

  var s: [0..<min((...A.shape))] real(64);
  var rank: c_int;

  var info = LAPACK.gelsd(matrix_order, workA, workB, s, rcond, rank);

  // Check for errors
  if info < 0 then
    throw new owned IllegalArgumentError('gelsd(): Argument %i is incorrect'.format(-info));
  else if info > 0 then
    throw new owned LinearAlgebraError('gelsd(): SVD failed to converge with %i off-diagonal elements not converged to 0'.format(info));

  var x1 = workB[0..<n, 0];

  var residue: t;
  if rank == n {
    residue = + reduce (abs(workB[n.., 0]**2));
  }

  return (x1, residue, rank, s);
}

pragma "no doc" // TODO: To be publicly documented in the future
/* Generate Vandermonde matrix */
proc vander(x: [?d], in N=0) where d.rank == 1 {

  if N == 0 then N = d.size;

  var resultDom = {d.dim(0), 0..<N};
  var result: [resultDom] x.eltType;

  forall (i,j) in resultDom {
    result[i, j] = x[i]**(N-1-j);
  }

  return result;
}


/* Perform a Cholesky factorization on matrix ``A``.  ``A`` must be square.
   Argument ``lower`` indicates whether to return the lower or upper
   triangular factor.  Matrix ``A`` is not modified.  Returns an array with
   the same shape as argument ``A`` with the lower or upper triangular
   Cholesky factorization of ``A``.

    .. note::

      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``off``.
 */
proc cholesky(A: [] ?t, lower = true)
  where A.rank == 2 && isLAPACKType(t) && usingLAPACK {
  if isDistributed(A) then
    compilerError("cholesky does not support distributed vectors/matrices");
  if !isSquare(A) then
    halt("Matrix passed to cholesky must be square");

  var copy = A;
  const uploStr = if lower then "L" else "U";
  var hasError = LAPACK.potrf(lapack_memory_order.row_major, uploStr, copy);
  if(hasError > 0){
    if(isComplexType(t)) then
      halt("Matrix passed must be hermitian positive definite ");
    if(isRealType(t)) then
      halt("Matrix passed must be symmetric positive definite");
  }
  // tril and triu make/return an extra copy.  Should we zero the unused
  // triangle of the array manually instead to avoid the copy?
  return if lower then tril(copy) else triu(copy);
}


pragma "no doc"
proc cholesky(A: [] ?t, lower = true)
  where A.rank == 2 && isLAPACKType(t) && !usingLAPACK
{
  compilerError("cholesky() requires LAPACK");
}


/* Find the eigenvalues of a real-symmetric/complex-hermitian matrix
   ``A``. ``A`` must be square.

   The algorithms uses either the lower-triangular (if ``lower`` is ``true``,
   or upper-triangular part of the matrix only. If ``overwrite`` is
   true, on exiting, this part
   of the matrix, including the diagonal is overwritten.

   .. note::

     This procedure currently just returns all eigenvalues.
     To selectively return certain eigenvalues, the user should call the
     LAPACK routine directly.

   .. note::

      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``off``.

*/
proc eigvalsh(A: [] ?t, lower=true, param overwrite=false) throws where (A.domain.rank == 2) && (usingLAPACK) {
  if isDistributed(A) then
    compilerError("eigvalsh does not support distributed vectors/matrices");
  return eigh(A, lower=lower, overwrite=overwrite, eigvalsOnly=true);
}

/* Find the eigenvalues and eigenvectors of a real-symmetric/complex-hermitian matrix
   ``A``. ``A`` must be square.

   The algorithms uses either the lower-triangular (if ``lower`` is ``true``,
   or upper-triangular part of the matrix only.

   If ``overwrite`` is true, the matrix is overwritten with the eigenvectors and
   only the eigenvalues are returned, otherwise the original matrix is preserved.

   The eigenvectors are stored in the columns of the returned matrix i.e. ``A[..,i]`` is the
   ``i``'th eigenvector.

   .. note::

     This procedure currently returns all eigenvalues and eigenvectors.
     To selectively return certain eigenvalues/eigenvectors, the user should call the
     LAPACK routine directly.

   .. note::

      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``off``.

*/
proc eigh(A: [] ?t, lower=true, param eigvalsOnly=false, param overwrite=false) throws where (A.domain.rank == 2) && (usingLAPACK) {
  if isDistributed(A) then
    compilerError("eigh does not support distributed vectors/matrices");

  const (n,m) = A.shape;
  if n != m then throw new LinearAlgebraError("Non-square matrix passed to eigh");
  param nbits = numBits(t);
  var w = Vector(n, eltType = if isComplexType(t) then real(nbits/2) else t);

  var Acopy = if overwrite then none else A;
  ref Aref = if overwrite then A else Acopy;
  var jobz = if !eigvalsOnly then "V" else "N";

  var info : int;
  const uploStr = if lower then "L" else "U";

  if isComplexType(t) {
    compilerAssert((nbits==64)||(nbits==128),"LAPACK only supports 64 and 128 bit complex types");
    info = LAPACK.heev(lapack_memory_order.row_major, jobz, uploStr, Aref, w);
  } else if isRealType(t) {
    compilerAssert((nbits==32)||(nbits==64),"LAPACK only supports 32 and 64 bit real types");
    info = LAPACK.syev(lapack_memory_order.row_major, jobz, uploStr, Aref, w);
  } else {
    compilerError("eigh received unsupported type : ",t:string);
  }

  if info > 0 {
    var msg = 'Eigenvalue computation did not converge. Number of elements of the intermediate tridiagonal form that did not converge to zero: ' + info:string;
    throw new owned LinearAlgebraError(msg);
  } else if info < 0 {
    var msg = 'eigvalsh received an illegal argument in LAPACK.heev/syev() argument position: ' + info:string;
    throw new owned LinearAlgebraError(msg);
  }

  if (!eigvalsOnly && !overwrite) then return (w, Acopy); else return w;
}

/* Find the eigenvalues of matrix ``A``. ``A`` must be square.

    .. note::

      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``off``.

*/
proc eigvals(A: [] ?t) where A.domain.rank == 2 && usingLAPACK {
  if isDistributed(A) then
    compilerError("eigvals does not support distributed vectors/matrices");
  return eig(A, left=false, right=false);
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

    .. note::

      This procedure depends on the :mod:`LAPACK` module, and will generate a
      compiler error if ``lapackImpl`` is ``off``.

 */
proc eig(A: [] ?t, param left = false, param right = false)
  where A.domain.rank == 2 && usingLAPACK {
  if isDistributed(A) then
    compilerError("eig does not support distributed vectors/matrices");

  proc convertToCplx(wr: [] t, wi: [] t) {
    const n = wi.size;
    var eigVals: [0..<n] complex(numBits(t)*2);
    forall (rv, re, im) in zip(eigVals, wr, wi) {
      rv = (re, im): complex(numBits(t)*2);
    }
    return eigVals;
  }

  proc flattenCplxEigenVecs(wi: [] t, vec: [] t) {
    const n = wi.size;
    const dim = 0..<n;
    var cplx: [dim, dim] complex(numBits(t)*2);

    var skipNext = false;
    for j in dim {
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

  const n = A.domain.dim(0).size;
  const dim = 0..<n;
  const dom = {0..<n, 0..<n};
  if !isSquare(A) then
    halt("Matrix passed to eigvals must be square");
  var copy = A;
  var wr: [dim] t;
  var wi: if t == complex then nothing else [dim] t;
  var eigVals: if t == complex then [dim] t else [dim] complex(numBits(t)*2);

  if !left && !right {
    var vl, vr: [0..0, dim] t;
    if t == complex {
      LAPACK.geev(lapack_memory_order.row_major, 'N', 'N', copy, wr, vl, vr);
      eigVals = wr;
    } else {
      LAPACK.geev(lapack_memory_order.row_major, 'N', 'N', copy, wr, wi, vl, vr);
      eigVals = convertToCplx(wr, wi);
    }
    return eigVals;
  } else if left && !right {
    var vl: [dom] t;
    var vr: [0..0, dim] t;
    var vlcplx: if t == complex then [dom] t else [dom] complex(numBits(t)*2);
    if t == complex {
      LAPACK.geev(lapack_memory_order.row_major, 'V', 'N', copy, wr, vl, vr);
      eigVals = wr;
      vlcplx = vl;
    } else {
      LAPACK.geev(lapack_memory_order.row_major, 'V', 'N', copy, wr, wi, vl, vr);
      eigVals = convertToCplx(wr, wi);
      vlcplx = flattenCplxEigenVecs(wi, vl);
    }
    return (eigVals, vlcplx);
  } else if right && !left {
    var vl: [0..0, dim] t;
    var vr: [dom] t;
    var vrcplx: if t == complex then [dom] t else [dom] complex(numBits(t)*2);
    if t == complex {
      LAPACK.geev(lapack_memory_order.row_major, 'N', 'V', copy, wr, vl, vr);
      eigVals = wr;
      vrcplx = vr;
    } else {
      LAPACK.geev(lapack_memory_order.row_major, 'N', 'V', copy, wr, wi, vl, vr);
      eigVals = convertToCplx(wr, wi);
      vrcplx = flattenCplxEigenVecs(wi, vr);
    }
    return (eigVals, vrcplx);
  } else {
    // left && right
    var vl: [dom] t;
    var vr: [dom] t;
    var vlcplx: if t == complex then [dom] t else [dom] complex(numBits(t)*2);
    var vrcplx: if t == complex then [dom] t else [dom] complex(numBits(t)*2);
    if t == complex {
      LAPACK.geev(lapack_memory_order.row_major, 'V', 'V', copy, wr, vl, vr);
      eigVals = wr;
      vlcplx = vl;
      vrcplx = vr;
    } else {
      LAPACK.geev(lapack_memory_order.row_major, 'V', 'V', copy, wr, wi, vl, vr);
      eigVals = convertToCplx(wr, wi);
      vlcplx = flattenCplxEigenVecs(wi, vl);
      vrcplx = flattenCplxEigenVecs(wi, vr);
    }
    return (eigVals, vlcplx, vrcplx);
  }
}

/*
  Singular Value Decomposition.

  Factorizes the `m x n` matrix ``A`` such that:


  .. math::

    \mathbf{A} = \textbf{U} \cdot \Sigma \cdot \mathbf{V^H}

  where

    - :math:`\mathbf{U}` is an `m x m` unitary matrix,
    - :math:`\Sigma` is a diagonal `m x n` matrix,
    - :math:`\mathbf{V}` is an `n x n` unitary matrix, and
      :math:`\mathbf{V^H}` is the Hermitian transpose.

  This procedure returns a tuple of ``(U, s, Vh)``, where ``s`` is a vector
  containing the diagonal elements of :math:`\Sigma`, known as the
  singular values.

  For example:

  .. code-block:: chapel

    var A = Matrix([3, 2,  2],
                   [2, 3, -2],
                   eltType=real);
    var (U, s, Vh) = svd(A);

  ``LinearAlgebraError`` will be thrown if the SVD computation does not
  converge or an illegal argument, such as a matrix containing a ``NAN`` value,
  is given.

  .. note::

   A temporary copy of ``A`` will be created within this computation.

  .. note::

    This procedure depends on the :mod:`LAPACK` module, and will generate a
    compiler error if ``lapackImpl`` is ``off``.
*/
proc svd(A: [?Adom] ?t) throws
  where isLAPACKType(t) && usingLAPACK && Adom.rank == 2 {
  if isDistributed(A) then
    compilerError("svd does not support distributed vectors/matrices");

  const (m, n) = A.shape;

  /* real(32) or real(64) for singular values and superb */
  type realType = if t == complex(128) || t == real(64) then real(64)
                  else real(32);

  // TODO: Support argument to allow overwriting A as performance optimization
  // Copy over A since it gets destroyed during SVD
  var Acopy: [Adom] t = A;

  // Results

  // Stores singular values, sorted
  var s: [0..<min((...A.shape))] realType;
  // Unitary matrix, U
  var u: [0..<m, 0..<m] t;
  // Unitary matrix V^T (or V^H)
  var vt: [0..<n, 0..<n] t;

  // if return code 'info' > 0, then this stores unconverged superdiagonal
  // elements of upper bidiagonal matrix 'B' whose diagonal is in 's'.
  var superb: [0..<min((...A.shape))-1] realType;

  // TODO: Support option for gesdd (trading memory usage for speed)
  const info = LAPACK.gesvd(lapack_memory_order.row_major, 'A', 'A', Acopy, s, u, vt, superb);

  if info > 0 {
    var msg = 'SVD computation did not converge. Number of superdiagonals of the intermediate bidiagonal that did not converge to zero: ' + info:string;
    throw new owned LinearAlgebraError(msg);
  } else if info < 0 {
    var msg = 'SVD received an illegal argument in LAPACK.gesvd() argument position: ' + info:string;
    throw new owned LinearAlgebraError(msg);
  }

  return (u, s, vt);
}

/*
  Compute the approximate solution to ``A * x = b`` using the Jacobi method.
  iteration will stop when ``maxiter`` is reached or error is smaller than
  ``tol``, whichever comes first. Return the number of iterations performed.

  .. note::
    ``X`` is passed as a reference, meaning the initial solution guess can be
    stored in ``X`` before calling the procedure, and the approximate solution
    will be stored in the same array.

    Dense and CSR arrays are supported.
*/
proc jacobi(A: [?Adom] ?eltType, ref X: [?Xdom] eltType,
            b: [Xdom] eltType, tol = 0.0001, maxiter = 1000) {
  if Adom.rank != 2 || X.rank != 1 || b.rank != 1 then
    compilerError("jacobi: Wrong shape of input matrix or vector");
  if !isSquare(A) then
    halt("jacobi: Matrix A is not a square");
  if Adom.shape(0) != Xdom.shape(0) then
    halt("jacobi: Mismatch shape between matrix side length and vector length");

  var itern = 0, err: eltType = 1;

  var t: [Xdom] eltType = 0;

  while (itern < maxiter) {
    itern = itern + 1;
    forall i in Adom.dim(0) {
      var sigma = 0.0;
      for j in Adom.dim(1) {
        if i!=j then sigma += A(i,j) * X(j);
      }
      t(i) = (b(i) - sigma) / A(i,i);
    }
    err = max reduce abs(t - X);
    X = t;
    if err < tol {
      break;
    }
  }
  return itern;
}


pragma "no doc"
proc eig(A: [] ?t, param left = false, param right = false)
  where A.domain.rank == 2 && !usingLAPACK {
  compilerError("eigvals() requires LAPACK");
}


/* Return the Kronecker Product of matrix ``A`` and matrix ``B``.
   If the size of A is ``x * y`` and of B is ``a * b`` then size of the resulting
   matrix will be ``(x * a) * (y * b)`` */
proc kron(A: [?ADom] ?eltType, B: [?BDom] eltType) {
  if ADom.rank != 2 || BDom.rank != 2 then compilerError("Ranks not 2");

  const (rowA, colA) = A.shape;
  const (rowB, colB) = B.shape;

  const A1Dom = {0..<rowA, 0..<colA},
        B1Dom = {0..<rowB, 0..<colB};

  // Reindex to ensure 0-based indices
  ref A1 = A.reindex(A1Dom),
      B1 = B.reindex(B1Dom);

  var C = Matrix(rowA*rowB, colA*colB, eltType=eltType);

  forall (i, j) in A1Dom {
    const stR = i*rowB,
          stC = j*colB;
    for (k, l) in B1Dom {
      C[stR+k, stC+l] = A1[i, j]*B1[k, l];
    }
  }
  return C;
}


//
// Type helpers
//

// TODO: Add this to public interface eventually
pragma "no doc"
/* Returns ``true`` if the array is dense N-dimensional non-distributed array. */
proc isDenseArr(A: [?D]) param : bool {
  return isDenseDom(D);
}

// TODO: Add this to public interface eventually
pragma "no doc"
/* Returns ``true`` if the domain is dense N-dimensional non-distributed domain. */
proc isDenseDom(D: domain) param : bool {
  return isRectangularDom(D);
}

// TODO: Add this to public interface eventually
pragma "no doc"
/* Returns ``true`` if the array is N-dimensional non-distributed array. */
proc isLocalArr(A: [?D]) param : bool {
  return isLocalDom(D);
}

// TODO: Add this to public interface eventually
pragma "no doc"
/* Returns ``true`` if the domain is dense N-dimensional non-distributed domain. */
proc isLocalDom(D: domain) param : bool {
  return D.dist.type == defaultDist.type;
}

// TODO: Add this to public interface eventually
pragma "no doc"
/* Returns ``true`` if the array is dense 2-dimensional non-distributed array. */
proc isDenseMatrix(A: []) param : bool {
  return A.rank == 2 && isDenseArr(A) && isLocalArr(A);
}

// Work-around for #8543
pragma "no doc"
proc type _array.rank param {
  var x: this;
  return x.rank;
}

//
// Type helpers
//

pragma "no doc"
/* Returns ``true`` if the domain is ``DefaultSparse`` */
private proc isDefaultSparseDom(D: domain) param {
  return isSubtype(_to_borrowed(D.dist.type), DefaultDist) && isSparseDom(D);
}

pragma "no doc"
/* Returns ``true`` if the array is ``DefaultSparse`` */
private proc isDefaultSparseArr(A: []) param {
  return isDefaultSparseDom(A.domain);
}

//
// Utility functions
//

/* Machine epsilon for real(64) */
private proc epsilon(type t: real(64)) : real {
  extern const DBL_EPSILON: real;
  return DBL_EPSILON;
}

/* Machine epsilon for real(32) */
private proc epsilon(type t: real(32)) : real {
  extern const FLT_EPSILON: real;
  return FLT_EPSILON;
}

/* Machine epsilon for non-real */
private proc epsilon(type t) param : real {
  return 0.0;
}

/* Linear Algebra Sparse Submodule

A high-level interface to linear algebra operations and procedures for sparse
matrices (2D arrays).

Sparse matrices are represented as 2D arrays domain-mapped to a sparse *layout*.
All sparse operations support the CSR layout (``LayoutCS.CS(compressRows=true)``)
and some operations support COO layout (default sparse array layout).

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

  public use LayoutCS;
  private use LinearAlgebra;

  /* Return an empty CSR domain over parent domain:
     ``{0..<rows, 0..<rows}``
   */
  proc CSRDomain(rows) where isIntegral(rows) {
    if rows <= 0 then halt("Matrix dimensions must be > 0");
    return CSRDomain(0..<rows, 0..<rows);
  }


  /* Return an empty CSR domain  over parent domain: ``{0..<rows, 0..<cols}``*/
  proc CSRDomain(rows, cols) where isIntegral(rows) && isIntegral(cols) {
    if rows <= 0 || cols <= 0 then halt("Matrix dimensions must be > 0");
    return CSRDomain(0..<rows, 0..<cols);
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
    var csrDom: sparse subdomain(Dom.parentDom) dmapped CS(sortedIndices=false);
    csrDom += Dom;
    return csrDom;
  }

  pragma "no doc"
  /* Return a CSR domain based on domain: ``Dom`` - Dense case */
  proc CSRDomain(Dom: domain(2)) where Dom.rank == 2 {
    var csrDom: sparse subdomain(Dom) dmapped CS(sortedIndices=false);
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
  proc CSRMatrix(Dom: domain, type eltType=real) where Dom.rank == 2 &&
                                                       isDenseDom(Dom) &&
                                                       isLocalDom(Dom) {
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
  proc CSRMatrix(A: [?Dom] ?Atype, type eltType=Atype) where isCSArr(A) {
    var M: [Dom] eltType = A: eltType;
    return M;
  }

  pragma "no doc"
  /* Return a CSR matrix with domain and values of ``A`` - Dense case */
  proc CSRMatrix(A: [?Dom] ?Atype, type eltType=Atype) where isDenseMatrix(A) {
    var D = CSRDomain(Dom);
    var M: [D] eltType;

    for (i,j) in Dom {
      if A[i,j] != 0 {
        D += (i,j);
        M[i,j] += A[i,j];
      }
    }

    return M;
  }

  /* Return a CSR matrix constructed from internal representation:

    - ``parentDom``: parent domain for sparse matrix, bounding box for nonzeros indices
    - ``data``: non-zero element values
    - ``indices``: non-zero row pointers
    - ``indptr``: index pointers

  */
  proc CSRMatrix(parentDom: domain(2), data: [?nnzDom] ?eltType, indices: [nnzDom], indptr: [?indDom])
        where indDom.rank == 1 && nnzDom.rank == 1 {
    var ADom = CSRDomain(parentDom, indices, indptr);
    var A: [ADom] eltType;
    A.data = data;
    return A;
  }

  /* Return a CSR domain constructed from internal representation */
  proc CSRDomain(parentDom: domain(2), indices: [?nnzDom], indptr: [?indDom])
        where indDom.rank == 1 && nnzDom.rank == 1 {
    const rowRange = parentDom.dim(0).low..parentDom.dim(0).high;
    var ADom: sparse subdomain(parentDom) dmapped CS(sortedIndices=false);

    ADom.startIdxDom = {rowRange.low..rowRange.high+1};
    ADom.startIdx = indptr;
    ADom._nnz = indices.size;
    ADom.nnzDom = {0..#indices.size};
    ADom.idx = indices;

    return ADom;
  }

  /* Return a CSR matrix constructed from internal representation:
    - ``shape``: (M,N) bounding box will be {0..#M,0..#N}
    - ``data``: non-zero element values
    - ``indices``: non-zero row pointers
    - ``indptr``: index pointers
  */
  proc CSRMatrix(shape: 2*int, data: [?nnzDom] ?eltType, indices: [nnzDom], indptr: [?indDom])
        where indDom.rank == 1 && nnzDom.rank == 1 {
    const (M, N) = shape;
    return CSRMatrix({0..#M, 0..#N}, data, indices, indptr);
  }

  /* Return a CSR domain constructed from internal representation */
  proc CSRDomain(shape: 2*int, indices: [?nnzDom], indptr: [?indDom])
        where indDom.rank == 1 && nnzDom.rank == 1 {
    const (M, N) = shape;
    return CSRDomain({0..#M, 0..#N}, indices, indptr);
  }

  /*
      Generic matrix multiplication, ``A`` and ``B`` can be a scalar, dense
      vector, or sparse matrix.

      .. note::

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
        compilerError("Only CSR format is supported for sparse multiplication");
      return _csrmatvecMult(A, B);
    }
    // vector-matrix
    else if Adom.rank == 1 && Bdom.rank == 2 {
      if !isCSArr(B) then
        compilerError("Only CSR format is supported for sparse multiplication");
      return _csrmatvecMult(B, A, trans=true);
    }
    // matrix-matrix
    else if Adom.rank == 2 && Bdom.rank == 2 {
      if !isCSArr(A) || !isCSArr(B) then
        compilerError("Only CSR format is supported for sparse multiplication");
      return _csrmatmatMult(A, B);
    }
    else {
      compilerError("Ranks are not 1 or 2");
    }
  }

  /* Compute the dot-product */
  proc _array.dot(A: []) where isCSArr(A) || isCSArr(this) {
    import LinearAlgebra;
    return LinearAlgebra.Sparse.dot(this, A);
  }

  /* Compute the dot-product */
  proc _array.dot(a) where isNumeric(a) && isCSArr(this) {
    import LinearAlgebra;
    return LinearAlgebra.dot(this, a);
  }


  /* CSR Matrix-vector multiplication */
  private proc _csrmatvecMult(A: [?Adom] ?eltType, X: [?Xdom] eltType,
                              trans=false) where isCSArr(A)
  {

    if Adom.rank != 2 || Xdom.rank != 1 then
      compilerError("Ranks are not 2 and 1");

    const Ydom = if trans then {Adom.dim(1)}
                    else {Adom.dim(0)};
    var Y: [Ydom] eltType;

    if !trans {
      if Adom.shape(1) != Xdom.shape(0) then
        halt("Mismatched shape in matrix-vector multiplication");
        // TODO: Loop over non-zero rows only
        forall i in Adom.dim(0) {
          for j in Adom.dimIter(1, i) {
            Y[i] += A[i, j] * X[j];
          }
        }
    } else {
      if Adom.shape(0) != Xdom.shape(0) then
        halt("Mismatched shape in matrix-vector multiplication");

      // Ensure same domain indices
      ref X2 = X.reindex(Adom.dim(0));

      forall i in Adom.dim(0) with (+ reduce Y) {
        for j in Adom.dimIter(1, i) {
          Y[j] += A[i, j] * X2[i];
        }
      }
    }
    return Y;
  }

  /* Sparse matrix-matrix multiplication.

     Does not assume sorted indices, but preserves sorted indices.

     Implementation derived from the SMMP algorithm:

      "Sparse Matrix Multiplication Package (SMMP)"
        Randolph E. Bank and Craig C. Douglas

      https://link.springer.com/article/10.1007/BF02070824

  */
  private proc _csrmatmatMult(A: [?ADom] ?eltType, B: [?BDom] eltType)
    where isCSArr(A) && isCSArr(B)
  {
    type idxType = ADom.idxType;

    // The ranges for the inner dimensions need to match up
    const rowRange = ADom.dim(0);
    const innerARange = ADom.dim(1);
    const innerBRange = BDom.dim(0);
    const colRange = BDom.dim(1);

    if innerARange != innerBRange then
        halt("Mismatched shape in sparse matrix-matrix multiplication");

    // TODO: Return empty (rowRange, colRange) sparse array if A or B size == 0

    /* Note on compressed sparse (CS) internals:
       - startIdx.domain (indPtr) can start with any index
       - idx.domain (ind) starts on 0
       - data.domain starts on 0
     */

    // major axis (or row) for result matrix
    var indPtr: [rowRange.low..rowRange.high+1] idxType;

    pass1(A, B, indPtr);

    const nnz = indPtr[indPtr.domain.last];
    var ind: [0..#nnz] idxType;
    var data: [0..#nnz] eltType;

    pass2(A, B, indPtr, ind, data);

    var C = CSRMatrix({rowRange, colRange}, data, ind, indPtr);

    if C.domain.sortedIndices {
      sortIndices(C);
    }

    return C;
  }


  /* Populate indPtr and total nnz (last element of indPtr) */
  private proc pass1(ref A: [?ADom] ?eltType, ref B: [?BDom] eltType, ref indPtr) {
    // TODO: Parallelize - mask -> atomic ints,
    //                   - Write a scan to compute idxPtr in O(log(n))

    /* Aliases for readability */
    proc _array.indPtr ref return this.dom.startIdx;
    proc _array.ind ref return this.dom.idx;

    const row_range = ADom.dim(0);
    const inner_Arange = ADom.dim(1);
    const inner_Brange = BDom.dim(0);
    const col_range = BDom.dim(1);

    type idxType = ADom.idxType;

    var mask: [col_range.low..col_range.high] idxType;
    mask = col_range.low-1;    // init to something not in col range
    indPtr[col_range.low] = 0; // col indices definitely start at 0
    var nnz = 0: idxType;

    // Rows of output matrix C
    for i in row_range {
      var row_nnz = 0;
      const Apos_range = A.indPtr[i]..A.indPtr[i+1]-1;  // nonzero position range
      // Row pointers of A
      for jpos in Apos_range {
        // Column index of A
        const j = A.ind[jpos];
        const Bpos_range = B.indPtr[j]..B.indPtr[j+1]-1;
        // Row pointers of B
        for kk in Bpos_range {
          // Column index of B
          var k = B.ind[kk];
          if mask[k] != i { // default of 0 for mask would not count a 0-indexed matrix
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
  private proc pass2(ref A: [?ADom] ?eltType, ref B: [?BDom] eltType, ref indPtr, ref ind, ref data) {
    // TODO: Parallelize - next, sums -> task-private stacks

    /* Aliases for readability */
    proc _array.indPtr ref return this.dom.startIdx;
    proc _array.ind ref return this.dom.idx;

    type idxType = ADom.idxType;

    const row_range = ADom.dim(0);
    const inner_Arange = ADom.dim(1);
    const inner_Brange = BDom.dim(0);
    const col_range = BDom.dim(1);

    const cols = col_range;

    var next: [cols] idxType = -1,
        sums: [cols] eltType;

    var nnz = 0;

    for i in row_range {
      var head = 0:idxType,
          length = 0:idxType;

      // Maps row index (i) -> nnz index of A
      const Apos_range = A.indPtr[i]..A.indPtr[i+1]-1;
      for jpos in Apos_range {
        // Nonzero column index of A for row i
        const j = A.ind[jpos];
        const v = A.data[jpos];

        // Maps row index (j) -> nnz index of B
        const Bpos_range = B.indPtr[j]..B.indPtr[j+1]-1;
        for kk in Bpos_range {
          // Nonzero column index of B for row j
          const k = B.ind[kk];

          sums[k] += v*B.data[kk];

          // ?fromMMS: what is all of this pushing about?
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
        ind[nnz] = head;
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

    const rowRange = Dom.dim(0);

    proc _array.indPtr ref return this.dom.startIdx;
    proc _array.ind return this.dom.idx;
    type idxType = A.ind.eltType;

    var temp: [0..#A.ind.size] (idxType, eltType);
    temp = for (idx, datum) in zip(A.ind, A.data) do (idx, datum);

    for i in rowRange {
      const rowStart = A.indPtr[i],
            rowEnd = A.indPtr[i+1]-1;
      if rowEnd - rowStart > 0 {
        insertionSort(temp[rowStart..rowEnd]);
      }
    }

    for i in temp.domain {
      (A.ind[i], A.data[i]) = temp[i];
    }
  }

  /* Transpose CSR domain */
  proc transpose(D: domain) where isCSDom(D) {
    const parentDT = transpose(D.parentDom);
    var Dom: sparse subdomain(parentDT) dmapped CS(sortedIndices=false);

    var idxBuffer = Dom.makeIndexBuffer(size=D.size);
    for (i,j) in D do idxBuffer.add((j,i));
    idxBuffer.commit();
    return Dom;
  }

  /* Transpose CSR matrix */
  proc transpose(A: [?Adom] ?eltType) where isCSArr(A) {
    var Dom = transpose(Adom);
    var B: [Dom] eltType;

    forall i in Adom.dim(0) {
      for j in Adom.dimIter(1, i) {
        B[j, i] = A[i, j];
      }
    }
    return B;
  }

  /* Transpose CSR matrix */
  proc _array.T where isCSArr(this) { return transpose(this); }

  /* Element-wise addition, supports CSR and COO. */
  proc _array.plus(A: [?Adom] ?eltType) where isCSArr(this) && isCSArr(A) {
    if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
    if this.domain.shape != Adom.shape then halt("Unmatched shapes");
    var sps = CSRDomain(Adom.parentDom);
    sps += this.domain;
    sps += Adom;
    var S: [sps] eltType;
    forall (i,j) in sps {
      S[i,j] = this[i,j] + A[i,j];
    }
    return S;
  }

  pragma "no doc"
  proc _array.plus(A: [?Adom] ?eltType) where isSparseArr(this) && !isCSArr(this)
                                              && isSparseArr(A) && !isCSArr(A) {
    if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
    if this.domain.shape != Adom.shape then halt("Unmatched shapes");
    var sps: sparse subdomain(Adom.parentDom);
    sps += this.domain;
    sps += Adom;
    var S: [sps] eltType;
    forall (i,j) in sps {
      S[i,j] = this[i,j] + A[i,j];
    }
    return S;
  }

  /* Element-wise subtraction, supports CSR and COO.  */
  proc _array.minus(A: [?Adom] ?eltType) where isCSArr(this) && isCSArr(A) {
    if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
    if this.domain.shape != Adom.shape then halt("Unmatched shapes");
    var sps = CSRDomain(Adom.parentDom);
    sps += this.domain;
    sps += Adom;
    var S: [sps] eltType;
    forall (i,j) in sps {
      S[i,j] = this[i,j] - A[i,j];
    }
    return S;
  }

  pragma "no doc"
  proc _array.minus(A: [?Adom] ?eltType) where isSparseArr(this) && !isCSArr(this)
                                               && isSparseArr(A) && !isCSArr(A) {
    if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
    if this.domain.shape != Adom.shape then halt("Unmatched shapes");
    var sps: sparse subdomain(Adom.parentDom);
    sps += this.domain;
    sps += Adom;
    var S: [sps] eltType;
    forall (i,j) in sps {
      S[i,j] = this[i,j] - A[i,j];
    }
    return S;
  }

  /* Element-wise multiplication, supports CSR and COO.  */
  proc _array.times(A) where isCSArr(this) && isCSArr(A) {
    if this.domain.parentDom != A.domain.parentDom then
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

  pragma "no doc"
  proc _array.times(A: [?Adom] ?eltType) where isSparseArr(this) && !isCSArr(this)
                                               && isSparseArr(A) && !isCSArr(A) {
    if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
    if this.domain.shape != Adom.shape then halt("Unmatched shapes");
    // TODO: sps should only contain non-zero entries in resulting array,
    //       i.e. intersection of this.domain and Adom
    var sps: sparse subdomain(Adom.parentDom);
    sps += this.domain;
    sps += Adom;
    var S: [sps] eltType;
    forall (i,j) in sps {
      S[i,j] = this[i,j] * A[i,j];
    }
    return S;
  }

  /* Element-wise division, supports CSR and COO.  */
  proc _array.elementDiv(A) where isCSArr(this) && isCSArr(A) {
    if this.domain.parentDom != A.domain.parentDom then
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

  pragma "no doc"
  proc _array.elementDiv(A: [?Adom] ?eltType) where
                                            isSparseArr(this) && !isCSArr(this)
                                            && isSparseArr(A) && !isCSArr(A) {
    if Adom.rank != this.domain.rank then compilerError("Unmatched ranks");
    if this.domain.shape != Adom.shape then halt("Unmatched shapes");
    // TODO: sps should only contain non-zero entries in resulting array
    var sps: sparse subdomain(Adom.parentDom);
    sps += this.domain;
    sps += Adom;
    var S: [sps] eltType;
    forall (i,j) in Adom {
      S[i,j] = this[i,j] / A[i,j];
    }
    return S;
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
    const idx = if m <= n then 0 else 1;
    for i in Dom.parentDom.dim(idx) {
      D += (i,i);
    }
    var A = CSRMatrix(D, eltType);
    for i in Dom.parentDom.dim(idx) {
      A[i,i] = 1 : eltType;
    }
    return A;
  }

  pragma "no doc"
  proc jacobi(A: [?Adom] ?eltType, ref X: [?Xdom] eltType,
              b: [Xdom] eltType, tol = 0.0001, maxiter = 1000) where isCSArr(A) {
    if Adom.rank != 2 || X.rank != 1 || b.rank != 1 then
      compilerError("jacobi: Wrong shape of input matrix or vector");
    if Adom.shape(0) != Adom.shape(1) then
      halt("jacobi: Matrix A is not a square");
    if Adom.shape(0) != Xdom.shape(0) then
      halt("jacobi: Mismatch shape between matrix side length and vector length");

    var itern = 0, err: eltType = 1;

    var t: [Xdom] eltType = 0;

    while (itern < maxiter) {
      itern = itern + 1;
      forall i in Adom.dim(0) {
        var sigma = 0.0;
        for j in Adom.dimIter(1,i) {
          if i!=j then sigma += A(i,j) * X(j);
        }
        t(i) = (b(i) - sigma) / A(i,i);
      }
      err = max reduce abs(t - X);
      X = t;
      if err < tol {
        break;
      }
    }
    return itern;
  }

  pragma "no doc"
  proc setDiag (ref X: [?D] ?eltType, in k: int = 0, val: eltType = 0)
                where isSparseArr(X) {
      if D.rank != 2 then
        halt("Wrong rank for setDiag");

      if D.shape(0) != D.shape(1) then
        halt("setDiag only supports square matrices");

      var start, end = 0;
      if (k >= 0) { // upper or main diagonal
        start = 0;
        end = D.shape(0) - k;
      }
      else { // lower diagonal
        start = -k;
        end = D.shape(0);
      }
      var indices : [start..end] (D.idxType, D.idxType);
      forall ind in {start..end} {
        indices[ind] = (ind, ind+k);
      }
      D.bulkAdd(indices, dataSorted=true, isUnique=true, preserveInds=false);
      forall ind in indices {
        X(ind) = val;
      }
  }


  /* Return ``true`` if sparse matrix is diagonal. Supports CSR and COO arrays. */
  proc isDiag(A: [?D] ?eltType) where isSparseArr(A) {
    return _isDiag(A);
  }


  /* Return ``true`` if matrix is Hermitian. Supports CSR and COO arrays. */
  proc isHermitian(A: [?D]) where isSparseArr(A) {
    if D.rank != 2 then
      compilerError("Rank is not 2");
    if !isSquare(A) then
      return false;

    for (i, j) in D {
      if A[i, j] != conjg(A[j, i]) then return false;
    }
    return true;
  }


  /* Return ``true`` if sparse matrix is symmetric. Supports CSR and COO arrays. */
  proc isSymmetric(A: [?D]) where isSparseArr(A): bool {
    if D.rank != 2 then
      compilerError("Rank is not 2");
    if !isSquare(A) then
      return false;

    for (i, j) in D {
      if A[i, j] != A[j, i] then return false;
    }
    return true;
  }


  //
  // Type helpers
  //


  pragma "no doc"
  /* Returns ``true`` if the array is dmapped to ``CS`` layout. */
  proc isCSArr(A: []) param { return isCSType(A.domain.dist.type); }

  pragma "no doc"
  /* Returns ``true`` if the domain is dmapped to ``CS`` layout. */
  proc isCSDom(D: domain) param { return isCSType(D.dist.type); }


} // submodule LinearAlgebra.Sparse


} // module LinearAlgebra
