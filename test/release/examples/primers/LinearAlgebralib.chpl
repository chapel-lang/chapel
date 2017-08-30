// LinearAlgebra


/*
  Example usage of the :mod:`LinearAlgebra` module in Chapel.

  Compiling
  ---------

  Because the linear algebra module uses the :mod:`BLAS` module, it requires
  a BLAS implementation to be available on the system and additional compiler
  flags to specify paths to that implementation. This is necessary for
  compiling this primer.

  See the :mod:`BLAS` documentation for further details.

*/

use LinearAlgebra;


/*
  Basics
  ------

  Linear Algebra Types
  ~~~~~~~~~~~~~~~~~~~~

  In Chapel, there are no ``Vector`` or ``Matrix`` types.
  Instead, vectors and matrices are represented as 1D and 2D native Chapel
  arrays, respectively. As a result, the terms *vector* and *1D array* are used
  interchangably, as well as *matrix* and *2D array*.

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
  Learn more about Chapel arrays in :mod:`ChapelArray` and
  :ref:`primers-arrays`.

*/

// Create a 3 x 5 matrix of reals, default-initialized to values of ``0.0``
var a = Matrix(3, 5);

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
writeln(a.rank); // 2

// Size
writeln(a.size); // 15

// Shape
writeln(a.shape); // (3, 5)

// Element type
writeln(a.eltType: string); // real(64)

// Type
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
 Array Factory Functions
 -----------------------

 The Linear Algebra module contains several factory functions for creating
 1D (vector) and 2D (matrix) arrays. These functions are purely a convenience
 that enables less verbose array declarations as well as an interface similar
 to other common linear algebra libraries. Below are some examples.

Note: The LinearAlgebra functions create arrays with ``0``-based indices by default.
  This default behavior differs from native Chapel array creation, which
  defaults to ``1``-based arrays.

*/


/*
Vectors
~~~~~~~

Shown below are a variety of way to create the same 5-element array of ``real``
type.

Note: The LinearAlgebra module represents *row-vectors* the same way as
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
// the first argument, as shown here:
var v6 = Vector(0.0, 0, 0, 0, 0);

/*
Matrices
~~~~~~~~

Shown below are a variety of way to create the same 3-by-3 array of ``real``
type.

Note: The base LinearAlgebra module strictly supports ``DefaultRectangular`` arrays
    for matrix operations. Other layouts (such as ``LayoutCS``) and distributions
    (such as ``BlockDist``) will be supported through LinearAlgebra submodules.

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


/*
  Operations
  ----------
 */

// Taking the transpose of a matrix:
var T1 = transpose(M0);
var T2 = M0.T; // syntactic sugar

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
writeln(yz);

// Matrix powers can be computed on square matrices with ``matPow()``. 
// Only integer powers are currently supported:
var MM4 = matPow(MM, 4);

// Cross products of 3-element vectors can be computed with ``cross()``:
var crossProduct = cross(y, y);

/*
Properties
----------

There are several functions for computing matrix properties like the ``trace``
 and ``norm``.
*/


/*
Structure
---------

There are several functions for inspecting and extracting matrix structure.
*/

/*

Open Issues
-----------


Note: For deep dive purposes only.

Indices
~~~~~~~

Should we switch to 1-based indices like the rest of Chapel arrays?

- Many linear algebra enthusiasts advocate for 1-based indices.
- Initially chosen due to using BLAS under the hood (which relies on 0-based arrays)
- Would improve interoperability between arrays created with native Chapel syntax and linear algebra syntax.

Matrix Operators
~~~~~~~~~~~~~~~~

Candidates for matrix-multiplication (``dot()``):

  - ``A @ B``
  - ``A . B``
  - ``A.dot(B)``

Candidates for matrix-division (``solve()``):

  - ``B \ A``
  - ``A.div(B)``

Candidates for matrix-exponentials (``matPow()``):

  - ``A.pow(n)``

Promotion-flattening
~~~~~~~~~~~~~~~~~~~~

It's really a pain:

*/

  var A = Matrix(3,5);
  var B = A + A; // This creates a 1-D array....

/*

Probably not going to be addressed by 1.16..

Once it is, we can deprecate ``matPlus`` and ``matMinus``.

Next Steps
----------

LAPACK Support
~~~~~~~~~~~~~~

- Will enable linear equations, eigenvalues, singular value decomposition, matrix decomposition, etc.
- For reference, see MatLab's library: https://www.mathworks.com/help/matlab/linear-algebra.html

Layouts (sparse)
~~~~~~~~~~~~~~~~

- Sparse LinearAlgebra to be supported via native Chapel.
- Sparsity described through layout (domain map).
- Limited functionality will be documented by what's present in the ``LinearAlgebra.Sparse`` submodule.

- Initially will support basic matrix operations  for ``LayoutCS``.


Distributions (CyclicDist)
~~~~~~~~~~~~~~~~~~~~~~~~~~

- To be supported via ScaLAPACK with ``CyclicDist``.
- Nikhil has done some exploratory work on this via Chapel + MPI project.


*/
