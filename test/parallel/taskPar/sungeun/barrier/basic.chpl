use Collectives;
use BlockDist;

config const numTasks = 31;
config const numRemoteTasks = numLocales*11;

proc localTest(b: barrier, numTasks) {
  const barSpace = 0..#numTasks;
  var A: [barSpace] int = -1;
  coforall t in barSpace with (ref A) do {
    A[t] = t;
    b.barrier();
    if t==0 then writeln(A);
  }
}

proc remoteTest(b: barrier, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  var A: [{barSpace} dmapped new blockDist({barSpace})] int = barSpace;
  var B: [{barSpace} dmapped new blockDist({barSpace})] int = -1;
  coforall t in barSpace with (ref B) do on A.domain.distribution.idxToLocale(t) {
    B[t] = A[t];
    b.barrier();
    if t==0 then writeln(B);
  }
}

var b: barrier = new barrier(numTasks);
localTest(b, numTasks);

b.reset(numRemoteTasks);
remoteTest(b, numRemoteTasks);
