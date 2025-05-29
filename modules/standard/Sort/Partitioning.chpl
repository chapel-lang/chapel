/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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


/* This module contains a partition routine (which is parallel and possibly
   distributed parallel, and does a count and then a shuffle according
   to which bucket each element should go into).

   Building on that, it contains a parallel and distributed MSB radix sort
   and sample sort.
 */
@chpldoc.nodoc
module Partitioning {



use Sort.PartitioningUtility;

import Reflection.canResolveMethod;
import Sort;
import Sort.{sort, defaultComparator, keyPartStatus, keyPartComparator};
use Random; // 'use' vs 'import' to workaround an issue
import Math.{log2, divCeil};
import CTypes.c_array;
import BlockDist.blockDist;
import CopyAggregation.{SrcAggregator,DstAggregator};
import BitOps;
import Time;
import RangeChunk;
import Collectives;
import ChplConfig.CHPL_COMM;

// These settings control the sample sort and classification process

// how much more should we sample to create splitters?
// 1.0 would be only to sample enough for the splitters
@unstable("'sampleRatio' is unstable and may change in the future")
private config const sampleRatio = 1.5;
@unstable("'seed' is unstable and may change in the future")
private config const seed = 1;

// switch to base case sort if number of elements is < nBuckets * this
@unstable("'partitionSortBaseCaseMultiplier' is unstable and may change in the future")
private config const partitionSortBaseCaseMultiplier = 100.0;

private config param PARTITIONING_EXTRA_CHECKS = false;
private config param PARTITIONING_SORT_TIMING = false;

param CLASSIFY_UNROLL_FACTOR = 7;
const SAMPLE_RATIO = min(1.0, sampleRatio);
const SEED = seed;
const PARTITION_SORT_BASE_CASE_MULTIPLIER = partitionSortBaseCaseMultiplier;

// compute logarithm base 2 rounded down
proc log2int(n: int) {
  if n <= 0 then
    return 0;
  return log2(n);
}

// compare two records according to a comparator, but allow them
// to be different types.
inline proc mycompare(a, b, comparator) {
  if canResolveMethod(comparator, "key", a) &&
     canResolveMethod(comparator, "key", b) {
    // Use the default comparator to compare the integer keys
    const d = new defaultComparator();
    return d.compare(comparator.key(a), comparator.key(b));
  // Use comparator.compare(a, b) if is defined by user
  } else if canResolveMethod(comparator, "compare", a, b) {
    return comparator.compare(a ,b);
  } else if canResolveMethod(comparator, "keyPart", a, 0) &&
            canResolveMethod(comparator, "keyPart", b, 0) {
    return myCompareByPart(a, b, comparator);
  } else {
    compilerError("The comparator " + comparator.type:string + " requires a 'key(a)', 'compare(a, b)', or 'keyPart(a, i)' method");
  }
}

private inline proc myCompareByPart(a, b, comparator) {
  var curPart = 0;
  while true {
    var (aSection, aPart) = comparator.keyPart(a, curPart);
    var (bSection, bPart) = comparator.keyPart(b, curPart);
    if aSection != keyPartStatus.returned ||
       bSection != keyPartStatus.returned {
      return aSection:int - bSection:int;
    }
    if aPart < bPart {
      return -1;
    }
    if aPart > bPart {
      return 1;
    }

    curPart += 1;
  }

  // This is never reached. The return below is a workaround for issue #10447.
  return 1;
}

inline proc myGetKeyPart(a, comparator, i:int) {
  if canResolveMethod(comparator, "keyPart", a, 0) {
    return comparator.keyPart(a, i);
  } else if canResolveMethod(comparator, "key", a) {
    const ikp = new defaultComparator();
    return ikp.keyPart(comparator.key(a), i);
  } else {
    compilerError("Bad comparator for radix sort ", comparator.type:string,
                  " with eltType ", a.type:string);
  }
}

inline proc myGetBin(a, comparator, startbit:int, param radixBits:int) {
  if canResolveMethod(comparator, "keyPart", a, 0) {
    return myGetBinForKeyPart(a, comparator, startbit, radixBits);
  } else if canResolveMethod(comparator, "key", a) {
    return myGetBinForKeyPart(comparator.key(a),
                              new defaultComparator(),
                              startbit, radixBits);
  } else {
    compilerError("Bad comparator for radix sort ", comparator.type:string,
                  " with eltType ", a.type:string);
  }
}

// Get the bin for a record by calling comparator.keyPart
//
// p = 1 << radixBits
//
// bin 0 is for the end was reached (sort before)
// bins 1..p are for data with next part starting with 0..<p
// bin p+1 is for the end was reached (sort after)
//
// returns bin
inline proc myGetBinForKeyPart(a, comparator, startbit:int, param radixBits:int) {
  // We have keyPart(element, start):(keyPartStatus, part which is integral)
  const testRet: comparator.keyPart(a, 0).type;
  const testPart = testRet(1); // get the numeric part
  param bitsPerPart = numBits(testPart.type);
  if PARTITIONING_EXTRA_CHECKS {
    assert(bitsPerPart >= radixBits);
    assert(bitsPerPart % radixBits == 0);
  }

  // startbit must be a multiple of radixBits because the radix
  // sort operates radixBits at a time.

  // startbit might be partway through a part (e.g. 16 bits into a uint(64))
  const whichpart = startbit / bitsPerPart;
  const bitsinpart = startbit % bitsPerPart;

  const (section, part) = comparator.keyPart(a, whichpart);
  var ubits = part:uint(bitsPerPart);
  // If the number is signed, invert the top bit, so that
  // the negative numbers sort below the positive numbers
  if isInt(part) {
    const one:ubits.type = 1;
    ubits = ubits ^ (one << (bitsPerPart - 1));
  }
  const mask:uint = (1 << radixBits) - 1;
  const ubin = (ubits >> (bitsPerPart - bitsinpart - radixBits)) & mask;

  if section:int == 0 then
    return ubin:int + 1; // a regular bin
  else if section:int < 0 then
    return 0; // the sort-before bin
  else
    return (1 << radixBits) + 1; // the sort-after bin
}

/* This enum describes to what extent the sample is already sorted */
enum sortLevel {
  unsorted,
  approximately,
  fully
}

// Compute splitters from a sorted sample.
// Returns an array of splitters that is of size 2**n,
// where only the first 2**n-1 elements are used.
// If equality buckets are not in use, there will be 2**n buckets.
// If they are in use, there will be 2**(n+1)-1 buckets.
// n will be chosen by this function so that the number of buckets
// is <= max(2,requestedNumBuckets).
// Assumes that SortedSample is 0-based and non-strided.
private proc computeSplitters(const SortedSample,
                              in requestedNumBuckets: int,
                              comparator,
                              reSort: bool,
                              out useEqualBuckets: bool) {
  if requestedNumBuckets > SortedSample.size then
    requestedNumBuckets = SortedSample.size;
  var myNumBuckets = max(2, 1 << log2int(requestedNumBuckets));
  var numSplitters = myNumBuckets-1;
  var SortedSplitters:[0..<myNumBuckets] SortedSample.eltType;

  // gather the sample assuming that SortedSample is sorted
  if myNumBuckets == SortedSample.size {
    // don't try to sample it, we already have what we need!
    SortedSplitters = SortedSample;
  } else {
    const perSplitter = SortedSample.size:real / (numSplitters+1):real;
    var start = perSplitter:int;

    for i in 0..<numSplitters {
      var sampleIdx = start + (i*perSplitter):int;
      sampleIdx = min(max(sampleIdx, 0), SortedSample.size-1);
      SortedSplitters[i] = SortedSample[sampleIdx];
    }
  }

  if reSort {
    sort(SortedSplitters[0..<numSplitters], comparator);
    if PARTITIONING_EXTRA_CHECKS {
      assert(isSorted(SortedSplitters[0..<numSplitters], comparator));
    }
  }

  // check for duplicates.
  var nDuplicates = 0;
  for i in 1..<numSplitters {
    if mycompare(SortedSplitters[i-1], SortedSplitters[i], comparator) == 0 {
      nDuplicates += 1;
    }
  }

  // if there are no duplicates, proceed with what we have
  if nDuplicates == 0 {
    useEqualBuckets = false;
    return SortedSplitters;
  }

  // copy the last element to make the following code simpler
  // (normally we leave space in the last element for use in build())
  SortedSplitters[numSplitters] = SortedSplitters[numSplitters-1];

  // if there were duplicates, reduce the number of splitters accordingly,
  // activate equality buckets, and return a de-duplicated array.
  // note, when using equality buckets, the number of buckets
  // will be 2 * the number of splitters, so here we
  // are aiming for a smaller number of splitters.

  var oldNumSplitters = numSplitters;
  const nUnique = oldNumSplitters - nDuplicates;
  myNumBuckets = 1 << (1+log2int(nUnique));
  while 2*myNumBuckets > requestedNumBuckets {
    myNumBuckets /= 2;
  }
  myNumBuckets = max(1, myNumBuckets);
  numSplitters = myNumBuckets-1;

  var UniqueSplitters:[0..<myNumBuckets] SortedSample.eltType;

  var next = 0;

  // gather the sample from SortedSplitters
  {
    if nUnique <= myNumBuckets {
      // Gather the unique elements
      UniqueSplitters[0] = SortedSplitters[0];
      next = 1;
      for i in 1..<oldNumSplitters {
        // keep elements that differ from the last splitter added,
        // and discard elements that are the same.
        if mycompare(UniqueSplitters[next-1],
                     SortedSplitters[i], comparator) != 0 {
          UniqueSplitters[next] = SortedSplitters[i];
          next += 1;
        }
      }
    } else {
      // myNumBuckets < nUnique
      const perSplitter = nUnique:real / myNumBuckets:real;
      var start = perSplitter:int;

      next = 0;
      for i in 0..<oldNumSplitters {
        if next == numSplitters then break;
        var sampleIdx = start + (i*perSplitter):int;
        sampleIdx = min(max(sampleIdx, 0), SortedSplitters.size-1);
        if next == 0 ||
           mycompare(UniqueSplitters[next-1],
                     SortedSplitters[sampleIdx], comparator) != 0 {
          UniqueSplitters[next] = SortedSplitters[sampleIdx];
          next += 1;
        }
      }
    }
  }

  if PARTITIONING_EXTRA_CHECKS {
    for i in 1..<next {
      assert(mycompare(UniqueSplitters[i-1], UniqueSplitters[i], comparator) < 0);
    }
  }

  // repeat the last splitter to get to the power of 2
  // note: myNumBuckets-1 is not set here, it is set in build()
  while next < numSplitters {
    UniqueSplitters[next] = UniqueSplitters[next-1];
    next += 1;
  }

  useEqualBuckets = true;
  return UniqueSplitters;
}

/*
   The splitters record helps with distribution sorting, where input elements
   are split among buckets according to how they compares with a group of
   splitter elements.

   It creates a binary comparison tree and uses that to classify the input in an
   optimized manner.
 */
record splitters : writeSerializable {
  type eltType;

  var logSplitters: int;
  var myNumBuckets: int; // number of buckets if no equality buckets
  var equalBuckets: bool;

  // filled from 1..<myNumBuckets
  var storage: [0..<(1<<logSplitters)] eltType;
  // filled from 0..myNumBuckets-2; myNumBuckets-1 is a duplicate of previous
  var sortedStorage: [0..<(1<<logSplitters)] eltType;

  proc init(type eltType) {
    // default init, creates invalid splitters, but useful for replicating
    this.eltType = eltType;
  }
  // creates space for splitters without creating valid splitters
  // numBuckets should be 2**n for some n
  // creates space for splitters assuming that equality bucket will not be used
  // (if they are, a fewer number of splitters will be needed)
  proc init(type eltType, numBuckets: int) {
    this.eltType = eltType;
    this.logSplitters = log2int(numBuckets);
    this.myNumBuckets = 1 << logSplitters;
    init this; // allocate 'storage' and 'sortedStorage'
    myNumBuckets = 0;
  }

  // Create splitters based on some precomputed, already sorted splitters
  // useSplitters needs to be of size 2**n and the last element will
  // not be used. If 'useEqualBuckets=false', there will be 2**n
  // buckets; otherwise there will be 2**(n+1)-1 buckets.
  // Assumes that UseSplitters starts at 0 and is not strided.
  proc init(in UseSplitters: [], useEqualBuckets: bool) {
    assert(UseSplitters.size >= 2);
    this.eltType = UseSplitters.eltType;
    this.logSplitters = log2int(UseSplitters.size);
    this.myNumBuckets = 1 << logSplitters;
    assert(this.myNumBuckets == UseSplitters.size);
    assert(this.myNumBuckets >= 2);
    this.equalBuckets = useEqualBuckets;
    this.sortedStorage = UseSplitters;
    init this;

    // Build the tree in 'storage'
    this.build();
  }

  // create splitters based upon a sample of data.
  proc init(const Sample,
            requestedNumBuckets: int,
            comparator,
            param howSorted: sortLevel) where howSorted!=sortLevel.unsorted {
    var useEqualBuckets = false;
    const Splitters = computeSplitters(Sample, requestedNumBuckets,
                                       comparator,
                                       reSort=
                                         (howSorted==sortLevel.approximately),
                                       /*out*/ useEqualBuckets);

    this.init(Splitters, useEqualBuckets);

    if PARTITIONING_EXTRA_CHECKS then assert(this.numBuckets <= max(2,requestedNumBuckets));
  }

  // create splitters based upon a sample of data by sorting it
  proc init(ref Sample:[],
            requestedNumBuckets: int,
            comparator,
            param howSorted: sortLevel) where howSorted==sortLevel.unsorted {
    // sort the sample
    sort(Sample, comparator);

    var useEqualBuckets = false;
    const Splitters = computeSplitters(Sample, requestedNumBuckets,
                                       comparator, reSort=false,
                                       /*out*/ useEqualBuckets);

    this.init(Splitters, useEqualBuckets);

    if PARTITIONING_EXTRA_CHECKS then assert(this.numBuckets <= max(2,requestedNumBuckets));
  }

  proc ref setStorageFrom(const ref rhs: splitters(?)) {
    // use bulk comms to copy from a remote array
    var arrayBounds = storage.domain.dim(0);
    var region = arrayBounds[0..<rhs.myNumBuckets];
    bulkCopy(this.storage, region, rhs.storage, region);
    bulkCopy(this.sortedStorage, region, rhs.sortedStorage, region);

    // clear any elements beyond the number of splitters
    for i in region.high+1..arrayBounds.high {
      var empty: eltType;
      this.storage[i] = empty;
      this.sortedStorage[i] = empty;
    }
  }

  // these allow splitters to be pre-allocated even though
  // the number of splitter elements might change.
  proc init=(const ref rhs: splitters(?)) {
    this.eltType = rhs.eltType;
    this.logSplitters = rhs.logSplitters;
    this.myNumBuckets = rhs.myNumBuckets;
    this.equalBuckets = rhs.equalBuckets;
    init this;
    this.setStorageFrom(rhs);
    //writeln("splitters init= on ", here, " from ", rhs.locale, " trace: ", stackTraceAsString(b" "));
  }
  operator =(ref lhs: splitters(?), const ref rhs: splitters(?)) {
    lhs.logSplitters = rhs.logSplitters;
    lhs.myNumBuckets = rhs.myNumBuckets;
    lhs.equalBuckets = rhs.equalBuckets;
    lhs.setStorageFrom(rhs);
    //writeln("splitters = on ", here, " from ", rhs.locale, " trace: ", stackTraceAsString(b" "));
  }
  operator ==(const ref lhs: splitters(?), const ref rhs: splitters(?)) {
    if lhs.logSplitters != rhs.logSplitters ||
       lhs.myNumBuckets != rhs.myNumBuckets ||
       lhs.equalBuckets != rhs.equalBuckets {
      return false;
    }
    for i in 0..<rhs.myNumBuckets {
      if i < lhs.myNumBuckets {
        if lhs.storage[i] != rhs.storage[i] ||
           lhs.sortedStorage[i] != rhs.sortedStorage[i] {
          return false;
        }
      }
    }
    return true;
  }

  proc serialize(writer, ref serializer) throws {
    writer.write("splitters(");
    writer.write("\n logSplitters=", logSplitters);
    writer.write("\n myNumBuckets=", myNumBuckets);
    writer.write("\n equalBuckets=", equalBuckets);
    writer.write("\n storage.size=", storage.size);
    writer.write("\n storage=");
    for i in 0..<myNumBuckets {
      writer.writeln(storage[i]);
    }
    writer.write("\n sortedStorage=");
    for i in 0..<myNumBuckets {
      writer.writeln(sortedStorage[i]);
    }
    writer.write(")\n");
  }

  proc summary() {
    var ret = new splittersSummary(logSplitters, myNumBuckets, equalBuckets);
    if PARTITIONING_EXTRA_CHECKS {
      assert(ret.numBuckets == numBuckets);
      for i in 0..<numBuckets {
        assert(ret.bucketHasEqualityBound(i) == bucketHasEqualityBound(i));
      }
    }
    return ret;
  }

  proc numBuckets {
    if equalBuckets {
      return myNumBuckets*2-1;
    } else {
      return myNumBuckets;
    }
  }

  proc hasEqualityBuckets {
    return equalBuckets;
  }

  proc bucketHasLowerBound(bucketIdx: int) {
    // bucket 0 never has a lower bound
    if bucketIdx == 0 {
      return false;
    }
    // the equality buckets are odd buckets
    if equalBuckets {
      return bucketIdx % 2 == 0;
    } else {
      return true;
    }
  }
  // things in the bucket are > this
  proc bucketLowerBound(bucketIdx: int) const ref {
    if equalBuckets {
      return sortedSplitter(bucketIdx/2-1);
    } else {
      return sortedSplitter(bucketIdx-1);
    }
  }

  proc bucketHasUpperBound(bucketIdx: int) {
    if equalBuckets {
      if bucketIdx >= 2*myNumBuckets-2 {
        return false;
      }
      return bucketIdx % 2 == 0; // odd buckets are equality buckets
    } else {
      if bucketIdx >= myNumBuckets-1 {
        return false;
      }
      return true;
    }
  }
  // things in the bucket are <= the result of this function
  // (actually, < the result, if equality buckets are in use)
  proc bucketUpperBound(bucketIdx: int) const ref {
    if equalBuckets {
      return sortedSplitter(bucketIdx/2);
    } else {
      return sortedSplitter(bucketIdx);
    }
  }

  proc bucketHasEqualityBound(bucketIdx: int) {
    if equalBuckets {
      return bucketIdx % 2 == 1;
    }
    return false;
  }

  // things in the bucket are < the result of this function
  proc bucketEqualityBound(bucketIdx: int) const ref {
    return sortedSplitter((bucketIdx-1)/2);
  }

  // Build the tree from the sorted splitters
  // logSplitters does not account for equalBuckets.
  proc ref build() {
    // Copy the last element
    sortedStorage[myNumBuckets-1] = sortedStorage[myNumBuckets-2];
    build(0, myNumBuckets-1, 1);
  }

  // Recursively builds the tree
  proc ref build(left: int, right: int, pos: int) {
    var mid = left + (right - left) / 2;
    storage[pos] = sortedStorage[mid];
    if 2*pos < myNumBuckets {
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

  proc bucketForRecord(a, comparator) {
    var bk = 1;
    for lg in 0..<logSplitters {
      bk = 2*bk + (mycompare(splitter(bk), a, comparator) < 0):int;
    }
    if equalBuckets {
      bk = 2*bk + (mycompare(sortedSplitter(bk-myNumBuckets), a, comparator) == 0):int;
    }
    return bk - (if equalBuckets then 2*myNumBuckets else myNumBuckets);
  }
  // yields (value, bucket index) for start_n..end_n
  // gets the elements by calling Input[i] to get element i
  // Input does not have to be an array, but it should have an eltType.
  iter classify(Input, start_n, end_n, comparator) {
    const paramEqualBuckets = equalBuckets;
    const paramLogBuckets = logSplitters;
    const paramNumBuckets = 1 << (paramLogBuckets + paramEqualBuckets:int);
    var b:[0..<CLASSIFY_UNROLL_FACTOR] int;
    var elts:[0..<CLASSIFY_UNROLL_FACTOR] Input.eltType;

    var cur = start_n;
    // Run the main (unrolled) loop
    while cur <= end_n-(CLASSIFY_UNROLL_FACTOR-1) {
      for /*param*/ i in 0..CLASSIFY_UNROLL_FACTOR-1 {
        b[i] = 1;
        elts[i] = Input[cur+i];
      }
      for /*param*/ lg in 0..paramLogBuckets-1 {
        for /*param*/ i in 0..CLASSIFY_UNROLL_FACTOR-1 {
          b[i] = 2*b[i] +
                 (mycompare(splitter(b[i]), elts[i],comparator)<0):int;
        }
      }
      if paramEqualBuckets {
        for /*param*/ i in 0..CLASSIFY_UNROLL_FACTOR-1 {
          b[i] = 2*b[i] +
                 (mycompare(sortedSplitter(b[i] - paramNumBuckets/2),
                            elts[i],
                            comparator)==0):int;
        }
      }
      for /*param*/ i in 0..CLASSIFY_UNROLL_FACTOR-1 {
        yield (elts[i], b[i]-paramNumBuckets);
      }
      cur += CLASSIFY_UNROLL_FACTOR;
    }
    // Handle leftover
    while cur <= end_n {
      elts[0] = Input[cur];
      var bk = 1;
      for lg in 0..<paramLogBuckets {
        bk = 2*bk + (mycompare(splitter(bk), elts[0], comparator)<0):int;
      }
      if paramEqualBuckets {
        bk = 2*bk + (mycompare(sortedSplitter(bk - paramNumBuckets/2),
                               elts[0],
                               comparator)==0):int;
      }
      yield (elts[0], bk - paramNumBuckets);
      cur += 1;
    }
  }
} // end record splitters

/* helper record for splitters that doesn't actually include the splitters,
   only the bounds information */
pragma "always RVF" // bug workaround
record splittersSummary {
  var logSplitters: int;
  var myNumBuckets: int; // number of buckets if no equality buckets
  var equalBuckets: bool;

  proc numBuckets {
    if equalBuckets {
      return myNumBuckets*2-1;
    } else {
      return myNumBuckets;
    }
  }

  proc bucketHasEqualityBound(bucketIdx: int) {
    if equalBuckets {
      return bucketIdx % 2 == 1;
    }
    return false;
  }
}

proc isSampleSplitters(type splitType) param {
  return isSubtype(splitType, splitters);
}

// splits into (1 << radixBits) + 2 bins
//
// p = 1 << radixBits
//
// bin 0 is for the end was reached (sort before)
// bins 1..p are for data with next part starting with 0..<p
// bin p+1 is for the end was reached (sort after)
record radixSplitters : writeSerializable {
  param radixBits: int; // how many bits to sort at once
  var startbit: int;  // start bit position
  var endbit: int;    // when startbit==endbit, everything compares equal

  proc init(param radixBits) {
    this.radixBits = radixBits;
    // default init, creates invalid splitters, but useful for replicating
  }
  proc init(numBuckets: int) {
    startbit = 0;
    endbit = max(int);
  }
  proc init(param radixBits, numBuckets: int) {
    this.radixBits = radixBits;
    startbit = 0;
    endbit = max(int);
  }
  // creates a valid radixSplitter
  proc init(param radixBits: int, startbit: int, endbit: int) {
    this.radixBits = radixBits;
    this.startbit = startbit;
    this.endbit = endbit;
  }

  proc serialize(writer, ref serializer) throws {
    writer.write("radixSplitters(");
    writer.write("\n radixBits=", radixBits);
    writer.write("\n startbit=", startbit);
    writer.write("\n endbit=", endbit);
    writer.write(")\n");
  }

  proc summary() {
    var ret = new radixSplittersSummary(radixBits, startbit, endbit);
    if PARTITIONING_EXTRA_CHECKS {
      assert(ret.numBuckets == numBuckets);
      for i in 0..<numBuckets {
        assert(ret.bucketHasEqualityBound(i) == bucketHasEqualityBound(i));
      }
    }
    return ret;
  }

  proc numBuckets param {
    return (1 << radixBits) + 2; // +2 for end-before and end-after bins
  }

  proc bucketHasEqualityBound(bucketIdx: int) {
    return bucketIdx == 0 ||
           bucketIdx == numBuckets - 1 ||
           startbit >= endbit - radixBits;
  }


  inline proc bucketForRecord(a, comparator) {
    return myGetBin(a, comparator, startbit, radixBits);
  }

  // yields (value, bucket index) for start_n..end_n
  // gets the elements by calling Input[i] to get element i
  // Input does not have to be an array, but it should have an eltType.
  iter classify(Input, start_n, end_n, comparator) {
    var cur = start_n;
    while cur <= end_n-(CLASSIFY_UNROLL_FACTOR-1) {
      for /*param*/ j in 0..CLASSIFY_UNROLL_FACTOR-1 {
        const elt = Input[cur+j];
        yield (elt, bucketForRecord(elt, comparator));
      }
      cur += CLASSIFY_UNROLL_FACTOR;
    }
    while cur <= end_n {
      const elt = Input[cur];
      yield (elt, bucketForRecord(elt, comparator));
      cur += 1;
    }
  }
} // end record radixSplitters

pragma "always RVF" // bug workaround
record radixSplittersSummary {
  var radixBits: int;
  var startbit: int;
  var endbit: int;

  proc numBuckets {
    return (1 << radixBits) + 2; // +2 for end-before and end-after bins
  }

  proc bucketHasEqualityBound(bucketIdx: int) {
    return bucketIdx == 0 ||
           bucketIdx == numBuckets - 1 ||
           startbit >= endbit - radixBits;
  }
}


record bktCount {
  var start: int;
  var count: int;
}


/*
   Stores the elements Input[InputDomain] in a partitioned manner
   into Output[OutputDomain].

   InputDomain must not be strided. It must be local rectangular domains or
   Block distributed domains.

   Input can be an array over InputDomain or something that simulates
   an array with a 'proc this' and an 'eltType' to generate element i.

   'inputRegion' is the region within InputDomain to consider.

   Output is expected to be an array or something that functions as an array.
   If Output is 'none', this function will only count, and skip the partition
   step.

   OutputShift is a value that can be added to each bucket position
   to adjust for the output position. It can be:

     * 'none' to do nothing special
     * an integer index to add to all output positions
     * an array of size nBuckets to add an amount per-bucket

   'filterBucket' provides a mechanism to only process certain buckets.
   If 'filterBucket' is provided and not 'none', it will be called as
   'filterBucket(bucketForRecord(Input[i]))' to check if that bucket should
   be processed. Only elements where it returns 'true' will be processed.

   Return an array of bktCount counts to indicate how many elements
   ended up in each bucket, the start of the bucket, and if it
   is an equality bucket. This resulting array is never distributed.

   This is done in parallel & distributed (if InputDom is distributed).

   'split' is the splitters and it should be either 'record splitters'
   or something else that behaves similarly to it.

   If equality buckets are not in use:

     Bucket 0 consists of elts with
       elts <= split.sortedSplitter(0)
     Bucket 1 consists of elts with
       split.sortedSplitter(0) < elts <= split.sortedSplitter(1)

     Bucket i consists of elts with
       split.sortedSplitter(i-1) < elts <= split.sortedSplitter(i)

     Bucket nBuckets-1 consits of elt with
       split.sortedSplitter(numBuckets-2) < elts

   If equality buckets are in use:

     Bucket 0 consists of elts with
       elts < split.sortedSplitter(0)
     Bucket 1 consists of elts with
       elts == split.sortedSplitter(0)
     Bucket 2 consists of elts with
       split.sortedSplitter(0) < elts < split.sortedSplitter(1)
     Bucket 3 consists of elts with
       elts == split.sortedSplitter(1)
     Bucket 4 consists of elts with
       split.sortedSplitter(1) < elts < split.sortedSplitter(2)

     Bucket i, with i being even, consists of elts with
       split.sortedSplitter(i/2-1) < elts < split.sortedSplitter(i/2)
     Bucket i, with i being odd, consists of elts with
       elts == split.sortedSplitter((i-1)/2)

     Bucket nBuckets-2 consits of elt with
       elts == split.sortedSplitter((numBuckets-2)/2) < elts
     Bucket nBuckets-1 consits of elt with
       split.sortedSplitter((numBuckets-2)/2) < elts

 */
proc partition(const InputDomain: domain(?),
               const inputRegion: range,
               const Input,
               const OutputShift,
               ref Output,
               split, comparator,
               const nTasksPerLocale: int,
               const activeLocs: [] locale = computeActiveLocales(InputDomain,
                                                                  inputRegion),
               filterBucket: ?t = none,
               noSerialPartition = false) {
  if PARTITIONING_EXTRA_CHECKS {
    // check that the splitters are sorted according to comparator
    if isSampleSplitters(split.type) {
      assert(isSorted(split.sortedStorage[0..<split.myNumBuckets-1],
                      comparator));
    }

    // 'here' should be one of the active locales
    var found = false;
    for loc in activeLocs {
      if loc == here then found = true;
    }
    assert(found);
  }

  const nBuckets = split.numBuckets;

  if nTasksPerLocale <= 1 && activeLocs.size <= 1 && !noSerialPartition {
    var Counts:[0..<nBuckets] int;
    var Starts:[0..<nBuckets] int;
    var Ret: [0..<nBuckets] bktCount;

    serialStablePartition(inputRegion, Input, OutputShift, Output,
                          split, comparator, filterBucket,
                          Counts, Starts, Ret);
    return Ret;
  }

  const nActiveLocales = activeLocs.size;
  const countsPerBucket = nActiveLocales*nTasksPerLocale;
  const countsSize = nBuckets*countsPerBucket;

  if activeLocs.size <= 2 {
    // allocate local counts as a local array which should go OK
    // when working with 1 or 2 locales and avoid distributed array creation
    // overheads.
    var GlobCounts: [0..<countsSize] int;
    var Ends:[0..<nBuckets] int;
    var Ret: [0..<nBuckets] bktCount;

    parStablePartition(InputDomain, inputRegion, Input,
                       OutputShift, Output,
                       split, comparator, filterBucket,
                       nTasksPerLocale, activeLocs,
                       GlobCounts, Ends, Ret);
    return Ret;

  } else {
    // use a distributed counts arrays
    const GlobCountsDom = blockDist.createDomain(0..<countsSize);
    var GlobCounts: [GlobCountsDom] int;
    const CountsDom = blockDist.createDomain(0..<nBuckets);
    var EndsDist:[CountsDom] int;
    var RetDist:[CountsDom] bktCount;
    var Ret:[0..<nBuckets] bktCount;

    parStablePartition(InputDomain, inputRegion, Input,
                       OutputShift, Output,
                       split, comparator, filterBucket,
                       nTasksPerLocale, activeLocs,
                       GlobCounts, EndsDist, RetDist);

    // This is a Local = Distributed assignment,
    // written this way for AVE.
    // Expect all locales to be involved.
    Ret[0..<nBuckets] = RetDist[0..<nBuckets];

    return Ret;
  }
}

proc serialUnstablePartition(const region: range,
                             ref A: [],
                             split,
                             comparator,
                             ref Starts:[] int,
                             ref Ends:[] int,
                             ref Ret:[] bktCount) : void {
  const nBuckets = split.numBuckets;

  if PARTITIONING_EXTRA_CHECKS {
    assert(Starts.domain.dim(0) == 0..<nBuckets);
    assert(Ends.domain.dim(0) == 0..<nBuckets);
    assert(Ret.domain.dim(0) == 0..<nBuckets);
    for i in 0..<nBuckets {
      assert(Starts[i] == 0);
      assert(Ends[i] == 0);
    }
  }

  // Step 1: count
  for (_,bin) in split.classify(A, region.low, region.high, comparator) {
    Starts[bin] += 1;
  }

  // Step 2: scan (this one is an exclusive scan)
  {
    var sum: int = region.low;
    for i in 0..<nBuckets {
      ref start = Starts[i];
      ref end = Ends[i];
      var bktstart = sum;
      sum += start; // starts stores counts at first
      var bktend = sum;
      start = bktstart;
      end = bktend;
    }
  }

  // Step 3: distribute
  var curBucket = 0;
  while true {
    // find the next bin that isn't totally in place
    while curBucket < nBuckets && Starts[curBucket] == Ends[curBucket] {
      curBucket += 1;
    }
    if curBucket >= nBuckets {
      break;
    }

    param max_buf = CLASSIFY_UNROLL_FACTOR;
    var buf: max_buf*A.eltType;
    var used_buf = 0;
    var start = Starts[curBucket];
    var end = Ends[curBucket];
    var endfast = max(start, end-2*max_buf);
    var bufstart = max(start, end-max_buf);
    var i = bufstart;

    // Fill buf with up to max_buf elements from the end of this bin.
    while i < end {
      buf[used_buf] <=> A[i];
      used_buf += 1;
      i += 1;
    }

    // put the elements in buf into their correct home,
    // swapping in whatever was there
    while Starts[curBucket] < endfast {
      for param j in 0..<max_buf {
        // TODO: adjust classify() to return the input index
        // and then call it here instead
        var bkt = split.bucketForRecord(buf[j], comparator);
        // Store it in the right bkt and increment that bucket start
        ref next = Starts[bkt];
        A[next] <=> buf[j];
        next += 1;
      }
    }

    // handle elements in bufstart...end_offsets[curBucket]
    while Starts[curBucket] < end {
      // Put buf[j] into its right home
      var j = 0;
      while used_buf >= 0 && j < used_buf {
        var bkt = split.bucketForRecord(buf[j], comparator);
        // Swap buf[j] into its appropriate bin.
        ref next = Starts[bkt];
        var offset = next;
        A[offset] <=> buf[j];
        next += 1;
        // Leave buf[j] with the next unsorted item.
        // But offsets[bin] might be in the region we already read.
        if bkt == curBucket && offset >= bufstart {
          used_buf -= 1;
          buf[j] <=> buf[used_buf];
        }
        j += 1;
      }
    }
  }

  // Compute the array to return using Ends
  for i in 0..<nBuckets {
    var end = Ends[i];
    var prevEnd = 0;
    if i > 0 {
      prevEnd = Ends[i-1];
    }
    var count = end - prevEnd;
    var start = end - count;
    ref r = Ret[i];
    r.start = start;
    r.count = count;
  }
}

proc serialStablePartition(const inputRegion: range,
                           const Input,
                           const OutputShift,
                           ref Output,
                           split,
                           comparator,
                           filterBucket,
                           ref Counts:[] int,
                           ref Starts:[] int,
                           ref Ret:[] bktCount) : void {
  const nBuckets = split.numBuckets;

  if PARTITIONING_EXTRA_CHECKS {
    assert(Counts.domain.dim(0) == 0..<nBuckets);
    assert(Starts.domain.dim(0) == 0..<nBuckets);
    assert(Ret.domain.dim(0) == 0..<nBuckets);
    for i in 0..<nBuckets {
      assert(Counts[i] == 0);
      assert(Starts[i] == 0);
    }
  }

  // Step 1: count
  for (_,bkt) in split.classify(Input, inputRegion.low, inputRegion.high,
                                comparator) {
    if filterBucket.type == nothing || filterBucket(bkt) {
      Counts[bkt] += 1;
    }
  }

  if Output.type != nothing {
    // Step 2: scan (this one is an exclusive scan)
    var sum: int = 0;
    for (start, count, bucketIdx) in zip(Starts, Counts, Counts.domain) {
      var shift = 0;
      if isArrayType(OutputShift.type) {
        shift = OutputShift[bucketIdx];
      } else if isIntType(OutputShift.type) {
        shift = OutputShift;
      }
      start = sum + shift;
      sum += count;
    }

    // Step 3: distribute
    for (elt,bkt) in split.classify(Input, inputRegion.low, inputRegion.high,
                                    comparator) {
      if filterBucket.type == nothing || filterBucket(bkt) {
        // Store it in the right bucket & increment the bucket counter
        ref next = Starts[bkt];
        Output[next] = elt;
        next += 1;
      }
    }
  }

  // Compute the array to return
  var sum: int = 0;
  for (r, count, bucketIdx) in zip(Ret, Counts, Counts.domain) {
    var shift = 0;
    if isArrayType(OutputShift.type) {
      shift = OutputShift[bucketIdx];
    } else if isIntType(OutputShift.type) {
      shift = OutputShift;
    }

    r.start = sum + shift;
    r.count = count;
    sum += count;
  }
}

inline proc getGlobalCountIdx(bucketIdx: int,
                              locIdx: int,
                              nLocales: int,
                              taskIdInLoc: int,
                              nTasksPerLocale: int): int {
  return bucketIdx*nLocales*nTasksPerLocale
         + locIdx*nTasksPerLocale
         + taskIdInLoc;
}

// perTaskCounts is an array-of-arrays containing the counts
//   perTaskCounts[taskIdInLoc][bucketIdx]
// GlobCounts is the global counts array
proc savePerTaskCountsToGlobal(const ref perTaskCounts,
                               ref GlobCounts: [] int,
                               const nBuckets: int,
                               const nActiveLocales: int,
                               const activeLocIdx: int,
                               const nTasksPerLocale: int) {
  // store the perTaskCounts into the global counts array in parallel
  // but do so in a way that somewhat matches the global counts ordering
  // (tasks within a bucket go together).
  forall bucketIdx in 0..<nBuckets
  with (var agg = new DstAggregator(int)) {
    for taskIdInLoc in 0..<nTasksPerLocale {
      var countIdx =
        getGlobalCountIdx(bucketIdx, activeLocIdx, nActiveLocales,
                          taskIdInLoc, nTasksPerLocale);
      agg.copy(GlobCounts[countIdx], perTaskCounts[taskIdInLoc][bucketIdx]);
    }
  }
}

// perTaskNext is an array-of-arrays containing the counts
//   perTaskNext[taskIdInLoc][bucketIdx]
// GlobEnds is the global ends array
proc getTaskCountsFromGlobal(ref perTaskNext,
                             const ref GlobEnds,
                             const nBuckets: int,
                             const nActiveLocales: int,
                             const activeLocIdx: int,
                             const nTasksPerLocale: int) {
  // read the start positions from GlobEnds into perTaskNext
  // but do so in a way that somewhat matches the global counts ordering
  // (tasks within a bucket go together).
  forall bucketIdx in 0..<nBuckets
  with (var agg = new SrcAggregator(int)) {
    for taskIdInLoc in 0..<nTasksPerLocale {
      var countIdx =
        getGlobalCountIdx(bucketIdx, activeLocIdx, nActiveLocales,
                          taskIdInLoc, nTasksPerLocale);
      if countIdx > 0 {
        agg.copy(perTaskNext[taskIdInLoc][bucketIdx], GlobEnds[countIdx-1]);
      } else {
        perTaskNext[taskIdInLoc][bucketIdx] = 0;
      }
    }
  }
}


proc parStablePartition(const InputDomain: domain(?),
                        const inputRegion: range,
                        const Input,
                        const OutputShift,
                        ref Output,
                        split, comparator, filterBucket,
                        const nTasksPerLocale: int,
                        const activeLocs: [] locale,
                        // the following may be distributed
                        ref GlobCounts: [] int,
                        ref Ends:[] int,
                        ref Ret:[] bktCount) : void {

  // GlobalCounts stores counts like this:
  //   count for bin 0, locale 0, task 0..<nTasksPerLocale
  //   count for bin 0, locale 1, task 0..<nTasksPerLocale
  //   ...
  //   count for bin 0, locale numLocales-1, task 0..<nTasksPerLocale
  //   count for bin 1, locale 0, task 0..<nTasksPerLocale
  //   count for bin 1, locale 1, task 0..<nTasksPerLocale
  //   ...
  //   count for bin 1, locale numLocales-1, task 0..<nTasksPerLocale
  //   ...
  // i.e. GlobalCounts[bucketIdx*numLocales*nTasksPerLocale
  //                   + here.id*nTasksPerLocale
  //                   + taskIdInLoc]

  // the structure here and use of perTaskCounts is based on the
  // optimization described in https://github.com/Bears-R-Us/arkouda/pull/1635

  const nBuckets = split.numBuckets;
  const nActiveLocales = activeLocs.size;

  if PARTITIONING_EXTRA_CHECKS {
    const globSize = nBuckets*nActiveLocales*nTasksPerLocale;
    assert(GlobCounts.domain.dim(0) == 0..<globSize);
    assert(Ends.domain.dim(0) == 0..<nBuckets);
    assert(Ret.domain.dim(0) == 0..<nBuckets);
    forall elt in GlobCounts {
      assert(elt == 0);
    }
    forall elt in Ends {
      assert(elt == 0);
    }
  }

  var GlobEnds: GlobCounts.type;

  //const useAllLocalesBarrier = activeLocs.equals(Locales);
  var smallerBarrier = new Collectives.barrier(activeLocs.size);

  forall (activeLocIdx, locRegion)
  in divideByLocales(InputDomain, inputRegion, activeLocs) {
    var lsplit = split;
    var perTaskCounts: [0..<nTasksPerLocale] [0..<nBuckets] int;

    // Step 1: Count
    // count & save the result to the perTaskCounts
    coforall (chunk, taskIdInLoc)
    in zip(RangeChunk.chunks(locRegion, nTasksPerLocale), 0..) {
      ref mycounts = perTaskCounts[taskIdInLoc];
      for (_,bkt) in lsplit.classify(Input, chunk.low, chunk.high, comparator) {
        if filterBucket.type == nothing || filterBucket(bkt) {
          mycounts[bkt] += 1;
        }
      }
    }

    // save the perTaskCounts back into GlobCounts
    savePerTaskCountsToGlobal(perTaskCounts, GlobCounts,
                              nBuckets, nActiveLocales, activeLocIdx,
                              nTasksPerLocale);

    // wait for all counts to be stored in GlobCounts
    /*if useAllLocalesBarrier { error about too many callers
      AllLocalesBarriers.allLocalesBarrier.barrier();
    } else*/ {
      smallerBarrier.barrier();
    }

    // Step 2: Scan

    if activeLocIdx == 0 {
      // only locale 0 launches the scan, but this will involve all locales
      // note: could implement a custom scan that only uses activeLocales;
      // current strategy is to assume it's either all locales (more or less)
      // or a small number of them.
      GlobEnds = + scan GlobCounts;
    }

    // wait for the scan to be complete
    /*if useAllLocalesBarrier {
      AllLocalesBarriers.allLocalesBarrier.barrier();
    } else*/ {
      smallerBarrier.barrier();
    }

    if Output.type != nothing {
      // Step 3: Distribute
      const LOutputShift = OutputShift;

      var perTaskNext: [0..<nTasksPerLocale] [0..<nBuckets] int;
      // fill in perTaskNext from GlobEnds
      getTaskCountsFromGlobal(perTaskNext, GlobEnds,
                              nBuckets, nActiveLocales, activeLocIdx,
                              nTasksPerLocale);

      // distribute, updating the perTaskNext for each task
      coforall (chunk, taskIdInLoc)
      in zip(RangeChunk.chunks(locRegion, nTasksPerLocale), 0..) {
        ref nextOffsets = perTaskNext[taskIdInLoc];

        // first adjust nextOffsets for OutputShift
        if LOutputShift.type != nothing {
          foreach bucketIdx in 0..<nBuckets {
            var shift = 0;
            if isArrayType(LOutputShift.type) {
              shift = LOutputShift[bucketIdx];
            } else if isIntType(LOutputShift.type) {
              shift = LOutputShift;
            }
            nextOffsets[bucketIdx] += shift;
          }
        }

        var agg = new DstAggregator(Input.eltType);

        for (elt,bkt) in lsplit.classify(Input, chunk.low, chunk.high,
                                         comparator) {
          if filterBucket.type == nothing || filterBucket(bkt) {
            // Store it in the right bin
            ref next = nextOffsets[bkt];
            agg.copy(Output[next], elt);
            next += 1;
          }
        }
      }
    }
  }

  // Compute the total counts to return
  forall (end, bucketIdx) in zip(Ends, Ends.domain)
  with (var agg = new SrcAggregator(int)) {
    // read the last entry for each bin
    var countIdx =
      getGlobalCountIdx(bucketIdx, nActiveLocales-1, nActiveLocales,
                        nTasksPerLocale-1, nTasksPerLocale);

    agg.copy(end, GlobEnds[countIdx]);
  }

  forall (r, bucketIdx) in zip(Ret, Ret.domain) {
    var end = Ends[bucketIdx];
    var prevEnd = 0;
    if bucketIdx > 0 {
      prevEnd = Ends[bucketIdx-1];
    }
    var count = end - prevEnd;
    var start = end - count;

    var shift = 0;
    if isArrayType(OutputShift.type) {
      shift = OutputShift[bucketIdx];
    } else if isIntType(OutputShift.type) {
      shift = OutputShift;
    }

    r.start = start + shift;
    r.count = count;
  }
}


///// partitioning sort


record partitioningSorter {
  type eltType;
  type splitterType;
  param radixBits: int; // 0 -> sample sort, e.g. 8 indicates radix 2**8
  const logBuckets: int; // when sample sorting, how many buckets?
  const nTasksPerLocale: int;
  const endbit: int;
  const baseCaseLimit: int;
  const noBaseCase: bool;
  const noSerialPartition: bool;
  const markAllEquals: bool;
  const useExistingBuckets: bool;
}

proc type partitioningSorter.computeBaseCaseLimit(logBuckets: int,
                                                  noBaseCase: bool) {
  if noBaseCase {
    return 1;
  }

  var limit = (PARTITION_SORT_BASE_CASE_MULTIPLIER * (1 << logBuckets)):int;
  if maybeDistributed() {
    // distributed sorting has even more overhead
    limit *= 10;
  }
  return max(limit, 2);
}

proc partitioningSorter.init(type eltType, type splitterType,
                             param radixBits: int,
                             logBuckets: int,
                             nTasksPerLocale: int,
                             endbit: int,
                             markAllEquals=false,
                             useExistingBuckets=false,
                             noBaseCase=false) {
  this.eltType = eltType;
  this.splitterType = splitterType;
  this.radixBits = radixBits;
  this.logBuckets = logBuckets;
  this.nTasksPerLocale = nTasksPerLocale;
  this.endbit = endbit;
  this.baseCaseLimit =
    partitioningSorter.computeBaseCaseLimit(logBuckets, noBaseCase);
  this.noBaseCase = noBaseCase;
  this.noSerialPartition = noBaseCase;
  this.markAllEquals = markAllEquals;
  this.useExistingBuckets = useExistingBuckets;
  init this;

  if (radixBits == 0) != isSampleSplitters(splitterType) {
    compilerError("bad call to partitioningSorter.init -- radix bits wrong");
  }
}

proc createSampleSplitters(const ref ADom,
                           const ref A, /* array or array-like */
                           region: range,
                           comparator,
                           activeLocs: [] locale,
                           nTasksPerLocale: int,
                           logBuckets: int)
 : splitters(A.eltType) {

  //writeln("creating splitters for ", region);

  const nTasks = activeLocs.size * nTasksPerLocale;
  const requestBuckets = 1 << logBuckets;
  const perTask = divCeil((SAMPLE_RATIO*requestBuckets):int, nTasks);
  const nToSample = perTask*nTasks;
  const firstElt = A[region.low];
  var SortSamplesSpace:[0..<nToSample] A.eltType = firstElt;
  const SortSamplesSpaceDomRange = SortSamplesSpace.domain.dim(0);

  // read some random elements from each locale
  // each should set SortSampleSpace[perTask*taskId..#perTask]
  //forall (taskId, chk) in divideIntoTasks(Dom, nTasksPerLocale)
  forall (activeLocIdx, taskIdInLoc, chunk)
  in divideIntoTasks(ADom, region, nTasksPerLocale, activeLocs)
  with (var agg = new DstAggregator(A.eltType)) {
    const taskId = activeLocIdx*nTasksPerLocale + taskIdInLoc;
    const dstFullRange = perTask*taskId..#perTask;
    const dstRange = SortSamplesSpaceDomRange[dstFullRange];
    const dstRangeDom = {dstRange};

    // note: this will give different
    // results with the same seed if the number of tasks
    // or the number of locales differs.
    var randNums;
    if SEED == 0 {
      randNums = new Random.randomStream(int);
    } else {
      randNums = new Random.randomStream(int, seed=SEED*taskId);
    }

    const low = chunk.low;
    const high = chunk.high;
    for (dstIdx, randIdx) in zip(dstRangeDom,
                                 randNums.next(dstRangeDom, low, high)) {
      //writeln("SortSamplesSpace[", dstIdx, "] = A[", randIdx, "]");
      // store the value at randIdx (which should be local) to dstIdx
      const val = A[randIdx];
      agg.copy(SortSamplesSpace[dstIdx], val); // TODO: array header comms
      //SortSamplesSpace[dstIdx] = A[randIdx];
    }
  }

  // sort them using any kind of sort
  /*writeln("before sorting");
  for i in SortSamplesSpace.domain {
    writeln("SortSamplesSpace[", i, "] = ", SortSamplesSpace[i]);
  }*/


  // TODO: this seems to cause it not to compile
  /*
  if SortSamplesSpace.size <= baseCaseLimit {
    sort(SortSamplesSpace, comparator=comparator);
  } else {
    var Scratch: [SortSamplesSpace.domain] A.eltType;
    var BucketBoundaries: [SortSamplesSpace.domain] uint(8);
    parallelPartitioningSort(SortSamplesSpace, Scratch, BucketBoundaries,
                             0..<nToSample, radixSort=false,
                             comparator, logBuckets, nTasksPerLocale,
                             startbit=0, endbit=max(int));
  }*/
  // TODO: using default unstable sort seems to fail due to out of stack space
  // with all-zeros input.
  sort(SortSamplesSpace, comparator=comparator, 0..<nToSample, stable=true);

  /*writeln("after sorting");
  for i in SortSamplesSpace.domain {
    writeln("SortSamplesSpace[", i, "] = ", SortSamplesSpace[i]);
  }*/

  if PARTITIONING_EXTRA_CHECKS {
    //writeln("sorted samples to ", SortSamplesSpace);
    assert(isSorted(SortSamplesSpace, comparator));
  }

  // now form splitters
  //writeln("forming splitters with requestBuckets ", requestBuckets);
  const split = new splitters(SortSamplesSpace, requestBuckets, comparator,
                              howSorted=sortLevel.fully);

  //writeln("splitters are ", split);

  return split;
}

proc createRadixSplitters(const ref A: [],
                          region: range,
                          comparator,
                          activeLocs: [] locale,
                          param radixBits: int,
                          in startbit: int,
                          in endbit: int,
                          nTasksPerLocale: int)
 : radixSplitters(radixBits) {

  if startbit != 0 {
    return new radixSplitters(radixBits=radixBits,
                              startbit=startbit,
                              endbit=endbit);
  }

  var nBitsInCommon = 0;
  var part = 0;
  while true {
    // compute the minimum and maximum key part
    var minElt = myGetKeyPart(A[region.low], comparator, part)(1);
    var maxElt = myGetKeyPart(A[region.low], comparator, part)(1);
    var nEnd = 0;
    const p = part;
    forall (activeLocIdx, taskIdInLoc, chunk)
    in divideIntoTasks(A.domain, region, nTasksPerLocale, activeLocs)
    with (min reduce minElt, max reduce maxElt, + reduce nEnd) {
      for i in chunk {
        const (section, elt) = myGetKeyPart(A[i], comparator, p);
        if section == keyPartStatus.returned {
          minElt reduce= elt;
          maxElt reduce= elt;
        } else {
          nEnd += 1;
        }
      }
    }
    if nEnd > 0 {
      // stop because we reached an end element, make no change to startbit
      break;
    } else if minElt == maxElt {
      // continue the while loop, but advance to the next part
      // and adjust nBitsInCommon
      nBitsInCommon += numBits(minElt.type);
      part += 1;
    } else {
      // stop the loop because we reached elements that differed
      // and adjust nBitsInCommon according to the min and max element
      nBitsInCommon += BitOps.clz(minElt ^ maxElt):int;
      break;
    }
  }

  // set startbit to nBitsInCommon rounded down to a radixBits group
  var nRadixesInCommon = nBitsInCommon / radixBits;
  startbit = nRadixesInCommon * radixBits;

  return new radixSplitters(radixBits=radixBits,
                            startbit=startbit,
                            endbit=endbit);
}

// "not boundary" is indicated by any number i with 0 <= i < 250
param boundaryTypeMaxNotBoundary: uint(8) = 249;
param boundaryTypeUnsortedBucketInScratch: uint(8) = 250;
param boundaryTypeUnsortedBucketInA: uint(8) = 251;
param boundaryTypeEqualBucketInScratch: uint(8) = 252;
param boundaryTypeEqualBucketInA: uint(8) = 253;
param boundaryTypeBaseCaseSortedBucketInScratch: uint(8) = 254;
param boundaryTypeBaseCaseSortedBucketInA: uint(8) = 255;

inline proc isBucketBoundary(boundaryType: uint(8)) {
  return boundaryTypeMaxNotBoundary < boundaryType;
}
inline proc isInA(boundaryType: uint(8)) {
  return (boundaryType & 1) > 0;
}
inline proc isBaseCaseBoundary(boundaryType: uint(8)) {
  return boundaryTypeBaseCaseSortedBucketInScratch <= boundaryType &&
         boundaryType <= boundaryTypeBaseCaseSortedBucketInA;
}
inline proc isEqualBucketBoundary(boundaryType: uint(8)) {
  return boundaryTypeEqualBucketInScratch <= boundaryType &&
         boundaryType <= boundaryTypeEqualBucketInA;
}
inline proc isUnsortedBucketBoundary(boundaryType: uint(8)) {
  return boundaryTypeUnsortedBucketInScratch <= boundaryType &&
         boundaryType <= boundaryTypeUnsortedBucketInA;
}

private proc partitionSortBaseCase(ref A: [], region: range, comparator) {
  if region.size == 0 {
    return; // nothing to do
  }

  if region.size == 1 {
    return; // nothing to do
  }

  if region.size == 2 {
    const i = region.low;
    const j = region.low + 1;
    var cmp = mycompare(A[i], A[j], comparator);
    if cmp > 0 {
      A[i] <=> A[j];
    }
    return;
  }

  if A.domain.localSubdomain().dim(0).contains(region) {
    // sort it with a stable sort
    local {
      // TODO: use an unstable sort if requested
      sort(A.localSlice(region), comparator, region, stable=true);
    }

  } else {
    // copy it locally and sort it with a stable sort
    var LocA:[region] A.eltType;
    bulkCopy(LocA, region, A, region);
    local {
      // TODO: use an unstable sort if requested
      sort(LocA, comparator, region, stable=true);
    }
    // copy the sorted data back
    bulkCopy(A, region, LocA, region);
  }
}

proc bitsInCommon(a, b, comparator) {
  if canResolveMethod(comparator, "keyPart", a, 0) {
    return bitsInCommonForKeyPart(a, b, comparator);
  } else if canResolveMethod(comparator, "key", a) {
    return bitsInCommonForKeyPart(comparator.key(a), comparator.key(b),
                                  new defaultComparator());
  } else {
    compilerError("Bad comparator for radix sort ", comparator.type:string,
                  " with eltType ", a.type:string);
  }

}

proc bitsInCommonForKeyPart(a, b, comparator) {
  var curPart = 0;
  var bitsInCommon = 0;
  while true {
    var (aSection, aPart) = comparator.keyPart(a, curPart);
    var (bSection, bPart) = comparator.keyPart(b, curPart);
    if aSection != keyPartStatus.returned ||
       bSection != keyPartStatus.returned {
      break;
    }
    if aPart == bPart {
      bitsInCommon += numBits(aPart.type);
    } else {
      // compute the common number of bits
      bitsInCommon += BitOps.clz(aPart ^ bPart):int;
      break;
    }

    curPart += 1;
  }

  return bitsInCommon;
}

// mark the bucket boundaries
proc markBoundaries(ref BucketBoundaries: [] uint(8),
                    Split, // splitters / radixSplitters
                    Bkts: [] bktCount,
                    const nowInA: bool,
                    const nextbit: int) {
  const equalType;
  const sortedType;
  const unsortedType;

  if nowInA {
    equalType = boundaryTypeEqualBucketInA;
    sortedType = boundaryTypeBaseCaseSortedBucketInA;
    unsortedType = boundaryTypeUnsortedBucketInA;
  } else {
    equalType = boundaryTypeEqualBucketInScratch;
    sortedType = boundaryTypeBaseCaseSortedBucketInScratch;
    unsortedType = boundaryTypeUnsortedBucketInScratch;
  }

  const smm = Split.summary();

  forall (bkt,bucketIdx) in zip(Bkts, Bkts.domain)
  with (var agg = new DstAggregator(uint(8)), in smm) {
    if bkt.count > 0 {
      var t: uint(8);
      if bkt.count == 1 {
        t = sortedType;
      } else if smm.bucketHasEqualityBound(bucketIdx) {
        t = equalType;
      } else {
        t = unsortedType;
      }
      setBucketBoundary(BucketBoundaries, t,
                        bkt.start, bkt.count, nextbit, agg);
    }
  }
}

/* A parallel partitioning sort step. Sorts 'region' a bit more in A/Scratch.

   When this returns, A/Scratch will be more sorted, and BucketBoundaries
   will be updated to indicate how A/Scratch is more sorted.

   Scratch is temporary space of similar size to the sorted region.

   BucketBoundaries[i] marks locations where A[i-1] differs from A[i]
   (that is a bucket start), tracks the start bit, and also tracks
   which array (A or Scratch) contains the bucket data.

   A, Scratch, and BucketBoundaries can be distributed. This code
   assumes that they are distributed in the same manner.
 */
proc partitioningSorter.sortStep(ref A: [],
                                 ref Scratch: [] A.eltType,
                                 ref BucketBoundaries: [] uint(8),
                                 const region: range,
                                 const comparator,
                                 const startbit: int,
                                 const bktType: uint(8),
                                 const sequential: bool,
                                 const ifAllLocal: bool) : void {
  if region.size == 0 {
    return;
  }

  //writeln("sortStep ", region, " bktType ", bktType);

  if PARTITIONING_EXTRA_CHECKS {
    assert(A.domain.dim(0).contains(region));
    assert(Scratch.domain.dim(0).contains(region));
    assert(BucketBoundaries.domain.dim(0).contains(region));

    // we should only call sortStep on unsorted buckets or ones not in A
    assert(isUnsortedBucketBoundary(BucketBoundaries[region.low]) ||
           !isInA(BucketBoundaries[region.low]));
    // we shouldn't call sortStep on something spanning bucket boundaries
    for i in region.low+1..region.high {
      assert(!isBucketBoundary(BucketBoundaries[i]));
    }

    assert(BucketBoundaries[region.low] == bktType);
  }

  /*
  for i in region {
    writeln("starting partitioningSortStep A[", i, "] = ", A[i], " BucketBoundaries[", i, "] = ", BucketBoundaries[i]);
  }*/

  const inputInA = isInA(bktType);

  if !isUnsortedBucketBoundary(bktType) {
    // copy it to A if it is not already there
    if !inputInA {
      local ifAllLocal {
        bulkCopy(A, region, Scratch, region);
        // update the bucket boundary
        if isBaseCaseBoundary(bktType) {
          BucketBoundaries[region.low] = boundaryTypeBaseCaseSortedBucketInA;
        } else if isEqualBucketBoundary(bktType) {
          BucketBoundaries[region.low] = boundaryTypeEqualBucketInA;
        } else {
          assert(false); // should not be possible
        }
      }
    } else {
      assert(false); // should not be called this way
    }
    return;
  }

  if region.size <= baseCaseLimit {
    // handle a small region with the base case sort
    local ifAllLocal {
      // copy it to A if it is not already there
      if !inputInA {
        bulkCopy(A, region, Scratch, region);
      }
      var agg = new DstAggregator(uint(8));
      baseCase(A, BucketBoundaries, region, comparator, agg);
    }
    return;
  }

  local ifAllLocal {
    // What are the input and output for the partition?
    /*const*/ ref Input = if inputInA then A else Scratch;
    ref Output = if inputInA then Scratch else A;

    const activeLocs = computeActiveLocales(A.domain, region);

    // create the splitters
    const Split;
    const nextbit;
    if radixBits == 0 {
      Split = createSampleSplitters(Input.domain, Input, region,
                                    comparator, activeLocs,
                                    nTasksPerLocale, logBuckets);
      nextbit = 0;
    } else {
      Split = createRadixSplitters(Input, region, comparator, activeLocs,
                                   radixBits=radixBits,
                                   startbit=startbit,
                                   endbit=endbit,
                                   nTasksPerLocale=nTasksPerLocale);
      nextbit = startbit + radixBits;
    }

    // partition from Input to Output
    const useTasksPerLocale = if sequential then 1 else nTasksPerLocale;
    const Bkts = partition(Input.domain, region, Input, region.low, Output,
                           Split, comparator,
                           useTasksPerLocale, activeLocs,
                           noSerialPartition=noSerialPartition);

    // mark the bucket boundaries for the data now in Output
    markBoundaries(BucketBoundaries, Split, Bkts, nowInA=!inputInA, nextbit);
  }

  /*for i in region {
    writeln("after sortStep A[", i, "] = ", A[i], " Scratch[", i, "] = ", Scratch[i], " BucketBoundaries[", i, "] = ", BucketBoundaries[i]);
  }*/
}

type encodedTupleType = 10*uint(8); // because 64 < 10*7
param bktHeaderSize = 22; // 1 type + 1 saturated + 10 size + 10 startbit

// encode x to a tuple of uint(8) using only the bottom 7 bits of each
proc encodeToTuple(x: uint) {
  var ret:encodedTupleType;
  for param i in 0..<ret.size {
    ret[i] = ((x >> (7*i)) & 0x7f):uint(8);
  }
  if PARTITIONING_EXTRA_CHECKS {
    assert(decodeFromTuple(ret) == x);
  }
  return ret;
}
proc decodeFromTuple(tup: encodedTupleType) {
  var ret: uint = 0;
  for param i in 0..<tup.size {
    ret |= tup[i]:uint << 7*i;
  }
  return ret;
}

proc setBucketBoundary(ref BucketBoundaries: [] uint(8),
                       boundaryType: uint(8),
                       bktStart: int,
                       bktSize: int,
                       bktStartBit: int,
                       ref agg: DstAggregator(uint(8)))
{
  // set the first byte
  agg.copy(BucketBoundaries[bktStart], boundaryType);
  // if the bucket is large enough, set the subsequent bytes
  if bktSize >= 2 {
    var i = 1;
    const saturatedSize = min(bktSize, boundaryTypeMaxNotBoundary): uint(8);
    agg.copy(BucketBoundaries[bktStart+i], saturatedSize);
    i += 1;

    if bktSize >= bktHeaderSize {
      // store the encoded bucket size
      const sTup = encodeToTuple(bktSize);
      for j in 0..<sTup.size {
        agg.copy(BucketBoundaries[bktStart+i], sTup[j]);
        i += 1;
      }
      // store the encoded start bit
      const bTup = encodeToTuple(bktStartBit);
      for j in 0..<bTup.size {
        agg.copy(BucketBoundaries[bktStart+i], bTup[j]);
        i += 1;
      }
      if PARTITIONING_EXTRA_CHECKS {
        assert(i <= bktSize);
      }
    }
  }

  if PARTITIONING_EXTRA_CHECKS && CHPL_COMM=="none" {
    // this check does not work correctly when working with
    // multiple locales and shared aggregators,
    // so it is disabled except for numLocales==1 when the aggregators
    // should not be used.
    agg.flush();
    /*writeln("checking setBucketBoundary bktStart ", bktStart,
            " bktSize ", bktSize, " bktStartBit ", bktStartBit);
    for i in bktStart..#bktSize {
      writeln("BucketBoundaries[", i, "] = ", BucketBoundaries[i]);
    }*/
    var gotBoundaryType: uint(8);
    var gotBktSize: int;
    var gotBktStartBit: int;
    readBucketBoundary(BucketBoundaries, bktStart..#bktSize,
                       bktStart, gotBoundaryType, gotBktSize, gotBktStartBit);
    assert(gotBoundaryType == boundaryType);
    assert(gotBktSize == bktSize);
    if bktSize >= bktHeaderSize {
      assert(gotBktStartBit == bktStartBit);
    } else {
      assert(gotBktStartBit == 0);
    }
  }
}

proc readBucketBoundary(ref BucketBoundaries: [] uint(8),
                        allRegion:range,
                        bktStart: int,
                        out boundaryType: uint(8),
                        out bktSize: int,
                        out bktStartBit: int) : void {
  boundaryType = BucketBoundaries[bktStart];
  const endAll = allRegion.high+1;
  var bktSizeRead = false;
  if bktStart + 1 < endAll {
    var i = 1;
    const saturatedSize = BucketBoundaries[bktStart+i];
    i += 1;
    if bktHeaderSize <= saturatedSize &&
       saturatedSize <= boundaryTypeMaxNotBoundary {
      var sTup: encodedTupleType;
      for j in 0..<sTup.size {
        sTup[j] = BucketBoundaries[bktStart+i];
        i += 1;
      }
      bktSize = decodeFromTuple(sTup):int;
      bktSizeRead = true;

      var bTup: encodedTupleType;
      for j in 0..<bTup.size {
        bTup[j] = BucketBoundaries[bktStart+i];
        i += 1;
      }
      bktStartBit = decodeFromTuple(bTup):int;
    } else if saturatedSize <= 127 {
      bktSize = saturatedSize;
      bktSizeRead = true;
    }
  }

  var computedBucketSize = 0;
  if !bktSizeRead || PARTITIONING_EXTRA_CHECKS {
    // compute the bucket size by scanning forward
    var next = bktStart + 1;
    while next < endAll && !isBucketBoundary(BucketBoundaries[next]) {
      next += 1;
    }
    computedBucketSize = next - bktStart;
  }

  if !bktSizeRead {
    bktSize = computedBucketSize;
    bktStartBit = 0;
  } else if PARTITIONING_EXTRA_CHECKS {
    // check that the read bucket size matches the computed bucket size
    if bktSize != computedBucketSize {
      writeln("bucket boundary does not match at ", bktStart,
              " read ", bktSize, " but computed ", computedBucketSize);
    }
    assert(bktSize == computedBucketSize);
  }
}

proc partitioningSorter.baseCase(ref A: [],
                                 ref BucketBoundaries: [] uint(8),
                                 region: range,
                                 comparator,
                                 ref agg: DstAggregator(uint(8))) {
  partitionSortBaseCase(A, region, comparator);

  if region.size == 1 || !markAllEquals {
    setBucketBoundary(BucketBoundaries, boundaryTypeBaseCaseSortedBucketInA,
                      region.low, region.size, bktStartBit=0, agg);
  } else {
    // compare the elements just sorted to distinguish equal elements
    var cur = region.low;
    var end = region.high+1;
    while cur < end {
      // 'cur' is different from its previous. what is the next
      // element that differs?
      var next = cur + 1;
      while next < end && 0 == mycompare(A[cur], A[next], comparator) {
        next += 1;
      }
      // now 'next' is either one that differs from 'cur' or it is 'end'
      if next == cur + 1 {
        // it is a singleton bucket
        setBucketBoundary(BucketBoundaries,
                          boundaryTypeBaseCaseSortedBucketInA,
                          cur, 1, bktStartBit=0, agg);
      } else {
        // there are some equal elements
        setBucketBoundary(BucketBoundaries,
                          boundaryTypeEqualBucketInA,
                          cur, next - cur, bktStartBit=0, agg);
      }

      cur = next;
    }
  }
}



record spanHelper {
  var region: range;
  var bktType: uint(8);
  var startbit: int;
}

proc nextBucket(ref BucketBoundaries: [] uint(8),
                taskRegion: range,
                allRegion:range,
                in cur: int,
                out bktType: uint(8)) {
  const end = taskRegion.high+1;

  // move 'cur' forward until it finds a bucket boundary
  while cur < end && !isBucketBoundary(BucketBoundaries[cur]) {
    cur += 1;
  }
  if cur >= end {
    // return since it's in a different task's region
    return end..end-1;
  }

  var bktSize: int;
  var bktStartBit: int;
  readBucketBoundary(BucketBoundaries, allRegion, cur,
                     /* out */ bktType, bktSize, bktStartBit);

  return cur..#bktSize;
}

// This function computes the start of the next bucket containing
// unsorted data that a task is responsible for.
//   * 'taskRegion' is the region a task should handle (from divideIntoTasks)
//   * 'allRegion' is the region being processed across all tasks
//   * 'cur' is the starting position
// returns a range indicating the bucket.
//
// Each task is responsible for buckets that start in its taskRegion.
proc nextUnsortedBucket(ref BucketBoundaries: [] uint(8),
                        taskRegion: range,
                        allRegion:range,
                        in cur: int,
                        out bktType: uint(8),
                        out bktStartBit: int) {
  const end = taskRegion.high+1;

  // help to yield periodically
  var yh: yieldHelper;

  // move 'cur' forward until it finds a bucket boundary
  while cur < end && !isBucketBoundary(BucketBoundaries[cur]) {
    cur += 1;
    yh.maybeYield();
  }
  if cur >= end {
    // return since it's in a different task's region
    return end..end-1;
  }

  // read forward in buckets until we find an unsorted bucket
  while cur < end {
    var foundType: uint(8);
    var foundSize: int;
    var foundStartBit: int;
    readBucketBoundary(BucketBoundaries,
                       allRegion,
                       cur,
                       /*out*/ foundType, foundSize, foundStartBit);
    if isUnsortedBucketBoundary(foundType) || !isInA(foundType) {
      bktType = foundType;
      bktStartBit = foundStartBit;
      if PARTITIONING_EXTRA_CHECKS {
        assert(taskRegion.contains(cur));
        assert(allRegion.contains(cur));
        assert(allRegion.contains(cur+foundSize-1));
      }
      return cur..#foundSize;
    }
    cur += foundSize;
    yh.maybeYield();
  }

  // return empty since we found no unsorted buckets starting in the task region
  return end..end-1;
}

// Finds a bucket region for which the bucket spans multiple tasks
// Each task is responsible for buckets that start in its region
// Returns the bucket region, and other details with 'out' arguments.
// Returns an empty range if there is no region this task is responsible for
proc partitioningSorter.findSpanningBucket(ref BucketBoundaries: [] uint(8),
                                           taskRegion: range, allRegion:range,
                                           out bktType: uint(8),
                                           out bktStartBit: int) {
  if taskRegion.size > 0 &&
     allRegion.contains(taskRegion.high+1) &&
     !isBucketBoundary(BucketBoundaries[taskRegion.high+1]) {
    var cur = taskRegion.high;
    while taskRegion.contains(cur) && !isBucketBoundary(BucketBoundaries[cur]) {
      cur -= 1;
    }
    if taskRegion.contains(cur) &&
       (isUnsortedBucketBoundary(BucketBoundaries[cur]) ||
        !isInA(BucketBoundaries[cur])) {

      const bkt = nextUnsortedBucket(BucketBoundaries, taskRegion, allRegion,
                                     cur,
                                     /* out */ bktType,
                                     /* out */ bktStartBit);

      if PARTITIONING_EXTRA_CHECKS {
        if isUnsortedBucketBoundary(BucketBoundaries[cur]) {
          assert(!isBucketBoundary(BucketBoundaries[cur+1]));
          assert(isUnsortedBucketBoundary(BucketBoundaries[bkt.low]));
        }
        assert(taskRegion.contains(bkt.low));
      }

      return bkt;
    }
  }

  // return an empty range
  return taskRegion.high+1..taskRegion.high;
}

proc partitioningSortInitialPartition(ref A: [],
                                      ref Scratch: [] A.eltType,
                                      ref BucketBoundaries: [] uint(8),
                                      const activeLocs: [] locale,
                                      region: range,
                                      comparator,
                                      param radixBits,
                                      logBuckets: int,
                                      nTasksPerLocale: int,
                                      endbit: int,
                                      markAllEquals:bool,
                                      noBaseCase:bool) : void {
  type splitterType = if radixBits != 0
                      then radixSplitters(radixBits)
                      else splitters(A.eltType);

  const s = new partitioningSorter(A.eltType, splitterType,
                                   radixBits=radixBits,
                                   logBuckets=logBuckets,
                                   nTasksPerLocale=nTasksPerLocale,
                                   endbit=endbit,
                                   markAllEquals=markAllEquals,
                                   useExistingBuckets=false,
                                   noBaseCase=noBaseCase);

  const Split;
  const nextbit;
  if radixBits == 0 {
    Split = createSampleSplitters(A.domain, A, region, comparator, activeLocs,
                                  s.nTasksPerLocale, s.logBuckets);
    nextbit = 0;
  } else {
    Split = createRadixSplitters(A, region, comparator, activeLocs,
                                 radixBits=s.radixBits,
                                 startbit=0, endbit=s.endbit,
                                 nTasksPerLocale=nTasksPerLocale);
    nextbit = s.radixBits;
  }

  // allocate distributed counts to use for the initial partition
  const nBuckets = Split.numBuckets;
  const nActiveLocales = activeLocs.size;
  const countsPerBucket = nActiveLocales*nTasksPerLocale;
  const countsSize = nBuckets*countsPerBucket;

  const GlobCountsDom = blockDist.createDomain(0..<countsSize);
  var GlobCounts: [GlobCountsDom] int;
  const CountsDom = blockDist.createDomain(0..<nBuckets);
  var Ends:[CountsDom] int;
  var Bkts:[CountsDom] bktCount;

  // partition from A into Scratch
  var noFilterBucket = none;
  parStablePartition(A.domain, region, A, region.low, Scratch,
                     Split, comparator, noFilterBucket,
                     s.nTasksPerLocale, activeLocs,
                     GlobCounts, Ends, Bkts);

  markBoundaries(BucketBoundaries, Split, Bkts, nowInA=false, nextbit);
}

/* A parallel partitioning sort.

   When this returns, A will be sorted, and BucketBoundaries
   will be updated to indicate how A is more sorted.

   Each call to parallelPartitioningSort will write to 'split' and 'rsplit',
   so make sure each gets its own if running in a parallel context.

   Uses temporary space 'Scratch' of similar size
   to the sorted region, as well as BucketBoundaries.

   BucketBoundaries[i] indicates the relationship between A[i] and A[i-1]:
     * unsorted: ordering of A[i] and A[i-1] is not known
     * ordered: A[i] > A[i-1] (i.e. they are in sorted order)
     * equal: A[i] == A[i-1] (i.e. they are in sorted order)

   split is space for some splitters
   rsplit is space for those splitters replicated

   Then input is in A and the output will be stored in A.

   A, Scratch, and BucketBoundaries can be distributed
   (and should be distributed the same).
 */
proc partitioningSorter.psort(ref A: [],
                              ref Scratch: [] A.eltType,
                              ref BucketBoundaries: [] uint(8),
                              region: range,
                              comparator) : void {
  if PARTITIONING_EXTRA_CHECKS {
    assert(A.domain.dim(0).contains(region));
    assert(Scratch.domain.dim(0).contains(region));
    assert(BucketBoundaries.domain.dim(0).contains(region));

    if !useExistingBuckets {
      forall elt in BucketBoundaries {
        assert(elt == 0);
      }
    }
  }

  /*for i in region {
    writeln("starting psort A[", i, "] = ", A[i], " BucketBoundaries[", i, "] = ", BucketBoundaries[i]);
  }*/

  if region.size <= baseCaseLimit && !useExistingBuckets {
    var agg = new DstAggregator(uint(8));
    baseCase(A, BucketBoundaries, region, comparator, agg);
    return;
  }

  const activeLocs = computeActiveLocales(A.domain, region);

  if !useExistingBuckets {
    var firstPartitionTime = startTime(PARTITIONING_SORT_TIMING);

    // Get started by partitioning from A into Scratch
    // Ideally, this creates a number of buckets >> num tasks

    const totalTasks = activeLocs.size * nTasksPerLocale;
    param isRadixSort = if radixBits > 0 then 1 else 0;
    if noBaseCase {
      partitioningSortInitialPartition(A, Scratch, BucketBoundaries,
                                       activeLocs,
                                       region, comparator,
                                       radixBits=radixBits,
                                       logBuckets=radixBits,
                                       nTasksPerLocale=nTasksPerLocale,
                                       endbit=endbit,
                                       markAllEquals=markAllEquals,
                                       noBaseCase=noBaseCase);
    } else if totalTasks < 1000 && logBuckets < 16 {
      partitioningSortInitialPartition(A, Scratch, BucketBoundaries,
                                       activeLocs,
                                       region, comparator,
                                       radixBits=8*isRadixSort,
                                       logBuckets=8,
                                       nTasksPerLocale=nTasksPerLocale,
                                       endbit=endbit,
                                       markAllEquals=markAllEquals,
                                       noBaseCase=noBaseCase);
    } else {
      partitioningSortInitialPartition(A, Scratch, BucketBoundaries,
                                       activeLocs,
                                       region, comparator,
                                       radixBits=16*isRadixSort,
                                       logBuckets=16,
                                       nTasksPerLocale=nTasksPerLocale,
                                       endbit=endbit,
                                       markAllEquals=markAllEquals,
                                       noBaseCase=noBaseCase);
    }

    reportTime(firstPartitionTime, "first step time", region.size);
  }

  /*for i in region {
    writeln("after initial Scratch[", i, "] = ", Scratch[i], " BucketBoundaries[", i, "] = ", BucketBoundaries[i]);
  }*/

  var spanTime = startTime(PARTITIONING_SORT_TIMING);

  const s = this;

  // sort any buckets that spans multiple locales / multiple tasks
  //  * each task is responsible for buckets that start in its region
  //    * so, generally speaking, the last bucket in this region
  //      is getting smaller (and will stop being in the region)
  //  * there is a tricky case though, when a bucket spans multiple
  //    tasks/locales. In that case, the first locale might work on
  //    it, and in the process create work for the others!
  //    Here we avoid that by operating in phases, where the spanning
  //    bucket for each task is computed first, and then it is sorted.
  if activeLocs.size > 1 {
    while true {
      const SpansDom = blockDist.createDomain(0..<activeLocs.size,
                                              targetLocales=activeLocs);
      var Spans:[SpansDom] spanHelper;
      var nToSort = 0;

      forall (activeLocIdx, locRegion)
      in divideByLocales(A.domain, region, activeLocs)
      with (in s, + reduce nToSort) {
        var bktType: uint(8);
        var bktStartBit: int;
        var bkt = s.findSpanningBucket(BucketBoundaries, locRegion, region,
                                       /* out */ bktType, bktStartBit);
        Spans[activeLocIdx] = new spanHelper(bkt, bktType, bktStartBit);
        if bkt.size > 0 {
          nToSort += 1;
        }
      }

      if nToSort == 0 then break;

      forall (activeLocIdx, locRegion)
      in divideByLocales(A.domain, region, activeLocs)
      with (in s) {
        var span = Spans[activeLocIdx];
        if span.region.size > 0 {
          if PARTITIONING_EXTRA_CHECKS {
            assert(locRegion.contains(span.region.low));
          }

          // sort the spanning bucket a bit more
          s.sortStep(A, Scratch, BucketBoundaries, span.region, comparator,
                     startbit=span.startbit, bktType=span.bktType,
                     sequential=false, ifAllLocal=false);
        }
      }
    }
  }

  // sort buckets spanning multiple tasks within each locale
  forall (activeLocIdx, locRegion)
  in divideByLocales(A.domain, region, activeLocs)
  with (in s) {
    ref localA = A.localSlice(locRegion);
    ref localScratch = Scratch.localSlice(locRegion);
    ref localBuckets = BucketBoundaries.localSlice(locRegion);

    while true {
      var Spans:[0..<nTasksPerLocale] spanHelper;
      var nToSort = 0;

      forall (activeLocIdx, taskIdInLoc, taskRegion)
      in divideIntoTasks(A.domain, locRegion, nTasksPerLocale)
      with (+ reduce nToSort) {
        var bktType: uint(8);
        var bktStartBit: int;
        var bkt = s.findSpanningBucket(BucketBoundaries,
                                       taskRegion, region,
                                       /* out */ bktType, bktStartBit);
        Spans[taskIdInLoc] = new spanHelper(bkt, bktType, bktStartBit);
        if bkt.size > 0 {
          nToSort += 1;
        }
      }

      if nToSort == 0 then break;

      forall (activeLocIdx, taskIdInLoc, taskRegion)
      in divideIntoTasks(A.domain, locRegion, nTasksPerLocale) {
        var span = Spans[taskIdInLoc];
        if span.region.size > 0 {
          if PARTITIONING_EXTRA_CHECKS {
            assert(taskRegion.contains(span.region.low));
          }
          s.sortStep(localA, localScratch, localBuckets,
                     span.region, comparator,
                     startbit=span.startbit, bktType=span.bktType,
                     sequential=false, ifAllLocal=true);
        }
      }
    }
  }

  reportTime(spanTime, "span time", 0);

  /*for i in region {
    writeln("after spans A[", i, "] = ", A[i], " Scratch[", i, "] = ", Scratch[i], " BucketBoundaries[", i, "] = ", BucketBoundaries[i]);
  }*/

  // sort buckets within each task's region

  var innerSortTime = startTime(PARTITIONING_SORT_TIMING);

  forall (activeLocIdx, taskIdInLoc, taskRegion)
  in divideIntoTasks(A.domain, region, nTasksPerLocale, activeLocs)
  with (in s,
        const locRegion = A.domain.localSubdomain().dim(0),
        ref localA = A.localSlice(locRegion),
        ref localScratch = Scratch.localSlice(locRegion),
        ref localBuckets = BucketBoundaries.localSlice(locRegion)) {
    //writeln("working on task for ", taskRegion);
    var yh: yieldHelper;
    var cur = taskRegion.low;
    var end = taskRegion.high+1;
    while cur < end {
      // find the next unsorted bucket, starting at cur
      var bktType: uint(8);
      var bktStartBit: int;
      var bkt = nextUnsortedBucket(BucketBoundaries, taskRegion, region, cur,
                                   /*out*/ bktType, bktStartBit);
      // if the initial position has moved forward, record that in 'cur'
      cur = bkt.low;

      if cur >= end {
        break;
      }

      if bkt.size > 0 {
        //writeln("working on bucket ", bkt);

        if PARTITIONING_EXTRA_CHECKS {
          assert(taskRegion.contains(bkt));
        }

        // sort the bucket further
        s.sortStep(localA, localScratch, localBuckets,
                   bkt, comparator,
                   startbit=bktStartBit, bktType=bktType,
                   sequential=true, ifAllLocal=true);

        yh.maybeYield(bkt.size);
      }
    }
  }

  reportTime(spanTime, "inner sort time", region.size);

  /*for i in region {
    writeln("after inner A[", i, "] = ", A[i], " Scratch[", i, "] = ", Scratch[i], " BucketBoundaries[", i, "] = ", BucketBoundaries[i]);
  }*/
}

proc psort(ref A: [],
           ref Scratch: [] A.eltType,
           ref BucketBoundaries: [] uint(8),
           region: range,
           comparator,
           param radixBits: int,
           logBuckets: int,
           nTasksPerLocale: int,
           endbit: int,
           markAllEquals=false,
           useExistingBuckets=false,
           noBaseCase=false) : void {
  type splitterType = if radixBits != 0
                      then radixSplitters(radixBits)
                      else splitters(A.eltType);


  var sorter = new partitioningSorter(A.eltType, splitterType,
                                      radixBits=radixBits,
                                      logBuckets=logBuckets,
                                      nTasksPerLocale=nTasksPerLocale,
                                      endbit=endbit,
                                      markAllEquals=markAllEquals,
                                      useExistingBuckets=useExistingBuckets,
                                      noBaseCase=noBaseCase);

  var sorterRunTime = startTime(PARTITIONING_SORT_TIMING);

  sorter.psort(A, Scratch, BucketBoundaries, region, comparator);

  reportTime(sorterRunTime, "sorter run time", region.size);
}

proc psort(ref A: [],
           ref Scratch: [] A.eltType,
           region: range,
           comparator,
           param radixBits: int,
           logBuckets:int=radixBits,
           endbit:int=max(int),
           nTasksPerLocale: int = computeNumTasks()) {
  type splitterType = if radixBits != 0
                      then radixSplitters(radixBits)
                      else splitters(A.eltType);

  var sorter = new partitioningSorter(A.eltType, splitterType,
                                      radixBits=radixBits,
                                      logBuckets=logBuckets,
                                      nTasksPerLocale=nTasksPerLocale,
                                      endbit=endbit);

  if region.size <= sorter.baseCaseLimit {
    partitionSortBaseCase(A, region, comparator);
    return;
  }

  var BucketBoundaries:[A.domain[region]] uint(8);
  sorter.psort(A, Scratch, BucketBoundaries, region, comparator);
}

/* Use a tournament tree (tree of losers) to perform multi-way merging.
   This does P-way merging, assuming that the P ranges in InputRanges
   represent the P sorted regions. OutputRange represents where the
   output should be placed in the Output array and should have a matching size.

   The type readEltType will be used for storing the element for comparison
   in the tournament tree. It might be useful for it to be a different type
   from eltType (e.g. if eltType are offsets into another array or otherwise
   pointers, it might be useful to store full records in the tournament tree).
   If readEltType differs from eltType, this code will cast (with operator : )
   from eltType to readEltType and back again.
   */
proc multiWayMerge(Input: [] ?eltType,
                   InputRanges: [] range,
                   ref Output: [] ?outEltType,
                   outputRange: range,
                   comparator,
                   type readEltType=eltType) {
  const P = InputRanges.size;

  if P <= 1 {
    // Copy the input ranges to the output
    var pos = outputRange.low;
    for r in InputRanges {
      for i in r {
        Output[pos] = Input[i]:outEltType;
        pos += 1;
      }
    }
    return;
  }

  var InternalNodes: [0..<P] int; // integer indices into ExternalNodes
                                   // indicating what the loser was,
                                   // except Losers[0] is the winner of the
                                   // tournament

  // We will store the tree in the order described in Knuth vol.
  // Sorting and Searching:
  //
  // This is the example of the internal nodes of a 12-node tree,
  // followed by the external nodes, which start with e, but continue
  // the numbering:
  //                              1
  //                  2                             3
  //         4                 5               6        7
  //    8        9        10       11       e12 e13  e14 e15
  // e16 e17  e18 e19  e20 e21  e22 e23

  // some observations about this way of numbering nodes:
  //  * for node i, the parent node number can be computed by i / 2
  //  * for node i, the child nodes are 2*i and 2*i + 1
  //  * the leftmost node in each row is a power of 2
  //  * there are always an even number of elements in the bottom row

  // these are numbered P..<2*P to match the external node numbering above
  // the element 2*P is also included to allow the algorithm to consider
  // that the "infinity" element without too much fuss.
  var ExternalNodes: [P..2*P] readEltType; // values that have been read
  var ReadPosition: [P..2*P] int; // index into Input for each sorted list
  var ReadEnd: [P..2*P] int; // end position for each Input list (inclusive)

  // Set up ReadPosition and ReadEnd, and read in the initial records
  for i in 0..<P {
    ReadPosition[P+i] = InputRanges[i].low;
    ReadEnd[P+i] = InputRanges[i].high;
    if ReadPosition[P+i] <= ReadEnd[P+i] {
      ExternalNodes[P+i] = Input[ReadPosition[P+i]]: readEltType;
    }
  }
  // Position/End for 2*P should represent an invalid range, so that
  // checks for end-of-sequence on infinity will say it's end-of-sequence.
  ReadPosition[2*P] = 1;
  ReadEnd[2*P] = 0;

  // compute the regular tournament tree (storing winners)
  var nRows = 2 + log2(P); // e.g. 5 rows for the example tree of 12
                           // Losers[0] is not considered a row

  var inf = 2*P; // how we represent ∞ in internal nodes,
                 // but ExternalNodes[inf] actually exists

  proc doCompare(eltA, eltB, addrA, addrB) {
    //writeln("doCompare ", eltA, " ", eltB, " ", addrA, " ", addrB);
    if addrB == inf {
      return -1; // a is less if b is infinity
    }
    if addrA == inf {
      return 1; // b is less if a is infinity
    }
    return mycompare(eltA, eltB, comparator);
  }

  // consider the rows in reverse order; we will compare elements
  for row in 1..<nRows by -1 {
    //writeln("Working on row ", row);

    const rowStart = 1 << row; // e.g., last row in example starts at 16
    const maxRowSize = 1 << row; // e.g. last row could have up to 16 elts
    const rowSize = min(maxRowSize, 2*P - rowStart);
    for i in rowStart..#rowSize by 2 {
      // compare element i with element i+1

      //writeln("i is ", i);

      // get a reference to the elements to compare
      const ref eltA = if i < P
                       then ExternalNodes[InternalNodes[i]]
                       else ExternalNodes[i];
      const ref eltB = if i+1 < P
                       then ExternalNodes[InternalNodes[i+1]]
                       else ExternalNodes[i+1];
      // what number will we store if the comparison indicates?
      // need to propagate a winner from the current InternalNode
      // if we are working on an internal node.
      const tmpAddrA = if i < P then InternalNodes[i] else i;
      const tmpAddrB = if i+1 < P then InternalNodes[i+1] else i+1;
      //writeln("tmpAddrA ", tmpAddrA);
      //writeln("tmpAddrB ", tmpAddrB);
      const addrA = if ReadPosition[tmpAddrA] <= ReadEnd[tmpAddrA]
                    then tmpAddrA
                    else inf;
      const addrB = if ReadPosition[tmpAddrB] <= ReadEnd[tmpAddrB]
                    then tmpAddrB
                    else inf;
      //writeln("addrA ", addrA);
      //writeln("addrB ", addrB);
      ref eltDst = InternalNodes[i/2];
      //writeln("Comparing ", addrA, " vs ", addrB);
      if doCompare(eltA, eltB, addrA, addrB) < 0 {
        //writeln("Setting node ", i/2, " to ", addrA);
        eltDst = addrA;
      } else {
        //writeln("Setting node ", i/2, " to ", addrB);
        eltDst = addrB;
      }
    }
  }
  // copy the champion to the top of the tree
  InternalNodes[0] = InternalNodes[1];

  //writeln("Winners tree");
  //writeln("InternalNodes ", ExternalNodes[InternalNodes]);

  // change the InternalNodes to store losers rather than winners
  // note that the order in which this loop executes is important
  // (since it reads from 2*i while setting i)
  for i in 1..<P {
    const left = 2*i;
    const right = 2*i + 1;
    const tmpAddrLeft =  if left < P then InternalNodes[left] else left;
    const tmpAddrRight = if right < P then InternalNodes[right] else right;
    const addrLeft =  if ReadPosition[tmpAddrLeft] <= ReadEnd[tmpAddrLeft]
                      then tmpAddrLeft
                      else inf;
    const addrRight = if ReadPosition[tmpAddrRight] <= ReadEnd[tmpAddrRight]
                      then tmpAddrRight
                      else inf;

    if InternalNodes[i] == addrLeft {
      // addrLeft was the winner, so store addrRight
      InternalNodes[i] = addrRight;
    } else if InternalNodes[i] == addrRight {
      // addrRight was the winner, so store addrLeft
      InternalNodes[i] = addrLeft;
    } else {
      assert(false && "problem constructing tournament tree");
    }
  }

  //writeln("Loser's tree");
  //writeln("InternalNodes ", ExternalNodes[InternalNodes]);


  var outPos = outputRange.low;
  while true {
    //writeln("looping");
    //writeln("InternalNodes ", InternalNodes);
    //writeln("ExtarnalNodes[InternalNodes] ", ExternalNodes[InternalNodes]);

    var championAddr = InternalNodes[0]; // index of external node in P..<2*P
    if championAddr == inf {
      break;
    }

    // output the champion
    //writeln("outputting ", ExternalNodes[championAddr]);
    Output[outPos] = ExternalNodes[championAddr] : outEltType;
    outPos += 1;

    // input the new value
    var championAddrOrInf = championAddr;
    ref ChampionPos = ReadPosition[championAddr];
    if ChampionPos+1 <= ReadEnd[championAddr] {
      ChampionPos += 1;
      ExternalNodes[championAddr] = Input[ChampionPos];
      //writeln("Read ", ExternalNodes[championAddr], " into ", championAddr);
    } else {
      championAddrOrInf = inf;
    }

    // move up the tree, adjusting the losers in InternalNodes
    // and updating championAddr based on the comparisons
    var i = championAddr / 2; // parent internal node
    while i >= 1 {
      //writeln("Setting Internal Node ", i);
      // championAddr is an outer variable loop, updated as needed
      const ref championElt = ExternalNodes[championAddrOrInf];

      ref Loser = InternalNodes[i];
      const otherAddr = Loser; // load the current value
      const ref otherElt = ExternalNodes[otherAddr];

      if doCompare(championElt, otherElt, championAddrOrInf, otherAddr) < 0 {
        // newElt has won, nothing to do:
        //  * championAddr is still correct
        //  * Loser is still correct
        //writeln("champion beats ", ExternalNodes[otherAddr]);
      } else {
        // otherElt has won, update the loser and champion
        Loser = championAddrOrInf;
        championAddrOrInf = otherAddr;
        //writeln("champion lost to ", ExternalNodes[otherAddr]);
      }

      i /= 2;
    }
    // store the champion back into the tree
    InternalNodes[0] = championAddrOrInf;
  }
}


} // end module Partitioning
