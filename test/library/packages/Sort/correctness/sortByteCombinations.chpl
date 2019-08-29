config const size = 100000;

use Sort;

proc checkSorts(arr) {
  // check default sorter
  var a = arr;
  sort(a);
  assert(isSorted(a));
  // check msbRadixSort
  var b = arr;
  MSBRadixSort.msbRadixSort(b);
  assert(isSorted(b));
  // check quickSort
  var c = arr;
  QuickSort.quickSort(c);
  assert(isSorted(c));
}

proc checkem(type t) {
  { // check all bytes present in last byte
    var arr:[1..size] t;
    for (i,a) in zip(arr.domain, arr) {
      a = (i & 0xff):t;
    }
    checkSorts(arr);
  }

  { // check all bytes present in first byte
    var arr:[1..size] t;
    for (i,a) in zip(arr.domain, arr) {
      a = ((i & 0xff) << (numBits(t) - 8)):t;
    }
    checkSorts(arr);
  }

  { // check all bytes present in first, last byte
    var arr:[1..size] t;
    for (i,a) in zip(arr.domain, arr) {
      a = ((i & 0xff) + ((i & 0xff) << (numBits(t) - 8))):t;
    }
    checkSorts(arr);
  }

  { // check repeated byte
    var arr:[1..size] t;
    for (i,a) in zip(arr.domain, arr) {
      var tmp:uint = (i & 0xff):uint;
      var j = 0;
      while j < numBits(t) {
        tmp <<= 8;
        tmp |= (i & 0xff);
        j += 8;
      } 
      a = tmp:t;
    }
    checkSorts(arr);
  }
}

checkem(int);
checkem(uint);
