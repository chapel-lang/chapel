// LAPACK

/*
  Example usage of the LAPACK module in Chapel.

  This particular file demonstrates how to use the gesv procedure with Chapel's
  arrays. The gesv procedure solves ``A*X = B`` for ``X`` given both ``A`` and
  ``B``, where ``A`` is a square matrix.

  Here, we create ``A`` and ``X``, then compute ``B`` with a matrix
  multiplication, and show that the result of the gesv procedure is the same as
  our synthesized ``X`` array (within a small margin of error for floating
  point).

  To compile a program with LAPACK, you may need to add some additional flags
  depending on how LAPACK was installed on your system:

  .. code-block:: sh

      chpl -I$PATH_TO_LAPACK_INCLUDE_DIR \
           -L$PATH_TO_LAPACK_BINARIES \
           -lgfortran

  See the :mod:`LAPACK` module documentation for more information on compiling.
*/

// Start by using the :mod:`LAPACK` module to gain access to the
// :proc:`~LAPACK.gesv` function.  The ``Random`` module will be used to fill
// arrays with random values.
use LAPACK;
use Random;

/*
  Here we set up several config consts that represent, in order:
    - ``N``, the dimension for the square array ``A``
    - ``K``, the second dimension for arrays ``X`` and ``B``
    - ``epsilon``, the margin of error for success
    - ``seed``, a seed for random number generation
*/
config const N = 2;
config const K = 1;
config const epsilon = 1e-13;
config const seed = 41;

//
// Create the arrays ``A``, ``X``, and ``B``. Fill ``A`` and ``X`` with random
// values.
//

var A : [1..N, 1..N] real;
fillRandom(A, seed);

var X : [1..N, 1..K] real;
fillRandom(X, seed);

var B : [1..N, 1..K] real;

// Matrix multiply ``A*X``, store result in ``B``
for i in 1..N do
  for j in 1..K do
    for k in 1..N do
      B[i,j] += A[i,k] * X[k,j];

writeln("Matrix A:\n", A, "\n");
writeln("Matrix X:\n", X, "\n");
writeln("Matrix B:\n", B, "\n");

// Copy original arrays into temporary arrays.

// Input and work array. Becomes garbage.
var WorkA = A;

// Input and output. Becomes result of solution (``X``)
var WorkBX = B;

// Output array. Stores pivot indices
var ipiv : [1..N] c_int;

//
// Call the :proc:`~LAPACK.gesv` function to solve for ``X``. Note that Chapel arrays are
// row-major order by default.
//
var info = gesv(lapack_memory_order.row_major, WorkA, ipiv, WorkBX);

// LAPACK returns an error code to indicate a failure.
if info != 0 {
  writeln("There was an error!");
  if info < 0 {
    writeln("Argument ", -info, " was incorrect.");
  } else {
    writeln("The matrix A is a singular matrix. U", (info,info), " is zero");
  }
}

writeln("gesv result for X:\n", WorkBX, "\n");

//
// The arrays may not be identical due to floating point errors. Use a small
// value as a margin of error to measure success.
//
const closeEnough = && reduce [d in (WorkBX - X)] abs(d) < epsilon;

if closeEnough then
  writeln("SUCCESS");
else
  writeln("FAILURE");
