config const numTasks = here.numCores;


coforall tid in 0..#numTasks do
  writeln("Hello, world! (from task " + tid + " of " + numTasks + ")");
