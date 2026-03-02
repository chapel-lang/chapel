config const verbose = true;

use Sort.PartitioningUtility;

import IO;
import FileSystem;
import BlockDist;
import Random;
import Math;
import List;

// problem size for various tests
config const nAtomicTest = 100_000;
config const n = 1_000;
config const nBuckets = 8*numLocales*computeNumTasks(ignoreRunning=true);

proc testIsDistributed() {
  if verbose then
    writeln("testIsDistributed");

  const BlockDomain = BlockDist.blockDist.createDomain(0..100);
  const DefaultDomain = {0..100};

  assert(isDistributedDomain(BlockDomain));
  assert(!isDistributedDomain(DefaultDomain));
}

proc testBulkCopy() {
  if verbose then
    writeln("testBulkCopy");

  const Dom = BlockDist.blockDist.createDomain(0..<n);
  const A:[Dom] int = Dom;
  var LocA: [0..n+1] int = 0..n+1;

  // test local dst block src
  for size in [1, 10, 100, n] {
    if verbose then
      writeln("testing GETs with max size ", size);
    for i in 0..<n {
      LocA = -1;
      // copy 'size' bytes starting from 'i'
      var sz = size;
      if i+sz >= n {
        sz = n - i;
      }
      assert(A.domain.contains(i..#sz));
      assert(LocA.domain.contains(1..#sz));
      const srcRegion = i..#sz;
      if srcRegion.size > 0 {
        const dstRegion = 1..#srcRegion.size;
        bulkCopy(LocA, dstRegion, A, srcRegion);
        assert(LocA[0] == -1);
        for j in 0..<srcRegion.size {
          assert(LocA[1+j] == A[i+j]);
        }
        assert(LocA[dstRegion.high+1] == -1);
      }
    }
  }

  // test block dst local src
  var B = BlockDist.blockDist.createArray(0..n, int);
  const LocB:[0..n+1] int = 0..n+1;
  for size in [1, 10, 100, n] {
    if verbose then
      writeln("testing PUTs with max size ", size);
    for i in 1..<n {
      B = -1;
      // copy 'size' bytes starting from 'i'
      var sz = size;
      if i+sz >= n {
        sz = n - i;
      }
      assert(B.domain.contains(1..#sz));
      assert(LocB.domain.contains(i..#sz));
      const dstRegion = i..#sz;
      if dstRegion.size > 0 {
        const srcRegion = 1..#dstRegion.size;
        bulkCopy(B, dstRegion, LocB, srcRegion);
        assert(B[0] == -1);
        for j in 0..<dstRegion.size {
          assert(B[i+j] == LocB[1+j]);
        }
        assert(B[dstRegion.high+1] == -1);
      }
    }
  }

  // test block dst block src
  {
    var Dst = BlockDist.blockDist.createArray(0..n+1, int);
    var Src = BlockDist.blockDist.createArray(0..n+1, int);
    Src = 0..n+1;
    on Locales[numLocales - 1] {
      for size in [1, 10, 100, n] {
        if verbose then
          writeln("testing GET-PUTs with max size ", size);
        for i in 0..<n {
          Dst = -1;
          // copy 'size' bytes starting from 'i'
          var sz = size;
          if i+sz >= n {
            sz = n - i;
          }
          assert(Src.domain.contains(i..#sz));
          assert(Dst.domain.contains(1..#sz));
          const srcRegion = i..#sz;
          if srcRegion.size > 0 {
            const dstRegion = 1..#srcRegion.size;
            bulkCopy(Dst, dstRegion, Src, srcRegion);
            assert(Dst[0] == -1);
            for j in 0..<srcRegion.size {
              assert(Dst[1+j] == Src[i+j]);
            }
            assert(Dst[dstRegion.high+1] == -1);
          }
        }
      }
    }
  }

  // test dst remote src local
  {
    var Dst:[0..n+1] int;
    on Locales[numLocales - 1] {
      var Src:[0..n+1] int;
      Src = 0..n+1;
      for size in [1, 10, 100, n] {
        if verbose then
          writeln("testing non-block PUTs with max size ", size);
        for i in 0..<n {
          Dst = -1;
          // copy 'size' bytes starting from 'i'
          var sz = size;
          if i+sz >= n {
            sz = n - i;
          }
          assert(Src.domain.contains(i..#sz));
          assert(Dst.domain.contains(1..#sz));
          const srcRegion = i..#sz;
          if srcRegion.size > 0 {
            const dstRegion = 1..#srcRegion.size;
            bulkCopy(Dst, dstRegion, Src, srcRegion);
            assert(Dst[0] == -1);
            for j in 0..<srcRegion.size {
              assert(Dst[1+j] == Src[i+j]);
            }
            assert(Dst[dstRegion.high+1] == -1);
          }
        }
      }
    }
  }

  // test dst local src remote
  {
    const Src:[0..n+1] int = 0..n+1;
    on Locales[numLocales - 1] {
      var Dst:[0..n+1] int;
      for size in [1, 10, 100, n] {
        if verbose then
          writeln("testing non-block GETs with max size ", size);
        for i in 0..<n {
          Dst = -1;
          // copy 'size' bytes starting from 'i'
          var sz = size;
          if i+sz >= n {
            sz = n - i;
          }
          assert(Src.domain.contains(i..#sz));
          assert(Dst.domain.contains(1..#sz));
          const srcRegion = i..#sz;
          if srcRegion.size > 0 {
            const dstRegion = 1..#srcRegion.size;
            bulkCopy(Dst, dstRegion, Src, srcRegion);
            assert(Dst[0] == -1);
            for j in 0..<srcRegion.size {
              assert(Dst[1+j] == Src[i+j]);
            }
            assert(Dst[dstRegion.high+1] == -1);
          }
        }
      }
    }
  }
}

proc testActiveLocales() {
  if verbose then
    writeln("testActiveLocales");

  const Dom = BlockDist.blockDist.createDomain(0..<n);
  const nLocales = Dom.targetLocales().size;
  const nTasksPerLocale = computeNumTasks();
  const EmptyLocales:[1..0] locale;

  assert(computeActiveLocales(Dom, 0..<n).equals(Locales));
  assert(computeActiveLocales(Dom, 1..0).size == 0);

  for region in [0..0, 0..1, 1..10, 0..n/4, n/4..<n/2, 0..<n] {
    var expectActiveElts: [0..<n] int = 0;
    var expectActiveLocs:[0..<numLocales] int = 0;
    var activeElts: [0..<n] int = 0;
    var activeLocs:[0..<numLocales] int = 0;

    // compute 'expect' by iterating over a slice of the domain
    forall i in Dom with (+ reduce expectActiveLocs) {
      if region.contains(i) {
        expectActiveElts[i] = 1;
        expectActiveLocs[here.id] = 1;
      }
    }

    // compute 'got' by computing the active locales
    var mylocs = computeActiveLocales(Dom, region);

    // also check that computing active locales is a local task
    local {
      computeActiveLocales(Dom, region);
    }

    coforall loc in mylocs with (+ reduce activeLocs) {
      on loc {
        var intersect = Dom.localSubdomain()[region];
        assert(intersect.size > 0);
        for i in intersect {
          activeElts[i] = 1;
        }
        activeLocs[here.id] = 1;
      }
    }

    // we don't care about the counts for expectActiveLocs / activeLocs
    forall (a, b) in zip(expectActiveLocs, activeLocs) {
      if a > 1 then a = 1;
      if b > 1 then b = 1;
    }

    assert(expectActiveElts.equals(activeElts));
    assert(expectActiveLocs.equals(activeLocs));
  }
}

proc testDivideIntoTasks() {
  if verbose then
    writeln("testDivideIntoTasks");
  const Dom = BlockDist.blockDist.createDomain(0..<n);
  const nLocales = Dom.targetLocales().size;
  const nTasksPerLocale = computeNumTasks();
  var A:[Dom] int = -1; // store task IDs
  forall (activeLocIdx, taskIdInLoc, chunk)
  in divideIntoTasks(Dom, 0..<n, nTasksPerLocale) {
    for i in chunk {
      assert(A[i] == -1); // should not have any overlap
      A[i] = activeLocIdx*nTasksPerLocale + taskIdInLoc;
    }
  }
  // check that it works the same even if some tasks are running
  coforall i in 1..10 {
    var B:[Dom] int = -1;
    forall (activeLocIdx, taskIdInLoc, chunk)
    in divideIntoTasks(Dom, 0..<n, nTasksPerLocale) {
      for i in chunk {
        assert(B[i] == -1); // should not have any overlap
        B[i] = activeLocIdx*nTasksPerLocale + taskIdInLoc;
      }
    }
    assert(B.equals(A));
  }

  // count the number per task. It should be within 1% of the min/max.
  var countPerTask:[0..<nLocales*nTasksPerLocale] int;
  for x in A {
    countPerTask[x] += 1;
  }
  const minCount = min reduce countPerTask;
  const maxCount = max reduce countPerTask;
  if verbose then
    writeln("minCount = ", minCount, " maxCount = ", maxCount);
  assert(minCount <= maxCount &&
         maxCount <= minCount + 1 + 0.01*minCount);

  // check that the tasks divide the work in an increasing order,
  // that is, the task assignment in A is only increasing.
  // this is important for making the partition stable.
  for i in Dom {
    if i > 0 {
      assert(A[i-1] <= A[i]);
    }
  }

  // check that dividing with region on a single locale
  // only runs on one locale
  coforall loc in Dom.targetLocales() {
    on loc {
      const region:range = Dom.localSubdomain().dim(0);
      local {
        forall (activeLocIdx, taskIdInLoc, chunk)
        in divideIntoTasks(Dom, 0..<n, nTasksPerLocale) {
          // nothing to do here, the point was to check it is local
          var sum = 0;
          for i in chunk {
            sum += A[i]; // accessing to make sure it is local
          }
        }
      }
    }
  }
}

proc testDivideIntoPages(lower: integral, size: integral, alignment: integral) {
  if verbose then
    writeln("testDivideIntoPages(", lower, ",", size, ",", alignment, ")");

  var region = lower..#size;
  var ByTask: [region] atomic int;
  var nUnaligned = 0;

  // check serial
  for pageRange in divideIntoPages(region, alignment) {
    assert(pageRange.size > 0);
    // check alignment
    if pageRange.low % alignment != 0 {
      nUnaligned += 1;
    }
    // count for checking elements are all visited once
    for i in pageRange {
      ByTask[i].add(1);
    }
  }

  assert(nUnaligned <= 1);

  // each position should be visited exactly once
  for elt in ByTask {
    assert(elt.read() == 1);
  }

  // check parallel
  for i in region {
    ByTask[i].write(0);
  }
  nUnaligned = 0;
  forall pageRange in divideIntoPages(region, alignment)
  with (+ reduce nUnaligned) {
    assert(pageRange.size > 0);
    // check alignment
    if pageRange.low % alignment != 0 {
      nUnaligned += 1;
    }
    // count for checking elements are all visited once
    for i in pageRange {
      ByTask[i].add(1);
    }
  }

  assert(nUnaligned <= 1);

  // each position should be visited exactly once
  for elt in ByTask {
    assert(elt.read() == 1);
  }
}

proc testDivideIntoPages() {
  if verbose then
    writeln("testDivideIntoPages");

  const bigN = if numLocales == 1 then 1024*1024 else 10_000;
  for lower in [0, 100, 4096] {
    for size in [0, 9, 21, 100, bigN] {
      for alignment in [1, 16, 21, 1024] {
        testDivideIntoPages(lower, size, alignment);
      }
    }
  }

  // test also some cases with uints
  testDivideIntoPages(max(int):uint, 10_000:uint, 1024:uint);
  testDivideIntoPages(max(uint) - 10_000_000, 10_000:uint, 8000:uint);
}

proc main() throws {
  testIsDistributed();

  serial {
    testActiveLocales();
  }
  testActiveLocales();

  serial {
    testDivideIntoTasks();
  }
  testDivideIntoTasks();

  serial {
    testDivideIntoPages();
  }
  testDivideIntoPages();

  testBulkCopy();

  writeln("Partitioning Utilities OK");
}
