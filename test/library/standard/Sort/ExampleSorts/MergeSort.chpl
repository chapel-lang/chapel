// merge sort has in-place and not in-place variants
// this implements a version using scratch space (not in place)
// also, this version uses partial parallelism (for recursive subproblems)
// could be faster using a parallel merge algorithm or with a k-way merge.
// merge sort is a stable sort
module MergeSort {
  private use Sort;
  /*
    Sort the 1D array `Data` using a parallel merge sort algorithm.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
    :type minlen: `integral`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.

   */
  proc mergeSort(ref Data: [?Dom] ?eltType, minlen=16,
                 comparator:?rec = new DefaultComparator()) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("mergeSort() requires 1-D array");
    }

    var Scratch: Data.type;

    _MergeSort(Data, Scratch, Dom.low, Dom.high, minlen, comparator, 0);
  }

  /*
   * Use Scratch[lo..hi] as scratch space to sort the Data[lo..hi].
   *
   * Rather than copy our portion of Data into Scratch to start off
   * each _Merge(), the recursive levels will alternate merging from
   * Data into Scratch, and merging from Scratch into Data.
   *
   * At even depths -- including the initial one -- we leave the
   * sorted data in Data.  At odd depths, we leave the sorted data in
   * Scratch.
   *
   * The data stays in Data "all they way down" until the first
   * _Merge(), then is moved back and forth as we return up the chain.
   */
  private proc _MergeSort(ref Data: [?Dom], ref Scratch: [], lo:int, hi:int,
                          minlen=16, comparator:?rec = new DefaultComparator(),
                          depth: int)
    where Dom.rank == 1 {
    import Sort.InsertionSort;

    const stride = abs(Dom.stride): Dom.idxType,
          size = (hi - lo) / stride,
          mid = lo + (size/2) * stride;

    /*
     * When we return from an even depth, the data must be in Data.
     * When we return from an odd depth, the data must be in Scratch.
     * At the point we dispatch to insertionSort, the data is still in
     * Data.  So, if we get here at an odd depth, we'd have to sort
     * Data and then copy to Scratch.  Avoid the copy by doing the
     * sort one level before that, while we're still even.
     *
     * "size" is a misnomer.  For 1..10, size works out to 9.  That's
     * the value we want to base the calculation of mid on.  But for
     * the loop control, we really need to consider the size as 10.
     */
    if ((size+1) < minlen || (((depth & 1) == 0) && (size+1) < 2 * minlen)) {
      InsertionSort.insertionSort(Data, comparator=comparator, lo, hi);

      if depth & 1 {
        // At odd depths, we need to return the results in Scratch.
        // But if the test above is correct, we'll never reach this point.
        if ! Dom.strides.isPosNegOne() then
          Scratch[lo..hi by Dom.stride] = Data[lo..hi by Dom.stride];
        else
          Scratch[lo..hi] = Data[lo..hi];
      }
      return;
    }

    if(here.runningTasks() < here.numPUs(logical=true)) {
      cobegin {
        { _MergeSort(Data, Scratch, lo, mid, minlen, comparator, depth+1); }
        { _MergeSort(Data, Scratch, mid+stride, hi, minlen, comparator, depth+1); }
      }
    } else {
      _MergeSort(Data, Scratch, lo, mid, minlen, comparator, depth+1);
      _MergeSort(Data, Scratch, mid+stride, hi, minlen, comparator, depth+1);
    }

    if depth & 1 == 0 {
      _Merge(Data, Scratch, lo, mid, hi, comparator);
    } else {
      _Merge(Scratch, Data, lo, mid, hi, comparator);
    }
  }

  private proc _Merge(ref Dst: [?Dom] ?eltType, Src: [], lo:int, mid:int,
                      hi:int, comparator:?rec = new DefaultComparator()) {
    /* Data[lo..mid by stride] is much slower than Data[lo..mid] when
     * Dom is unstrided.  So specify the latter explicitly when possible. */
    const stride = abs(Dom.stride): Dom.idxType;
    const a1range = if Dom.hasPosNegUnitStride() then lo..mid
                    else lo..mid by stride;
    const a1max = mid;

    const a2range = if Dom.hasPosNegUnitStride() then (mid+1)..hi
                    else (mid+stride)..hi by stride;
    const a2max = hi;

    ref A1 = Src[a1range];
    ref A2 = Src[a2range];

    var a1 = a1range.first;
    var a2 = a2range.first;
    var i = lo;
    while ((a1 <= a1max) && (a2 <= a2max)) {
      if (chpl_compare(A1(a1), A2(a2), comparator) <= 0) {
        Dst[i] = A1[a1];
        a1 += stride;
        i += stride;
      } else {
        Dst[i] = A2[a2];
        a2 += stride;
        i += stride;
      }
    }
    while (a1 <= a1max) {
      Dst[i] = A1[a1];
      a1 += stride;
      i += stride;
    }
    while (a2 <= a2max) {
      Dst[i] = A2[a2];
      a2 += stride;
      i += stride;
    }
  }
}
