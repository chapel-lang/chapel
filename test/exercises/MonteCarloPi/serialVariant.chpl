use Random;

//
// Replace config const n with epsilon since we're now computing n
//
config const epsilon = 0.0000001,
             seed = 589494289;

//
// store a "correct" solution for pi to compare against
//
const pi = 3.14159265358979323846;

writef("Epsilon             = %{#.#################}\n", epsilon);
writeln("Random number seed  = ", seed);

var rs = new RandomStream(seed, parSafe=false);

//
// keep track of the number of points we generate before converging
//
var n = 0, count = 0;
do {
  n += 1;
  count += (rs.getNext()**2 + rs.getNext()**2) <= 1.0;
} while abs((count * 4.0 / n) - pi) > epsilon;

writef("Approximation of pi = %{#.###############}\n", count * 4.0 / n);
writeln("Number of points    = ", count);

delete rs;
