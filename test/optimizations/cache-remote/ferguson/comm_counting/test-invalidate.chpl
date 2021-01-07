use SysCTypes;
use CPtr;
use CommDiagnostics;
use Random;

pragma "insert line file info"
extern proc chpl_cache_invalidate(node:c_int, raddr:c_void_ptr,
                                  size: size_t);

config const verbose = false;
param testSize = 4096;
param lineSize = 64; // should match cache line size in bytes

var buf = c_aligned_alloc(uint(8), testSize, testSize);
// store buf as a _ddata so we can GET from it (otherwise always narrow)
const A = buf:_ddata(uint(8));

var rng = new RandomStream(int);

var expectSum = 0;
for i in 0..#testSize {
  A[i] = i:uint(8);
  expectSum += A[i];
}

{ // sanity checking the allocation
  var sum = 0;
  for i in 0..#testSize {
    assert(A[i] == i:uint(8));
    sum += A[i];
  }
  assert(sum == expectSum);
}

proc doRead(start:int, size:int) {
  resetCommDiagnosticsHere();
  startCommDiagnosticsHere();

  var sum = 0;
  for i in start..#size {
    var tmp = A[i];
    assert(tmp == i:uint(8));
    sum += tmp;
  }

  if start == 0 && size == testSize {
    assert(sum == expectSum);
  }

  stopCommDiagnosticsHere();
  var counts = getCommDiagnosticsHere();

  return counts;
}

// mode 0 -- read only start
// mode 1 -- read only invalid
// mode 2 -- read only end
//
// testOffset is the offset to a random position in start..#size
proc test_invalidate_mode(const start:int, const size:int, const mode:int,
                          const challengeOffset:int, const challengeSize:int) {

  const eltPtr:c_void_ptr = c_ptrTo(A[start]):c_void_ptr;
  // this is a workaround for this error:
  //   references to remote data cannot be passed to external routines like
  //   'c_pointer_return'

  on Locales[1] {
    // Read the first few pages of the array into the cache
    var coldCounts = doRead(0, testSize);

    if coldCounts.cache_get_hits + coldCounts.cache_get_misses == 0 {
      halt("Did you mean to compile this program with --cache-remote?");
    }

    // Read again. All of this data should be in the cache.
    var warmCounts = doRead(0, testSize);
    assert(warmCounts.cache_get_hits > 0);
    assert(warmCounts.cache_get_misses == 0);

    // Now do the invalidate call
    chpl_cache_invalidate(0:c_int, eltPtr, size:size_t);

    // Read up until 'start'
    if mode == 0 {
      var toStart = doRead(0, start);
      if verbose then
        writeln("toStart ", toStart);

      // if start is not cache-line-aligned then we expect
      // 1 miss in the toStart read. Otherwise 0.
      if (start % lineSize) != 0 {
        assert(toStart.cache_get_misses == 1);
      } else {
        assert(toStart.cache_get_misses == 0);
      }
    }

    // Read start..#size
    if mode == 1 {
      assert(start <= challengeOffset && challengeOffset < start+size);
      assert(start <= challengeOffset+challengeSize &&
                      challengeOffset+challengeSize <= start+size);

      var invalid = doRead(challengeOffset, challengeSize);
      if verbose then
        writeln("invalid ", invalid);

      assert(invalid.cache_get_misses > 0);

      if testSize == 1 then
        assert(invalid.cache_get_misses == 1);
    }

    // Read start+size..end
    if mode == 2 {
      var toEnd = doRead(start+size, testSize-(start+size));
      if verbose then
        writeln("toEnd ", toEnd);

      // if start+size is not cache-line-aligned then we expect
      // 1 miss in the toEnd read. Otherwise 0.
      if ((start+size) % lineSize) != 0 {
        assert(toEnd.cache_get_misses == 1);
      } else {
        assert(toEnd.cache_get_misses == 0);
      }
    }
  }
}

proc test_invalidate(const start:int, const size:int) {
  // skip invalid test cases
  if start+size > testSize then
    return;

  if verbose then
    writeln("test_invalidate(", start, ", ", size, ")");

  // check reading before start
  test_invalidate_mode(start, size, 0, -1, -1);

  // check reading 1 random byte within start..#size
  var challengeOffset = rng.getNext(start, start+size-1);
  test_invalidate_mode(start, size, 1, challengeOffset, 1);
  // check reading the entire region start..#size
  test_invalidate_mode(start, size, 1, start, size);

  // check reading after start+size
  test_invalidate_mode(start, size, 2, -1, -1);
}

writeln("Checking 1 byte invalidations");
// invalidate 1 byte -> invalidate 1 line -> miss on 1st get in that line
for i in 0..#testSize by 23 {
  test_invalidate(i, 1);
}
for i in 0..#testSize by 64 {
  test_invalidate(i, 1);
}

writeln("Checking 1 line invalidation");
// invalidate 1st cache line -> miss on 1st get in that line
for i in 0..#testSize by 64 {
  test_invalidate(i, 64);
}

writeln("Checking 2 line invalidation");
for i in 0..#testSize by 64 {
  test_invalidate(i, 128);
}


writeln("Checking some odd starts and sizes");
for start in 0..#testSize by 23 {
  test_invalidate(start, 2);
  test_invalidate(start, 17);
  test_invalidate(start, 64);
  test_invalidate(start, 99);
  test_invalidate(start, 128);
  test_invalidate(start, 250);
}

c_free(buf);
