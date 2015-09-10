//
// Serial Monte Carlo Approximation of PI
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
writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);

//
// Construct a RandomStream object.  Since there are no parallel
// accesses to this object, set parSafe to false to avoid locking
// overhead.
//
var rs = new RandomStream(seed, parSafe=false);

//
// Run the Monte Carlo simulation.
//
var count = 0;
for i in 1..n do
  count += rs.getNext()**2 + rs.getNext()**2 <= 1.0;

//
// Delete the Random Stream object.
//
delete rs;

//
// Output the approximation of PI.
//
writef("Approximation of PI = %{#.#######}\n", count * 4.0 / n);
