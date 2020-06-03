use Barriers;
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
  { // block keeps above variables alive
    resetCommDiagnostics();
    startCommDiagnostics();
    coforall t in barSpace do on A.domain.dist.idxToLocale(t) {
      B[t] = A[t];
      b.barrier();
    }
    stopCommDiagnostics();
    printCommDiagnostics(getCommDiagnostics());
  }
}

proc remoteTestSplitPhase(b: Barrier, numRemoteTasks) {
  const barSpace = 0..#numRemoteTasks;
  const hi = barSpace.high;
  var A: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = barSpace;
  var B: [{barSpace} dmapped new dmap(new Block({barSpace}))] int = -1;
  { // block keeps above variables alive
    resetCommDiagnostics();
    startCommDiagnostics();
    coforall t in barSpace do on A.domain.dist.idxToLocale(t) {
      B[t] = A[t];
      b.notify();
      b.wait();
  /* Vass 9/5/17: instead of b.wait(), we used to have the code below in this
  comment. With #7147, the "on b" clause started causing increased comm counts
  (see that PR for details). One solution is to replace it with "on b.bar".
  BenH suggested instead to remove it altogether.
    The "on b" clause was originally added in c395289b "to make the [comm
  counts] the same for the atomics and sync/single version". This seems
  no longer necessary.
    The 'if' seems to be here for historic reasons, as the 'else' clause
  contained a writeln(), so I removed it as well.
      if t!=hi {            // Use an 'on' to get same results for atomics
        on b { b.wait(); }  //  and sync/single.  Without the on, there will
      } else {              //  be additional comm for the sync/single case
        on b { b.wait(); }  //  due to returning the value to the original
      }                     //  locale.
  */
    }
    stopCommDiagnostics();
    printCommDiagnostics(getCommDiagnostics());
  }
}

var b = new Barrier(numRemoteTasks);
writeln("atomic remote test basic");
remoteTestBasic(b, numRemoteTasks);

b.reset(numRemoteTasks);
writeln("atomic remote test split phase");
remoteTestSplitPhase(b, numRemoteTasks);

var sb = new Barrier(numRemoteTasks, BarrierType.Sync);
writeln("sync remote test basic");
remoteTestBasic(sb, numRemoteTasks);

sb.reset(numRemoteTasks);
writeln("sync remote test split phase");
remoteTestSplitPhase(sb, numRemoteTasks);
