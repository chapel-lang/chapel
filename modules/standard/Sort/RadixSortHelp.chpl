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



@chpldoc.nodoc
module RadixSortHelp {
  private use Sort;
  import Reflection.canResolveMethod;

  // This is the number of bits to sort at a time in the radix sorter.
  // The code assumes that all integer types are a multiple of it.
  // That would need to change if it were to increase.
  //
  // At the same time, using a value less than 8 will probably perform poorly.
  // TODO: parameterize functions using this so that different
  // algorithms can use a different number of radix bits
  param RADIX_BITS = 8;

  param classifyUnrollFactor = 7;

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
    const testPart = testRet(1);
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

    const (section, part) = criterion.keyPart(a, whichpart);
    var ubits = part:uint(bitsPerPart);
    // If the number is signed, invert the top bit, so that
    // the negative numbers sort below the positive numbers
    if isInt(part) {
      const one:ubits.type = 1;
      ubits = ubits ^ (one << (bitsPerPart - 1));
    }
    param mask:uint = (1 << RADIX_BITS) - 1;
    const ubin = (ubits >> (bitsPerPart - bitsinpart - RADIX_BITS)) & mask;

    if section:int == 0 then
      return (ubin:int + 1, ubits);
    else if section:int < 0 then
      return (0, ubits);
    else
      return ((1 << RADIX_BITS) + 1, ubits);
  }


  // Get the bin for a record with criterion.key or criterion.keyPart
  //
  // See binForRecordKeyPart for what the arguments / returns mean.
  inline
  proc binForRecord(a, criterion, startbit:int)
  {
    if canResolveMethod(criterion, "keyPart", a, 0) {
      return binForRecordKeyPart(a, criterion, startbit);
    } else if canResolveMethod(criterion, "key", a) {
      // Try to use the default comparator to get a keyPart.
      return binForRecordKeyPart(criterion.key(a),
                                 new defaultComparator(),
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
      return tmp.size * numBits(tmp(0).type);
    }

    return -1;
  }

  // Returns a compile-time known final startbit
  // e.g. for uint(64), returns 56 (since that's 64-8 and the
  // last sort pass will sort on the last 8 bits).
  //
  // Returns -1 if no such ending is known at compile-time.
  proc msbRadixSortParamLastStartBit(Data:[], comparator) param {
    // Compute end_bit if it's known
    // Default comparator on integers has fixed width
    const ref element = Data[Data.domain.low];
    if comparator.type == defaultComparator && fixedWidth(element.type) > 0 {
      return fixedWidth(element.type) - RADIX_BITS;
    } else if canResolveMethod(comparator, "key", element) {
      type keyType = comparator.key(element).type;
      if fixedWidth(keyType) > 0 then
        return fixedWidth(keyType) - RADIX_BITS;
    }

    return -1;
  }

  // Returns a compile-time known key width
  // e.g. for uint(64), returns 64
  //
  // Returns -1 if no such ending is known at compile-time.
  proc radixSortParamEndBit(Data:[], comparator) param {
    // Compute end_bit if it's known
    // Default comparator on integers has fixed width
    const ref element = Data[Data.domain.low];
    if comparator.type == defaultComparator && fixedWidth(element.type) > 0 {
      return fixedWidth(element.type);
    } else if canResolveMethod(comparator, "key", element) {
      type keyType = comparator.key(element).type;
      if fixedWidth(keyType) > 0 then
        return fixedWidth(keyType);
    }

    return -1;
  }


  proc radixSortClz(val) {
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

    var new_start = radixSortClz(xor);
    var new_digit = new_start / RADIX_BITS;
    var new_start_bit_rounded = new_digit * RADIX_BITS;

    return new_start_bit_rounded:int;
  }
  // Check that the elements from start_n..end_n in A are sorted by criterion

  pragma "insert line file info"
  pragma "always propagate line file info"
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

  record RadixBucketizer {
    proc isSampleSort param do return false;

    proc getNumBuckets() {
      return (1 << RADIX_BITS) + 2; // +2 for end-before and end-after bins
    }
    proc getBinsToRecursivelySort() {
      // first and last bin represent end-of-key and do not need
      // to be recursively sorted.
      return 1..(1 << RADIX_BITS);
    }
    proc getNextStartBit(start_bit:int) {
      return start_bit+8;
    }

    // returns the bin for classifying a single record
    inline proc bucketForRecord(a, criterion, startbit) {
      const (bin, _) = binForRecord(a, criterion, startbit);
      return bin;
    }

    // yields (index, bucket index) for A[start_n..end_n]
    iter classify(A, start_n, end_n, criterion, startbit) {
      type idxType = if isArray(A) then A.idxType else int;
      var cur = start_n;
      while cur <= end_n-(classifyUnrollFactor-1) {
        for /*param*/ j in 0..classifyUnrollFactor-1 {
          const cur_j_idx = (cur+j):idxType;
          yield (cur+j,
                 bucketForRecord(A[cur_j_idx], criterion, startbit));
        }
        cur += classifyUnrollFactor;
      }
      while cur <= end_n {
        const cur_idx = cur:idxType;
        yield (cur, bucketForRecord(A[cur_idx], criterion, startbit));
        cur += 1;
      }
    }
  }
}

