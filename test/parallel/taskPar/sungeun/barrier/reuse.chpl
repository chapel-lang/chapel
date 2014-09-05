use Barrier;
use BlockDist;

config const printResults = false;
config const numTasks = 31;
config const numRemoteTasks = numLocales*11;

proc localTest(ref b, numTasks) {
  const barSpace = 0..#numTasks;
  var A: [barSpace] int = -1;
  coforall t in barSpace with (ref b) do {
    A[t] = t;
    for i in 1..numTasks {
      if i%2 {
        b.barrier();
      } else {
        b.notify();
        b.wait();
      }
    }
    if printResults && t==0 then writeln(A);
  }
}

proc remoteTest(ref b, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  var A: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = barSpace;
  var B: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = -1;
  coforall t in barSpace with (ref b) do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    for i in 1..numRemoteTasks {
      if i%2 {
        b.barrier();
      } else {
        b.notify();
        b.wait();
      }
    }
    if printResults && t==0 then writeln(B);
  }
}


var b = new Barrier(numTasks);
localTest(b, numTasks);

b.reset(numRemoteTasks);
remoteTest(b, numRemoteTasks);
