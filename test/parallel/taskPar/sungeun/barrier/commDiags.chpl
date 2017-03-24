use Barrier;
use BlockDist;
use CommDiagnostics;

const numRemoteTasks = numLocales;

proc printCommDiagnostics(diag) {
  for i in 0..#diag.size {
    writeln(diag(i));
  }
}

proc remoteTestBasic(b: Barrier, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  var A: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = barSpace;
  var B: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = -1;
  resetCommDiagnostics();
  startCommDiagnostics();
  coforall t in barSpace do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    b.barrier();
  }
  stopCommDiagnostics();
  printCommDiagnostics(getCommDiagnostics());
}

proc remoteTestSplitPhase(b: Barrier, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  const hi = barSpace.high;
  var A: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = barSpace;
  var B: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = -1;
  resetCommDiagnostics();
  startCommDiagnostics();
  coforall t in barSpace do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    b.notify();
    if t!=hi {            // Use an 'on' to get same results for atomics
      on b { b.wait(); }  //  and sync/single.  Without the on, there will
    } else {              //  be additional comm for the sync/single case
      on b { b.wait(); }  //  due to returning the value to the original
    }                     //  locale.
  }
  stopCommDiagnostics();
  printCommDiagnostics(getCommDiagnostics());
}

var b = new Barrier(numRemoteTasks);
writeln("atomic remote test basic");
remoteTestBasic(b, numRemoteTasks);

b.reset(numRemoteTasks);
writeln("atomic remote test split phase");
remoteTestSplitPhase(b, numRemoteTasks);

var sb1 = new Barrier(numRemoteTasks, BarrierType.Sync);
writeln("sync remote test basic");
remoteTestBasic(sb1, numRemoteTasks);

var sb2 = new Barrier(numRemoteTasks, BarrierType.Sync);
writeln("sync remote test split phase");
remoteTestSplitPhase(sb2, numRemoteTasks);
