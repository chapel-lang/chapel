// binary insertion sort is similar to insertion sort (stable, in-place)
// and still O(n**2) but it can reduce the number of comparisons
module BinaryInsertionSort {
  private use Sort;
  /*
    Sort the 1D array `Data` in-place using a sequential, stable binary
    insertion sort algorithm.
    Should be used when there is a high cost of comparison.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.
   */
  proc binaryInsertionSort(ref Data: [?Dom] ?eltType,
                           comparator:?rec = new DefaultComparator()) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("binaryInsertionSort() requires 1-D array");
    }

    const low = Dom.low,
          high = Dom.high,
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
  private
  proc _binarySearchForLastOccurrence(Data: [?Dom], val,
                                      comparator:?rec = new DefaultComparator(),
                                      in lo=Dom.low, in hi=Dom.high) {
    const stride = abs(Dom.stride): Dom.idxType;

    var loc = -1;                                        // index of the last occurrence of val in Data

    while lo <= hi {
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
