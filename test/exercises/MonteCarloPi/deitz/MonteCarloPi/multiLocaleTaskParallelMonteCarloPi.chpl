//
// Multi-Locale Task-Parallel Monte Carlo Approximation of PI
//

//
// Use the standard random numbers module.
//
use Random;

//
// Declare command-line configuration constants for:
//   n: the number of random points to generate
//   seed: the random number generator seed
//   tasks: the number of tasks to parallelize the computation (per locale)
//
config const n = 100000;
config const tasks = here.maxTaskPar;
config const seed = 314159265;

//
// Output simulation setup.
//
writeln("Number of locales   = ", numLocales);
writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);
writeln("Number of tasks     = ", tasks, " (per locale)");

//
// On each locale, for each task on that locale, construct a
// RandomStream object, run the Monte Carlo simulation, and delete the
// RandomStream object.  Store the resulting count in an array of
// counts, one element per task per locale.  Since there are no
// parallel accesses to the RandomStream object (each task has its own
// object), set parSafe to false to avoid locking overhead.
//
var counts: [LocaleSpace] [1..tasks] int;
coforall loc in Locales do on loc {
  var myN = (loc.id+1)*n/numLocales - (loc.id)*n/numLocales;
  coforall task in 1..tasks {
    var rs = new NPBRandomStream(seed + loc.id*tasks*2 + task*2, parSafe=false);
    var count = 0;
    for i in (task-1)*myN/tasks+1..task*myN/tasks do
      count += rs.getNext()**2 + rs.getNext()**2 <= 1.0;
    delete rs;
    counts[loc.id][task] = count;
  }
}

//
// Sum the counts across all the tasks.
//
var count = 0;
for loc in Locales do
  for task in 1..tasks do
    count += counts[loc.id][task];

//
// Output the approximation of PI.
//
writef("Approximation of PI = %{#.#######}\n", count * 4.0 / n);
