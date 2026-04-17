config const verbose = true;

use Sort.Partitioning;
use Sort.PartitioningUtility;

import Sort.{sort, defaultComparator, isSorted,
             keyPartStatus, keyComparator, keyPartComparator};
use Random;
import Math;
import Map;
import Time;
import BlockDist;

config const skipslow = false;

const myDefaultComparator = new defaultComparator();

// nSplit positive: create that many splitters
// nSplit negative: create a sample from the Input array
// nTasks == 0 means serial partitioner
// nTasks == -1 means serial in-place partitioner
proc testPartition(n: int, nSplit: int, useEqualBuckets: bool, nTasks: int) {
  if verbose then
    writeln("testPartition(n=", n, ", nSplit=", nSplit, ", ",
            "useEqualBuckets=", useEqualBuckets, ", nTasks=", nTasks, ")");

  const useNLocales = max(1, min(nTasks, Locales.size));
  const nTasksPerLocale = max(1, nTasks / useNLocales);
  const targetLocales = for i in 0..<useNLocales do Locales[i];

  const InputDom = makeBlockDomain(0..<n, targetLocales);
  const OutputDom = makeBlockDomain(0..<n, targetLocales);

  var Input: [InputDom] int = 0..<n by -1;
  var Output: [OutputDom] int = -1;

  var InputCounts:Map.map(int, int);

  for x in Input {
    InputCounts[x] += 1;
  }

  const nSplitters = (1 << Math.log2(abs(nSplit))) - 1;
  var UseSplitters:[0..<nSplitters+1] int;
  for i in 0..<nSplitters {
    UseSplitters[i] = 1 + i * n / nSplitters;
  }

  const sp;
  if nSplit > 0 {
    sp = new splitters(UseSplitters, useEqualBuckets);
  } else {
    var randNums = new Random.randomStream(int);
    var SplittersSampleDom = {0..<abs(10*nSplit)};
    var SplittersSample:[SplittersSampleDom] int;
    for (x, r) in zip(SplittersSample,
                      randNums.next(SplittersSampleDom, 0, n-1)) {
      x = r;
    }
    sp = new splitters(SplittersSample, abs(nSplit), myDefaultComparator,
                       howSorted=sortLevel.unsorted);
  }

  assert(isSorted(sp.sortedStorage));

  const nBuckets = sp.numBuckets;
  const hasEqualityBuckets = sp.hasEqualityBuckets;

  var Bkts: [0..<nBuckets] bktCount;

  if nTasks > 0 {
    Bkts = partition(Input.domain, Input.domain.dim(0), Input,
                     OutputShift=none, Output,
                     sp, myDefaultComparator,
                     nTasksPerLocale=nTasksPerLocale,
                     noSerialPartition=nTasks>0);
  } else if nTasks == 0 {
    var Counts:[0..<nBuckets] int;
    var Starts:[0..<nBuckets] int;
    serialStablePartition(Input.domain.dim(0), Input,
                          OutputShift=none, Output,
                          sp, myDefaultComparator,
                          filterBucket=none,
                          Counts, Starts, Bkts);

  } else if nTasks == -1 {
    var Starts:[0..<nBuckets] int;
    var Ends:[0..<nBuckets] int;
    Output = Input;
    serialUnstablePartition(Output.domain.dim(0), Output,
                            sp, myDefaultComparator,
                            Starts, Ends, Bkts);
  }

  assert(Bkts.size == nBuckets);

  var total = 0;

  for bin in 0..<nBuckets {
    const binSize = Bkts[bin].count;
    const binStart = Bkts[bin].start;
    const binEnd = binStart + binSize - 1;

    total += binSize;

    if bin == 0 {
      assert(binStart == 0);
    }
    if bin == nBuckets-1 {
      assert(binEnd == n-1);
    }

    var lower = -1;
    var upper = -1;
    var equals = -1;
    if sp.bucketHasLowerBound(bin) {
      lower = sp.bucketLowerBound(bin);
    }
    if sp.bucketHasUpperBound(bin) {
      upper = sp.bucketUpperBound(bin);
    }
    if sp.bucketHasEqualityBound(bin) {
      equals = sp.bucketEqualityBound(bin);
    }

    for i in binStart..binEnd {
      if lower != -1 {
        assert(lower < Output[i]);
      }
      if upper != -1 {
        if hasEqualityBuckets then
          assert(Output[i] < upper);
        else
          assert(Output[i] <= upper);
      }
      if equals != -1 then
        assert(Output[i] == equals);
    }
  }

  var OutputCounts:Map.map(int, int);
  for x in Output {
    OutputCounts[x] += 1;
  }

  assert(InputCounts == OutputCounts);

  assert(total == n);


  if nTasks >= 0 {
    // check also that the partitioning is stable
    Input = 0..<n;
    Output = -1;
    var ExpectOutput = Input;
    partition(Input.domain, Input.domain.dim(0), Input,
              OutputShift=none, Output,
              sp, myDefaultComparator,
              nTasksPerLocale=nTasksPerLocale,
              noSerialPartition=nTasks>0);
    assert(Output.equals(ExpectOutput));
  }
}

proc testPartitionsEven(n: int, nSplit: int) {
  if verbose then
    writeln("testPartitionsEven(n=", n, ", nSplit=", nSplit, ")");

  var Input: [0..<n] int = 0..<n by -1;
  var Output: [0..<n] int = -1;

  var Sample = Input;
  const sp = new splitters(Sample, nSplit, myDefaultComparator,
                           howSorted=sortLevel.unsorted);
  assert(isSorted(sp.sortedStorage));

  const nBuckets = sp.numBuckets;
  const hasEqualityBuckets = sp.hasEqualityBuckets;

  const Bkts = partition(Input.domain, Input.domain.dim(0), Input,
                         OutputShift=none, Output,
                         sp, myDefaultComparator,
                         nTasksPerLocale=1);
  assert(Bkts.size == nBuckets);

  var minSize = max(int);
  var maxSize = -1;
  for bin in 0..<nBuckets {
    const binSize = Bkts[bin].count;

    if verbose && nBuckets < 100 {
      writeln("  bucket ", bin, " has ", binSize, " elements");
    }

    minSize = min(minSize, binSize);
    maxSize = max(maxSize, binSize);
  }

  if verbose {
    writeln("  minSize ", minSize);
    writeln("  maxSize ", maxSize);
  }


  assert(minSize + 2 >= maxSize);
}

proc testPartitionSingleSplitter(n: int) {
  if verbose then
    writeln("testPartitionSingleSplitter(n=", n, ")");

  var Input: [0..<n] int = 0..<n by -1;
  var Output: [0..<n] int = -1;

  var Sample = [n/2, n/2, n/2, n/2, n/2, n/2];
  const sp = new splitters(Sample, 100, myDefaultComparator,
                           howSorted=sortLevel.unsorted);
  assert(isSorted(sp.sortedStorage));

  const nBuckets = sp.numBuckets;
  assert(sp.hasEqualityBuckets);
  assert(nBuckets == 3); // < == and > buckets

  const Bkts = partition(Input.domain, Input.domain.dim(0), Input,
                           OutputShift=none, Output,
                           sp, myDefaultComparator,
                           nTasksPerLocale=1);
  assert(Bkts.size == nBuckets);

  var total = 0;
  var minSize = max(int);
  var maxSize = -1;
  for bin in 0..<nBuckets {
    const binSize = Bkts[bin].count;

    total += binSize;
  }

  assert(total == n);
}

proc checkArrayMatches(got: [], expect: []) {
  if got.domain != expect.domain {
    writeln("array does not match : domains differ");
    writeln("got ", got.domain);
    writeln("exp ", expect.domain);
    assert(got.domain == expect.domain);
  }
  for (g, e, i) in zip(got, expect, expect.domain) {
    if g != e {
      writeln("array does not match : element ", i, " differs");
      writeln("got ", got);
      writeln("exp ", expect);
      assert(g == e);
    }
  }
}

proc testSplitters() {
  if verbose then
    writeln("testSplitters");

  {
    if verbose then
      writeln("  sorted repeating");
    var sample = [1, 1, 1, 5,  5,  5, 11, 11];
    var expect = [1, 5, 11, 11]; // smaller due to equality buckets
    var s = new splitters(sample,
                          requestedNumBuckets=9,
                          myDefaultComparator,
                          sortLevel.fully);
    assert(s.numBuckets == 7);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  sorted");
    var sample = [1, 1, 1, 5,  7,  9, 11, 32];
    var expect = [1, 5, 7, 7]; // smaller due to equality buckets
    var s = new splitters(sample,
                          requestedNumBuckets=9,
                          myDefaultComparator,
                          sortLevel.fully);
    assert(s.numBuckets == 7);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  unsorted");
    var sample = [1, 5, 7, 9, 11,  1, 32,  1];
    // sorts to  [1, 1, 1, 5,  7,  9, 11, 32];
    var expect = [1, 5, 7, 7]; // smaller due to equality buckets
    var s = new splitters(sample,
                          requestedNumBuckets=9,
                          myDefaultComparator,
                          sortLevel.unsorted);
    assert(s.numBuckets == 7);
    checkArrayMatches(s.sortedStorage, expect);
  }
  {
    if verbose then
      writeln("  approx sorted");
    var sample = [1, 5, 7, 9, 11,  1, 32, 1];
    var expect = [1, 5, 9, 9]; // smaller due to equality buckets
    var s = new splitters(sample,
                          requestedNumBuckets=8,
                          myDefaultComparator,
                          sortLevel.approximately);
    assert(s.numBuckets == 7);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  checking span 2/16");
    var sample = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    var expect = [8, 8];
    var s = new splitters(sample,
                          requestedNumBuckets=2,
                          myDefaultComparator,
                          sortLevel.fully);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  checking span 4/16");
    var sample = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    var expect = [4, 8, 12, 12];
    var s = new splitters(sample,
                          requestedNumBuckets=4,
                          myDefaultComparator,
                          sortLevel.fully);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  checking span 8/16");
    var sample = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    var expect = [2, 4, 6, 8, 10, 12, 14, 14];
    var s = new splitters(sample,
                          requestedNumBuckets=8,
                          myDefaultComparator,
                          sortLevel.fully);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  checking span 16/16");
    var sample = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];
    var expect = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 14];
    var s = new splitters(sample,
                          requestedNumBuckets=16,
                          myDefaultComparator,
                          sortLevel.fully);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  checking span 4/15");
    var sample = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14];
    var expect = [3, 6, 10, 10];
    var s = new splitters(sample,
                          requestedNumBuckets=4,
                          myDefaultComparator,
                          sortLevel.fully);
    checkArrayMatches(s.sortedStorage, expect);
  }

  {
    if verbose then
      writeln("  checking span 4/11");
    var sample = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    var expect = [2, 4, 7, 7];
    var s = new splitters(sample,
                          requestedNumBuckets=4,
                          myDefaultComparator,
                          sortLevel.fully);
    checkArrayMatches(s.sortedStorage, expect);
  }

}

proc testBucketBoundary() {
  if verbose then
    writeln("testBucketBoundary()");

  for x in [0:uint,
            1:uint,
            127:uint,
            128:uint,
            1000:uint,
            10000:uint,
            10000000:uint,
            max(uint(16)):uint,
            max(uint(32)):uint,
            (max(int)-1):uint,
            max(int):uint,
            max(uint)-1,
            max(uint)] {
    var tup = encodeToTuple(x);
    var y = decodeFromTuple(tup);
    assert(x == y);
  }
}

proc testSort(n: int, max: uint, param logBuckets: int, seed: int,
              noBaseCase:bool, random: bool, fullBoundaries:bool,
              sorter:string) {

  if verbose then
    writeln("testSort(n=", n, ", max=", max, ", logBuckets=", logBuckets,
            ", seed=", seed, ", noBaseCase=", noBaseCase, ", random=", random,
            ", fullBoundaries=", fullBoundaries, ", sorter='", sorter, "')");

  const Dom = makeBlockDomain(0..<n, Locales);
  var Elts: [Dom] uint;
  var Scratch: [Dom] uint;
  var BucketBoundaries: [Dom] uint(8);
  if random {
    Random.fillRandom(Elts, min=0, max=max, seed=seed);
  } else {
    Elts = 0..<n by -1;
  }
  const nTasksPerLocale = computeNumTasks();
  var EltsCopy: [0..<n] uint = Elts;

  if sorter == "sample" {
    psort(Elts, Scratch, BucketBoundaries,
          0..<n,
          myDefaultComparator,
          radixBits=0, // sample sort
          logBuckets=logBuckets,
          nTasksPerLocale=nTasksPerLocale,
          endbit=numBits(uint),
          markAllEquals=fullBoundaries,
          noBaseCase=noBaseCase);
  } else if sorter == "radix" {
    psort(Elts, Scratch, BucketBoundaries,
          0..<n,
          myDefaultComparator,
          radixBits=logBuckets,
          logBuckets=logBuckets,
          nTasksPerLocale=nTasksPerLocale,
          endbit=numBits(uint),
          markAllEquals=fullBoundaries,
          noBaseCase=noBaseCase);
  } else {
    halt("Unknown sorter in testSort");
  }

  assert(isInA(BucketBoundaries[0]));
  assert(isBucketBoundary(BucketBoundaries[0]));
  assert(!isUnsortedBucketBoundary(BucketBoundaries[0]));
  var lastBoundary = BucketBoundaries[0];
  for i in 1..<n {
    if Elts[i-1] > Elts[i] {
      writeln("unsorted at element ", i);
      assert(false);
    }
    if isBucketBoundary(BucketBoundaries[i]) {
      assert(isInA(BucketBoundaries[i]));
      assert(!isUnsortedBucketBoundary(BucketBoundaries[i]));
      // there might not be a bucket boundary every time the element
      // differs; but if there is, we can't have the same element in
      // a previous bucket
      assert(Elts[i-1] < Elts[i]);
      lastBoundary = BucketBoundaries[i];
    } else if (isEqualBucketBoundary(lastBoundary)) {
      assert(Elts[i-1] == Elts[i]);
    }
    if fullBoundaries {
      assert(isBucketBoundary(BucketBoundaries[i]) ||
             isEqualBucketBoundary(lastBoundary));
    }
  }

  assert(isSorted(Elts));

  var UnstableSortCopy:[0..<n] uint = EltsCopy;
  sort(EltsCopy, stable=true);

  if max > 10 {
    sort(UnstableSortCopy);
    assert(EltsCopy.equals(UnstableSortCopy));
  }

  for i in Dom {
    if Elts[i] != EltsCopy[i] {
      writeln("sort mismatch with element ", i);
      if i > 0 {
        writeln("Elts[i-1] = ", Elts[i-1]);
        writeln("EltsCopy[i-1] = ", EltsCopy[i-1]);
      }
      writeln("Elts[i] = ", Elts[i]);
      writeln("EltsCopy[i] = ", EltsCopy[i]);
      if i+1 < n {
        writeln("Elts[i+1] = ", Elts[i+1]);
        writeln("EltsCopy[i+1] = ", EltsCopy[i+1]);
      }
      assert(false);
    }
  }
  assert(Elts.equals(EltsCopy));
}

proc testSorts() {
  var seed = 1;
  const bign = if numLocales == 1 then 100_000 else 10_000;
  for sorter in ["sample", "radix"] {
    for n in [10, 100, bign] {
      for max in [10, max(uint)] {
        for rnd in [false, true] {
          for noBaseCase in [false, true] {
            for fullBoundaries in [false, true] {
              proc help(param logBuckets) {
                testSort(n=n,max=max,logBuckets=logBuckets,seed=seed,
                         noBaseCase=noBaseCase,random=rnd,fullBoundaries=fullBoundaries,sorter);
              }

              // skip these as they are slow
              if n >= 10_000 && noBaseCase {
                continue;
              }

              if n <= 100 {
                help(2);
                help(4);
              }
              help(8);
              if sorter != "radix" {
                // radix sorter assumes radix divides key type
                help(10);
              }
              if n >= 10_000 && !noBaseCase {
                help(16);
              }

              seed += 1;
            }
          }
        }
      }
    }
  }
}

proc testMultiWayMerge() {
  {
    if verbose then
      writeln("12 way merge");
    // this input example is from Knuth vol 2 5.4.1 pp 253
    var Input = [/*0*/ 503, 504,
                 /*2*/ 87,
                 /*3*/ 512,
                 /*4*/ 61, 62, 300,
                 /*7*/ 908,
                 /*8*/ 170,
                 /*9*/ 897,
                /*10*/ 275,
                /*11*/ 653,
                /*12*/ 426,
                /*13*/ 154,
                /*14*/ 509];

    var InputRanges = [0..1,
                       2..2,
                       3..3,
                       4..6,
                       7..7,
                       8..8,
                       9..9,
                       10..10,
                       11..11,
                       12..12,
                       13..13,
                       14..14];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [61, 62, 87, 154, 170,
                               275, 300, 426, 503, 504,
                               509, 512, 653, 897, 908]);
  }

  {
    if verbose then
      writeln("4 way merge");
    // this input example is from Knuth vol 2 5.4.1 pp 252
    var Input = [/*0*/ 87, 503,
                 /*2*/ 170, 908,
                 /*4*/ 154, 426, 653,
                 /*7*/ 612];
    var InputRanges = [0..1,
                       2..3,
                       4..6,
                       7..7];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [87, 154, 170, 426, 503, 612, 653, 908]);
  }

  {
    if verbose then
      writeln("4 way merge with empty");
    // test with some empty sorted sequences
    var Input = [/*0*/ 87, 503,
                 /*2*/
                 /*2*/ 154, 426, 653,
                 /*5*/ 612];
    var InputRanges = [0..1,
                       2..1,
                       2..4,
                       5..5];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [87, 154, 426, 503, 612, 653]);
  }

  {
    if verbose then
      writeln("4 way merge with more empty");
    // test with some empty sorted sequences
    var Input = [/*0*/
                 /*0*/
                 /*0*/ 154, 426, 653,
                 /*3*/];
    var InputRanges = [0..-1,
                       0..-1,
                       0..2,
                       3..2];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [154, 426, 653]);
  }

  {
    if verbose then
      writeln("1-way merge");
    var Input = [/*0*/ 9, 11];
    var InputRanges = [0..1,];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [9, 11]);
  }

  {
    if verbose then
      writeln("2-way merge");
    var Input = [/*0*/ 9, 11,
                 /*2*/ 10, 22, 36];
    var InputRanges = [0..1,
                       2..4];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [9, 10, 11, 22, 36]);
  }

  {
    if verbose then
      writeln("3-way merge");
    var Input = [/*0*/ 9, 11,
                 /*2*/ 10, 22, 36,
                 /*5*/ 2, 12];
    var InputRanges = [0..1,
                       2..4,
                       5..6];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [2, 9, 10, 11, 12, 22, 36]);
  }

  {
    if verbose then
      writeln("5-way merge");
    var Input = [/*0*/ 9, 11,
                 /*2*/ 10, 22, 36,
                 /*5*/ 2, 12,
                 /*7*/ 1, 10,
                 /*9*/ 2];
    var InputRanges = [0..1,
                       2..4,
                       5..6,
                       7..8,
                       9..9];
    var Output:[Input.domain] int;
    multiWayMerge(Input, InputRanges, Output, Output.dim(0),
                  myDefaultComparator);
    checkArrayMatches(Output, [1, 2, 2, 9, 10, 10, 11, 12, 22, 36]);
  }
}

proc testBitsInCommon() {
  if verbose then
    writeln("testBitsInCommon()");

  record myTupleComparator : keyPartComparator {
    inline proc keyPart(tup, i: int): (keyPartStatus, tup(0).type) {
      if i >= tup.size {
        return (keyPartStatus.pre, tup(0));
      } else {
        return (keyPartStatus.returned, tup(i));
      }
    }
  }

  record myIntKeyComparator : keyComparator {
    proc key(elt) { return elt; }
  }

  param intbits = numBits(0.type);
  assert(intbits == bitsInCommon(0, 0, new myIntKeyComparator()));
  assert(intbits-8 == bitsInCommon(0xff, 0x11, new myIntKeyComparator()));

  var a = (0, 0xff);
  var b = (0, 0x11);
  assert(intbits + intbits - 8 == bitsInCommon(a, b, new myTupleComparator()));

  // test the related functionality in createRadixSplitters
  {
    var s = createRadixSplitters([0, 0], 0..1, new myIntKeyComparator(),
                                 activeLocs=[here], radixBits=1,
                                 startbit=0, endbit=max(int),
                                 nTasksPerLocale=computeNumTasks());
    assert(s.startbit == intbits);
  }
  {
    var s = createRadixSplitters([0, 1], 0..1, new myIntKeyComparator(),
                                 activeLocs=[here], radixBits=1,
                                 startbit=0, endbit=max(int),
                                 nTasksPerLocale=computeNumTasks());
    assert(s.startbit == intbits-1);
  }
  {
    var s = createRadixSplitters([0, 1], 0..1, new myIntKeyComparator(),
                                 activeLocs=[here], radixBits=8,
                                 startbit=0, endbit=max(int),
                                 nTasksPerLocale=computeNumTasks());
    assert(s.startbit == intbits-8);
  }
  {
    var s = createRadixSplitters([a, b], 0..1, new myTupleComparator(),
                                 activeLocs=[here], radixBits=1,
                                 startbit=0, endbit=max(int),
                                 nTasksPerLocale=computeNumTasks());
    assert(s.startbit == intbits + intbits - 8);
  }
  {
    var s = createRadixSplitters([a, b], 0..1, new myTupleComparator(),
                                 activeLocs=[here], radixBits=8,
                                 startbit=0, endbit=max(int),
                                 nTasksPerLocale=computeNumTasks());
    assert(s.startbit == intbits + intbits - 8);
  }
}


proc runTests() {
  // test multi-way merge
  testMultiWayMerge();

  // test partition

  // test serial partition
  testPartition(10, 4, false, 0);
  testPartition(10, 4, true, 0);
  testPartition(100, 20, false, 0);
  testPartition(100, 20, true, 0);

  // test serial in-place partition
  testPartition(10, 4, false, -1);
  testPartition(10, 4, true, -1);
  testPartition(100, 20, false, -1);
  testPartition(100, 20, true, -1);
  testPartition(10000, 100, false, -1);
  testPartition(10000, 100, true, -1);

  testPartition(10, 4, false, 1);
  testPartition(10, 4, true, 1);
  testPartition(100, 20, false, 1);
  testPartition(100, 20, true, 1);

  testPartition(10, 4, false, 2);
  testPartition(10, 4, true, 2);
  testPartition(100, 20, false, 2);
  testPartition(100, 20, true, 2);
  testPartition(10000, 100, false, 8);
  testPartition(10000, 100, true, 8);

  // test with random samples
  testPartition(10, -4, false, 0);
  testPartition(100, -20, false, 0);
  testPartition(10, -4, false, -1);
  testPartition(100, -20, false, -1);
  testPartition(10, -4, false, 1);
  testPartition(100, -20, false, 1);
  testPartition(10, -4, false, 2);
  testPartition(100, -20, false, 2);
  testPartition(10000, -100, false, 8);

  // test partitions are even with a perfect sample
  testPartitionsEven(10, 4);
  testPartitionsEven(1000, 20);
  testPartitionsEven(1000, 100);
  testPartitionsEven(10000, 80);
  testPartitionsEven(10000, 9876);

  // test that creating a single splitter works OK
  testPartitionSingleSplitter(10);

  // test creating splitters in other cases
  testSplitters();

  // test bucket boundary helpers
  testBucketBoundary();

  // test bitsInCommon
  testBitsInCommon();

  // test sorters
  testSorts();
}

config const sampleLogBuckets = 8;
config param radixLogBuckets = 8;
config const minn = 1;
config const maxn = 10**8;
config param wordsper = 1;

record testElt {
  var elts: wordsper * uint;
}
proc min(type t: testElt) {
  var ret: testElt;
  for i in 0..<wordsper {
    ret.elts(i) = min(uint);
  }
  return ret;
}
proc max(type t: testElt) {
  var ret: testElt;
  for i in 0..<wordsper {
    ret.elts(i) = max(uint);
  }
  return ret;
}

record testEltKeyPartComparator : keyPartComparator {
  inline proc keyPart(elt: testElt, i: int): (keyPartStatus, uint) {
    if i >= wordsper {
      return (keyPartStatus.pre, elt.elts(0));
    } else {
      return (keyPartStatus.returned, elt.elts(i));
    }
  }
}


proc fillRandomTuples(ref Elts) {
  var rs = new randomStream(uint, seed=1);
  // set each tuple element in a separate iteration
  for i in 0..<wordsper {
    forall (r, a) in zip(rs.next(Elts.domain), Elts) {
      a.elts(i) = r;
    }
  }
}

proc testTiming() {
  var n = minn;
  while n <= maxn {
    const Dom = makeBlockDomain(0..<n, Locales);
    var Elts: [Dom] testElt;
    var Scratch: [Dom] testElt;
    var BucketBoundaries: [Dom] uint(8);
    const nTasksPerLocale = computeNumTasks();

    var ntrials = min(max(1, maxn / n), 1000);

    var sample: Time.stopwatch;
    for trial in 0..<ntrials {
      BucketBoundaries = 0;
      fillRandomTuples(Elts);
      sample.start();
      psort(Elts, Scratch, BucketBoundaries,
            0..<n,
            new testEltKeyPartComparator(),
            radixBits=0,
            logBuckets=sampleLogBuckets,
            nTasksPerLocale,
            endbit=numBits(uint));
      sample.stop();
    }

    var radix: Time.stopwatch;
    for trial in 0..<ntrials {
      BucketBoundaries = 0;
      fillRandomTuples(Elts);
      radix.start();
      psort(Elts, Scratch, BucketBoundaries,
            0..<n,
            new testEltKeyPartComparator(),
            radixBits=radixLogBuckets,
            logBuckets=radixLogBuckets,
            nTasksPerLocale,
            endbit=numBits(uint));
      radix.stop();
    }

    var stdstable: Time.stopwatch;
    var stdunstable: Time.stopwatch;
    for trial in 0..<ntrials {
      BucketBoundaries = 0;
      BucketBoundaries[0] = boundaryTypeBaseCaseSortedBucketInA;
      fillRandomTuples(Elts);
      stdstable.start();

      // copy to a local array
      const region = 0..<n;
      var LocA:[region] Elts.eltType;
      LocA[region] = Elts[region];

      sort(LocA, new testEltKeyPartComparator(), region=0..<n, stable=true);
      forall i in 0..<n {
        if i > 0 {
          if LocA[i-1] < LocA[i] {
            BucketBoundaries[i] = boundaryTypeBaseCaseSortedBucketInA;
          }
        }
      }

      // copy back
      Elts[region] = LocA[region];

      stdstable.stop();
    }

    for trial in 0..<ntrials {
      BucketBoundaries = 0;
      BucketBoundaries[0] = boundaryTypeBaseCaseSortedBucketInA;
      fillRandomTuples(Elts);
      stdunstable.start();

      // copy to a local array
      const region = 0..<n;
      var LocA:[region] Elts.eltType;
      LocA[region] = Elts[region];

      sort(LocA, new testEltKeyPartComparator(), region=0..<n, stable=false);
      forall i in 0..<n {
        if i > 0 {
          if LocA[i-1] < LocA[i] {
            BucketBoundaries[i] = boundaryTypeBaseCaseSortedBucketInA;
          }
        }
      }

      // copy back
      Elts[region] = LocA[region];

      stdunstable.stop();
    }

    if n == minn {
      writeln("sorting ", wordsper, " words per element");
      writef("% <14s % <14s % <14s % <14s % <14s\n",
             "n", "sample MB/s", "radix MB/s",
             "std stable MB/s", "std unstable MB/s");
    }

    const nb = n*wordsper*numBytes(uint);

    writef("% <14i % <14r % <14r % <14r % <14r\n",
           n,
           nb / 1000.0 / 1000.0 / (sample.elapsed()/ntrials),
           nb / 1000.0 / 1000.0 / (radix.elapsed()/ntrials),
           nb / 1000.0 / 1000.0 / (stdstable.elapsed()/ntrials),
           nb / 1000.0 / 1000.0 / (stdunstable.elapsed()/ntrials));

    n *= 10;
  }
}

config const timing = false;

proc main() {
  if timing {
    testTiming();
    return;
  }

  runTests();

  writeln("Partitioning Sort OK");
}
