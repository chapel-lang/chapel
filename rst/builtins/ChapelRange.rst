.. default-domain:: chpl

.. module:: ChapelRange
   :synopsis: A ``range`` is a first-class, constant-space representation of a

Ranges
======
A ``range`` is a first-class, constant-space representation of a
regular sequence of values.  These values are typically integers,
though ranges over enum types are also supported.  Ranges
support iteration over the sequences they represent as well as
operations such as counting, striding, intersection, shifting, and
comparisons.

Range Values
------------
In their simplest form, ranges are represented by their low and high
bounds:


.. code-block:: chapel

  1..3    // 1, 2, 3
  0..n    // 0, 1, 2, 3, ..., n
  lo..hi  // lo, lo+1, lo+2, ..., hi

Ranges may also be `unbounded`, in which case, the lower and/or upper
bounds may be omitted:

.. code-block:: chapel

  1..   // 1, 2, 3, ...
  ..10  // .., 8, 9, 10
  ..    // ..., -2, -1, 0, 1, 2, ...

Ranges over enum types respect the declaration order of its values:

.. code-block:: chapel

  enum color {red=4, orange=2, yellow=1, green=3, blue=6, indigo=7, violet=5};
  color.orange..color.green;   // orange, yellow, green

Range Types
-----------
Range types are generic with respect to three fields:

* ``idxType``: The type of the range's values—must an integral or enum type (defaults to ``int``)
* ``boundedType``: A :enum:`BoundedRangeType` value indicating which bounds the range stores (defaults to ``bounded``)
* ``stridable``: A boolean indicating whether or not the range can be strided (defaults to ``false``)

The following code shows range variables declared with specified
type signatures:

.. code-block:: chapel

  var r1: range = 1..10;
  var r2: range(int(8)) = 1..myInt8;
  var r3: range(color) = color.green..color.blue;
  var r4: range(stridable=true) = 1..10 by 2;
  var r5: range(boundedType=BoundedRangeType.boundedNone) = ..;

Like other variables, these types can be inferred by the compiler
from the initializing expressions for simplicity:

.. code-block:: chapel

  var r1 = 1..10;
  var r2 = 1..myInt8;
  var r3 = color.green..color.blue;
  var r4 = 1..10 by 2;
  var r5 = ..;



Range Operators
---------------
New ranges can be constructed from existing ones using the counting,
striding, and/or alignment operators, ``#``, ``by``, and ``align``:

.. code-block:: chapel

  0..#10              // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  0..10 by 2          // 0, 2, 4, 6, 8, 10
  0..10 by 2 align 1  // 1, 3, 5, 7, 9
  0.. by 2 # 10       // 0, 2, 4, 6, 8, 10, 12, 14, 16, 18

Iteration over ranges
---------------------
Ranges can be used as the iterable expression in ``for``, ``forall``, and ``coforall`` loops.

.. code-block:: chapel

  for i in 1..10 { ... f(i) ... }
  forall i in 1..1000 { ... f(i) ... }
  coforall i in 0..#numTasks { ... f(i) ... }

When ranges that are not fully bounded are zipped with another iterator,
the other iterator is used to determine an ending point.

.. code-block:: chapel

  // (i, j) will take the values: (1, 7), (2, 8), (3, 9), (4, 10)
  for (i, j) in zip(1..4, 7..) { ... }

  // (i, j) will take the values: (1, 10), (2, 9), (3, 8), (4, 7)
  for (i,j) in zip(1..4, ..10 by -1) { ... }

Range Intersection
------------------
A range can be intersected with another range to form a new range representing the intersection of the two ranges by `slicing` one range with the other.

.. code-block:: chapel

  (1..10)[3..8] // 3..8
  (0..20)[1..20 by 2] // 1..20 by 2
  (1..10)[5..] // 5..10
  (1..10)[..5] // 1..5

Range Shifting
--------------
A range can be shifted by an integer using the ``+`` and ``-`` operators.

.. code-block:: chapel

  (1..10) + 5 // 6..15
  (1..10) - 3 // -2..7
  (1..) + 1 // 2..
  (..10) + 1 // ..11

Range Comparisons
-----------------
Ranges can be compared for equality using the ``==`` and ``!=`` operators.

.. code-block:: chapel

  1..10 == 1..10 // true
  1.. == 1.. // true
  1..10 != (1..10 by 2) // true



.. enum:: enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone }

   
   The ``BoundedRangeType`` enum is used to specify the types of bounds a
   range is required to have.
   
   * ``bounded`` - The range has finite low and high bounds.
   * ``boundedLow`` - The range starts at a given low bound, but conceptually goes up to infinity.
   * ``boundedHigh`` - The range conceptually starts at negative infinity and ends at a given high bound.
   * ``boundedNone`` - The range conceptually runs from negative infinity to infinity.
   


.. method:: proc range.intIdxType type

   The ``idxType`` as represented by an integer type.  When
   ``idxType`` is an enum type, this evaluates to ``int``.
   Otherwise, it evaluates to ``idxType``. 

.. function:: proc isRangeType(type t) param

   Return true if argument ``t`` is a range type, false otherwise 

.. function:: proc isBoundedRange(r: range(?)) param

   Return true if argument ``r`` is a fully bounded range, false otherwise 

.. method:: proc range.hasLowBound() param

   Return true if this range has a low bound, false otherwise 

.. method:: proc range.hasHighBound() param

   Returns true if this range has a high bound, false otherwise 

.. method:: proc range.stride

   Returns the stride of the range 

.. method:: proc range.alignment

   Returns the alignment of the range 

.. method:: proc range.aligned

   Returns true if the range is aligned 

.. method:: proc range.first

   Return the first element in the sequence the range represents 

.. method:: proc range.last

   Return the last element in the sequence the range represents 

.. method:: proc range.low

   Return the range's low bound. If the range does not have a low
   bound the behavior is undefined. 

.. method:: proc range.high

   Return the range's high bound. If the range does not have a high
   bound the behavior is undefined. 

.. method:: proc range.alignedLow: idxType

   Returns the range's aligned low bound. If the aligned low bound is
   undefined (does not exist), the behavior is undefined.
   

.. method:: proc range.alignedHigh: idxType

.. method:: proc range.isEmpty()

   If the sequence represented by the range is empty, return true.  An
   error is reported if the range is ambiguous.
   

.. method:: proc range.length: intIdxType

   Deprecated - please use :proc:`range.size`. 

.. method:: proc range.size: intIdxType

   Returns the number of elements in this range, cast to the index type.
   
   Note: The result is undefined if the index is signed
   and the low and high bounds differ by more than ``max(``:proc:`range.intIdxType` ``)``.
   

.. method:: proc range.hasFirst() param

   Return true if the range has a first index, false otherwise 

.. method:: proc range.hasLast() param

   Return true if the range has a last index, false otherwise 

.. method:: proc range.isNaturallyAligned()

   Returns true if this range is naturally aligned, false otherwise 

.. method:: proc range.isAmbiguous() param

   Returns true if the range is ambiguously aligned, false otherwise 

.. method:: proc range.contains(ind: idxType)

   Returns true if ``ind`` is in this range, false otherwise 

.. method:: proc range.contains(other: range(?))

   Returns true if the range ``other`` is contained within this one,
   false otherwise
   

.. method:: proc range.member(ind: idxType)

   Deprecated - please use :proc:`range.contains`. 

.. method:: proc range.member(other: range(?))

   Deprecated - please use :proc:`range.contains`. 

.. function:: proc ident(r1: range(?), r2: range(?))

   Returns true if the two ranges are the same in every respect: i.e. the
   two ranges have the same ``idxType``, ``boundedType``, ``stridable``,
   ``low``, ``high``, ``stride`` and ``alignment`` values.
   

.. method:: proc range.boundsCheck(other: range(?e, ?b, ?s))

   Returns true if ``other`` lies entirely within this range and false
   otherwise.  Returns false if either range is ambiguously aligned.
   

.. method:: proc range.boundsCheck(other: idxType)

   Return true if ``other`` is contained in this range and false otherwise 

.. method:: proc range.indexOrder(ind: idxType)

   
        If ``ind`` is a member of the range's represented sequence, returns
        an integer giving the ordinal index of ind within the sequence
        using zero-based indexing. Otherwise, returns
        ``(-1):``:proc:`range.intIdxType`. It is an error to invoke
        ``indexOrder`` if the represented sequence is not defined or the
        range does not have a first index.
   
        The following calls show the order of index 4 in each of the given ranges:
   
   .. code-block:: chapel
   
          (0..10).indexOrder(4) == 4
          (1..10).indexOrder(4) == 3
          (3..5).indexOrder(4) == 1
          (0..10 by 2).indexOrder(4) == 2
          (3..5 by 2).indexOrder(4) == -1
      

.. method:: proc range.orderToIndex(ord: integral): idxType

   Returns the zero-based ``ord``-th element of this range's represented
   sequence. It is an error to invoke ``orderToIndex`` if the range is not
   defined, or if ``ord`` is negative or greater than the range's size.
   The ``orderToIndex`` procedure is the reverse of ``indexOrder``.
   
   Example:
   
   .. code-block:: chapel
   
     0..10.orderToIndex(4) == 4
     1..10.orderToIndex(3) == 4
     3..5.orderToIndex(1)  == 4
     0..10 by 2.orderToIndex(2) == 4
   

.. method:: proc range.translate(offset: integral)

   Return a range with elements shifted from this range by ``offset``.
   
   Example:
   
   .. code-block:: chapel
   
     0..9.translate(1) == 1..10
     0..9.translate(2) == 2..11
     0..9.translate(-1) == -1..8
     0..9.translate(-2) == -2..7
   

.. method:: proc range.interior(offset: integral)

   Return a range with ``offset`` elements from the interior portion of this
   range. If ``offset`` is positive, take elements from the high end, and if
   ``offset`` is negative, take elements from the low end.
   
   Example:
   
   .. code-block:: chapel
   
     0..9.interior(1)  == 9..9
     0..9.interior(2)  == 8..9
     0..9.interior(-1) == 0..0
     0..9.interior(-2) == 0..1
   

.. method:: proc range.exterior(offset: integral)

   Return a range with ``offset`` elements from the exterior portion of this
   range. If ``offset`` is positive, take elements from the high end, and if
   ``offset`` is negative, take elements from the low end.
   
   Example:
   
   .. code-block:: chapel
   
     0..9.exterior(1)  = 10..10
     0..9.exterior(2)  = 10..11
     0..9.exterior(-1) = -1..-1
     0..9.exterior(-2) = -2..-1
   

.. method:: proc range.expand(offset: integral)

   Return a range expanded by ``offset`` elements from each end.  If ``offset`` is
   negative, the range will be contracted.
   
   Example:
   
   .. code-block:: chapel
   
     0..9.expand(1)  == -1..10
     0..9.expand(2)  == -2..11
     0..9.expand(-1) == 1..8
     0..9.expand(-2) == 2..7
   

.. method:: proc range.offset(in offset: integral)

   Returns a range whose alignment is this range's first index plus ``offset``.
   If the range has no first index, a runtime error is generated.
   

