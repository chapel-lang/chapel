use Random;

config const n = 100000,
             tasksPerLocale = here.maxTaskPar,
             seed = 589494289;

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
// locale's unique ID and compute what portion of the random points
// that locale owns.
//
coforall loc in Locales {
  on loc {
    //
    // locid = the unique ID of this locale (0..numLocales-1)
    // nPerLoc = (the floor of) the number of points per locale
    // globExtras = the number of locales that need 1 extra point
    // locN = the number of points (of n) owned by this locale
    // locFirstPt = the first point owned by this locale
    //
    const locid = here.id;  // equivalently, could use loc.id
    const nPerLoc = n/numLocales, 
          globExtras = n%numLocales;
    const locN = nPerLoc + (locid < globExtras),
          locFirstPt = locid*nPerLoc
                       + (if (locid < globExtras) then locid else globExtras);

    //
    //  Declare an array of counts for the locale's tasks' results
    //
    var locCounts: [0..#tasksPerLocale] int;

    //
    // Create the specified number of tasks within each locale, have
    // each construct a RandomStream object, fast-forward to its first
    // point, run the Monte Carlo method, store its local result, and
    // delete the RandomStream object.
    //
    coforall tid in 0..#tasksPerLocale {
      var rs = new owned NPBRandomStream(real, seed, parSafe=false);
      const locNPerTask = locN/tasksPerLocale,
            extras = locN%tasksPerLocale;
      rs.skipToNth(2*(locFirstPt + 
                      tid*locNPerTask + (if tid < extras then tid else extras))
                   );

      var count = 0;
      for i in 1..locNPerTask + (tid < extras) do
        count += (rs.getNext()**2 + rs.getNext()**2) <= 1.0;

      locCounts[tid] = count;
    }

    //
    // Compute a local reduction within each locale.
    //
    counts[locid] = + reduce locCounts;
  }
}

//
// And then a global reduction across the locales.
//
const count = + reduce counts;


writef("Approximation of pi = %{#.#######}\n", count * 4.0 / n);
