// bubble sort is generally too slow to be useful in practice
// but, it is stable and in-place
module BubbleSort {
  import Sort.{defaultComparator, chpl_check_comparator, chpl_compare};

  /*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc bubbleSort(ref Data: [?Dom] ?eltType,
                  comparator:?rec = new defaultComparator()) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("bubbleSort() requires 1-D array");
    }

    const low = Dom.low,
          high = Dom.high,
          stride = abs(Dom.stride);

    var swapped = true;

    while swapped {
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
