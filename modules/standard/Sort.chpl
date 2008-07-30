def InsertionSort(Data: [?Dom], doublecheck = false) where Dom.rank == 1 {
  const lo = Dom.low;
  for i in Dom {
    const ithVal = Data(i);
    var inserted = false;
    for j in lo..i-1 by -1 {
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
    for i in Dom.low..Dom.high-1 do
      if (Data(i+1) < Data(i)) then
        halt("InsertionSort did not sort properly: ", Data);
}


def QuickSort(Data: [?Dom], doublecheck = false) where Dom.rank == 1 {
  // grab obvious indices
  const lo = Dom.low, 
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case -- use insertion sort
  if (hi - lo < 10) {
    InsertionSort(Data);
    return;
  }

  // find pivot using median-of-3 method
  if (Data(mid) < Data(lo)) then Data(mid) <=> Data(lo);
  if (Data(hi) < Data(lo)) then Data(hi) <=> Data(lo);
  if (Data(hi) < Data(mid)) then Data(hi) <=> Data(mid);
  const pivotVal = Data(mid);
  Data(mid) = Data(hi-1);
  Data(hi-1) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo, 
      hiptr = hi-1;
  while (loptr < hiptr) {
    do { loptr += 1; } while (Data(loptr) < pivotVal);
    do { hiptr -= 1; } while (pivotVal < Data(hiptr));
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-1) = Data(loptr);
  Data(loptr) = pivotVal;

  //  cobegin {
    QuickSort(Data[..loptr-1]);  // could use unbounded ranges here
    QuickSort(Data[loptr+1..]);
    //  }

  if (doublecheck) then
    for i in Dom.low..Dom.high do
      if (Data(i+1) < Data(i)) then
        halt("QuickSort did not sort properly: ", Data);
}
