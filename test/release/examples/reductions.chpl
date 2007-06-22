//
// An example use of the maxloc and minloc reductions.  An array is filled
// with random values and the locations of the maximum and minum are found.
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
// Print array and the results of the reductions
//
writeln("A is:"); writeln(A);
writeln("The maximum value in A is: A", maxLoc, " = ", maxVal);
writeln("The minimum value in A is: A", minLoc, " = ", minVal);
writeln("The difference is: ", maxVal - minVal);
