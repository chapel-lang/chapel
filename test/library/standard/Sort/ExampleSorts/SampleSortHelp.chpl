
module SampleSortHelp {
  private use Sort;
  private use CTypes;
  private use Math;

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



  record SampleBucketizer : writeSerializable {
    type eltType;

    proc isSampleSort param do return true;

    // filled from 1 to num_buckets_
    var storage: c_array(eltType, 1<<maxLogBuckets);
    // filled from 0 to num_buckets, last one is duplicated
    var sortedStorage: c_array(eltType, 1<<maxLogBuckets);

    var logBuckets: int;
    var numBuckets: int;
    var equalBuckets: bool;

    proc serialize(writer, ref serializer) throws {
      writer.write("SampleBucketizer(");
      writer.write("\n logBuckets=", logBuckets);
      writer.write("\n numBuckets=", numBuckets);
      writer.write("\n equalBuckets=", equalBuckets);
      writer.write("\n storage=");
      for i in 0..numBuckets {
        writer.write((try! " %xt".format(storage[i])));
      }
      writer.write("\n sortedStorage=");
      for i in 0..numBuckets {
        writer.write(try! " %xt".format(sortedStorage[i]));
      }
      writer.write(")\n");
    }

    proc getNumBuckets() {
      return numBuckets * (1 + equalBuckets:int);
    }
    proc getBinsToRecursivelySort() {
      var r:range(strides = strideKind.positive);
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
    proc ref build(logBuckets: int, equalBuckets: bool) {
      this.logBuckets = logBuckets;
      this.numBuckets = 1 << logBuckets;
      this.equalBuckets = equalBuckets;

      var numSplitters = this.numBuckets - 1;
      // Copy the last element
      sortedStorage[numSplitters] = sortedStorage[numSplitters-1];
      build(0, numSplitters, 1);
    }
    // Recursively builds the tree
    proc ref build(left: int, right: int, pos: int) {
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
      type idxType = A.idxType;
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
            const cur_i_idx = (cur+i):idxType;
            b[i] = 2*b[i] +
                   (chpl_compare(splitter(b[i]), A[cur_i_idx],criterion)<0):int;
          }
        }
        if paramEqualBuckets {
          for /*param*/ i in 0..classifyUnrollFactor-1 {
            const cur_i_idx = (cur+i):idxType;
            b[i] = 2*b[i] +
                   (chpl_compare(A[cur_i_idx],
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
          const cur_idx = cur:idxType;
          bk = 2*bk + (chpl_compare(splitter(bk), A[cur_idx], criterion)<0):int;
        }
        if paramEqualBuckets {
          const cur_idx = cur:idxType;
          bk = 2*bk + (chpl_compare(A[cur_idx],
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
    type idxType = A.idxType;
    ref splitters = splitterBucketizer.sortedStorage;
    var arrayIndex = start_n + sampleStep - 1;
    var splitterIndex = 0;
    splitters[splitterIndex] = A[arrayIndex:idxType];
    for 2..numBuckets-1 {
      arrayIndex += sampleStep;
      // Skip duplicates
      if chpl_compare(splitters[splitterIndex], A[arrayIndex:idxType], criterion)!=0 {
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
    for uniqueSplitters+1 .. numBuckets-1 {
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
                                   ref A:[],
                                   in numSamples:int,
                                   seed=1) {
    private use Random;
    import Sort.ShallowCopy;
    var Tmp:[1..1] A.eltType;
    var randNums = new randomStream(seed=seed, eltType=int);
    while numSamples > 0 {
      numSamples -= 1;

      var offset = randNums.next(start_n, end_n);
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
