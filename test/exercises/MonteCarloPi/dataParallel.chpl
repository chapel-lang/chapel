use Random;

config const n = 100000,
             seed = 589494289;

writeln("Number of points      = ", n);
writeln("Random number seed    = ", seed);
writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);

var rs = new RandomStream(seed, parSafe=false);

//
// Create a domain over the number of random points to generate.
// Since there is no array, the memory required is O(1) regardless
// of the number of points.
//
var D = {1..n};

//
// Run the Monte Carlo method using a data parallel reduction to
// compute the count.  The reduction is over a forall loop that
// zippers two consecutive iterations over the RandomStream object.
//
var count = + reduce [(x,y) in zip(rs.iterate(D), rs.iterate(D))]
                       (x**2 + y**2) <= 1.0;

writef("Approximation of pi   = #.#######\n", count * 4.0 / n);

delete rs;
