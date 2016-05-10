/*
 * Copyright 2004-2016 Cray Inc.
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

This module supports a variety of standard sorting routines on 1D arrays.

The current interface is minimal and should be expected to
grow and evolve over time.

Comparators
-----------

Comparators allow sorting data by a mechanism other than the
default comparison of array elements. To use a comparator, define a
record with either a ``key(a)`` or ``compare(a, b)`` method, and pass an
instance of that record to the sort function.

Most sort routines support comparators, which is denoted by their function
signature.

The ``key(a)`` method accepts one argument with the array ``eltType`` and
returns a value that supports all of comparison operations: ``<``, ``<=``,
``>``, ``>=``, or ``==``.

The default key method would look like this:

.. code-block:: chapel

  proc default.key(a) {
    return a;
  }


As an example, if we want to sort an array by the absolute values of its
elements, we can use the following comparator record with a key method:

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record comparator { }

  // key method maps an element to the value to be used for comparison
  proc comparator.key(a) { return abs(a); }

  myComparator = new comparator();

  QuickSort(Array, comparator=myComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);


The ``compare(a, b)`` method accepts 2 arguments of the data ``eltType`` and
returns an integer between ``1`` if ``a > b``, ``0`` if ``a == b``, and ``-1`` if ``a < b``.

The default compare method would look like this:

.. code-block:: chapel

    proc default.compare(a, b) {
      if a > b {
        return 1;
      } else if a < b {
        return -1;
      } else {
        return 0;
      }
    }


The absolute value comparison example from above can alternatively be
implemented with a compare method:

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record comparator { }

  // compare method defines how 2 elements are compared
  proc comparator.compare(a, b) {
    if abs(a) > abs(b) {
      return 1;
    } else if abs(a) < abs(b) {
      return -1;
    } else {
      return 0;
    }
  }

  myComparator = new comparator();

  QuickSort(Array, comparator=myComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);

If both methods are implemented on the record passed as the comparator, the
``compare(a, b)`` method will take priority over the ``key(a)`` method.

 */

module Sort {

pragma "no doc"
/* Empty record to serve as default comparator value in function arguments */
record defaultComparator{ }

pragma "no doc"
const emptyRecord = new defaultComparator();


pragma "no doc"
/*
    Compare function used for all sorts of sort functions.

    Returns:
      1 if a > b
      0 if a == b
     -1 if a < b

 */
private inline proc chpl_compare(a, b, param reverse=false, comparator:?rec=emptyRecord): int {
  use Reflection;

  // TODO -- we could also check types
  // Use comparator.compare(a, b) if is defined by user
  if canResolveMethod(comparator, "compare", a, b) {
    if reverse { return -comparator.compare(a, b); }
    else { return  comparator.compare(a, b); }
  }

  // TODO -- It may be faster to call key() on all data once, and mirror swaps
  // Compare results of comparator.key(a) if is defined by user
  if canResolveMethod(comparator, "key", a) && canResolveMethod(comparator, "key", b) then
    return chpl_compare(comparator.key(a), comparator.key(b), reverse);

  if reverse {
    if a < b { return 1; }
    else if b < a { return -1; }
  } else {
    if a < b { return -1; }
    else if b < a { return 1; }
  }

  return 0;
}


/*
    Check if a comparator was passed and confirm that it will work, otherwise
    throw a compile-time error.

    :arg comparator: Comparator record with 'key(a)' or 'compare(a,b)' method
    :arg a: Sample data passed to confirm that comparator methods can resolve
    :arg comparator: Record that redefines the comparison mechanism with one of
      the methods: ``comparator.key(a)`` or ``comparator.compare(a,b)``

 */
private proc chpl_check_comparator(comparator, a) {
  use Reflection;

  if comparator.type == defaultComparator {}
  else if canResolveMethod(comparator, "compare", a, a) {}
  else if canResolveMethod(comparator, "key", a) {}
  else {
    // If we make it this far, the passed comparator does not work
    compilerError("The comparator record requires a 'key(a)' or 'compare(a, b)' method");
  }
}

/*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: `bool`
   :arg reverse: Sort in reverse numerical order
   :type reverse: `bool`
   :arg comparator: Record that redefines the comparison mechanism with one of
      the methods: ``comparator.key(a)`` or ``comparator.compare(a,b)``

 */
proc BubbleSort(Data: [?Dom] ?eltType, doublecheck=false, param reverse=false, comparator:?rec=emptyRecord) where Dom.rank == 1 && isRecord(rec) {
  chpl_check_comparator(comparator, Data(Dom.dim(1).low));
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  var swapped = true;

  while (swapped) {
    swapped = false;
    for i in lo..hi-1 {
      if chpl_compare(Data(i), Data(i+1), reverse, comparator) >= 0 {
        Data(i) <=> Data(i+1);
        swapped = true;
      }
    }
  }

  if (doublecheck) then VerifySort(Data, "BubbleSort", reverse, comparator);
}


/*
   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: `bool`
   :arg reverse: Sort in reverse numerical order
   :type reverse: `bool`
   :arg comparator: Record that redefines the comparison mechanism with one of
      the methods: ``comparator.key(a)`` or ``comparator.compare(a,b)``

 */
proc HeapSort(Data: [?Dom] ?eltType, doublecheck=false, param reverse=false, comparator:?rec=emptyRecord) where Dom.rank == 1 && isRecord(rec) {
  chpl_check_comparator(comparator, Data(Dom.dim(1).low));
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  const len = Dom.dim(1).size;

  // heapify
  var start = (len - 2) / 2 + lo;
  while (start >= lo) {
    SiftDown(start, hi, comparator);
    start = start - 1;
  }

  // sort, moving max element to end and re-heapifying the rest
  var end = hi;
  while (end > lo) {
    Data(end) <=> Data(lo);
    end = end - 1;
    SiftDown(lo, end, comparator);
  }

  proc SiftDown(start, end, comparator:?rec=emptyRecord) where isRecord(rec) {
    var root = start;
    while (root * 2 + 1 - lo <= end) {
      const child = root * 2 + 1 - lo;
      var swap = root;
      if chpl_compare(Data(swap), Data(child), reverse, comparator) < 0 then swap = child;
      if (child + 1 <= end) && (chpl_compare(Data(swap), Data(child + 1), reverse, comparator) < 0) then swap = child + 1;
      if swap != root {
        Data(root) <=> Data(swap);
        root = swap;
      } else {
        return;
      }
    }
  }
  if (doublecheck) then VerifySort(Data, "HeapSort", reverse, comparator);
}


/*
   Sort the 1D array `Data` in-place using a sequential insertion sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: `bool`
   :arg reverse: Sort in reverse numerical order
   :type reverse: `bool`
   :arg comparator: Record that redefines the comparison mechanism with one of
      the methods: ``comparator.key(a)`` or ``comparator.compare(a,b)``

 */
proc InsertionSort(Data: [?Dom] ?eltType, doublecheck=false, param reverse=false, comparator:?rec=emptyRecord) where Dom.rank == 1 && isRecord(rec) {
  chpl_check_comparator(comparator, Data(Dom.dim(1).low));
  const lo = Dom.low;
  for i in Dom {
    const ithVal = Data(i);
    var inserted = false;
    for j in lo..i-1 by -1 {
      if chpl_compare(ithVal, Data(j), reverse, comparator) < 0 {
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

  if (doublecheck) then VerifySort(Data, "InsertionSort", reverse, comparator);
}


/*
   Sort the 1D array `Data` in-place using a parallel merge sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg minlen: When the array size is less than `minlen` use insertion sort algorithm
   :type minlen: `integral`
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: `bool`
   :arg reverse: Sort in reverse numerical order
   :type reverse: `bool`
   :arg comparator: Record that redefines the comparison mechanism with one of
      the methods: ``comparator.key(a)`` or ``comparator.compare(a,b)``

 */
proc MergeSort(Data: [?Dom] ?eltType, minlen=16, doublecheck=false, param reverse=false, comparator:?rec=emptyRecord) where Dom.rank == 1 && isRecord(rec) {
  chpl_check_comparator(comparator, Data(Dom.dim(1).low));
  _MergeSort(Data, minlen, reverse=reverse, comparator=comparator);
  if (doublecheck) then VerifySort(Data, "MergeSort", reverse, comparator);
}


private proc _MergeSort(Data: [?Dom], minlen=16, param reverse=false, comparator:?rec=emptyRecord) where Dom.rank == 1 && isRecord(rec) {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  if hi-lo < minlen {
    InsertionSort(Data, reverse=reverse, comparator=comparator);
    return;
  }
  const mid = (hi-lo)/2+lo;
  var A1 = Data[lo..mid];
  var A2 = Data[mid+1..hi];
  cobegin {
    { _MergeSort(A1, minlen, reverse=reverse, comparator=comparator); }
    { _MergeSort(A2, minlen, reverse=reverse, comparator=comparator); }
  }

  for (a, _a) in zip(Data[lo..hi], _MergeIterator(A1, A2, reverse=reverse, comparator=comparator)) do a = _a;
}


private iter _MergeIterator(A1: [] ?eltType, A2: [] eltType, param reverse=false, comparator:?rec=emptyRecord) where isRecord(rec) {
  var a1 = A1.domain.dim(1).low;
  const a1hi = A1.domain.dim(1).high;
  var a2 = A2.domain.dim(1).low;
  const a2hi = A2.domain.dim(1).high;
  while ((a1 <= a1hi) && (a2 <= a2hi)) {
    while (chpl_compare(A1(a1) ,A2(a2), reverse, comparator) <= 0) {
      yield A1(a1);
      a1 += 1;
      if a1 > a1hi then break;
    }
    if a1 > a1hi then break;
    while (chpl_compare(A2(a2), A1(a1), reverse, comparator) <= 0) {
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
   :type Data: [] `eltType`
   :arg minlen: When the array size is less than `minlen` use insertion sort algorithm
   :type minlen: `integral`
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: `bool`
   :arg reverse: Sort in reverse numerical order
   :type reverse: `bool`
   :arg comparator: Record that redefines the comparison mechanism with one of
      the methods: ``comparator.key(a)`` or ``comparator.compare(a,b)``

 */
proc QuickSort(Data: [?Dom] ?eltType, minlen=16, doublecheck=false, param reverse=false, comparator:?rec=emptyRecord) where Dom.rank == 1  && isRecord(rec) {
  chpl_check_comparator(comparator, Data(Dom.dim(1).low));
  // grab obvious indices
  const lo = Dom.low,
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
    InsertionSort(Data, reverse=reverse, comparator=comparator);
    return;
  }

  // find pivot using median-of-3 method
  if (chpl_compare(Data(mid), Data(lo), reverse, comparator) < 0) then
    Data(mid) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(lo), reverse, comparator) < 0) then
    Data(hi) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(mid), reverse, comparator) < 0) then
    Data(hi) <=> Data(mid);

  const pivotVal = Data(mid);
  Data(mid) = Data(hi-1);
  Data(hi-1) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo,
      hiptr = hi-1;
  while (loptr < hiptr) {
    do { loptr += 1; } while (chpl_compare(Data(loptr), pivotVal, reverse, comparator) < 0);
    do { hiptr -= 1; } while (chpl_compare(pivotVal, Data(hiptr), reverse, comparator) < 0);
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-1) = Data(loptr);
  Data(loptr) = pivotVal;

  //  cobegin {
    QuickSort(Data[..loptr-1], reverse=reverse, comparator=comparator);  // could use unbounded ranges here
    QuickSort(Data[loptr+1..], reverse=reverse, comparator=comparator);
    //  }

  if (doublecheck) then VerifySort(Data, "QuickSort", reverse, comparator);
}


// TODO -- support comparators
/*
   Sort the 1D array `Data` in-place using a sequential selection sort
   algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg doublecheck: Verify the array is correctly sorted before returning
   :type doublecheck: `bool`
   :arg reverse: Sort in reverse numerical order
   :type reverse: `bool`

 */
proc SelectionSort(Data: [?Dom] ?eltType, doublecheck=false, param reverse=false) where Dom.rank == 1 {
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
   :type Data: [] `eltType`
   :arg str: string to print while halting if an element is out of order
   :type str: `string`
   :arg reverse: if true, expect the values to be sorted in reverse.
   :type reverse: `bool`
   :arg comparator: Record that redefines the comparison mechanism with one of
      the methods: ``comparator.key(a)`` or ``comparator.compare(a,b)``

 */
inline proc VerifySort(Data: [?Dom] ?eltType, str: string, param reverse=false, comparator:?rec=emptyRecord) where isRecord(rec) {
  chpl_check_comparator(comparator, Data(Dom.dim(1).low));
  for i in Dom.low..Dom.high-1 do
    if chpl_compare(Data(i+1), Data(i), reverse, comparator) < 0 then
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
// TODO -- support comparators
/*
   Yield the elements of argument `x` in sorted order.

   :arg x: An iterable value to be sorted and yielded element by element
   :type x: `iterable`

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
