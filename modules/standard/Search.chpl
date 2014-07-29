proc LinearSearch(Data:[?Dom], val) {
  for i in Dom {
    if (Data(i) == val) {
      return (true, i);
    } else if (Data(i) > val) {
      return (false, i);
    }
  }
  return (false,Dom.high+1);
}


// would really like to drop the lo/hi arguments here, but right now
// that causes too big of a memory leak
proc BinarySearch(Data:[?Dom], val, in lo = Dom.low, in hi = Dom.high) {
  while (lo <= hi) {
    const mid = (hi - lo)/2 + lo;
    if (Data(mid) == val) {
      return (true, mid);
    } else if (val > Data(mid)) {
      lo = mid+1;
    } else {
      hi = mid-1;
    }
  }
  return (false, lo);
}
