.. default-domain:: chpl

.. module:: Sort
   :synopsis: The Sort module is designed to support standard sort routines.

Sort
====
**Usage**

.. code-block:: chapel

   use Sort;



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

 

.. data:: const defaultComparator: DefaultComparator

   
   Instance of :record:`DefaultComparator` used as default ``comparator=``
   argument when no comparator is passed to a sort function

.. data:: const reverseComparator: ReverseComparator(DefaultComparator)

   
   Instance of :record:`ReverseComparator`. Pass this as the ``comparator=``
   argument of a sort function to reverse the sort order.
   

.. function:: proc sort(Data: [?Dom] ?eltType, comparator: ?rec = defaultComparator)

.. function:: proc isSorted(Data: [?Dom] ?eltType, comparator: ?rec = defaultComparator): bool

   
   Check if array `Data` is in sorted order
   
   :arg Data: The array to verify
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :returns: ``true`` if array is sorted
   :rtype: `bool`
   

.. iterfunction:: iter sorted(x, comparator: ?rec = defaultComparator)

   
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
   
   

.. record:: DefaultComparator

   Default comparator used in sort functions.


   .. method:: proc compare(a, b)

      
      Default compare method used in sort functions.
      Uses the `<` operator to compute the ordering between ``a`` and ``b``.
      See also `The .compare method`_.
      
      :returns: 1 if ``b < a``
      :returns: 0 if ``a == b``
      :returns: -1 if ``a < b``
      

   .. method:: proc keyPart(x: integral, i: int): (int(8), x.type )

      
      Default ``keyPart`` method for integral values.
      See also `The .keyPart method`_.
      
      :arg x: the `int` or `uint` of any size to sort
      :arg i: the part number requested
      
      :returns: ``(0, x)`` if ``i==0``, or ``(-1, x)`` otherwise
      

   .. method:: proc keyPart(x: chpl_anyreal, i: int): (int(8), uint(numBits(x.type )))

      
      Default ``keyPart`` method for `real` values.
      See also `The .keyPart method`_.
      
      :arg x: the `real` of any width to sort
      :arg i: the part number requested
      
      :returns: ``(0, u)`` if ``i==0``, or ``(-1, u)`` otherwise,
                where `u` is a `uint` storing the bits of the `real`
                but with some transformations applied to produce the
                correct sort order.
      

   .. method:: proc keyPart(x: chpl_anyimag, i: int): (int(8), uint(numBits(x.type )))

      
      Default ``keyPart`` method for `imag` values.
      See also `The .keyPart method`_.
      
      This method works by calling keyPart with the corresponding `real` value.
      

   .. method:: proc keyPart(x: _tuple, i: int)

      
      Default ``keyPart`` method for tuples of `int`, `uint`, `real`, or `imag`
      values.
      See also `The .keyPart method`_.
      
      :arg x: homogeneous tuple of the numeric type (of any bit width) to sort
      :arg i: the part number requested
      
      :returns: For `int` and `uint`, returns
                ``(0, x(i))`` if ``i <= x.size``, or ``(-1, 0)`` otherwise.
                For `real` and `imag`, uses ``keyPart`` to find the `uint`
                to provide the sorting order.
      

   .. method:: proc keyPart(x: string, i: int): (int(8), uint(8))

      
      Default ``keyPart`` method for sorting strings.
      See also `The .keyPart method`_.
      
      .. note::
        Currently assumes that the string is local.
      
      :arg x: the string to sort
      :arg i: the part number requested
      
      :returns: ``(0, byte i of string)`` or ``(-1, 0)`` if ``i > x.size``
      

   .. method:: proc keyPart(x: c_string, i: int): (int(8), uint(8))

      
      Default ``keyPart`` method for sorting `c_string`.
      See also `The .keyPart method`_.
      
      :arg x: the `c_string` to sort
      :arg i: the part number requested
      
      :returns: ``(0, byte i of string)`` or ``(-1, 0)`` if byte ``i`` is ``0``
      

.. record:: ReverseComparator

   Reverse comparator built from another comparator.


   .. attribute:: var comparator

      Generic comparator defined in initializer.

   .. method:: proc init()

      
      Initializer - builds a comparator with a compare method that
      reverses the sort order of the default comparator.
      

   .. method:: proc init(comparator)

      
      Initializer - builds a comparator with a compare method that
      reverses the sort order of the argument-provided comparator.
      
      :arg comparator: :ref:`Comparator <comparators>` record that defines how the
         data is sorted.
      
      

   .. method:: proc keyPart(a, i)

      
      Reverses ``comparator.keyPart``. See also `The .keyPart method`_.
      

   .. method:: proc compare(a, b)

      
      Reverses ``comparator.compare``. See also `The .compare method`_.
      

