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
config const epsilon = 0.0000001;
config const seed = 314159265;

//
// Declare a constant for pi that is "precise enough."
//
const pi = 3.14159265358979323846;

//
// Output simulation setup.
//
writef("Epsilon             = #.#################\n", epsilon);
writeln("Random number seed  = ", seed);

//
// Construct a RandomStream object.  Since there are no parallel
// accesses to this object, set parSafe to false to avoid locking
// overhead.
//
var rs = new RandomStream(seed, parSafe=false);

//
// Run the Monte Carlo simulation until the approximation of PI is
// within epsilon of the "precise enough" constant.
//
var n = 0, count = 0;
do {
  n += 1;
  count += rs.getNext()**2 + rs.getNext()**2 <= 1.0;
} while abs(count * 4.0 / n - pi) > epsilon;

//
// Delete the Random Stream object.
//
delete rs;

//
// Output the approximation of PI.
//
writef("Approximation of PI = #.###############\n", count * 4.0 / n);
writeln("Number of points    = ", count);
