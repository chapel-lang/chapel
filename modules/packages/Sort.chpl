/*
 * Copyright 2004-2018 Cray Inc.
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


// TODO -- performance test sort routines and optimize (see other TODO's)
/*

The Sort module is designed to support standard sort routines.

.. _comparators:

Comparators
-----------

Comparators allow sorting data by a mechanism other than the
default comparison operations between array elements. To use a comparator,
define a record with either a ``key(a)`` or ``compare(a, b)`` method, and pass
an instance of that record to the sort function (examples shown below).

If both methods are implemented on the record passed as the comparator, the
``key(a)`` method will take priority over the ``compare(a, b)`` method.

Key Comparator
~~~~~~~~~~~~~~

The ``key(a)`` method accepts 1 argument, which will be an element from the
array being sorted.

The default key method would look like this:

.. code-block:: chapel

  proc DefaultComparator.key(a) {
    return a;
  }


As an example, if the user wants to sort an array by the absolute values of its
elements, the user can define a comparator with a key method as follows:

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record Comparator { }

  // key method maps an element to the value to be used for comparison
  proc Comparator.key(a) { return abs(a); }

  var absComparator: Comparator;

  sort(Array, comparator=absComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);

The return type of ``key(a)`` must support the ``<``
operator, which is used by the base compare method of all sort routines. If the
``<`` operator is not defined for the return type, the user may define it
themselves like so:

.. code-block:: chapel

  proc op<(a: returnType, b: returnType): bool {
    ...
  }


Compare Comparator
~~~~~~~~~~~~~~~~~~

The ``compare(a, b)`` method accepts 2 arguments, which will be 2 elements from
the array being sorted. The return value should be a numeric signed type
indicating how a and b compare to each other. The conditions between ``a`` and
``b`` should result in the following return values for ``compare(a, b)``:

  ============ ==========
  Return Value Condition
  ============ ==========
  ``> 0``      ``a > b``
  ``0``        ``a == b``
  ``< 0``      ``a < b``
  ============ ==========

The default compare method for a numeric signed type would look like this:

.. code-block:: chapel

    proc DefaultComparator.compare(a, b) {
      return a - b;
    }


The absolute value comparison example from above can alternatively be
implemented with a compare method:

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record Comparator { }

  // compare method defines how 2 elements are compared
  proc Comparator.compare(a, b) {
    return abs(a) - abs(b);
  }

  var absComparator: Comparator;

  sort(Array, comparator=absComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);

.. _reverse-comparator:

Reverse Comparator
~~~~~~~~~~~~~~~~~~

Sort functions in Chapel do not have a ``reverse`` argument. Instead, reverse
sorting is handled through the comparator interface.

A module-defined :const:`reverseComparator` can be passed to a sort function to
reverse the default sorting order.

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Using module-defined 'reverseComparator'
  sort(Array, comparator=reverseComparator)

  // This will output: 3, 2, -1, -4
  writeln(Array);


To reverse the sort order of a user-defined comparator, pass the user-defined
comparator to the constructor of the module-defined
:record:`ReverseComparator` record, which can be passed to the sort function.

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record Comparator { }

  // compare method defines how 2 elements are compared
  proc Comparator.compare(a, b) {
    return abs(a) - abs(b);
  }

  var absReverseComparator: ReverseComparator(Comparator);

  sort(Array, comparator=absReverseComparator);

  // This will output: -4, 3, 2, -1
  writeln(Array);

 */
module Sort {


/* Module-defined comparators */

/*
  Instance of :record:`DefaultComparator` used as default ``comparator=``
  argument when no comparator is passed to a sort function
*/
const defaultComparator: DefaultComparator;


/*
   Instance of :record:`ReverseComparator`. Pass this as the ``comparator=``
   argument of a sort function to reverse the sort order.
 */
const reverseComparator: ReverseComparator(DefaultComparator);


/* Private methods */

pragma "no doc"
/*
   Base compare method of all sort functions.

   By default, it returns the value of defaultComparator.compare(a, b).

   If a comparator with a key method is passed, it will return the value of
   defaultComparator(comparator.key(a), comparator.key(b)).

   If a comparator with a compare method is passed, it will return the value of
   comparator.compare(a, b).

   Return values conventions:

     a < b : returns value < 0
     a > b : returns value > 0
     a == b: returns 0
*/
inline proc chpl_compare(a, b, comparator:?rec=defaultComparator) {
  use Reflection;

  // TODO -- In cases where values are larger than keys, it may be faster to
  //         key data once and sort the keyed data, mirroring swaps in data.
  // Compare results of comparator.key(a) if is defined by user
  if canResolveMethod(comparator, "key", a) && canResolveMethod(comparator, "key", b) {
    return chpl_compare(comparator.key(a), comparator.key(b));
  // Use comparator.compare(a, b) if is defined by user
  } else if canResolveMethod(comparator, "compare", a, b) {
    return comparator.compare(a ,b);
  } else {
    compilerError("The comparator record requires a 'key(a)' or 'compare(a, b)' method");
  }
}


pragma "no doc"
/*
    Check if a comparator was passed and confirm that it will work, otherwise
    throw a compile-time error.

   :arg a: Sample data passed to confirm that comparator methods can resolve
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc chpl_check_comparator(comparator, type eltType) {
  use Reflection;

  // Dummy data for checking method resolution
  // This may need updating when constructors support non-default args
  const data: eltType;

  if comparator.type == DefaultComparator {}
  // Check for valid comparator methods
  else if canResolveMethod(comparator, "key", data) {
    // Check return type of key
    const keydata = comparator.key(data);
    type keytype = keydata.type;
    if !(canResolve("<", keydata, keydata)) then
      compilerError("The key method must return an object that supports the '<' function");
  }
  else if canResolveMethod(comparator, "compare", data, data) {
    // Check return type of compare
    type comparetype = comparator.compare(data, data).type;
    if !(isNumericType(comparetype)) then
      compilerError("The compare method must return a numeric type");
  }
  else {
    // If we make it this far, the passed comparator was defined incorrectly
    compilerError("The comparator record requires a 'key(a)' or 'compare(a, b)' method");
  }
}


/* Basic Functions */

/*
   General purpose sorting interface.

   .. note:: Currently this method calls a sequential :proc:`quickSort`, but
             this may change the future as other algorithms are implemented.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc sort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  quickSort(Data, comparator=comparator);
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc sort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("sort() requires 1-D array");
}


/*
   Check if array `Data` is in sorted order

   :arg Data: The array to verify
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :returns: ``true`` if array is sorted
   :rtype: `bool`
 */
proc isSorted(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator): bool {
  chpl_check_comparator(comparator, eltType);
  const stride = if Dom.stridable then abs(Dom.stride) else 1;

  for i in Dom.low..Dom.high-stride by stride do
    if chpl_compare(Data[i+stride], Data[i], comparator) < 0 then
      return false;
  return true;
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc isSorted(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("isSorted() requires 1-D array");
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
// TODO - Make standalone or leader/follower parallel iterator
/*
   Yield the elements of argument `x` in sorted order, using sort
   algorithm.

   .. note:

      This is currently implemented as a serial iterator, but will eventually
      support parallel iteration.

   :arg x: An iterable value to be sorted and yielded element by element
   :type x: `iterable`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   :yields: The elements of x in sorted order
   :ytype: x's element type

 */
iter sorted(x, comparator:?rec=defaultComparator) {
  var y = x;
  quickSort(y, comparator=comparator);
  for i in y do
    yield i;
}


/* Sort Functions */

/*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc bubbleSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high,
        stride = abs(Dom.stride);

  var swapped = true;

  while (swapped) {
    swapped = false;
    for i in low..high-stride by stride {
      if chpl_compare(Data(i), Data(i+stride), comparator) > 0 {
        Data(i) <=> Data(i+stride);
        swapped = true;
      }
    }
  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc bubbleSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("bubbleSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc heapSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high,
        size = Dom.size,
        stride = abs(Dom.stride);

  // Heapify
  var start = if high == low then high
              else if size % 2 then low + ((size - 1)/2) * stride
              else low + (size/2 - 1) * stride;

  while (start >= low) {
    SiftDown(start, high, comparator);
    start = start - stride;
  }

  // Sort, moving max element to end and re-heapifying the rest
  var end = high;
  while (end > low) {
    Data(end) <=> Data(low);
    end = end - stride;
    SiftDown(low, end, comparator);
  }

  proc SiftDown(start, end, comparator:?rec=defaultComparator) {
    var root = start;
    while ((2*root - low + stride) <= end) {
      const child = 2*root - low + stride;
      var swap = root;

      if chpl_compare(Data(swap), Data(child), comparator) < 0 then
        swap = child;

      if (child + stride <= end) && (chpl_compare(Data(swap), Data(child + stride), comparator) < 0) then
        swap = child + stride;

      if swap != root {
        Data(root) <=> Data(swap);
        root = swap;
      } else {
        return;
      }
    }
  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc heapSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("heapSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential insertion sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc insertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high,
        stride = abs(Dom.stride);

  for i in low..high by stride {
    var ithVal = Data[i];
    var inserted = false;
    for j in low..i-stride by -stride {
      if chpl_compare(ithVal, Data[j], comparator) < 0 {
        Data[j+stride] = Data[j];
      } else {
        Data[j+stride] = ithVal;
        inserted = true;
        break;
      }
    }
    if (!inserted) {
      Data[low] = ithVal;
    }
  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc insertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("insertionSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential, stable binary insertion sort algorithm.
   Should be used when there is a high cost of comparison.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high,
        stride = abs(Dom.stride);

  for i in low..high by stride {
    var valToInsert = Data[i],
        lo = low,
        hi = i - stride; 

    var (found, loc) = _binarySearchForLastOccurrence(Data, valToInsert, comparator, lo, hi);
    loc = if found then loc + stride else loc;              // insert after last occurrence if exists; else insert after expected location

    for j in loc..i-stride by -stride {
      // backward swap until loc
      Data[j + stride] = Data[j];
    }

    Data[loc] = valToInsert;
  }
}

/*
  Binary searches for the index of the last occurrence of `val` in the 1D array `Data` based on a comparator.
  If `val` is not in `Data`, the index that it should be inserted at is returned.
  Does not check for a valid comparator.
*/
private proc _binarySearchForLastOccurrence(Data: [?Dom], val, comparator:?rec=defaultComparator, in lo=Dom.low, in hi=Dom.high) {
  const stride = if Dom.stridable then abs(Dom.stride) else 1;

  var loc = -1;                                        // index of the last occurrence of val in Data

  while (lo <= hi) {
    const size = (hi - lo) / stride,
          mid = lo + (size/2) * stride;

    if chpl_compare(val, Data[mid], comparator) == 0 {
        loc = mid;                                    // index of last occurrence of val in 1..mid
        lo = loc + stride;
    }
    else if chpl_compare(val, Data[mid], comparator) > 0 then
      lo = mid + stride;
    else
      hi = mid - stride;
  }

  if loc == -1 then return (false, lo);              // returns index where val should be
  return (true, loc);                                // returns index of the last occurrence of val
}

pragma "no doc"
/* Error message for multi-dimension arrays */
proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("binaryInsertionSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a parallel merge sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
   :type minlen: `integral`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc mergeSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  _MergeSort(Data, minlen, comparator);
}

private proc _MergeSort(Data: [?Dom], minlen=16, comparator:?rec=defaultComparator)
  where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  if hi-lo < minlen {
    insertionSort(Data, comparator);
    return;
  }
  const mid = (hi-lo)/2+lo;
  var A1 = Data[lo..mid];
  var A2 = Data[mid+1..hi];
  cobegin {
    { _MergeSort(A1, minlen, comparator); }
    { _MergeSort(A2, minlen, comparator); }
  }

  // TODO -- This iterator causes unnecessary overhead - we can do without it
  for (a, _a) in zip(Data[lo..hi], _MergeIterator(A1, A2, comparator=comparator)) do a = _a;
}


private iter _MergeIterator(A1: [] ?eltType, A2: [] eltType, comparator:?rec=defaultComparator) {
  var a1 = A1.domain.dim(1).low;
  const a1hi = A1.domain.dim(1).high;
  var a2 = A2.domain.dim(1).low;
  const a2hi = A2.domain.dim(1).high;
  while ((a1 <= a1hi) && (a2 <= a2hi)) {
    while (chpl_compare(A1(a1), A2(a2), comparator) <= 0) {
      yield A1(a1);
      a1 += 1;
      if a1 > a1hi then break;
    }
    if a1 > a1hi then break;
    while (chpl_compare(A2(a2), A1(a1), comparator) <= 0) {
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


pragma "no doc"
/* Error message for multi-dimension arrays */
proc mergeSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("mergeSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential quick sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
   :type minlen: `integral`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc quickSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  // grab obvious indices
  const stride = abs(Dom.stride),
        lo = Dom.low,
        hi = Dom.high,
        size = Dom.size,
        mid = if hi == lo then hi
              else if size % 2 then lo + ((size - 1)/2) * stride
              else lo + (size/2 - 1) * stride;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
    insertionSort(Data, comparator=comparator);
    return;
  }

  // find pivot using median-of-3 method
  if (chpl_compare(Data(mid), Data(lo), comparator) < 0) then
    Data(mid) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(lo), comparator) < 0) then
    Data(hi) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(mid), comparator) < 0) then
    Data(hi) <=> Data(mid);

  const pivotVal = Data(mid);
  Data(mid) = Data(hi-stride);
  Data(hi-stride) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo,
      hiptr = hi-stride;
  while (loptr < hiptr) {
    do { loptr += stride; } while (chpl_compare(Data(loptr), pivotVal, comparator) < 0);
    do { hiptr -= stride; } while (chpl_compare(pivotVal, Data(hiptr), comparator) < 0);
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-stride) = Data(loptr);
  Data(loptr) = pivotVal;

  // TODO -- Get this cobegin working and tested
  //  cobegin {
    quickSort(Data[..loptr-stride], minlen, comparator);  // could use unbounded ranges here
    quickSort(Data[loptr+stride..], minlen, comparator);
  //  }
}

pragma "no doc"
/* Non-stridable quickSort */
proc quickSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator)
  where !Dom.stridable {
  chpl_check_comparator(comparator, eltType);

  // grab obvious indices
  const lo = Dom.low,
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
    insertionSort(Data, comparator=comparator);
    return;
  }

  // find pivot using median-of-3 method
  if (chpl_compare(Data(mid), Data(lo), comparator) < 0) then
    Data(mid) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(lo), comparator) < 0) then
    Data(hi) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(mid), comparator) < 0) then
    Data(hi) <=> Data(mid);

  const pivotVal = Data(mid);
  Data(mid) = Data(hi-1);
  Data(hi-1) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo,
      hiptr = hi-1;
  while (loptr < hiptr) {
    do { loptr += 1; } while (chpl_compare(Data(loptr), pivotVal, comparator) < 0);
    do { hiptr -= 1; } while (chpl_compare(pivotVal, Data(hiptr), comparator) < 0);
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-1) = Data(loptr);
  Data(loptr) = pivotVal;

  // TODO -- Get this cobegin working and tested
  //  cobegin {
    quickSort(Data[..loptr-1], minlen, comparator);  // could use unbounded ranges here
    quickSort(Data[loptr+1..], minlen, comparator);
  //  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc quickSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("quickSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential selection sort
   algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc selectionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
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


pragma "no doc"
/* Error message for multi-dimension arrays */
proc selectionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("selectionSort() requires 1-D array");
}


/* Comparators */

/* Default comparator used in sort functions.*/
record DefaultComparator {

  /*
   Default compare method used in sort functions.

   :arg a: Array element
   :type a: `eltType`
   :arg b: Array element
   :type b: `eltType`
   :returns: 1 if ``b < a``
   :returns: 0 if ``a == b``
   :returns: -1 if ``a < b``

   */
  proc compare(a, b) {
    if a < b { return -1; }
    else if b < a { return 1; }
    else return 0;
  }
}

/* Reverse comparator built from another comparator.*/
record ReverseComparator {

  /* Generic comparator defined in constructor.*/
  var comparator;

  /*
   Initializer - builds a comparator with a compare method that
   reverses the sort order of the default comparator.
   */
  proc init() {
    this.comparator = defaultComparator;
    super.init();
  }

  /*
   Initializer - builds a comparator with a compare method that
   reverses the sort order of the argument-provided comparator.

   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc init(comparator) {
    this.comparator = comparator;
    super.init();
  }

  /*
   Copy Initializer - builds a comparator that's a copy of
   its argument.

   :arg revcomp: :ref:`ReverseComparator <reverse-comparator>` to copy.
   */
  pragma "no doc"
  proc init(revcomp: ReverseComparator(?)) {
    this.comparator = revcomp.comparator;
    super.init();
  }

  /*
   Reversed compare method defined based on ``comparator.key`` if defined,
   otherwise ``comparator.compare``.

   :arg a: Array element
   :type a: `eltType`
   :arg b: Array element
   :type b: `eltType`
   :returns: -1 if ``b < a``
   :returns: 0 if ``a == b``
   :returns: 1 if ``a < b``
   */
  proc compare(a, b) {
    use Reflection;

    chpl_check_comparator(this.comparator, a.type);

    // Key defined
    if canResolveMethod(this.comparator, "key", a) && canResolveMethod(this.comparator, "key", b) {
      const A = this.comparator.key(a),
            B = this.comparator.key(b);
      if B < A { return -1; }
      else if A < B { return 1; }
      else return 0;

    // Compare defined
    } else if canResolveMethod(this.comparator, "compare", a, b) && canResolveMethod(this.comparator, "compare", a, b) {
      return this.comparator.compare(b, a);
    } else {
      compilerError("The comparator record requires a 'key(a)' or 'compare(a, b)' method");
    }
  }
}
} // Sort Module
