/* This example demonstrates how one would use a LAPACK procedure in Chapel
 * using Chapel arrays.
 * The procedure used is the gesv procedure which solves A * X = B for X, 
 * given both A and B, where A is a square matrix.
 * 
 * Here, we create A and X, then compute B with a matrix multiplication, and
 * show that the result the gesv of the procedure give the correct output.
 */

use LAPACK;

var N = 2;
var K = 1;

// Create our 2-D arrays for out matrices 
// Our synthesized matrix A
var A : [1..N,1..N] real(64); 
A[1,1..2] = [ 1, 2 ]; // first row of matrix
A[2,1..2] = [ 3, 4 ]; // second row of matrix

// Our synthesized matrix X
var X : [1..N,1..K] real(64);
X[1,1] = 2;
X[2,1] = 1;

// Our computed matrix B
var B : [1..N,1..K] real(64);

// Matrix multiply Ax = B
for i in A.domain.dim(1) do
  for j in X.domain.dim(2) do
    for k in A.domain.dim(1) do
      B[i,j] += A[i,k] * X[k, j];


writeln( "A:\n", A, "\n\nX:\n", X, "\n\nB:\n", B , "\n==============");

// copy our results into temporary work/result arrays
// since LAPACK walks over them and stores the result in them
var work_A = A; // Input and work array. Becomes garbage. 
var work_B = B; // Input and output. Becomes result of solution (X)

var ipiv : [1..N] c_int; // Output array. Stores pivot indices

// Call the gesv function 
// Our matrices are in row_major order
var info = gesv( lapack_memory_order.row_major, work_A, ipiv, work_B );

// LAPACK and LAPACK return error codes when they fail.
if info != 0 {
  writeln( "There was an error!" );
  if info < 0 {
    writeln( "Argument ", -info, " was incorrect." );
  } else {
    writeln( "The matrix A is a sigular matrix. U", (info,info), " is zero" );
  }
}

writeln( "X:\n", work_B, "\n" ); 
// We can see that work_B is the same as X
// Unfortunately, there will be some small amount of error
writeln( "X == result: ", && reduce (work_B == X), "\n");

// We can approximate with a small epsilon (lets try 1e-15)
var err = 1e-15; // one ten-quadrillionth
var closeEnough = && reduce [ (b,x) in zip( work_B, X ) ] abs( b - x ) < err;
writeln( "X == result ±", err, ": ", closeEnough);
