use Barrier;
use BlockDist;
use CommDiagnostics;

const numRemoteTasks = numLocales;

proc remoteTestBasic(ref b, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  var A: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = barSpace;
  var B: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = -1;
  resetCommDiagnostics();
  startCommDiagnostics();
  coforall t in barSpace with (ref b) do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    b.barrier();
  }
  stopCommDiagnostics();
  writeln(getCommDiagnostics());
}

proc remoteTestSplitPhase(ref b, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  const hi = barSpace.high;
  var A: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = barSpace;
  var B: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = -1;
  resetCommDiagnostics();
  startCommDiagnostics();
  coforall t in barSpace with (ref b) do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    b.notify();
    if t!=hi {            // Use an 'on' to get same results for atomics
      on b { b.wait(); }  //  and sync/single.  Without the on, there will
    } else {              //  be additional comm for the sync/single case
      on b { b.wait(); }  //  due to returning the value to the original
    }                     //  locale.
  }
  stopCommDiagnostics();
  writeln(getCommDiagnostics());
}

var b = new Barrier(numRemoteTasks);
remoteTestBasic(b, numRemoteTasks);

b.reset(numRemoteTasks);
remoteTestSplitPhase(b, numRemoteTasks);

var sb1 = new sBarrier(numRemoteTasks);
remoteTestBasic(sb1, numRemoteTasks);

var sb2 = new sBarrier(numRemoteTasks);
remoteTestSplitPhase(sb2, numRemoteTasks);
