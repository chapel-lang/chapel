
// this selection sort is in-place but not stable
@chpldoc.nodoc
module SelectionSort {
  private use Sort;
  /*
    Sort the 1D array `Data` in-place using a sequential selection sort
    algorithm.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.

   */
  proc selectionSort(ref Data: [?Dom] ?eltType,
                     comparator:?rec = new DefaultComparator()) {
    // note: selection sort is not a stable sort

    if Dom.rank != 1 {
      compilerError("selectionSort() requires 1-D array");
    }

    const low = Dom.low,
          high = Dom.high,
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
