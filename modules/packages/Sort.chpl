/*
 * Copyright 2004-2019 Cray Inc.
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
define a record or a class with an appropriate method and then pass
an instance of it to the sort function. Examples are shown below.

Comparators need to include at least one of the following methods:

 * ``key(a)``  -- see `The .key method`_
 * ``compare(a, b)``  -- see `The .compare method`_
 * ``keyPart(a, i)`` -- see `The .keyPart method`_

See the section below for discussion of each of these methods.

A comparator can contain both ``compare`` and ``keyPart`` methods. In that
event, the sort algorithm will use whichever is appropriate for the algorithm
and expect that they have consistent results.

It is an error for a comparator to contain a ``key`` method as well as one of
the other methods.

The .key method
~~~~~~~~~~~~~~~

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


The .compare method
~~~~~~~~~~~~~~~~~~~

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

The default compare method for a signed integral type can look like this:

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

The .keyPart method
~~~~~~~~~~~~~~~~~~~

A ``keyPart(a, i)`` method returns *parts* of key value at a time. This
interface supports radix sorting for variable length data types, such as
strings. It accepts two arguments:

 * ``a`` is the element being sorted
 * ``i`` is the part number of the key requested, starting from 1

A ``keyPart`` method should return a tuple consisting of *section* and a *part*.

 * The *section* can be any signed integral type and should have the value `-1`,
   `0`, or `1`. It indicates when the end of the ``a`` has been reached
   and in that event how it should be sorted relative to other array elements.

   ================ ====================================
   Returned section Interpretation
   ================ ====================================
   ``-1``           no more key parts for ``a``,
                    sort it before those with more parts

   ``0``            a key part for ``a`` is returned in
                    the second tuple element

   ``1``            no more key parts for ``a``,
                    sort it after those with more parts
   ================ ====================================

 * The *part* can be any signed or unsigned integral type and can contain any
   value. The *part* will be ignored unless the *section* returned is ``0``.


Let's consider several example ``keyPart`` methods. All of these are
simplifications of ``keyPart`` methods already available in the
``DefaultComparator``.

This ``keyPart`` method supports sorting tuples of 2 integers:

.. code-block:: chapel

  proc keyPart(x:2*int, i:int) {
    if i > 2 then
      return (-1, 0);

    return (0, x(i));
  }


Here is a ``keyPart`` to support sorting of strings:

.. code-block:: chapel

  proc keyPart(x:string, i:int):(int(8), uint(8)) {
    var len = x.length;
    var section = if i <= len then 0:int(8) else -1:int(8);
    var part =    if i <= len then x.byte(i) else  0:uint(8);
    return (section, part);
  }


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
comparator to the initializer of the module-defined
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

private inline
proc compareByPart(a:?t, b:t, comparator:?rec) {
  var curPart = 1;
  while true {
    var (aSection, aPart) = comparator.keyPart(a, curPart);
    var (bSection, bPart) = comparator.keyPart(b, curPart);
    if aSection != 0 || bSection != 0 {
      return aSection - bSection;
    }
    if aPart < bPart {
      return -1;
    }
    if aPart > bPart {
      return 1;
    }

    curPart += 1;
  }

  // This is never reached. The return below is a workaround for issue #10447.
  return 1;
}

/*
   Base compare method of all sort functions.

   By default, it returns the value of defaultComparator.compare(a, b).

   If a comparator with a key method is passed, it will return the value of
   defaultComparator(comparator.key(a), comparator.key(b)).

   If a comparator with a compare method is passed, it will return the value of
   comparator.compare(a, b).

   Otherwise, if the comparator supports keyPart calls, it will
   use those to compare the elements.

   Return values conventions:

     a < b : returns value < 0
     a > b : returns value > 0
     a == b: returns 0
*/
pragma "no doc"
inline proc chpl_compare(a:?t, b:t, comparator:?rec) {
  use Reflection;

  // TODO -- In cases where values are larger than keys, it may be faster to
  //         key data once and sort the keyed data, mirroring swaps in data.
  // Compare results of comparator.key(a) if is defined by user
  if canResolveMethod(comparator, "key", a) {
    // Use the default comparator to compare the integer keys
    return defaultComparator.compare(comparator.key(a), comparator.key(b));
  // Use comparator.compare(a, b) if is defined by user
  } else if canResolveMethod(comparator, "compare", a, b) {
    return comparator.compare(a ,b);
  } else if canResolveMethod(comparator, "keyPart", a, 1) {
    return compareByPart(a, b, comparator);
  } else {
    compilerError("The comparator " + comparator.type:string + " requires a 'key(a)', 'compare(a, b)', or 'keyPart(a, i)' method");
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
proc chpl_check_comparator(comparator, type eltType) param {
  use Reflection;

  // Dummy data for checking method resolution
  // This may need updating when constructors support non-default args
  const data: eltType;

  param errorDepth = 2;

  if comparator.type == DefaultComparator {}
  // Check for valid comparator methods
  else if canResolveMethod(comparator, "key", data) {
    // Check return type of key
    const keydata = comparator.key(data);
    type keytype = keydata.type;
    if !(canResolve("<", keydata, keydata)) then
      compilerError(errorDepth=errorDepth, "The key method in ", comparator.type:string, " must return an object that supports the '<' function when used with ", eltType:string, " elements");

    // Check that there isn't also a compare or keyPart
    if canResolveMethod(comparator, "compare", data, data) {
      compilerError(errorDepth=errorDepth, comparator.type:string, " contains both a key method and a compare method");
    }
    if canResolveMethod(comparator, "keyPart", data, 1) {
      compilerError(errorDepth=errorDepth, comparator.type:string, " contains both a key method and a keyPart method");
    }
  }
  else if canResolveMethod(comparator, "compare", data, data) {
    // Check return type of compare
    type comparetype = comparator.compare(data, data).type;
    if !(isNumericType(comparetype)) then
      compilerError(errorDepth=errorDepth, "The compare method in ", comparator.type:string, " must return a numeric type when used with ", eltType:string, " elements");
  }
  else if canResolveMethod(comparator, "keyPart", data, 1) {
    var idx: int = 1;
    type partType = comparator.keyPart(data, idx).type;
    if !isTupleType(partType) then
      compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple when used with ", eltType:string, " elements");
    var tmp: partType;
    var expectInt = tmp(1);
    var expectIntUint = tmp(2);
    if !isInt(expectInt.type) then
      compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with 1st element int(?) when used with ", eltType:string, " elements");
    if !(isInt(expectIntUint) || isUint(expectIntUint)) then
      compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with 2nd element int(?) or uint(?) when used with ", eltType:string, " elements");
  }
  else {
    // If we make it this far, the passed comparator was defined incorrectly
    compilerError(errorDepth=errorDepth, "The comparator " + comparator.type:string + " requires a 'key(a)', 'compare(a, b)', or 'keyPart(a, i)' method " + " for element type " + eltType:string );
  }

  return true;
}


/* Basic Functions */

private
proc radixSortOk(Data: [?Dom] ?eltType, comparator) param {
  use Reflection;

  if !Dom.stridable {
    var tmp:Data[Dom.low].type;
    if canResolveMethod(comparator, "keyPart", tmp, 1) {
      return true;
    } else if canResolveMethod(comparator, "key", tmp) {
      var key:comparator.key(tmp).type;
      // Does the defaultComparator have a keyPart for this?
      if canResolveMethod(defaultComparator, "keyPart", key, 1) then
        return true;
    }
  }
  return false;
}

/*

Sort the elements in an array. It is up to the implementation to choose
the sorting algorithm.

.. note::
  This function currently either uses a parallel radix sort or a serial
  quickSort. The algorithms used will change over time.

  It currently uses parallel radix sort if the following conditions are met:

    * the array being sorted is over a non-strided domain
    * ``comparator`` includes a ``keyPart`` method for ``eltType``
      or includes a ``key`` returning a value for which the default comparator
      includes a ``keyPart`` method

  Note that the default comparator includes ``keyPart`` methods for:

    * ``int``
    * tuples of ``int``
    * ``uint``
    * tuples of ``uint``
    * ``string``

:arg Data: The array to be sorted
:type Data: [] `eltType`
:arg comparator: :ref:`Comparator <comparators>` record that defines how the
  data is sorted.

 */
// TODO: This should have a flag `stable` to request a stable sort
proc sort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);

  if Dom.low >= Dom.high then
    return;

  if radixSortOk(Data, comparator) {
    msbRadixSort(Data, comparator=comparator);
  } else {
    quickSort(Data, comparator=comparator);
  }
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
  if !isArrayValue(y) then
    compilerError("Sort.sorted called on non-iterable");

  sort(y, comparator=comparator);
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
   Sort the 1D array `Data` using a parallel merge sort algorithm.

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

pragma "no doc"
proc shellSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator,
               start=Dom.low, end=Dom.high)
{
  chpl_check_comparator(comparator, eltType);

  if Dom.rank != 1 then
    compilerError("shellSort() requires 1-D array");
  if Dom.stridable then
    compilerError("shellSort() requires an array over a non-stridable domain");

  // Based on Sedgewick's Shell Sort -- see
  // Analysis of Shellsort and Related Algorithms 1996
  // and see Marcin Ciura - Best Increments for the Average Case of Shellsort
  // for the choice of these increments.
  var n = 1 + end - start;
  var js,hs:int;
  var v,tmp:Data.eltType;
  const incs = (701, 301, 132, 57, 23, 10, 4, 1);
  for h in incs {
    hs = h + start;
    for is in hs..end {
      v = Data[is];
      js = is;
      while js >= hs && chpl_compare(v,Data[js-h],comparator) < 0 {
        Data[js] = Data[js - h];
        js -= h;
      }
      Data[js] = v;
    }
  }
}

// This is the number of bits to sort at a time in the radix sorter.
// The code assumes that all integer types are a multiple of it.
// That would need to change if it were to increase.
//
// At the same time, using a value less than 8 will probably perform poorly.
private param RADIX_BITS = 8;

// This structure tracks configuration for the radix sorter.
pragma "no doc"
record MSBRadixSortSettings {
  param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step
  const sortSwitch = 256; // when sorting <= this many elements, use shell sort
  const minForTask = 256; // when sorting >= this many elements, go parallel
  param CHECK_SORTS = false; // do costly extra checks that data is sorted
  param progress = false; // print progress
  const alwaysSerial = false; // never create tasks
  const maxTasks = here.numPUs(logical=true); // maximum number of tasks to make
}

// Get the bin for a record by calling criterion.keyPart
//
// startbit is starting from 0
// bin 0 is for the end was reached (sort before)
// bins 1..256 are for data with next part 0..255.
// bin 256 is for the end was reached (sort after)
//
// ubits are the result of keyPart normalized to a uint.
//
// returns (bin, ubits)
private inline
proc binForRecordKeyPart(a, criterion, startbit:int)
{
  // We have keyPart(element, start):(section:int(8), part:int/uint)
  const testRet: criterion.keyPart(a, 1).type;
  const testPart = testRet(2);
  param bitsPerPart = numBits(testPart.type);
  param bitsPerPartModRadixBits = bitsPerPart % RADIX_BITS;
  if bitsPerPartModRadixBits != 0 then
    compilerError("part size must be a multiple of radix bits");
    // or else the implementation below would have to handle crossing parts

  // startbit must be a multiple of RADIX_BITS because the radix
  // sort operates RADIX_BITS at a time.

  // startbit might be partway through a part (e.g. 16 bits into a uint(64))
  const whichpart = startbit / bitsPerPart;
  const bitsinpart = startbit % bitsPerPart;

  const (section, part) = criterion.keyPart(a, 1+whichpart);
  var ubits = part:uint(bitsPerPart);
  // If the number is signed, invert the top bit, so that
  // the negative numbers sort below the positive numbers
  if isInt(part) {
    const one:ubits.type = 1;
    ubits = ubits ^ (one << (bitsPerPart - 1));
  }
  param mask:uint = (1 << RADIX_BITS) - 1;
  const ubin = (ubits >> (bitsPerPart - bitsinpart - RADIX_BITS)) & mask;

  if section == 0 then
    return (ubin:int + 1, ubits);
  else if section < 0 then
    return (0, ubits);
  else
    return ((1 << RADIX_BITS) + 1, ubits);
}

// Get the bin for a record with criterion.key or criterion.keyPart
//
// See binForRecordKeyPart for what the arguments / returns mean.
private inline
proc binForRecord(a, criterion, startbit:int)
{
  use Reflection;

  if canResolveMethod(criterion, "keyPart", a, 1) {
    return binForRecordKeyPart(a, criterion, startbit);
  } else if canResolveMethod(criterion, "key", a) {
    // Try to use the default comparator to get a keyPart.
    return binForRecordKeyPart(criterion.key(a),
                               defaultComparator,
                               startbit);
  } else {
    compilerError("Bad comparator for radix sort ", criterion.type:string,
                  " with eltType ", a.type:string);
  }
}

// Returns the fixed number of bits in a value, if known.
// Returns -1 otherwise.
private
proc fixedWidth(type eltTy) param {
  if (isUintType(eltTy) || isIntType(eltTy) ||
      isRealType(eltTy) || isImagType(eltTy)) then
    return numBits(eltTy);

  if (isHomogeneousTuple(eltTy)) {
    var tmp:eltTy;
    return tmp.size * numBits(tmp(1).type);
  }

  return -1;
}

// Returns a compile-time known final startbit
// e.g. for uint(64), returns 56 (since that's 64-8 and the
// last sort pass will sort on the last 8 bits).
//
// Returns -1 if no such ending is known at compile-time.
private
proc msbRadixSortParamLastStartBit(Data:[], comparator) param {
  use Reflection;

  // Compute end_bit if it's known
  // Default comparator on integers has fixed width
  const ref element = Data[Data.domain.low];
  if comparator.type == DefaultComparator && fixedWidth(element.type) > 0 {
    return fixedWidth(element.type) - RADIX_BITS;
  } else if canResolveMethod(comparator, "key", element) {
    type keyType = comparator.key(element).type;
    if fixedWidth(keyType) > 0 then
      return fixedWidth(keyType) - RADIX_BITS;
  }

  return -1;
}

pragma "no doc"
proc msbRadixSortClz(val) {
  // This could use BitOps.clz but that adds new
  // module dependencies that confuse testing.
  // Since it's not performance critical, here we
  // have a version using a while loop.
  param nBits = numBits(val.type);
  if val == 0 {
    return nBits;
  }

  var cur = val;
  var one = 1:val.type;
  var hi = one << (nBits - 1);
  var n = 0;
  while (cur & hi) == 0 {
    n += 1;
    cur <<= 1;
  }
  return n;
}

// Compute the startbit location that could be used based on the
// min/max of values returned by keyPart.
private
proc findDataStartBit(startbit:int, min_ubits, max_ubits):int {
  var xor = min_ubits ^ max_ubits;

  // Clear the top bits in xor if they are after bitsinpart
  param bitsPerPart = numBits(min_ubits.type);
  const bitsinpart = startbit % bitsPerPart;
  xor <<= bitsinpart;
  xor >>= bitsinpart;

  var new_start = msbRadixSortClz(xor);
  var new_digit = new_start / RADIX_BITS;
  var new_start_bit_rounded = new_digit * RADIX_BITS;

  return new_start_bit_rounded:int;
}

pragma "no doc"
proc msbRadixSort(Data:[], comparator:?rec=defaultComparator) {

  var endbit:int;
  endbit = msbRadixSortParamLastStartBit(Data, comparator);
  if endbit < 0 then
    endbit = max(int);

  msbRadixSort(start_n=Data.domain.low, end_n=Data.domain.high,
               Data, comparator,
               startbit=0, endbit=endbit,
               settings=new MSBRadixSortSettings());
}

// startbit counts from 0 and is a multiple of RADIX_BITS
pragma "no doc"
proc msbRadixSort(start_n:int, end_n:int, A:[], criterion,
                  startbit:int, endbit:int,
                  settings /* MSBRadixSortSettings */)
{
  if startbit > endbit then
    return;

  if( end_n - start_n < settings.sortSwitch ) {
    shellSort(A, criterion, start=start_n, end=end_n);
    if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
    return;
  }

  if settings.progress then writeln("radix sort start=", start_n, " end=", end_n, " startbit=", startbit, " endbit=", endbit);

  const radixbits = RADIX_BITS;
  const radix = (1 << radixbits) + 1;

  // 0th bin is for records where we've consumed all the key.
  var offsets:[0..radix] int;
  var end_offsets:[0..radix] int;
  type ubitsType = binForRecord(A[start_n], criterion, startbit)(2).type;
  var min_ubits: ubitsType = max(ubitsType);
  var max_ubits: ubitsType = 0;
  var min_bin = radix+1;
  var max_bin = 0;
  var any_ending = false;

  // Step 1: count.
  if settings.alwaysSerial == false {
    forall i in start_n..end_n
      with (+ reduce offsets,
            min reduce min_ubits,
            max reduce max_ubits,
            || reduce any_ending) {
      const (bin, ubits) = binForRecord(A[i], criterion, startbit);
      if ubits < min_ubits then
        min_ubits = ubits;
      if ubits > max_ubits then
        max_ubits = ubits;
      if bin == 0 || bin == radix then
        any_ending = true;
      offsets[bin] += 1;
    }
  } else {
    // The serial version
    for i in start_n..end_n {
      const (bin, ubits) = binForRecord(A[i], criterion, startbit);
      if ubits < min_ubits then
        min_ubits = ubits;
      if ubits > max_ubits then
        max_ubits = ubits;
      if bin == 0 || bin == radix then
        any_ending = true;
      offsets[bin] += 1;
    }
  }

  // If the data parts we gathered all have the same leading bits,
  // we might be able to skip ahead immediately to the next count step.
  if any_ending == false {
    var dataStartBit = findDataStartBit(startbit, min_ubits, max_ubits);
    if dataStartBit > startbit {
      // Re-start count again immediately at the new start position.
      msbRadixSort(start_n, end_n, A, criterion,
                   dataStartBit, endbit, settings);
      return;
    }
  }

  if settings.progress then writeln("accumulate");

  // Step 2: accumulate
  var sum = 0;
  for (off,end) in zip(offsets,end_offsets) {
    var binstart = sum;
    sum += off;
    var binend = sum;
    off = start_n + binstart;
    end = start_n + binend;
  }

  var curbin = 0;

  if settings.progress then writeln("shuffle");

  // Step 3: shuffle
  while true {
    // Find the next bin that isn't totally in place.
    while curbin <= radix && offsets[curbin] == end_offsets[curbin] {
      curbin += 1;
    }
    if curbin > radix {
      break;
    }

    param max_buf = settings.DISTRIBUTE_BUFFER;
    var buf: max_buf*A.eltType;
    var used_buf = 0;
    var end = end_offsets[curbin];
    var endfast = max(offsets[curbin], end_offsets[curbin]-2*max_buf);
    var bufstart = max(offsets[curbin], end_offsets[curbin]-max_buf);
    var i = bufstart;

    // Fill buf with up to max_buf records from the end of this bin.
    while i < end {
      buf[used_buf+1] <=> A[i];
      used_buf += 1;
      i += 1;
    }

    while offsets[curbin] < endfast {
      // Now go through the records in buf
      // putting them in their right home.
      for param j in 1..max_buf {
        const (bin, _) = binForRecord(buf[j], criterion, startbit);
        // prefetch(A[offsets[bin]]) could be here but doesn't help

        // Swap buf[j] into its appropriate bin.
        // Leave buf[j] with the next unsorted item.
        A[offsets[bin]] <=> buf[j];
        offsets[bin] += 1;
      }
    }
    // Now, handle elements in bufstart...end_offsets[cur_bin]
    while offsets[curbin] < end {
      // Put buf[j] into its right home
      var j = 1;
      while used_buf > 0 && j <= used_buf {
        const (bin, _) = binForRecord(buf[j], criterion, startbit);
        // Swap buf[j] into its appropriate bin.
        var offset = offsets[bin];
        A[offset] <=> buf[j];
        offsets[bin] += 1;
        // Leave buf[j] with the next unsorted item.
        // But offsets[bin] might be in the region we already read.
        if bin == curbin && offset >= bufstart {
          buf[j] <=> buf[used_buf];
          used_buf -= 1;
        }
        j += 1;
      }
    }
  }

  if settings.progress then writeln("sort sub-problems");

  // Step 4: sort sub-problems.
  // Note that shuffle changed the offsets to be == end_offset..
  // put offsets back.
  offsets[0] = start_n;
  for i in 1..radix {
    offsets[i] = end_offsets[i-1];
  }

  // This is a parallel version
  if settings.alwaysSerial == false {
    const subbits = startbit + radixbits;
    var nbigsubs = 0;
    var bigsubs:[0..radix] (int,int);
    const runningNow = here.runningTasks();

    // Never recursively sort the first or last bins
    // (these store the end)

    for bin in 1..radix-1 {
      // Does the bin contain more than one record?
      const bin_start = offsets[bin];
      const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
      const num = 1 + bin_end - bin_start;
      if num <= 1 || startbit >= endbit {
        // do nothing
      } else if num < settings.minForTask || runningNow >= settings.maxTasks {
        // sort it in this thread
        msbRadixSort(bin_start, bin_end, A, criterion,
                     subbits, endbit, settings);
      } else {
        // Add it to the list of things to do in parallel
        bigsubs[nbigsubs] = (bin_start, bin_end);
        nbigsubs += 1;
      }
    }

    forall (bin,(bin_start,bin_end)) in zip(0..#nbigsubs,bigsubs) {
      msbRadixSort(bin_start, bin_end, A, criterion, subbits, endbit, settings);
    }
  } else {
    // The serial version
    for bin in 1..radix-1 {
      // Does the bin contain more than one record?
      const bin_start = offsets[bin];
      const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
      const num = 1 + bin_end - bin_start;
      if num <= 1 || startbit >= endbit {
        // do nothing
      } else {
        // sort it in this thread
        msbRadixSort(bin_start, bin_end, A, criterion,
                     startbit + radixbits, endbit, settings);
      }
    }
  }

  if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
}

// Check that the elements from start_n..end_n in A are sorted by criterion
private
proc checkSorted(start_n:int, end_n:int, A:[], criterion, startbit = 0)
{
  for i in start_n+1..end_n {
    var cmp = chpl_compare(A[i-1], A[i], criterion);
    if cmp > 0 {
      writeln("Error: not sorted properly at i=", i, " A[i-1]=", A[i-1], " A[i]=", A[i], " in start=", start_n, " end=", end_n);
      writeln(A);

      // Halt. Note, this is only intended to be called by unit testing.
      halt("failed checkSorted");
    }
  }
}

/* Comparators */

/* Default comparator used in sort functions.*/
record DefaultComparator {

  /*
   Default compare method used in sort functions.
   Uses the `<` operator to compute the ordering between ``a`` and ``b``.
   See also `The .compare method`_.

   :returns: 1 if ``b < a``
   :returns: 0 if ``a == b``
   :returns: -1 if ``a < b``
   */
  inline
  proc compare(a, b) {
    if a < b { return -1; }
    else if b < a { return 1; }
    else return 0;
  }

  /*
   Default ``keyPart`` method for integral values.
   See also `The .keyPart method`_.

   :arg x: the `int` or `uint` of any size to sort
   :arg i: the part number requested

   :returns: ``(0, x)`` if ``i==0``, or ``(-1, x)`` otherwise
   */
  inline
  proc keyPart(x: integral, i:int):(int(8), x.type) {
    var section:int(8) = if i > 1 then -1:int(8) else 0:int(8);
    return (section, x);
  }

  inline
  proc keyPart(x: chpl_anyreal, i:int):(int(8), uint(numBits(x.type))) {
    var section:int(8) = if i > 1 then -1:int(8) else 0:int(8);

    param nbits = numBits(x.type);
    // Convert the real bits to a uint
    var src = x;
    var dst: uint(nbits);
    c_memcpy(c_ptrTo(dst), c_ptrTo(src), c_sizeof(src.type));

    if (dst >> (nbits-1)) == 1 {
      // negative bit is set, flip all bits
      dst = ~dst;
    } else {
      const one: uint(nbits) = 1;
      // negative bit is not set, flip only top bit
      dst = dst ^ (one << (nbits-1));
    }
    return (section, dst);
  }
  inline
  proc keyPart(x: chpl_anyimag, i:int):(int(8), uint(numBits(x.type))) {
    return keyPart(x:real(numBits(x.type)), i);
  }

  /*
   Default ``keyPart`` method for tuples of integral values.
   See also `The .keyPart method`_.

   :arg x: tuple of the `int` or `uint` (of any bit width) to sort
   :arg i: the part number requested

   :returns: ``(0, x(i))`` if ``i <= x.size``, or ``(-1, 0)`` otherwise
   */
  inline
  proc keyPart(x: _tuple, i:int) where isHomogeneousTuple(x) &&
                                       (isInt(x(1)) || isUint(x(1))) {
    type tt = x(1).type;

    if i > x.size then
      return (-1, 0:tt);

    return (0, x(i):tt);
  }

  /*
   Default ``keyPart`` method for sorting strings. See also `The .keyPart method`_.

   .. note::
     Currently assumes that the string is local.

   :arg x: the string to sort
   :arg i: the part number requested

   :returns: ``(0, byte i of string)`` or ``(-1, 0)`` if ``i > x.size``
   */
  inline
  proc keyPart(x:string, i:int):(int(8), uint(8)) {
    // This assumes that the string is local, which should
    // be OK for the sort module (because the array containing
    // the string must currently be local).
    // In the future it should use bytes access into the string.
    if boundsChecking then
      assert(x.locale_id == here.id);

    var ptr = x.c_str():c_ptr(uint(8));
    var len = x.length;
    var section = if i <= len then 0:int(8) else -1:int(8);
    var part =    if i <= len then ptr[i-1] else  0:uint(8);
    return (section, part);
  }

  inline
  proc keyPart(x:c_string, i:int):(int(8), uint(8)) {
    var ptr = x:c_ptr(uint(8));
    var byte = ptr[i-1];
    var section = if byte != 0 then 0:int(8) else -1:int(8);
    var part = byte;
    return (section, part);
  }
}

/* Reverse comparator built from another comparator.*/
record ReverseComparator {

  /* Generic comparator defined in initializer.*/
  var comparator;

  /*
   Initializer - builds a comparator with a compare method that
   reverses the sort order of the default comparator.
   */
  proc init() {
    this.comparator = defaultComparator;
  }

  /*
   Initializer - builds a comparator with a compare method that
   reverses the sort order of the argument-provided comparator.

   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc init(comparator) {
    this.comparator = comparator;
  }

  /*
   Copy Initializer - builds a comparator that's a copy of
   its argument.

   :arg revcomp: :ref:`ReverseComparator <reverse-comparator>` to copy.
   */
  pragma "no doc"
  proc init=(revcomp: ReverseComparator(?)) {
    this.comparator = revcomp.comparator;
  }

  pragma "no doc"
  proc typeIsBitReversible(type t) param {
    if isHomogeneousTupleType(t) {
      var tmp:t;
      return typeIsBitReversible(tmp(1).type);
    }
    if isUintType(t) then
      return true;
    if isIntType(t) then
      return true;

    return false;
  }
  pragma "no doc"
  proc typeIsNegateReversible(type t) param {
    if isHomogeneousTupleType(t) {
      var tmp:t;
      return typeIsNegateReversible(tmp(1).type);
    }
    if isIntType(t) || isUintType(t) then
      // You might think that int(8) should have its sort order
      // reversed by negating it, but that runs into the problem
      // that -128 -> 128 which won't fit into an int(8).
      return false;
    if isNumericType(t) then
      return true;

    return false;
  }

  pragma "no doc"
  proc hasKeyPart(a) param {
    use Reflection;
    return canResolveMethod(this.comparator, "keyPart", a, 1);
  }
  pragma "no doc"
  proc hasKeyPartFromKey(a) param {
    use Reflection;
    if canResolveMethod(this.comparator, "key", a) {
      var key:comparator.key(a).type;
      // Does the defaultComparator have a keyPart for this?
      return canResolveMethod(defaultComparator, "keyPart", key, 1);
    }
    return false;
  }

  pragma "no doc"
  proc hasCompare(a,b) param {
    use Reflection;
    return canResolveMethod(this.comparator, "compare", a, b);
  }
  pragma "no doc"
  proc hasCompareFromKey(a) param {
    use Reflection;
    if canResolveMethod(this.comparator, "key", a) {
      var key:comparator.key(a).type;
      // Does the defaultComparator have a compare for this?
      return canResolveMethod(defaultComparator, "compare", key, key);
    }
    return false;
  }

  pragma "no doc"
  inline
  proc getKeyPart(cmp, a, i) {
    var (section, part) = cmp.keyPart(a, i);
    if typeIsBitReversible(part.type) {
      return (-section, ~part);
    } else if typeIsNegateReversible(part.type) {
      return (-section, -part);
    } else {
      compilerError("keyPart must return int or uint");
    }
  }

  /*
   Reverses ``comparator.keyPart``. See also `The .keyPart method`_.
   */
  inline
  proc keyPart(a, i) where hasKeyPart(a) || hasKeyPartFromKey(a) {
    chpl_check_comparator(this.comparator, a.type);

    if hasKeyPartFromKey(a) {
      return getKeyPart(defaultComparator, this.comparator.key(a), i);
    } else {
      return getKeyPart(this.comparator, a, i);
    }

  }

  pragma "no doc"
  inline
  proc doCompare(cmp, a, b) {
    return cmp.compare(b, a);
  }

  /*
   Reverses ``comparator.compare``. See also `The .compare method`_.
   */
  inline
  proc compare(a, b) where hasCompare(a, b) || hasCompareFromKey(a) {

    chpl_check_comparator(this.comparator, a.type);

    if hasCompareFromKey(a) {
      return doCompare(defaultComparator,
                       this.comparator.key(a),
                       this.comparator.key(b));
    } else {
      return doCompare(this.comparator, a, b);
    }
  }
}


} // Sort Module
