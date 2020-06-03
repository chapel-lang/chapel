use IO;
use MatrixOps;

// configuration variables:
//   inputfile: a string representing the file containing matrix data
//   block:     the block size to use for the block-LU factorization

config const inputfile = "blockLU-Mat.dat";
config var block = 2;

proc main() {

  // Open the input file, read in the array size, ensure it's square
  var infile = open(inputfile, iomode.r).reader();
  const m = infile.read(int),
        n = infile.read(int);
  if (m != n) then halt("Input matrix must be square");

  // Ensure the block size is appropriate.
  // It cannot be less than 1.  Reset to 1 if this happens.
  // It cannot exceed the size of n.  Reset to n if this happens.
  block = max(1, block);
  block = min(block, n);

  // declare a range A1D, 2D domain AD, and array (matrix) A
  const A1D = 1..n,
        AD = {A1D, A1D}; 
  var A: [AD] real;

  // Initialize A by reading values in from infile.
  for ij in AD do
    infile.read(A(ij));

  // close the input file
  infile.close();

  // declare the pivot permutation vector
  var piv: [A1D] int;  

  // print out the initial matrix
  writeln("Unfactored Matrix:\n", A, "\n");

  // perform the blocking
  blockLU(A, block, piv);

  // print out the results
  writeln("Factored Matrix:\n", A, "\n");
  writeln("Pivot Vector:\n", piv);
}
