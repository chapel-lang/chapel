use Random;

config const n = 100000,
             tasksPerLocale = here.numCores,
             seed = 314159265;

writeln("Number of locales   = ", numLocales);
writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);
writeln("Number of tasks/loc = ", tasksPerLocale);

//
//
// Allocate a distributed array of counts (one per locale) using
// LocaleSpace as the domain.
//
var counts: [LocaleSpace] int;

//

// Create a task per locale and then on that locale, capture the
// locale's unique ID, declare an array of counts for its tasks
// and compute what portion of the random points that locale owns.
// Create the specified number of tasks within each locale, have
// each construct a RandomStream object, run the Monte Carlo method,
// store its local result, and delete the RandomStream object.
// Compute a local reduction within each locale.
//
coforall loc in Locales {
  on loc {
    const locid = here.id;  // could also use loc.id
    var locCounts: [0..#tasksPerLocale] int;
    const nPerLoc = n/numLocales, globExtras = n%numLocales;
    const locN = nPerLoc + (locid < n%numLocales),
          locFirstPt = locid*nPerLoc + (locid < globExtras);

    coforall tid in 0..#tasksPerLocale {
      var rs = new RandomStream(seed, parSafe=false);
      const locNPerTask = locN/tasksPerLocale, extras = locN%tasksPerLocale;
      rs.skipToNth(2*(locFirstPt + 
                      tid*locNPerTask + (if tid < extras then tid else extras))
                   + 1);

      var count = 0;
      for i in 1..locNPerTask + (tid < extras) do
        count += (rs.getNext()**2 + rs.getNext()**2) <= 1.0;

      locCounts[tid] = count;

      delete rs;
    }
    counts[locid] = + reduce locCounts;
  }
}

//
// And then sum the counts across the locales.
//
const count = + reduce counts;


writeln("Approximation of pi = ", format("#.#######", count * 4.0 / n));
