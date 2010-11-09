use Random;

config const n = 100000,
       tasks = here.numCores,
       seed = 314159265;

writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);
writeln("Number of tasks     = ", tasks);

//
// Use a coforall to create the configured number of tasks.  Have each 
// task construct a RandomStream object with a unique seed, run the
// Monte Carlo method for its portion of the n total points, and delete
// the RandomStream object.  Store the resulting count in an array of
// counts with one element per task.
//
var counts: [0..#tasks] int;
coforall tid in 0..#tasks {
  var rs = new RandomStream(seed + tid*2, parSafe=false);

  var count = 0;
  for i in 1..(n/tasks) + (tid < n%tasks) do
    count += (rs.getNext()**2 + rs.getNext()**2) <= 1.0;

  counts[tid] = count;

  delete rs;
}

//
// Sum the counts across all the tasks using a reduction.
//
var count = + reduce counts;

writeln("Approximation of pi = ", format("#.#######", count * 4.0 / n));
