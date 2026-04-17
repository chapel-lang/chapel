// This test was originally so that it can have examples
// in the optimization guide, but after analyzing the performance
// of the different cases, I think the main thing is, make sure
// Array View Elision (AVE) is firing.
//
// Otherwise, with --cache-remote, for loop is the fastest
// for small enough transfers. But that can be folded in to the AVE
// short-transfer logic.

use BlockDist;
use CommDiagnostics;
use CopyAggregation;
use Random;
use Sort;
use Time;
import Sort.PartitioningUtility;

config const nPer = 1024*1024*1024;
config const nReads = 1_000_000;
config const readSize = 1000;
config const method = 0;
config const timing = true;
config const counting = false;

proc main() {
  const n = nPer * numLocales;
  const D = blockDist.createDomain(0..<n);
  //writeln("total number of elements is ", n);
  
  var A:[D] int = D;
  const ReadStarts = blockDist.createArray(0..<nReads, int);

  fillRandom(A, min=0, max=n-readSize-1);

  if method == 0 {
    writeln("Copying with for loop");
  } else if method == 1 {
    writeln("Copying with assignment from slice");
  } else if method == 2 {
    writeln("Copying with assignment to slice from slice");
  } else if method == 3 {
    writeln("Copying with bulkCopy");
  }

  if counting {
    resetCommDiagnostics();
    startCommDiagnostics();
  }
 
  var t: stopwatch;

  t.start();

  forall read in ReadStarts with (var Loc:[0..<readSize] int) {
    if method == 0 {
      /* START_FOR */
      for i in 0..<readSize {
        Loc[i] = A[read+i];
      }
      /* END_FOR */
    } else if method == 1 {
      /* START_ARRAY_FROM_SLICE */
      Loc = A[read..#readSize];
      /* END_ARRAY_FROM_SLICE */
    } else if method == 2 {
      /* START_SLICE_TO_SLICE */
      Loc[0..#readSize] = A[read..#readSize];
      /* END_SLICE_TO_SLICE */
    } else if method == 3 {
      /* START_BULK_COPY */
      PartitioningUtility.bulkCopy(Loc, 0..#readSize, A, read..#readSize);
      /* END_BULK_COPY */
    }
  }
  t.stop();
  if timing {
    writeln("copying took ", t.elapsed(), " s ");
  }

  if counting {
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
  }

  writeln("Done Copying");
} 


