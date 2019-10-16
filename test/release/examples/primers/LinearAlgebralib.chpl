// LinearAlgebra


/*
  Example usage of the :mod:`LinearAlgebra` module in Chapel.

  .. contents:: Table of Contents

*/
use LinearAlgebra;

/*

  Compiling with Linear Algebra
  -----------------------------

  Some of the linear algebra module procedures rely on the :mod:`BLAS` and
  :mod:`LAPACK` modules.  If using routines that rely on these modules,
  be sure to have a BLAS and LAPACK implementation available on your system.
  See the :mod:`BLAS` and :mod:`LAPACK` documentation for further details.

  To explicitly opt out of using the :mod:`BLAS` and :mod:`LAPACK` procedures,
  compile your Chapel program with the flags:

  .. code-block:: bash

    chpl --set blasImpl=none --set lapackImpl=none myProgram.chpl

  This will result in a cleaner compiler error when using a procedure that is
  only available with :mod:`BLAS` or :mod:`LAPACK`.

*/

/*
  Basics
  ------

  Linear Algebra Types
  ~~~~~~~~~~~~~~~~~~~~

  In Chapel, there are no ``Vector`` or ``Matrix`` types.
  Instead, vectors and matrices are represented as 1D and 2D native Chapel
  arrays, respectively. As a result, the terms *vector* and *1D array* are used
  interchangeably, as well as *matrix* and *2D array*.

  The following example demonstrates this:

*/

var vector1: [0..#5] int;             // Using Chapel array syntax
var vector2 = Vector(5, eltType=int); // Using LinearAlgebra Vector function
assert(vector1.type == vector2.type); // ... produces same result

var matrix1: [0..#3, 0..#5] int;         // Using Chapel array syntax
var matrix2 = Matrix(3, 5, eltType=int); // Using LinearAlgebra Matrix function
assert(matrix1.type == matrix2.type);    // ... produces same result


/*
  Array operations
  ~~~~~~~~~~~~~~~~

  This section demonstrates some of the functionality built into Chapel
  arrays that can be useful in a linear algebra context. None of the operations
  in this section require usage of :mod:`LinearAlgebra`.
  Learn more about Chapel arrays in the :mod:`ChapelArray` documentation and
  the :ref:`Array Primer <primers-arrays>`.

*/

// Create a 3 x 5 matrix of reals, default-initialized to values of ``0.0``
var a: [0..#3, 0..#5] real; // Equivalent to: var a = Matrix(3, 5);

// Set all elements to ``1.0``
a = 1.0;

// Print matrix
writeln(a);
  // Prints:
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0

// Number of dimensions
a.rank; // 2

// Size
a.size; // 15

// Shape
a.shape; // (3, 5)

// Element type
writeln(a.eltType: string); // real(64)

// Array type in format of:
//   ``[domain(rank, index-type, stridable)] element-type``
writeln(a.type: string); // [domain(2,int(64),false)] real(64)

// Element-wise addition (and subtraction)
a = a + 1; // or, a += 1
writeln(a);
  // Prints:
  //   2.0 2.0 2.0
  //   2.0 2.0 2.0
  //   2.0 2.0 2.0
  //   2.0 2.0 2.0
  //   2.0 2.0 2.0

// Element-wise multiplication
a = 2 * a;
writeln(a);
  // Prints:
  //   4.0 4.0 4.0
  //   4.0 4.0 4.0
  //   4.0 4.0 4.0
  //   4.0 4.0 4.0
  //   4.0 4.0 4.0

// Element-wise division
a = a / 4.0;
writeln(a);
  // Prints:
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0

// Array slicing
a[0, ..] = 0.0; // Sets first row to 0.0
writeln(a);
  // Prints:
  //   0.0 0.0 0.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0

a[.., 1] = 3.0; // Sets second column to 3.0
writeln(a);
  // Prints:
  //   0.0 3.0 0.0
  //   1.0 3.0 1.0
  //   1.0 3.0 1.0
  //   1.0 3.0 1.0
  //   1.0 3.0 1.0


/*
  Factory Functions
  -----------------

  The Linear Algebra module contains several factory functions for creating
  1D (vector) and 2D (matrix) arrays. These functions are purely a convenience
  that provides a less verbose syntax for array declarations as well as an
  interface similar to other common linear algebra libraries.
  Below are some examples. See the documentation starting at
  :proc:`LinearAlgebra.Vector` for a comprehensive list of the available
  factory functions.

*/


/*
  Vectors
  ~~~~~~~

  Shown below are a variety of way to create the same 5-element array of
  ``real`` type.

  .. note::

      The LinearAlgebra module represents *row-vectors* the same way as
      *column-vectors*. They are both 1D arrays.

*/

// Creating 1D arrays (vectors):
var v0: [0..#5] real;     // Chapel array syntax
writeln(v0);
  // Prints:
  // 0.0 0.0 0.0 0.0 0.0
var v1 = Vector(5);       // from a length
var v2 = Vector(0..#5);   // from a range
var v3 = Vector({0..#5}); // from a domain
var v4 = Vector([0, 0, 0, 0, 0], eltType=real); // from a 1D array
var v5 = Vector(0, 0, 0, 0, 0, eltType=real);   // from a list of values as arguments

// If the ``eltType`` is omitted for the argument case, it will be inferred from
// the first argument. For example, this creates a vector of reals:
var v6 = Vector(0.0, 0, 0, 0, 0);
assert(v6.eltType == real);

/*
  Matrices
  ~~~~~~~~

  Shown below are a variety of way to create the same 3 x 3 array of ``real``
  type.

  .. note::

      The base LinearAlgebra module strictly supports ``DefaultRectangular``
      arrays for matrix operations. Other layouts (such as ``LayoutCS``) and
      distributions (such as ``BlockDist``) will be supported through
      LinearAlgebra submodules.

*/

// Creating 2D arrays (matrices):
var M0: [0..#3, 0..#3] real; // Chapel array syntax
writeln(M0);
  // Prints:
  //   0.0 0.0 0.0
  //   0.0 0.0 0.0
  //   0.0 0.0 0.0
var M1 = Matrix(3);                 // from a length
var M2 = Matrix(3, 3);              // from a length and width
var M3 = Matrix(0..#3);             // from 1 range
var M4 = Matrix(0..#3, 0..#3);      // from 2 ranges
var M5 = Matrix({0..#3, 0..#3});    // from a 2D domain
var M6 = Matrix(M5);                // from a 2D array
var M7 = Matrix([0,0,0],[0,0,0],[0,0,0], eltType=real); // from a list of 1D arrays as arguments

// Creating identity matrices:
var I1 = eye(3); // From a length
writeln(I1);
  // Prints:
  //   1.0 0.0 0.0
  //   0.0 1.0 0.0
  //   0.0 0.0 1.0
var I2 = eye(3,3); // From a length and width
var I3 = eye({0..#3, 0..#3}); // from a 2D domain

// Creating diagonal matrix from a vector:
var vec = Vector(1, 2, 3, eltType=real);
var diagMatrix = diag(vec);
writeln(diagMatrix);
  // Prints:
  //   1.0 0.0 0.0
  //   0.0 2.0 0.0
  //   0.0 0.0 3.0


/*
  Operations
  ----------
*/

// Setup some matrices

{ // Operations scope

var A = Matrix(3,5),
    B = Matrix(3,5);

A = 1.0;
B = 2.0;


// Element-wise addition
var ApB = A + B;

// Element-wise subtraction
var AmB = A - B;

// Element-wise multiplication
var AtB = A * B;

// Element-wise division
var AdB = A / B;

// Taking the transpose of a matrix:
var M0T = transpose(M0);
M0T = M0.T; // short-hand syntax

// Taking the transpose of a vector simply returns the vector because row and
// column vectors are equivalent:
var t1 = v0.T;
assert(t1.type == v0.type);
assert(t1 == v0);

// Creating a matrix and some vectors to use:
var X = Matrix(3, 5),
    y = Vector(3),
    z = Vector(5);

X = 1;
X -= eye(3,5);
writeln(X);
  // Prints:
  //   0.0 1.0 1.0
  //   1.0 0.0 1.0
  //   1.0 1.0 0.0
  //   1.0 1.0 1.0
  //   1.0 1.0 1.0
y = 2;
z = 1;

// Matrix-matrix multiplication is computed by ``dot()``:
var MM = dot(X, X.T);

// ... as well as matrix-vector multiplication:
var Mv = dot(X, z);

// ... or vector-matrix multiplication:
var vM = dot(y, X);

// ... and vector-vector multiplication:
var vv = dot(y, y);

// Because row and column vectors are indistinguishable, an ``dot()`` always
// computes the inner product of 2 vectors.

// To compute an outer product, one must explicitly call ``outer()``:
var yz = outer(y, z);
writeln('outer:', yz);

// Matrix powers can be computed on square matrices with ``matPow()``.
// Only integer powers are currently supported:
var MM4 = matPow(MM, 4);

// Cross products of 3-element vectors can be computed with ``cross()``:
var crossProduct = cross(y, y);

} // Operations scope

/*
  Properties
  ----------

  There are several functions for computing matrix properties.

  Note that ``norm`` procedures are documented in the :mod:`Norm` module.

*/

// Trace of a matrix
var tr = trace(diagMatrix); // 6.0

// Norm of a vector
var N1 = norm(vec); // 3.74166

// Norm of a matrix
var N2 = norm(diagMatrix); // 3.74166

/*
  Structure
  ---------

  There are several functions for inspecting and extracting matrix structure.

  Below are a few examples. For the comprehensive list of available structural
  functions, see the documentation starting at :proc:`LinearAlgebra.diag`.

*/

// Extract diagonal of matrix
var diagVec = diag(diagMatrix); // 1.0 2.0 3.0

// Confirm that a matrix is diagonal
writeln(isDiag(diagMatrix)); // true

// Extract upper triangle of matrix
var onesMatrix = Matrix(5,5);
onesMatrix = 1.0;
writeln(onesMatrix);
  // Prints:
  //   1.0 1.0 1.0 1.0 1.0
  //   1.0 1.0 1.0 1.0 1.0
  //   1.0 1.0 1.0 1.0 1.0
  //   1.0 1.0 1.0 1.0 1.0
  //   1.0 1.0 1.0 1.0 1.0
var upper = triu(onesMatrix, k=0); // k=0 includes diagonal
writeln(upper);
  // Prints:
  //   1.0 1.0 1.0 1.0 1.0
  //   0.0 1.0 1.0 1.0 1.0
  //   0.0 0.0 1.0 1.0 1.0
  //   0.0 0.0 0.0 1.0 1.0
  //   0.0 0.0 0.0 0.0 1.0

// Confirm that a matrix is upper triangular
writeln(isTriu(upper));         // true
writeln(isTriu(upper, k=1));    // false (k=1 does not include diagonal)

/*

  LinearAlgebra.Sparse
  --------------------

  The :mod:`Sparse` submodule supports operations on sparse
  matrices.

  When using the :mod:`LinearAlgebra` module, this submodule can be accessed
  like so:
*/
{
  var A = Sparse.CSRDomain(100, 100);
}

/*
  Alternatively, one can expose all the submodule symbols with an additional
  use statement:
*/

use LinearAlgebra.Sparse;

// Now we can do:
{
  var A = CSRDomain(100, 100);
}

/*

  Supported Sparse Layouts
  ~~~~~~~~~~~~~~~~~~~~~~~~

  Currently only CSR matrices are supported. A CSR matrix is
  composed of a 2D array that is domain-mapped to a ``CS()`` layout, from
  the :mod:`LayoutCS` layout module.

  For example, the following CSR matrix (``CS(compressRows=true)``)
  **is supported** by this submodule:
*/

{
  use LayoutCS;
  const parentDom = {1..100, 1..100};
  var csrDom: sparse subdomain(parentDom) dmapped CS();
  var csrMatrix: [csrDom] real; // Supported by LinearAlgebra.Sparse
}

/*
   And as a counter-example, the following COO matrix (``DefaultSparse``)
   **is not supported** by this module:
*/

{
  const parentDom = {1..100, 1..100};
  var cooDom: sparse subdomain(parentDom);
  var cooMatrix: [cooDom] real; // NOT supported by LinearAlgebra.Sparse
}

/*
  For more information about working with sparse arrays in Chapel, see the
  :ref:`Sparse Primer <primers-sparse>`.
*/

/*
  Factory Functions
  ~~~~~~~~~~~~~~~~~

  The :mod:`Sparse` submodule provides
  factory functions for both sparse domains and sparse matrices.

  In Chapel, sparse domains can only be modified directly, so it is important
  to maintain access to both
  the sparse array and sparse domain.

  Consider the following example:

*/
{
  // Bad example of LinearAlgebra.Sparse usage:
  var A = CSRMatrix({1..100, 1..100}); // Create a CSR matrix from dense domain
  writeln(A.domain); // empty sparse domain, with no way to add elements..
}

/*
  The above sparse matrix is initialized with an empty domain, and the user
  has no way of accessing that domain. Therefore, it is not possible to add
  indices to the sparse domain.

  Instead, users should interact with both the sparse domain and sparse array:
*/
{
  // Good example of LinearAlgebra.Sparse usage:
  var D = CSRDomain(100, 100); // empty sparse domain
  var A = CSRMatrix(D); // Create a CSR matrix from a sparse domain
  D += [(10,20), (30, 24), (64, 42)]; // Add some indices to the sparse domain
}

/*
  Below are a list of the available factory functions:
*/

{ // Factory functions scope
// Creating CSR domains & arrays in Chapel without ``LinearAlgebra.Sparse``:
use LayoutCS;
var parentDom = {0..#3, 0..#3}; // Parent domain
var csrDom: sparse subdomain(parentDom) dmapped CS(); // CSR domain
var csrArr: [csrDom] real; // CSR array

// Adding indices to a sparse domain:
csrDom += (0,0);
csrDom += [(1,1), (2,2)];

// Creating CSR domains with ``LinearAlgebra.Sparse``:
var D1 = CSRDomain(3);                 // from a length
var D2 = CSRDomain(3, 3);              // from a length and width
var D3 = CSRDomain(0..#3);             // from 1 range
var D4 = CSRDomain(0..#3, 0..#3);      // from 2 ranges
var D5 = CSRDomain({0..#3, 0..#3});    // from a 2D domain

// Creating CSR matrices:
var M1 = CSRMatrix(D1);             // from a CSR domain
var M2 = CSRMatrix({0..#3, 0..#3}); // from a dense domain
var M3 = CSRMatrix(M1);             // From a CSR matrix
const I = eye(3,3);
var M4 = CSRMatrix(I);              // From a dense matrix
                                    // Indices holding non-zero elements are
                                    // added to M4's sparse domain


} // Factory functions scope

/*
  Operations
  ~~~~~~~~~~

  Below is a list of the currently supported operations.

  .. note::

    For sparse matrices, the element-wise operations such as ``+``, ``-``, ``*``, and ``/``
    are :ref:`promoted <ug-promotion>`, and will result in an error
    if there is a sparse domain mismatch.
    To avoid this, it is preferred to use the element-wise methods:
    ``.plus()``, ``.minus()``, ``.times()``, and ``.elementDiv()``.

*/

// Setup some sparse domains and arrays
{ // Operations scope

var Adom = CSRDomain(100, 100),
    Bdom = CSRDomain(100, 100),
    Cdom = CSRDomain(100, 100);

Adom += (1,1);
Bdom += (2,2);
Cdom += [(1,1), (2,2)];

var A = CSRMatrix(Adom),
    B = CSRMatrix(Bdom),
    C = CSRMatrix(Cdom);

A = 1.0;
B = 2.0;
C = 4.0;

// Element-wise addition.
A.plus(B); // 1.0, 2.0
A.plus(C); // 5.0, 4.0

// Element-wise subtraction
A.minus(B); // 1.0, -2.0

// Element-wise multiplication
A.times(B); // 0.0, 0.0
A.times(C); // 4.0, 4.0

// Element-wise division

A.elementDiv(C); // 0.25, 0.0
B.elementDiv(C); // 0.0, 0.5

// Matrix-matrix multiplication
C.dot(C); // 16.0, 16.0

// Matrix-vector multiplication
var a = Vector(100);
a = 2.0;
C.dot(a);

// Matrix-scalar multiplication
C.dot(2.0); // equivalent to C.times(2.0);

// Matrix transpose
transpose(C);
C.T; // short-hand syntax

} // Operations scope
