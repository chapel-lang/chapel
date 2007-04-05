def InsertionSort(Data: [?Dom], doublecheck = false) where Dom.rank == 1 {
  const lo = Dom.low;
  for i in Dom {
    const ithVal = Data(i);
    var inserted = false;
    for j in [lo..i) by -1 {
      if (ithVal < Data(j)) {
        Data(j+1) = Data(j);
      } else {
        Data(j+1) = ithVal;
        inserted = true;
        break;
      }
    }
    if (!inserted) {
      Data(lo) = ithVal;
    }
  }

  if (doublecheck) then
    for i in [Dom) do
      if (Data(i+1) < Data(i)) then
        halt("InsertionSort did not sort properly: ", Data);
}


def QuickSort(Data: [?Dom], doublecheck = false) where Dom.rank == 1 {
  // grab obvious indices
  const lo = Dom.low, 
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case
  if (hi - lo < 10) {
    InsertionSort(Data);
    return;
  }

  // find pivot
  const pivotVal = Data(mid);
  Data(mid) = Data(hi);
  Data(hi) = pivotVal;

  var loptr = lo, 
      hiptr = hi-1;
  while (loptr < hiptr) {
    while (Data(loptr) < pivotVal) { loptr += 1; }
    while (Data(hiptr) > pivotVal && hiptr > loptr) { hiptr -= 1; }
    if (loptr < hiptr) {
      // Data(loptr) <=> Data(hiptr);
      const tmp = Data(loptr);
      Data(loptr) = Data(hiptr);
      Data(hiptr) = tmp;
    }
  }
  // Data(loptr) <=> Data(hi);
  Data(hi) = Data(loptr);
  Data(loptr) = pivotVal;

  //  cobegin {
    QuickSort(Data[lo..loptr-1]);  // could use unbounded ranges here
    QuickSort(Data[loptr+1..hi]);
    //  }

  if (doublecheck) then
    for i in [Dom) do
      if (Data(i+1) < Data(i)) then
        halt("QuickSort did not sort properly: ", Data);
}
