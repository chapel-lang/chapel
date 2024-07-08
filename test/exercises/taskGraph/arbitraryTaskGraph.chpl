use IO;

// take in a sparse matrix that represents a task graph
// an entry in A(i,j) means task i can't start before task j
// the matrix is lower triangular because it describes a DAG
// FIXME: starting out with a dense 2D array
config const N = 4,
             verbose = false;
var A: [0..<N,0..<N] bool;

// 0 and 1 can execute right away
// 2 and 3 have to wait for
A[2,0] = true;
A[2,1] = true;
A[3,1] = true;

// can I declare an array of atomics? yes
var numToWaitFor: [0..<N] atomic int;
writeln(numToWaitFor);

// encoding how many each task needs to wait for
forall (i,j) in A.domain with (ref numToWaitFor) {
  if A[i,j] { numToWaitFor[i].fetchAdd(1); }
}

writeln(numToWaitFor);

// start all of the tasks and have them wait as needed and
// have them decrement the appropriate numToWaitFor
coforall i in 0..<N with (ref numToWaitFor) {
  numToWaitFor[i].waitFor(0);

  // do task i work
  if verbose then writeln("Task: ", i, " is working away");

  for j in i+1..<N { // should only be later tasks
    if A[j,i] { numToWaitFor[j].fetchAdd(-1); }
  }
}
