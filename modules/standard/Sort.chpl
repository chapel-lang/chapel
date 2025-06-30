/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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


// TODO: rewrite these docs to describe the interfaces

// TODO -- performance test sort routines and optimize (see other TODO's)
/*

This module supports standard algorithms for sorting data.
It is designed to
be flexible and efficient, allowing the user to define custom comparators to
sort any data type, as long as the comparator implements the appropriate
sorting interface.

The simplest way to sort an array is to call the :proc:`sort` function on the
array. The sort function will use the default comparator to sort the array in
ascending order.

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/basicSort.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

The sort function can also accept a region argument to sort a subset of an
array. This is offered as an optimization over using an array slice which may
have performance overhead.

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/regionSort.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

The sort function can also be called on a list, be stable or unstable, and
accept a custom comparator.
See the :proc:`sort(x: list)<Sort.sort>` function for details.

.. _comparators:

Comparators
-----------

Comparators allow sorting data by a mechanism other than the
default comparison operations between array elements.

The :proc:`sort` function can accept a comparator argument, which defines how
the data is sorted. If no comparator is passed, the default comparator is
used.

Reverse sorting is handled by the :record:`reverseComparator`.
See :ref:`Reverse Comparator<reverse-comparator>` for details.


To use a custom comparator, define a record or a class which implements the
appropriate sorting interface.

Comparators need to implement one, and only one, of the following interfaces
as well as at least one of their associated methods:

  * :interface:`keyComparator` -- see `The keyComparator interface`_
  * :interface:`relativeComparator` -- see `The relativeComparator interface`_
  * :interface:`keyPartComparator` -- see `The keyPartComparator interface`_

See the section below for discussion of each of these interfaces and methods.

*Future:*

  Provide a unified ``sortComparator`` interface, which can represent an
  exclusive or (XOR) of the three interfaces above.


The keyComparator interface
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``keyComparator`` interface is used to sort data by a key value. Records
implementing this interface must define a ``key`` method.

Today, it is an error for a comparator implementing the ``keyComparator``
interface to contain a ``key`` method as well as one of the other methods
that are part of the ``relativeComparator`` or ``keyPartComparator``
interfaces. This restriction might be lifted in future releases.

The .key method
***************

The ``key(elt)`` method accepts 1 argument, which will be an element from the
array being sorted.

The default key method would look like this:

.. code-block:: chapel

  proc defaultComparator.key(elt) {
    return elt;
  }


As an example, if the user wants to sort an array by the absolute values of its
elements, the user can define a comparator with a key method as follows:

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/keyComparator.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

The return type of ``key(elt)`` must support the ``<``
operator, which is used by the base compare method of all sort routines. If the
``<`` operator is not defined for the return type, the user may define it
themselves like so:

.. code-block:: chapel

  operator <(x: returnType, y: returnType): bool {
    ...
  }


The relativeComparator interface
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``relativeComparator`` interface is used to sort data by comparing two
elements directly. Records implementing this interface must define a
``compare`` method.

The .compare method
*******************

The ``compare(x, y)`` method accepts 2 arguments, which will be 2 elements from
the array being sorted. The return value should be a numeric signed type
indicating how x and y compare to each other. The conditions between ``x`` and
``y`` should result in the following return values for ``compare(x, y)``:

  ============ ==========
  Return Value Condition
  ============ ==========
  ``> 0``      ``x > y``
  ``0``        ``x == y``
  ``< 0``      ``x < y``
  ============ ==========

The default compare method for a signed integral type can look like this:

.. code-block:: chapel

    proc defaultComparator.compare(x, y) {
      if x < y then return -1;
      else if x > y then return 1;
      else return 0;
    }


The absolute value comparison example from above can alternatively be
implemented with a ``relativeComparator`` as follows:

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/compareComparator.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

The keyPartComparator interface
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The keyPartComparator interface defines how a comparator should sort parts of
a key using the ``keyPart`` method. This is used for certain sort algorithms.
Records implementing this interface must define a ``keyPart`` method.

A comparator implementing this interface can optionally also provide a
`compare` method. In that event, the sort algorithm will use whichever is
appropriate for the algorithm and expect that they have consistent results.

The .keyPart method
*******************

A ``keyPart(elt, i)`` method returns *parts* of key value at a time. This
interface supports radix sorting for variable length data types, such as
strings. It accepts two arguments:

 * ``elt`` is the element being sorted
 * ``i`` is the part number of the key requested, starting from 0

A ``keyPart`` method should return a tuple consisting of *section* and a *part*.

 * The *section* must be of type :type:`keyPartStatus`. It indicates when the
   end of ``elt`` has been reached and in that event how it should be sorted
   relative to other array elements.

 * The *part* can be any signed or unsigned integral type and can contain any
   value. The *part* will be ignored unless the *section* returned is
   :enumconstant:`keyPartStatus.returned`.


Let's consider several example ``keyPart`` methods. All of these are
simplifications of ``keyPart`` methods already available in the
:type:`defaultComparator`.

This ``keyPart`` method supports sorting tuples of 2 integers:

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/keyPartComparator.chpl
   :language: chapel
   :start-after: START_EXAMPLE_TUPLE_INT
   :end-before: STOP_EXAMPLE_TUPLE_INT

Here is a ``keyPart`` to support sorting of strings:

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/keyPartComparator.chpl
   :language: chapel
   :start-after: START_EXAMPLE_STRING
   :end-before: STOP_EXAMPLE_STRING

.. _reverse-comparator:

Reverse Comparator
~~~~~~~~~~~~~~~~~~

Sort functions in Chapel do not have a ``reverse`` argument. Instead, reverse
sorting is handled through the comparator interface.

An instance of the type :record:`reverseComparator` can be passed to a sort
function to reverse the default sorting order.

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/reversedSort.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE

To reverse the sort order of a user-defined comparator, pass the user-defined
comparator to the initializer of the module-defined
:record:`reverseComparator` record, which can be passed to the sort function.

For this example, we will reverse the absolute value comparison from above
using the ``relativeComparator`` interface, although the same can be done with
the ``keyComparator`` interface.

.. literalinclude:: ../../../../test/library/standard/Sort/doc-examples/reversedCustomSort.chpl
   :language: chapel
   :start-after: START_EXAMPLE
   :end-before: STOP_EXAMPLE
*/
module Sort {

  private use List;
  private use Reflection;
  private use CTypes;

/* Private methods */

private inline
proc compareByPart(a:?t, b:t, comparator:?rec) {
  var curPart = 0;
  while true {
    var (aSection, aPart) = comparator.keyPart(a, curPart);
    var (bSection, bPart) = comparator.keyPart(b, curPart);
    if aSection:int != 0 || bSection:int != 0 {
      return aSection:int - bSection:int;
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
inline proc chpl_compare(a:?t, b:t, comparator:?rec) {
  // this innerFunc is a hack so that errorDepth is correct.
  // it has to be 3 to match the errorDepth of chpl_check_comparator, otherwise
  // users will get additional and confusing error messages.
  proc innerFunc() param do
    chpl_check_comparator_helper(comparator, a, t, errorDepth=3);
  innerFunc();
  // TODO -- In cases where values are larger than keys, it may be faster to
  //         key data once and sort the keyed data, mirroring swaps in data.
  // Compare results of comparator.key(a) if is defined by user
  if canResolveMethod(comparator, "key", a) {
    // Use the default comparator to compare the integer keys
    return (new defaultComparator()).compare(comparator.key(a),
                                             comparator.key(b));
  // Use comparator.compare(a, b) if is defined by user
  } else if canResolveMethod(comparator, "compare", a, b) {
    return comparator.compare(a, b);
  } else if canResolveMethod(comparator, "keyPart", a, 0) {
    return compareByPart(a, b, comparator);
  } else {
    proc baseComparatorType(type c) type {
      if c == defaultComparator then
        return c;
      else if isSubtype(c, reverseComparator) then
        return baseComparatorType(c.comparator);
      else
        return c;
    }
    if baseComparatorType(comparator.type) == defaultComparator {
      compilerError(
        "The defaultComparator does not support sorting elements of type '",
        t:string, "'");
    } else {
      compilerError(
        "The comparator ", comparator.type:string,
        " must implement either 'keyComparator', 'keyPartComparator', or ",
        "'relativeComparator' for ", t:string);
    }
  }
}


// helper for chpl_check_comparator, should not be called elsewhere
private proc chpl_check_comparator_keyPart(comparator,
                                           data,
                                           type eltType,
                                           param errorDepth = 2,
                                           param doDeprecationCheck = true) param {
  if canResolveMethod(comparator, "keyPart", data, 0) {
    if comparatorImplementsKeyPart(comparator) {
      var idx: int = 0;
      type partType = comparator.keyPart(data, idx).type;
      if !isTupleType(partType) then
        compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple when used with ", eltType:string, " elements");
      var tmp: partType;
      var expectKeyPartStatus = tmp(0);
      var expectIntUint = tmp(1);
      if expectKeyPartStatus.type != keyPartStatus then
        compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with element 0 of type keyPartStatus when used with ", eltType:string, " elements");
      if !(isInt(expectIntUint) || isUint(expectIntUint)) then
        compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with element 1 of type int(?) or uint(?) when used with ", eltType:string, " elements");
    } else {
      if doDeprecationCheck {
        param atType = if isRecord(comparator) then "record" else "class";
        param fixString = "'" + atType + " " +
                              comparator.type:string + ": keyPartComparator'";
        compilerWarning(errorDepth=errorDepth,
          "Defining a comparator with a 'keyPart' method without " +
          "implementing the keyPartComparator interface is deprecated. " +
          "Please implement the keyPartComparator interface (i.e. " + fixString + ").");
      }

      var idx: int = 0;
      type partType = comparator.keyPart(data, idx).type;
      if !isTupleType(partType) then
        compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple when used with ", eltType:string, " elements");
      var tmp: partType;
      var expectInt = tmp(0);
      var expectIntUint = tmp(1);
      if !isInt(expectInt.type) then
        compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with element 0 of type int(?) when used with ", eltType:string, " elements");
      if !(isInt(expectIntUint) || isUint(expectIntUint)) then
        compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with element 1 of type int(?) or uint(?) when used with ", eltType:string, " elements");
    }
    return true;
  }
  return false;
}

private proc chpl_check_comparator_helper(comparator,
                                          data,
                                          type eltType,
                                          param errorDepth = 3,
                                          param doDeprecationCheck = true) param {
  // if more than 1 interface is implemented, error
  if (comparatorImplementsKey(comparator):int +
      comparatorImplementsKeyPart(comparator):int +
      comparatorImplementsRelative(comparator):int) > 1 {
    compilerError(errorDepth=errorDepth, "The comparator " + comparator.type:string + " should only implement one sort comparator interface.");
  }

  if comparator.type == defaultComparator {}
  else if isSubtype(comparator.type, reverseComparator) {
    return chpl_check_comparator_helper(comparator.comparator, data, eltType, errorDepth+1);
  }
  // Check for valid comparator methods
  else if canResolveMethod(comparator, "key", data) {
    // Check return type of key
    const keydata = comparator.key(data);
    type keytype = keydata.type;
    if !(canResolve("<", keydata, keydata)) then
      compilerError(errorDepth=errorDepth,
        "The key method in ",
        comparator.type:string,
        " must return an object that supports the '<' function when used with ",
        eltType:string,
        " elements");

    // TODO: I think these can be removed once we enforce the interface in
    //       the compiler and remove `canResolveMethod`
    // Check that there isn't also a compare or keyPart
    if canResolveMethod(comparator, "compare", data, data) {
      compilerError(errorDepth=errorDepth,
        comparator.type:string,
        " contains both a key method and a compare method");
    }
    if canResolveMethod(comparator, "keyPart", data, 0) {
      compilerError(errorDepth=errorDepth,
        comparator.type:string,
        " contains both a key method and a keyPart method");
    }
    if doDeprecationCheck && !comparatorImplementsKey(comparator) {
      param atType = if isRecord(comparator) then "record" else "class";
      param fixString = "'" + atType + " " +
                        comparator.type:string + ": keyComparator'";
      compilerWarning(errorDepth=errorDepth,
        "Defining a comparator with a 'key' method without " +
        "implementing the keyComparator interface is deprecated. " +
        "Please implement the keyComparator interface (i.e. " + fixString + ").");
    }
  }
  else if canResolveMethod(comparator, "compare", data, data) {
    if doDeprecationCheck {
      if !comparatorImplementsRelative(comparator) &&
         !comparatorImplementsKeyPart(comparator) {
        // if there is a keyPart method, we should use that interface
        param hasKeyPart = canResolveMethod(comparator, "keyPart", data, 0);
        param atType = if isRecord(comparator) then "record" else "class";
        param fixString = "'" + atType + " " +
                              comparator.type:string + ": relativeComparator'";
        if !hasKeyPart {
          compilerWarning(errorDepth=errorDepth,
            "Defining a comparator with a 'compare' method without " +
            "implementing the relativeComparator interface is deprecated. " +
            "Please implement the relativeComparator interface (i.e. " + fixString + ").");
        } else {
          compilerWarning(errorDepth=errorDepth,
            "Defining a comparator with both a 'compare' method and a 'keyPart' without " +
            "implementing the keyPartComparator interface is deprecated. " +
            "Please implement the keyPartComparator interface (i.e. " + fixString + ").");
        }
      }
    }
    // Check return type of compare
    type comparetype = comparator.compare(data, data).type;
    if !(isNumericType(comparetype)) then
      compilerError(errorDepth=errorDepth, "The compare method in ", comparator.type:string, " must return a numeric type when used with ", eltType:string, " elements");

    // if the user has implemented the keyPart interface, we also have to check
    // that the keyPart method is implemented correctly to satisfy the interface
    if comparatorImplementsKeyPart(comparator) then
      if !chpl_check_comparator_keyPart(comparator,
                                        data,
                                        eltType,
                                        errorDepth+1,
                                        doDeprecationCheck=false) then
        compilerError(errorDepth=errorDepth, "The comparator " + comparator.type:string + " implements the keyPartComparator interface, but the keyPart method is not implemented");
  } else if chpl_check_comparator_keyPart(comparator, data, eltType, errorDepth+1, doDeprecationCheck) {
    // the check and error are in chpl_check_comparator_keyPart
  }
  else {
    // If we make it this far, the passed comparator was defined incorrectly
    if comparatorImplementsKey(comparator) {
      compilerError(errorDepth=errorDepth,
        "The comparator ", comparator.type:string,
        " implements 'keyComparator' but does not correctly provide a ",
        "'key' method for ", eltType:string);
    } else if comparatorImplementsKeyPart(comparator) {
      compilerError(errorDepth=errorDepth,
        "The comparator ", comparator.type:string,
        " implements 'keyPartComparator' but does not correctly provide a ",
        "'keyPart' method for ", eltType:string);
    } else if comparatorImplementsRelative(comparator) {
      compilerError(errorDepth=errorDepth,
        "The comparator ", comparator.type:string,
        " implements 'relativeComparator' but does not correctly provide a ",
        "'compare' method for ", eltType:string);
    } else {
      compilerError(errorDepth=errorDepth,
        "The comparator ", comparator.type:string,
        " must implement either 'keyComparator', 'keyPartComparator', or ",
        "'relativeComparator' for ", eltType:string);
    }
  }

  return true;
}

pragma "unsafe" // due to 'data' default-initialized to nil for class types
/*
    Check if a comparator was passed and confirm that it will work, otherwise
    throw a compile-time error.

   :arg a: Sample data passed to confirm that comparator methods can resolve
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

*/
proc chpl_check_comparator(comparator,
                           type eltType,
                           param errorDepth = 2,
                           param doDeprecationCheck = true) param {
  // Dummy data for checking method resolution
  // This may need updating when constructors support non-default args
  const data: eltType;

  return chpl_check_comparator_helper(comparator, data, eltType, errorDepth+1, doDeprecationCheck);
}

/* Basic Functions */

pragma "unsafe" // due to 'tmp' default-initialized to nil for class types
private
proc radixSortOkAndStrideOne(Data: [] ?eltType,
                             comparator,
                             region: range(?)) param {
  if region.strides == strideKind.one {
    var tmp:Data[Data.domain.low].type;
    if canResolveMethod(comparator, "keyPart", tmp, 0) {
      return true;
    } else if canResolveMethod(comparator, "key", tmp) {
      var key:comparator.key(tmp).type;
      // Does the defaultComparator have a keyPart for this?
      if canResolveMethod(new defaultComparator(), "keyPart", key, 0) then
        return true;
    }
  }
  return false;
}

private proc unstableSort(ref x: [], comparator, region: range(?)) {
  if region.low >= region.high then
    return;

  if !chpl_domainDistIsLayout(x.domain) {
    // sort distributed arrays using Partitioning
    if !isPODType(x.eltType) {
      compilerError("distributed sort currently only works with trivially copyable types");
    }
    if x.domain.rank != 1 {
      compilerError("distributed sort currently only works with rank 1 arrays");
    }

    // TODO: if it is small, copy it locally and sort it,
    // without allocating these arrays
    var Scratch: [x.domain] x.eltType;
    var BucketBoundaries: [x.domain] uint(8);

    param logBuckets = 8;

    if radixSortOkAndStrideOne(x, comparator, region) {
      var endbit:int;
      endbit = RadixSortHelp.radixSortParamEndBit(x, comparator);
      if endbit < 0 then
        endbit = max(int);

      Partitioning.psort(x, Scratch, BucketBoundaries,
                         x.domain.dim(0),
                         comparator,
                         radixBits=logBuckets,
                         logBuckets=logBuckets,
                         nTasksPerLocale=PartitioningUtility.computeNumTasks(),
                         endbit=endbit);
    } else {
      Partitioning.psort(x, Scratch, BucketBoundaries,
                         x.domain.dim(0),
                         comparator,
                         radixBits=0,
                         logBuckets=logBuckets,
                         nTasksPerLocale=PartitioningUtility.computeNumTasks(),
                         endbit=max(int));
    }
  } else {
    // sort local arrays using two-array or quicksort for now

    if radixSortOkAndStrideOne(x, comparator, region) {
      // TODO: use a sample sort if the input does not have enough
      // randomness, according to some heuristic

      var simplerSortSize=50_000;
      if region.size < simplerSortSize {
        // TODO: use quicksort instead in these small cases
        MSBRadixSort.msbRadixSort(x, comparator=comparator, region=region);
        return;
      }

      // use the two-array radix sort which is more parallel / faster
      TwoArrayRadixSort.twoArrayRadixSort(x, comparator=comparator,
                                          region=region);
    } else {
      // use quick sort, which is currently in-place
      // TODO: use a parallel sample sort instead
      // TODO: use Partitioning.psort here since that is faster
      QuickSort.quickSort(x, comparator=comparator, region=region);
    }
  }
}

private proc stableSort(ref x: [], comparator, region: range(?)) {
  if region.low >= region.high then
    return;

  if !chpl_domainDistIsLayout(x.domain) {
    // sort distributed arrays using Partitioning
    if !isPODType(x.eltType) {
      compilerError("distributed sort currently only works with trivially copyable types");
    }
    if x.domain.rank != 1 {
      compilerError("distributed sort currently only works with rank 1 arrays");
    }

    // TODO: if it is small, copy it locally and sort it,
    // without allocating these arrays
    var Scratch: [x.domain] x.eltType;
    var BucketBoundaries: [x.domain] uint(8);

    param logBuckets = 8;

    if radixSortOkAndStrideOne(x, comparator, region) {
      var endbit:int;
      endbit = RadixSortHelp.radixSortParamEndBit(x, comparator);
      if endbit < 0 then
        endbit = max(int);

      Partitioning.psort(x, Scratch, BucketBoundaries,
                         x.domain.dim(0),
                         comparator,
                         radixBits=logBuckets,
                         logBuckets=logBuckets,
                         nTasksPerLocale=PartitioningUtility.computeNumTasks(),
                         endbit=endbit);
    } else {
      Partitioning.psort(x, Scratch, BucketBoundaries,
                         x.domain.dim(0),
                         comparator,
                         radixBits=0,
                         logBuckets=logBuckets,
                         nTasksPerLocale=PartitioningUtility.computeNumTasks(),
                         endbit=max(int));
    }
  } else {
    // TODO: use Partitioning.psort here since that is stable

    // TODO: implement a stable merge sort with parallel merge
    // TODO: create an in-place merge sort for the stable+minimizeMemory case
    // TODO: create a stable variant of the radix sort
    TimSort.timSort(x, comparator=comparator, region=region);
  }
}

/*

Sort the elements in the 1D rectangular array ``x``.
After the call, ``x`` will store elements in sorted order.

The choice of sorting algorithm used is made by the implementation.

At present, non-distributed arrays and Block-distributed arrays can be sorted.

.. note::

  When reordering elements, the sort implementation might use assignment, memory
  moves, or the swap operator. Additionally, the sort might
  copy-initialize some elements, for example, to create a pivot in quicksort.

.. note::

  This function chooses among the following algorithms based on the arguments:

   * a local, parallel radix sort
   * a parallel improved quick sort
   * TimSort
   * a distributed, parallel radix sort
   * a distributed, parallel sample sort

  The algorithms used will change over time. Currently, the choice process is:

   * use the local radix sort for ``stable=false`` sorting a non-distributed
     array where radix sorting is possible (see below)
   * use the improved quick sort for ``stable=false`` sorting of a
     non-distributed array when radix sorting is not possible
   * use TimSort for ``stable=true`` sorting of a non-distributed array
   * use a distributed, parallel radix sort when sorting a distributed array
     where radix sorting is possible (for both ``stable=true`` and
     ``stable=false``)
   * use a distributed, parallel sample sort when sorting a distributed array
     and radix sorting is not possible (for both ``stable=true`` and
     ``stable=false``).

  Radix sorting is possible if the following conditions are met:

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

:arg x: The array to be sorted
:type x: `array`
:arg comparator: :ref:`Comparator <comparators>` record that defines how the
  data is sorted.
:arg stable: Defaults to ``false``. If it is ``false``, the implementation
  can sort in a way that reorders equal keys. If it is ``true``, it will use a
  stable algorithm in order to preserve the order of equal keys.
 */
proc sort(ref x: [], comparator:? = new defaultComparator(),
          param stable:bool = false) {
  chpl_check_comparator(comparator, x.eltType);

  if stable {
    stableSort(x, comparator, x.domain.dim(0));
  } else {
    unstableSort(x, comparator, x.domain.dim(0));
  }
}

/*

Sort the elements in the list ``x``. After the call, ``x`` will store elements
in sorted order.

See :proc:`sort` declared above for details.

.. warning::

  Sorting the elements of a list may invalidate existing references
  to the elements contained in the list.

:arg x: The list to be sorted
:type x: :type:`~List.list`
:arg comparator: :ref:`Comparator <comparators>` record that defines how the
  data is sorted.
:arg stable: Defaults to ``false``. If it is ``false``, the implementation
  can sort in a way that reorders equal keys. If it is ``true``, it will use a
  stable algorithm in order to preserve the order of equal keys.
 */
proc sort(ref x: list(?), comparator:? = new defaultComparator(),
          param stable:bool = false) {
  chpl_check_comparator(comparator, x.eltType);
  // NOTE: this uses very low-level and non-public list methods to avoid overheads
  on x {
    x._enter();
    // TODO: This is not ideal, how do we communicate the internals of List to Sort?
    if x._size > 1 {
      // Copy current list contents into an array.
      var arr: [0..#x._size] x.eltType;
      for i in 0..#x._size do
        arr[i] = x._getRef(i);
      Sort.sort(arr, comparator=comparator, stable=stable);
      // This is equivalent to the clear routine.
      x._fireAllDestructors();
      x._freeAllArrays();
      x._firstTimeInitializeArrays();
      x._appendGeneric(arr);
    }
    x._leave();
  }
}

/*

Sort the elements in the range 'region' within in the 1D rectangular array
``x``.  After the call, ``x[region]`` will store elements in sorted order.
This function accepts a 'region' range as an optimized alternative to using an
array view.

See the :proc:`sort` declared just above for details.

.. note::

  Due to uncertainty about the usefulness of this routine, it is unstable.
  Please comment on https://github.com/chapel-lang/chapel/issues/25648 if
  you find this routine important in your work.

 */
@unstable("sort with a region argument is unstable")
proc sort(ref x: [], comparator, region: range(?), param stable:bool = false) {
  chpl_check_comparator(comparator, x.eltType);

  if stable {
    stableSort(x, comparator, region);
  } else {
    unstableSort(x, comparator, region);
  }
}


/*

Sort the elements in the 1D rectangular array ``Data``.
After the call, ``Data`` will store elements in sorted order.

The choice of sorting algorithm used is made by the implementation.

.. note::

  When reordering elements, the sort implementation might use assignment, memory
  moves, or the swap operator. Additionally, the sort might
  copy-initialize some elements, for example, to create a pivot in quicksort.

.. note::

  This function currently either uses a parallel radix sort or a parallel
  improved quick sort.  For stable sort, use :proc:`sort` with ``stable=true``.
  The algorithms used will change over time.

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
:arg stable: Defaults to ``false``. If it is ``false``, the implementation
  can sort in a way that reorders equal keys. If it is ``true``, it will use a
  stable algorithm in order to preserve the order of equal keys.
:arg inPlaceAlgorithm: Defaults to ``false``. If it is ``false``, the
  implementation can make a copy of ``Data`` for scratch storage during the
  sort. If it is ``true``, it will use an in-place algorithm in order to use
  less memory.
 */
pragma "last resort"
@deprecated("The 'sort' function with 'Data' and 'inPlaceAlgorithm' arguments has been deprecated, please use the 'sort' function with an 'x' argument instead")
proc sort(ref Data: [?Dom] ?eltType, comparator:?rec= new defaultComparator(),
          param stable:bool = false, param inPlaceAlgorithm:bool = false) {
  chpl_check_comparator(comparator, eltType);

  if stable {
    // TODO: we already have a stable sort, but it is not called here
    // maybe we should call it here, even though this one is deprecated
    // TODO: implement a stable merge sort with parallel merge
    // TODO: create an in-place merge sort for the stable+minimizeMemory case
    // TODO: create a stable variant of the radix sort
    compilerError("stable sort not yet implemented");
  } else {
    unstableSort(Data, comparator, Data.domain.dim(0));
  }
}


@chpldoc.nodoc
/* Error message for multi-dimension arrays */
proc sort(ref x: [?Dom] , comparator:? = new defaultComparator(), param stable:bool = false)
  where Dom.rank != 1 || !x.isRectangular() {
    compilerError("sort() is currently only supported for 1D rectangular arrays");
}
@chpldoc.nodoc
proc sort(ref x: domain,
          comparator:? = new defaultComparator(),
          param stable:bool = false) do
  compilerError("sort() is not supported on domains");

/*
   Check if array `x` is in sorted order

   :arg x: The array to verify
   :type x: `array`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :returns: ``true`` if array is sorted
   :rtype: `bool`
 */
proc isSorted(x: [], comparator:? = new defaultComparator()): bool {
  chpl_check_comparator(comparator, x.eltType);

  const D = x.domain;

  const stride = abs(D.stride): D.idxType;
  var sorted = true;
  forall (element, i) in zip(x, D) with (&& reduce sorted) {
    if i > D.low {
      sorted &&= (chpl_compare(x[i-stride], element, comparator) <= 0);
    }
  }
  return sorted;
}

/*
   Check if list `x` is in sorted order

   :arg x: The list to verify
   :type x: :type:`~List.list`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :returns: ``true`` if list is sorted
   :rtype: `bool`
 */
proc isSorted(x: list(?), comparator:? = new defaultComparator()): bool {
  chpl_check_comparator(comparator, x.eltType);

  // NOTE: this uses very low-level and non-public list methods to avoid overheads
  var sorted = true;
  on x {
    x._enter();
    var sortedLocal = true;
    forall i in 1..<x._size with (&& reduce sortedLocal) do
      sortedLocal &&= (chpl_compare(x._getRef(i-1), x._getRef(i), comparator) <= 0);
    sorted = sortedLocal;
    x._leave();
  }

  return sorted;
}


@chpldoc.nodoc
/* Error message for multi-dimension arrays */
proc isSorted(x: [], comparator:? = new defaultComparator())
  where x.rank != 1 || !x.isRectangular() {
    compilerError("isSorted() is currently only supported for 1D rectangular arrays");
}
@chpldoc.nodoc
proc isSorted(x: domain, comparator:? = new defaultComparator()) do
  compilerError("isSorted() is not supported on domains");

/*
   Check if array `Data` is in sorted order

   :arg Data: The array to verify
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :returns: ``true`` if array is sorted
   :rtype: `bool`
 */
pragma "last resort"
@deprecated("'isSorted' with the argument name 'Data' is deprecated, please use the version with the argument name 'x' instead")
proc isSorted(Data: [?Dom] ?eltType, comparator:?rec= new defaultComparator()): bool {
  return isSorted(x=Data, comparator);
}

@chpldoc.nodoc
iter sorted(x : domain, comparator:? = new defaultComparator()) {
  if !x.isAssociative() then
    compilerError("sorted() is currently only supported on associative domains");
  for i in x._value.dsiSorted(comparator) {
    yield i;
  }
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
   Yield the elements of argument `x` in sorted order, using the same algorithm
   as :proc:`sort`.

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
iter sorted(x, comparator:? = new defaultComparator()) {
  if isArrayValue(x) && Reflection.canResolveMethod(x._value, "dsiSorted", comparator)
  {
    // As far as I know this branch is only encountered for associative arrays
    for i in x._value.dsiSorted(comparator) {
      yield i;
    }
  } else if isArrayValue(x) && Reflection.canResolveMethod(x._value, "dsiSorted") {
    compilerError(x._value.type:string + " does not support dsiSorted(comparator)");
  } else if isArrayValue(x) && x.isSparse() {
    compilerError("sorted() is not supported on sparse arrays");
  }
  else {
    var y = x; // need to do before isArrayValue test in case x is an iterable
    param iterable = isArrayValue(y) || isSubtype(y.type, List.list(?));
    if iterable {
      sort(y, comparator=comparator);
      for i in y do
        yield i;
    } else {
      compilerError("Sort.sorted called on non-iterable type. Type is: " + x.type : string);
    }
  }
}

// general helpers
include module RadixSortHelp;
include module ShallowCopy;

// sort implementations
include module InsertionSort;
include module TimSort;
include module QuickSort;
include module ShellSort;
include module MSBRadixSort;

// more sort implementations & support modules for them
include module TwoArrayPartitioning;
include module TwoArrayRadixSort;
include module TwoArrayDistributedPartitioning;
include module TwoArrayDistributedRadixSort;

// and the partitioning sort modules
include module PartitioningUtility;
include module Partitioning;

/* Comparators */

/*
  The keyComparator interface defines how a comparator should sort elements by
  returning a *key* for each element in the array.
*/
@unstable("keyComparator is not yet stable")
interface keyComparator {
  /*
    Given an array element, returns a key element to sort by.

    :arg elt: the array element being compared
    :returns: a *key* element to sort by
    :rtype: a type that support '<'
  */
  pragma "docs only"
  proc Self.key(elt);// due to current limitations this signature is only for chpldoc
}

private proc comparatorImplementsKey(cmp) param do
  return __primitive("implements interface", cmp, keyComparator) != 2;

/*
  Indicates when the end of an element has been reached and in that event how
  it should be sorted relative to other array elements.
*/
enum keyPartStatus {
  /* No more key parts for element, sort it before those with more parts */
  pre = -1,
  /* A key part for element is being returned */
  returned = 0,
  /* No more key parts for element, sort it after those with more parts */
  post = 1
}

private inline proc reverseKeyPartStatus(status: keyPartStatus): keyPartStatus do
  return try! (status:int * -1): keyPartStatus;
private inline proc reverseKeyPartStatus(status) do
  return -status;

// TODO: this is a hack to workaround issues with interfaces
/*
  The keyPartComparator interface defines how a comparator should sort parts of
  a key, by defining :proc:`~keyPartComparator.Self.keyPart`. This is used for
  certain sort algorithms. If :proc:`~keyPartComparator.Self.keyPart` is not
  appropriate, the sort implementation may use
  :proc:`~keyPartComparator.Self.compare` instead.
*/
@unstable("keyPartComparator is not yet stable")
interface keyPartComparator {

  /*
    A ``keyPart(elt, i)`` method returns *parts* of key value at a time. This
    interface supports radix sorting for variable length data types, such as
    strings. It accepts two arguments:

    * ``elt`` is the element being sorted
    * ``i`` is the part number of the key requested, starting from 0

    A ``keyPart`` method should return a tuple consisting of *section* and a *part*.

    * The *section* must be of type :type:`keyPartStatus`. It indicates when the
      end of ``elt`` has been reached and in that event how it should be sorted
      relative to other array elements.

    * The *part* can be any signed or unsigned integral type and can contain any
      value. The *part* will be ignored unless the *section* returned is
      :enumconstant:`keyPartStatus.returned`.

    :arg elt: the element being sorted
    :arg i: the part number requested
    :returns: ``(section, part)`` where ``section`` is a :type:`keyPartStatus`
              and ``part`` is an integral type.
  */
  pragma "docs only"
  proc Self.keyPart(elt, i: int): (keyPartStatus, integral); // due to current limitations this signature is only for chpldoc

  /*
    Defines a comparison between two elements of the same type. This method is
    not required to be implemented by comparators that implement the
    :interface:`keyPartComparator` interface.

    :arg x: the first element to compare
    :arg y: the second element to compare
    :returns: -1 if ``x`` should be sorted before ``y``,
               1 if ``x`` should be sorted after ``y``,
               and 0 if ``x`` and ``y`` are equal
    :rtype: a signed integral
  */
  pragma "docs only"
  proc Self.compare(x, y: x.type) { // due to current limitations this signature is only for chpldoc
    if x < y      then return -1;
    else if y < x then return 1;
    else          return 0;
  }
}
private proc comparatorImplementsKeyPart(cmp) param do
  return __primitive("implements interface", cmp, keyPartComparator) != 2;

@chpldoc.nodoc
@deprecated("'useKeyPartStatus' is deprecated and inactive. comparator keyPart methods must now return the keyPartStatus enum rather than an integer")
config param useKeyPartStatus = true;

// TODO: this is a hack to workaround issues with interfaces
/*
  The relativeComparator interface defines a comparison between two elements
*/
@unstable("relativeComparator is not yet stable")
interface relativeComparator {
  /*
    Defines a comparison between two elements of the same type.

    :arg x: the first element to compare
    :arg y: the second element to compare
    :returns: -1 if ``x`` should be sorted before ``y``,
               1 if ``x`` should be sorted after ``y``,
               and 0 if ``x`` and ``y`` are equal
    :rtype: a signed integral
  */
  pragma "docs only"
  proc Self.compare(x, y: x.type); // due to current limitations this signature is only for chpldoc
}
private proc comparatorImplementsRelative(cmp) param do
  return __primitive("implements interface", cmp, relativeComparator) != 2;

// TODO: this represents the mutually exclusive OR of keyComparator,
//       keyPartComparator, and relativeComparator
//       This cannot be represented in Chapel today, but we still want to
//       reserve the identifier.
//       See https://github.com/chapel-lang/chapel/issues/25554.
@chpldoc.nodoc // keep this nodoc since its not implemented yet
@unstable("sortComparator is not yet stable")
interface sortComparator { }

/* Default comparator used in sort functions.*/
record defaultComparator: keyPartComparator {

  /*
   Default compare method used in sort functions.
   Uses the `<` operator to compute the ordering between ``x`` and ``y``.
   See also `The .compare method`_.

   :returns: 1 if ``y < x``
   :returns: 0 if ``x == y``
   :returns: -1 if ``x < y``
   */
  inline
  proc compare(x, y: x.type) {
    if x < y { return -1; }
    else if y < x { return 1; }
    else return 0;
  }

  /*
    Default ``keyPart`` method for integral values.
    See also `The .keyPart method`_.

    :arg elt: the `int` or `uint` of any size to sort
    :arg i: the part number requested

    :returns: ``(keyPartStatus.returned, x)`` if ``i==0``, or
              ``(keyPartStatus.pre, x)`` otherwise
   */
  inline proc keyPart(elt: integral, i: int): (keyPartStatus, elt.type) {
    var section = if i > 0 then keyPartStatus.pre else keyPartStatus.returned;
    return (section, elt);
  }

  /*
    Default ``keyPart`` method for `real` values.
    See also `The .keyPart method`_.

    :arg elt: the `real` of any width to sort
    :arg i: the part number requested

    :returns: ``(keyPartStatus.returned, u)`` if ``i==0``, or
              ``(keyPartStatus.pre, u)`` otherwise,
              where `u` is a `uint` storing the bits of the `real`
              but with some transformations applied to produce the
              correct sort order.
  */
  inline proc keyPart(elt: real(?), i:int): (keyPartStatus, uint(numBits(elt.type))) {
    import OS.POSIX.memcpy;
    var section = if i > 0 then keyPartStatus.pre else keyPartStatus.returned;

    param nbits = numBits(elt.type);
    // Convert the real bits to a uint
    var src = elt;
    var dst: uint(nbits);
    memcpy(c_ptrTo(dst), c_ptrTo(src), c_sizeof(src.type));

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
  inline proc keyPart(elt: imag(?), i: int): (keyPartStatus, uint(numBits(elt.type))) {
    return keyPart(elt:real(numBits(elt.type)), i);
  }

  /*
    Default ``keyPart`` method for tuples of `int`, `uint`, `real`, or `imag`
    values.
    See also `The .keyPart method`_.

    :arg elt: homogeneous tuple of the numeric type (of any bit width) to sort
    :arg i: the part number requested

    :returns: For `int` and `uint`, returns
              ``(keyPartStatus.pre, elt(i))`` if ``i < elt.size``,
              or ``(keyPartStatus.returned, 0)`` otherwise.
              For `real` and `imag`, uses ``keyPart`` to find the `uint`
              to provide the sorting order.
  */
  inline proc keyPart(elt: _tuple, i: int) where isHomogeneousTuple(elt) &&
                                       (isInt(elt(0)) || isUint(elt(0)) ||
                                        isReal(elt(0)) || isImag(elt(0))) {
    // Re-use the keyPart for imag, real
    if i >= elt.size {
      const (_,part) = this.keyPart(elt(0), 0); // calling just to get type
      return (keyPartStatus.pre, 0:part.type);
    } else {
      const (_,part) = this.keyPart(elt(i), 0); // retrieve the value
      return (keyPartStatus.returned, part);
    }
  }

  /*
    Default ``keyPart`` method for sorting strings.
    See also `The .keyPart method`_.

    .. note::
      Currently assumes that the string is local.

    :arg elt: the string to sort
    :arg i: the part number requested

    :returns: ``(keyPartStatus.returned, byte i of string)`` or
              ``(keyPartStatus.pre, 0)`` if ``i > elt.size``
  */
  inline proc keyPart(elt: string, i: int): (keyPartStatus, uint(8)) {
    // This assumes that the string is local, which should
    // be OK for the sort module (because distributed sort currently
    // only works for POD data types)
    // In the future it should use bytes access into the string.
    if boundsChecking then
      assert(elt.locale_id == here.id);

    var ptr = elt.c_str():c_ptr(uint(8));
    var len = elt.numBytes;
    var section = if i < len then keyPartStatus.returned else keyPartStatus.pre;
    var part =    if i < len then ptr[i]                 else 0:uint(8);
    return (section, part);
  }

  /*
    Default ``keyPart`` method for sorting `c_ptrConst(c_char)`.
    See also `The .keyPart method`_.

    :arg elt: the `c_ptrConst(c_char)` to sort
    :arg i: the part number requested

    :returns: ``(keyPartStatus.returned, byte i of string)`` or
              ``(keyPartStatus.pre, 0)`` if byte ``i`` is ``0``
  */
  inline proc keyPart(elt: c_ptrConst(c_char), i: int): (keyPartStatus, uint(8)) {
    var ptr = elt:c_ptr(uint(8));
    var byte = ptr[i];
    var section = if byte != 0 then keyPartStatus.returned else keyPartStatus.pre;
    var part = byte;
    return (section, part);
  }
}

/* Reverse comparator built from another comparator.*/
record reverseComparator: keyPartComparator {

  /* Generic comparator defined in initializer.*/
  var comparator;

  /*
   Initializer - builds a comparator with a compare method that
   reverses the sort order of the default comparator.
   */
  proc init() {
    this.comparator = new defaultComparator();
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

   :arg revcomp: :ref:`reverseComparator <reverse-comparator>` to copy.
   */
  @chpldoc.nodoc
  proc init=(revcomp: reverseComparator(?)) {
    this.comparator = revcomp.comparator;
  }

  @chpldoc.nodoc
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
  @chpldoc.nodoc
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

  @chpldoc.nodoc
  proc hasKeyPart(a) param {
    return canResolveMethod(this.comparator, "keyPart", a, 0);
  }
  @chpldoc.nodoc
  proc hasKeyPartFromKey(a) param {
    if canResolveMethod(this.comparator, "key", a) {
      var key:comparator.key(a).type;
      // Does the defaultComparator have a keyPart for this?
      return canResolveMethod(new defaultComparator(), "keyPart", key, 0);
    }
    return false;
  }

  @chpldoc.nodoc
  proc hasCompare(a,b) param {
    return canResolveMethod(this.comparator, "compare", a, b);
  }
  @chpldoc.nodoc
  proc hasCompareFromKey(a) param {
    if canResolveMethod(this.comparator, "key", a) {
      var key:comparator.key(a).type;
      // Does the defaultComparator have a compare for this?
      return canResolveMethod(new defaultComparator(), "compare", key, key);
    }
    return false;
  }

  @chpldoc.nodoc
  inline proc getKeyPart(cmp, elt, i) {
    var (section, part) = cmp.keyPart(elt, i);
    if typeIsBitReversible(part.type) {
      return (reverseKeyPartStatus(section), ~part);
    } else if typeIsNegateReversible(part.type) {
      return (reverseKeyPartStatus(section), -part);
    } else {
      compilerError("keyPart must return int or uint");
    }
  }

  /*
    Reverses ``comparator.keyPart``. See also `The .keyPart method`_.
  */
  inline proc keyPart(elt, i) where (hasKeyPart(elt) ||
                                     hasKeyPartFromKey(elt)) {
    chpl_check_comparator(this.comparator, elt.type, doDeprecationCheck=false);
    if hasKeyPartFromKey(elt) {
      return getKeyPart(new defaultComparator(), this.comparator.key(elt), i);
    } else {
      return getKeyPart(this.comparator, elt, i);
    }
  }

  @chpldoc.nodoc
  inline
  proc doCompare(cmp, a, b) {
    return cmp.compare(b, a);
  }

  /*
   Reverses ``comparator.compare``. See also `The .compare method`_.
   */
  inline
  proc compare(x, y: x.type) where hasCompare(x, y) || hasCompareFromKey(x) {

    chpl_check_comparator(this.comparator, x.type, doDeprecationCheck=false);

    if hasCompareFromKey(x) {
      return doCompare(new defaultComparator(),
                       this.comparator.key(x),
                       this.comparator.key(y));
    } else {
      return doCompare(this.comparator, x, y);
    }
  }
}


} // Sort Module
