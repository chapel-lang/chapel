use BlockDist;
use CommDiagnostics;
use CopyAggregation;
use Random;
use Sort;
use Time;

config const nPer = 1024*1024*1024;
config const aggregate = true;
config const timing = true;
config const counting = false;

proc main() {

  const n = nPer * numLocales;
  const D = blockDist.createDomain(0..<n);
  //writeln("total number of elements is ", n);
  
  // Generate a random permutation
  var PermutationTuples:[D] (uint, int);
  var rs = new randomStream(uint); 
  forall (i, elt, r) in zip(D, PermutationTuples, rs.next(D)) {
    elt(0) = r;
    elt(1) = i;
  }
  sort(PermutationTuples);

  // Extract the indices
  var Idx:[D] int;
  forall (idx, tup) in zip(Idx, PermutationTuples) {
    idx = tup(1);
  }

  // Now permute so that Out[Idx[i]] = In[i]
  var In:[D] int = D;
  var Out:[D] int = 0;

  writeln("Permuting");

  if counting {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
 
  var t: stopwatch;

  t.start();
  if aggregate {
    /* START_OPT */
    forall i in D with (var agg = new DstAggregator(int)) {
      agg.copy(Out[Idx[i]], In[i]);
    }
    /* END_OPT */
  } else {
    /* START_UNOPT */
    forall i in D {
      Out[Idx[i]] = In[i];
    }
    /* END_UNOPT */
  }
  t.stop();
  if timing {
    writeln("permutation took ", t.elapsed(), " s ");
  }

  if counting {
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
  }

  writeln("Done Permuting");
} 


