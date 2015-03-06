//
// Reductions Primer
//
// This primer includes example reductions.
//
// An array is filled with random values and the locations of the
// maximum and minimum are found. The Euclidean norm of the Array's
// columns is computed using + reductions over slices of A.  Finally,
// an && reduction is used to compute whether all values in A are
// greater than 0.25 and the results of the computations are printed.
//

//
// Note: example usage of the standard module Random can be found in
// the primer randomNumbers.chpl, located in the current directory.
//
use Random; // For random number generation

config const seed = 31415; // Random generation seed
config const size = 10;    // The size of each side of the array

var A: [1..size, 1..size] real; // The 2D work array

//
// Fill A with random real values between 0 and 1.
//
fillRandom(A, seed);
writeln("A is: "); writeln(A);
writeln();

//
// Find the average value of the array, by summing over its elements
// and dividing by the number of elements it contains.
//
var eltAvg = (+ reduce A) / size**2;
writeln("The average element of A has the value ", eltAvg);
writeln();

//
// We can find the 1-norm of A by summing over the absolute value of the
// elements.
//
// The expression "abs(A)" creates a new matrix which contains in each of its
// elements the absolute value of the corresponding element in A.
// The "+ reduce" clause just sums these up.
// 
var oneNorm = + reduce abs(A);
writeln("The 1-norm of A is ", oneNorm);
writeln();

//
// The Frobenius norm is the square root of the sum over all elements of their
// respective squares.
//
// The expression below can be broken down thus:
//  1) The Frobenius norm is the square root of sum quantity.
//  2) The quantity is the sum over all elements of a matrix.
//  3) The matrix is the promotion of A by "**2".
//     That is, a matrix each of whose elements the square of the corresponding
//       element in A.
//
var frobNorm = sqrt(+ reduce A**2);
writeln("The Frobenius norm of A is ", frobNorm);
writeln();

//
// Apply minloc and maxloc reductions. Capture the results into
// the maxVal, maxLoc, minVal, minLoc variables.
//
// maxloc and minloc define reductions which expect a tuple of arrays
// that can be iterated using zippered iteration 
// (meaning that the iterator returns one value from each array in a 2-tuple).
//
// The reduce operator combines each successive element with a running result
// which is also a tuple.  The first element of the result tuple is the running
// maximum (or minimum); the second element is its index (i.e. location).
//
var (maxVal, maxLoc) = maxloc reduce zip(A, A.domain);
var (minVal, minLoc) = minloc reduce zip(A, A.domain);
writeln("The maximum value in A is: A", maxLoc, " = ", maxVal);
writeln("The minimum value in A is: A", minLoc, " = ", minVal);
writeln("The difference is: ", maxVal - minVal);
writeln();

//
// Compute Euclidean norms for each column using + reductions.
//
// Breaking down the statement below:
//  1) vecNorms is a 1-D array containing size elements (indexed by 1..size).
//  2) The elements of vecNorms are the square-root of some quantity.
//  3) The quantity is the sum over all of the elements of some vector.
//  4) The vector consists of the promotion of the j-th column of A by **2.
//     (That is, each element of that column vector is squared.)
//
var vecNorms = [j in 1..size] sqrt(+ reduce A(1..size, j)**2);
writeln("The Euclidean norm of each column is: ", vecNorms);
writeln();

//
// Use the && reduction to compute if all values in A are greater than 0.25.
//
// The parenthesized value is the promotion of A by "> 0.25".  This yields
// an array of the same size as A, containing boolean values that are true
// if the corresponding element in A exceeds 0.25 and false otherwise.
//
// The clause "&& reduce" applies the Boolean AND operator to each element in
// the array, and accumulates the result.
//
var onlyBigValues = && reduce (A > 0.25);
if onlyBigValues then
  writeln("The values in A are all greater than 0.25");
else
  writeln("Some values in A are less than 0.25");
writeln();
