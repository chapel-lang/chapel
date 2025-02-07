use CopyAggregation;
use BlockDist;
use RangeChunk;

config const n = 1_000_000;
proc main() {
  assert(n > 2); // this test assumes this

  const nTasksPerLocale =
    if dataParTasksPerLocale > 0
    then dataParTasksPerLocale
    else here.maxTaskPar;

  // store numbers in forward order in Fwd
  var Fwd =  blockDist.createArray(0..<n, int);
  Fwd = 0..<n;

  // store numbers in reverse order in Rev
  var Rev = blockDist.createArray(0..<n, int);
  Rev = 0..<n by -1;

  assert(Fwd[0] == 0 && Fwd[1] == 1);
  assert(Rev[0] == n-1 && Rev[1] == n-2);

  var A = Rev;
  assert(A.equals(Rev));

  var B = blockDist.createArray(0..<n, int);

  // -- test DstAggregator --
  // Set B to the elements of Rev reordered by value
  B = 0;
  forall elt in A with (var agg = new DstAggregator(int)) {
    agg.copy(B[elt], elt);
  }
  assert(B.equals(Fwd));

  // do the same with a coforall
  B = 0;
  coforall loc in Locales {
    on loc {
      const ref locDom = A.localSubdomain();
      const locRegion = locDom.dim(0)[0..<n];
      coforall chunk in chunks(locRegion, nTasksPerLocale) {
        var agg = new DstAggregator(int);
        const halfway = chunk.low + chunk.size / 2;
        const firstchunk = chunk.low..<halfway;
        const secondchunk = halfway..chunk.high;
        for i in firstchunk {
          const elt = A[i];
          agg.copy(B[elt], elt);
        }
        agg.flush();
        // check that the aggregator still works after flush
        for i in secondchunk {
          const elt = A[i];
          agg.copy(B[elt], elt);
        }
      }
    }
  }
  assert(B.equals(Fwd));

  // -- test SrcAggregator --
  B = 0;
  // Set B to the elements of A reordered by value
  forall (elt, i) in zip(B, B.domain) with (var agg = new SrcAggregator(int)) {
    agg.copy(elt, A[n-1-i]);
  }
  assert(B.equals(Fwd));

  // do the same with a coforall
  B = 0;
  coforall loc in Locales {
    on loc {
      const ref locDom = B.localSubdomain();
      const locRegion = locDom.dim(0)[0..<n];
      coforall chunk in chunks(locRegion, nTasksPerLocale) {
        var agg = new SrcAggregator(int);
        const halfway = chunk.low + chunk.size / 2;
        const firstchunk = chunk.low..<halfway;
        const secondchunk = halfway..chunk.high;
        for i in firstchunk {
          agg.copy(B[i], A[n-1-i]);
        }
        agg.flush();
        // check that the aggregator still works after flush
        for i in secondchunk {
          agg.copy(B[i], A[n-1-i]);
        }
      }
    }
  }
  assert(B.equals(Fwd));

  writeln("OK");
}
