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

.. code-block:: chapel

  var Array = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5];

  sort(Array);

  // This will output: 1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9
  writeln(Array);


The sort function can also accept a region argument to sort a subset of an
array. This is offered as an optimization over using an array slice which may
have performance overhead.


.. code-block:: chapel

  var Array = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5];

  // Sort only the elements in the range 1..5
  // Same as sort(Array[1..5]);
  sort(Array, region=1..5);

  // This will output: 3, 1, 1, 4, 5, 9, 2, 6, 5, 3, 5
  writeln(Array);


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

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator, implements the keyComparator interface
  record absComparator : keyComparator { }

  // key method maps an element to the value to be used for comparison
  proc absComparator.key(elt) { return abs(elt); }

  var absoluteComparator: absComparator;

  sort(Array, comparator=absoluteComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);

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
      return x - y;
    }


The absolute value comparison example from above can alternatively be
implemented with a ``relativeComparator`` as follows:

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record absComparator : relativeComparator { }

  // compare method defines how 2 elements are compared
  proc absComparator.compare(x, y) {
    return abs(x) - abs(y);
  }

  var absoluteComparator: absComparator;

  sort(Array, comparator=absoluteComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);


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

.. code-block:: chapel

  proc keyPart(elt: 2*int, i: int) {
    if i > 1 then
      return (keyPartStatus.pre, 0); // second value is not used

    return (keyPartStatus.returned, elt(i));
  }


Here is a ``keyPart`` to support sorting of strings:

.. code-block:: chapel

  proc keyPart(x: string, i: int): (keyPartStatus, uint(8)) {
    var len = x.numBytes;
    var section = if i < len then keyPartStatus.returned  else keyPartStatus.pre;
    var part =    if i < len then x.byte(i)               else 0:uint(8);
    return (section, part);
  }


.. _reverse-comparator:

Reverse Comparator
~~~~~~~~~~~~~~~~~~

Sort functions in Chapel do not have a ``reverse`` argument. Instead, reverse
sorting is handled through the comparator interface.

An instance of the type :record:`reverseComparator` can be passed to a sort
function to reverse the default sorting order.

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Using module-defined 'reverseComparator'
  sort(Array, comparator = new reverseComparator())

  // This will output: 3, 2, -1, -4
  writeln(Array);


To reverse the sort order of a user-defined comparator, pass the user-defined
comparator to the initializer of the module-defined
:record:`reverseComparator` record, which can be passed to the sort function.

For this example, we will reverse the absolute value comparison from above
using the ``relativeComparator`` interface, although the same can be done with
the ``keyComparator`` interface.

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record absComparator : relativeComparator{ }

  // compare method defines how 2 elements are compared
  proc absComparator.compare(x, y) {
    return abs(x) - abs(y); // ascending order
  }

  var absReverseComparator: reverseComparator(absComparator); // reverse order

  sort(Array, comparator=absReverseComparator);

  // This will output: -4, 3, 2, -1
  writeln(Array);

 */

module Sort {

  private use List;
  private use Reflection;
  private use CTypes;

/* Private methods */

private inline
proc compareByPart(a:?t, b:t, comparator:?rec) {
  var curPart = 0;
  if canResolveMethod(comparator, "chpl_keyPartInternal", a, 0) {
    while true {
      var (aSection, aPart) = comparator.chpl_keyPartInternal(a, curPart);
      var (bSection, bPart) = comparator.chpl_keyPartInternal(b, curPart);
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
  } else {
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
  } else if canResolveMethod(comparator, "chpl_keyPartInternal", a, 0) ||
            canResolveMethod(comparator, "keyPart", a, 0) {
    return compareByPart(a, b, comparator);
  } else {
    // never reached, chpl_check_comparator will catch this
    proc baseComparatorType(type c) type {
      if c == defaultComparator then
        return c;
      else if isSubtype(c, reverseComparator) then
        return baseComparatorType(c.comparator);
      else
        return c;
    }
    if baseComparatorType(comparator.type) == defaultComparator {
      compilerError("The defaultComparator does not support sorting ", t:string, " elements");
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
  if canResolveMethod(comparator, "chpl_keyPartInternal", data, 0) {
    var idx: int = 0;
    type partType = comparator.chpl_keyPartInternal(data, idx).type;
    if !isTupleType(partType) then
      compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple when used with ", eltType:string, " elements");
    var tmp: partType;
    var expectKeyPartStatus = tmp(0);
    var expectIntUint = tmp(1);
    if expectKeyPartStatus.type != keyPartStatus then
      compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with element 0 of type keyPartStatus when used with ", eltType:string, " elements");
    if !(isInt(expectIntUint) || isUint(expectIntUint)) then
      compilerError(errorDepth=errorDepth, "The keyPart method in ", comparator.type:string, " must return a tuple with element 1 of type int(?) or uint(?) when used with ", eltType:string, " elements");
    // no need to check the interface,'chpl_keyPartInternal' is internal

    return true;
  }
  else if canResolveMethod(comparator, "keyPart", data, 0) {
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
    if canResolveMethod(comparator, "chpl_keyPartInternal", tmp, 0) ||
       canResolveMethod(comparator, "keyPart", tmp, 0) {
      return true;
    } else if canResolveMethod(comparator, "key", tmp) {
      var key:comparator.key(tmp).type;
      // Does the defaultComparator have a keyPart for this?
      if canResolveMethod(new defaultComparator(), "chpl_keyPartInternal", key, 0) ||
         canResolveMethod(new defaultComparator(), "keyPart", key, 0) then
        return true;
    }
  }
  return false;
}

private proc unstableSort(ref x: [], comparator, region: range(?)) {
  if region.low >= region.high then
    return;

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
    QuickSort.quickSort(x, comparator=comparator, region=region);
  }
}

private proc stableSort(ref x: [], comparator, region: range(?)) {
  if region.low >= region.high then
    return;

  // TODO: implement a stable merge sort with parallel merge
  // TODO: create an in-place merge sort for the stable+minimizeMemory case
  // TODO: create a stable variant of the radix sort
  TimSort.timSort(x, comparator=comparator, region=region);
}

/*

Sort the elements in the 1D rectangular array ``x``.
After the call, ``x`` will store elements in sorted order.

The choice of sorting algorithm used is made by the implementation.

.. note::

  When reordering elements, the sort implementation might use assignment, memory
  moves, or the swap operator. Additionally, the sort might
  copy-initialize some elements, for example, to create a pivot in quicksort.

.. note::

  This function currently either uses a parallel radix sort or a parallel
  improved quick sort. For stable sort, it uses Timsort.
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

// insertion sort is stable and in-place but has
// poor time complexity (O(n**2)) for large problem sizes
@chpldoc.nodoc
module InsertionSort {
  private use Sort;
  /*
   Sort the 1D array `Data` in-place using a sequential insertion sort
   algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc insertionSort(ref Data: [?Dom] ?eltType,
                     comparator:?rec = new defaultComparator(), lo:int=Dom.low,
                     hi:int=Dom.high) {
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

  proc insertionSortMoveElts(ref Data: [?Dom] ?eltType,
                             comparator:?rec = new defaultComparator(),
                             lo:int=Dom.low, hi:int=Dom.high) {
    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("insertionSort() requires 1-D array");
    }

    const low = lo,
          high = hi,
          stride = abs(Dom.stride);

    for i in low..high by stride {
      pragma "no auto destroy"
      pragma "no copy"
      var ithVal = ShallowCopy.shallowCopyInit(Data[i]);

      var inserted = false;
      for j in low..i-stride by -stride {
        if chpl_compare(ithVal, Data[j], comparator) < 0 {
          ShallowCopy.shallowCopy(Data[j+stride], Data[j]);
        } else {
          ShallowCopy.shallowCopy(Data[j+stride], ithVal);
          inserted = true;
          break;
        }
      }
      if (!inserted) {
        ShallowCopy.shallowCopy(Data[low], ithVal);
      }
    }
  }
}

// TimSort is stable, nlogn, but not in-place
@chpldoc.nodoc
module TimSort {
  private use Sort;

    /*
    Sort the 1D array `x` using a parallel timSort algorithm.
    For more information on timSort, follow the link.
      https://github.com/python/cpython/blob/master/Objects/listsort.txt

    :arg x: The array to be sorted
    :type x: [] `eltType`
    :arg blockSize: use :proc: `insertionSort` on blocks of `blockSize`
    :type blockSize: `integral`
    :arg comparator: :ref:`Comparator <comparators>` record that defines how the
       data is sorted.

   */

  proc timSort(ref x: [],
               blockSize = 16,
               comparator: ? = new defaultComparator(),
               region: range(?) = x.dim(0))
  {
    chpl_check_comparator(comparator, x.eltType);

    if x.rank != 1 {
      compilerError("timSort() requires 1-D array");
    }

    _TimSort(x, region.low, region.high, blockSize, comparator);
  }

  private proc _TimSort(ref Data: [?Dom], lo:int, hi:int, blockSize=16,
                        comparator:?rec = new defaultComparator()) {
    import Sort.InsertionSort;

    /*Parallelly apply insertionSort on each block of size `blockSize`
     using forall loop*/

    const stride = abs(Dom.stride): Dom.idxType;
    const size = (hi - lo) / stride + 1;
    const chunks = (size + blockSize - 1) / blockSize;

    forall i in 0..#chunks with (ref Data) {
      InsertionSort.insertionSort(Data, comparator = comparator, lo + (i * blockSize) * stride, min(hi, lo + ((i + 1) * blockSize * stride) - stride));
    }

    /* apply merge operations on each block
    *as the merges at a level are independent of each other
    *they can be applied in parallel
    */

    var numSize = blockSize;
    while(numSize < size) {
      forall i in 0..<size by 2 * numSize with (ref Data) {

        const l = lo + i * stride;
        const mid = lo + (i + numSize - 1) * stride;
        const r = min(lo + (i + 2 * numSize - 1) * stride, hi);

        _Merge(Data, l, mid, r, comparator=comparator);
      }

      numSize = numSize * 2;

    }
  }

  /*
   This TimSort._Merge() differs from MergeSort._Merge() in the following way:
   MergeSort._Merge() alternates the storage of segments in the original memory and the copied memory.
   TimSort._Merge() creates a copy of the segments to be merged and
   stores the results back into the original memory.
  */
  private proc _Merge(ref Dst: [?Dom] ?eltType, lo:int, mid:int, hi:int,
                      comparator:?rec = new defaultComparator()) {
    /* Data[lo..mid by stride] is much slower than Data[lo..mid] when
     * Dom is unstrided.  So specify the latter explicitly when possible. */
    if mid >= hi {
      return;
    }
    const stride = abs(Dom.stride): Dom.idxType;
    const a1range = if Dom.hasPosNegUnitStride() then lo..mid
                    else lo..mid by stride:uint;
    const a1max = mid;

    const a2range = if Dom.hasPosNegUnitStride() then (mid+1)..hi
                    else (mid+stride)..hi by stride:uint;
    const a2max = hi;

    var A1 = Dst[a1range];
    var A2 = Dst[a2range];

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

// this quick sort is not stable
// it is in-place however
// it is parallel but has limited parallelism
@chpldoc.nodoc
module QuickSort {
  private use Sort;
  use Sort.ShallowCopy;

  /*
   Partition the array Data[lo..hi] using the pivot at Data[pivIdx].

   This is the 3-way symmetric partition described
   in Engineering a Sort Function (1993) by Jon L. Bentley , M. Douglas McIlroy

   Returns the (eqStart,eqEnd) where eqStart..eqEnd elements are
   equal to the pivot (and elements less are before eqStart and elements
   greater are after eqEnd).
   */
  proc partition(ref Data: [?Dom] ?eltType,
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
    pragma "no copy"
    var piv: eltType = ShallowCopy.shallowCopyInit(Data[lo]); // leaves Data[lo] empty

    while true {
      while b <= c {
        // continue while Data[b] <= piv
        var cmp = chpl_compare(Data[b], piv, comparator);
        if cmp > 0 then
          break;
        if cmp == 0 {
          if a != b {
            ShallowCopy.shallowSwap(Data[a], Data[b]);
          }
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
 proc quickSort(ref Data: [?Dom] ?eltType,
                minlen = 16,
                comparator:?rec = new defaultComparator(),
                region:range(?) = Data.domain.dim(0)) {

    chpl_check_comparator(comparator, eltType);

    if Dom.rank != 1 {
      compilerError("quickSort() requires 1-D array");
    }

    if region.strides != strideKind.one && region.stride != 1 {
      ref reindexed = Data.reindex(region.low..#region.size);
      assert(reindexed.domain.stride == 1);
      quickSortImpl(reindexed, minlen, comparator);
      return;
    }

    assert(Dom.stride == 1);
    quickSortImpl(Data, minlen, comparator,
                  start=region.low, end=region.high);
  }


  /* Non-stridable quickSort to sort Data[start..end] */
  proc quickSortImpl(ref Data: [?Dom] ?eltType,
                     minlen=16,
                     comparator:?rec = new defaultComparator(),
                     start:int = Dom.low, end:int = Dom.high) {
    import Sort.InsertionSort;

    // grab obvious indices
    var lo = start,
        hi = end;

    // keep iterating over subproblems
    while lo < hi {
        var mid = lo + (hi - lo + 1) / 2;
        var piv = mid;

        if hi - lo < minlen {
          // base case -- use insertion sort
          InsertionSort.insertionSortMoveElts(Data, comparator=comparator, lo, hi);
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

        // stay sequential
        if hi-lo < 300 || here.runningTasks() > here.numPUs(logical=true)  {
          // handle smaller subproblem recursively and iterate for larger one
          if eqStart - lo > hi - eqEnd {
            // recur for smaller right half
            quickSortImpl(Data, minlen, comparator, eqEnd + 1, hi);
            // change value of hi to the end of left half
            hi = eqStart - 1;
          } else {
            // recur for smaller left half
            quickSortImpl(Data, minlen, comparator, lo,        eqStart - 1);
            // change value of lo to the start of right half
            lo = eqEnd  + 1;
          }
        } else {
          // do the subproblems in parallel
          cobegin {
            quickSortImpl(Data, minlen, comparator, lo, eqStart-1);
            quickSortImpl(Data, minlen, comparator, eqEnd+1, hi);
          }
          break;
        }
      }
    }
}

// shell sort is in-place but not stable
@chpldoc.nodoc
module ShellSort {
  private use Sort;
  proc shellSort(ref Data: [?Dom] ?eltType,
                 comparator:?rec = new defaultComparator(), start = Dom.low,
                 end = Dom.high)
  {
    chpl_check_comparator(comparator, eltType);

    type idxType = Data.idxType;

    if Dom.rank != 1 then
      compilerError("shellSort() requires 1-D array");
    if ! Dom.hasUnitStride() then
      compilerError("shellSort() requires an array over a non-stridable domain");

    // Based on Sedgewick's Shell Sort -- see
    // Analysis of Shellsort and Related Algorithms 1996
    // and see Marcin Ciura - Best Increments for the Average Case of Shellsort
    // for the choice of these increments.
    var js,hs:idxType;
    var v:Data.eltType;
    const incs = (701, 301, 132, 57, 23, 10, 4, 1);
    for hh in incs {
      // skip past cases in which the 'incs' value was too big for
      // idxType, or in which h+start will overflow idxType.
      // start may be negative, so the first test isn't redundant.
      if hh > max(idxType) || hh >= max(idxType):uint - start:uint then
        continue;

      const h = hh:idxType;
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

  // Like the shell sort above, but this version uses
  // ShallowCopy to move elements instead of assigning them.
  proc shellSortMoveElts(ref Data: [?Dom] ?eltType,
                         comparator:?rec = new defaultComparator(),
                         start: Data.idxType = Dom.low,
                         end: Data.idxType = Dom.high)
  {
    chpl_check_comparator(comparator, eltType);

    type idxType = Data.idxType;

    if Dom.rank != 1 then
      compilerError("shellSort() requires 1-D array");
    if ! Dom.hasUnitStride() then
      compilerError("shellSort() requires an array over a non-stridable domain");

    // Based on Sedgewick's Shell Sort -- see
    // Analysis of Shellsort and Related Algorithms 1996
    // and see Marcin Ciura - Best Increments for the Average Case of Shellsort
    // for the choice of these increments.
    var js,hs:idxType;
    const incs = (701, 301, 132, 57, 23, 10, 4, 1);
    for hh in incs {
      // skip past cases in which the 'incs' value was too big for
      // idxType, or in which h+start will overflow idxType.
      // start may be negative, so the first test isn't redundant.
      if hh > max(idxType) || hh >= max(idxType):uint - start:uint then
        continue;

      const h = hh:idxType;
      hs = h + start;
      for is in hs..end {
        // move Data[is] into v
        pragma "no auto destroy"
        pragma "no copy"
        var v = ShallowCopy.shallowCopyInit(Data[is]);
        js = is;
        while js >= hs && chpl_compare(v,Data[js-h],comparator) < 0 {
          // move Data[js - h] into Data[js]
          ShallowCopy.shallowCopy(Data[js], Data[js - h]);
          js -= h;
        }
        // move v into Data[js]
        ShallowCopy.shallowCopy(Data[js], v);
      }
    }
  }

  // this version takes int start and end and casts to Data.idxType
  // to make it more convenient to call from the radix sorting code
  proc shellSortMoveEltsIntIdx(ref Data: [?Dom] ?eltType,
                               comparator:?rec = new defaultComparator(),
                               start:int = Dom.low,
                               end:int = Dom.high)
  {
    type idxType = Data.idxType;
    shellSortMoveElts(Data, comparator, start:idxType, end:idxType);
  }
}

@chpldoc.nodoc
module RadixSortHelp {
  private use Sort;
  import Reflection.canResolveMethod;

  // This is the number of bits to sort at a time in the radix sorter.
  // The code assumes that all integer types are a multiple of it.
  // That would need to change if it were to increase.
  //
  // At the same time, using a value less than 8 will probably perform poorly.
  // TODO: parameterize functions using this so that different
  // algorithms can use a different number of radix bits
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
    if canResolveMethod(criterion, "chpl_keyPartInternal", a, 0) {
      // We have keyPart(element, start):(section:int(8), part:int/uint)
      const testRet: criterion.chpl_keyPartInternal(a, 1).type;
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

      const (section, part) = criterion.chpl_keyPartInternal(a, whichpart);
      var ubits = part:uint(bitsPerPart);
      // If the number is signed, invert the top bit, so that
      // the negative numbers sort below the positive numbers
      if isInt(part) {
        const one:ubits.type = 1;
        ubits = ubits ^ (one << (bitsPerPart - 1));
      }
      param mask:uint = (1 << RADIX_BITS) - 1;
      const ubin = (ubits >> (bitsPerPart - bitsinpart - RADIX_BITS)) & mask;

      if section:int == 0 then
        return (ubin:int + 1, ubits);
      else if section:int < 0 then
        return (0, ubits);
      else
        return ((1 << RADIX_BITS) + 1, ubits);
    } else {
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

      const (section, part) = criterion.keyPart(a, whichpart);
      var ubits = part:uint(bitsPerPart);
      // If the number is signed, invert the top bit, so that
      // the negative numbers sort below the positive numbers
      if isInt(part) {
        const one:ubits.type = 1;
        ubits = ubits ^ (one << (bitsPerPart - 1));
      }
      param mask:uint = (1 << RADIX_BITS) - 1;
      const ubin = (ubits >> (bitsPerPart - bitsinpart - RADIX_BITS)) & mask;

      if section:int == 0 then
        return (ubin:int + 1, ubits);
      else if section:int < 0 then
        return (0, ubits);
      else
        return ((1 << RADIX_BITS) + 1, ubits);
    }
  }


  // Get the bin for a record with criterion.key or criterion.keyPart
  //
  // See binForRecordKeyPart for what the arguments / returns mean.
  inline
  proc binForRecord(a, criterion, startbit:int)
  {
    if canResolveMethod(criterion, "chpl_keyPartInternal", a, 0) ||
       canResolveMethod(criterion, "keyPart", a, 0) {
      return binForRecordKeyPart(a, criterion, startbit);
    } else if canResolveMethod(criterion, "key", a) {
      // Try to use the default comparator to get a keyPart.
      return binForRecordKeyPart(criterion.key(a),
                                 new defaultComparator(),
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
    if comparator.type == defaultComparator && fixedWidth(element.type) > 0 {
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
    proc isSampleSort param do return false;

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
      type idxType = if isArray(A) then A.idxType else int;
      var cur = start_n;
      while cur <= end_n-(classifyUnrollFactor-1) {
        for /*param*/ j in 0..classifyUnrollFactor-1 {
          const cur_j_idx = (cur+j):idxType;
          yield (cur+j,
                 bucketForRecord(A[cur_j_idx], criterion, startbit));
        }
        cur += classifyUnrollFactor;
      }
      while cur <= end_n {
        const cur_idx = cur:idxType;
        yield (cur, bucketForRecord(A[cur_idx], criterion, startbit));
        cur += 1;
      }
    }
  }
}

@chpldoc.nodoc
module ShallowCopy {
  private use CTypes;
  private use OS.POSIX;

  // The shallowCopy / shallowSwap code needs to be able to copy/swap
  // _array records. But c_ptrTo on an _array will return a pointer to
  // the first element, which messes up the shallowCopy/shallowSwap code
  //
  // As a workaround, this function just returns a pointer to the argument,
  // whether or not it is an array.
  //
  // TODO: these should be replaced with the appropriate c_addrOf[Const] calls
  private inline proc ptrTo(ref x) {
    return c_pointer_return(x);
  }
  private inline proc ptrToConst(const ref x) {
    return c_pointer_return_const(x);
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
      memcpy(ptrTo(dst), ptrTo(src), size);
      if boundsChecking {
        // The version moved from should never be used again,
        // but we clear it out just in case.
        memset(ptrTo(src), 0, size);
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
      memcpy(ptrTo(tmp), ptrTo(lhs), size);
      // lhs = rhs
      memcpy(ptrTo(lhs), ptrTo(rhs), size);
      // rhs = tmp
      memcpy(ptrTo(rhs), ptrTo(tmp), size);
    }
  }

  inline proc shallowCopy(ref A, dst, src, nElts) {

    // Ideally this would just be
    //A[dst..#nElts] = A[src..#nElts];

    type idxType = A.idxType;
    const dst_idx = dst:idxType;
    const src_idx = src:idxType;
    const nElts_idx = nElts:idxType;

    if boundsChecking {
      assert(nElts > 0);
      // check that the bounds can be safely converted to array indices
      dst.safeCast(idxType);
      src.safeCast(idxType);
      nElts.safeCast(idxType);
      // check that the domain contains the indices
      assert(A.domain.contains(dst_idx..#nElts_idx));
      assert(A.domain.contains(src_idx..#nElts_idx));
    }

    if A._instance.isDefaultRectangular() {
      type st = __primitive("static field type", A._value, "eltType");
      var size = (nElts:c_size_t)*c_sizeof(st);
      memcpy(ptrTo(A[dst_idx]), ptrTo(A[src_idx]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ A,
                                       {dst_idx..#nElts_idx},
                                       /*src*/ A,
                                       {src_idx..#nElts_idx});
      if !ok {
        // fall back on PRIM_ASSIGN to move the elements
        // at present, this is needed for Cyclic and Replicated.
        foreach i in 0..#nElts {
          __primitive("=", A[dst_idx+i:idxType], A[src_idx+i:idxType]);
        }
      }
    }
  }
  inline proc shallowCopy(ref DstA, dst, SrcA, src, nElts) {

    // Ideally this would just be
    //DstA[dst..#nElts] = SrcA[src..#nElts];

    const dst_idx = dst:DstA.idxType;
    const src_idx = src:SrcA.idxType;
    const nElts_dst_idx = nElts:DstA.idxType;
    const nElts_src_idx = nElts:SrcA.idxType;

    if boundsChecking {
      assert(nElts > 0);
      // check that the bounds can be safely converted to array indices
      dst.safeCast(DstA.idxType);
      src.safeCast(SrcA.idxType);
      nElts.safeCast(DstA.idxType);
      nElts.safeCast(SrcA.idxType);
      // check that the domain contains the indices
      assert(DstA.domain.contains(dst_idx..#nElts_dst_idx));
      assert(SrcA.domain.contains(src_idx..#nElts_src_idx));
    }


    if DstA._instance.isDefaultRectangular() &&
       SrcA._instance.isDefaultRectangular() {
      type st = __primitive("static field type", DstA._value, "eltType");
      var size = (nElts:c_size_t)*c_sizeof(st);
      memcpy(ptrTo(DstA[dst_idx]), ptrToConst(SrcA[src_idx]), size);
    } else {
      var ok = chpl__bulkTransferArray(/*dst*/ DstA,
                                       {dst_idx..#nElts_dst_idx},
                                       /*src*/ SrcA,
                                       {src_idx..#nElts_src_idx});
      if !ok {
        // fall back on PRIM_ASSIGN to move the elements
        // at present, this is needed for Cyclic and Replicated.
        foreach i in 0..#nElts_dst_idx {
          __primitive("=", DstA[dst_idx+i], SrcA[src_idx+i:SrcA.idxType]);
        }
      }
    }
  }
  proc shallowCopyPutGetRefs(ref dst, const ref src, numBytes: c_size_t) {
    if dst.locale.id == here.id {
      __primitive("chpl_comm_get", dst, src.locale.id, src, numBytes);
    } else if src.locale.id == here.id {
      __primitive("chpl_comm_put", src, dst.locale.id, dst, numBytes);
    } else {
      halt("Remote src and remote dst not yet supported");
    }
  }

  // For the case in which we know that the source and dest regions
  // are contiguous within a locale
  proc shallowCopyPutGet(ref DstA, dst, const ref SrcA, src, nElts) {
    var size = (nElts:c_size_t)*c_sizeof(DstA.eltType);
    shallowCopyPutGetRefs(DstA[dst], SrcA[src], size);
  }
}

@chpldoc.nodoc
module TwoArrayPartitioning {
  private use Math;
  public use List only list;
  import Sort.{ShellSort, MSBRadixSort};
  import Sort.{RadixSortHelp, ShallowCopy};
  import Sort;
  use MSBRadixSort;

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

  proc defaultNumTasks() {
    if __primitive("task_get_serial") {
      return 1;
    }

    const tasksPerLocale = dataParTasksPerLocale;
    const ignoreRunning = dataParIgnoreRunningTasks;
    var nTasks = if tasksPerLocale > 0 then tasksPerLocale else here.maxTaskPar;
    if !ignoreRunning {
      const otherTasks = here.runningTasks() - 1; // don't include self
      nTasks = if otherTasks < nTasks then (nTasks-otherTasks):int else 1;
    }

    return nTasks;
  }

  record TwoArrayBucketizerSharedState {
    var nTasks:int = defaultNumTasks();
    var countsSize:int = nTasks*maxBuckets;

    type bucketizerType;
    var bucketizer: bucketizerType; // contains e.g. sample

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

    proc init(type bucketizerType) {
      this.bucketizerType = bucketizerType;
    }

    proc init(in bucketizer,
              baseCaseSize: int = 16,
              sequentialSizePerTask: int = 4096,
              endbit: int = max(int)) {
      this.bucketizerType = bucketizer.type;
      this.bucketizer = bucketizer;
      this.baseCaseSize = baseCaseSize;
      this.sequentialSizePerTask = sequentialSizePerTask;
      this.endbit = endbit;
    }
  }

  // Stores the data in dst in buckets according to the bucketizer.
  // (e.g. sorted by the next digit in radix sort)
  // Counts per bin are stored in state.counts. Other data in
  // state is used locally by this routine or used elsewhere
  // If allowSkipahead=true, and the count determines everything is
  // in just one bin, startbit can be modified and the bucketize
  // will reflect a later startbit.
  proc bucketize(start_n: int, end_n: int, ref dst:[], src:[],
                 ref state: TwoArrayBucketizerSharedState,
                 criterion, inout startbit:int,
                 allowSkipahead=false) {

    if debug then
      writeln("bucketize ", start_n..end_n, " startbit=", startbit);

    type idxType = dst.idxType;
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
    const blockSize = divCeil(n, nTasks);
    const nBlocks = divCeil(n, blockSize);

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
      for (_,bin) in state.bucketizer.classify(src, start, end,
                                               criterion, startbit) {
        counts[bin] += 1;
      }
      // Now store the counts into the global counts array
      foreach bin in 0..#nBuckets with (ref state) {
        state.globalCounts[bin*nTasks + tid] = counts[bin];
      }
    }

    // Compute the total counts for the next check and for use
    // after this function returns.
    ref counts = state.counts;
    forall bin in 0..#nBuckets with (ref counts) {
      var total = 0;
      for tid in 0..#nTasks {
        total += state.globalCounts[bin*nTasks + tid];
      }
      counts[bin] = total;
    }

    if !state.bucketizer.isSampleSort && allowSkipahead {
      // If the data parts we gathered all have the same leading bits,
      // we can skip ahead immediately to the next count step.
      //
      // Check: was there actually only one bin with data?
      var onlyBin: int = -1;
      for bin in 0..#nBuckets {
        var total = counts[bin];
        if total == 0 {
          // ok, continue
        } else if total == n {
          // everything is in one bin, so we can stop
          onlyBin = bin;
          break;
        } else {
          // a bin contained not 0 and not n,
          // so this check is done
          break;
        }
      }

      if onlyBin >= 0 {
        // TODO: would it help performance to compute min and max
        // here and reset startbit according to these?
        // Or, would it be better to compute min and max word
        // in the above loop? (similar to 'ubits' in msbRadixSort)

        // compute the next start bit since there was no need to sort
        // at this start bit
        startbit = state.bucketizer.getNextStartBit(startbit);
        // stop if the next startbit is too far
        if startbit > state.endbit then
          return;

        // stop if it's a bin that doesn't need sorting
        // (bin for end-of-string indicator)
        if !state.bucketizer.getBinsToRecursivelySort().contains(onlyBin) then
          return;

        // start over with the new start bit
        bucketize(start_n, end_n, dst, src, state, criterion,
                  startbit, allowSkipahead=true);
        return; // note: startbit is inout so will change at call site
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
        ShallowCopy.shallowCopy(dst, next, src, i:idxType, 1:idxType);
        next += 1;
      }
    }
  }
  proc testBucketize(start_n: int, end_n: int, ref dst:[], src:[],
                     bucketizer, criterion, startbit:int) {

    var state = new TwoArrayBucketizerSharedState(bucketizer=bucketizer);

    var myStartBit = startbit;
    bucketize(start_n, end_n, dst, src, state, criterion, myStartBit);

    return state.counts;
  }


 proc partitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if state.bucketizer.isSampleSort {
      var n = 1 + end_n - start_n;
      var logNumBuckets = computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debug then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debug then
        writeln("recursing to sort the sample");

      // sort the sample using the usual sorting algorithm
      Sort.sort(A[start_n..#sampleSize], comparator=criterion);

      if debug {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debug {
        writeln("sample bucketizer ", state.bucketizer);
        writef("A %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
      }
    }
  }

  // Sorts the data in A.
  proc partitioningSortWithScratchSpace(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {

    if startbit > state.endbit then
      return;

    if end_n - start_n < state.baseCaseSize {
      ShellSort.shellSortMoveEltsIntIdx(A, criterion, start=start_n, end=end_n);
      return;
    }

    if debug {
      writeln("partitioningSortWithScratchSpace(", start_n, ",", end_n, ")");
      writef("A %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
    }


    const n = (end_n - start_n + 1);
    const maxSequentialSize = max(n / state.nTasks,
                                  state.nTasks*state.sequentialSizePerTask);

    state.bigTasks.pushBack(new TwoArraySortTask(start_n, n, startbit, inA=true, doSort=true));
    assert(state.bigTasks.size == 1);
    assert(state.smallTasks.size == 0);

    while !state.bigTasks.isEmpty() {
      const task = state.bigTasks.popBack();
      const taskEnd = task.start + task.size - 1;
      var taskStartBit = task.startbit;

      assert(task.doSort);

      if debug then {
        writeln("doing big task ", task.start..taskEnd);
      }

      if task.inA {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, A, Scratch,
              state, criterion, taskStartBit);

        // Count and partition
        bucketize(task.start, taskEnd, Scratch, A, state,
                  criterion, taskStartBit, allowSkipahead=true);
        // bucketized data now in Scratch
        if debug {
          writef("pb %i %i Scratch=%?\n", task.start, taskEnd, Scratch[task.start..taskEnd]);
        }
      } else {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, Scratch, A,
              state, criterion, taskStartBit);

        // Count and partition
        bucketize(task.start, taskEnd, A, Scratch, state,
                  criterion, taskStartBit, allowSkipahead=true);
        // bucketized data now in A
        if debug {
          writef("pb %i %i A=%?\n", task.start, taskEnd, A[task.start..taskEnd]);
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
        const binStartBit = state.bucketizer.getNextStartBit(taskStartBit);

        const sortit =
          binSize > 1 && // have 2 or more elements to sort
          binStartBit <= state.endbit && // have bits to sort
          state.bucketizer.getBinsToRecursivelySort().contains(bin);

        if binSize == 0 {
          // Do nothing
        } else if nowInA && !sortit {
          // no need to copy it or to sort it
        } else if !nowInA && !sortit {
          // Enqueue a small task to do the copy but not sort it
          // TODO: handle large copies in big tasks,
          // or enqueue several tasks here
          state.smallTasks.pushBack(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));
        } else if //binStartBit > state.endbit ||
                  //binStart >= binEnd ||
                  binSize <= maxSequentialSize {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as small");
          }

          // Enqueue a small subproblem to sort and possibly copy.
          state.smallTasks.pushBack(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));

        } else {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as big");
          }

          // Enqueue a big subproblem to sort
          state.bigTasks.pushBack(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));
        }
      }
    }

    // Now handle any small tasks.

    // TODO: sort small tasks by size

    forall task in state.smallTasks with (ref A) {
      const size = task.size;
      const taskEnd = task.start + size - 1;
      if debug {
        writef("doing small task %i %i A=%?\n", task.start, taskEnd, A[task.start..taskEnd]);
      }

      if !task.inA {
        ShallowCopy.shallowCopy(A, task.start, Scratch, task.start, size);
      }

      if task.doSort {
        // Sort it serially.
        // Note that the subproblems here are on the order of 500,000 elements
        // because the two-array method will create small subproblems as soon
        // as it does not seem useful to use parallelism to sort.
        // Because of this, it matters to use a radix sort here.
        // It also seems to matter to use an in-place algorithm here,
        // but I am not completely confident of that.
        msbRadixSort(A, task.start:A.idxType, taskEnd:A.idxType,
                     criterion,
                     task.startbit, state.endbit,
                     settings=new MSBRadixSortSettings(alwaysSerial=true));
      }
    }

    if debug {
      writef("ps %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
      writef("ps %i %i Scratch=%?\n", start_n, end_n, Scratch[start_n..end_n]);
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion, startbit);
    }
  }
}

@chpldoc.nodoc
module TwoArrayDistributedPartitioning {
  public use super.TwoArrayPartitioning;
  private use BlockDist;
  private use Math;
  public use List only list;
  import Sort.{ShellSort, MSBRadixSort, QuickSort};
  import Sort.{RadixSortHelp, ShallowCopy, keyComparator};
  use MSBRadixSort;

  private param debugDist = false;

  record TwoArrayDistSortPerBucketTaskStartComparator: keyComparator {
    proc key(elt: TwoArrayDistSortPerBucketTask) {
      return elt.start;
    }
  }

  record TwoArrayDistSortPerBucketTask {
    var start: int;
    var size: int;
    var startbit: int; // could be moved to TwoArrayDistSortTask

    var firstLocaleId: int;
    var lastLocaleId: int;
    var useSecondState: bool;

    proc isEmpty() {
      return size == 0;
    }

    // create an easy-to-identify empty task
    proc init() {
      this.start = -1;
      this.size = 0;
      this.startbit = max(int);
      this.firstLocaleId = max(int);
      this.lastLocaleId = -1;
      this.useSecondState = false;
    }
    // To make sure all fields are specified
    proc init(start:int, size:int, startbit:int,
              firstLocaleId:int, lastLocaleId:int,
              useSecondState:bool) {
      this.start = start;
      this.size = size;
      this.startbit = startbit;
      this.firstLocaleId = firstLocaleId;
      this.lastLocaleId = lastLocaleId;
      this.useSecondState = useSecondState;
    }
    proc nLocales() {
      return lastLocaleId - firstLocaleId + 1;
    }
    // yields tuples of (loc, tid) for the locales involved with this bucket
    iter localeAndIds(A) {
      const ref tgtLocs = A.targetLocales();
      foreach tid in firstLocaleId..lastLocaleId {
        const loc = tgtLocs[tid];
        yield (loc, tid);
      }
    }
    // yield the other ids but do so in an order that depends on myId
    //  myId + 1 will be the first id.
    iter otherIds(myId) {
      const nIds = lastLocaleId-firstLocaleId+1;
      foreach i in 1..#nIds {
        yield firstLocaleId + ((myId + i) % nIds);
      }
    }
    iter otherIds(param tag: iterKind, myId) where tag == iterKind.standalone {
      const nIds = lastLocaleId-firstLocaleId+1;
      forall i in 1..#nIds {
        yield firstLocaleId + ((myId + i) % nIds);
      }
    }
  }

  record TwoArrayDistSortTask : writeSerializable {
    var tasks: list(TwoArrayDistSortPerBucketTask);

    // Create an empty one
    proc init() { }
    // Create one with just 1 bucket
    proc init(start:int, size:int, startbit:int,
              firstLocaleId:int, lastLocaleId:int) {
      var t = new TwoArrayDistSortPerBucketTask(start, size, startbit,
                                                firstLocaleId, lastLocaleId,
                                                false);
      assert(!t.isEmpty());
      init this;
      tasks.pushBack(t);
    }
    proc serialize(writer, ref serializer) throws {
      writer.write("TwoArrayDistSortTask");
      for t in tasks {
        writer.write(" ");
        writer.write(t);
      }
    }

    proc isEmpty() {
      return tasks.isEmpty();
    }
    // yield (loc, locId, task) for each non-empty bucket
    // loc is the locale "owning" the bucket.
    iter localesAndTasks(A) {
      foreach t in tasks {
        const locId = t.firstLocaleId;
        const loc = A.targetLocales()[locId];
        yield (loc, locId, t);
      }
    }
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

  record TwoArrayDistributedBucketizerSharedState {
    type bucketizerType;

    var numLocales:int;
    var perLocale = blockDist.createArray(0..#numLocales,
        TwoArrayDistributedBucketizerStatePerLocale(bucketizerType));

    const baseCaseSize:int;
    const distributedBaseCaseSize:int;
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

    proc ref postinit() {
      // Copy some vars to the compat
      for p in perLocale {
        p.compat.baseCaseSize = baseCaseSize;
        p.compat.endbit = endbit;
        assert(p.compat.nTasks > 0);
        assert(p.compat.countsSize > 0);
      }
    }
  }

  private proc distributedPartitioningSortWithScratchSpaceBaseCase(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref compat: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {

    if startbit > compat.endbit then
      return;

    const n = end_n - start_n + 1;

    const localSubdomain = A.localSubdomain();
    const curDomain = {start_n..end_n};
    const intersect = curDomain[localSubdomain];
    if curDomain == intersect {
      local {
        if n > compat.baseCaseSize {
          compat.bigTasks.clear();
          compat.smallTasks.clear();
          partitioningSortWithScratchSpace(start_n, end_n,
                       A.localSlice(curDomain), Scratch.localSlice(curDomain),
                       compat, criterion,
                       startbit);
        } else {
          ShellSort.shellSortMoveEltsIntIdx(A.localSlice(curDomain), criterion, start=start_n, end=end_n);
        }
      }
    } else {
      const size = end_n-start_n+1;
      // Copy it to one locale
      var LocalA:[start_n..end_n] A.eltType;
      var LocalScratch:[start_n..end_n] A.eltType;
      ShallowCopy.shallowCopy(LocalA, start_n, A, start_n, size);
      // Sort it
      if n > compat.baseCaseSize {
        compat.bigTasks.clear();
        compat.smallTasks.clear();
        partitioningSortWithScratchSpace(start_n, end_n,
                     LocalA, LocalScratch,
                     compat, criterion,
                     startbit);
      } else {
        ShellSort.shellSortMoveEltsIntIdx(LocalA, criterion, start=start_n, end=end_n);
      }
      // Copy it back
      ShallowCopy.shallowCopy(A, start_n, LocalA, start_n, size);
    }

    if debugDist {
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion);
    }
    return;
  }


  private proc distributedPartitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if state.perLocale[0].compat.bucketizer.isSampleSort {
      var n = 1 + end_n - start_n;
      var logNumBuckets = computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debugDist then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debugDist then
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
      if debugDist {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.perLocale[0].compat.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debugDist {
        writeln("sample bucketizer ", state.perLocale[0].compat.bucketizer);
        writef("A %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
      }

      // Now copy the bucketizer sample to all other locales
      for lid in 1..state.numLocales {
        state.perLocale[lid].bucketizer = state.perLocale[0].bucketizer;
      }
    }
  }

  proc distributedPartitioningSortWithScratchSpace(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state1: TwoArrayDistributedBucketizerSharedState,
          ref state2: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int): void {

    if !A.hasSingleLocalSubdomain() {
      compilerError("distributedPartitioningSortWithScratchSpace needs single local subdomain");
    }
    // TODO: assert that src and dst have the same distribution?

    if startbit > state1.endbit then
      return;

    // If it's really small, just sort it on Locale 0.
    if end_n - start_n < state1.distributedBaseCaseSize {
      ref compat = state1.perLocale[0].compat;
      distributedPartitioningSortWithScratchSpaceBaseCase(start_n, end_n,
                                                          A, Scratch,
                                                          compat, criterion,
                                                          startbit);
      return;
    }


    if debugDist {
      writeln("in distributed radix sort ", start_n, "..", end_n,
              " startbit ", startbit, " endbit ", state1.endbit);
    }

    // TODO: use something more like distributed bag for these
    var distTask: TwoArrayDistSortTask =
      new TwoArrayDistSortTask(start_n, end_n - start_n + 1,
                               startbit,
                               0, state1.numLocales-1);
    var nextDistTaskElts: list(TwoArrayDistSortPerBucketTask, parSafe=true);
    var smallTasksPerLocale = blockDist.createArray(0..#numLocales,
                                          list(TwoArraySortTask, parSafe=true));

    assert(!distTask.isEmpty());

    const nBuckets = state1.perLocale[0].compat.bucketizer.getNumBuckets();
    const nLocalesTotal = state1.numLocales;

    // Part A: Handle the "big" subproblems
    while true {
      if distTask.isEmpty() then break;

      if debugDist then
        writeln("Doing big task ", distTask);

      if debugDist {
        var usedLocales1:[0..#nLocalesTotal] bool;
        var usedLocales2:[0..#nLocalesTotal] bool;
        // check: non-overlapping locales are used by each bucket in distTask
        for t in distTask.tasks {
          if !t.isEmpty() {
            if t.useSecondState {
              for (_, tid) in t.localeAndIds(A) {
                assert(!usedLocales2[tid]); // means race condition would occur
                usedLocales2[tid] = true;
              }

            } else {
              for (_, tid) in t.localeAndIds(A) {
                assert(!usedLocales1[tid]); // means race condition would occur
                usedLocales1[tid] = true;
              }
            }
          }
        }
      }

      // TODO: put this call back in somewhere to support sample sort
      // ... or just rewrite sample sort.
      // Distributed sample sort should have sample size ~= numLocales,
      // but that would interfere with maxBuckets being param.
      // distributedPartitioningSortWithScratchSpaceHandleSampling(
      //       task.start, taskEnd, A, Scratch,
      //       state, criterion, task.startbit);

      coforall (bktLoc, bktLocId, bktTask) in distTask.localesAndTasks(A)
      with (ref state1, ref state2, ref nextDistTaskElts, ref smallTasksPerLocale) do
      on bktLoc {
        // Each bucket can run in parallel - this allows each
        // bucket to use nested coforalls to barrier.
        assert(!bktTask.isEmpty());
        const nLocalesBucket = bktTask.nLocales();
        ref state = if bktTask.useSecondState then state2 else state1;

        if debugDist then
          writeln(bktLocId, " doing big task component ", bktTask);

        coforall (loc,tid) in bktTask.localeAndIds(A) with (ref state) do
        on loc {
          const task = bktTask;

          // Step 1: Each locale sorts local portions into buckets
          // and shares those counts in globalCounts.
          // This uses perLocale[tid].compat.
          const taskStart = task.start;
          const taskEnd = task.start + task.size - 1;
          var taskStartBit = task.startbit;

          const localDomain = A.localSubdomain()[task.start..taskEnd];
          ref localSrc = A.localSlice(localDomain);
          ref localDst = Scratch.localSlice(localDomain);

          if debugDist then
            writeln(tid, " bucketizing local portion ", localDomain);

          bucketize(localDomain.low,
                    localDomain.high,
                    localDst, localSrc,
                    state.perLocale[tid].compat, criterion, taskStartBit);

          ref localCounts = state.perLocale[tid].compat.counts;

          if debugDist {
            var total = 0;
            for bin in 0..#nBuckets {
              if localCounts[bin] > 0 {
                total += localCounts[bin];
              }
            }
            assert(total == localDomain.size);
          }

          // Do an all-to-all to send the counts to everybody
          // To make this communication more efficient, temporarily store
          // in the order
          //     count for locale 0, bin 0
          //     count for locale 0, bin 1
          //     ...
          // (i.e. the transpose of the order needed for scan)
          const toIdx = maxBuckets * tid;
          ref perLocale = state.perLocale;
          forall dstTid in task.otherIds(tid) with (ref perLocale) {
            // perLocale[dstTid].globalCounts[toIdx..#maxBuckets] = localCounts;
            ShallowCopy.shallowCopyPutGet(
                perLocale[dstTid].globalCounts, toIdx,
                localCounts, 0, maxBuckets);
          }
        }
        // Now the data is in Scratch

        if debugDist {
          writef("%i after bucketize local portions, Scratch is %?\n",
              bktLocId,
              Scratch[bktTask.start..#bktTask.size]);
        }

        // ending coforall and recreating it is just a barrier

        coforall (loc,tid) in bktTask.localeAndIds(A) with (ref state) do
        on loc {
          const task = bktTask;
          const taskStart = task.start;
          const taskEnd = task.start + task.size - 1;

          const bktFirstLocale = bktTask.firstLocaleId;
          const bktLastLocale = bktTask.lastLocaleId;

          // Step 2: scan
          // Note that the globalCounts arrays are stored in transpose
          // order up until now to optimize communication, so we first
          // rearrange them.

          // Temporarily use the globalEnds array to reorder
          {
            ref globalCounts = state.perLocale[tid].globalCounts;
            ref globalEnds = state.perLocale[tid].globalEnds;

            // Compute the transpose
            forall (tid,bkt) in {0..#nLocalesTotal, 0..#maxBuckets} with (ref globalEnds) {
              var count = 0;
              if bktFirstLocale <= tid && tid <= bktLastLocale then
                count = globalCounts[tid*maxBuckets+bkt];

              globalEnds[bkt*nLocalesTotal+tid] = count;
            }
            globalCounts = globalEnds;

            // Do the scan itself
            globalEnds = (+ scan globalCounts) + task.start;
          }

          if debugDist && tid == bktLocId {
            var total = 0;
            for i in 0..#state.countsSize {
              if state.perLocale[bktLocId].globalCounts[i] != 0 {
                total += state.perLocale[bktLocId].globalCounts[i];
                writeln(tid, " state.globalCounts[", i, "]=",
                    state.perLocale[bktLocId].globalCounts[i]);
                writeln(tid, " state.globalEnds[", i, "]=",
                    state.perLocale[bktLocId].globalEnds[i]);
              }
            }
            assert(total == task.size);
          }

          // Step 3: distribute the keys to the src array according
          // to the globalEnds.
          // In particular, bin i from task j should be stored into
          // globalEnds[i*ntasks+j-1] .. globalEnds[i*ntasks+j] - 1
          // (because the scan is inclusive)

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
              offset += globalCounts[bin*nLocalesTotal + tid];
            }
          }

          forall bin in 0..#nBuckets with (ref A) {
            var size = globalCounts[bin*nLocalesTotal + tid];
            if size > 0 {
              var localStart = localOffsets[bin];
              var localEnd = localStart + size - 1;
              var globalStart = if bin*nLocalesTotal+tid > 0
                                then globalEnds[bin*nLocalesTotal+tid-1]
                                else taskStart;
              var globalEnd = globalEnds[bin*nLocalesTotal+tid] - 1;
              if debugDist {
                writeln("bin ", bin, " tid ", tid, " range ", taskStart..taskEnd,
                        " A[", globalStart, "..", globalEnd, "] = Scratch[",
                        localStart, "..", localEnd, "]");
                assert(globalCounts[bin*nLocalesTotal+tid] ==
                    state.perLocale[bktLocId].globalCounts[bin*nLocalesTotal+tid]);
                assert(globalEnds[bin*nLocalesTotal+tid] ==
                    state.perLocale[bktLocId].globalEnds[bin*nLocalesTotal+tid]);
              }
              ShallowCopy.shallowCopy(A, globalStart, Scratch, localStart, size);
            }
          }
        }

        // aka barrier

        // now the data is all in A

        // Step 4: Add sub-tasks depending on if the bin is local or distributed
        // still.
        ref bktOwnerState = state.perLocale[bktLocId];

        for bin in bktOwnerState.compat.bucketizer.getBinsToRecursivelySort() {
          const binStart = if bin*nLocalesTotal > 0
                            then bktOwnerState.globalEnds[bin*nLocalesTotal-1]
                            else bktTask.start;
          const binEnd = bktOwnerState.globalEnds[bin*nLocalesTotal+nLocalesTotal-1] - 1;
          const binSize = binEnd - binStart + 1;
          const binStartBit = bktOwnerState.compat.bucketizer.getNextStartBit(bktTask.startbit);
          if binSize > 1 {
            var small = false;
            var theLocaleId = -1;

            // Compute the regions on the same locale as the first, last
            // elements in the bin.
            const firstLoc = A.domain.distribution.idxToLocale(binStart);
            const lastLoc = A.domain.distribution.idxToLocale(binEnd);
            const onFirstLoc = A.localSubdomain(firstLoc)[binStart..binEnd];
            const onLastLoc = A.localSubdomain(lastLoc)[binStart..binEnd];
            var theLocale = firstLoc;
            if onFirstLoc.size == binSize {
              // case 1: all elements are on firstLoc
              small = true;
            } else if binSize <= state.distributedBaseCaseSize {
              // case 2: size is small enough to do on 1 locale
              small = true;
            }
            theLocaleId = theLocale.id;
            assert(A.targetLocales()[theLocaleId] == theLocale);

            if debugDist then
              writeln(bktLocId,
                      " Recursive bin ", bin,
                      " start = ", binStart,
                      " size = ", binSize,
                      " startbit = ", binStartBit,
                      " small = ", small);

            if small {
              var small = new TwoArraySortTask(binStart, binSize, binStartBit,
                                               true, true);

              if debugDist then
                writeln(bktLocId, " Adding small task ", small);

              smallTasksPerLocale[theLocaleId].pushBack(small);
            } else {
              // Create a distributed sorting task
              var firstLocId = firstLoc.id;
              var lastLocId = lastLoc.id;
              assert(A.targetLocales()[firstLocId] == firstLoc);
              assert(A.targetLocales()[lastLocId] == lastLoc);
              if debugDist {
                // the above assumes something block-like.
                // check that the bin only involves firstLoc..lastLoc
                for (loc,tid) in zip(A.targetLocales(),0..) {
                  if !(firstLocId..lastLocId).contains(tid) {
                    assert(A.localSubdomain(loc)[binStart..binEnd].size == 0);
                  }
                }
              }

              var t = new TwoArrayDistSortPerBucketTask(
                                     binStart, binSize, binStartBit,
                                     firstLocId, lastLocId,
                                     useSecondState=false);

              if debugDist then
                writeln(bktLocId, " Adding big subtask", t);

              nextDistTaskElts.pushBack(t);
            }
          }
        }
      }

      // Update distTasks based on nextDistTaskElts
      sort(nextDistTaskElts,
           comparator=new TwoArrayDistSortPerBucketTaskStartComparator());

      // For each of those tasks, decide if they should use
      // counts1 or counts2
      var lastLocaleIdIn1 = -1;
      var lastLocaleIdIn2 = -1;
      for t in nextDistTaskElts {
        if lastLocaleIdIn1 < t.firstLocaleId {
          t.useSecondState = false;
          lastLocaleIdIn1 = t.lastLocaleId;
        } else if lastLocaleIdIn2 < t.firstLocaleId {
          t.useSecondState = true;
          lastLocaleIdIn2 = t.lastLocaleId;
        } else {
          halt("Algorithm Problem!");
        }
      }

      distTask.tasks = nextDistTaskElts;
      nextDistTaskElts.clear();
    }

    // Part B: Handle the "small" subproblems

    if debugDist then
      writef("After big tasks, A is: %?\n", A);

    // Always use state 1 for small subproblems...
    ref state = state1;
    coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state, ref smallTasksPerLocale) do
    on loc {
      // Get the tasks to sort here

      while true {
        if smallTasksPerLocale[tid].isEmpty() then break;
        const task = smallTasksPerLocale[tid].popBack();

        if debugDist then
          writeln(tid, " Doing small task ", task);

        // Run the local task
        ref compat = state.perLocale[tid].compat;
        const taskEnd = task.start + task.size - 1;

        distributedPartitioningSortWithScratchSpaceBaseCase(
            task.start, taskEnd,
            A, Scratch,
            compat, criterion, task.startbit);

        if debugDist {
          checkSorted(task.start, taskEnd, A, criterion);
          writef("%i after small sort, dst is %?\n", tid, A[task.start..taskEnd]);
        }
      }
    }

    if debugDist {
      writef("After small tasks, A is: %?\n", A);
      checkSorted(start_n, end_n, A, criterion);
    }
  }
}


@chpldoc.nodoc
module TwoArrayRadixSort {
  import Sort.defaultComparator;
  private use super.TwoArrayPartitioning;
  private use super.RadixSortHelp;

  proc twoArrayRadixSort(ref Data:[],
                         comparator:?rec = new defaultComparator(),
                         region:range(?) = Data.domain.dim(0)) {

    if !chpl_domainDistIsLayout(Data.domain) {
      compilerWarning("twoArrayRadix sort no longer handles distributed arrays. Please use TwoArrayDistributedRadixSort.twoArrayDistributedRadixSort instead (but note that it is not stable)");
    }

    var baseCaseSize=16;
    var sequentialSizePerTask=4096;
    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    var ScratchDom = {region};
    pragma "no auto destroy"
    var Scratch: [ScratchDom] Data.eltType =
      ScratchDom.buildArray(Data.eltType, initElts=false);

    // It would make sense to touch the memory first here, but early experiments
    // suggest that it doesn't help with CHPL_COMM=none.
    Scratch.dsiElementInitializationComplete();

    var state = new TwoArrayBucketizerSharedState(
      bucketizer=new RadixBucketizer(),
      baseCaseSize=baseCaseSize,
      sequentialSizePerTask=sequentialSizePerTask,
      endbit=endbit);


    partitioningSortWithScratchSpace(region.low.safeCast(int),
                                     region.high.safeCast(int),
                                     Data, Scratch,
                                     state, comparator, 0);

    _do_destroy_array(Scratch, deinitElts=false);
  }
}

@chpldoc.nodoc
module TwoArrayDistributedRadixSort {
  import Sort.defaultComparator;
  private use super.TwoArrayDistributedPartitioning;
  private use super.RadixSortHelp;
  private use super.TwoArrayRadixSort;

  proc twoArrayDistributedRadixSort(ref Data:[],
                                    comparator:?rec = new defaultComparator()) {

    // just run the local version if Data isn't distributed
    if Data._instance.isDefaultRectangular() {
      twoArrayRadixSort(Data, comparator);
      return;
    }

    var sequentialSizePerTask=4096;
    var baseCaseSize=16;
    var distributedBaseCaseSize=1024;

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    // Allocate the Scratch array.
    pragma "no auto destroy"
    var Scratch: Data.type =
      Data.domain.buildArray(Data.eltType, initElts=false);

    // TODO: do some first-touch, which should matter for comm=ugni
    Scratch.dsiElementInitializationComplete();

    var state1 = new TwoArrayDistributedBucketizerSharedState(
      bucketizerType=RadixBucketizer,
      numLocales=Data.targetLocales().size,
      baseCaseSize=baseCaseSize,
      distributedBaseCaseSize=distributedBaseCaseSize,
      endbit=endbit);
    var state2 = new TwoArrayDistributedBucketizerSharedState(
      bucketizerType=RadixBucketizer,
      numLocales=Data.targetLocales().size,
      baseCaseSize=baseCaseSize,
      distributedBaseCaseSize=distributedBaseCaseSize,
      endbit=endbit);

    distributedPartitioningSortWithScratchSpace(
                                     Data.domain.low.safeCast(int),
                                     Data.domain.high.safeCast(int),
                                     Data, Scratch,
                                     state1, state2,
                                     comparator, 0);

    _do_destroy_array(Scratch, deinitElts=false);
  }
}


@chpldoc.nodoc
module InPlacePartitioning {
  // TODO -- based on ips4o
}


// the algorithm here is also called "American Flag Sort"
// it is not stable and not fully parallel
@chpldoc.nodoc
module MSBRadixSort {
  import Sort.{defaultComparator, ShellSort};
  private use super.RadixSortHelp;
  private use OS.POSIX;

  // This structure tracks configuration for the radix sorter.
  record MSBRadixSortSettings {
    param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step
    const sortSwitch = 256; // when sorting <= this # elements, use other sort
    const minForTask = 256; // when sorting >= this many elements, go parallel
    param CHECK_SORTS = false; // do costly extra checks that data is sorted
    param progress = false; // print progress
    const alwaysSerial = false; // never create tasks
    const maxTasks = here.maxTaskPar;//;here.numPUs(logical=true); // maximum number of tasks to make
  }

  proc msbRadixSort(ref Data:[], comparator:?rec = new defaultComparator(),
                    region:range(?)=Data.domain.dim(0)) {

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    msbRadixSort(Data, start_n=region.low, end_n=region.high,
                 comparator,
                 startbit=0, endbit=endbit,
                 settings=new MSBRadixSortSettings());
  }

  // forall with intents used in tuple expansion causes compilation errors,
  // for now, explicitly thwart kernel generation here, as detecting intents is
  // not easy that late in compilation
  pragma "no gpu codegen"
  // startbit counts from 0 and is a multiple of RADIX_BITS
  proc msbRadixSort(ref A:[], start_n:A.idxType, end_n:A.idxType, criterion,
                    startbit:int, endbit:int,
                    settings /* MSBRadixSortSettings */)
  {
    type idxType = A.idxType;
    if startbit > endbit then
      return;

    if( end_n - start_n < settings.sortSwitch ) {
      // Shell sort here works reasonably well for
      // ordered and random input sequences.
      // Insertion sort would improve performance for random sequences
      // but causes performance problems with ordered sequences.
      // Using quicksort here has a similar, but less extreme impact.
      ShellSort.shellSortMoveElts(A, criterion,
                                  start=start_n, end=end_n);
      if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
      return;
    }

    if settings.progress then writeln("radix sort start=", start_n, " end=", end_n, " startbit=", startbit, " endbit=", endbit);

    const radixbits = RADIX_BITS;
    const radix = (1 << radixbits) + 1;

    // 0th bin is for records where we've consumed all the key.
    var offsets:[0..radix] idxType;
    var end_offsets:[0..radix] idxType;
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
        msbRadixSort(A, start_n, end_n, criterion,
                     dataStartBit, endbit, settings);
        return;
      }
    }

    if settings.progress then writeln("accumulate");

    // Step 2: accumulate
    var sum = 0:idxType;
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
      var bigsubs:[0..radix] (idxType,idxType);
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
          msbRadixSort(A, bin_start, bin_end, criterion,
                       subbits, endbit, settings);
        } else {
          // Add it to the list of things to do in parallel
          bigsubs[nbigsubs] = (bin_start, bin_end);
          nbigsubs += 1;
        }
      }

      forall (_,(bin_start,bin_end)) in zip(0..#nbigsubs,bigsubs) with (ref A) {
        msbRadixSort(A, bin_start, bin_end, criterion, subbits, endbit, settings);
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
          msbRadixSort(A, bin_start, bin_end, criterion,
                       startbit + radixbits, endbit, settings);
        }
      }
    }

    if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
  }
}

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
config param useKeyPartStatus = false;

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
@deprecated("The DefaultComparator record has been renamed to :record:`defaultComparator`, please use that name instead")
type DefaultComparator = defaultComparator;

/* Default comparator used in sort functions.*/
record defaultComparator: keyPartComparator {

  /*
   Default compare method used in sort functions.
   Uses the `<` operator to compute the ordering between ``a`` and ``b``.
   See also `The .compare method`_.

   :returns: 1 if ``b < a``
   :returns: 0 if ``a == b``
   :returns: -1 if ``a < b``
   */
  pragma "last resort"
  @deprecated("compare with 'a' and 'b' arguments is deprecated, please use compare with 'x' and 'y' arguments instead")
  inline
  proc compare(a, b) {
    return compare(x=a, y=b);
  }

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

  // TODO: remove and inline into `keyPart` when `useKeyPartStatus` is deprecated
  @chpldoc.nodoc
  inline proc chpl_keyPartInternal(elt: integral, i: int): (keyPartStatus, elt.type) {
    var section = if i > 0 then keyPartStatus.pre else keyPartStatus.returned;
    return (section, elt);
  }

  /*
    Default ``keyPart`` method for integral values.
    See also `The .keyPart method`_.

    :arg elt: the `int` or `uint` of any size to sort
    :arg i: the part number requested

    :returns: ``(keyPartStatus.returned, x)`` if ``i==0``, or
              ``(keyPartStatus.pre, x)`` otherwise
   */
  inline proc keyPart(elt: integral, i: int): (keyPartStatus, elt.type)
    where useKeyPartStatus {
    return chpl_keyPartInternal(elt, i);
  }
  /*
    Default ``keyPart`` method for integral values.
    See also `The .keyPart method`_.

    :arg x: the `int` or `uint` of any size to sort
    :arg i: the part number requested

    :returns: ``(0, x)`` if ``i==0``, or ``(-1, x)`` otherwise
   */
  @deprecated("Using :proc:`keyPart` without 'keyPartStatus' is deprecated, compile with '-suseKeyPartStatus' and update your types if necessary")
  inline proc keyPart(x: integral, i: int): (int(8), x.type)
    where !useKeyPartStatus {
    var (section, part) = chpl_keyPartInternal(x, i);
    return (section:int(8), part);
  }

  // TODO: remove and inline into `keyPart` when `useKeyPartStatus` is deprecated
  @chpldoc.nodoc
  inline proc chpl_keyPartInternal(elt: real(?), i:int): (keyPartStatus, uint(numBits(elt.type))) {
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
  inline proc keyPart(elt: real(?), i:int): (keyPartStatus, uint(numBits(elt.type)))
    where useKeyPartStatus {
    return chpl_keyPartInternal(elt, i);
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
  @deprecated("Using :proc:`keyPart` without 'keyPartStatus' is deprecated, compile with '-suseKeyPartStatus' and update your types if necessary")
  inline proc keyPart(x: chpl_anyreal, i:int): (int(8), uint(numBits(x.type)))
    where !useKeyPartStatus {
    var (section, part) = chpl_keyPartInternal(x, i);
    return (section:int(8), part);
  }

  // TODO: remove and inline into `keyPart` when `useKeyPartStatus` is deprecated
  @chpldoc.nodoc
  inline proc chpl_keyPartInternal(elt: imag(?), i: int): (keyPartStatus, uint(numBits(elt.type))) {
    return chpl_keyPartInternal(elt:real(numBits(elt.type)), i);
  }
  /*
    Default ``keyPart`` method for `imag` values.
    See also `The .keyPart method`_.

    This method works by calling keyPart with the corresponding `real` value.
  */
  inline proc keyPart(elt: imag(?), i: int): (keyPartStatus, uint(numBits(elt.type)))
    where useKeyPartStatus {
    return chpl_keyPartInternal(elt, i);
  }

  /*
    Default ``keyPart`` method for `imag` values.
    See also `The .keyPart method`_.

    This method works by calling keyPart with the corresponding `real` value.
  */
  @deprecated("Using :proc:`keyPart` without 'keyPartStatus' is deprecated, compile with '-suseKeyPartStatus' and update your types if necessary")
  inline proc keyPart(x: chpl_anyimag, i:int):(int(8), uint(numBits(x.type)))
    where !useKeyPartStatus {
    var (section, part) = chpl_keyPartInternal(x, i);
    return (section:int(8), part);
  }

  // TODO: remove and inline into `keyPart` when `useKeyPartStatus` is deprecated
  @chpldoc.nodoc
  inline proc chpl_keyPartInternal(elt: _tuple, i: int) where
                                        isHomogeneousTuple(elt) &&
                                       (isInt(elt(0)) || isUint(elt(0)) ||
                                        isReal(elt(0)) || isImag(elt(0))) {
    // Re-use the keyPart for imag, real
    const (_,part) = this.chpl_keyPartInternal(elt(i), 1);
    if i >= elt.size then
      return (keyPartStatus.pre, 0:part.type);
    else
      return (keyPartStatus.returned, part);
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
  inline proc keyPart(elt: _tuple, i: int) where useKeyPartStatus &&
                                        isHomogeneousTuple(elt) &&
                                       (isInt(elt(0)) || isUint(elt(0)) ||
                                        isReal(elt(0)) || isImag(elt(0))) {
    return chpl_keyPartInternal(elt, i);
  }

  /*
    Default ``keyPart`` method for tuples of `int`, `uint`, `real`, or `imag`
    values.
    See also `The .keyPart method`_.

    :arg x: homogeneous tuple of the numeric type (of any bit width) to sort
    :arg i: the part number requested

    :returns: For `int` and `uint`, returns
              ``(0, x(i))`` if ``i < x.size``, or ``(-1, 0)`` otherwise.
              For `real` and `imag`, uses ``keyPart`` to find the `uint`
              to provide the sorting order.
  */
  @deprecated("Using :proc:`keyPart` without 'keyPartStatus' is deprecated, compile with '-suseKeyPartStatus' and update your types if necessary")
  inline proc keyPart(x: _tuple, i:int) where !useKeyPartStatus &&
                                        isHomogeneousTuple(x) &&
                                       (isInt(x(0)) || isUint(x(0)) ||
                                        isReal(x(0)) || isImag(x(0))) {
    var (section, part) = chpl_keyPartInternal(x, i);
    return (section:int(8), part);
  }

  // TODO: remove and inline into `keyPart` when `useKeyPartStatus` is deprecated
  @chpldoc.nodoc
  inline proc chpl_keyPartInternal(elt: string, i: int): (keyPartStatus, uint(8)) {
    // This assumes that the string is local, which should
    // be OK for the sort module (because the array containing
    // the string must currently be local).
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
    Default ``keyPart`` method for sorting strings.
    See also `The .keyPart method`_.

    .. note::
      Currently assumes that the string is local.

    :arg elt: the string to sort
    :arg i: the part number requested

    :returns: ``(keyPartStatus.returned, byte i of string)`` or
              ``(keyPartStatus.pre, 0)`` if ``i > elt.size``
  */
  inline proc keyPart(elt: string, i: int): (keyPartStatus, uint(8))
    where useKeyPartStatus {
    return chpl_keyPartInternal(elt, i);
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
  @deprecated("Using :proc:`keyPart` without 'keyPartStatus' is deprecated, compile with '-suseKeyPartStatus' and update your types if necessary")
  inline proc keyPart(x:string, i:int): (int(8), uint(8))
    where !useKeyPartStatus {
    var (section, part) = chpl_keyPartInternal(x, i);
    return (section:int(8), part);
  }

  // TODO: remove and inline into `keyPart` when `useKeyPartStatus` is deprecated
  @chpldoc.nodoc
  inline proc chpl_keyPartInternal(elt: c_ptrConst(c_char), i: int): (keyPartStatus, uint(8)) {
    var ptr = elt:c_ptr(uint(8));
    var byte = ptr[i];
    var section = if byte != 0 then keyPartStatus.returned else keyPartStatus.pre;
    var part = byte;
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
  inline proc keyPart(elt: c_ptrConst(c_char), i: int): (keyPartStatus, uint(8))
    where useKeyPartStatus {
    return chpl_keyPartInternal(elt, i);
  }

  /*
    Default ``keyPart`` method for sorting `c_ptrConst(c_char)`.
    See also `The .keyPart method`_.
    :arg x: the `c_ptrConst(c_char)` to sort
    :arg i: the part number requested
    :returns: ``(0, byte i of string)`` or ``(-1, 0)`` if byte ``i`` is ``0``
  */
  @deprecated("Using :proc:`keyPart` without 'keyPartStatus' is deprecated, compile with '-suseKeyPartStatus' and update your types if necessary")
  inline proc keyPart(x:c_ptrConst(c_char), i:int):(int(8), uint(8))
    where !useKeyPartStatus {
    var (section, part) = chpl_keyPartInternal(x, i);
    return (section:int(8), part);
  }
}

/* Reverse comparator built from another comparator.*/
@deprecated("The ReverseComparator record has been renamed to :record:`reverseComparator`, please use that name instead")
type ReverseComparator = reverseComparator;

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
    return canResolveMethod(this.comparator, "chpl_keyPartInternal", a, 0) ||
           canResolveMethod(this.comparator, "keyPart", a, 0);
  }
  @chpldoc.nodoc
  proc hasKeyPartFromKey(a) param {
    if canResolveMethod(this.comparator, "key", a) {
      var key:comparator.key(a).type;
      // Does the defaultComparator have a keyPart for this?
      return canResolveMethod(new defaultComparator(), "chpl_keyPartInternal", key, 0) ||
             canResolveMethod(new defaultComparator(), "keyPart", key, 0);
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
    if canResolveMethod(cmp, "chpl_keyPartInternal", elt, 0) {
      var (section, part) = cmp.chpl_keyPartInternal(elt, i);
      if typeIsBitReversible(part.type) {
        return (reverseKeyPartStatus(section), ~part);
      } else if typeIsNegateReversible(part.type) {
        return (reverseKeyPartStatus(section), -part);
      } else {
        compilerError("keyPart must return int or uint");
      }
    } else {
      var (section, part) = cmp.keyPart(elt, i);
      if typeIsBitReversible(part.type) {
        return (reverseKeyPartStatus(section), ~part);
      } else if typeIsNegateReversible(part.type) {
        return (reverseKeyPartStatus(section), -part);
      } else {
        compilerError("keyPart must return int or uint");
      }
    }
  }

  // TODO: remove and inline into `keyPart` when `useKeyPartStatus` is deprecated
  @chpldoc.nodoc
  inline proc chpl_keyPartInternal(elt, i)
    where (hasKeyPart(elt) || hasKeyPartFromKey(elt)) {
    chpl_check_comparator(this.comparator, elt.type, doDeprecationCheck=false);
    if hasKeyPartFromKey(elt) {
      return getKeyPart(new defaultComparator(), this.comparator.key(elt), i);
    } else {
      return getKeyPart(this.comparator, elt, i);
    }
  }

  /*
    Reverses ``comparator.keyPart``. See also `The .keyPart method`_.
  */
  inline proc keyPart(elt, i) where useKeyPartStatus &&
                                    (hasKeyPart(elt) ||
                                     hasKeyPartFromKey(elt)) {
    return chpl_keyPartInternal(elt, i);
  }

  /*
    Reverses ``comparator.keyPart``. See also `The .keyPart method`_.
  */
  @deprecated("Using :proc:`keyPart` without 'keyPartStatus' is deprecated, compile with '-suseKeyPartStatus' and update your types if necessary")
  inline proc keyPart(a, i) where !useKeyPartStatus &&
                                  (hasKeyPart(a) || hasKeyPartFromKey(a)) {
    var (section, part) = chpl_keyPartInternal(a, i);
    return (section:int(8), part);
  }

  @chpldoc.nodoc
  inline
  proc doCompare(cmp, a, b) {
    return cmp.compare(b, a);
  }

  /*
   Reverses ``comparator.compare``. See also `The .compare method`_.
   */
  pragma "last resort"
  @deprecated("compare with 'a' and 'b' arguments is deprecated, please use compare with 'x' and 'y' arguments instead")
  inline
  proc compare(a, b) where hasCompare(a, b) || hasCompareFromKey(a) {
    return compare(x=a, y=b);
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
