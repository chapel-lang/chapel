use Barrier;
use BlockDist;

config const numTasks = 31;
config const numRemoteTasks = numLocales*11;

proc localTest(b: Barrier, numTasks) {
  const barSpace = 0..#numTasks;
  var A: [barSpace] int = -1;
  coforall t in barSpace do {
    A[t] = t;
    b.barrier();
    if t==0 then writeln(A);
  }
}

proc remoteTest(b: Barrier, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  var A: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = barSpace;
  var B: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = -1;
  coforall t in barSpace do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    b.barrier();
    if t==0 then writeln(B);
  }
}

var b: Barrier = new Barrier(numTasks);
localTest(b, numTasks);

b.reset(numRemoteTasks);
remoteTest(b, numRemoteTasks);
delete b;

var sb1 = new Barrier(numTasks, BarrierType.Sync);
localTest(sb1, numTasks);
delete sb1;

var sb2 = new Barrier(numRemoteTasks, BarrierType.Sync);
remoteTest(sb2, numRemoteTasks);
delete sb2;
