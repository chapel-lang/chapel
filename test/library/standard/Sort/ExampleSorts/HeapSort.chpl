// heap sort has good n*log(n) worst case performance
// it is in-place but not stable.
module HeapSort {
  import Sort.{DefaultComparator, chpl_check_comparator, chpl_compare};
  /*

   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc heapSort(ref Data: [?Dom] ?eltType,
                comparator:?rec = new DefaultComparator()) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("heapSort() requires 1-D array");
    }

    const low = Dom.low,
          high = Dom.high,
          size = Dom.size,
          stride = abs(Dom.stride);

    // Heapify
    var start = if high == low then high
                else if size % 2 then low + ((size - 1)/2) * stride
                else low + (size/2 - 1) * stride;

    while start >= low {
      siftDown(start, high, comparator);
      start = start - stride;
    }

    // Sort, moving max element to end and re-heapifying the rest
    var end = high;
    while end > low {
      Data(end) <=> Data(low);
      end = end - stride;
      siftDown(low, end, comparator);
    }

    proc siftDown(start, end, comparator:?rec = new DefaultComparator()) {
      var root = start;
      while (2*root - low + stride) <= end {
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
