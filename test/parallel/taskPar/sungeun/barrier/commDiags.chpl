use Barrier;
use BlockDist;

const numRemoteTasks = numLocales;

proc remoteTestBasic(b, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  var A: [[barSpace] dmapped new dmap(new Block([barSpace]))] int = barSpace;
  var B: [[barSpace] dmapped new dmap(new Block([barSpace]))] int = -1;
  var commDiags: [LocaleSpace] commDiagnostics;
  coforall t in barSpace do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    resetCommDiagnosticsHere();
    startCommDiagnosticsHere();
    b.barrier();
    stopCommDiagnosticsHere();
    commDiags[here.id] = getCommDiagnosticsHere();
    if t==0 then writeln(B);
  }
  writeln(commDiags);
}

proc remoteTestSplitPhase(b, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  const hi = barSpace.high;
  var A: [[barSpace] dmapped new dmap(new Block([barSpace]))] int = barSpace;
  var B: [[barSpace] dmapped new dmap(new Block([barSpace]))] int = -1;
  var commDiags: [LocaleSpace] commDiagnostics;
  coforall t in barSpace do on A.domain.dist.idxToLocale(t) {
    B[t] = A[t];
    resetCommDiagnosticsHere();
    startCommDiagnosticsHere();
    b.notify();
    if t!=hi {
      b.wait();
      stopCommDiagnosticsHere();
      commDiags[here.id] = getCommDiagnosticsHere();
    } else {
      b.wait();
      stopCommDiagnosticsHere();
      commDiags[here.id] = getCommDiagnosticsHere();
      writeln(B);
    }
  }
  writeln(commDiags);
}

var b = new Barrier(numRemoteTasks);
remoteTestBasic(b, numRemoteTasks);

b.reset(numRemoteTasks);
remoteTestSplitPhase(b, numRemoteTasks);


/*

Omit sync vars versions since it's difficult to consistently track
comm diag, because (1) we don't necessarily capture every remote put
from locale 0 for done.readFE(), and (2) we end up accidentally
capturing some of the comm for shutting down the coforall on locale 0.

We usually get lucky in the case of the basic barrier, but never seem
to get consistent results for the split-phase barrier.

var sb1 = new sBarrier(numRemoteTasks);
remoteTestBasic(sb1, numRemoteTasks);


var sb2 = new sBarrier(numRemoteTasks);
remoteTestSplitPhase(sb2, numRemoteTasks);
*/
