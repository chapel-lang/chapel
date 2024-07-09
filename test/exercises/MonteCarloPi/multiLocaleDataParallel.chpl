use Random, BlockDist;  // Add a use for the standard block distribution

config const n = 100000,
             seed = 589494289;

writeln("Number of locales     = ", numLocales);
writeln("Number of points      = ", n);
writeln("Random number seed    = ", seed);
writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);

var rs = new randomStream(real, seed);

//
// Distribute the domain representing the random points in a 
// Blocked manner over the target locale set.
//
var D = {1..n} dmapped new blockDist({1..n});

//
// The computation is identical to the non-distributed case.
//
var count = + reduce [(x,y) in zip(rs.next(D), rs.next(D))]
                       (x**2 + y**2) <= 1.0;

writef("Approximation of pi   = %{#.#######}\n", count * 4.0 / n);
