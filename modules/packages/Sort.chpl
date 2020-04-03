/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
    var len = x.numBytes;
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

  private use List;
  private use Reflection;

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
pragma "unsafe" // due to 'data' default-initialized to nil for class types
/*
    Check if a comparator was passed and confirm that it will work, otherwise
    throw a compile-time error.

   :arg a: Sample data passed to confirm that comparator methods can resolve
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc chpl_check_comparator(comparator, type eltType) param {
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
    var expectInt = tmp(0);
    var expectIntUint = tmp(1);
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

pragma "unsafe" // due to 'tmp' default-initialized to nil for class types
private
proc radixSortOk(Data: [?Dom] ?eltType, comparator) param {
  if !Dom.stridable {
    var tmp:Data[Dom.alignedLow].type;
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
    * ``real``
    * tuples of ``real``
    * ``imag``
    * tuples of ``imag``
    * ``string``
    * ``c_string``

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
    MSBRadixSort.msbRadixSort(Data, comparator=comparator);
  } else {
    QuickSort.quickSort(Data, comparator=comparator);
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
  var sorted = true;
  forall (element, i) in zip(Data, Dom) with (&& reduce sorted) {
    if i > Dom.alignedLow {
      sorted &&= (chpl_compare(Data[i-stride], element, comparator) <= 0);
    }
  }
  return sorted;
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

pragma "no doc"
module BubbleSort {
  import Sort.defaultComparator;

  /*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc bubbleSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("bubbleSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
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
}

pragma "no doc"
module HeapSort {
  import Sort.defaultComparator;
  /*

   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc heapSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("heapSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
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
}

pragma "no doc"
module InsertionSort {
  import Sort.defaultComparator;
  /*
   Sort the 1D array `Data` in-place using a sequential insertion sort
   algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc insertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator, lo:int=Dom.alignedLow, hi:int=Dom.alignedHigh) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("insertionSort() requires 1-D array");
    }

    const low = lo,
          high = hi,
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
}

pragma "no doc"
module BinaryInsertionSort {
  import Sort.defaultComparator;
  /*
    Sort the 1D array `Data` in-place using a sequential, stable binary
    insertion sort algorithm.
    Should be used when there is a high cost of comparison.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.
   */
  proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("binaryInsertionSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
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
  private proc _binarySearchForLastOccurrence(Data: [?Dom], val, comparator:?rec=defaultComparator, in lo=Dom.alignedLow, in hi=Dom.alignedHigh) {
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
}

pragma "no doc"
module MergeSort {
  import Sort.defaultComparator;
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

    if Dom.rank != 1 {
      compilerError("mergeSort() requires 1-D array");
    }

    var Scratch: Data.type;

    _MergeSort(Data, Scratch, Dom.alignedLow, Dom.alignedHigh, minlen, comparator, 0);
  }

  /*
   * Use Scratch[lo..hi] as scratch space to sort the Data[lo..hi].
   *
   * Rather than copy our portion of Data into Scratch to start off
   * each _Merge(), the recursive levels will alternate merging from
   * Data into Scratch, and merging from Scratch into Data.
   *
   * At even depths -- including the initial one -- we leave the
   * sorted data in Data.  At odd depths, we leave the sorted data in
   * Scratch.
   *
   * The data stays in Data "all they way down" until the first
   * _Merge(), then is moved back and forth as we return up the chain.
   */
  private proc _MergeSort(Data: [?Dom], Scratch: [], lo:int, hi:int, minlen=16, comparator:?rec=defaultComparator, depth: int)
    where Dom.rank == 1 {
    import Sort.InsertionSort;

    const stride = if Dom.stridable then abs(Dom.stride) else 1,
          size = (hi - lo) / stride,
          mid = lo + (size/2) * stride;

    /*
     * When we return from an even depth, the data must be in Data.
     * When we return from an odd depth, the data must be in Scratch.
     * At the point we dispatch to insertionSort, the data is still in
     * Data.  So, if we get here at an odd depth, we'd have to sort
     * Data and then copy to Scratch.  Avoid the copy by doing the
     * sort one level before that, while we're still even.
     *
     * "size" is a misnomer.  For 1..10, size works out to 9.  That's
     * the value we want to base the calculation of mid on.  But for
     * the loop control, we really need to consider the size as 10.
     */
    if ((size+1) < minlen || (((depth & 1) == 0) && (size+1) < 2 * minlen)) {
      InsertionSort.insertionSort(Data, comparator=comparator, lo, hi);

      if depth & 1 {
        // At odd depths, we need to return the results in Scratch.
        // But if the test above is correct, we'll never reach this point.
        if Dom.stridable then
          Scratch[lo..hi by Dom.stride] = Data[lo..hi by Dom.stride];
        else
          Scratch[lo..hi] = Data[lo..hi];
      }
      return;
    }

    if(here.runningTasks() < here.numPUs(logical=true)) {
      cobegin {
        { _MergeSort(Data, Scratch, lo, mid, minlen, comparator, depth+1); }
        { _MergeSort(Data, Scratch, mid+stride, hi, minlen, comparator, depth+1); }
      }
    } else {
      _MergeSort(Data, Scratch, lo, mid, minlen, comparator, depth+1);
      _MergeSort(Data, Scratch, mid+stride, hi, minlen, comparator, depth+1);
    }

    if depth & 1 == 0 {
      _Merge(Data, Scratch, lo, mid, hi, comparator);
    } else {
      _Merge(Scratch, Data, lo, mid, hi, comparator);
    }
  }

  private proc _Merge(Dst: [?Dom] ?eltType, Src: [], lo:int, mid:int, hi:int, comparator:?rec=defaultComparator) {
    /* Data[lo..mid by stride] is much slower than Data[lo..mid] when
     * Dom is unstrided.  So specify the latter explicitly when possible. */
    const stride = if Dom.stridable then abs(Dom.stride) else 1;
    const a1range = if Dom.stridable then lo..mid by stride else lo..mid;
    const a1max = mid;

    const a2range = if Dom.stridable then (mid+stride)..hi by stride else (mid+1)..hi;
    const a2max = hi;

    ref A1 = Src[a1range];
    ref A2 = Src[a2range];

    var a1 = a1range.first;
    var a2 = a2range.first;
    var i = lo;
    while ((a1 <= a1max) && (a2 <= a2max)) {
      if (chpl_compare(A1(a1), A2(a2), comparator) <= 0) {
        Dst[i] = A1[a1];
        a1 += stride;
        i += stride;
      } else {
        Dst[i] = A2[a2];
        a2 += stride;
        i += stride;
      }
    }
    while (a1 <= a1max) {
      Dst[i] = A1[a1];
      a1 += stride;
      i += stride;
    }
    while (a2 <= a2max) {
      Dst[i] = A2[a2];
      a2 += stride;
      i += stride;
    }
  }
}

pragma "no doc"
module QuickSort {
  import Sort.defaultComparator;
  use Sort.ShallowCopy;

  /*
   Partition the array Data[lo..hi] using the pivot at Data[pivIdx].

   This is the 3-way symmetric partition described
   in Engineering a Sort Function (1993) by Jon L. Bentley , M. Douglas McIlroy

   Returns the (eqStart,eqEnd) where eqStart..eqEnd elements are
   equal to the pivot (and elements less are before eqStart and elements
   greater are after eqEnd).
   */
  proc partition(Data: [?Dom] ?eltType,
                 lo: int, pivIdx: int, hi: int,
                 comparator)
  {
    // The following section categorizes array elements as follows:
    //
    //   |  =  |  <  |  ?  |  >  |  =   |
    //    lo    a     b   c     d     hi
    //
    //  lo..a-1 stores equal elements
    //   a..b-1 stores elements < pivot
    //   b..c   store uncategorized elements
    // c+1..d-1 store elements > pivot
    // d+1..hi  stores equal elements

    // initially, entire array is in b..c
    var a = lo;
    var b = lo;
    var c = hi;
    var d = hi;

    // Now put the pivot in Data[lo] so we can
    // avoid keeping track of its position.
    if lo != pivIdx then
      ShallowCopy.shallowSwap(Data[lo], Data[pivIdx]);

    a += 1;
    b += 1;

    // Now swap the pivot to a local variable
    pragma "no auto destroy"
    var piv: eltType = ShallowCopy.shallowCopyInit(Data[lo]); // leaves Data[lo] empty

    while true {
      while b <= c {
        // continue while Data[b] <= piv
        var cmp = chpl_compare(Data[b], piv, comparator);
        if cmp > 0 then
          break;
        if cmp == 0 {
          ShallowCopy.shallowSwap(Data[a], Data[b]);
          a += 1; // one more equal element (on left)
        }
        b += 1; // one more categorized element
      }
      while c >= b {
        // continue while Data[c] >= piv
        var cmp = chpl_compare(Data[c], piv, comparator);
        if cmp < 0 then
          break;
        if cmp == 0 {
          ShallowCopy.shallowSwap(Data[d], Data[c]);
          d -= 1; // one more equal element (on right)
        }
        c -= 1; // one more categorized element
      }
      if b > c then
        break; // stop here

      // then Data[b] > piv and Data[c] < piv,
      // so Data[c] < Data[b] and they are an inversion
      ShallowCopy.shallowSwap(Data[b], Data[c]);
      b += 1;
      c -= 1;
    }

    // Now put piv back in Data[lo]
    ShallowCopy.shallowCopy(Data[lo], piv); // leaves piv empty

    // now we are in the state:
    //   |  =  |  <  |  > |  =   |
    //    lo    a   c b  d     hi

    // now place the equal regions in the right places
    var s, l, h: int;

    // Fix the first = region
    s = min(a-lo, b-a); // the number of of elements to swap
    l = lo;
    h = b-s;
    while s > 0 {
      ShallowCopy.shallowSwap(Data[l], Data[h]);
      l += 1;
      h += 1;
      s -= 1;
    }

    // Fix the second = region
    var n = hi+1;
    s = min(d-c, hi-d);
    l = b;
    h = n-s;
    while s > 0 {
      ShallowCopy.shallowSwap(Data[l], Data[h]);
      l += 1;
      h += 1;
      s -= 1;
    }

    var eqStart = b-a+lo;
    var eqEnd = hi-(d-c);

    return (eqStart, eqEnd);
  }


  // Returns the index of the median element
  // of Data[lo], Data[mid], Data[hi]
  // (in other words it returns lo, mid, or hi).
  proc order3(Data: [?Dom] ?eltType,
              lo: int, mid: int, hi: int,
              comparator): int {

    if chpl_compare(Data[lo], Data[mid], comparator) < 0 {
      // lo < mid
      if chpl_compare(Data[hi], Data[lo], comparator) < 0 {
        // lo < mid, hi < lo -> hi < lo < mid
        return lo;
      } else if chpl_compare(Data[mid], Data[hi], comparator) < 0 {
        // lo < mid, lo <= hi, mid < hi -> lo < mid < hi
        return mid;
      } else {
        // lo < mid, lo <= hi, hi <= mid -> lo <= hi <= mid
        return hi;
      }
    } else {
      // mid <= lo
      if chpl_compare(Data[lo], Data[hi], comparator) < 0 {
        // mid <= lo, lo < hi -> mid <= lo < hi
        return lo;
      } else if chpl_compare(Data[hi], Data[mid], comparator) < 0 {
        // mid <= lo, hi <= lo, hi < mid -> hi < mid <= lo
        return mid;
      } else {
        // mid <= lo, hi <= lo, mid <= hi -> mid <= hi <= lo
        return hi;
      }
    }
  }

 /* Use quickSort to sort Data */
 proc quickSort(Data: [?Dom] ?eltType,
                minlen=16,
                comparator:?rec=defaultComparator) {

    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("quickSort() requires 1-D array");
    }

    if Dom.stridable && Dom.stride != 1 {
      ref reindexed = Data.reindex(Dom.alignedLow..#Dom.size);
      assert(reindexed.domain.stride == 1);
      quickSortImpl(reindexed, minlen, comparator);
      return;
    }

    assert(Dom.stride == 1);
    quickSortImpl(Data, minlen, comparator);
  }


  /* Non-stridable quickSort to sort Data[start..end] */
  proc quickSortImpl(Data: [?Dom] ?eltType,
                     minlen=16,
                     comparator:?rec=defaultComparator,
                     start:int = Dom.low, end:int = Dom.high) {
    import Sort.InsertionSort;

    // grab obvious indices
    const lo = start,
          hi = end,
          mid = lo + (hi-lo+1)/2;
    var piv = mid;

    if hi - lo < minlen {
      // base case -- use insertion sort
      InsertionSort.insertionSort(Data, comparator=comparator, lo, hi);
      return;
    } else if hi <= lo {
      // nothing to sort
      return;
    }

    // find pivot using median-of-3 method for small arrays
    // and a "ninther" for bigger arrays. Places the pivot in
    // Data[lo].
    if hi - lo < 100 {
      piv = order3(Data, lo, mid, hi, comparator);
    } else {
      // assumes array size > 9 at the very least

      // median of each group of 3
      const medLo  = order3(Data, lo,    lo+1, lo+2,  comparator);
      const medMid = order3(Data, mid-1, mid,  mid+1, comparator);
      const medHi  = order3(Data, hi-2,  hi-1, hi,    comparator);
      // median of the medians
      piv = order3(Data, medLo, medMid, medHi, comparator);
    }

    var (eqStart, eqEnd) = partition(Data, lo, piv, hi, comparator);

    if hi-lo < 300 {
      // stay sequential
      quickSortImpl(Data, minlen, comparator, lo, eqStart-1);
      quickSortImpl(Data, minlen, comparator, eqEnd+1, hi);
    } else {
      // do the subproblems in parallel
      forall i in 1..2 {
        if i == 1 then
          quickSortImpl(Data, minlen, comparator, lo, eqStart-1);
        else
          quickSortImpl(Data, minlen, comparator, eqEnd+1, hi);
      }
    }
  }
}

pragma "no doc"
module SelectionSort {
  import Sort.defaultComparator;
  /*
    Sort the 1D array `Data` in-place using a sequential selection sort
    algorithm.

    :arg Data: The array to be sorted
    :type Data: [] `eltType`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.

   */
  proc selectionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {

    if Dom.rank != 1 {
      compilerError("selectionSort() requires 1-D array");
    }

    const low = Dom.alignedLow,
          high = Dom.alignedHigh,
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
}

pragma "no doc"
module ShellSort {
  import Sort.defaultComparator;
  proc shellSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator,
                 start=Dom.alignedLow, end=Dom.alignedHigh)
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
}


pragma "no doc"
module SampleSortHelp {
  param maxLogBuckets = 8; // not counting equality buckets.
  param classifyUnrollFactor = 7;
  const equalBucketThreshold = 5;
  const maxInline = 16;
  const oversampleFactor = 0.2;

  // compute logarithm base 2 rounded down
  proc log2int(n: int) {
    if n <= 0 then
      return 0;
    return log2(n);
  }

  proc computeLogBucketSize(n: int) {
    const LogBuckets = 8;
    const BaseCaseSize = 16;
    const BaseCaseMultiplier = 16;
    const SingleLevelThreshold = maxInline * LogBuckets;
    const TwoLevelThreshold = SingleLevelThreshold * LogBuckets;
    var ret = 0;
    if n <= SingleLevelThreshold {
      ret = log2int(n / maxInline / 2);
    } else if n <= TwoLevelThreshold {
      ret = (log2int(n / maxInline / 2)+1)/2;
    } else {
      ret = maxLogBuckets;
    }

    ret = max(1, ret); // make sure it's at least 1
    ret = min(maxLogBuckets, ret); // make sure it's at most maxLogBuckets.

    return ret;
  }
  proc chooseSampleStep(n:int, logNumBuckets:int) {
    var oversample = min(1.0, oversampleFactor * log2int(n));
    var step = max(1, oversample:int);
    return step;
  }



  record SampleBucketizer {
    type eltType;

    // filled from 1 to num_buckets_
    var storage: c_array(eltType, 1<<maxLogBuckets);
    // filled from 0 to num_buckets, last one is duplicated
    var sortedStorage: c_array(eltType, 1<<maxLogBuckets);

    var logBuckets: int;
    var numBuckets: int;
    var equalBuckets: bool;

    proc writeThis(ch) throws {
      ch <~> "SampleBucketizer(";
      ch <~> "\n logBuckets=" <~> logBuckets;
      ch <~> "\n numBuckets=" <~> numBuckets;
      ch <~> "\n equalBuckets=" <~> equalBuckets;
      ch <~> "\n storage=";
      for i in 0..numBuckets {
        ch <~> (try! " %xt".format(storage[i]));
      }
      ch <~> "\n sortedStorage=";
      for i in 0..numBuckets {
        ch <~> (try! " %xt".format(sortedStorage[i]));
      }
      ch <~> ")\n";
    }

    proc getNumBuckets() {
      return numBuckets * (1 + equalBuckets:int);
    }
    proc getBinsToRecursivelySort() {
      var r:range(stridable=true);
      if equalBuckets {
        // odd bins will be equality buckets
        r = (0..(getNumBuckets()-1)) by 2;
      } else {
        r = 0..(getNumBuckets()-1);
      }
      return r;
    }
    proc getNextStartBit(start_bit:int) {
      return start_bit;
    }

    // Build the tree from the sorted splitters
    // logBuckets does not account for equalBuckets.
    proc build(logBuckets: int, equalBuckets: bool) {
      this.logBuckets = logBuckets;
      this.numBuckets = 1 << logBuckets;
      this.equalBuckets = equalBuckets;

      var numSplitters = this.numBuckets - 1;
      // Copy the last element
      sortedStorage[numSplitters] = sortedStorage[numSplitters-1];
      build(0, numSplitters, 1);
    }
    // Recursively builds the tree
    proc build(left: int, right: int, pos: int) {
      var mid = left + (right - left) / 2;
      storage[pos] = sortedStorage[mid];
      if 2*pos < numBuckets {
        build(left, mid, 2*pos);
        build(mid, right, 2*pos + 1);
      }
    }
    inline proc splitter(i:int) const ref : eltType {
      return storage[i];
    }
    inline proc sortedSplitter(i:int) const ref : eltType {
      return sortedStorage[i];
    }

    proc bucketForRecord(a, criterion, startbit) {
      var bk = 1;
      for lg in 0..#logBuckets {
        bk = 2*bk + (chpl_compare(splitter(bk), a, criterion) < 0):int;
      }
      if equalBuckets {
        bk = 2*bk + (chpl_compare(a, sortedSplitter(bk-numBuckets), criterion) == 0):int;
      }
      return bk - (if equalBuckets then 2*numBuckets else numBuckets);
    }
    // yields (index, bucket index) for A[start_n..end_n]
    iter classify(A, start_n, end_n, criterion, startbit) {
      const paramEqualBuckets = equalBuckets;
      const paramLogBuckets = logBuckets;
      const paramNumBuckets = 1 << (paramLogBuckets + paramEqualBuckets:int);
      var b:c_array(int, classifyUnrollFactor);

      var cur = start_n;
      // Run the main (unrolled) loop
      while cur <= end_n-(classifyUnrollFactor-1) {
        for /*param*/ i in 0..classifyUnrollFactor-1 {
          b[i] = 1;
        }
        for /*param*/ lg in 0..paramLogBuckets-1 {
          for /*param*/ i in 0..classifyUnrollFactor-1 {
            b[i] = 2*b[i] + (chpl_compare(splitter(b[i]), A[cur+i],criterion)<0):int;
          }
        }
        if paramEqualBuckets {
          for /*param*/ i in 0..classifyUnrollFactor-1 {
            b[i] = 2*b[i] +
                   (chpl_compare(A[cur+i],
                           sortedSplitter(b[i] - paramNumBuckets/2),criterion)==0):int;
          }
        }
        for /*param*/ i in 0..classifyUnrollFactor-1 {
          yield (cur+i, b[i]-paramNumBuckets);
        }
        cur += classifyUnrollFactor;
      }
      // Handle leftover
      while cur <= end_n {
        var bk = 1;
        for lg in 0..#paramLogBuckets {
          bk = 2*bk + (chpl_compare(splitter(bk), A[cur], criterion)<0):int;
        }
        if paramEqualBuckets {
          bk = 2*bk + (chpl_compare(A[cur],
                               sortedSplitter(bk - paramNumBuckets/2),criterion)==0):int;
        }
        yield (cur, bk - paramNumBuckets);
        cur += 1;
      }
    }
  }

  proc createSplittersFromSample(A,
                                 ref splitterBucketizer:SampleBucketizer,
                                 criterion,
                                 start_n: int,
                                 sampleSize: int,
                                 sampleStep: int,
                                 in numBuckets: int,
                                 forceEqualBuckets:? = none) {

    // Create the splitters
    ref splitters = splitterBucketizer.sortedStorage;
    var arrayIndex = start_n + sampleStep - 1;
    var splitterIndex = 0;
    splitters[splitterIndex] = A[arrayIndex];
    for i in 2..numBuckets-1 {
      arrayIndex += sampleStep;
      // Skip duplicates
      if chpl_compare(splitters[splitterIndex], A[arrayIndex], criterion)!=0 {
        splitterIndex += 1;
        splitters[splitterIndex] = A[arrayIndex];
      }
    }

    // Check for duplicate splitters
    var uniqueSplitters = 1 + splitterIndex;
    var myUseEqualBuckets = numBuckets - 1 - uniqueSplitters >= equalBucketThreshold;
    if 1+2*uniqueSplitters < 1<<maxLogBuckets {
      myUseEqualBuckets = true;
    }
    if forceEqualBuckets.type != nothing then
      myUseEqualBuckets = forceEqualBuckets;

    // Fill the array to the next power of two
    var logBuckets = log2(uniqueSplitters) + 1;
    numBuckets = 1 << logBuckets;
    for i in uniqueSplitters+1 .. numBuckets-1 {
      splitterIndex += 1;
      splitters[splitterIndex] = A[arrayIndex];
    }

    // Build the tree
    splitterBucketizer.build(logBuckets, myUseEqualBuckets);
  }

  // Using this to build the sample is inherently un-stable.
  // A stable sort would probably sort several subsections of the array,
  // and then use a median approach to create the samples. Alternatively,
  // it could copy the sample somewhere else for sorting.
  proc putRandomSampleAtArrayStart(in start_n:int,
                                   end_n:int,
                                   A:[],
                                   in numSamples:int,
                                   seed=1) {
    private use Random;
    import Sort.ShallowCopy;
    var Tmp:[1..1] A.eltType;
    var randNums = createRandomStream(seed=seed, eltType=int, parSafe=false);
    while numSamples > 0 {
      numSamples -= 1;

      var offset = randNums.getNext(start_n, end_n);
      if offset != start_n {
        // A[start_n] <=> A[offset] but with shallow copy.
        var tmp: A.eltType;
        ShallowCopy.shallowCopy(Tmp, 1, A, start_n, 1);
        ShallowCopy.shallowCopy(A, start_n, offset, 1);
        ShallowCopy.shallowCopy(A, offset, Tmp, 1, 1);
      }

      start_n += 1;
    }
  }
}

pragma "no doc"
module RadixSortHelp {
  import Sort.{defaultComparator, DefaultComparator};
  import Reflection.canResolveMethod;

  // This is the number of bits to sort at a time in the radix sorter.
  // The code assumes that all integer types are a multiple of it.
  // That would need to change if it were to increase.
  //
  // At the same time, using a value less than 8 will probably perform poorly.
  param RADIX_BITS = 8;

  param classifyUnrollFactor = 7;

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
  inline
  proc binForRecordKeyPart(a, criterion, startbit:int)
  {
    // We have keyPart(element, start):(section:int(8), part:int/uint)
    const testRet: criterion.keyPart(a, 1).type;
    const testPart = testRet(1);
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
  inline
  proc binForRecord(a, criterion, startbit:int)
  {
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
  proc fixedWidth(type eltTy) param {
    if (isUintType(eltTy) || isIntType(eltTy) ||
        isRealType(eltTy) || isImagType(eltTy)) then
      return numBits(eltTy);

    if (isHomogeneousTuple(eltTy)) {
      var tmp:eltTy;
      return tmp.size * numBits(tmp(0).type);
    }

    return -1;
  }

  // Returns a compile-time known final startbit
  // e.g. for uint(64), returns 56 (since that's 64-8 and the
  // last sort pass will sort on the last 8 bits).
  //
  // Returns -1 if no such ending is known at compile-time.
  proc msbRadixSortParamLastStartBit(Data:[], comparator) param {
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

  proc radixSortClz(val) {
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
  proc findDataStartBit(startbit:int, min_ubits, max_ubits):int {
    var xor = min_ubits ^ max_ubits;

    // Clear the top bits in xor if they are after bitsinpart
    param bitsPerPart = numBits(min_ubits.type);
    const bitsinpart = startbit % bitsPerPart;
    xor <<= bitsinpart;
    xor >>= bitsinpart;

    var new_start = radixSortClz(xor);
    var new_digit = new_start / RADIX_BITS;
    var new_start_bit_rounded = new_digit * RADIX_BITS;

    return new_start_bit_rounded:int;
  }
  // Check that the elements from start_n..end_n in A are sorted by criterion

  pragma "insert line file info"
  pragma "always propagate line file info"
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

  record RadixBucketizer {
    proc getNumBuckets() {
      return (1 << RADIX_BITS) + 2; // +2 for end-before and end-after bins
    }
    proc getBinsToRecursivelySort() {
      // first and last bin represent end-of-key and do not need
      // to be recursively sorted.
      return 1..(1 << RADIX_BITS);
    }
    proc getNextStartBit(start_bit:int) {
      return start_bit+8;
    }

    // returns the bin for classifying a single record
    inline proc bucketForRecord(a, criterion, startbit) {
      const (bin, _) = binForRecord(a, criterion, startbit);
      return bin;
    }

    // yields (index, bucket index) for A[start_n..end_n]
    iter classify(A, start_n, end_n, criterion, startbit) {
      var cur = start_n;
      while cur <= end_n-(classifyUnrollFactor-1) {
        for /*param*/ j in 0..classifyUnrollFactor-1 {
          yield (cur+j, bucketForRecord(A[cur+j], criterion, startbit));
        }
        cur += classifyUnrollFactor;
      }
      while cur <= end_n {
        yield (cur, bucketForRecord(A[cur], criterion, startbit));
        cur += 1;
      }
    }
  }
}

pragma "no doc"
module ShallowCopy {
  private use SysCTypes;

  // The shallowCopy / shallowSwap code needs to be able to copy/swap
  // _array records. But c_ptrTo on an _array will return a pointer to
  // the first element, which messes up the shallowCopy/shallowSwap code
  //
  // As a workaround, this function just returns a pointer to the argument,
  // whether or not it is an array.
  private inline proc ptrTo(ref x) {
    return c_pointer_return(x);
  }

  // These shallow copy functions "move" a record around
  // (i.e. they neither swap nor call a copy initializer).
  //
  // TODO: move these out of the Sort module and/or consider
  // language support for it. See issue #14576.

  inline proc shallowCopy(ref dst, ref src) {
    type st = __primitive("static typeof", dst);
    if isPODType(st) {
      dst = src;
    } else {
      var size = c_sizeof(st);
      c_memcpy(ptrTo(dst), ptrTo(src), size);
      if boundsChecking {
        // The version moved from should never be used again,
        // but we clear it out just in case.
        c_memset(ptrTo(src), 0, size);
      }
    }
  }

  // returns the result of shallow copying src
  pragma "unsafe"
  pragma "no copy return"
  inline proc shallowCopyInit(ref src) {
    type st = __primitive("static typeof", src);
    pragma "no init"
    pragma "no auto destroy"
    var dst: st;
    shallowCopy(dst, src);
    return dst;
  }

  pragma "unsafe"
  inline proc shallowSwap(ref lhs:?t, ref rhs:t) {
    type st = __primitive("static typeof", lhs);
    pragma "no init"
    pragma "no auto destroy"
    var tmp: st;
    if isPODType(st) {
      tmp = lhs;
      lhs = rhs;
      rhs = tmp;
    } else {
      var size = c_sizeof(st);
      // tmp = lhs
      c_memcpy(ptrTo(tmp), ptrTo(lhs), size);
      // lhs = rhs
      c_memcpy(ptrTo(lhs), ptrTo(rhs), size);
      // rhs = tmp
      c_memcpy(ptrTo(rhs), ptrTo(tmp), size);
    }
  }

  // TODO: These shallowCopy functions should handle Block,Cyclic arrays
  inline proc shallowCopy(ref A, dst, src, nElts) {
    use SysCTypes;
    // Ideally this would just be
    //A[dst..#nElts] = A[src..#nElts];

    if boundsChecking {
      assert(nElts > 0);
      assert(A.domain.contains(dst..#nElts));
      assert(A.domain.contains(src..#nElts));
    }

    if A._instance.isDefaultRectangular() {
      type st = __primitive("static field type", A._value, "eltType");
      var size = (nElts:size_t)*c_sizeof(st);
      c_memcpy(ptrTo(A[dst]), ptrTo(A[src]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ A, {dst..#nElts},
                                       /*src*/ A, {src..#nElts});
      if !ok {
        halt("bulk transfer failed in sorting");
        for i in vectorizeOnly(0..#nElts) {
          __primitive("=", A[dst+i], A[src+i]);
        }
      }
    }
  }
  inline proc shallowCopy(ref DstA, dst, ref SrcA, src, nElts) {
    use SysCTypes;
    // Ideally this would just be
    //DstA[dst..#nElts] = SrcA[src..#nElts];

    if boundsChecking {
      assert(nElts > 0);
      assert(DstA.domain.contains(dst..#nElts));
      assert(SrcA.domain.contains(src..#nElts));
    }

    if DstA._instance.isDefaultRectangular() &&
       SrcA._instance.isDefaultRectangular() {
      type st = __primitive("static field type", DstA._value, "eltType");
      var size = (nElts:size_t)*c_sizeof(st);
      c_memcpy(ptrTo(DstA[dst]), ptrTo(SrcA[src]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ DstA, {dst..#nElts},
                                       /*src*/ SrcA, {src..#nElts});
      if !ok {
        halt("bulk transfer failed in sorting");
        for i in vectorizeOnly(0..#nElts) {
          __primitive("=", DstA[dst+i], SrcA[src+i]);
        }
      }
    }
  }
}
pragma "no doc"
module SequentialInPlacePartitioning {
  private param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step

  // Returns the count for each bucket
  // Stores the data in dst in buckets according to the bucketizer.
  // (e.g. sorted by the next digit in radix sort)
  proc bucketize(start_n: int, end_n: int, A:[],
                 bucketizer,
                 criterion, startbit:int) {

    var nBuckets = bucketizer.getNumBuckets();
    const nTasks = if dataParTasksPerLocale > 0
               then dataParTasksPerLocale
               else here.maxTaskPar;

    // Divide the input into nTasks chunks.
    const countsSize = nTasks * nBuckets;
    const n = end_n - start_n + 1;
    const blockSize = divceil(n, nTasks);
    const nBlocks = divceil(n, blockSize);

    var counts: [0..#nBuckets] int;

    // Step 1: count
    coforall tid in 0..#nTasks with (+ reduce counts) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      for (i,bin) in bucketizer.classify(A, start, end, criterion, startbit) {
        counts[bin] += 1;
      }
    }

    // Step 2: scan
    var end_offsets = (+ scan counts) + start_n;
    var offsets = end_offsets - counts;

    // Step 3: distribute/shuffle
    var curbin = 0;
    while true {
      // Find the next bin that isn't totally in place.
      while curbin < nBuckets && offsets[curbin] == end_offsets[curbin] {
        curbin += 1;
      }
      if curbin >= nBuckets {
        break;
      }

      // TODO: I think it might be possible to make this sort stable
      // by populating buf from the start of the data instead of from the end.
      // buf would need to be populated with the first M elements that aren't
      // already in the correct bin.

      // TODO: I think it's possible to make this shuffle parallel
      // by imagining each task has a max_buf and have them update
      // atomic offsets.
      param max_buf = DISTRIBUTE_BUFFER;
      var buf: max_buf*A.eltType;
      var used_buf = 0;
      var end = end_offsets[curbin];
      var endfast = max(offsets[curbin], end_offsets[curbin]-2*max_buf);
      var bufstart = max(offsets[curbin], end_offsets[curbin]-max_buf);
      var i = bufstart;

      // Fill buf with up to max_buf records from the end of this bin.
      while i < end {
        buf[used_buf] <=> A[i];
        used_buf += 1;
        i += 1;
      }

      while offsets[curbin] < endfast {
        // Now go through the records in buf
        // putting them in their right home.
        for (idx, bin) in bucketizer.classify(buf, 1, max_buf,
                                              criterion, startbit) {
          // Swap buf[j] into its appropriate bin.
          // Leave buf[j] with the next unsorted item.
          A[offsets[bin]] <=> buf[idx];
          offsets[bin] += 1;
        }
      }
      // Now, handle elements in bufstart...end_offsets[cur_bin]
      while offsets[curbin] < end {
        // Put buf[j] into its right home
        var j = 0;
        while used_buf >= 0 && j < used_buf {
          const bin = bucketizer.bucketForRecord(buf[j], criterion, startbit);
          // Swap buf[j] into its appropriate bin.
          var offset = offsets[bin];
          A[offset] <=> buf[j];
          offsets[bin] += 1;
          // Leave buf[j] with the next unsorted item.
          // But offsets[bin] might be in the region we already read.
          if bin == curbin && offset >= bufstart {
            used_buf -= 1;
            buf[j] <=> buf[used_buf];
          }
          j += 1;
        }
      }
    }

    return counts;
  }
}

pragma "no doc"
module TwoArrayPartitioning {
  private use BlockDist;
  private use MSBRadixSort;
  public use List only list;
  import Sort.{ShellSort, RadixSortHelp, SampleSortHelp, ShallowCopy};

  private param debug = false;
  param maxBuckets = 512;

  record TwoArraySortTask {
    var start: int;
    var size: int;
    var startbit: int;
    var inA: bool;
    var doSort: bool;

    proc init() { }
    // To make sure all fields are specified
    proc init(start:int, size:int, startbit:int, inA:bool, doSort:bool) {
      this.start = start;
      this.size = size;
      this.startbit = startbit;
      this.inA = inA;
      this.doSort = doSort;
    }
  }

  record TwoArrayBucketizerPerTaskState {
    var localCounts: [0..#maxBuckets] int;
  }

  record TwoArrayBucketizerSharedState {
    var nTasks:int = if dataParTasksPerLocale > 0
                      then dataParTasksPerLocale
                      else here.maxTaskPar;
    var countsSize:int = nTasks*maxBuckets;

    var bucketizer; // contains e.g. sample

    // globalCounts stores counts like this:
    //   count for bin 0, task 0
    //   count for bin 0, task 1
    //   ...
    //   count for bin 1, task 0
    //   count for bin 1, task 1
    // i.e. bin*nTasks + taskId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;

    var localState:[0..#nTasks] TwoArrayBucketizerPerTaskState;

    // Set by bucketizer, used by sorter
    var counts:[0..#maxBuckets] int;

    // Used by sorter
    var ends:[0..#maxBuckets] int;
    var outputInA:[0..#maxBuckets] bool;

    // start, end, isInA
    // were [0..-1]
    var bigTasks: list(TwoArraySortTask);
    var smallTasks: list(TwoArraySortTask);

    var baseCaseSize:int = 16;
    var sequentialSizePerTask:int = 4096;
    var endbit:int = max(int);
  }

  record TwoArrayDistributedBucketizerStatePerLocale {
    type bucketizerType;

    var compat:TwoArrayBucketizerSharedState(bucketizerType);

    var countsSize:int = numLocales*maxBuckets;
    // globalCounts stores counts like this:
    //   count for bin 0, locale 0
    //   count for bin 0, locale 1
    //   ...
    //   count for bin 1, locale 0
    //   count for bin 1, locale 1
    // i.e. bin*nTasks + localeId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;
  }

  record TasksForLocale {
    // was [0..-1]
    var localTasks: list(TwoArraySortTask);
  }

  record TwoArrayDistributedBucketizerSharedState {
    type bucketizerType;

    var numLocales:int;
    var perLocale = newBlockArr(0..#numLocales,
        TwoArrayDistributedBucketizerStatePerLocale(bucketizerType));

    const baseCaseSize:int;
    const endbit:int = max(int);

    const countsSize:int = numLocales*maxBuckets;

    // globalCounts stores counts like this:
    //   count for bin 0, locale 0
    //   count for bin 0, locale 1
    //   ...
    //   count for bin 1, locale 0
    //   count for bin 1, locale 1
    // i.e. bin*nTasks + localeId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;

    // was [0..-1]
    var distTasks: list(TwoArraySortTask);
    var localTasks: [0..#numLocales] TasksForLocale;

    proc postinit() {
      // Copy some vars to the compat
      for p in perLocale {
        p.compat.baseCaseSize = baseCaseSize;
        p.compat.endbit = endbit;
        assert(p.compat.nTasks > 0);
        assert(p.compat.countsSize > 0);
      }
    }
  }

  // Stores the data in dst in buckets according to the bucketizer.
  // (e.g. sorted by the next digit in radix sort)
  // Counts per bin are stored in state.counts. Other data in
  // state is used locally by this routine or used elsewhere
  proc bucketize(start_n: int, end_n: int, dst:[], src:[],
                 ref state: TwoArrayBucketizerSharedState,
                 criterion, startbit:int) {

    if debug then
      writeln("bucketize ", start_n..end_n, " startbit=", startbit);

    const nBuckets = state.bucketizer.getNumBuckets();
    const n = end_n - start_n + 1;
    const nTasks = if n >= state.nTasks then state.nTasks else 1;
    assert(nTasks > 0);

    if n == 0 {
      state.counts = 0;
      return;
    }

    // Divide the input into nTasks chunks.
    const countsSize = nTasks * nBuckets;
    const blockSize = divceil(n, nTasks);
    const nBlocks = divceil(n, blockSize);

    // Count
    coforall tid in 0..#nTasks with (ref state) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      if debug then
        writeln("tid ", tid, " considering ", start..end);

      ref counts = state.localState[tid].localCounts;
      for bin in 0..#nBuckets {
        counts[bin] = 0;
      }
      for (i,bin) in state.bucketizer.classify(src, start, end,
                                               criterion, startbit) {
        counts[bin] += 1;
      }
      // Now store the counts into the global counts array
      for bin in vectorizeOnly(0..#nBuckets) {
        state.globalCounts[bin*nTasks + tid] = counts[bin];
      }
    }

    // Step 2: scan
    state.globalEnds = (+ scan state.globalCounts) + start_n;

    if debug {
      for bin in 0..#nBuckets {
        for tid in 0..#nTasks {
          var gb = bin*nTasks+tid;
          if state.globalCounts[gb] != 0 {
            writeln("tid ", tid, " count[", bin, "] = ", state.globalCounts[gb],
                    " end = ", state.globalEnds[gb] - 1);
          }
        }
      }
    }

    // Step 3: distribute
    coforall tid in 0..#nTasks with (ref state) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      ref nextOffsets = state.localState[tid].localCounts;
      // initialize nextOffsets
      for bin in 0..#nBuckets {
        var globalBin = bin*nTasks+tid;
        nextOffsets[bin] = if globalBin > 0
                           then state.globalEnds[globalBin-1]
                           else start_n;
        if debug {
          if state.globalCounts[globalBin] != 0 {
            writeln("tid ", tid, " nextOffsets[", bin, "] = ", nextOffsets[bin]);
          }
        }
      }

      for (i,bin) in state.bucketizer.classify(src, start, end,
                                               criterion, startbit) {
        // Store it in the right bin
        ref next = nextOffsets[bin];
        if debug {
          writeln("tid ", tid, " dst[", next, "] = src[", i, "] bin ", bin);
        }
        ShallowCopy.shallowCopy(dst, next, src, i, 1);
        next += 1;
      }
    }

    // Compute the total counts
    ref counts = state.counts;
    forall bin in 0..#nBuckets {
      var total = 0;
      for tid in 0..#nTasks {
        total += state.globalCounts[bin*nTasks + tid];
      }
      counts[bin] = total;
    }
  }
  proc testBucketize(start_n: int, end_n: int, dst:[], src:[],
                     bucketizer, criterion, startbit:int) {

    var state = new TwoArrayBucketizerSharedState(bucketizer=bucketizer);

    bucketize(start_n, end_n, dst, src, state, criterion, startbit);

    return state.counts;
  }


  private proc partitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if isSubtype(state.bucketizer.type, SampleSortHelp.SampleBucketizer) {
      var n = 1 + end_n - start_n;
      var logNumBuckets = SampleSortHelp.computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = SampleSortHelp.chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debug then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      SampleSortHelp.putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debug then
        writeln("recursing to sort the sample");

      // sort the sample


      // TODO: make it adjustable from the settings
      if sampleSize <= 1024*1024 {
        // base case sort, parallel OK
        msbRadixSort(start_n, start_n + sampleSize - 1,
                     A, criterion,
                     startbit, state.endbit,
                     settings=new MSBRadixSortSettings());
      } else {
        partitioningSortWithScratchSpace(start_n, start_n + sampleSize - 1,
                                         A, Scratch,
                                         state, criterion, startbit);
      }
      if debug {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debug {
        writeln("sample bucketizer ", state.bucketizer);
        writef("A %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
      }
    }

  }

  // Sorts the data in A.
  proc partitioningSortWithScratchSpace(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {

    if startbit > state.endbit then
      return;

    if end_n - start_n < state.baseCaseSize {
      ShellSort.shellSort(A, criterion, start=start_n, end=end_n);
      return;
    }

    if debug {
      writeln("partitioningSortWithScratchSpace(", start_n, ",", end_n, ")");
      writef("A %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
    }


    const n = (end_n - start_n + 1);
    const maxSequentialSize = max(n / state.nTasks,
                                  state.nTasks*state.sequentialSizePerTask);

    state.bigTasks.append(new TwoArraySortTask(start_n, n, startbit, inA=true, doSort=true));
    assert(state.bigTasks.size == 1);
    assert(state.smallTasks.size == 0);

    while !state.bigTasks.isEmpty() {
      const task = state.bigTasks.pop();
      const taskEnd = task.start + task.size - 1;

      assert(task.doSort);

      if debug then {
        writeln("doing big task ", task.start..taskEnd);
      }

      if task.inA {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, A, Scratch,
              state, criterion, task.startbit);

        // Count and partition
        bucketize(task.start, taskEnd, Scratch, A, state,
                  criterion, task.startbit);
        // bucketized data now in Scratch
        if debug {
          writef("pb %i %i Scratch=%xt\n", task.start, taskEnd, Scratch[task.start..taskEnd]);
        }
      } else {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, Scratch, A,
              state, criterion, task.startbit);

        // Count and partition
        bucketize(task.start, taskEnd, A, Scratch, state,
                  criterion, task.startbit);
        // bucketized data now in A
        if debug {
          writef("pb %i %i A=%xt\n", task.start, taskEnd, A[task.start..taskEnd]);
        }
      }
      const nowInA = !task.inA;

      // Compute the bucket ends
      state.ends = (+ scan state.counts) + task.start;

      // enqueue any sorting tasks not yet completed
      const nBuckets = state.bucketizer.getNumBuckets();
      for bin in 0..#nBuckets {
        const binSize = state.counts[bin];
        const binStart = state.ends[bin] - binSize;
        const binEnd = binStart + binSize - 1;
        const binStartBit = state.bucketizer.getNextStartBit(task.startbit);

        const sortit = state.bucketizer.getBinsToRecursivelySort().contains(bin);

        if binSize == 0 {
          // Do nothing
        } else if !nowInA && !sortit {
          // Enqueue a small task to do the copy.
          // TODO: handle large copies in big tasks, or enqueue several tasks here
          state.smallTasks.append(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));

        } else if binStartBit > state.endbit ||
                  binStart >= binEnd ||
                  binSize <= maxSequentialSize {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as small");
          }

          // Enqueue a small task to sort and possibly copy.
          state.smallTasks.append(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));

        } else {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as big");
          }

          // Enqueue a big task
          state.bigTasks.append(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));
        }
      }
    }

    // Now handle any small tasks.

    // TODO: sort small tasks by size

    forall task in state.smallTasks {
      const size = task.size;
      const taskEnd = task.start + size - 1;
      if size > 0 {
        if !task.inA {
          ShallowCopy.shallowCopy(A, task.start, Scratch, task.start, size);
        }

        if debug {
          writef("doing small task %i %i A=%xt\n", task.start, taskEnd, A[task.start..taskEnd]);
        }

        if task.doSort {
          // Sort it serially.
          msbRadixSort(task.start, taskEnd,
                       A, criterion,
                       task.startbit, state.endbit,
                       settings=new MSBRadixSortSettings(alwaysSerial=true));
        }
      }
    }

    if debug {
      writef("ps %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
      writef("ps %i %i Scratch=%xt\n", start_n, end_n, Scratch[start_n..end_n]);
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion, startbit);
    }
  }

  private proc distributedPartitioningSortWithScratchSpaceBaseCase(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int):void {

    if startbit > state.endbit then
      return;

    const n = end_n - start_n + 1;

    const localSubdomain = A.localSubdomain();
    const curDomain = {start_n..end_n};
    const intersect = curDomain[localSubdomain];
    if curDomain == intersect {
      if n > state.baseCaseSize {
        msbRadixSort(start_n, end_n,
                     A.localSlice(curDomain), criterion,
                     startbit, state.endbit,
                     settings=new MSBRadixSortSettings());
      } else {
        ShellSort.shellSort(A.localSlice(curDomain), criterion, start=start_n, end=end_n);
      }
    } else {
      const size = end_n-start_n+1;
      // Copy it to one locale
      var LocalA:[start_n..end_n] A.eltType;
      ShallowCopy.shallowCopy(LocalA, start_n, A, start_n, size);
      // Sort it
      if n > state.baseCaseSize {
        msbRadixSort(start_n, end_n,
                     LocalA, criterion,
                     startbit, state.endbit,
                     settings=new MSBRadixSortSettings());
      } else {
        ShellSort.shellSort(LocalA, criterion, start=start_n, end=end_n);
      }
      // Copy it back
      ShallowCopy.shallowCopy(A, start_n, LocalA, start_n, size);
    }

    if debug {
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion);
    }
    return;
  }


  private proc distributedPartitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if isSubtype(state.perLocale[0].compat.bucketizer.type, SampleSortHelp.SampleBucketizer) {
      var n = 1 + end_n - start_n;
      var logNumBuckets = SampleSortHelp.computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = SampleSortHelp.chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debug then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      SampleSortHelp.putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debug then
        writeln("recursing to sort the sample");

      // sort the sample

      // TODO: make it adjustable in the settings
      if sampleSize <= 1024*1024 {
        // base case sort, parallel OK
        distributedPartitioningSortWithScratchSpaceBaseCase(start_n, start_n + sampleSize - 1,
                                                            A, Scratch,
                                                            state, criterion,
                                                            startbit);
      } else {
        distributedPartitioningSortWithScratchSpace(start_n, start_n + sampleSize - 1,
                                                    A, Scratch,
                                                    state, criterion, startbit);
      }
      if debug {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.perLocale[0].compat.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debug {
        writeln("sample bucketizer ", state.perLocale[0].compat.bucketizer);
        writef("A %i %i A=%xt\n", start_n, end_n, A[start_n..end_n]);
      }

      // Now copy the bucketizer sample to all other locales
      for lid in 1..state.numLocales {
        state.perLocale[lid].bucketizer = state.perLocale[0].bucketizer;
      }
    }
  }

  proc distributedPartitioningSortWithScratchSpace(
          start_n:int, end_n:int, A:[], Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int): void {

    //use BlockDist only;

    if !A.hasSingleLocalSubdomain() {
      compilerError("distributedPartitioningSortWithScratchSpace needs single local subdomain");
    }
    // TODO: assert that src and dst have the same distribution?

    if startbit > state.endbit then
      return;

    if end_n - start_n < state.baseCaseSize {
      distributedPartitioningSortWithScratchSpaceBaseCase(start_n, end_n,
                                                          A, Scratch,
                                                          state, criterion,
                                                          startbit);
      return;
    }


    if debug {
      writeln("in distributed radix sort ", start_n, "..", end_n,
              " startbit ", startbit, " endbit ", state.endbit);
    }

    const n = (end_n - start_n + 1);
    state.distTasks.append(new TwoArraySortTask(start_n, n, startbit, true, true));
    assert(state.distTasks.size == 1);

    while !state.distTasks.isEmpty() {
      const task = state.distTasks.pop();
      const taskStart = task.start;
      const taskEnd = task.start + task.size - 1;

      assert(task.doSort);
      assert(task.inA);

      distributedPartitioningSortWithScratchSpaceHandleSampling(
            task.start, taskEnd, A, Scratch,
            state, criterion, task.startbit);

      const nBuckets = state.perLocale[0].compat.bucketizer.getNumBuckets();
      const nTasks = state.numLocales;

      // Step 1: Each locale sorts local portions into buckets
      // and saves counts in globalCounts
      coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
        on loc do {
          const localDomain = A.localSubdomain()[task.start..taskEnd];
          ref localSrc = A.localSlice(localDomain);
          ref localDst = Scratch.localSlice(localDomain);

          bucketize(localDomain.alignedLow,
                    localDomain.alignedHigh,
                    localDst, localSrc,
                    state.perLocale[tid].compat, criterion, task.startbit);
          ref localCounts = state.perLocale[tid].compat.counts;
          if debug {
            var total = 0;
            for bin in 0..#nBuckets {
              if localCounts[bin] > 0 {
                writeln("localCounts[", bin, "]=", localCounts[bin]);
                total += localCounts[bin];
              }
            }
            assert(total == localDomain.size);
          }
          // Now store the counts into the global counts array
          for bin in vectorizeOnly(0..#nBuckets) {
            state.perLocale[0].globalCounts[bin*nTasks + tid] = localCounts[bin];
          }
          //state.globalCounts[tid.. by nTasks] = localCounts;
        }
      }
      // Now the data is in Scratch

      if debug {
        writef("after bucketize local portions, Scratch is %xt\n", Scratch[task.start..taskEnd]);
      }

      // Step 2: scan
      state.perLocale[0].globalEnds = (+ scan state.perLocale[0].globalCounts) + task.start;

      // Store counts on each other locale
      forall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
        if tid != 0 {
          state.perLocale[tid].globalCounts = state.perLocale[0].globalCounts;
          state.perLocale[tid].globalEnds = state.perLocale[0].globalEnds;
        }
      }

      if debug {
        var total = 0;
        for i in 0..#state.countsSize {
          if state.perLocale[0].globalCounts[i] != 0 {
            total += state.perLocale[0].globalCounts[i];
            writeln("state.globalCounts[", i, "]=", state.perLocale[0].globalCounts[i]);
            writeln("state.globalEnds[", i, "]=", state.perLocale[0].globalEnds[i]);
          }
        }
        assert(total == task.size);
      }

      // Step 3: distribute the keys to the src array according
      // to the globalEnds.
      // In particular, bin i from task j should be stored into
      // globalEnds[i*ntasks+j-1] .. globalEnds[i*ntasks+j] - 1
      // (because the scan is inclusive)
      coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
        on loc do {
          const ref globalCounts = state.perLocale[tid].globalCounts;
          const ref globalEnds = state.perLocale[tid].globalEnds;
          const localSubdomain = A.localSubdomain()[task.start..taskEnd];
          // Compute localOffsets array based on the counts here
          // This could be written as a scan expression...
          ref localOffsets = state.perLocale[tid].compat.counts;
          {
            var offset = localSubdomain.low;
            for bin in 0..#nBuckets {
              localOffsets[bin] = offset;
              offset += globalCounts[bin*nTasks + tid];
            }
          }

          forall bin in 0..#nBuckets {
            var size = globalCounts[bin*nTasks + tid];
            if size > 0 {
              var localStart = localOffsets[bin];
              var localEnd = localStart + size - 1;
              var globalStart = if bin*nTasks+tid > 0
                                then globalEnds[bin*nTasks+tid-1]
                                else taskStart;
              var globalEnd = globalEnds[bin*nTasks+tid] - 1;
              if debug {
                writeln("bin ", bin, " tid ", tid, " range ", taskStart..taskEnd,
                        " A[", globalStart, "..", globalEnd, "] = Scratch[",
                        localStart, "..", localEnd, "]");
                assert(globalCounts[bin*nTasks+tid] ==
                    state.perLocale[0].globalCounts[bin*nTasks+tid]);
                assert(globalEnds[bin*nTasks+tid] ==
                    state.perLocale[0].globalEnds[bin*nTasks+tid]);
              }
              ShallowCopy.shallowCopy(A, globalStart, Scratch, localStart, size);
            }
          }
        }
      }
      // now the data is all in A
      if debug {
        writef("after distribute, A is %xt\n", A[task.start..taskEnd]);
      }

      // Step 4: Add sub-tasks depending on if the bin is local or distributed
      // still.
      for bin in state.perLocale[0].compat.bucketizer.getBinsToRecursivelySort() {
        const binStart = if bin*nTasks > 0
                          then state.perLocale[0].globalEnds[bin*nTasks-1]
                          else task.start;
        const binEnd = state.perLocale[0].globalEnds[bin*nTasks+nTasks-1] - 1;
        const binSize = binEnd - binStart + 1;
        const binStartBit = state.perLocale[0].compat.bucketizer.getNextStartBit(task.startbit);
        if binSize > 1 {
          // could this work for block?
          //var isOnOneLocale = A.domain.dist.idxToLocale(globalStart) ==
          //                    A.domain.dist.idxToLocale(globalEnd)
          var isOnOneLocale = false;
          var theLocaleId = -1;
          for (loc,tid) in zip(A.targetLocales(),0..) {
            const localSubdomain = A.localSubdomain(loc)[task.start..taskEnd];
            const curDomain = {binStart..binEnd};
            const intersect = curDomain[localSubdomain];
            if curDomain == intersect { // curDomain.isSubset(localSubdomain)
              isOnOneLocale = true;
              theLocaleId = tid;
            }
          }

          if isOnOneLocale {
            state.localTasks[theLocaleId].localTasks.append(
                new TwoArraySortTask(binStart, binSize, binStartBit, true, true));
          } else {
            state.distTasks.append(
                new TwoArraySortTask(binStart, binSize, binStartBit, true, true));
          }
        }
      }
    }

    // Handle the local tasks
    coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state) {
      on loc do {
        // Copy the tasks to do from locale 0
        var myTasks = state.localTasks[tid].localTasks;
        var baseCaseSize = state.baseCaseSize;
        ref compat = state.perLocale[tid].compat;

        for task in myTasks {
          const taskEnd = task.start + task.size - 1;
          const curDomain = {task.start..taskEnd};

          // Great! Just sort it locally.
          if n > baseCaseSize {
            compat.bigTasks.clear();
            compat.smallTasks.clear();
            partitioningSortWithScratchSpace(
                task.start, taskEnd,
                A.localSlice(curDomain), Scratch.localSlice(curDomain),
                compat, criterion, task.startbit);
          } else {
            ShellSort.shellSort(A.localSlice(curDomain), criterion,
                start=task.start, end=taskEnd);
          }
          if debug {
            writef("after recursive sorts, dst is %xt\n", A[task.start..taskEnd]);
          }
        }
      }
    }
  }
}

pragma "no doc"
module TwoArrayRadixSort {
  import Sort.defaultComparator;
  private use TwoArrayPartitioning;
  private use RadixSortHelp;

  proc twoArrayRadixSort(Data:[], comparator:?rec=defaultComparator) {

    var sequentialSizePerTask=4096;
    var baseCaseSize=16;

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    var Scratch: Data.type;

    if Data._instance.isDefaultRectangular() {
      var state = new TwoArrayBucketizerSharedState(
        bucketizer=new RadixBucketizer(),
        baseCaseSize=baseCaseSize,
        sequentialSizePerTask=sequentialSizePerTask,
        endbit=endbit);


      partitioningSortWithScratchSpace(Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state, comparator, 0);
    } else {
      var state = new TwoArrayDistributedBucketizerSharedState(
        bucketizerType=RadixBucketizer,
        numLocales=Data.targetLocales().size,
        baseCaseSize=baseCaseSize,
        endbit=endbit);


      distributedPartitioningSortWithScratchSpace(
                                       Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state,
                                       comparator, 0);
    }
  }
}


pragma "no doc"
module TwoArraySampleSort {
  import Sort.defaultComparator;
  private use TwoArrayPartitioning;
  private use SampleSortHelp;
  private use RadixSortHelp;

  proc twoArraySampleSort(Data:[], comparator:?rec=defaultComparator) {

    var baseCaseSize=16;

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    var Scratch: Data.type;

    if Data._instance.isDefaultRectangular() {
      var state = new TwoArrayBucketizerSharedState(
        bucketizer=new SampleBucketizer(Data.eltType),
        baseCaseSize=baseCaseSize,
        endbit=endbit);

      partitioningSortWithScratchSpace(Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state, comparator, 0);
    } else {
      var state = new TwoArrayDistributedBucketizerSharedState(
        bucketizerType=SampleBucketizer(Data.eltType),
        numLocales=Data.targetLocales().size,
        baseCaseSize=baseCaseSize,
        endbit=endbit);

      distributedPartitioningSortWithScratchSpace(
                                       Data.domain.low, Data.domain.high,
                                       Data, Scratch,
                                       state, comparator, 0);
    }
  }
}

pragma "no doc"
module InPlacePartitioning {
  // TODO -- based on ips4o
}


pragma "no doc"
module MSBRadixSort {
  import Sort.{defaultComparator, ShellSort};
  private use RadixSortHelp;

  // This structure tracks configuration for the radix sorter.
  record MSBRadixSortSettings {
    param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step
    const sortSwitch = 256; // when sorting <= this many elements, use shell sort
    const minForTask = 256; // when sorting >= this many elements, go parallel
    param CHECK_SORTS = false; // do costly extra checks that data is sorted
    param progress = false; // print progress
    const alwaysSerial = false; // never create tasks
    const maxTasks = here.maxTaskPar;//;here.numPUs(logical=true); // maximum number of tasks to make
  }

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
  proc msbRadixSort(start_n:int, end_n:int, A:[], criterion,
                    startbit:int, endbit:int,
                    settings /* MSBRadixSortSettings */)
  {
    if startbit > endbit then
      return;

    if( end_n - start_n < settings.sortSwitch ) {
      ShellSort.shellSort(A, criterion, start=start_n, end=end_n);
      if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
      return;
    }

    if settings.progress then writeln("radix sort start=", start_n, " end=", end_n, " startbit=", startbit, " endbit=", endbit);

    const radixbits = RADIX_BITS;
    const radix = (1 << radixbits) + 1;

    // 0th bin is for records where we've consumed all the key.
    var offsets:[0..radix] int;
    var end_offsets:[0..radix] int;
    type ubitsType = binForRecord(A[start_n], criterion, startbit)(1).type;
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

    // TODO: any_ending should be replaced by checking
    // offsets[0] and offsets[radix]

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

      // TODO: I think it might be possible to make this sort stable
      // by populating buf from the start of the data instead of from the end.
      // buf would need to be populated with the first M elements that aren't
      // already in the correct bin.

      // TODO: I think it's possible to make this shuffle parallel
      // by imagining each task has a max_buf and have them update
      // atomic offsets.
      param max_buf = settings.DISTRIBUTE_BUFFER;
      var buf: max_buf*A.eltType;
      var used_buf = 0;
      var end = end_offsets[curbin];
      var endfast = max(offsets[curbin], end_offsets[curbin]-2*max_buf);
      var bufstart = max(offsets[curbin], end_offsets[curbin]-max_buf);
      var i = bufstart;

      // Fill buf with up to max_buf records from the end of this bin.
      while i < end {
        buf[used_buf] <=> A[i];
        used_buf += 1;
        i += 1;
      }

      while offsets[curbin] < endfast {
        // Now go through the records in buf
        // putting them in their right home.
        for param j in 0..max_buf-1 {
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
        var j = 0;
        while used_buf >= 0 && j < used_buf {
          const (bin, _) = binForRecord(buf[j], criterion, startbit);
          // Swap buf[j] into its appropriate bin.
          var offset = offsets[bin];
          A[offset] <=> buf[j];
          offsets[bin] += 1;
          // Leave buf[j] with the next unsorted item.
          // But offsets[bin] might be in the region we already read.
          if bin == curbin && offset >= bufstart {
            used_buf -= 1;
            buf[j] <=> buf[used_buf];
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

  /*
   Default ``keyPart`` method for `real` values.
   See also `The .keyPart method`_.

   :arg x: the `real` of any width to sort
   :arg i: the part number requested

   :returns: ``(0, u)`` if ``i==0``, or ``(-1, u)`` otherwise,
             where `u` is a `uint` storing the bits of the `real`
             but with some transformations applied to produce the
             correct sort order.
   */
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
  /*
   Default ``keyPart`` method for `imag` values.
   See also `The .keyPart method`_.

   This method works by calling keyPart with the corresponding `real` value.
   */

  inline
  proc keyPart(x: chpl_anyimag, i:int):(int(8), uint(numBits(x.type))) {
    return keyPart(x:real(numBits(x.type)), i);
  }

  /*
   Default ``keyPart`` method for tuples of `int`, `uint`, `real`, or `imag`
   values.
   See also `The .keyPart method`_.

   :arg x: homogeneous tuple of the numeric type (of any bit width) to sort
   :arg i: the part number requested

   :returns: For `int` and `uint`, returns
             ``(0, x(i))`` if ``i <= x.size``, or ``(-1, 0)`` otherwise.
             For `real` and `imag`, uses ``keyPart`` to find the `uint`
             to provide the sorting order.
   */
  inline
  proc keyPart(x: _tuple, i:int) where isHomogeneousTuple(x) &&
                                       (isInt(x(0)) || isUint(x(0)) ||
                                        isReal(x(0)) || isImag(x(0))) {
    // Re-use the keyPart for imag, real
    const (_,part) = this.keyPart(x(i-1), 1);
    if i > x.size then
      return (-1, 0:part.type);
    else
      return (0, part);
  }

  /*
   Default ``keyPart`` method for sorting strings.
   See also `The .keyPart method`_.

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
    var len = x.numBytes;
    var section = if i <= len then 0:int(8) else -1:int(8);
    var part =    if i <= len then ptr[i-1] else  0:uint(8);
    return (section, part);
  }

  /*
   Default ``keyPart`` method for sorting `c_string`.
   See also `The .keyPart method`_.

   :arg x: the `c_string` to sort
   :arg i: the part number requested

   :returns: ``(0, byte i of string)`` or ``(-1, 0)`` if byte ``i`` is ``0``
   */

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
      return typeIsBitReversible(tmp(0).type);
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
      return typeIsNegateReversible(tmp(0).type);
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
    return canResolveMethod(this.comparator, "keyPart", a, 1);
  }
  pragma "no doc"
  proc hasKeyPartFromKey(a) param {
    if canResolveMethod(this.comparator, "key", a) {
      var key:comparator.key(a).type;
      // Does the defaultComparator have a keyPart for this?
      return canResolveMethod(defaultComparator, "keyPart", key, 1);
    }
    return false;
  }

  pragma "no doc"
  proc hasCompare(a,b) param {
    return canResolveMethod(this.comparator, "compare", a, b);
  }
  pragma "no doc"
  proc hasCompareFromKey(a) param {
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
