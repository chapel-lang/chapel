use Sort;
use SampleSortHelp;
use Random;

config const debug = false;

proc makeBucketizerTest(in nBuckets:int, equalBuckets:bool, A, criterion) {
  var AA = A;

  var n = A.domain.size;

  if nBuckets >= n then
    nBuckets = n;
  var sampleStep = 14;
  if sampleStep < 1 || sampleStep*nBuckets > n then
    sampleStep = 1;
  var sampleSize = sampleStep*nBuckets;
  if sampleSize > n then
    sampleSize = max(1,n/2);

  if debug then
    writeln("nBuckets=", nBuckets,
            " equalBuckets=", equalBuckets,
            " sampleSize=", sampleSize,
            " sampleStep=", sampleStep);


  // select the sample
  putRandomSampleAtArrayStart(AA.domain.low, AA.domain.high, AA, sampleSize);

  // sort the sample
  sort(AA, criterion);
  var b = new SampleBucketizer(A.eltType);
  createSplittersFromSample(AA, b, criterion,
                            AA.domain.low,
                            sampleSize,
                            sampleStep,
                            nBuckets,
                            forceEqualBuckets=equalBuckets);


  return b;
}

proc testBucketizer(nBuckets:int, equalBuckets:bool) {
  var A = [0x1234567800000000:uint,
           0x1111111111111111:uint,
           0x2222222222222222:uint,
           0xffffffffffffffff:uint,
           0x1111111111111111:uint,
           0x1122334455667788:uint,
           0x1122334455667788:uint,
           0x1122334455667788:uint,
           0x0000000000000000:uint];

  var AUniqSorted = [0x0000000000000000:uint,
                     0x1111111111111111:uint,
                     0x1122334455667788:uint,
                     0x1234567800000000:uint,
                     0x2222222222222222:uint,
                     0xffffffffffffffff:uint];

  var criterion = defaultComparator;

  if debug then
    writeln("nBuckets=", nBuckets,
            " equalBuckets=", equalBuckets);

  var b = makeBucketizerTest(nBuckets, equalBuckets, A, criterion);

  if debug then
    writeln(b);

  {
    var i = 1;
    for (idx,bin) in b.classify(A, 1, A.domain.high, criterion, 0) {
      assert(idx == i);
      var bin2 = b.bucketForRecord(A[i], criterion, 0);
      if debug then
        writef("element %xt in bin %i %i\n", A[i], bin, bin2);
      assert(bin == bin2);
      i += 1;
    }
  }

  // test 1-splitter bucketizers
  for split in AUniqSorted {
    var AA:[1..1] uint;
    AA[1] = split;
    var b = new SampleBucketizer(A.eltType);
    createSplittersFromSample(AA, b, criterion, 1, 1, 1, 2,
                              forceEqualBuckets=equalBuckets);

    for (idx, bin) in b.classify(AUniqSorted, 1, AUniqSorted.domain.high,
                                 criterion, 0) {
      var val = AUniqSorted[idx];
      var bin2 = b.bucketForRecord(val, criterion, 0);
      assert(bin == bin2);

      var bin3 = 0;
      if equalBuckets {
        if val < split {
          bin3 = 0;
        } else if val == split {
          bin3 = 1;
        } else {
          bin3 = 2;
        }
      } else {
        if val <= split {
          bin3 = 0;
        } else {
          bin3 = 1;
        }
      }

      if debug then
        writef("split %xt val %xt bin %i bin3 %i\n", split, val, bin, bin3);

      assert(bin == bin3);
    }
  }

  // Check two-array partitioning
  var src = A;
  var dst = A;
  dst = 0;
  var counts = TwoArrayPartitioning.testBucketize(1, A.domain.high, dst, src,
                                              b, criterion, 0);

  assert(src.equals(A)); // src should not have been modified

  // Check that all of the elements are represented
  var dstSorted = dst;
  var ASorted = A;
  sort(dstSorted, criterion);
  sort(ASorted, criterion);
  assert(ASorted.equals(dstSorted));

  if debug then
    writef("bucketized %xt\n", dst);

  // Check two-array sorting
  var TwoSorted = A;
  TwoArraySampleSort.twoArraySampleSort(TwoSorted, criterion);
  if debug {
    writef("std sorted %xt\n", ASorted);
    writef("two sorted %xt\n", TwoSorted);
  }
  assert(isSorted(TwoSorted));
  assert(ASorted.equals(TwoSorted));
}
proc testBucketizer() {
  for i in 2..9 {
    testBucketizer(i, false);
    testBucketizer(i, true);
  }
}
testBucketizer();

proc testBucketizerRandomized(nelts:int, nBuckets:int, equalBuckets:bool) {

  if debug then
    writeln("testBucketizerRandomized(",
            " nelts=", nelts,
            " nBuckets=", nBuckets,
            " equalBuckets=", equalBuckets, ")");

  var A:[0..#nelts] uint;
  Random.fillRandom(A);

  var criterion = defaultComparator;
  var b = makeBucketizerTest(nBuckets, equalBuckets, A, criterion);

  var src = A;
  var dst = A;
  dst = 0;
  var counts = TwoArrayPartitioning.testBucketize(A.domain.low, A.domain.high,
                                              dst, src,
                                              b, criterion, 0);

  // Check that all of the elements are represented
  var dstSorted = dst;
  var ASorted = A;
  sort(dstSorted, criterion);
  sort(ASorted, criterion);
  assert(ASorted.equals(dstSorted));

  var total = + reduce counts;
  assert(total == nelts);

  // Check two-array sorting
  var TwoSorted = A;
  TwoArraySampleSort.twoArraySampleSort(TwoSorted, criterion);
  assert(ASorted.equals(TwoSorted));
}
proc testBucketizerRandomized() {
  for n in 1..30 {
    for nb in 2..n {
      testBucketizerRandomized(n, nb, false);
      testBucketizerRandomized(n, nb, true);
    }
  }
  for n in 30..1000 by 113 {
    for nb in 2..255 by 31 {
      if n >= nb {
        testBucketizerRandomized(n, nb, false);
        testBucketizerRandomized(n, nb, true);
      }
    }
    if n > 256 {
      testBucketizerRandomized(n, 256, false);
      testBucketizerRandomized(n, 256, true);
    }
  }
}
testBucketizerRandomized();
