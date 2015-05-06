//
// Serial Monte Carlo Approximation of pi
//

//
// Use the standard random numbers module.
//
use Random;

//
// Declare command-line configuration constants for:
//   n: the number of random points to generate
//   seed: the seed for the random number generator
// As with all configuration constants and variables,
// these default values can be overrideen on the
// executable's command line (using --n and --seed).
//
config const n = 100000,
             seed = 589494289;

//
// Print simulation configuration.
//
writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);

//
// Construct a RandomStream object.  Since there are no concurrent
// accesses to this object by distinct tasks, parSafe can be set to
// false to avoid thread safety overheads.
// 
var rs = new RandomStream(seed, parSafe=false);

//
// Run the Monte Carlo simulation.  'count' is the number of random
// points that fall within the circle.
//
var count = 0;
for i in 1..n do
  count += (rs.getNext()**2 + rs.getNext()**2) <= 1.0;

//
// Output the approximation of pi.
//
writef("Approximation of pi = #.#######\n", count * 4.0 / n);

//
// Delete the Random Stream object.
//
delete rs;
