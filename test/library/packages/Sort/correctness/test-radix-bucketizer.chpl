use Sort;
use RadixSortHelp;
use Random;

config const debug = false;
config var seed = SeedGenerator.oddCurrentTime;

proc testBucketizer() {
  var A = [0x1234567800000000,
           0x1111111111111111,
           0x2222222222222222,
           0x2222222222222222,
           0x1111111111111111,
           0x1122334455667788,
           0x1122334455667788,
           0x1122334455667788,
           0x0000000000000000 ]:uint;
  var firstBytes = [0x12,
                    0x11,
                    0x22,
                    0x22,
                    0x11,
                    0x11,
                    0x11,
                    0x11,
                    0x00];

  var criterion = defaultComparator;
  var ASorted = A;
  sort(ASorted, criterion);
  assert(isSorted(ASorted));

  var b = new RadixBucketizer();
  assert(b.getNumBuckets() == 258);
  assert(b.getBinsToRecursivelySort().low == 1);
  assert(b.getBinsToRecursivelySort().high == 256);

  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 0*8) == 1+0x12);
  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 1*8) == 1+0x34);
  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 2*8) == 1+0x56);
  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 3*8) == 1+0x78);
  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 4*8) == 1+0xaa);
  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 5*8) == 1+0xbb);
  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 6*8) == 1+0xcc);
  assert(b.bucketForRecord(0x12345678aabbccdd:uint, criterion, 7*8) == 1+0xdd);

  // Check that signed numbers go in the right bin
  assert(b.bucketForRecord(max(int), criterion, 0*8) == 1+0xff);
  assert(b.bucketForRecord(min(int), criterion, 0*8) == 1+0x00);

  // checking for int(8)s
  {
    var bin = 0;
    for i in -128..127 {
      assert(b.bucketForRecord(i:int(8), criterion, 0*8) == 1+bin);
      bin += 1;
    }
  }

  {
    var i = A.domain.low;
    for (idx,bin) in b.classify(A, A.domain.low, A.domain.high, criterion, 0) {
      assert(idx == i);
      assert(bin == 1+firstBytes[idx]);
      assert(bin == b.bucketForRecord(A[i], criterion, 0));
      i += 1;
    }
  }

  // Check two-array partitioning
  {
    var src = A;
    var dst = A;
    dst = 0;
    var counts = TwoArrayPartitioning.testBucketize(A.domain.low,
                                                    A.domain.high,
                                                    dst, src, b, criterion, 0);

    var expect = [0x0000000000000000,
                  0x1111111111111111,
                  0x1111111111111111,
                  0x1122334455667788,
                  0x1122334455667788,
                  0x1122334455667788,
                  0x1234567800000000,
                  0x2222222222222222,
                  0x2222222222222222,
                 ]:uint;
    assert(counts[0     ] == 0);
    assert(counts[1+0x00] == 1);
    assert(counts[1+0x01] == 0);
    assert(counts[1+0x11] == 5);
    assert(counts[1+0x12] == 1);
    assert(counts[1+0x22] == 2);

    assert(dst.equals(expect));
    assert(src.equals(A)); // src should not have been modified

    // Check that all of the elements are represented
    var dstSorted = dst;
    sort(dstSorted, criterion);
    assert(ASorted.equals(dstSorted));
  }


  {
    // Check in-place sequential partitioning
    var dst = A;
    var counts2 = SequentialInPlacePartitioning.bucketize(A.domain.low,
                                                          A.domain.high,
                                                          dst, b, criterion,
                                                          0);

    var expect = [0x0000000000000000,
                  0x1122334455667788,
                  0x1111111111111111,
                  0x1122334455667788,
                  0x1111111111111111,
                  0x1122334455667788,
                  0x1234567800000000,
                  0x2222222222222222,
                  0x2222222222222222,
                 ]:uint;
    assert(counts2[0     ] == 0);
    assert(counts2[1+0x00] == 1);
    assert(counts2[1+0x01] == 0);
    assert(counts2[1+0x11] == 5);
    assert(counts2[1+0x12] == 1);
    assert(counts2[1+0x22] == 2);

    if debug {
      writef("expect %xt\n", expect);
      writef("dst %xt\n", dst);
    }

    assert(dst.equals(expect));

    // Check that all of the elements are represented
    var dstSorted = dst;
    sort(dstSorted, criterion);
    assert(ASorted.equals(dstSorted));
  }


  {
    // Check two-array sorting
    var TwoSorted = A;
    TwoArrayRadixSort.twoArrayRadixSort(TwoSorted, criterion);
    assert(isSorted(TwoSorted));
    assert(ASorted.equals(TwoSorted));
  }
}
testBucketizer();

proc testBucketizerRandomized(nelts:int, seed:int) {
  if debug then
    writeln("testBucketizerRandomized(", nelts, ", ", seed, ")");

  var A:[0..#nelts] uint;
  Random.fillRandom(A, seed=seed);

  var b = new RadixBucketizer();
  var criterion = defaultComparator;

  var ASorted = A;
  sort(ASorted, criterion);

  {
    var src = A;
    var dst = A;
    dst = 0;
    var counts = TwoArrayPartitioning.testBucketize(A.domain.low, A.domain.high,
                                                dst, src,
                                                b, criterion, 0);
    if debug {
      writef("A = %xt\n", A);
      writef("TwoArray Bucketized = %xt\n", dst);
    }


    // Check that top byte is in order.
    for i in A.domain {
      if i != A.domain.low {
        var topPrev = dst[i-1] >> 56;
        var topHere = dst[i] >> 56;
        assert(topPrev <= topHere);
        assert(counts[(1+topHere):int] > 0);
      }
    }

    // Check that all of the elements are represented
    var dstSorted = dst;
    sort(dstSorted, criterion);
    assert(ASorted.equals(dstSorted));

    var total = + reduce counts;
    assert(total == nelts);
  }

  {
    var dst = A;
    var counts = SequentialInPlacePartitioning.bucketize(
                                                A.domain.low, A.domain.high,
                                                dst,
                                                b, criterion, 0);
    if debug {
      writef("A = %xt\n", A);
      writef("SequentialInPlaceBucketized = %xt\n", dst);
    }

    // Check that top byte is in order.
    for i in A.domain {
      if i != A.domain.low {
        var topPrev = dst[i-1] >> 56;
        var topHere = dst[i] >> 56;
        assert(topPrev <= topHere);
        assert(counts[(1+topHere):int] > 0);
      }
    }

    // Check that all of the elements are represented
    var dstSorted = dst;
    sort(dstSorted, criterion);
    assert(ASorted.equals(dstSorted));

    var total = + reduce counts;
    assert(total == nelts);
  }


  // Check two-array sorting
  var TwoSorted = A;
  TwoArrayRadixSort.twoArrayRadixSort(TwoSorted, criterion);

  if debug {
    writef("ASorted = %xt\n", ASorted);
    writef("TwoSorted = %xt\n", TwoSorted);
  }

  assert(isSorted(TwoSorted));
  assert(ASorted.equals(TwoSorted));
}

testBucketizerRandomized(16,16);

proc testBucketizerRandomized() {
  for n in 1..30 {
    testBucketizerRandomized(n, seed);
    seed += 1;
  }
  for n in 30..1000 by 31 {
    testBucketizerRandomized(n, seed);
    seed += 1;
  }
}
testBucketizerRandomized();
