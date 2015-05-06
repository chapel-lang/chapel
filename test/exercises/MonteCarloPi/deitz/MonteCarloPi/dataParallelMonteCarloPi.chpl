//
// Data-Parallel Monte Carlo Approximation of PI
//

//
// Use the standard random numbers module.
//
use Random;

//
// Declare command-line configuration constants for:
//   n: the number of random points to generate
//   seed: the random number generator seed
//
config const n = 100000;
config const seed = 314159265;

//
// Output simulation setup.
//
writeln("Number of points      = ", n);
writeln("Random number seed    = ", seed);
writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);

//
// Construct a RandomStream object.  Since there are no parallel
// accesses to this object, set parSafe to false to avoid locking
// overhead.
//
var rs = new RandomStream(seed, parSafe=false);

//
// Create a domain over the number of random points to generate.
// Since there is no array, the memory required is O(1) in terms of
// the number of points.
//
var D = {1..n};

//
// Run the Monte Carlo simulation using a data parallel reduction to
// compute the count.  The reduction is over a forall loop that
// zippers two iterators over the RandomStream object (the second call
// to iterate starts at the point after the first iterator finishes).
//
var count = + reduce [(x,y) in zip(rs.iterate(D),rs.iterate(D))] x**2+y**2 <= 1.0;

//
// Delete the Random Stream object.
//
delete rs;

//
// Output the approximation of PI.
//
writef("Approximation of PI   = #.#######\n", count * 4.0 / n);
