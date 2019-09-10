/*
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


module SortExtras {

  private use Sort;
  private use List;
  private use Reflection;

/*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   .. note:: This function is deprecated - please use :proc:`sort`.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc bubbleSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  BubbleSort.bubbleSort(Data, comparator);
}

/*
   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   .. note:: This function is deprecated - please use :proc:`sort`.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc heapSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  HeapSort.heapSort(Data, comparator);
}

// insertionSort is in Sort.chpl

/*
  Sort the 1D array `Data` in-place using a sequential, stable binary
  insertion sort algorithm.
  Should be used when there is a high cost of comparison.

  .. note:: This function is deprecated - please use :proc:`sort`.

  :arg Data: The array to be sorted
  :type Data: [] `eltType`
  :arg comparator: :ref:`Comparator <comparators>` record that defines how the
     data is sorted.

 */
proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  BinaryInsertionSort.binaryInsertionSort(Data, comparator);
}

/*
  Sort the 1D array `Data` using a parallel merge sort algorithm.

  .. note:: This function is deprecated - please use :proc:`sort`.

  :arg Data: The array to be sorted
  :type Data: [] `eltType`
  :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
  :type minlen: `integral`
  :arg comparator: :ref:`Comparator <comparators>` record that defines how the
    data is sorted.
 */
proc mergeSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator) {
  MergeSort.mergeSort(Data, minlen, comparator);
}

/*
  Sort the 1D array `Data` in-place using a sequential quick sort algorithm.

  .. note:: This function is deprecated - please use :proc:`sort`.

  :arg Data: The array to be sorted
  :type Data: [] `eltType`
  :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
  :type minlen: `integral`
  :arg comparator: :ref:`Comparator <comparators>` record that defines how the
    data is sorted.

 */
proc quickSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator) {
  QuickSort.quickSort(Data, minlen, comparator);
}

/*
  Sort the 1D array `Data` in-place using a sequential selection sort
  algorithm.

  .. note:: This function is deprecated - please use :proc:`sort`.

  :arg Data: The array to be sorted
  :type Data: [] `eltType`
  :arg comparator: :ref:`Comparator <comparators>` record that defines how the
     data is sorted.

 */
proc selectionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  SelectionSort.selectionSort(Data, comparator);
}

pragma "no doc"
module BubbleSort {

  /*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc bubbleSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("bubbleSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
          stride = abs(Dom.stride);

    var swapped = true;

    while (swapped) {
      swapped = false;
      for i in low..high-stride by stride {
        if chpl_compare(Data(i), Data(i+stride), comparator) > 0 {
          Data(i) <=> Data(i+stride);
          swapped = true;
        }
      }
    }
  }
}

pragma "no doc"
module HeapSort {
  /*

   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc heapSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("heapSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
          size = Dom.size,
          stride = abs(Dom.stride);

    // Heapify
    var start = if high == low then high
                else if size % 2 then low + ((size - 1)/2) * stride
                else low + (size/2 - 1) * stride;

    while (start >= low) {
      SiftDown(start, high, comparator);
      start = start - stride;
    }

    // Sort, moving max element to end and re-heapifying the rest
    var end = high;
    while (end > low) {
      Data(end) <=> Data(low);
      end = end - stride;
      SiftDown(low, end, comparator);
    }

    proc SiftDown(start, end, comparator:?rec=defaultComparator) {
      var root = start;
      while ((2*root - low + stride) <= end) {
        const child = 2*root - low + stride;
        var swap = root;

        if chpl_compare(Data(swap), Data(child), comparator) < 0 then
          swap = child;

        if (child + stride <= end) && (chpl_compare(Data(swap), Data(child + stride), comparator) < 0) then
          swap = child + stride;

        if swap != root {
          Data(root) <=> Data(swap);
          root = swap;
        } else {
          return;
        }
      }
    }
  }
}

// InsertionSort is in Sort.chpl

pragma "no doc"
module BinaryInsertionSort {
  /*
    Sort the 1D array `Data` in-place using a sequential, stable binary
    insertion sort algorithm.
    Should be used when there is a high cost of comparison.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.
   */
  proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("binaryInsertionSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
          stride = abs(Dom.stride);

    for i in low..high by stride {
      var valToInsert = Data[i],
          lo = low,
          hi = i - stride;

      var (found, loc) = _binarySearchForLastOccurrence(Data, valToInsert, comparator, lo, hi);
      loc = if found then loc + stride else loc;              // insert after last occurrence if exists; else insert after expected location

      for j in loc..i-stride by -stride {
        // backward swap until loc
        Data[j + stride] = Data[j];
      }

      Data[loc] = valToInsert;
    }
  }

  /*
    Binary searches for the index of the last occurrence of `val` in the 1D array `Data` based on a comparator.
    If `val` is not in `Data`, the index that it should be inserted at is returned.
    Does not check for a valid comparator.
  */
  private proc _binarySearchForLastOccurrence(Data: [?Dom], val, comparator:?rec=defaultComparator, in lo=Dom.alignedLow, in hi=Dom.alignedHigh) {
    const stride = if Dom.stridable then abs(Dom.stride) else 1;

    var loc = -1;                                        // index of the last occurrence of val in Data

    while (lo <= hi) {
      const size = (hi - lo) / stride,
            mid = lo + (size/2) * stride;

      if chpl_compare(val, Data[mid], comparator) == 0 {
          loc = mid;                                    // index of last occurrence of val in 1..mid
          lo = loc + stride;
      }
      else if chpl_compare(val, Data[mid], comparator) > 0 then
        lo = mid + stride;
      else
        hi = mid - stride;
    }

    if loc == -1 then return (false, lo);              // returns index where val should be
    return (true, loc);                                // returns index of the last occurrence of val
  }
}

pragma "no doc"
module MergeSort {
  /*
    Sort the 1D array `Data` using a parallel merge sort algorithm.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
    :type minlen: `integral`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.

   */
  proc mergeSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("mergeSort() requires 1-D array");
    }

    _MergeSort(Data, Dom.alignedLow, Dom.alignedHigh, minlen, comparator);
  }

  private proc _MergeSort(Data: [?Dom], lo:int, hi:int, minlen=16, comparator:?rec=defaultComparator)
    where Dom.rank == 1 {
    if (hi-lo < minlen) {
      InsertionSort.insertionSort(Data, comparator=comparator, lo, hi);
      return;
    }

    const stride = if Dom.stridable then abs(Dom.stride) else 1,
          size = (hi - lo) / stride,
          mid = lo + (size/2) * stride;

    if(here.runningTasks() < here.numPUs(logical=true)) {
      cobegin {
        { _MergeSort(Data, lo, mid, minlen, comparator); }
        { _MergeSort(Data, mid+stride, hi, minlen, comparator); }
      }
    } else {
      _MergeSort(Data, lo, mid, minlen, comparator);
      _MergeSort(Data, mid+stride, hi, minlen, comparator);
    }
    _Merge(Data, lo, mid, hi, comparator);
  }

  private proc _Merge(Data: [?Dom] ?eltType, lo:int, mid:int, hi:int, comparator:?rec=defaultComparator) {
    /* Data[lo..mid by stride] is much slower than Data[lo..mid] when
     * Dom is unstrided.  So specify the latter explicitly when possible. */
    const stride = if Dom.stridable then abs(Dom.stride) else 1;
    const a1size = (lo..mid by stride).size;
    var A1: [1..a1size] Data.eltType =
      if Dom.stridable
      then Data[lo..mid by stride]
      else Data[lo..mid];
    const a2size = ((mid+stride)..hi by stride).size;
    var A2: [1..a2size] Data.eltType =
      if Dom.stridable
      then Data[(mid+stride)..hi by stride]
      else Data[(mid+1)..hi];
    var a1 = 1;
    var a2 = 1;
    var i = lo;
    while ((a1 <= a1size) && (a2 <= a2size)) {
      if (chpl_compare(A1(a1), A2(a2), comparator) <= 0) {
        Data[i] = A1[a1];
        a1 += 1;
        i += stride;
      } else {
        Data[i] = A2[a2];
        a2 += 1;
        i += stride;
      }
    }
    while (a1 <= a1size) {
      Data[i] = A1[a1];
      a1 += 1;
      i += stride;
    }
    while (a2 <= a2size) {
      Data[i] = A2[a2];
      a2 += 1;
      i += stride;
    }
  }
}

// QuickSort is in Sort.chpl

pragma "no doc"
module SelectionSort {
  /*
    Sort the 1D array `Data` in-place using a sequential selection sort
    algorithm.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.

   */
  proc selectionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {

    if Dom.rank != 1 {
      compilerError("selectionSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
          stride = abs(Dom.stride);

    for i in low..high-stride by stride {
      var jMin = i;
      // TODO -- should be a minloc reduction, when they can support comparators
      for j in i..high by stride {
        if chpl_compare(Data[j], Data[jMin], comparator) < 0 then
          jMin = j;
      }
      Data(i) <=> Data(jMin);
    }
  }
}

// ShellSort is in Sort.chpl

pragma "no doc"
module SampleSortHelp {
  param maxLogBuckets = 8; // not counting equality buckets.
  param classifyUnrollFactor = 7;
  const equalBucketThreshold = 5;
  const maxInline = 16;
  const oversampleFactor = 0.2;

  // compute logarithm base 2 rounded down
  proc log2int(n: int) {
    if n <= 0 then
      return 0;
    return log2(n);
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



  record SampleBucketizer {
    type eltType;

    // filled from 1 to num_buckets_
    var storage: c_array(eltType, 1<<maxLogBuckets);
    // filled from 0 to num_buckets, last one is duplicated
    var sortedStorage: c_array(eltType, 1<<maxLogBuckets);

    var logBuckets: int;
    var numBuckets: int;
    var equalBuckets: bool;

    proc writeThis(ch) {
      ch <~> "SampleBucketizer(";
      ch <~> "\n logBuckets=" <~> logBuckets;
      ch <~> "\n numBuckets=" <~> numBuckets;
      ch <~> "\n equalBuckets=" <~> equalBuckets;
      ch <~> "\n storage=";
      for i in 0..numBuckets {
        ch <~> (try! " %xt".format(storage[i]));
      }
      ch <~> "\n sortedStorage=";
      for i in 0..numBuckets {
        ch <~> (try! " %xt".format(sortedStorage[i]));
      }
      ch <~> ")\n";
    }

    proc getNumBuckets() {
      return numBuckets * (1 + equalBuckets:int);
    }
    proc getBinsToRecursivelySort() {
      var r:range(stridable=true);
      if equalBuckets {
        // odd bins will be equality buckets
        r = (0..(getNumBuckets()-1)) by 2;
      } else {
        r = 0..(getNumBuckets()-1);
      }
      return r;
    }
    proc getNextStartBit(start_bit:int) {
      return start_bit;
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
    inline proc splitter(i:int) const ref : eltType {
      return storage[i];
    }
    inline proc sortedSplitter(i:int) const ref : eltType {
      return sortedStorage[i];
    }

    proc bucketForRecord(a, criterion, startbit) {
      var bk = 1;
      for lg in 0..#logBuckets {
        bk = 2*bk + (chpl_compare(splitter(bk), a, criterion) < 0):int;
      }
      if equalBuckets {
        bk = 2*bk + (chpl_compare(a, sortedSplitter(bk-numBuckets), criterion) == 0):int;
      }
      return bk - (if equalBuckets then 2*numBuckets else numBuckets);
    }
    // yields (index, bucket index) for A[start_n..end_n]
    iter classify(A, start_n, end_n, criterion, startbit) {
      const paramEqualBuckets = equalBuckets;
      const paramLogBuckets = logBuckets;
      const paramNumBuckets = 1 << (paramLogBuckets + paramEqualBuckets:int);
      var b:c_array(int, classifyUnrollFactor);

      var cur = start_n;
      // Run the main (unrolled) loop
      while cur <= end_n-(classifyUnrollFactor-1) {
        for /*param*/ i in 0..classifyUnrollFactor-1 {
          b[i] = 1;
        }
        for /*param*/ lg in 0..paramLogBuckets-1 {
          for /*param*/ i in 0..classifyUnrollFactor-1 {
            b[i] = 2*b[i] + (chpl_compare(splitter(b[i]), A[cur+i],criterion)<0):int;
          }
        }
        if paramEqualBuckets {
          for /*param*/ i in 0..classifyUnrollFactor-1 {
            b[i] = 2*b[i] +
                   (chpl_compare(A[cur+i],
                           sortedSplitter(b[i] - paramNumBuckets/2),criterion)==0):int;
          }
        }
        for /*param*/ i in 0..classifyUnrollFactor-1 {
          yield (cur+i, b[i]-paramNumBuckets);
        }
        cur += classifyUnrollFactor;
      }
      // Handle leftover
      while cur <= end_n {
        var bk = 1;
        for lg in 0..#paramLogBuckets {
          bk = 2*bk + (chpl_compare(splitter(bk), A[cur], criterion)<0):int;
        }
        if paramEqualBuckets {
          bk = 2*bk + (chpl_compare(A[cur],
                               sortedSplitter(bk - paramNumBuckets/2),criterion)==0):int;
        }
        yield (cur, bk - paramNumBuckets);
        cur += 1;
      }
    }
  }

  proc createSplittersFromSample(A,
                                 ref splitterBucketizer:SampleBucketizer,
                                 criterion,
                                 start_n: int,
                                 sampleSize: int,
                                 sampleStep: int,
                                 in numBuckets: int,
                                 forceEqualBuckets:? = none) {

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
    if 1+2*uniqueSplitters < 1<<maxLogBuckets {
      myUseEqualBuckets = true;
    }
    if forceEqualBuckets.type != nothing then
      myUseEqualBuckets = forceEqualBuckets;

    // Fill the array to the next power of two
    var logBuckets = log2(uniqueSplitters) + 1;
    numBuckets = 1 << logBuckets;
    for i in uniqueSplitters+1 .. numBuckets-1 {
      splitterIndex += 1;
      splitters[splitterIndex] = A[arrayIndex];
    }

    // Build the tree
    splitterBucketizer.build(logBuckets, myUseEqualBuckets);
  }

  // Using this to build the sample is inherently un-stable.
  // A stable sort would probably sort several subsections of the array,
  // and then use a median approach to create the samples. Alternatively,
  // it could copy the sample somewhere else for sorting.
  proc putRandomSampleAtArrayStart(in start_n:int,
                                   end_n:int,
                                   A:[],
                                   in numSamples:int,
                                   seed=1) {
    private use Random;
    var Tmp:[1..1] A.eltType;
    var randNums = makeRandomStream(seed=seed, eltType=int, parSafe=false);
    while numSamples > 0 {
      numSamples -= 1;

      var offset = randNums.getNext(start_n, end_n);
      if offset != start_n {
        // A[start_n] <=> A[offset] but with shallow copy.
        var tmp: A.eltType;
        ShallowCopy.shallowCopy(Tmp, 1, A, start_n, 1);
        ShallowCopy.shallowCopy(A, start_n, offset, 1);
        ShallowCopy.shallowCopy(A, offset, Tmp, 1, 1);
      }

      start_n += 1;
    }
  }
}

// RadixSortHelp is in Sort.chpl

pragma "no doc"
module ShallowCopy {

  // These shallow copy functions "move" a record around
  // (i.e. they neither swap nor call a copy initializer).

  // TODO: move these out of the Sort module and/or consider
  // language support for it.

  // TODO: These shallowCopy functions should handle Block,Cyclic arrays
  inline proc shallowCopy(ref A, dst, src, nElts) {
    // Ideally this would just be
    //A[dst..#nElts] = A[src..#nElts];

    if boundsChecking {
      assert(nElts > 0);
      assert(A.domain.contains(dst..#nElts));
      assert(A.domain.contains(src..#nElts));
    }

    if A._instance.isDefaultRectangular() {
      var size = (nElts:size_t)*c_sizeof(A.eltType);
      c_memcpy(c_ptrTo(A[dst]), c_ptrTo(A[src]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ A, {dst..#nElts},
                                       /*src*/ A, {src..#nElts});
      if !ok {
        halt("bulk transfer failed in sorting");
        for i in vectorizeOnly(0..#nElts) {
          __primitive("=", A[dst+i], A[src+i]);
        }
      }
    }
  }
  inline proc shallowCopy(ref DstA, dst, ref SrcA, src, nElts) {
    // Ideally this would just be
    //DstA[dst..#nElts] = SrcA[src..#nElts];

    if boundsChecking {
      assert(nElts > 0);
      assert(DstA.domain.contains(dst..#nElts));
      assert(SrcA.domain.contains(src..#nElts));
    }

    if DstA._instance.isDefaultRectangular() &&
       SrcA._instance.isDefaultRectangular() {
      var size = (nElts:size_t)*c_sizeof(DstA.eltType);
      c_memcpy(c_ptrTo(DstA[dst]), c_ptrTo(SrcA[src]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ DstA, {dst..#nElts},
                                       /*src*/ SrcA, {src..#nElts});
      if !ok {
        halt("bulk transfer failed in sorting");
        for i in vectorizeOnly(0..#nElts) {
          __primitive("=", DstA[dst+i], SrcA[src+i]);
        }
      }
    }
  }
  inline proc parallelShallowCopy(ref DstA, dst, ref SrcA, src, nElts) {
    const nTasks = if dataParTasksPerLocale > 0
                   then dataParTasksPerLocale
                   else here.maxTaskPar;

    halt("buggy");
    // TODO: something about this code is wrong right now.
    const blockSize = divceil(nElts, nTasks);
    const nBlocks = divceil(nElts, blockSize);
    coforall tid in 0..#nTasks {
      var start = tid * blockSize;
      var n = blockSize;
      if start + n > nElts then
        n = nElts - start;
      shallowCopy(DstA, dst+start, SrcA, src+start, n);
    }
  }
}

pragma "no doc"
module SequentialInPlacePartitioning {
  private param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step

  // Returns the count for each bucket
  // Stores the data in dst in buckets according to the bucketizer.
  // (e.g. sorted by the next digit in radix sort)
  proc bucketize(start_n: int, end_n: int, A:[],
                 bucketizer,
                 criterion, startbit:int) {

    var nBuckets = bucketizer.getNumBuckets();
    const nTasks = if dataParTasksPerLocale > 0
               then dataParTasksPerLocale
               else here.maxTaskPar;

    // Divide the input into nTasks chunks.
    const countsSize = nTasks * nBuckets;
    const n = end_n - start_n + 1;
    const blockSize = divceil(n, nTasks);
    const nBlocks = divceil(n, blockSize);

    var counts: [0..#nBuckets] int;

    // Step 1: count
    coforall tid in 0..#nTasks with (+ reduce counts) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      for (i,bin) in bucketizer.classify(A, start, end, criterion, startbit) {
        counts[bin] += 1;
      }
    }

    // Step 2: scan
    var end_offsets = (+ scan counts) + start_n;
    var offsets = end_offsets - counts;

    // Step 3: distribute/shuffle
    var curbin = 0;
    while true {
      // Find the next bin that isn't totally in place.
      while curbin < nBuckets && offsets[curbin] == end_offsets[curbin] {
        curbin += 1;
      }
      if curbin >= nBuckets {
        break;
      }

      // TODO: I think it might be possible to make this sort stable
      // by populating buf from the start of the data instead of from the end.
      // buf would need to be populated with the first M elements that aren't
      // already in the correct bin.

      // TODO: I think it's possible to make this shuffle parallel
      // by imagining each task has a max_buf and have them update
      // atomic offsets.
      param max_buf = DISTRIBUTE_BUFFER;
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
        for (idx, bin) in bucketizer.classify(buf, 1, max_buf,
                                              criterion, startbit) {
          // Swap buf[j] into its appropriate bin.
          // Leave buf[j] with the next unsorted item.
          A[offsets[bin]] <=> buf[idx];
          offsets[bin] += 1;
        }
      }
      // Now, handle elements in bufstart...end_offsets[cur_bin]
      while offsets[curbin] < end {
        // Put buf[j] into its right home
        var j = 1;
        while used_buf > 0 && j <= used_buf {
          const bin = bucketizer.bucketForRecord(buf[j], criterion, startbit);
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

    return counts;
  }
}

pragma "no doc"
module TwoArrayPartitioning {
  private use BlockDist;
  private use MSBRadixSort;

  private param debug = false;
  param maxBuckets = 512;

  record TwoArraySortTask {
    var start: int;
    var size: int;
    var startbit: int;
    var inA: bool;
    var doSort: bool;

    proc init() { }
    // To make sure all fields are specified
    proc init(start:int, size:int, startbit:int, inA:bool, doSort:bool) {
      this.start = start;
      this.size = size;
      this.startbit = startbit;
      this.inA = inA;
      this.doSort = doSort;
    }
  }

  record TwoArrayBucketizerPerTaskState {
    var localCounts: [0..#maxBuckets] int;
  }

  record TwoArrayBucketizerSharedState {
    var nTasks:int = if dataParTasksPerLocale > 0
                      then dataParTasksPerLocale
                      else here.maxTaskPar;
    var countsSize:int = nTasks*maxBuckets;

    var bucketizer; // contains e.g. sample

    // globalCounts stores counts like this:
    //   count for bin 0, task 0
    //   count for bin 0, task 1
    //   ...
    //   count for bin 1, task 0
    //   count for bin 1, task 1
    // i.e. bin*nTasks + taskId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;

    var localState:[0..#nTasks] TwoArrayBucketizerPerTaskState;

    // Set by bucketizer, used by sorter
    var counts:[0..#maxBuckets] int;

    // Used by sorter
    var ends:[0..#maxBuckets] int;
    var outputInA:[0..#maxBuckets] bool;

    // start, end, isInA
    // were [0..-1]
    var bigTasks: list(TwoArraySortTask);
    var smallTasks: list(TwoArraySortTask);

    var baseCaseSize:int = 16;
    var sequentialSizePerTask:int = 4096;
    var endbit:int = max(int);
  }

  record TwoArrayDistributedBucketizerStatePerLocale {
    type bucketizerType;

    var compat:TwoArrayBucketizerSharedState(bucketizerType);

    var countsSize:int = numLocales*maxBuckets;
    // globalCounts stores counts like this:
    //   count for bin 0, locale 0
    //   count for bin 0, locale 1
    //   ...
    //   count for bin 1, locale 0
    //   count for bin 1, locale 1
    // i.e. bin*nTasks + localeId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;
  }

  record TasksForLocale {
    // was [0..-1]
    var localTasks: list(TwoArraySortTask);
  }

  record TwoArrayDistributedBucketizerSharedState {
    type bucketizerType;

    var numLocales:int;
    var perLocale = newBlockArr(0..#numLocales,
        TwoArrayDistributedBucketizerStatePerLocale(bucketizerType));

    const baseCaseSize:int;
    const endbit:int = max(int);

    const countsSize:int = numLocales*maxBuckets;

    // globalCounts stores counts like this:
    //   count for bin 0, locale 0
    //   count for bin 0, locale 1
    //   ...
    //   count for bin 1, locale 0
    //   count for bin 1, locale 1
    // i.e. bin*nTasks + localeId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;

    // was [0..-1]
    var distTasks: list(TwoArraySortTask);
    var localTasks: [0..#numLocales] TasksForLocale;

    proc postinit() {
      // Copy some vars to the compat
      for p in perLocale {
        p.compat.baseCaseSize = baseCaseSize;
        p.compat.endbit = endbit;
        assert(p.compat.nTasks > 0);
        assert(p.compat.countsSize > 0);
      }
    }
  }

  // Stores the data in dst in buckets according to the bucketizer.
  // (e.g. sorted by the next digit in radix sort)
  // Counts per bin are stored in state.counts. Other data in
  // state is used locally by this routine or used elsewhere
  proc bucketize(start_n: int, end_n: int, dst:[], src:[],
                 ref state: TwoArrayBucketizerSharedState,
                 criterion, startbit:int) {

    if debug then
      writeln("bucketize ", start_n..end_n, " startbit=", startbit);

    const nBuckets = state.bucketizer.getNumBuckets();
    const n = end_n - start_n + 1;
    const nTasks = if n >= state.nTasks then state.nTasks else 1;
    assert(nTasks > 0);

    if n == 0 {
      state.counts = 0;
      return;
    }

    // Divide the input into nTasks chunks.
    const countsSize = nTasks * nBuckets;
    const blockSize = divceil(n, nTasks);
    const nBlocks = divceil(n, blockSize);

    // Count
    coforall tid in 0..#nTasks with (ref state) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      if debug then
        writeln("tid ", tid, " considering ", start..end);

      ref counts = state.localState[tid].localCounts;
      for bin in 0..#nBuckets {
        counts[bin] = 0;
      }
      for (i,bin) in state.bucketizer.classify(src, start, end,
                                               criterion, startbit) {
        counts[bin] += 1;
      }
      // Now store the counts into the global counts array
      for bin in vectorizeOnly(0..#nBuckets) {
        state.globalCounts[bin*nTasks + tid] = counts[bin];
      }
    }

    // Step 2: scan
    state.globalEnds = (+ scan state.globalCounts) + start_n;

    if debug {
      for bin in 0..#nBuckets {
        for tid in 0..#nTasks {
          var gb = bin*nTasks+tid;
          if state.globalCounts[gb] != 0 {
            writeln("tid ", tid, " count[", bin, "] = ", state.globalCounts[gb],
                    " end = ", state.globalEnds[gb] - 1);
          }
        }
      }
    }

    // Step 3: distribute
    coforall tid in 0..#nTasks with (ref state) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      ref nextOffsets = state.localState[tid].localCounts;
      // initialize nextOffsets
      for bin in 0..#nBuckets {
        var globalBin = bin*nTasks+tid;
        nextOffsets[bin] = if globalBin > 0
                           then state.globalEnds[globalBin-1]
                           else start_n;
        if debug {
          if state.globalCounts[globalBin] != 0 {
            writeln("tid ", tid, " nextOffsets[", bin, "] = ", nextOffsets[bin]);
          }
        }
      }

      for (i,bin) in state.bucketizer.classify(src, start, end,
                                               criterion, startbit) {
        // Store it in the right bin
        ref next = nextOffsets[bin];
        if debug {
          writeln("tid ", tid, " dst[", next, "] = src[", i, "] bin ", bin);
        }
        ShallowCopy.shallowCopy(dst, next, src, i, 1);
        next += 1;
      }
    }

    // Compute the total counts
    ref counts = state.counts;
    forall bin in 0..#nBuckets {
      var total = 0;
      for tid in 0..#nTasks {
        total += state.globalCounts[bin*nTasks + tid];
      }
      counts[bin] = total;
    }
  }
  proc testBucketize(start_n: int, end_n: int, dst:[], src:[],
                     bucketizer, criterion, startbit:int) {

    var state = new TwoArrayBucketizerSharedState(bucketizer=bucketizer);

    bucketize(start_n, end_n, dst, src, state, criterion, startbit);

    return state.counts;
  }


  private proc partitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if isSubtype(state.bucketizer.type, SampleSortHelp.SampleBucketizer) {
      var n = 1 + end_n - start_n;
      var logNumBuckets = SampleSortHelp.computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = SampleSortHelp.chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debug then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      SampleSortHelp.putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debug then
        writeln("recursing to sort the sample");

      // sort the sample


      // TODO: make it adjustable from the settings
      if sampleSize <= 1024*1024 {
        // base case sort, parallel OK
        msbRadixSort(start_n, start_n + sampleSize - 1,
                     A, criterion,
                     startbit, state.endbit,
                     settings=new MSBRadixSortSettings());
      } else {
        partitioningSortWithScratchSpace(start_n, start_n + sampleSize - 1,
                                         A, Scratch,
                                         state, criterion, startbit);
      }
      if debug {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debug {
        writeln("sample bucketizer ", state.bucketizer);
        writef("A %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
      }
    }

  }

  // Sorts the data in A.
  proc partitioningSortWithScratchSpace(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {


    if startbit > state.endbit then
      return;

    if end_n - start_n < state.baseCaseSize {
      ShellSort.shellSort(A, criterion, start=start_n, end=end_n);
      return;
    }

    if debug {
      writeln("partitioningSortWithScratchSpace(", start_n, ",", end_n, ")");
      writef("A %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
    }


    const n = (end_n - start_n + 1);
    const maxSequentialSize = max(n / state.nTasks,
                                  state.nTasks*state.sequentialSizePerTask);

    state.bigTasks.append(new TwoArraySortTask(start_n, n, startbit, inA=true, doSort=true));
    assert(state.bigTasks.size == 1);
    assert(state.smallTasks.size == 0);

    while !state.bigTasks.isEmpty() {
      const task = state.bigTasks.pop();
      const taskEnd = task.start + task.size - 1;

      assert(task.doSort);

      if debug then {
        writeln("doing big task ", task.start..taskEnd);
      }

      if task.inA {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, A, Scratch,
              state, criterion, task.startbit);

        // Count and partition
        bucketize(task.start, taskEnd, Scratch, A, state,
                  criterion, task.startbit);
        // bucketized data now in Scratch
        if debug {
          writef("pb %i %i Scratch=%xt\n", task.start, taskEnd, Scratch[task.start..taskEnd]);
        }
      } else {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, Scratch, A,
              state, criterion, task.startbit);

        // Count and partition
        bucketize(task.start, taskEnd, A, Scratch, state,
                  criterion, task.startbit);
        // bucketized data now in A
        if debug {
          writef("pb %i %i A=%xt\n", task.start, taskEnd, A[task.start..taskEnd]);
        }
      }
      const nowInA = !task.inA;

      // Compute the bucket ends
      state.ends = (+ scan state.counts) + task.start;

      // enqueue any sorting tasks not yet completed
      const nBuckets = state.bucketizer.getNumBuckets();
      for bin in 0..#nBuckets {
        const binSize = state.counts[bin];
        const binStart = state.ends[bin] - binSize;
        const binEnd = binStart + binSize - 1;
        const binStartBit = state.bucketizer.getNextStartBit(task.startbit);

        const sortit = state.bucketizer.getBinsToRecursivelySort().contains(bin);

        if binSize == 0 {
          // Do nothing
        } else if !nowInA && !sortit {
          // Enqueue a small task to do the copy.
          // TODO: handle large copies in big tasks, or enqueue several tasks here
          state.smallTasks.append(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));

        } else if binStartBit > state.endbit ||
                  binStart >= binEnd ||
                  binSize <= maxSequentialSize {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as small");
          }

          // Enqueue a small task to sort and possibly copy.
          state.smallTasks.append(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));

        } else {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as big");
          }

          // Enqueue a big task
          state.bigTasks.append(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));
        }
      }
    }

    // Now handle any small tasks.

    // TODO: sort small tasks by size

    forall task in state.smallTasks {
      const size = task.size;
      const taskEnd = task.start + size - 1;
      if size > 0 {
        if !task.inA {
          ShallowCopy.shallowCopy(A, task.start, Scratch, task.start, size);
        }

        if debug {
          writef("doing small task %i %i A=%xt\n", task.start, taskEnd, A[task.start..taskEnd]);
        }

        if task.doSort {
          // Sort it serially.
          msbRadixSort(task.start, taskEnd,
                       A, criterion,
                       task.startbit, state.endbit,
                       settings=new MSBRadixSortSettings(alwaysSerial=true));
        }
      }
    }

    if debug {
      writef("ps %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
      writef("ps %i %i Scratch=%xt\n", start_n, end_n, Scratch[start_n..end_n]);
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion, startbit);
    }
  }

  private proc distributedPartitioningSortWithScratchSpaceBaseCase(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int):void {

    if startbit > state.endbit then
      return;

    const n = end_n - start_n + 1;

    const localSubdomain = A.localSubdomain();
    const curDomain = {start_n..end_n};
    const intersect = curDomain[localSubdomain];
    if curDomain == intersect {
      if n > state.baseCaseSize {
        msbRadixSort(start_n, end_n,
                     A.localSlice(curDomain), criterion,
                     startbit, state.endbit,
                     settings=new MSBRadixSortSettings());
      } else {
        ShellSort.shellSort(A.localSlice(curDomain), criterion, start=start_n, end=end_n);
      }
    } else {
      const size = end_n-start_n+1;
      // Copy it to one locale
      var LocalA:[start_n..end_n] A.eltType;
      ShallowCopy.shallowCopy(LocalA, start_n, A, start_n, size);
      // Sort it
      if n > state.baseCaseSize {
        msbRadixSort(start_n, end_n,
                     LocalA, criterion,
                     startbit, state.endbit,
                     settings=new MSBRadixSortSettings());
      } else {
        ShellSort.shellSort(LocalA, criterion, start=start_n, end=end_n);
      }
      // Copy it back
      ShallowCopy.shallowCopy(A, start_n, LocalA, start_n, size);
    }

    if debug {
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion);
    }
    return;
  }


  private proc distributedPartitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if isSubtype(state.perLocale[0].compat.bucketizer.type, SampleSortHelp.SampleBucketizer) {
      var n = 1 + end_n - start_n;
      var logNumBuckets = SampleSortHelp.computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = SampleSortHelp.chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debug then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      SampleSortHelp.putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debug then
        writeln("recursing to sort the sample");

      // sort the sample

      // TODO: make it adjustable in the settings
      if sampleSize <= 1024*1024 {
        // base case sort, parallel OK
        distributedPartitioningSortWithScratchSpaceBaseCase(start_n, start_n + sampleSize - 1,
                                                            A, Scratch,
                                                            state, criterion,
                                                            startbit);
      } else {
        distributedPartitioningSortWithScratchSpace(start_n, start_n + sampleSize - 1,
                                                    A, Scratch,
                                                    state, criterion, startbit);
      }
      if debug {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.perLocale[0].compat.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debug {
        writeln("sample bucketizer ", state.perLocale[0].compat.bucketizer);
        writef("A %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
      }

      // Now copy the bucketizer sample to all other locales
      for lid in 1..state.numLocales {
        state.perLocale[lid].bucketizer = state.perLocale[0].bucketizer;
      }
    }
  }

  proc distributedPartitioningSortWithScratchSpace(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int): void {

    //use BlockDist only;

    if !A.hasSingleLocalSubdomain() {
      compilerError("distributedPartitioningSortWithScratchSpace needs single local subdomain");
    }
    // TODO: assert that src and dst have the same distribution?

    if startbit > state.endbit then
      return;

    if end_n - start_n < state.baseCaseSize {
      distributedPartitioningSortWithScratchSpaceBaseCase(start_n, end_n,
                                                          A, Scratch,
                                                          state, criterion,
                                                          startbit);
      return;
    }


    if debug {
      writeln("in distributed radix sort ", start_n, "..", end_n,
              " startbit ", startbit, " endbit ", state.endbit);
    }

    const n = (end_n - start_n + 1);
    state.distTasks.append(new TwoArraySortTask(start_n, n, startbit, true, true));
    assert(state.distTasks.size == 1);

    while !state.distTasks.isEmpty() {
      const task = state.distTasks.pop();
      const taskStart = task.start;
      const taskEnd = task.start + task.size - 1;

      assert(task.doSort);
      assert(task.inA);

      distributedPartitioningSortWithScratchSpaceHandleSampling(
            task.start, taskEnd, A, Scratch,
            state, criterion, task.startbit);

      const nBuckets = state.perLocale[0].compat.bucketizer.getNumBuckets();
      const nTasks = state.numLocales;

      // Step 1: Each locale sorts local portions into buckets
      // and saves counts in globalCounts
      coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
        on loc do {
          const localDomain = A.localSubdomain()[task.start..taskEnd];
          ref localSrc = A.localSlice(localDomain);
          ref localDst = Scratch.localSlice(localDomain);

          bucketize(localDomain.alignedLow,
                    localDomain.alignedHigh,
                    localDst, localSrc,
                    state.perLocale[tid].compat, criterion, task.startbit);
          ref localCounts = state.perLocale[tid].compat.counts;
          if debug {
            var total = 0;
            for bin in 0..#nBuckets {
              if localCounts[bin] > 0 {
                writeln("localCounts[", bin, "]=", localCounts[bin]);
                total += localCounts[bin];
              }
            }
            assert(total == localDomain.numIndices);
          }
          // Now store the counts into the global counts array
          for bin in vectorizeOnly(0..#nBuckets) {
            state.perLocale[0].globalCounts[bin*nTasks + tid] = localCounts[bin];
          }
          //state.globalCounts[tid.. by nTasks] = localCounts;
        }
      }
      // Now the data is in Scratch

      if debug {
        writef("after bucketize local portions, Scratch is %xt\n", Scratch[task.start..taskEnd]);
      }

      // Step 2: scan
      state.perLocale[0].globalEnds = (+ scan state.perLocale[0].globalCounts) + task.start;

      // Store counts on each other locale
      forall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
        if tid != 0 {
          state.perLocale[tid].globalCounts = state.perLocale[0].globalCounts;
          state.perLocale[tid].globalEnds = state.perLocale[0].globalEnds;
        }
      }

      if debug {
        var total = 0;
        for i in 0..#state.countsSize {
          if state.perLocale[0].globalCounts[i] != 0 {
            total += state.perLocale[0].globalCounts[i];
            writeln("state.globalCounts[", i, "]=", state.perLocale[0].globalCounts[i]);
            writeln("state.globalEnds[", i, "]=", state.perLocale[0].globalEnds[i]);
          }
        }
        assert(total == task.size);
      }

      // Step 3: distribute the keys to the src array according
      // to the globalEnds.
      // In particular, bin i from task j should be stored into
      // globalEnds[i*ntasks+j-1] .. globalEnds[i*ntasks+j] - 1
      // (because the scan is inclusive)
      coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
        on loc do {
          const ref globalCounts = state.perLocale[tid].globalCounts;
          const ref globalEnds = state.perLocale[tid].globalEnds;
          const localSubdomain = A.localSubdomain()[task.start..taskEnd];
          // Compute localOffsets array based on the counts here
          // This could be written as a scan expression...
          ref localOffsets = state.perLocale[tid].compat.counts;
          {
            var offset = localSubdomain.low;
            for bin in 0..#nBuckets {
              localOffsets[bin] = offset;
              offset += globalCounts[bin*nTasks + tid];
            }
          }

          forall bin in 0..#nBuckets {
            var size = globalCounts[bin*nTasks + tid];
            if size > 0 {
              var localStart = localOffsets[bin];
              var localEnd = localStart + size - 1;
              var globalStart = if bin*nTasks+tid > 0
                                then globalEnds[bin*nTasks+tid-1]
                                else taskStart;
              var globalEnd = globalEnds[bin*nTasks+tid] - 1;
              if debug {
                writeln("bin ", bin, " tid ", tid, " range ", taskStart..taskEnd,
                        " A[", globalStart, "..", globalEnd, "] = Scratch[",
                        localStart, "..", localEnd, "]");
                assert(globalCounts[bin*nTasks+tid] ==
                    state.perLocale[0].globalCounts[bin*nTasks+tid]);
                assert(globalEnds[bin*nTasks+tid] ==
                    state.perLocale[0].globalEnds[bin*nTasks+tid]);
              }
              ShallowCopy.shallowCopy(A, globalStart, Scratch, localStart, size);
            }
          }
        }
      }
      // now the data is all in A
      if debug {
        writef("after distribute, A is %xt\n", A[task.start..taskEnd]);
      }

      // Step 4: Add sub-tasks depending on if the bin is local or distributed
      // still.
      for bin in state.perLocale[0].compat.bucketizer.getBinsToRecursivelySort() {
        const binStart = if bin*nTasks > 0
                          then state.perLocale[0].globalEnds[bin*nTasks-1]
                          else task.start;
        const binEnd = state.perLocale[0].globalEnds[bin*nTasks+nTasks-1] - 1;
        const binSize = binEnd - binStart + 1;
        const binStartBit = state.perLocale[0].compat.bucketizer.getNextStartBit(task.startbit);
        if binSize > 1 {
          // could this work for block?
          //var isOnOneLocale = A.domain.dist.idxToLocale(globalStart) ==
          //                    A.domain.dist.idxToLocale(globalEnd)
          var isOnOneLocale = false;
          var theLocaleId = -1;
          for (loc,tid) in zip(A.targetLocales(),0..) {
            const localSubdomain = A.localSubdomain(loc)[task.start..taskEnd];
            const curDomain = {binStart..binEnd};
            const intersect = curDomain[localSubdomain];
            if curDomain == intersect { // curDomain.isSubset(localSubdomain)
              isOnOneLocale = true;
              theLocaleId = tid;
            }
          }

          if isOnOneLocale {
            state.localTasks[theLocaleId].localTasks.append(
                new TwoArraySortTask(binStart, binSize, binStartBit, true, true));
          } else {
            state.distTasks.append(
                new TwoArraySortTask(binStart, binSize, binStartBit, true, true));
          }
        }
      }
    }

    // Handle the local tasks
    coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
      on loc do {
        // Copy the tasks to do from locale 0
        var myTasks = state.localTasks[tid].localTasks;
        var baseCaseSize = state.baseCaseSize;
        ref compat = state.perLocale[tid].compat;

        for task in myTasks {
          const taskEnd = task.start + task.size - 1;
          const curDomain = {task.start..taskEnd};

          // Great! Just sort it locally.
          if n > baseCaseSize {
            compat.bigTasks.clear();
            compat.smallTasks.clear();
            partitioningSortWithScratchSpace(
                task.start, taskEnd,
                A.localSlice(curDomain), Scratch.localSlice(curDomain),
                compat, criterion, task.startbit);
          } else {
            ShellSort.shellSort(A.localSlice(curDomain), criterion,
                start=task.start, end=taskEnd);
          }
          if debug {
            writef("after recursive sorts, dst is %xt\n", A[task.start..taskEnd]);
          }
        }
      }
    }
  }
}

pragma "no doc"
module TwoArrayRadixSort {
  private use TwoArrayPartitioning;
  private use RadixSortHelp;

  proc twoArrayRadixSort(Data:[], comparator:?rec=defaultComparator) {

    var sequentialSizePerTask=4096;
    var baseCaseSize=16;

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    var Scratch: Data.type;

    if Data._instance.isDefaultRectangular() {
      var state = new TwoArrayBucketizerSharedState(
        bucketizer=new RadixBucketizer(),
        baseCaseSize=baseCaseSize,
        sequentialSizePerTask=sequentialSizePerTask,
        endbit=endbit);


      partitioningSortWithScratchSpace(Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state, comparator, 0);
    } else {
      var state = new TwoArrayDistributedBucketizerSharedState(
        bucketizerType=RadixBucketizer,
        numLocales=Data.targetLocales().numElements,
        baseCaseSize=baseCaseSize,
        endbit=endbit);


      distributedPartitioningSortWithScratchSpace(
                                       Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state,
                                       comparator, 0);
    }
  }
}


pragma "no doc"
module TwoArraySampleSort {
  private use TwoArrayPartitioning;
  private use SampleSortHelp;
  private use RadixSortHelp;

  proc twoArraySampleSort(Data:[], comparator:?rec=defaultComparator) {

    var baseCaseSize=16;

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    var Scratch: Data.type;

    if Data._instance.isDefaultRectangular() {
      var state = new TwoArrayBucketizerSharedState(
        bucketizer=new SampleBucketizer(Data.eltType),
        baseCaseSize=baseCaseSize,
        endbit=endbit);

      partitioningSortWithScratchSpace(Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state, comparator, 0);
    } else {
      var state = new TwoArrayDistributedBucketizerSharedState(
        bucketizerType=SampleBucketizer(Data.eltType),
        numLocales=Data.targetLocales().numElements,
        baseCaseSize=baseCaseSize,
        endbit=endbit);

      distributedPartitioningSortWithScratchSpace(
                                       Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state, comparator, 0);
    }
  }
}

pragma "no doc"
module InPlacePartitioning {
  // TODO -- based on ips4o
}

// MSBRadixSort is in Sort.chpl


} // SortExtras Module
