// atomic type examples for 04-Chapel-more-parallelism slide deck.

config const n = 100;
config const m = 10;

///////////////////////
// Counting barrier example

use Time;       // needed for the 'sleep' procedure
var count: atomic int, done: atomic bool;

proc barrier(numTasks) {
  const myCount = count.fetchAdd(1);
  if (myCount < numTasks - 1) then
    done.waitFor(true);
  else
    done.testAndSet();
}

// Create some tasks and use the barrier.
// Each task sleeps for a different amount of time, but task 1, which sleeps
// the shortest, will not be past the barrier until all other tasks are done.
const numTasks = 10;
coforall i in 1..numTasks {
  sleep(i);
  barrier(numTasks);


  if i==1 {
    writeln("All tasks done sleeping");
    // reset the barrier
    count.write(0); done.write(false);
  }
  
  // all tasks waiting for task 1 to finish resetting the barrier count
  count.waitFor(0);

  writeln("task ", i, " signing out");
}

///////////////////////
// Task intents in 'forall' loops: scalar variables

writeln();
{
  var bucketCount: [0..<m] atomic real;
  forall i in 1..n with (ref bucketCount) do
    bucketCount[i % m].add(1);
  writeln("bucketCount = ", bucketCount);
}

{
  var bucketCount: [0..<m] atomic real;
  forall i in 1..n do
    bucketCount[i % m].add(1);
  writeln("bucketCount = ", bucketCount, ", 'ref' intent is also the default for atomic");
}

