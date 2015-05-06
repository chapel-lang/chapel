use Random, BlockDist;  // Add a use for the standard block distribution

config const n = 100000,
             seed = 589494289;

writeln("Number of locales     = ", numLocales);
writeln("Number of points      = ", n);
writeln("Random number seed    = ", seed);
writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);

var rs = new RandomStream(seed, parSafe=false);

//
// Distribute the domain representing the random points in a 
// Blocked manner over the target locale set.
//
var D = {1..n} dmapped Block({1..n});

//
// The computation is identical to the non-distributed case.
//
var count = + reduce [(x,y) in zip(rs.iterate(D), rs.iterate(D))]
                       (x**2 + y**2) <= 1.0;

writef("Approximation of pi   = #.#######\n", count * 4.0 / n);

delete rs;

