.. _Chapter-Ranges:

Ranges
======

A *range* is a first-class, constant-space representation of a regular
sequence of indices of either integer, boolean, or enumerated type.
Ranges support iteration over the sequences they represent and are the
basis for defining domains (:ref:`Chapter-Domains`).

Ranges are presented as follows:

-  definition of the key range concepts :ref:`Range_Concepts`

-  range types :ref:`Range_Types`

-  range values :ref:`Range_Values`

-  range assignment :ref:`Range_Assignment`

-  operators on ranges :ref:`Range_Operators`

-  predefined functions on ranges
   :ref:`Predefined_Range_Functions`

.. _Range_Concepts:

Range Concepts
--------------

A range has four primary properties. Together they define the sequence
of indices that the range represents, or the *represented sequence*, as
follows.

-  The *low bound* is either a specific index value or -:math:`\infty`.

-  The *high bound* is either a specific index value or
   +\ :math:`\infty`. The low and high bounds determine the span of the
   represented sequence. Chapel does not represent :math:`\infty`
   explicitly. Instead, infinite bound(s) are represented implicitly in
   the range’s type (:ref:`Range_Types`). When the low and/or high
   bound is :math:`\infty`, the represented sequence is unbounded in the
   corresponding direction(s).

-  The *stride* is a non-zero integer. It defines the distance between
   any two adjacent members of the represented sequence. The sign of the
   stride indicates the direction of the sequence:

   -  :math:`stride > 0` indicates an increasing sequence,

   -  :math:`stride < 0` indicates a decreasing sequence.

-  The *alignment* is either a specific index value or is *ambiguous*.
   It defines how the represented sequence’s members are aligned
   relative to the stride. For a range with a stride other than 1 or -1,
   ambiguous alignment means that the represented sequence is undefined.
   In such a case, certain operations discussed later result in an
   error.

More formally, the represented sequence for the range
:math:`(low, high, stride, alignmt)` contains all indices :math:`ix`
such that:

=========================================================================== ============================================
:math:`low \leq ix \leq high` and :math:`ix \equiv alignmt \pmod{|stride|}` if :math:`alignmt` is not ambiguous
:math:`low \leq ix \leq high`                                               if :math:`stride = 1` or :math:`stride = -1`
the represented sequence is undefined                                       otherwise
=========================================================================== ============================================

The sequence, if defined, is increasing if :math:`stride > 0` and
decreasing if :math:`stride < 0`.

If the represented sequence is defined but there are no indices
satisfying the applicable equation(s) above, the range and its
represented sequence are *empty*. A common case of this occurs when the
high bound is greater than the low bound.

We will say that a value :math:`ix` is *aligned* w.r.t. the range
:math:`(low, high, stride, alignmt)` if:

-  :math:`alignmt` is not ambiguous and
   :math:`ix \equiv alignmt \pmod{|stride|}`, or

-  :math:`stride` is 1 or -1.

Furthermore, :math:`\infty` is never aligned.

Ranges have the following additional properties.

-  A range is *ambiguously aligned* if

   -  its alignment is ambiguous, and

   -  its stride is neither 1 nor -1.

-  The *first index* is the first member of the represented sequence.

   A range *has no* first index when the first member is undefined, that
   is, in the following cases:

   -  the range is ambiguously aligned,

   -  the represented sequence is empty,

   -  the represented sequence is increasing and the low bound is
      -:math:`\infty`,

   -  the represented sequence is decreasing and the high bound is
      +\ :math:`\infty`.

-  The *last index* is the last member of the represented sequence.

   A range *has no* last index when the last member is undefined, that
   is, in the following cases:

   -  it is ambiguously aligned,

   -  the represented sequence is empty,

   -  the represented sequence is increasing and the high bound is
      +\ :math:`\infty`,

   -  the represented sequence is decreasing and the low bound is
      -:math:`\infty`.

-  The *aligned low bound* is the smallest value that is greater than or
   equal to the low bound and is aligned w.r.t. the range, if such a
   value exists.

   The aligned low bound equals the smallest member of the represented
   sequence, when both exist.

-  The *aligned high bound* is the largest value that is less than or
   equal to the high bound and is aligned w.r.t. the range, if such a
   value exists.

   The aligned high bound equals the largest member of the represented
   sequence, when both exist.

-  The range is *iterable*, that is, it is legal to iterate over it, if
   it has a first index.

.. _Range_Types:

Range Types
-----------

The type of a range is characterized by three parameters:

-  ``idxType`` is the type of the indices of the range’s represented
   sequence. However, when the range’s low and/or high bound is
   :math:`\infty`, the represented sequence also contains indices that
   are not representable by ``idxType``.

   ``idxType`` must be an integral, boolean, or enumerated type and is
   ``int`` by default. The range’s low bound and high bound (when they
   are not :math:`\infty`) and alignment are of the type ``idxType``.
   The range’s stride is of the signed integer type that has the same
   bit size as ``idxType`` for integral ranges; for boolean and
   enumerated ranges, it is simply ``int``.

-  ``boundedType`` indicates which of the range’s bounds are not
   :math:`\infty`. ``boundedType`` is an enumeration constant of the
   type ``BoundedRangeType``. It is discussed further below.

-  ``stridable`` is a boolean that determines whether the range’s stride
   can take on values other than 1. ``stridable`` is ``false`` by
   default. A range is called *stridable* if its type’s ``stridable`` is
   ``true``.

``boundedType`` is one of the constants of the following type:



.. code-block:: chapel

   enum BoundedRangeType { bounded, boundedLow, boundedHigh, boundedNone };

The value of ``boundedType`` determines which bounds of the range are
specified (making the range “bounded”, as opposed to infinite, in the
corresponding direction(s)) as follows:

-  ``bounded``: both bounds are specified.

-  ``boundedLow``: the low bound is specified (the high bound is
   +\ :math:`\infty`).

-  ``boundedHigh``: the high bound is specified (the low bound is
   -:math:`\infty`).

-  ``boundedNone``: neither bound is specified (both bounds are
   :math:`\infty`).

``boundedType`` is ``BoundedRangeType.bounded`` by default.

The parameters ``idxType``, ``boundedType``, and ``stridable`` affect
all values of the corresponding range type. For example, the range’s low
bound is -:math:`\infty` if and only if the ``boundedType`` of that
range’s type is either ``boundedHigh`` or ``boundedNone``.

   *Rationale*.

   Providing ``boundedType`` and ``stridable`` in a range’s type allows
   the compiler to identify the more common cases where the range is
   ``bounded`` and/or its stride is 1. The compiler can also detect user
   and library code that is specialized to these cases. As a result, the
   compiler has the opportunity to optimize these cases and the
   specialized code more aggressively.

A range type has the following syntax: 

.. code-block:: syntax

   range-type:
     `range' ( named-expression-list )

That is, a range type is obtained as if by invoking the range type
constructor (:ref:`Type_Constructors`) that has the following
header:



.. code-block:: chapel

     proc range(type idxType = int,
                param boundedType = BoundedRangeType.bounded,
                param stridable = false) type

As a special case, the keyword ``range`` without a parenthesized
argument list refers to the range type with the default values of all
its parameters, i.e., ``range(int, BoundedRangeType.bounded, false)``.

   *Example (rangeVariable.chpl)*.

   The following declaration declares a variable ``r`` that can
   represent ranges of 32-bit integers, with both high and low bounds
   specified, and the ability to have a stride other than 1.
   

   .. code-block:: chapel

      var r: range(int(32), BoundedRangeType.bounded, stridable=true);

   

   .. BLOCK-test-chapelpost

      writeln(r);
      var i32: int(32) = 3;
      r = i32..13 by 3 align 1;
      writeln(r);

   

   .. BLOCK-test-chapeloutput

      1..0
      3..13 by 3 align 1

.. _Range_Values:

Range Values
------------

A range value consists of the range’s four primary properties
(:ref:`Range_Concepts`): low bound, high bound, stride and
alignment.

.. _Range_Literals:

Range Literals
~~~~~~~~~~~~~~

Range literals are specified with the following syntax.



.. code-block:: syntax

   range-literal:
     expression .. expression
     expression ..< expression
     expression ..
     .. expression
     ..< expression
     ..

The expressions to the left and to the right of ``..`` or ``..<``,
when given, are called the `lower bound expression` and the `upper
bound expression`, respectively.  The ``..`` operator defines a
closed-interval range, whereas the ``..<`` operator defines a
half-open interval.

The type of a range literal is a range with the following parameters:

-  ``idxType`` is determined as follows:

   -  If both the lower bound and the upper bound expressions are given and
      have the same type, then ``idxType`` is that type.

   -  If both the lower bound and the upper bound expressions are given and
      an implicit conversion is allowed from one expression’s type to
      the other’s, then ``idxType`` is that type.

   -  If only one bound expression is given and it has an integral,
      boolean, or enumerated type, then ``idxType`` is that type.

   -  If neither bound expression is given, then ``idxType`` is ``int``.

   -  Otherwise, the range literal is not legal.

-  ``boundedType`` is a value of the type ``BoundedRangeType`` that is
   determined as follows:

   -  ``bounded``, if both the lower bound and the upper bound expressions
      are given,

   -  ``boundedLow``, if only the upper bound expression is given,

   -  ``boundedHigh``, if only the lower bound expression is given,

   -  ``boundedNone``, if neither bound expression is given.

-  ``stridable`` is ``false``.

The value of a range literal is as follows:

- The low bound is given by the lower bound expression, if present, and
  is -:math:`\infty` otherwise.

- When the range has an upper bound expression, a closed-interval
  range (``..``) takes the expression's value as its high bound;
  whereas the high bound of a half-open interval range (``..<``)
  excludes the upper bound and is therefore one less than the upper
  bound expression.  If there is no upper bound expression, the high
  bound is +\ :math:`\infty`.

-  The stride is 1.

-  The alignment is ambiguous.

.. _Range_Default_Values:

Default Values
~~~~~~~~~~~~~~

The default value for a range type depends on the type’s ``boundedType``
parameter as follows:

-  ``1..0`` (an empty range) if ``boundedType`` is ``bounded``

-  ``1..`` if ``boundedType`` is ``boundedLow``

-  ``..0`` if ``boundedType`` is ``boundedHigh``

-  ``..`` if ``boundedType`` is ``boundedNone``

..

   *Rationale*.

   We use 0 and 1 to represent an empty range because these values are
   available for any ``idxType``.

   We have not found the natural choice of the default value for
   ``boundedLow`` and ``boundedHigh`` ranges. The values indicated above
   are distinguished by the following property. Slicing the default
   value for a ``boundedLow`` range with the default value for a
   ``boundedHigh`` range (or visa versa) produces an empty range,
   matching the default value for a ``bounded`` range

.. _Ranges_Common_Operations:

Common Operations
-----------------

All operations on a range return a new range rather than modifying the
existing one. This supports a coding style in which all ranges are
*immutable* (i.e. declared as ``const``).

   *Rationale*.

   The intention is to provide ranges as immutable objects.

   Immutable objects may be cached without creating coherence concerns.
   They are also inherently thread-safe. In terms of implementation,
   immutable objects are created in a consistent state and stay that
   way: Outside of initializers, internal consistency checks can be
   dispensed with.

   These are the same arguments as were used to justify making strings
   immutable in Java and C#.

.. _Range_Assignment:

Range Assignment
~~~~~~~~~~~~~~~~

Assigning one range to another results in the target range copying the
low and high bounds, stride, and alignment from the source range.

Range assignment is legal when:

-  An implicit conversion is allowed from ``idxType`` of the source
   range to ``idxType`` of the destination range type,

-  the two range types have the same ``boundedType``, and

-  either the destination range is stridable or the source range is not
   stridable.

.. _Range_Comparisons:

Range Comparisons
~~~~~~~~~~~~~~~~~

Ranges can be compared using equality and inequality.



.. code-block:: chapel

   proc ==(r1: range(?), r2: range(?)): bool

Returns ``true`` if the two ranges have the same represented sequence or
the same four primary properties, and ``false`` otherwise.

.. _Iterating_over_Ranges:

Iterating over Ranges
~~~~~~~~~~~~~~~~~~~~~

A range can be used as an iterator expression in a loop. This is legal
only if the range is iterable. In this case the loop iterates over the
members of the range’s represented sequence, in the order defined by the
sequence. If the range is empty, no iterations are executed.

   *Implementation Notes*.

   An attempt to iterate over a range causes an error if adding stride
   to the range’s last index overflows its index type, i.e. if the sum
   is greater than the index type’s maximum value, or smaller than its
   minimum value.

.. _Iterating_over_Unbounded_Ranges_in_Zippered_Iterations:

Iterating over Unbounded Ranges in Zippered Iterations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When a range with the first index but without the last index is used in
a zippered iteration ( :ref:`Zipper_Iteration`), it generates as
many indices as needed to match the other iterator(s).

   *Example (zipWithUnbounded.chpl)*.

   The code 

   .. code-block:: chapel

      for i in zip(1..5, 3..) do
        write(i, "; ");

   

   .. BLOCK-test-chapelpost

      writeln();

   produces the output 

   .. code-block:: printoutput

      (1, 3); (2, 4); (3, 5); (4, 6); (5, 7); 

.. _Range_Promotion_of_Scalar_Functions:

Range Promotion of Scalar Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Range values may be passed to a scalar function argument whose type
matches the range’s index type. This results in a promotion of the
scalar function as described in :ref:`Promotion`.

   *Example (rangePromotion.chpl)*.

   Given a function ``addOne(x:int)`` that accepts ``int`` values and a
   range ``1..10``, the function ``addOne()`` can be called with
   ``1..10`` as its actual argument which will result in the function
   being invoked for each value in the range.

   

   .. code-block:: chapel

      proc addOne(x:int) {
        return x + 1;
      }
      var A:[1..10] int;
      A = addOne(1..10);

   

   .. BLOCK-test-chapelpost

      writeln(A);

   

   .. BLOCK-test-chapeloutput

      2 3 4 5 6 7 8 9 10 11

The last statement is equivalent to: 

.. code-block:: chapel

   forall (a,i) in zip(A,1..10) do
     a = addOne(i);

.. _Range_Operators:

Range Operators
---------------

The following operators can be applied to range expressions and are
described in this section: stride (``by``), alignment (``align``), count
(``#``) and slicing (``()`` or ``[]``). Chapel also defines a set
of functions that operate on ranges. They are described in
:ref:`Predefined_Range_Functions`.



.. code-block:: syntax

   range-expression:
     expression
     strided-range-expression
     counted-range-expression
     aligned-range-expression
     sliced-range-expression

.. _By_Operator_For_Ranges:

By Operator
~~~~~~~~~~~

The ``by`` operator selects a subsequence of the range’s represented
sequence, optionally reversing its direction. The operator takes two
arguments, a base range and an integral step. It produces a new range
whose represented sequence contains each :math:`|`\ step\ :math:`|`-th
element of the base range’s represented sequence. The operator reverses
the direction of the represented sequence if step\ :math:`<`\ 0. If the
resulting sequence is increasing, it starts at the base range’s aligned
low bound, if it exists. If the resulting sequence is decreasing, it
starts at the base range’s aligned high bound, if it exists. Otherwise,
the base range’s alignment is used to determine which members of the
represented sequence to retain. If the base range’s represented sequence
is undefined, the resulting sequence is undefined, too.

The syntax of the ``by`` operator is: 

.. code-block:: syntax

   strided-range-expression:
     range-expression `by' step-expression

   step-expression:
     expression

The type of the step must be a signed or unsigned integer of the same
bit size as the base range’s ``idxType``, or an implicit conversion must
be allowed to that type from the step’s type. It is an error for the
step to be zero.

   *Future*.

   We may consider allowing the step to be of any integer type, for
   maximum flexibility.

The type of the result of the ``by`` operator is the type of the base
range, but with the ``stridable`` parameter set to ``true``.

Formally, the result of the ``by`` operator is a range with the
following primary properties:

-  The low and upper bounds are the same as those of the base range.

-  The stride is the product of the base range’s stride and the step,
   cast to the base range’s stride type before multiplying.

-  The alignment is:

   -  the aligned low bound of the base range, if such exists and the
      stride is positive;

   -  the aligned high bound of the base range, if such exists and the
      stride is negative;

   -  the same as that of the base range, otherwise.

..

   *Example (rangeByOperator.chpl)*.

   In the following declarations, range ``r1`` represents the odd
   integers between 1 and 20. Range ``r2`` strides ``r1`` by two and
   represents every other odd integer between 1 and 20: 1, 5, 9, 13 and
   17. 

   .. code-block:: chapel

      var r1 = 1..20 by 2;
      var r2 = r1 by 2;

   

   .. BLOCK-test-chapelpost

      writeln(r1);
      writeln(r2);

   

   .. BLOCK-test-chapeloutput

      1..20 by 2
      1..20 by 4

   *Rationale*.

   *Why isn’t the high bound specified first if the stride is negative?*
   The reason for this choice is that the ``by`` operator is binary, not
   ternary. Given a range ``R`` initialized to ``1..3``, we want
   ``R by -1`` to contain the ordered sequence :math:`3,2,1`. But then
   ``R by -1`` would be different from ``3..1 by -1`` even though it
   should be identical by substituting the value in R into the
   expression.

.. _Align_Operator_For_Ranges:

Align Operator
~~~~~~~~~~~~~~

The ``align`` operator can be applied to any range, and creates a new
range with the given alignment.

The syntax for the ``align`` operator is: 

.. code-block:: syntax

   aligned-range-expression:
     range-expression `align' expression

The type of the resulting range expression is the same as that of the
range appearing as the left operand, but with the ``stridable``
parameter set to ``true``. An implicit conversion from the type of the
right operand to the index type of the operand range must be allowed.
The resulting range has the same low and high bounds and stride as the
source range. The alignment equals the ``align`` operator’s right
operand and therefore is not ambiguous.

   *Example (alignedStride.chpl)*.

   
   .. BLOCK-test-chapelnoprint
      write("|");

   .. code-block:: chapel

      var r1 = 0 .. 10 by 3 align 0;
      for i in r1 do
        write(" ", i);			// Produces " 0 3 6 9".
      writeln();

   .. BLOCK-test-chapelnoprint
      write("|");

   .. code-block:: chapel

      var r2 = 0 .. 10 by 3 align 1;
      for i in r2 do
        write(" ", i);			// Produces " 1 4 7 10".
      writeln();

   .. BLOCK-test-chapeloutput

      | 0 3 6 9
      | 1 4 7 10

When the stride is negative, the same indices are printed in reverse:

   *Example (alignedNegStride.chpl)*.

   
   .. BLOCK-test-chapelnoprint
      write("|");

   .. code-block:: chapel

      var r3 = 0 .. 10 by -3 align 0;
      for i in r3 do
        write(" ", i);			// Produces " 9 6 3 0".
      writeln();

   .. BLOCK-test-chapelnoprint
      write("|");

   .. code-block:: chapel

      var r4 = 0 .. 10 by -3 align 1;
      for i in r4 do
        write(" ", i);			// Produces " 10 7 4 1".
      writeln();

   .. BLOCK-test-chapeloutput

      | 9 6 3 0
      | 10 7 4 1

To create a range aligned relative to its ``first`` index, use the
``offset`` method (:ref:`Range Offset Method <Range_Offset_Method>`).

.. _Count_Operator:

Count Operator
~~~~~~~~~~~~~~

The ``#`` operator takes a range and an integral count and creates a
new range containing the specified number of indices. The low or high
bound of the left operand is preserved, and the other bound adjusted to
provide the specified number of indices. If the count is positive,
indices are taken from the start of the range; if the count is negative,
indices are taken from the end of the range. The count must be less than
or equal to the ``length`` of the range.



.. code-block:: syntax

   counted-range-expression:
     range-expression # expression

The type of the count expression must be a signed or unsigned integer of
the same bit size as the base range’s ``idxType``, or an implicit
conversion must be allowed to that type from the count’s type.

The type of the result of the ``#`` operator is the type of the range
argument.

Depending on the sign of the count and the stride, the high or low bound
is unchanged and the other bound is adjusted so that it is
:math:`c * stride - 1` units away. Specifically:

-  If the count times the stride is positive, the low bound is preserved
   and the high bound is adjusted to be one less than the low bound plus
   that product.

-  If the count times the stride is negative, the high bound is
   preserved and the low bound is adjusted to be one greater than the
   high bound plus that product.

..

   *Rationale*.

   Following the principle of preserving as much information from the
   original range as possible, we must still choose the other bound so
   that exactly *count* indices lie within the range. Making the two
   bounds lie :math:`count * stride - 1` apart will achieve this,
   regardless of the current alignment of the range.

   This choice also has the nice symmetry that the alignment can be
   adjusted without knowing the bounds of the original range, and the
   same number of indices will be produced: 

   .. code-block:: chapel

      r # 4 align 0   // Contains four indices.
      r # 4 align 1   // Contains four indices.
      r # 4 align 2   // Contains four indices.
      r # 4 align 3   // Contains four indices.

It is an error to apply the count operator with a positive count to a
range that has no first index. It is also an error to apply the count
operator with a negative count to a range that has no last index. It is
an error to apply the count operator to a range that is ambiguously
aligned.

   *Example (rangeCountOperator.chpl)*.

   The following declarations result in equivalent ranges. 

   .. code-block:: chapel

      var r1 = 1..10 by -2 # -3;
      var r2 = ..6 by -2 # 3;
      var r3 = -6..6 by -2 # 3;
      var r4 = 1..#6 by -2;

   

   .. BLOCK-test-chapelpost

      writeln(r1 == r2 && r2 == r3 && r3 == r4);
      writeln((r1, r2, r3, r4));

   

   .. BLOCK-test-chapeloutput

      true
      (1..6 by -2, 1..6 by -2, 1..6 by -2, 1..6 by -2)

   Each of these ranges represents the ordered set of three indices: 6,
   4, 2.

.. _Range_Arithmetic:

Arithmetic Operators
~~~~~~~~~~~~~~~~~~~~

The following arithmetic operators are defined on ranges and integral
types:



.. code-block:: chapel

   proc +(r: range, s: integral): range
   proc +(s: integral, r: range): range
   proc -(r: range, s: integral): range

The ``+`` and ``-`` operators apply the scalar via the operator to the
range’s low and high bounds, producing a shifted version of the range.
If the operand range is unbounded above or below, the missing bounds are
ignored. The index type of the resulting range is the type of the value
that would result from an addition between the scalar value and a value
with the range’s index type. The bounded and stridable parameters for
the result range are the same as for the input range.

The stride of the resulting range is the same as the stride of the
original. The alignment of the resulting range is shifted by the same
amount as the high and low bounds. It is permissible to apply the shift
operators to a range that is ambiguously aligned. In that case, the
resulting range is also ambiguously aligned.

   *Example (rangeAdd.chpl)*.

   The following code creates a bounded, non-stridable range ``r`` which
   has an index type of ``int`` representing the indices
   :math:`{0, 1, 2, 3}`. It then uses the ``+`` operator to create a
   second range ``r2`` representing the indices :math:`{1, 2, 3, 4}`.
   The ``r2`` range is bounded, non-stridable, and is represented by
   indices of type ``int``. 

   .. code-block:: chapel

      var r = 0..3;
      var r2 = r + 1;    // 1..4

   

   .. BLOCK-test-chapelpost

      writeln((r, r2));

   

   .. BLOCK-test-chapeloutput

      (0..3, 1..4)

.. _Range_Slicing:

Range Slicing
~~~~~~~~~~~~~

Ranges can be *sliced* using other ranges to create new sub-ranges. The
resulting range represents the intersection between the two ranges’
represented sequences. The stride and alignment of the resulting range
are adjusted as needed to make this true. ``idxType`` and the sign of
the stride of the result are determined by the first operand.

Range slicing is specified by the syntax: 

.. code-block:: syntax

   sliced-range-expression:
     range-expression ( range-expression )
     range-expression [ range-expression ]

If either of the operand ranges is ambiguously aligned, then the
resulting range is also ambiguously aligned. In this case, the result is
valid only if the strides of the operand ranges are relatively prime.
Otherwise, an error is generated at run time.

   *Rationale*.

   If the strides of the two operand ranges are relatively prime, then
   they are guaranteed to have some elements in their intersection,
   regardless whether their relative alignment can be determined. In
   that case, the bounds and stride in the resulting range are valid
   with respect to the given inputs. The alignment can be supplied later
   to create a valid range.

   If the strides are not relatively prime, then the result of the
   slicing operation would be completely ambiguous. The only reasonable
   action for the implementation is to generate an error.

If the resulting sequence cannot be expressed as a range of the original
type, the slice expression evaluates to the empty range ``1..0``. This
can happen, for example, when the operands represent all odd and all
even numbers, or when the first operand is an unbounded range with
unsigned ``idxType`` and the second operand represents only negative
numbers.

   *Example (rangeSlicing.chpl)*.

   In the following example, ``r`` represents the integers from 1 to 20
   inclusive. Ranges ``r2`` and ``r3`` are defined using range slices
   and represent the indices from 3 to 20 and the odd integers between 1
   and 20 respectively. Range ``r4`` represents the odd integers between
   1 and 20 that are also divisible by 3. 

   .. code-block:: chapel

      var r = 1..20;
      var r2 = r[3..];
      var r3 = r[1.. by 2];
      var r4 = r3[0.. by 3];

   

   .. BLOCK-test-chapelpost

      writeln((r, r2, r3, r4));

   

   .. BLOCK-test-chapeloutput

      (1..20, 3..20, 1..20 by 2, 1..20 by 6 align 3)

.. _Predefined_Range_Functions:

Predefined Functions on Ranges
------------------------------

.. _Range_Type_Accessors:

Range Type Parameters
~~~~~~~~~~~~~~~~~~~~~



.. code-block:: chapel

   proc range.boundedType : BoundedRangeType

Returns the ``boundedType`` parameter of the range’s type.



.. code-block:: chapel

   proc range.idxType : type

Returns the ``idxType`` parameter of the range’s type.



.. code-block:: chapel

   proc range.stridable : bool

Returns the ``stridable`` parameter of the range’s type.

.. _Range_Properties:

Range Properties
~~~~~~~~~~~~~~~~

Most of the methods in this subsection report on the range properties
defined in :ref:`Range_Concepts`. A range’s represented sequence
can be examined, for example, by iterating over the range in a for loop
:ref:`The_For_Loop`.

   *Open issue*.

   The behavior of the methods that report properties that may be
   undefined, :math:`\infty`, or ambiguous, may change.



.. code-block:: chapel

   proc range.aligned : bool

Reports whether the range’s alignment is unambiguous.



.. code-block:: chapel

   proc range.alignedHigh : idxType

Returns the range’s aligned high bound. If the aligned high bound is
undefined (does not exist), the behavior is undefined.

   *Example (alignedHigh.chpl)*.

   The following code: 

   .. code-block:: chapel

      var r = 0..20 by 3;
      writeln(r.alignedHigh);

   produces the output 

   .. code-block:: printoutput

      18



.. code-block:: chapel

   proc range.alignedLow : idxType

Returns the range’s aligned low bound. If the aligned low bound is
undefined (does not exist), the behavior is undefined.



.. code-block:: chapel

   proc range.alignment : idxType

Returns the range’s alignment. If the alignment is ambiguous, the
behavior is undefined. See also ``aligned``.



.. code-block:: chapel

   proc range.first : idxType

Returns the range’s first index. If the range has no first index, the
behavior is undefined. See also ``hasFirst``.



.. code-block:: chapel

   proc range.hasFirst(): bool

Reports whether the range has the first index.



.. code-block:: chapel

   proc range.hasHighBound() param: bool

Reports whether the range’s high bound is *not* +\ :math:`\infty`.



.. code-block:: chapel

   proc range.hasLast(): bool

Reports whether the range has the last index.



.. code-block:: chapel

   proc range.hasLowBound() param: bool

Reports whether the range’s low bound is *not* -:math:`\infty`.



.. code-block:: chapel

   proc range.high : idxType

Returns the range’s high bound. If the high bound is +\ :math:`\infty`,
the behavior is undefined. See also ``hasHighBound``.



.. code-block:: chapel

   proc range.isAmbiguous(): bool

Reports whether the range is ambiguously aligned.



.. code-block:: chapel

   proc range.last : idxType

Returns the range’s last index. If the range has no last index, the
behavior is undefined. See also ``hasLast``.



.. code-block:: chapel

   proc range.length : idxType

Returns the number of indices in the range’s represented sequence. If
the represented sequence is infinite or is undefined, an error is
generated.



.. code-block:: chapel

   proc range.low : idxType

Returns the range’s low bound. If the low bound is -:math:`\infty`, the
behavior is undefined. See also ``hasLowBound``.



.. code-block:: chapel

   proc range.size : idxType

Same as :math:`range`.length.



.. code-block:: chapel

   proc range.stride : int(numBits(idxType))

Returns the range’s stride. This will never return 0. If the range is
not stridable, this will always return 1.

.. _Range_Queries:

Other Queries
~~~~~~~~~~~~~



.. code-block:: chapel

   proc range.boundsCheck(r2: range(?)): bool

Returns ``false`` if either range is ambiguously aligned. Returns
``true`` if range ``r2`` lies entirely within this range and ``false``
otherwise.



.. code-block:: chapel

   proc ident(r1: range(?), r2: range(?)): bool

Returns ``true`` if the two ranges are the same in every respect: i.e.
the two ranges have the same ``idxType``, ``boundedType``,
``stridable``, ``low``, ``high``, ``stride`` and ``alignment`` values.



.. code-block:: chapel

   proc range.indexOrder(i: idxType): idxType

If ``i`` is a member of the range’s represented sequence, returns an
integer giving the ordinal index of ``i`` within the sequence using
0-based indexing. Otherwise, returns ``(-1):idxType``. It is an error to
invoke ``indexOrder`` if the represented sequence is not defined or the
range does not have the first index.

   *Example*.

   The following calls show the order of index 4 in each of the given
   ranges: 

   .. code-block:: chapel

      (0..10).indexOrder(4) == 4
      (1..10).indexOrder(4) == 3
      (3..5).indexOrder(4) == 1
      (0..10 by 2).indexOrder(4) == 2
      (3..5 by 2).indexOrder(4) == -1



.. code-block:: chapel

   proc range.member(i: idxType): bool

Returns ``true`` if the range’s represented sequence contains ``i``,
``false`` otherwise. It is an error to invoke ``member`` if the
represented sequence is not defined.



.. code-block:: chapel

   proc range.member(other: range): bool

Reports whether ``other`` is a subrange of the receiver. That is, if the
represented sequences of the receiver and ``other`` are defined and the
receiver’s sequence contains all members of the ``other``\ ’s sequence.

.. _Range_Transformations:

Range Transformations
~~~~~~~~~~~~~~~~~~~~~



.. code-block:: chapel

   proc range.alignHigh()

Sets the high bound of this range to its aligned high bound, if it is
defined. Generates an error otherwise.



.. code-block:: chapel

   proc range.alignLow()

Sets the low bound of this range to its aligned low bound, if it is
defined. Generates an error otherwise.



.. code-block:: chapel

   proc range.expand(i: idxType)

Returns a new range whose bounds are extended by :math:`i` units on each
end. If :math:`i <
0` then the resulting range is contracted by its absolute value. In
symbols, given that the operand range is represented by the tuple
:math:`(l,h,s,a)`, the result is :math:`(l-i,h+i,s,a)`. The stride and
alignment of the original range are preserved. If the operand range is
ambiguously aligned, then so is the resulting range.



.. code-block:: chapel

   proc range.exterior(i: idxType)

Returns a new range containing the indices just outside the low or high
bound of the range (low if :math:`i < 0` and high otherwise). The stride
and alignment of the original range are preserved. Let the operand range
be denoted by the tuple :math:`(l,h,s,a)`. Then:

-  if :math:`i < 0`, the result is :math:`(l+i,l-1,s,a)`,

-  if :math:`i > 0`, the result is :math:`(h+1,h+i,s,a)`, and

-  if :math:`i = 0`, the result is :math:`(l,h,s,a)`.

If the operand range is ambiguously aligned, then so is the resulting
range.



.. code-block:: chapel

   proc range.interior(i: idxType)

Returns a new range containing the indices just inside the low or high
bound of the range (low if :math:`i < 0` and high otherwise). The stride
and alignment of the original range are preserved. Let the operand range
be denoted by the tuple :math:`(l,h,s,a)`. Then:

-  if :math:`i < 0`, the result is :math:`(l,l-(i-1),s,a)`,

-  if :math:`i > 0`, the result is :math:`(h-(i-1),h,s,a)`, and

-  if :math:`i = 0`, the result is :math:`(l,h,s,a)`.

This differs from the behavior of the count operator, in that
``interior()`` preserves the alignment, and it uses the low and high
bounds rather than ``first`` and ``last`` to establish the bounds of the
resulting range. If the operand range is ambiguously aligned, then so is
the resulting range.


.. _Range_Offset_Method:

.. code-block:: chapel

   proc range.offset(n: idxType)

Returns a new range whose alignment is this range’s first index plus
``n``. The new alignment, therefore, is not ambiguous. If the range has
no first index, a run-time error is generated.



.. code-block:: chapel

   proc range.translate(i: integral)

Returns a new range with its ``low``, ``high`` and ``alignment`` values
adjusted by :math:`i`. The ``stride`` value is preserved. If the range’s
alignment is ambiguous, the behavior is undefined.
