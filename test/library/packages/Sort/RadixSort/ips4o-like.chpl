use Random;
use Sort;
//use BlockDist;
use Time;
use Barriers;
use PeekPoke;

config const seed = SeedGenerator.oddCurrentTime;
config const skew = false;
config param debug = true;
config param checks = true;
config param maxLogBuckets = 8; // not counting equality buckets.
config param classifyUnrollFactor = 7;
config const maxInline = 16; // 1024 ?
config const oversampleFactor = 0.2;
config const maxSubtask = 100*1024;
config const minBlockSize = 1024;
config const maxBlockSize = 1<<31;
config const equalBucketThreshold = 5;

// TODO: put use block dist back in
// figure out why we're only using 1 task

// TODO: I'm concerned about read/write pointers overflowing.
// In particular, can't each worker in the block permutation
// do decrement on each bucket?

// Based on IPS4o
// https://github.com/SaschaWitt/ips4o/tree/master/ips4o
// https://arxiv.org/pdf/1705.02257.pdf

// * sample sort addresses load imbalance for skewed distributions
//  in radix sort
// * consider alternating sample sort passes and radix sort passes
// * radix sorter could be aided by knowning [min, max) element
// * distributed sorting ideas
//    * base case is to copy to one node, sort it there, copy back
//      copy to the node with the most data, splitting ties higher
//    * sort local and nonlocal blocks at the same time with cobegin
//    * use > numLocales blocks so that the number of local bins
//      is statistically likely to be higher so that the local sorts
//      can cover the smaller number of slower distributed bin sorts

// compute logarithm base 2 rounded down
proc log2int(n: int) {
  if n <= 0 then
    return 0;
  return (numBits(uint) - 1 - msbRadixSortClz(n:uint):int);
}

proc computeLogBucketSize(n: int) {
  const LogBuckets = 8;
  const BaseCaseSize = 16;
  const BaseCaseMultiplier = 16;
  const SingleLevelThreshold = maxInline * LogBuckets;
  const TwoLevelThreshold = SingleLevelThreshold * LogBuckets;
  var ret = 0;
  if n <= SingleLevelThreshold {
    ret = log2int(n / maxInline / 2);
  } else if n <= TwoLevelThreshold {
    ret = (log2int(n / maxInline / 2)+1)/2;
  } else {
    ret = maxLogBuckets;
  }

  ret = max(1, ret); // make sure it's at least 1
  ret = min(maxLogBuckets, ret); // make sure it's at most maxLogBuckets.

  return ret;
}
proc chooseSampleStep(n:int, logNumBuckets:int) {
  var oversample = min(1.0, oversampleFactor * log2int(n));
  var step = max(1, oversample:int);
  return step;
}

record SplitterBucketizer {
  type eltType;

  // filled from 1 to num_buckets_
  var storage: c_array(eltType, 1<<maxLogBuckets);
  // filled from 0 to num_buckets, last one is duplicated
  var sortedStorage: c_array(eltType, 1<<maxLogBuckets);

  var logBuckets: int;
  var numBuckets: int;
  var equalBuckets: bool;

  proc getNumBuckets() {
    return numBuckets * (1 + equalBuckets:int);
  }

  // Build the tree from the sorted splitters
  // logBuckets does not account for equalBuckets.
  proc build(logBuckets: int, equalBuckets: bool) {
    this.logBuckets = logBuckets;
    this.numBuckets = 1 << logBuckets;
    this.equalBuckets = equalBuckets;

    var numSplitters = this.numBuckets - 1;
    // Copy the last element
    sortedStorage[numSplitters] = sortedStorage[numSplitters-1];
    build(0, numSplitters, 1);
  }
  // Recursively builds the tree
  proc build(left: int, right: int, pos: int) {
    var mid = left + (right - left) / 2;
    storage[pos] = sortedStorage[mid];
    if 2*pos < numBuckets {
      build(left, mid, 2*pos);
      build(mid, right, 2*pos + 1);
    }
  }
  proc splitter(i:int) const ref : eltType {
    return storage[i];
  }
  proc sortedSplitter(i:int) const ref : eltType {
    return sortedStorage[i];
  }

  proc bucketForRecord(a, criterion, startbit) {
    var b = 1;
    for lg in 0..#logBuckets {
      b = 2*b + (chpl_compare(splitter(b), a, criterion) < 0):int;
    }
    if equalBuckets {
      b = 2*b + (chpl_compare(a, sortedSplitter(b-numBuckets), criterion) >= 0):int;
    }
    return b - (if equalBuckets then 2*numBuckets else numBuckets);
  }
  proc classify(A, start_n, end_n, criterion, startbit) {
    for param paramLogBuckets in 1..maxLogBuckets {
      for param paramEqualBuckets in false..true {
        if paramLogBuckets == logBuckets && paramEqualBuckets == equalBuckets {
          return classifyUnrolled(paramLogBuckets, paramEqualBuckets,
                                  A, start_n, end_n, criterion, startbit);
        }
      }
    }
    halt("Invalid configuration");
  }
  // yields (index, bucket index) for A[start_n..end_n]
  iter classifyUnrolled(param paramLogBuckets, param paramEqualBuckets,
                        A, start_n, end_n, criterion, startbit) {
    param paramNumBuckets = 1 << (paramLogBuckets + paramEqualBuckets:int); 
    var b:c_array(int, classifyUnrollFactor);

    var cur = start_n;
    // Run the main (unrolled) loop
    while cur <= end_n-(classifyUnrollFactor-1) {
      for param i in 0..classifyUnrollFactor-1 {
        b[i] = 1;
      }
      for param lg in 0..paramLogBuckets-1 {
        for param i in 0..classifyUnrollFactor-1 {
          b[i] = 2*b[i] + (chpl_compare(splitter(b[i]), A[cur+i],criterion)<0):int;
        }
      }
      if paramEqualBuckets {
        for param i in 0..classifyUnrollFactor-1 {
          b[i] = 2*b[i] +
                 (chpl_compare(A[cur+i],
                         sortedSplitter(b[i] - paramNumBuckets/2),criterion)>=0):int;
        }
      }
      for param i in 0..classifyUnrollFactor-1 {
        yield (cur+i, b[i]-paramNumBuckets);
      }
      cur += classifyUnrollFactor;
    }
    // Handle leftover
    while cur <= end_n {
      var bk:int;
      for param lg in 0..paramLogBuckets-1 {
        bk = 2*bk + (chpl_compare(splitter(bk), A[cur], criterion)<0):int;
      }
      if paramEqualBuckets {
        bk = 2*bk + (chpl_compare(A[cur],
                             sortedSplitter(bk - paramNumBuckets/2),criterion)>=0):int;
      }
      yield (cur, bk);
      cur += 1;
    }
  }
}


proc createSplittersFromSample(A,
                               criterion,
                               start_n: int,
                               sampleSize: int,
                               sampleStep: int,
                               in numBuckets: int,
                               out usedBuckets: int,
                               out usedEqualBuckets: bool) {

  var splitterBucketizer = new SplitterBucketizer(A.eltType);

  // Create the splitters
  ref splitters = splitterBucketizer.sortedStorage;
  var arrayIndex = start_n + sampleStep - 1;
  var splitterIndex = 0;
  splitters[splitterIndex] = A[arrayIndex];
  for i in 2..numBuckets-1 {
    arrayIndex += sampleStep;
    // Skip duplicates
    if chpl_compare(splitters[splitterIndex], A[arrayIndex], criterion)!=0 {
      splitterIndex += 1;
      splitters[splitterIndex] = A[arrayIndex];
    }
  }

  // Check for duplicate splitters
  var uniqueSplitters = 1 + splitterIndex;
  var myUseEqualBuckets = numBuckets - 1 - uniqueSplitters >= equalBucketThreshold;

  // Fill the array to the next power of two
  var logBuckets = log2int(uniqueSplitters) + 1;
  numBuckets = 1 << logBuckets;
  for i in uniqueSplitters+1 .. numBuckets-1 {
    splitterIndex += 1;
    splitters[splitterIndex] = A[arrayIndex];
  }

  // Build the tree
  splitterBucketizer.build(logBuckets, myUseEqualBuckets);

  usedBuckets = numBuckets * (1 + myUseEqualBuckets:int);
  usedEqualBuckets = myUseEqualBuckets;

  writeln("Computed splitters ", splitterBucketizer);
  return splitterBucketizer;
}

proc computeBlockSizeShift(type eltType, size:int) {
  // computing the block size 'b' in the paper
  const blockTargetSizeBytes = 2048;
  var eltSizeEstimate = 8;
  if fixedWidth(eltType) > 0 then
    eltSizeEstimate = fixedWidth(eltType);
  var blockSizeMin = blockTargetSizeBytes / eltSizeEstimate;
  if blockSizeMin <= eltSizeEstimate then
    blockSizeMin = 1;
  // compute log2(blockSizeMin)
  var blockSizeMinU = blockSizeMin:uint;
  var shift = (numBits(uint) - msbRadixSortClz(blockSizeMinU - 1)):int;

  // Make sure the number of blocks is <= 2**31
  while (size / (1 << shift)) >= (1 << 31) {
    shift += 1;
  }

  return shift;
}
proc computeBlockSize(start_n, end_n, A:[]) {
  var shift = computeBlockSizeShift(A.eltType, end_n - start_n + 1);
  var blockSize = 1 << shift;

  if blockSize < minBlockSize then
    blockSize = minBlockSize;

  if blockSize > maxBlockSize then
    blockSize = maxBlockSize;

  return blockSize;
  // TODO: create a block size to avoid poor load balance for Cyclic
  // arrays with power of two numbers of locales.
  // TODO: try using a Block distribution's block size and see if it
  // works for us
  // TODO: consider shifted Block distributions
}

// TODO: These shallowCopy functions should handle Block,Cyclic arrays
proc shallowCopy(ref A, dst, src, nElts) {
  // Ideally this would just be
  //A[dst..#nElts] = A[src..#nElts];

    var size = (nElts:size_t)*c_sizeof(A.eltType);
    c_memcpy(c_ptrTo(A[dst]), c_ptrTo(A[src]), size);
    /*
  if A._instance.isDefaultRectangular() {
    var size = (nElts:size_t)*c_sizeof(A.eltType);
    c_memcpy(c_ptrTo(A[dst]), c_ptrTo(A[src]), size);
  } else {
    var ok = chpl__bulkTransferArray(/*dst*/ A._instance, {dst..#nElts},
                                     /*src*/ A._instance, {src..#nElts});
    if !ok {
      halt("Bulk transfer failed");
    }
  }*/

}
proc shallowCopy(ref DstA, dst, ref SrcA, src, nElts) {
  // Ideally this would just be
  //DstA[dst..#nElts] = SrcA[src..#nElts];

    var size = (nElts:size_t)*c_sizeof(DstA.eltType);
    c_memcpy(c_ptrTo(DstA[dst]), c_ptrTo(SrcA[src]), size);
    /*
  if DstA._instance.isDefaultRectangular() && SrcA._instance.isDefaultRectangular() {
    var size = (nElts:size_t)*c_sizeof(DstA.eltType);
    c_memcpy(c_ptrTo(DstA[dst]), c_ptrTo(SrcA[src]), size);
  } else {
    var ok = chpl__bulkTransferArray(/*dst*/ DstA._instance, {dst..#nElts},
                                     /*src*/ SrcA._instance, {src..#nElts});
    if !ok {
      halt("Bulk transfer failed");
    }
  }*/
}


record Buffer {
  type eltType;
  var ptr:c_ptr(eltType);
  var end:c_ptr(eltType); // aka start of next bucket
  var blocksWritten:int;
  // Number of elements in buffer.
  proc size(blockSize:int) {
    return blockSize - (end - ptr);
  }
}
class LocalBuffers {
  // This is really per-sort-problem information, duplicated here
  type eltType;
  var nBuckets:int;
  var blockSize:int;

  // Information about this thread's stripe.
  // Stripe consists of full blocks and then empty blocks.
  // These are in blocks.
  var firstBlock:int;
  var nFullBlocks:int;
  var firstEmptyBlock:int;
  var nEmptyBlocks:int;

  // TODO: would it help to make this c_array ?
  var buckets:[0..#nBuckets] Buffer(eltType);
  // This allocation needs to be contiguous.
  // bucket i is stored in allocation[i*blockSize..#blockSize]
  var allocation:[0..#(nBuckets*blockSize)] eltType;
  var swap0:[0..#blockSize] eltType;
  var swap1:[0..#blockSize] eltType;
}

// The atomic buffer pointers indicate, for each bucket:
//  * Blocks to the left of 'write' are correctly placed
//  * Blocks between 'write' and 'read' (inclusive) are unprocessed
//  * Blocks to the right of max(write, read+1) (inclusive) are empty
//
record AtomicBufferPointers {
  // TODO: this needs to be a 128-bit atomic
  var wr: atomic uint;
  var numReading: atomic int;

  proc packOffset(w:uint(32), r:uint(32)) {
    var wu = w:uint;
    var ru = r:uint;
    var val = (wu << 32) | ru;
    return val;
  }

  // TODO: return 128-bit value
  proc pack(w:int, r:int):uint {
    var w32 = w.safeCast(int(32));
    if w32 < 0 then
      w32 = -1;
    var r32 = r.safeCast(int(32));
    if r32 < 0 then
      r32 = -1;
    var w32u = (2000+w32):uint(32);
    var r32u = (2000+r32):uint(32);
    return packOffset(w32u, r32u);
  }
  // returns (write pointer, read pointer)
  proc unpack(val:uint):(int, int) {
    const mask = 0xffffffff;
    var r32u = val & mask;
    var w32u = val >> 32;
    var r32 = r32u:int(32);
    var w32 = w32u:int(32);
    r32 -= 2000;
    w32 -= 2000;
    if r32 < 0 then
      r32 = -1;
    if w32 < 0 then
      w32 = -1;
    //writef("Unpack of %xu resulted in (%i, %i)\n", val, w32, r32);
    return (w32:int, r32:int);
  }

  proc set(w:int, r:int) {
    var val = pack(w, r);

    if debug {
      var (write, read) = unpack(val);
      assert(write == w && read == r);
    }

    wr.write(val);
    numReading.write(0, memoryOrder.relaxed);
  }

  // Gets the write pointer.
  proc peekWrite() {
    var val = wr.peek();
    var (w, r) = unpack(val);
    return w;
  }
  proc peek() {
    var val = wr.peek();
    return unpack(val);
  }

  // Gets (write pointer, read pointer) and increments write pointer
  proc incWrite() {
    var val = packOffset(1, 0);
    var got = wr.fetchAdd(val, memoryOrder.relaxed);
    return unpack(got);
  }

  // Gets (write pointer, read pointer), decreases the read pointer,
  // and increases the read counter
  proc decRead() {
    // numReading.add should not move after following fetch_sub
    // b/c another thread could write to our block, isReading() returning false
    numReading.fetchAdd(1, memoryOrder.acquire);
    var val = packOffset(0, 1);
    var got = wr.fetchSub(val, memoryOrder.relaxed);
    return unpack(got);
  }

  // Decreases the read counter
  proc stopRead() {
    // synchronizes with threads wanting to write to this bucket
    numReading.fetchSub(1, memoryOrder.release);
  }

  // Returns true if any thread is currently reading this bucket
  proc isReading() {
    // synchronize with threads currently reading from this bucket
    return numReading.read(memoryOrder.acquire) != 0;
  }
}



record RadixBucketizer {
  // aka k in the paper
  proc getNumBuckets() {
    return (1 << 8) + 2; // +2 for end-before and end-after bins
  }
  proc bucketForRecord(a, criterion, startbit) {
    const (bin, _) = binForRecord(a, criterion, startbit);
    return bin;
  }
  // yields (index, bucket index) for A[start_n..end_n]
  iter classify(A, start_n, end_n, criterion, startbit) {
    var cur = start_n;
    while cur <= end_n-(classifyUnrollFactor-1) {
      for param j in 0..classifyUnrollFactor-1 {
        yield (cur+j, bucketForRecord(A[cur+j], criterion, startbit));
      }
      cur += classifyUnrollFactor;
    }
    while cur <= end_n {
      yield (cur, bucketForRecord(A[cur], criterion, startbit));
      cur += 1;
    }
  }
}

proc copyifchecks(A:[], start_n:int, end_n:int) {
  if checks {
    return A[start_n..end_n];
  } else {
    return 0;
  }
}

proc putSampleAtArrayStart(in start_n:int, end_n:int, A:[], in numSamples:int) {
  use Random;
  var randNums = createRandomStream(0, eltType=int, parSafe=false);
  while numSamples > 0 {
    numSamples -= 1;

    var offset = randNums.getNext(start_n, end_n);
    if offset != start_n then
      A[start_n] <=> A[offset];

    start_n += 1;
  }
}

proc mysort(start_n:int, end_n:int,
            A:[],
            criterion,
            startbit:int, endbit:int) {
  // Terminology Note:
  //  this code uses the term "bin" and "bucket" interchangeably

  // Shortcut for small problem sizes.
  if start_n >= end_n {
    return;
  }

  if checks==false && 1 + end_n - start_n <= maxInline {
    shellSort(A, criterion, start=start_n, end=end_n);
    return;
  }

  var copy = copyifchecks(A, start_n, end_n);

  // Step 1: Sample (or not, for radix sorting)
  // We could use the sample to decide if we will radix sort
  // or sample sort.
  var n = 1 + end_n - start_n;
  var logNumBuckets = computeLogBucketSize(n);
  var numBuckets = 1 << logNumBuckets;
  var sampleStep = chooseSampleStep(n, logNumBuckets);
  var sampleSize = sampleStep * numBuckets - 1;

  if sampleSize >= n {
    if debug then
      writeln("Reducing sample size because it was too big");
    sampleSize = max(1, n/2);
  }

  // select the sample
  putSampleAtArrayStart(start_n, end_n, A, sampleSize);
  // sort the sample
  mysort(start_n, start_n + sampleSize - 1, A, criterion, 0, endbit=max(int));

  var usedBuckets = 0;
  var usedEqualBuckets = false;
  const bucketizer = createSplittersFromSample(A,
                                               criterion,
                                               start_n,
                                               sampleSize,
                                               sampleStep,
                                               numBuckets,
                                               usedBuckets,
                                               usedEqualBuckets);

  //const bucketizer = new RadixBucketizer();


  if debug {
    writeln("in mysort(", start_n, ", ", end_n, ")");
    writeln("BEFORE PARTITION");
    writef("%xt\n", A[start_n..end_n]);
  }

  var nBuckets = bucketizer.getNumBuckets();
  var alwaysSortSubs = 1 + end_n - start_n <= maxInline * nBuckets;

  //writeln("Partition ", 1+end_n-start_n, " ", A[start_n..end_n]);

  // Step 2: Partition
  const offsets =
    parallelInPlacePartition(start_n, end_n, A, bucketizer,
                             criterion, startbit, endbit,
                             alwaysSortSubs, maxInline);

  if debug {
    writeln("AFTER PARTITION");
    writef("%xt\n", A);
  }

  // Step 3: Recurse
  var todoBig:[0..nBuckets] (int,int);
  var nTodoBig = 0;
  var todoMedium:[0..nBuckets] (int,int);
  var nTodoMedium = 0;
  const subbits = startbit + 8;

  for bin in 1..nBuckets-2 { // bin 0 and last bin represent end of keys
    const bin_start = offsets[bin];
    const bin_end = offsets[bin+1]-1;
    const num = 1 + bin_end - bin_start;
    if num <= 1 || startbit >= endbit {
      // do nothing
    } else if num <= maxInline {
      // Already sorted in partitioning code (for cache reuse reasons)
    } else if num <= maxSubtask {
      // sort it in a parallel sub-task
      todoMedium[nTodoMedium] = (bin_start, bin_end);
      nTodoMedium += 1;
    } else {
      todoBig[nTodoBig] = (bin_start, bin_end);
      nTodoBig += 1;
    }
  }

  // Sort the small ones first, in parallel
  forall i in 0..#nTodoMedium {
    var (bin_start, bin_end) = todoMedium[i];
    // TODO: copy to one locale?
    msbRadixSort(bin_start, bin_end, A,
                 criterion,
                 subbits, endbit,
                 new MSBRadixSortSettings(alwaysSerial=true));
  }

  // Then recursively sort the big ones
  // (each of these recursions is parallel)
  for i in 0..#nTodoBig {
    var (bin_start, bin_end) = todoBig[i];
    mysort(bin_start, bin_end, A, criterion, subbits, endbit);
  }

  if checks {
    var end = end_n;
    if end_n - start_n > 100 {
      end = start_n + 100;
    }

    for i in start_n+1..end_n {
      var cmp = chpl_compare(A[i-1], A[i], criterion);
      if cmp > 0 {
        writef("Error: not sorted properly at i=%i A[i-1]=%xt A[i]=%xt in" +
               "start=%i end=%i\n", i, A[i-1], A[i], start_n, end_n);
        writef("%xt\n", A[start_n..end]);

        // Halt. Note, this is only intended to be called by unit testing.
        halt("failed checkSorted");
      }
    }

    var sorted = copy;
    sort(sorted);

    for i in start_n..end_n {
      if sorted[i] != A[i] {
        writef("Error: not sorted properly at i=%i sorted[i]=%xt A[i]=%xt in" +
               "start=%i end=%i\n", i, sorted[i], A[i], start_n, end_n);
        writef("A %xt\n", A[start_n..end]);
        writef("sorted %xt\n", sorted[start_n..end]);
        halt("failure");
      }
    }
  }
}


record BlockInfo {
  var start:int;
  var end: int;
  var owner: locale;
}

proc blocksForTask(blocksPerLocale:int, blocksPerTask:int,
                   lid: int, tid: int) {
  // This could conceivably do more with locality...
  // e.g. find blocks within particular locales, when the input is block
  // distributed. (But if the input is Block distributed, this might line
  // up OK already).
  const locStart = lid*blocksPerLocale;
  const taskStart = locStart + tid*blocksPerTask;
  return taskStart..#blocksPerTask;
}


proc parallelInPlacePartition(start_n: int, end_n: int,
                              A:[],
                              const bucketizer, const criterion,
                              startbit: int, endbit: int,
                              alwaysSortAfter: bool,
                              sortAfterIfSmallerEqThan: int) {

  // How many buckets are we sorting into? aka 'k' in the paper
  // When sample sorting, the paper describes adjusting this to
  // make the expected size of the final buckets reasonable (say 32,
  // rather than 2).
  const nBuckets = bucketizer.getNumBuckets();

  type eltType = A.eltType;
  const nLocales = A.targetLocales().size; //max(1, A.targetLocales().size);
  const nTasksPerLocale = if dataParTasksPerLocale>0 then dataParTasksPerLocale
                                                     else here.maxTaskPar;

  assert(nLocales > 0);
  assert(nTasksPerLocale > 0);

  // aka 'b'
  const blockSize = computeBlockSize(start_n, end_n, A);
  assert(blockSize > 0);
  const nBlocks = divceil(end_n - start_n + 1, blockSize);
  // Because we store block indexes in 32-bit pointers (to fit 2 into 64-bits)
  assert(nBlocks <= (1<<31));

  const blocksPerLocale = divceil(nBlocks, nLocales);
  const blocksPerTask = divceil(blocksPerLocale, nTasksPerLocale);
  const nTasks = nLocales*nTasksPerLocale; // aka 't' in the paper
  const DistributedTasks = {0..#nTasks};
//                             dmapped Block(boundingBox={0..#nTasks},
//                                           targetLocales=A.targetLocales());

  if debug {
    writeln("nTasksPerLocale ", nTasksPerLocale);
    writeln("blockSize ", blockSize);
    writeln("blocksPerTask ", blocksPerTask);
    writeln("nLocales ", nLocales);
    writeln("nTasks ", nTasks);
    writeln("nBuckets ", nBuckets);
  }

  var AtomicCounts:[0..#nBuckets] atomic int;
  var AllBuffers:[DistributedTasks] owned LocalBuffers(eltType);
  var GlobalAllBucketStartIndices:[0..nBuckets] int;

  // Atomic bucket pointers
  // TODO: consider alternative of 1 atomic per block
  // TODO: distributed
  var BucketPointers:[0..#nBuckets] AtomicBufferPointers;

  // TODO: Would it help for blockSize to be param?
  // TODO: For Block distribution
  //   - Block per-locale chunk size should be a multiple of blockSize
  //   - Need to find starting index
  var barrier = new Barrier(nTasks);

  // Overflow block
  var overflow:[0..#blockSize] eltType;
  var overflowBucket = -1;

  // The coforall here allows for per-task AllBuffers
  // but I would prefer to write it in a different way.
  // Additionally it allows easy setup of per-task data...
  coforall (loc,lid) in zip(A.targetLocales(),0..) with (ref overflowBucket)
    do on loc {

    coforall tid in 0..#nTasksPerLocale with (ref overflowBucket)
    {
      // Allocate the buffer pointers
      const id = lid*nTasksPerLocale + tid;

      // Step 1: Local Classification
      // Input array A is viewed as an array of blocks each containing b
      // elements (except the first and last blocks, which may be short).
      // Divide these blocks into A into t stripes of equal size,
      // one for each thread. Each thread works
      // with a local array of k buffer blocks - one for each bucket.


      // Each thread works with a local array of k buffer blocks

      // Allocates to have an extra element at the end to simplify
      // bounds checking.
      var LocalCounts:[0..nBuckets] int;

      // This one will be filled in later.
      var BucketStartOffsets:[0..nBuckets] int;

      // TODO: Could these LocalBuffers arrays just be local
      // arrays here? Would that save on pointer indirections?


      // Create the buffer here
      var bb = new owned LocalBuffers(eltType=eltType,
                                      nBuckets=nBuckets,
                                      blockSize=blockSize);

      // Set up the per-bucket pointers
      for bin in 0..#nBuckets {
        bb.buckets[bin].ptr = c_ptrTo(bb.allocation[bin*blockSize]);
        bb.buckets[bin].end = bb.buckets[bin].ptr + blockSize;
      }

      var buf = bb.borrow();

      assert(AllBuffers[id] == nil);
      // Save the buffers for later
      AllBuffers[id] = bb;

      // aka myBlocks
      var myStripe = blocksForTask(blocksPerLocale, blocksPerTask, lid, tid);
      if debug {
        writeln("task ", id, " stripe ", myStripe);
      }

      var outputBlock = myStripe.low;

      if debug {
        if id == 0 then writeln("local classification");
      }

      // Each thread scans its stripe, moving elements into the local
      // buffer blocks. If a buffer block is already full, write it at
      // to the local stripe, starting at the front.
      for block in myStripe {
        var start = start_n + block*blockSize;
        var end = start + blockSize; // exclusive
        if end > end_n+1 then
          end = end_n+1;

        // Ignore empty blocks
        if start >= end then
          continue;

        var curSize = end - start;
        if debug {
          writeln("task ", id, " considering block ", block, " start ", start, " end ", end);
        }
        assert(curSize >= 0);

        // Check if the block is local
        var islocal = false;
        if A.localSubdomain()[start..#curSize].size == curSize {
          islocal = true;
        } else {
          // Block is not local, so copy to swap0
          //buf.swap0[0..#curSize] = A[start..#curSize];
          shallowCopy(buf.swap0, 0, A, start, curSize);
          // And clear out whatever was there...
          // TODO: shallow copy
          //A[start..#curSize] = buf.swap1[0..#curSize];
        }

//        ref elts = if islocal then A.localSlice(start..#curSize)
//                              else buf.swap0.localSlice(0..#curSize);
        //ref elts = A[start..#curSize];


        //for i in start..#curSize {
        //  ref elt = A[i];
        //  const bucketi = bucketizer.bucketForRecord(elt, criterion, startbit);

        for (i, bucketi) in bucketizer.classify(A,
                                                start, start+curSize-1,
                                                criterion, startbit) {
          ref elt = A[i];

          ref bucket = buf.buckets[bucketi];
          // If the buffer is already full, write it back to the array
          if bucket.ptr == bucket.end {
            var startptr = bucket.ptr - blockSize;

            var outputOffset = start_n + outputBlock*blockSize;

            if debug {
              writef("task %i flushing bucket buffer %i to %i\n",
                     id, bucketi, outputBlock*blockSize);
            }

            // TODO: use swap if it's local
            // TODO: shallow copy
            A[outputOffset..#blockSize] =
              buf.allocation[bucketi*blockSize..#blockSize];

            outputBlock += 1;
            bucket.blocksWritten += 1;
            bucket.ptr = startptr;

            // Now empty the buffer
            buf.allocation[bucketi*blockSize..#blockSize] = buf.swap1;
          }

          LocalCounts[bucketi] += 1;

          // Move the element into the corresponding buffer block
          // TODO: shallow copy
          if debug {
            writef("task %i moving an element to bucket buffer %i -- %xt\n",
                   id, bucketi, elt);
          }
          bucket.ptr.deref() = elt;
          bucket.ptr += 1;

          if debug {
            writef(
                "task %i after moving, buffer contains %i elements %xt %xt\n",
                    id, bucket.size(blockSize),
                    (AllBuffers[id].buckets[bucketi].ptr - 1).deref(),
                    AllBuffers[id].allocation[bucketi*blockSize +
                        AllBuffers[id].buckets[bucketi].size(blockSize) - 1]);
          }
        }
      }

      // Make a note of the first empty block in our stripe for later
      {
        var blocksWritten = outputBlock - myStripe.low;
        var nMyBlocks = myStripe.size;
        buf.firstBlock = myStripe.low;
        buf.nFullBlocks = blocksWritten;
        buf.firstEmptyBlock = buf.firstBlock + buf.nFullBlocks;
        buf.nEmptyBlocks = nMyBlocks - blocksWritten;
      }

      // Clear swap0
      // TODO: shallow copy
      //buf.swap0 = buf.swap1;

      // Update AllCounts
      forall bucketi in 0..#nBuckets {
        // (Note, forall might allow communication optimization)
        AtomicCounts[bucketi].add(LocalCounts[bucketi], memoryOrder.relaxed);
      }

      // Make sure every task finishes the local classification & counting
      barrier.barrier();

      // Create local copy of atomic counts
      // (no need to do GETs or atomics on it after this point)
      var AllCounts:[0..nBuckets] int;
      forall bucketi in 0..#nBuckets {
        // (Note, forall might allow communication optimization)
        AllCounts[bucketi] = AtomicCounts[bucketi].read(memoryOrder.relaxed);
      }
      AllCounts[nBuckets] = 0;

      proc printState(early=false) {
        writeln("State dump. blockSize=", blockSize);

        for bucketi in 0..#nBuckets {
          // print out any non-empty data in A
          var (w, r) = BucketPointers[bucketi].peek();

          var start = BucketStartOffsets[bucketi];
          var end = BucketStartOffsets[bucketi+1];

          if early == false && start >= end then
            continue;

          var write = w*blockSize;
          var read = r*blockSize;
          if early == false {
            writef("bucket %i start=%i n=%i  write=%i read=%i\n",
                   bucketi, start, end - start, w, r);

            // Blocks to the left of w are correctly placed
            for i in start..write-blockSize {
              if i < end {
                writef("bucket %i block %i index %i %xt\n",
                       bucketi, i/blockSize, i,
                       A[start_n+i]);
              }
            }
            // Blocks between w and r (inclusive) are unprocessed
            for i in write..(max(write-blockSize,read)) {
              if 0 <= i && i < end {
                writef("bucket %i block %i index %i unprocessed %xt\n",
                       bucketi, i/blockSize, i,
                       A[start_n+i]);
              }
            }

            // Blocks to the right of max(w, r+1) (inclusive) are empty
            for i in (max(write, read+blockSize))..(end-1) {
              if 0 <= i && i < end {
                writef("bucket %i block %i index %i empty\n",
                       bucketi, i/blockSize, i);
              }
            }
          }

          for id in 0..#nTasks {
            var count = AllBuffers[id].buckets[bucketi].size(blockSize);
            var cur = AllBuffers[id].buckets[bucketi].ptr - count;
            for i in 0..#count {
              writef("bucket %i task %i %xt\n",
                     bucketi, id, cur.deref());
              cur += 1;
            }
          }
        }

        if overflowBucket >= 0 {
          for e in overflow {
            writef("overflow %xt\n", e);
          }
        }
      }


      // Compute the start of each bucket
      // These are offsets, so the minimum is 0 (and not start_n)
      // (Note, this is expected to be serial)
      // Bucket i has start at BucketStartOffsets[i]+start_n
      // and end at BucketStartOffsets[i+1]+start_n.
      BucketStartOffsets = (+ scan AllCounts) - AllCounts;


      // Just one task copies the bucket offsets to the global
      if id == 0 {
        GlobalAllBucketStartIndices = BucketStartOffsets + start_n;
      }

      if debug {
        barrier.barrier();
        if id == 0 {
          writeln("AllCounts ", AllCounts);
          writeln("BucketStartOffsets ", BucketStartOffsets);
          writeln("After local classification");
          printState(early=true);
        }
        barrier.barrier();
      }


      // Input is 0-based, i.e. the first element is 0 (aka A[start_n]).
      proc alignOffsetToNextBlock(offset:int) {
        var nextblock = (offset + blockSize - 1) / blockSize;
        var nextoffset = nextblock * blockSize;

        return nextoffset;
      }
      proc alignOffsetToPrevBlock(offset:int) {
        var prevblock = offset / blockSize;
        var blockoffset = prevblock * blockSize;

        return blockoffset;
      }
      // Returns the block containing this offset, rounding up
      // in the case of uneven division
      proc nextBlock(offset:int) {
        var nextblock = (offset + blockSize - 1) / blockSize;
        return nextblock;
      }

      //var myOffsets = (myStripe.low*blockSize)..(myStripe.high*blockSize-1);

      // TODO: consider specializing for small input (as the C++ code does)
      // might skip block movement & empty block movement.
      // (But, I suspect it's better in that case to use different strategy)

      // Step 2: Empty Block Movement
      //
      // The goal is to establish the invariant that all buckets are
      // full blocks followed by empty blocks. Additionally, this phase
      // sets up the BucketPointers for the block permutation phase.

      // After classification, a stripe consists of full blocks followed
      // by empty blocks. This means that the invariant above already holds
      // for all buckets except those that cross stripe boundaries.
      //
      // The following cases exist:
      // 1) The bucket is fully contained within one stripe.
      //    In this case, nothing needs to be done, just set the bucket
      //    pointers.
      // 2) The bucket starts in stripe i and ends in stripe i+1.
      //    In this case, thread i moves full blocks from the end of
      //    the bucket (from the stripe of thread i+1) to fill the holes at
      //    the end of its stripe.
      // 3) The bucket starts in stripe i, crosses more than one stripe
      //    boundary, and ends in stripe i+k. This is an extension of case 2.
      //    In this case, multiple threads work on the same bucket. Each
      //    thread is responsible for filling the empty blocks in its stripe.
      //    The left-most thread will take the right-most blocks. Therefore,
      //    we count how many blocks are fetched by threads to our left
      //    before merging our own blocks.

      if debug {
        if id == 0 then
          writeln("empty block movement");
      }

      { // empty block movement
        var myBegin = myStripe.low * blockSize;
        var myEnd = (myStripe.high + 1) * blockSize; // exclusive
        var nextBegin = myEnd;
        var nextEnd = nextBegin + blocksPerTask*blockSize;
        /*var n = 1 + end_n - start_n;
        myBegin = min(myBegin, n);
        myEnd = min(myEnd, n);
        nextBegin = min(nextBegin, n);
        nextEnd = min(nextEnd, n);*/

        /*
        // Find range of buckets that start in this stripe
        proc findFirstBucketIn(stripeBegin:int, stripeEnd:int) {
          var i = 0;
          // Find the first bucket in this stripe
          while i < nBuckets {
            if stripeBegin <= BucketStartOffsets[i] &&
                              BucketStartOffsets[i] < stripeEnd {
              break;
            }
            i += 1;
          }
          return i;
        }
        var bucketRangeStart = findFirstBucketIn(myBegin, myEnd);
        var bucketRangeEnd = findFirstBucketIn(nextBegin, nextEnd); // exclusive
        /*if id == nTasks-1 {
          bucketRangeEnd = nBuckets;
        }*/

        */

        proc computeStart(in i:int) {
          while i < nBuckets &&
                alignOffsetToNextBlock(BucketStartOffsets[i]) < myBegin {
            i += 1;
          }
          return i;
        }
        proc computeEnd(in i:int) {
          if id == nTasks-1 {
            return nBuckets;
          }
          while i < nBuckets &&
                alignOffsetToNextBlock(BucketStartOffsets[i]) < myEnd {
            i += 1;
          }
          return i;
        }
        var bucketRangeStart = computeStart(0);
        var bucketRangeEnd = computeEnd(bucketRangeStart); // exclusive

        assert(bucketRangeStart <= nBuckets);
        assert(bucketRangeEnd <= nBuckets);

        var bucketEnd =
          alignOffsetToNextBlock(BucketStartOffsets[bucketRangeEnd]);
        //bucketEnd = min(bucketEnd, n);
        var lastBucketIsOverlapping = bucketEnd > myEnd;
        var myFirstEmptyBlock = buf.firstEmptyBlock * blockSize;

        if debug {
          writeln("task ", id, " myStripe ", myStripe,
                  " myBegin=", myBegin, " myEnd=", myEnd,
                  " bucketRangeStart=", bucketRangeStart,
                  " bucketRangeEnd=", bucketRangeEnd,
                  " BucketStartOffsets[bucketRangeStart]=", BucketStartOffsets[bucketRangeStart],
                  " BucketStartOffsets[bucketRangeEnd]=", BucketStartOffsets[bucketRangeEnd],
                  " bucketEnd=", bucketEnd,
                  " lastBucketIsOverlapping=", lastBucketIsOverlapping,
                  " myFirstEmptyBlock=", myFirstEmptyBlock);
        }

        // Case 1)
        for b in bucketRangeStart..(bucketRangeEnd-1-lastBucketIsOverlapping) {
          var start = alignOffsetToNextBlock(BucketStartOffsets[b]);
          var stop = alignOffsetToNextBlock(BucketStartOffsets[b+1]);
          var read = stop;
          if myFirstEmptyBlock <= start {
            // Bucket is completely empty
            read = start;
          } else if myFirstEmptyBlock < stop {
            // Bucket is partially empty
            read = myFirstEmptyBlock;
          }
          var w = start/blockSize;
          var r = (read-blockSize)/blockSize;
          if debug {
            writeln("task ", id, " setting bucket pointers ",
                    b, " write=", w, " read=", r);
          }
          BucketPointers[b].set(w, r);
        }

        // Cases 2) and 3)
        if lastBucketIsOverlapping {
          var overlappingBucket = bucketRangeEnd - 1;
          var bucketStart =
            alignOffsetToNextBlock(BucketStartOffsets[overlappingBucket]);

          // Count how many filled blocks are in this bucket
          var flushedElementsInBucket = 0;
          if bucketStart < myBegin {
            var prevId = id - 1;
            // Iterate over stripes completely contained in this bucket
            while bucketStart < AllBuffers[prevId].firstBlock*blockSize {
              flushedElementsInBucket += AllBuffers[prevId].nFullBlocks*blockSize;
              prevId -= 1;
            }
            // Count blocks in stripe where bucket starts
            var eb = AllBuffers[prevId].firstEmptyBlock * blockSize;
            // Check if there are any filled blocks in this bucket
            if eb > bucketStart then
              flushedElementsInBucket += eb - bucketStart;
          }

          // Threads to our left will move this many blocks (0 if we are
          // the left-most thread)
          var elementsReserved = 0;
          if myBegin > bucketStart {
            // Thread to the left of us get priority
            elementsReserved = myBegin - bucketStart - flushedElementsInBucket;
            // Count how many elements we flushed into this bucket
            flushedElementsInBucket += myFirstEmptyBlock - myBegin;
          } else if myFirstEmptyBlock > bucketStart {
            // We are the left-most thread
            // Count how many elements we flushed into this bucket
            flushedElementsInBucket += myFirstEmptyBlock - bucketStart;
          }

          // Find stripe containing the last block of this bucket (off by one).
          // Also continue counting how many filled blocks are in this bucket.
          var readFromThread = id + 1;
          while readFromThread < nTasks &&
                bucketEnd > AllBuffers[readFromThread].firstBlock*blockSize {
            var eb = min(AllBuffers[readFromThread].firstEmptyBlock*blockSize, bucketEnd);
            flushedElementsInBucket += eb - AllBuffers[readFromThread].firstBlock*blockSize;
            readFromThread += 1;
          }

          // After moving blocks, will be the first empty block in this bucket
          var firstEmptyBlockInBucket = bucketStart + flushedElementsInBucket;

          if debug {
            writeln("task ", id, " firstEmptyBlockInBucket=",
                    firstEmptyBlockInBucket);
          }

          // This is the range of blocks we want to fill
          var writePtr = max(myFirstEmptyBlock, bucketStart);
          var writePtrEnd = min(firstEmptyBlockInBucket, myEnd);

          // Read from other stripes until we filled our blocks
          while writePtr < writePtrEnd {
            readFromThread -= 1;
            // This is range of blocks we can read from stripe readFromThread
            var readPtr = min(AllBuffers[readFromThread].firstEmptyBlock*blockSize, bucketEnd);
            var readRangeSize = readPtr - AllBuffers[readFromThread].firstBlock*blockSize;
            // Skip reserved blocks
            if elementsReserved >= readRangeSize {
              elementsReserved -= readRangeSize;
              continue;
            }
            readPtr -= elementsReserved;
            readRangeSize -= elementsReserved;
            elementsReserved = 0;

            // Move blocks
            var size = min(readRangeSize, writePtrEnd - writePtr);
            if debug {
              writeln("task ", id, " boving data A[start+",
                      readPtr-size..#size,
                      "] = A[start+", writePtr..#size, "]");
            }
            //A[start_n+writePtr..#size] = A[start_n+readPtr-size..#size];
            shallowCopy(A, start_n+writePtr, start_n+readPtr-size, size);
            writePtr += size;
          }

          // Set bucket pointers if the bucket starts in this stripe
          if myBegin <= bucketStart {
            var w = bucketStart/blockSize;
            var r = (firstEmptyBlockInBucket - blockSize)/blockSize;
            if debug {
              writeln("task ", id, " setting bucket pointers (2) ",
                      overlappingBucket, " write=", w, " read=", r);
            }
            BucketPointers[overlappingBucket].set(w, r);
          }
        }
      }

      // Make sure every task finishes empty block movement
      barrier.barrier();

      // Step 3: Block Permutation
      // Rearrange full blocks in the array so that they appear in the correct
      // order.
      // Each bucket has:
      //   * (partial start block)
      //   * blocks in the correct bucket
      //   * unprocessed blocks
      //   * empty blocks
      //   * (partial end block)

      if debug {
        barrier.barrier();
        if id == 0 {
          writeln("After local classification and move empty");
          printState();
          writeln("block permutation");
        }
        barrier.barrier();
      }

      // Tries to read a block from readBucket, returns -1 if it did not,
      // otherwise returns a bin number.
      proc classifyAndReadBlock(readBucket:int, swap0:[]) {
        ref bp = BucketPointers[readBucket];

        // First, check if the bucket has any more data
        {
          var (write, read) = bp.peek();
          if (read < 0 || read < write) {
            // No more blocks in this bucket
            return -1;
          }
        }

        var (write, read) = bp.decRead();

        if debug {
          writeln("in classifyAndReadBlock ",
                  readBucket, " write=", write, " read=", read);
        }

        if (read < 0 || read < write) {
          // No more blocks in this bucket
          bp.stopRead();
          return -1;
        }

        // Read the block
        var start = start_n + read*blockSize;
        //swap0 = A[start..#blockSize];
        shallowCopy(swap0, 0, A, start, blockSize);
        bp.stopRead();

        // Compute the destination bucket by classifying the first element
        const bucketi = bucketizer.bucketForRecord(swap0[0], criterion, startbit);
        return bucketi;
      }

      // currentSwap is a full swap buffer
      // Finds a slot for the block in the swap buffer. May or may not
      // read another block instead.
      proc swapBlock(dstBucket:int, currentSwap:[], nextSwap:[]) {

        var read: int;
        var write: int;
        ref bp = BucketPointers[dstBucket];
        var newDstBucket:int;

        do {
          (write, read) = bp.incWrite();
          if write > read {
            // Destination block is empty
            if write >= nBlocks-1 {
              // Out-of-bounds, write to overflow buffer instead
              overflow = currentSwap;
              overflowBucket = dstBucket;
              return -1;
            }
            // Make sure no other task is currently reading this block
            on bp {
              while (bp.isReading()) {
                chpl_task_yield();
              }
            }
            // Write the block
            var start = start_n + write*blockSize;
            //A[start..#blockSize] = currentSwap;
            shallowCopy(A, start, currentSwap, 0, blockSize);
            return -1;
          }
          //writeln("in swap block");
          //writeln(write);
          //writeln(start_n);

          // Otherwise, write points to an unprocessed block
          // Check if that block needs to be moved
          var start = start_n + write*blockSize;
          const bucketi = bucketizer.bucketForRecord(A[start], criterion, startbit);
          newDstBucket = bucketi;
          // Do nothing if the block doesn't need to be moved
        } while (newDstBucket == dstBucket);

        // Swap blocks
        var start = start_n + write*blockSize;
        //ref Ablock = A[start..#blockSize];
        //nextSwap = Ablock;
        //Ablock = currentSwap;
        shallowCopy(nextSwap, 0, A, start, blockSize);
        shallowCopy(A, start, currentSwap, 0, blockSize);

        // Currentswap is now an empty swap buffer, but nextSwap is full
        return newDstBucket;
      }

      // Distribute starting points of threads
      var readBucket:int = (id * nBuckets / nTasks) % nBuckets;

      // Go through all buckets
      for count in 0..#nBuckets {
        //writeln("task id ", id, " considering bucket ", count);
        var dstBucket:int;
        // Try to read a block
        while true {
          dstBucket = classifyAndReadBlock(readBucket, buf.swap0);
          if dstBucket == -1 then
            break;
          // and write it to the correct bucket
          var whichSwapIsCurrent = false;
          while true {
            dstBucket = swapBlock(dstBucket,
                          if whichSwapIsCurrent then buf.swap1 else buf.swap0,
                          if whichSwapIsCurrent then buf.swap0 else buf.swap1);
            if dstBucket == -1 then
              break;
            // Read another block, keep going
            whichSwapIsCurrent = !whichSwapIsCurrent;
          }
        }
        readBucket = (readBucket + 1) % nBuckets;
      }

      // Make sure every task finishes block permutation
      barrier.barrier();

      if debug {
        barrier.barrier();
        if id == 0 {
          writeln("After block permutation");
          printState();
          writeln("cleanup");
        }
        barrier.barrier();
      }


      // Step 4: Cleanup

      // Divide buckets among tasks. Each task writes (only) the data
      // in a bucket that it owns.
      const nBucketsPerTask = divceil(nBuckets, nTasks);
      // TODO: this might result in all buckets being on 1 locale,
      // if there nBuckets <= nTasksPerLocale. Should we use a more Cyclic
      // strategy? (Or just run this with nBuckets ~= nTasks ?)
      var myBucketsStart = id * nBucketsPerTask;
      var myBucketsEnd = (id + 1) * nBucketsPerTask; // exclusive
      myBucketsStart = min(myBucketsStart, nBuckets);
      myBucketsEnd = min(myBucketsEnd, nBuckets);

      // Save margins
      // each thread reads the head of the first bucket in the next thread
      // into one of its swap buffers. ('head' is the partial block at the
      // begininng of a bucket). It's not in the right place because it would
      // have been permuted somewhere based on the first element (which is
      // not from this bucket if it's a partial block).
      proc saveMargins() {
        var lastBucket = myBucketsEnd;
        var tail = BucketStartOffsets[myBucketsEnd];
        var end = alignOffsetToNextBlock(tail);

        // Do nothing if there's no overlap
        if tail == end || end > (end_n - start_n) then
          return (-1, 0);

        // Find the bucket this last belongs to
        {
          var startOfLastBlock = end - blockSize;
          var lastStart = 0;
          do {
            lastBucket -= 1;
            if lastBucket < 0 then break;
            lastStart = BucketStartOffsets[lastBucket];
          } while lastStart > startOfLastBlock;
        }

        if lastBucket < 0 then
          return (-1, 0);

        // Check if the last block is empty
        var write = BucketPointers[lastBucket].peekWrite();
        if write*blockSize < end then
          return (-1, 0);

        // Read excess elements, if necessary
        tail = BucketStartOffsets[lastBucket + 1];
        var n = end - tail;
        //buf.swap0[0..#n] = A[start_n+tail .. #n];
        shallowCopy(buf.swap0, 0, A, start_n+tail, n);
        return (lastBucket, n);
      }

      var saveMarginResult = saveMargins();
      var saveMarginBucket = saveMarginResult(1);
      var saveMarginInSwap = saveMarginResult(2);

      // Make sure every task finishes save margins
      barrier.barrier();

      // Fill empty elements
      // Elements to put somewhere include:
      //  * the head of any bucket
      //    (save margins put the last one for this thread in the swap buffer)
      //  * partially filled buffers (in any task) from local classification
      //  * the overflow buffer
      // Empty elements include:
      //  * any head of a bucket read in save margins
      //  * empty blocks to the right of the bucket's write pointer (inclusive)
      for bucketi in myBucketsStart..myBucketsEnd-1 {

        // Get bucket information
        var bucketStart = BucketStartOffsets[bucketi];
        var bucketEnd = BucketStartOffsets[bucketi+1]; // exclusive
        var bucketWrite = BucketPointers[bucketi].peekWrite() * blockSize;

        // Destination where elements can be written
        var dst = start_n + bucketStart;
        // How many elements are available in the head of the bucket?
        var remaining = alignOffsetToNextBlock(bucketStart) - bucketStart;

        // Don't try to move data around for empty buckets.
        if bucketStart == bucketEnd then continue;

        if debug {
          writeln("task ", id,
                  " bucketi ", bucketi,
                  " bucketStart ", bucketStart,
                  " bucketEnd ", bucketEnd,
                  " bucketWrite ", bucketWrite,
                  " dst ", dst,
                  " remaining ", remaining);
        }

        assert(dst <= end_n);

        if bucketi == overflowBucket {
          // If there's overflow, handle it

          if debug then
            writeln("task ", id, " Handling overflow");

          // Overflow buffer has been written => write pointer
          // must be at end of bucket.
          assert(alignOffsetToNextBlock(bucketEnd) == bucketWrite);

          // There must be space for at least blockSize elements
          assert((bucketEnd - (bucketWrite - blockSize)) + remaining >= blockSize);
          var tailSize = blockSize - remaining;

          // Fill head
          //A[dst..#remaining] = overflow[0..#remaining];
          shallowCopy(A, dst, overflow, 0, remaining);

          // Write remaining elements into tail
          dst = start_n + bucketWrite - blockSize;
          //A[dst..#tailSize] = overflow[remaining..#tailSize];
          shallowCopy(A, dst, overflow, remaining, tailSize);
          dst += tailSize;

          remaining = max(int);

          // TODO: reset overflow if it has been moved out of
        } else if bucketi == saveMarginBucket && saveMarginInSwap > 0 {
          // Did we save this in save margins?

          if debug then
            writeln("task ", id, " Handling saved margin");

          // Bucket of last block in this thread's area => write swap buffer
          // All elements from the buffer must fit
          assert(saveMarginInSwap <= remaining);

          //A[dst..#saveMarginInSwap] = buf.swap0[0..#saveMarginInSwap];
          shallowCopy(A, dst, buf.swap0, 0, saveMarginInSwap);
          dst += saveMarginInSwap;
          remaining -= saveMarginInSwap;

          // TODO: reset swap0 if it has been moved out of
        } else if bucketWrite > bucketEnd &&
                  bucketEnd - bucketStart > blockSize
        {
          if debug then
            writeln("task ", id, " Handling other case");

          // Final block has been written => move excess elements to head
          assert(alignOffsetToNextBlock(bucketEnd) == bucketWrite);

          var headSize = bucketWrite - bucketEnd;
          // Must fit, no other empty space left
          assert(headSize <= remaining);

          // Write to head
          //A[dst..#headSize] = A[start_n+bucketEnd .. #headSize];
          shallowCopy(A, dst, start_n+bucketEnd, headSize);
          dst += headSize;
          remaining -= headSize;
        }

        // Write elements from buffers
        for otherId in 0..#nTasks {
          var b = AllBuffers[otherId].borrow();
          var count = b.buckets[bucketi].size(blockSize);
          var src = bucketi*blockSize;

          // Do nothing if there were no elements.
          if count == 0 then
            continue;

          if debug then
            writeln("task ", id, " found ", count, " elements in bucket ",
                    bucketi, " from task ", otherId);

          if count <= remaining {
            if debug then
              writeln("task ", id, " doing copy to ", dst..#count);

            //A[dst..#count] = b.allocation[src..#count];
            shallowCopy(A, dst, b.allocation, src, count);
            dst += count;
            remaining -= count;
          } else {
            if remaining > 0 {
              if debug then
                writeln("task ", id, " doing copy2 to ", dst..#remaining);

              //A[dst..#remaining] = b.allocation[src..#remaining];
              shallowCopy(A, dst, b.allocation, src, remaining);
              src += remaining;
              count -= remaining;
            }
            remaining = max(int);

            dst = start_n + bucketWrite;

            if debug then
              writeln("task ", id, " doing copy3 to ", dst..#count);

            //A[dst..#count] = b.allocation[src..#count];
            shallowCopy(A, dst, b.allocation, src, count);
            dst += count;
          }

          // TODO: reset b.allocation if elements were moved from
        }

        // TODO: Perform base-case sorting immediately after the bucket
        // has been completely filled in the cleanup phase to have better
        // cache reuse.
        if alwaysSortAfter ||
           (bucketEnd - bucketStart <= sortAfterIfSmallerEqThan) {
          // TODO: copy to one locale?
          shellSort(A, criterion,
                    start=start_n + bucketStart,
                    end=start_n + bucketEnd - 1);
        }
      }
    } // coforall creating tasks within a local
  } // coforoall-on creating tasks on locales

  if checks {
    // Check that each key is in the right bucket.

    assert(GlobalAllBucketStartIndices[0] == start_n);
    assert(GlobalAllBucketStartIndices[nBuckets] == end_n+1);
    for bucketi in 1..#nBuckets {
      assert(GlobalAllBucketStartIndices[bucketi-1] <=
             GlobalAllBucketStartIndices[bucketi]);
    }

    for bucketi in 0..#nBuckets {
      var start = GlobalAllBucketStartIndices[bucketi];
      var end = GlobalAllBucketStartIndices[bucketi+1];
      for i in start..end-1 {
        const got = bucketizer.bucketForRecord(A[i], criterion, startbit);
        assert(got == bucketi);
      }
    }
  }

  return GlobalAllBucketStartIndices;
}

/*
// the src array has the input and will be used for scratch space
// the dst array has the output
proc distSortWithScratchSpace(start_n:int, end_n:int,
                                  dst:[], src:[],
                                  criterion,
                                  startbit:int, endbit:int) {

  if !src.hasSingleLocalSubdomain() {
    compilerError("Distributed sort needs single local subdomain");
  }

  if debug {
    writeln("in distributed sort ", start_n, "..", end_n);
  }

  const nlocales = src.targetLocales().size;
  const tasksPerLocale = 2 * dataParTasksPerLocale;
  const nbins = nlocales * tasksPerLocale;

  const samplesPerLocale = 8 * ntasks;
  const nsamples = nlocales * samplesPerLocale;
  var Sample:[0..#nsamples] src.eltType;

  // Compute nbins splitters

  // Step 1: Each locale sorts local portions and gathers
  // nbins elements into the global sample array
  coforall (loc,lid) in zip(src.targetLocales(),0..) {
    on loc do {
      ref localSlice = src.localSlice(src.localSubdomain()[start_n..end_n]);
      // Sort the local slice
      sort(localSlice, criterion);
      // Gather the splitter material
      //  -- gather nbins equal elements
      forall i in 0..samplesPerLocale {
        Sample[samplesPerLocale*lid + i] = localSlice[i];
      }

      if debug {
        for bin in 0..radix {
          if localCounts[bin] > 0 then
            writeln("localCounts[", bin, "]=", localCounts[bin]);
        }
      }
      // Now store the counts into the global counts array
      for (bin,count) in zip(localCounts.domain, localCounts) {
        globalCounts[bin*ntasks + tid] = count;
      }
    }
  }

  if debug {
    writef("after bucketize, src is %xt\n", src[start_n..end_n]);
  }

  // Step 2: scan
  var globalEnds = (+ scan globalCounts) + start_n;

  if debug {
    for i in 0..#countsSize {
      if globalCounts[i] != 0 {
        writeln("globalCounts[", i, "]=", globalCounts[i]);
        writeln("globalEnds[", i, "]=", globalEnds[i]);
      }
    }
  }

  // Step 3: distribute the keys to the dst array according
  // to the globalEnds.
  // In particular, bin i from task j should be stored into
  // globalEnds[i*ntasks+j-1] .. globalEnds[i*ntasks+j] - 1
  // (because the scan is inclusive)
  coforall (loc,tid) in zip(src.targetLocales(),0..) {
    on loc do {
      const localSubdomain = src.localSubdomain()[start_n..end_n];
      // Compute localOffsets array based on the counts here
      // This could be written as a scan expression...
      var localOffsets:[0..radix] int;
      {
        var offset = localSubdomain.low;
        for bin in 0..radix {
          localOffsets[bin] = offset;
          offset += globalCounts[bin*ntasks + tid];
        }
      }

      forall bin in 0..radix {
        var size = globalCounts[bin*ntasks + tid];
        if size > 0 {
          var localStart = localOffsets[bin];
          var localEnd = localStart + size - 1;
          var globalStart = if bin*ntasks+tid > 0
                            then globalEnds[bin*ntasks+tid-1]
                            else start_n;
          var globalEnd = globalEnds[bin*ntasks+tid] - 1;
          if debug {
            writeln("bin ", bin, " tid ", tid,
                    " dst[", globalStart, "..", globalEnd, "] = src[",
                    localStart, "..", localEnd, "]");
          }
          // Copy the elements!!!
          dst[globalStart..globalEnd] = src[localStart..localEnd];
        }
      }
    }
  }
  if debug {
    writef("after distribute, dst is %xt\n", dst[start_n..end_n]);
  }

  // Step 4: if any bin is now totally within a locale,
  // sort it on that locale.
  var doneBins:[0..radix] bool;

  coforall (loc,tid) in zip(src.targetLocales(),0..) {
    on loc do {
      forall bin in 0..radix {
        var globalStart = if bin*ntasks > 0
                          then globalEnds[bin*ntasks-1]
                          else start_n;
        var globalEnd = globalEnds[bin*ntasks+ntasks-1] - 1;
        if globalStart < globalEnd {
          const localSubdomain = dst.localSubdomain()[start_n..end_n];
          const curDomain = {globalStart..globalEnd};
          const intersect = curDomain[localSubdomain];
          if curDomain == intersect { // curDomain.isSubset(localSubdomain)
            if debug {
              writeln("bin ", bin, " tid ", tid,
                      " local sorting ", globalStart, "..", globalEnd);
            }
            // Great! Just sort it locally.
            msbRadixSort(globalStart, globalEnd, dst.localSlice(curDomain),
                         criterion,
                         startbit+8, // since we already distributed by first 8
                         endbit,
                         new MSBRadixSortSettings());
            doneBins[bin] = true;
          }
        }
      }
    }
  }
  if debug {
    writef("after local sorts, dst is %xt\n", dst[start_n..end_n]);
  }

  // Step 5: Recurse to sort any remaining bins that
  // are not totally sorted.
  for bin in 1..radix-1 {
    var globalStart = if bin*ntasks > 0
                      then globalEnds[bin*ntasks-1]
                      else start_n;
    var globalEnd = globalEnds[bin*ntasks+ntasks-1] - 1;
    if globalStart < globalEnd && !doneBins[bin] {
      msbRadixSortWithScratchSpace(globalStart, globalEnd,
                                   src, dst,
                                   criterion,
                                   startbit+8, endbit);
      // Copy back to the dst array
      dst[globalStart..globalEnd] = src[globalStart..globalEnd];
    }
  }
  if debug {
    writef("after recursive sorts, dst is %xt\n", dst[start_n..end_n]);
  }
}
*/

assert(log2int(-1) == 0);
assert(log2int(0) == 0);
assert(log2int(1) == 0);
assert(log2int(2) == 1);
assert(log2int(3) == 1);
assert(log2int(4) == 2);
assert(log2int(5) == 2);
assert(log2int(6) == 2);
assert(log2int(7) == 2);
assert(log2int(8) == 3);

proc simpletestcore(input:[], seed:int) {
  var localCopy = input;
  shuffle(localCopy, seed);

  const blockDom = input.domain; //newBlockDom(input.domain);
  var A: [blockDom] uint = localCopy;

  assert(isSorted(input));

  if debug {
    writef("input %xt\n", input);
  }

  mysort(blockDom.low, blockDom.high, A, defaultComparator, 0, max(int));

  if debug {
    writef("output %xt\n", A);
  }

  forall i in input.domain {
    if input[i] != A[i] {
      writeln("Sorting error in simpletest seed=", seed,
              " element ", i, " in incorrect order");
      writef("input was %xt\n", input);
      halt("failure");
    }
  }
}
proc simpletestloop(input:[]) {
  for i in 1..10 {
    simpletestcore(input, seed+i);
  }
}
proc simpletest(input:[]) {
  var size = input.domain.size;
  simpletestloop(input);
  simpletestloop(input.reindex(1..#size));
  simpletestloop(input.reindex(0..#size));
  simpletestloop(input.reindex(2..#size));
  simpletestloop(input.reindex(-1..#size));
}

simpletest([0:uint]);
simpletest([0:uint, 0:uint, 0:uint, 0:uint]);
simpletest([0x0:uint,
            0xff00000000000000:uint,
            0xff00000000888888:uint,
            0xffffffffffffffff:uint]);
simpletest([0x01:uint,
            0x02:uint,
            0x03:uint,
            0x04:uint,
            0x05:uint,
            0x06:uint]);
simpletest([0x01:uint,
            0x01:uint,
            0x03:uint,
            0x03:uint,
            0x05:uint,
            0x05:uint]);
simpletest([0xda524a179483bc7:uint,
            0xdd908e56b88cfd5:uint,
            0xde0546a7b5c06e9:uint]);

proc randomtest(n:int) {
  const blockDom = {0..#n}; //newBlockDom({0..#n});
  var A: [blockDom] uint;

  if skew {
    var rng = createRandomStream(seed, eltType=uint);
    A = rng.choice([1:uint, 17:uint,
                    100:uint, 0xffff:uint, 0xffffffffffffffff:uint], n);
  } else {
    fillRandom(A, seed);
  }
  var localCopy:[0..#n] uint = A;

  writeln("Sorting ", n, " elements");

  var timer:Timer;
  timer.start();
  mysort(0, n-1, A, defaultComparator, 0, max(int));
  timer.stop();

  writeln("Sorted ", n, " elements in ", timer.elapsed(), " seconds",
          " (", 8.0*n/timer.elapsed()/1024.0/1024.0, " MiB/s");

  writeln("Verifying");
  sort(localCopy);
  assert(isSorted(localCopy));
  var fail:atomic int;
  forall i in 0..#n {
    if i > 0 {
      if A[i-1] > A[i] {
        writeln("Sorting error in randomtest seed=", seed, " n=", n,
                " element ", i, " in incorrect order A[i-1] ", A[i-1],
                " A[i] ", A[i]);
        fail.add(1);
      }
    }
    if localCopy[i] != A[i] {
      writeln("Sorting error in randomtest seed=", seed, " n=", n,
              " element ", i, " in incorrect order A[i] ", A[i],
              " localCopy[i] ", localCopy[i]);
      fail.add(1);
    }

    if fail.read() > 100 {
      halt("failure");
    }
  }
  if fail.read() != 0 {
    halt("failure");
  }

  writeln("Done Verifying");
}
randomtest(10);
randomtest(100);
randomtest(1000);
randomtest(10000);
randomtest(100000);
randomtest(1000000);
randomtest(10000000);
randomtest(100000000);
randomtest(1024*1024*128);
