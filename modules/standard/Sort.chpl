/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
   The `Sort` module is designed to support standard sorting
   routines.  The current interface is minimal and should be expected to
   grow and evolve over time. Each of the following functions accepts an
   optional boolean argument, `reverse`, which is `false` by
   default. If `true`, then the sort function will order in reverse.

 */
module Sort {

pragma "no doc"
private inline proc chpl_sort_cmp(a, b, param reverse=false, param eq=false) {
  if eq {
    if reverse then return a >= b;
    else return a <= b;
  } else {
    if reverse then return a > b;
    else return a < b;
  }
}


/*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] eltType
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: bool
   :arg reverse: Sort in reverse numerical order
   :type reverse: bool

 */
proc BubbleSort(Data: [?Dom] ?elType, doublecheck=false, param reverse=false) where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  var swapped = true;

  while (swapped) {
    swapped = false;
    for i in lo..hi-1 {
      if !chpl_sort_cmp(Data(i), Data(i+1), reverse, true) {
        Data(i) <=> Data(i+1);
        swapped = true;
      }
    }
  }

  if (doublecheck) then VerifySort(Data, "BubbleSort", reverse);
}


/*
   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] elType
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: bool
   :arg reverse: Sort in reverse numerical order
   :type reverse: bool

 */
proc HeapSort(Data: [?Dom] ?elType, doublecheck=false, param reverse=false) where Dom.rank == 1 {
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
      if chpl_sort_cmp(Data(swap), Data(child), reverse) then swap = child;
      if (child + 1 <= end) && (chpl_sort_cmp(Data(swap), Data(child + 1), reverse)) then swap = child + 1;
      if swap != root {
        Data(root) <=> Data(swap);
        root = swap;
      } else {
        return;
      }
    }
  }
  if (doublecheck) then VerifySort(Data, "HeapSort", reverse);
}


/*
   Sort the 1D array `Data` in-place using a sequential insertion sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] eltType
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: bool
   :arg reverse: Sort in reverse numerical order
   :type reverse: bool

 */
proc InsertionSort(Data: [?Dom] ?elType, doublecheck=false, param reverse=false) where Dom.rank == 1 {
  const lo = Dom.low;
  for i in Dom {
    const ithVal = Data(i);
    var inserted = false;
    for j in lo..i-1 by -1 {
      if (chpl_sort_cmp(ithVal, Data(j), reverse)) {
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

  if (doublecheck) then VerifySort(Data, "InsertionSort", reverse);
}


/*
   Sort the 1D array `Data` in-place using a parallel merge sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] eltType
   :arg minlen: When the array size is less than `minlen` use insertion sort algorithm
   :type minlen: integral
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: bool
   :arg reverse: Sort in reverse numerical order
   :type reverse: bool

 */
proc MergeSort(Data: [?Dom], minlen=16, doublecheck=false, param reverse=false) where Dom.rank == 1 {
  _MergeSort(Data, minlen, reverse=reverse);
  if (doublecheck) then VerifySort(Data, "MergeSort", reverse);
}


pragma "no doc"
private proc _MergeSort(Data: [?Dom], minlen=16, param reverse=false) where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  if hi-lo < minlen {
    InsertionSort(Data, reverse=reverse);
    return;
  }
  const mid = (hi-lo)/2+lo;
  var A1 = Data[lo..mid];
  var A2 = Data[mid+1..hi];
  cobegin {
    { _MergeSort(A1, minlen, reverse=reverse); }
    { _MergeSort(A2, minlen, reverse=reverse); }
  }

  for (a, _a) in zip(Data[lo..hi], _MergeIterator(A1, A2, reverse=reverse)) do a = _a;
}


pragma "no doc"
private iter _MergeIterator(A1: [] ?elType, A2: [] elType, param reverse=false) {
  var a1 = A1.domain.dim(1).low;
  const a1hi = A1.domain.dim(1).high;
  var a2 = A2.domain.dim(1).low;
  const a2hi = A2.domain.dim(1).high;
  while ((a1 <= a1hi) && (a2 <= a2hi)) {
    while (chpl_sort_cmp(A1(a1) ,A2(a2), reverse, true)) {
      yield A1(a1);
      a1 += 1;
      if a1 > a1hi then break;
    }
    if a1 > a1hi then break;
    while (chpl_sort_cmp(A2(a2), A1(a1), reverse, true)) {
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


/*
   Sort the 1D array `Data` in-place using a sequential quick sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] eltType
   :arg minlen: When the array size is less than `minlen` use insertion sort algorithm
   :type minlen: integral
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: bool
   :arg reverse: Sort in reverse numerical order
   :type reverse: bool

 */
proc QuickSort(Data: [?Dom] ?elType, minlen=16, doublecheck=false, param reverse=false) where Dom.rank == 1 {
  // grab obvious indices
  const lo = Dom.low, 
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
    InsertionSort(Data, reverse=reverse);
    return;
  }

  // find pivot using median-of-3 method
  if (chpl_sort_cmp(Data(mid), Data(lo), reverse)) then Data(mid) <=> Data(lo);
  if (chpl_sort_cmp(Data(hi), Data(lo), reverse)) then Data(hi) <=> Data(lo);
  if (chpl_sort_cmp(Data(hi), Data(mid), reverse)) then Data(hi) <=> Data(mid);
  const pivotVal = Data(mid);
  Data(mid) = Data(hi-1);
  Data(hi-1) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo, 
      hiptr = hi-1;
  while (loptr < hiptr) {
    do { loptr += 1; } while (chpl_sort_cmp(Data(loptr), pivotVal, reverse));
    do { hiptr -= 1; } while (chpl_sort_cmp(pivotVal, Data(hiptr), reverse));
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-1) = Data(loptr);
  Data(loptr) = pivotVal;

  //  cobegin {
    QuickSort(Data[..loptr-1], reverse=reverse);  // could use unbounded ranges here
    QuickSort(Data[loptr+1..], reverse=reverse);
    //  }

  if (doublecheck) then VerifySort(Data, "QuickSort", reverse);
}


/*
   Sort the 1D array `Data` in-place using a sequential selection sort
   algorithm.

   :arg Data: The array to be sorted
   :type Data: [] eltType
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: bool
   :arg reverse: Sort in reverse numerical order
   :type reverse: bool

 */
proc SelectionSort(Data: [?Dom], doublecheck=false, param reverse=false) where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  for i in lo..hi-1 {
    var (_, loc) = if reverse then maxloc reduce zip(Data[i..hi], {i..hi})
      else minloc reduce zip(Data[i..hi], {i..hi});
    Data(i) <=> Data(loc);
  }

  if (doublecheck) then VerifySort(Data, "SelectionSort", reverse);
}


/*
   Verify that the array `Data` is in sorted order and halt if any element is
   out of order.

   :arg Data: The array to verify
   :type Data: [] eltType
   :arg str: string to print while halting if an element is out of order
   :type str: string
   :arg reverse: if true, expect the values to be sorted in reverse.
   :type reverse: bool

 */
inline proc VerifySort(Data: [?Dom] ?elType, str: string, param reverse=false) {
  for i in Dom.low..Dom.high-1 do
    if chpl_sort_cmp(Data(i+1), Data(i), reverse) then
      halt(str, " did not sort properly (", i, "): ", Data);
}


//
// This is a first draft "sorterator" which is designed to take some
// other iterator/iterable and yield its elements, in sorted order.
//
// The main limitations in the current code are (1) it should put some
// sort of constraint on 'x' to limit it to types for which this makes
// sense; and (2) there should be some generic way to say "y is an
// array of x's element type" (or to infer its element type) without
// saying a priori how big it is.  Without these mods, the result is
// that the sorterator works when it does and probably is confusing
// when it doesn't.
//
/*
   Yield the elements of argument `x` in sorted order.

   :arg x: An iterable value to be sorted and yielded element by element
   :type x: iterable

   :yields: The elements of x in sorted order
   :ytype: x's element type

 */
iter sorted(x) {
  var y = x;
  QuickSort(y);
  for i in y do
    yield i;
}
}
