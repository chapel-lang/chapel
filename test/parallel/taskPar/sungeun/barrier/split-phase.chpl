use Collectives;
use BlockDist;

config const numTasks = 31;
config const numRemoteTasks = numLocales*11;

proc localTest(b: barrier, numTasks) {
  const barSpace = 0..#numTasks;
  var A: [barSpace] int = -1;
  coforall t in barSpace with (ref A) do {
    A[t] = t;
    b.notify();
    if t!=barSpace.high {
      b.wait();
    } else {
      b.wait();
      writeln(A);
    }
  }
}

proc remoteTest(b: barrier, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  var A: [{barSpace} dmapped new blockDist({barSpace})] int = barSpace;
  var B: [{barSpace} dmapped new blockDist({barSpace})] int = -1;
  coforall t in barSpace with (ref B) do on A.domain.distribution.idxToLocale(t) {
    B[t] = A[t];
    b.notify();
    if t!=barSpace.high {
      b.wait();
    } else {
      b.wait();
      writeln(B);
    }
  }
}


var b = new barrier(numTasks);
localTest(b, numTasks);

b.reset(numRemoteTasks);
remoteTest(b, numRemoteTasks);
