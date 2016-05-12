/* Home grown selection sort that assumes only first element changed */
/**
  * Quick sort array of lists by list Array.size
  * When Chapel supports FCF default arguments, we can make this better
 **/
proc QuickSortLists(Data: [?Dom] ?eltType, minlen=16, param reverse=false) where Dom.rank == 1 {
  // grab obvious indices
  const lo = Dom.low,
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
    InsertionSortLists(Data, reverse=reverse);
    return;
  }

  // find pivot using median-of-3 method
  if (sortCompare(Data(mid).length, Data(lo).length, reverse)) {
    Data(mid) <=> Data(lo);
  }
  if (sortCompare(Data(hi).length, Data(lo).length, reverse)) {
    Data(hi) <=> Data(lo);
  }
  if (sortCompare(Data(hi).length, Data(mid).length, reverse)) {
    Data(hi) <=> Data(mid);
  }

  const pivotVal = Data(mid);
  Data(mid) = Data(hi-1);
  Data(hi-1) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo,
      hiptr = hi-1;
  while (loptr < hiptr) {
    do { loptr += 1; } while (sortCompare(Data(loptr).length, pivotVal.length, reverse));
    do { hiptr -= 1; } while (sortCompare(pivotVal.length, Data(hiptr).length, reverse));
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-1) = Data(loptr);
  Data(loptr) = pivotVal;

  //  cobegin {
    QuickSortLists(Data[..loptr-1], reverse=reverse);  // could use unbounded ranges here
    QuickSortLists(Data[loptr+1..], reverse=reverse);
    //  }
}

proc InsertionSortLists(Data: [?Dom] ?eltType, doublecheck=false, param reverse=false) where Dom.rank == 1 {
  const lo = Dom.low;
  for i in Dom {
    const ithVal = Data(i);
    var inserted = false;
    for j in lo..i-1 by -1 {
      if (sortCompare(ithVal.length, Data(j).length, reverse)) {
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
}

inline proc sortCompare(a, b, param reverse=false, param eq=false) {
  if eq {
    if reverse then return a >= b;
    else return a <= b;
  } else {
    if reverse then return a > b;
    else return a < b;
  }
}
