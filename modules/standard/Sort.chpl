proc HeapSort(Data: [?Dom], doublecheck=false) where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  const len = Dom.dim(1).size;
  
  // heapify
  var start = (len - 2) / 2 + lo;
  while (start >= lo) {
    SiftDown(start, hi);
    start = start - 1;
  }
  
  // sort, moving max element to end and re-heapifying the rest
  var end = hi;
  while (end > lo) {
    Data(end) <=> Data(lo);
    end = end - 1;
    SiftDown(lo, end);
  }

  proc SiftDown(start, end) {
    var root = start;
    while (root * 2 + 1 - lo <= end) {
      const child = root * 2 + 1 - lo;
      var swap = root;
      if Data(swap) < Data(child) then swap = child;
      if (child + 1 <= end) && (Data(swap) < Data(child + 1)) then swap = child + 1;
      if swap != root {
        Data(root) <=> Data(swap);
        root = swap;
      } else {
        return;
      }
    }
  }
  if (doublecheck) then VerifySort(Data, "HeapSort");
}


proc BubbleSort(Data: [?Dom], doublecheck=false) where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  var swapped = true;
  while (swapped) {
    swapped = false;
    for i in lo..hi-1 {
      if Data(i) > Data(i+1) {
        Data(i) <=> Data(i+1);
        swapped = true;
      }
    }
  }

  if (doublecheck) then VerifySort(Data, "BubbleSort");
}


proc InsertionSort(Data: [?Dom], doublecheck=false) where Dom.rank == 1 {
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

  if (doublecheck) then VerifySort(Data, "InsertionSort");
}


proc MergeSort(Data: [?Dom], minlen=16, doublecheck=false) where Dom.rank == 1 {
  _MergeSort(Data, minlen);
  if (doublecheck) then VerifySort(Data, "MergeSort");
}

proc _MergeSort(Data: [?Dom], minlen=16) where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;

  if hi-lo < minlen {
    InsertionSort(Data);
    return;
  }
  const mid = (hi-lo)/2+lo;
  var A1 = Data[lo..mid];
  var A2 = Data[mid+1..hi];
  cobegin {
    { _MergeSort(A1, minlen); }
    { _MergeSort(A2, minlen); }
  }

  for (a, _a) in zip(Data[lo..hi], _MergeIterator(A1, A2)) do a = _a;
}

iter _MergeIterator(A1: [], A2: []) {
  var a1 = A1.domain.dim(1).low;
  const a1hi = A1.domain.dim(1).high;
  var a2 = A2.domain.dim(1).low;
  const a2hi = A2.domain.dim(1).high;
  while ((a1 <= a1hi) && (a2 <= a2hi)) {
    while (A1(a1) <= A2(a2)) {
      yield A1(a1);
      a1 += 1;
      if a1 > a1hi then break;
    }
    if a1 > a1hi then break;
    while (A2(a2) < A1(a1)) {
      yield A2(a2);
      a2 += 1;
      if a2 > a2hi then break;
    }
  }
  if a1 == a1hi then yield A1(a1);
  else if a2 == a2hi then yield A2(a2);
  if a1 < a1hi then for a in A1[a1..a1hi] do yield a;
  else if a2 < a2hi then for a in A2[a2..a2hi] do yield a;
}


proc QuickSort(Data: [?Dom], minlen=16, doublecheck=false) where Dom.rank == 1 {
  // grab obvious indices
  const lo = Dom.low, 
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
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

  if (doublecheck) then VerifySort(Data, "QuickSort");
}


proc SelectionSort(Data: [?Dom], doublecheck=false) where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  for i in {lo..hi-1} {
    var (_, loc) = minloc reduce zip(Data[i..hi], {i..hi});
    Data(i) <=> Data(loc);
  }

  if (doublecheck) then VerifySort(Data, "SelectionSort");
}

inline proc VerifySort(Data: [?Dom], str: string) {
  for i in Dom.low..Dom.high-1 do
    if (Data(i+1) < Data(i)) then
      halt(str, " did not sort properly (", i, "): ", Data);
}
