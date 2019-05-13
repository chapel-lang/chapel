use Random;
use BlockDist;
use Time;
use Sort only;

config param debug = false;
config const timing = true;

module SortModuleStuff {
  use Sort only DefaultComparator, defaultComparator, chpl_compare, chpl_check_comparator;

/* BEGIN STUFF THAT SHOULD BE IN SORT MODULE */

pragma "no doc"
proc shellSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator,
               start=Dom.low, end=Dom.high)
{
  chpl_check_comparator(comparator, eltType);

  if Dom.rank != 1 then
    compilerError("shellSort() requires 1-D array");
  if Dom.stridable then
    compilerError("shellSort() requires an array over a non-stridable domain");

  // Based on Sedgewick's Shell Sort -- see
  // Analysis of Shellsort and Related Algorithms 1996
  // and see Marcin Ciura - Best Increments for the Average Case of Shellsort
  // for the choice of these increments.
  var n = 1 + end - start;
  var js,hs:int;
  var v,tmp:Data.eltType;
  const incs = (701, 301, 132, 57, 23, 10, 4, 1);
  for h in incs {
    hs = h + start;
    for is in hs..end {
      v = Data[is];
      js = is;
      while js >= hs && chpl_compare(v,Data[js-h],comparator) < 0 {
        Data[js] = Data[js - h];
        js -= h;
      }
      Data[js] = v;
    }
  }
}

// This is the number of bits to sort at a time in the radix sorter.
// The code assumes that all integer types are a multiple of it.
// That would need to change if it were to increase.
//
// At the same time, using a value less than 8 will probably perform poorly.
param RADIX_BITS = 8;

// This structure tracks configuration for the radix sorter.
pragma "no doc"
record MSBRadixSortSettings {
  param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step
  const sortSwitch = 256; // when sorting <= this many elements, use shell sort
  const minForTask = 256; // when sorting >= this many elements, go parallel
  param CHECK_SORTS = false; // do costly extra checks that data is sorted
  param progress = false; // print progress
  const alwaysSerial = false; // never create tasks
  const maxTasks = here.numPUs(logical=true); // maximum number of tasks to make
  param returnCounts = false;
}

// Get the bin for a record by calling criterion.keyPart
//
// startbit is starting from 0
// bin 0 is for the end was reached (sort before)
// bins 1..256 are for data with next part 0..255.
// bin 256 is for the end was reached (sort after)
//
// ubits are the result of keyPart normalized to a uint.
//
// returns (bin, ubits)
inline
proc binForRecordKeyPart(a, criterion, startbit:int)
{
  // We have keyPart(element, start):(section:int(8), part:int/uint)
  const testRet: criterion.keyPart(a, 1).type;
  const testPart = testRet(2);
  param bitsPerPart = numBits(testPart.type);
  param bitsPerPartModRadixBits = bitsPerPart % RADIX_BITS;
  if bitsPerPartModRadixBits != 0 then
    compilerError("part size must be a multiple of radix bits");
    // or else the implementation below would have to handle crossing parts

  // startbit must be a multiple of RADIX_BITS because the radix
  // sort operates RADIX_BITS at a time.

  // startbit might be partway through a part (e.g. 16 bits into a uint(64))
  const whichpart = startbit / bitsPerPart;
  const bitsinpart = startbit % bitsPerPart;

  const (section, part) = criterion.keyPart(a, 1+whichpart);
  var ubits = part:uint(bitsPerPart);
  // If the number is signed, invert the top bit, so that
  // the negative numbers sort below the positive numbers
  if isInt(part) {
    const one:ubits.type = 1;
    ubits = ubits ^ (one << (bitsPerPart - 1));
  }
  param mask:uint = (1 << RADIX_BITS) - 1;
  const ubin = (ubits >> (bitsPerPart - bitsinpart - RADIX_BITS)) & mask;

  if section == 0 then
    return (ubin:int + 1, ubits);
  else if section < 0 then
    return (0, ubits);
  else
    return ((1 << RADIX_BITS) + 1, ubits);
}

// Get the bin for a record with criterion.key or criterion.keyPart
//
// See binForRecordKeyPart for what the arguments / returns mean.
pragma "no doc"
inline
proc binForRecord(a, criterion, startbit:int)
{
  use Reflection;

  if canResolveMethod(criterion, "keyPart", a, 1) {
    return binForRecordKeyPart(a, criterion, startbit);
  } else if canResolveMethod(criterion, "key", a) {
    // Try to use the default comparator to get a keyPart.
    return binForRecordKeyPart(criterion.key(a),
                               defaultComparator,
                               startbit);
  } else {
    compilerError("Bad comparator for radix sort ", criterion.type:string,
                  " with eltType ", a.type:string);
  }
}

// Returns the fixed number of bits in a value, if known.
// Returns -1 otherwise.
proc fixedWidth(type eltTy) param {
  if (isUintType(eltTy) || isIntType(eltTy) ||
      isRealType(eltTy) || isImagType(eltTy)) then
    return numBits(eltTy);

  if (isHomogeneousTuple(eltTy)) {
    var tmp:eltTy;
    return tmp.size * numBits(tmp(1).type);
  }

  return -1;
}

// Returns a compile-time known final startbit
// e.g. for uint(64), returns 56 (since that's 64-8 and the
// last sort pass will sort on the last 8 bits).
//
// Returns -1 if no such ending is known at compile-time.
proc msbRadixSortParamLastStartBit(Data:[], comparator) param {
  use Reflection;

  // Compute end_bit if it's known
  // Default comparator on integers has fixed width
  const ref element = Data[Data.domain.low];
  if comparator.type == DefaultComparator && fixedWidth(element.type) > 0 {
    return fixedWidth(element.type) - RADIX_BITS;
  } else if canResolveMethod(comparator, "key", element) {
    type keyType = comparator.key(element).type;
    if fixedWidth(keyType) > 0 then
      return fixedWidth(keyType) - RADIX_BITS;
  }

  return -1;
}

pragma "no doc"
proc msbRadixSortClz(val) {
  // This could use BitOps.clz but that adds new
  // module dependencies that confuse testing.
  // Since it's not performance critical, here we
  // have a version using a while loop.
  param nBits = numBits(val.type);
  if val == 0 {
    return nBits;
  }

  var cur = val;
  var one = 1:val.type;
  var hi = one << (nBits - 1);
  var n = 0;
  while (cur & hi) == 0 {
    n += 1;
    cur <<= 1;
  }
  return n;
}

// Compute the startbit location that could be used based on the
// min/max of values returned by keyPart.
proc findDataStartBit(startbit:int, min_ubits, max_ubits):int {
  var xor = min_ubits ^ max_ubits;

  // Clear the top bits in xor if they are after bitsinpart
  param bitsPerPart = numBits(min_ubits.type);
  const bitsinpart = startbit % bitsPerPart;
  xor <<= bitsinpart;
  xor >>= bitsinpart;

  var new_start = msbRadixSortClz(xor);
  var new_digit = new_start / RADIX_BITS;
  var new_start_bit_rounded = new_digit * RADIX_BITS;

  return new_start_bit_rounded:int;
}

pragma "no doc"
proc msbRadixSort(Data:[], comparator:?rec=defaultComparator) {

  var endbit:int;
  endbit = msbRadixSortParamLastStartBit(Data, comparator);
  if endbit < 0 then
    endbit = max(int);

  msbRadixSort(start_n=Data.domain.low, end_n=Data.domain.high,
               Data, comparator,
               startbit=0, endbit=endbit,
               settings=new MSBRadixSortSettings());
}

// startbit counts from 0 and is a multiple of RADIX_BITS
pragma "no doc"
proc msbRadixSort(start_n:int, end_n:int, A:[], criterion,
                  startbit:int, endbit:int,
                  settings /* MSBRadixSortSettings */)
{
  const radixbits = RADIX_BITS;
  const radix = (1 << radixbits) + 1;

  if startbit > endbit {
    var counts:[0..radix] int;
    return counts;
  }

  if( end_n - start_n < settings.sortSwitch && !settings.returnCounts) {
    shellSort(A, criterion, start=start_n, end=end_n);
    if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
    var counts:[0..radix] int;
    return counts;
  }

  if settings.progress then writeln("radix sort start=", start_n, " end=", end_n, " startbit=", startbit, " endbit=", endbit);


  // 0th bin is for records where we've consumed all the key.
  var offsets:[0..radix] int;
  var end_offsets:[0..radix] int;
  type ubitsType = binForRecord(A[start_n], criterion, startbit)(2).type;
  var min_ubits: ubitsType = max(ubitsType);
  var max_ubits: ubitsType = 0;
  var min_bin = radix+1;
  var max_bin = 0;
  var any_ending = false;

  // Step 1: count.
  if settings.alwaysSerial == false {
    forall i in start_n..end_n
      with (+ reduce offsets,
            min reduce min_ubits,
            max reduce max_ubits,
            || reduce any_ending) {
      const (bin, ubits) = binForRecord(A[i], criterion, startbit);
      if ubits < min_ubits then
        min_ubits = ubits;
      if ubits > max_ubits then
        max_ubits = ubits;
      if bin == 0 || bin == radix then
        any_ending = true;
      offsets[bin] += 1;
    }
  } else {
    // The serial version
    for i in start_n..end_n {
      const (bin, ubits) = binForRecord(A[i], criterion, startbit);
      if ubits < min_ubits then
        min_ubits = ubits;
      if ubits > max_ubits then
        max_ubits = ubits;
      if bin == 0 || bin == radix then
        any_ending = true;
      offsets[bin] += 1;
    }
  }

  var counts = offsets;

  // If the data parts we gathered all have the same leading bits,
  // we might be able to skip ahead immediately to the next count step.
  if any_ending == false && !settings.returnCounts {
    var dataStartBit = findDataStartBit(startbit, min_ubits, max_ubits);
    if dataStartBit > startbit {
      // Re-start count again immediately at the new start position.
      msbRadixSort(start_n, end_n, A, criterion,
                   dataStartBit, endbit, settings);
      return counts;
    }
  }

  if settings.progress then writeln("accumulate");

  // Step 2: accumulate
  var sum = 0;
  for (off,end) in zip(offsets,end_offsets) {
    var binstart = sum;
    sum += off;
    var binend = sum;
    off = start_n + binstart;
    end = start_n + binend;
  }

  var curbin = 0;

  if settings.progress then writeln("shuffle");

  // Step 3: shuffle
  while true {
    // Find the next bin that isn't totally in place.
    while curbin <= radix && offsets[curbin] == end_offsets[curbin] {
      curbin += 1;
    }
    if curbin > radix {
      break;
    }

    // TODO: I think it might be possible to make this sort stable
    // by populating buf from the start of the data instead of from the end.
    // buf would need to be populated with the first M elements that aren't
    // already in the correct bin.

    // TODO: I think it's possible to make this shuffle parallel
    // by imagining each task has a max_buf and have them update
    // atomic offsets.
    param max_buf = settings.DISTRIBUTE_BUFFER;
    var buf: max_buf*A.eltType;
    var used_buf = 0;
    var end = end_offsets[curbin];
    var endfast = max(offsets[curbin], end_offsets[curbin]-2*max_buf);
    var bufstart = max(offsets[curbin], end_offsets[curbin]-max_buf);
    var i = bufstart;

    // Fill buf with up to max_buf records from the end of this bin.
    while i < end {
      buf[used_buf+1] <=> A[i];
      used_buf += 1;
      i += 1;
    }

    while offsets[curbin] < endfast {
      // Now go through the records in buf
      // putting them in their right home.
      for param j in 1..max_buf {
        const (bin, _) = binForRecord(buf[j], criterion, startbit);
        // prefetch(A[offsets[bin]]) could be here but doesn't help

        // Swap buf[j] into its appropriate bin.
        // Leave buf[j] with the next unsorted item.
        A[offsets[bin]] <=> buf[j];
        offsets[bin] += 1;
      }
    }
    // Now, handle elements in bufstart...end_offsets[cur_bin]
    while offsets[curbin] < end {
      // Put buf[j] into its right home
      var j = 1;
      while used_buf > 0 && j <= used_buf {
        const (bin, _) = binForRecord(buf[j], criterion, startbit);
        // Swap buf[j] into its appropriate bin.
        var offset = offsets[bin];
        A[offset] <=> buf[j];
        offsets[bin] += 1;
        // Leave buf[j] with the next unsorted item.
        // But offsets[bin] might be in the region we already read.
        if bin == curbin && offset >= bufstart {
          buf[j] <=> buf[used_buf];
          used_buf -= 1;
        }
        j += 1;
      }
    }
  }

  if settings.progress then writeln("sort sub-problems");

  // Step 4: sort sub-problems.
  // Note that shuffle changed the offsets to be == end_offset..
  // put offsets back.
  offsets[0] = start_n;
  for i in 1..radix {
    offsets[i] = end_offsets[i-1];
  }

  // This is a parallel version
  if settings.alwaysSerial == false {
    const subbits = startbit + radixbits;
    var nbigsubs = 0;
    var bigsubs:[0..radix] (int,int);
    const runningNow = here.runningTasks();

    // Never recursively sort the first or last bins
    // (these store the end)

    for bin in 1..radix-1 {
      // Does the bin contain more than one record?
      const bin_start = offsets[bin];
      const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
      const num = 1 + bin_end - bin_start;
      if num <= 1 || startbit >= endbit {
        // do nothing
      } else if num < settings.minForTask || runningNow >= settings.maxTasks {
        // sort it in this thread
        msbRadixSort(bin_start, bin_end, A, criterion,
                     subbits, endbit, settings);
      } else {
        // Add it to the list of things to do in parallel
        bigsubs[nbigsubs] = (bin_start, bin_end);
        nbigsubs += 1;
      }
    }

    forall (bin,(bin_start,bin_end)) in zip(0..#nbigsubs,bigsubs) {
      msbRadixSort(bin_start, bin_end, A, criterion, subbits, endbit, settings);
    }
  } else {
    // The serial version
    for bin in 1..radix-1 {
      // Does the bin contain more than one record?
      const bin_start = offsets[bin];
      const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
      const num = 1 + bin_end - bin_start;
      if num <= 1 || startbit >= endbit {
        // do nothing
      } else {
        // sort it in this thread
        msbRadixSort(bin_start, bin_end, A, criterion,
                     startbit + radixbits, endbit, settings);
      }
    }
  }

  if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);

  return counts;
}

// Check that the elements from start_n..end_n in A are sorted by criterion
proc checkSorted(start_n:int, end_n:int, A:[], criterion, startbit = 0)
{
  for i in start_n+1..end_n {
    var cmp = chpl_compare(A[i-1], A[i], criterion);
    if cmp > 0 {
      writeln("Error: not sorted properly at i=", i, " A[i-1]=", A[i-1], " A[i]=", A[i], " in start=", start_n, " end=", end_n);
      writeln(A);

      // Halt. Note, this is only intended to be called by unit testing.
      halt("failed checkSorted");
    }
  }
}

/** END STUFF THAT SHOULD BE IN SORT MODULE */
}

use SortModuleStuff;


/* BEGIN DISTRIBUTED RADIX SORT IMPLEMENTATION */

proc localBucketize(arr:[], criterion, startbit:int) {
  var endbit = startbit + 7;
  var settings = new MSBRadixSortSettings(returnCounts=true);
  if arr.domain.stride == 1 {
    return msbRadixSort(arr.domain.low, arr.domain.high, arr,
                        criterion, startbit, endbit, settings);
  } else {
    // TODO: update msbRadixSort to handle strided arrays
    var size = arr.domain.numIndices;
    ref reindex = arr.reindex(0..#size);
    return msbRadixSort(0, size-1, reindex,
                        criterion, startbit, endbit, settings);
  }
}


// the src array has the input and will be used for scratch space
// the dst array has the output
proc msbRadixSortWithScratchSpace(start_n:int, end_n:int,
                                  dst:[], src:[],
                                  criterion,
                                  startbit:int, endbit:int) {

  if !src.hasSingleLocalSubdomain() {
    compilerError("Distributed sort needs single local subdomain");
  }

  if debug {
    writeln("in distributed radix sort ", start_n, "..", end_n,
            " startbit ", startbit, " endbit ", endbit);
  }

  var radix = (1 << RADIX_BITS) + 1;
  const countsSize = numLocales * (radix+1);  // TODO: multiple tasks/locale
  //const countsSizeBox = {0..#countsSize};
  //const countsBlockDom = countsSizeBox dmapped
  //                       Block(countsSizeBox,
  //                             targetLocales=src.targetLocales());

  // globalCounts stores counts like this:
  //   count for bin 0, task 0
  //   count for bin 0, task 1
  //   ...
  //   count for bin 1, task 0
  //   count for bin 1, task 1
  // i.e. bin*ntasks + taskId

  // Should this be distributed?
  var globalCounts:[0..#countsSize] int;

  const ntasks = src.targetLocales().size;

  // Step 1: Each locale sorts local portions into buckets
  // and saves counts in globalCounts
  coforall (loc,tid) in zip(src.targetLocales(),0..) {
    on loc do {
      // TODO: instead factor msbRadixSort into count and distribute
      // helper functions
      ref localSlice = src.localSlice(src.localSubdomain()[start_n..end_n]);
      var localCounts = localBucketize(localSlice, criterion, startbit);
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

proc simpletestcore(input:[]) {
  var seed = SeedGenerator.oddCurrentTime;

  var localCopy = input;
  shuffle(localCopy, seed);

  const blockDom = newBlockDom(input.domain);
  var src: [blockDom] uint = localCopy;
  var dst: [blockDom] uint;

  if debug {
    writef("input %xt\n", input);
  }

  msbRadixSortWithScratchSpace(blockDom.low, blockDom.high,
                               dst, src,
                               defaultComparator,
                               0, max(int));

  if debug {
    writef("output %xt\n", dst);
  }

  forall i in input.domain {
    if input[i] != dst[i] {
      writeln("Sorting error in simpletest seed=", seed,
              " element ", i, " in incorrect order");
      writef("input was %xt\n", input);
      halt("failure");
    }
  }
}
proc simpletest(input:[]) {
  var size = input.domain.numIndices;
  simpletestcore(input.reindex(1..#size));
  simpletestcore(input.reindex(0..#size));
  simpletestcore(input.reindex(2..#size));
  simpletestcore(input.reindex(-1..#size));
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


proc randomtest(n:int) {
  const blockDom = newBlockDom({0..#n});
  var src: [blockDom] uint;
  var seed = SeedGenerator.oddCurrentTime;
  fillRandom(src, seed);
  var localCopy:[0..#n] uint = src; 
  var dst: [blockDom] uint;

  writeln("Sorting ", n, " elements");

  var timer:Timer;
  timer.start();
  msbRadixSortWithScratchSpace(0, n-1,
                               dst, src,
                               defaultComparator,
                               0, max(int));
  timer.stop();

  if timing {
    writeln("Sorted ", n, " elements in ", timer.elapsed(), " seconds",
            " (", 8.0*n/timer.elapsed()/1024.0/1024.0, " MiB/s");
  }

  writeln("Verifying");
  Sort.sort(localCopy);
  assert(Sort.isSorted(localCopy));
  forall i in 0..#n {
    if localCopy[i] != dst[i] {
      writeln("Sorting error in randomtest seed=", seed, " n=", n,
              " element ", i, " in incorrect order");
      halt("failure");
    }
  }
  writeln("Done Verifying");
}

proc main() {
  randomtest(10);
  randomtest(100);
  randomtest(1000);
  randomtest(10000);
  randomtest(100000);
  //randomtest(1000000);
  //randomtest(10000000);
  //randomtest(100000000);
}
