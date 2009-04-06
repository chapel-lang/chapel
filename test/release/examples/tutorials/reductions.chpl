//
// An example use of some reductions.  An array is filled with random values
// and the locations of the maximum and minimum are found. The Euclidean norm
// of the Array's columns is computed using + reductions over slices of A.
// Finally, an && reduction is used to compute whether all values in A are
// greater than 0.25 and the results of the computations are printed.
//
use Random; // For random number generation

config const seed = 31415; // Random generation seed
config const size = 5;     // The size of each side of the array

var A: [1..size, 1..size] real; // The 2D work array

//
// Fill A with random real values between 0 and 1.
//
fillRandom(A, seed);

//
// Apply minloc and maxloc reductions. Capture the results into
// the maxVal, maxLoc, minVal, minLoc variables.
//
var (maxVal, maxLoc) = maxloc reduce (A, A.domain);
var (minVal, minLoc) = minloc reduce (A, A.domain);

//
// Compute Euclidean norms for each column using + reductions
//
var vecNorms = [j in 1..size] sqrt(+ reduce A(1..size, j)**2);

//
// Use the && reduction to compute if all values in A are greater than 0.25
//
var onlyBigValues = && reduce (A > 0.25);

//
// Print the results of the reductions
//
writeln("A is: "); writeln(A);
writeln("The maximum value in A is: A", maxLoc, " = ", maxVal);
writeln("The minimum value in A is: A", minLoc, " = ", minVal);
writeln("The difference is: ", maxVal - minVal);
writeln("The Euclidean norms of each column are: ", vecNorms);

if onlyBigValues then
  writeln("The values in A are all greater than 0.25");
else
  writeln("Some values in A are less than 0.25");
