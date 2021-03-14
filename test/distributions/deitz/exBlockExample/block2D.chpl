/*
 * Block Distribution Example
 *
 * This example demonstrates a simple use of the Block distribution to
 * distribute a 2D array of integers across multiple locales.  Try it
 * with 4 locales by running it with the command-line argument -nl 4.
 * Note that you will need to set CHPL_COMM to gasnet.  Then try it
 * varying the numbers of locales and by changing the problem size
 * (--n=16).
 *
 * WARNING: Support for distributions is very much a work-in-progress
 * and many features that work with non-distributed arrays and domains
 * will not work with distributed arrays and domains.  Think of this
 * example as a preview.
 *
 */

//
// Standard module BlockDist defines the Block distribution.
//
use BlockDist;

//
// Size of each dimension of our domain.
//
config const n = 8;

//
// Declare and initialize an instance of the Block distribution Dist,
// a distributed domain Dom, and a distributed array A.  By default,
// the Block distribution distributes the domain across all locales.
//
var Dist = new dmap(new Block({1..n, 1..n}));
var Dom: domain(2) dmapped Dist = {1..n, 1..n};
var A: [Dom] int;

//
// Output the domain and array.  They print just like non-distributed
// domains and arrays.
//
writeln("Domain");
writeln(Dom);
writeln();

writeln("Array");
writeln(A);
writeln();

//
// Output the locale ID numbers for each array element, determining
// this information by applying ".locale" to each array element.
//
writeln("Distribution Map");
for i in 1..n {
  for j in 1..n {
    write(A(i,j).locale.id, " ");
  }
  writeln();
}
writeln();

//
// Loop over the array using a serial for-loop and assign each element
// and increasing number.
//
var j = 1;
for a in A {
  a = j;
  j += 1;
}
writeln("Initialized array");
writeln(A);
writeln();

//
// In parallel, subtract one from each element of the array.
//
forall i in Dom do {
  A(i) = A(i) - 1;
}
writeln("Subtracted 1 via parallel iteration over the domain");
writeln(A);
writeln();

//
// In parallel, iterate over the array and set each element to 0.
//
forall a in A do {
  a = 0;
}
writeln("Zeroed via parallel iteration over the array");
writeln(A);
