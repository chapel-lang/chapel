use Random;

config const n = 100000,
       tasks = here.maxTaskPar,
       seed = 589494289;

writeln("Number of points    = ", n);
writeln("Random number seed  = ", seed);
writeln("Number of tasks     = ", tasks);

//
// Use a coforall to create the configured number of tasks.  Have each 
// task construct a RandomStream object with the same seed, fast forward
// to the task's unique point in the stream (in order to avoid using
// the same random numbers redundantly while also getting the same
// answer as the serial version), run the Monte Carlo method for its
// portion of the n total points, and delete the RandomStream object.
// Store the resulting count in an array of counts with one element per
// task.
//
var counts: [0..#tasks] int;
coforall tid in 0..#tasks {
  var rs = new owned NPBRandomStream(real, seed, parSafe=false);
  const nPerTask = n/tasks,
        extras = n%tasks;
  rs.skipToNth(2*(tid*nPerTask + (if tid < extras then tid else extras)));

  var count = 0;
  for i in 1..nPerTask + (tid < extras) do
    count += (rs.getNext()**2 + rs.getNext()**2) <= 1.0;

  counts[tid] = count;
}

//
// Sum the counts across all the tasks using a reduction.
//
var count = + reduce counts;

writef("Approximation of pi = %{#.#######}\n", count * 4.0 / n);
