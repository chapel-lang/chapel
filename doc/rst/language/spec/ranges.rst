.. default-domain:: chpl

.. _Chapter-Ranges:

Ranges
======

A *range* is a first-class, constant-space representation of a regular
sequence of values.  These values are typically integers, though
ranges over bools, enums, and other types are also supported.  Ranges
support serial and parallel iteration over the sequence of values they
represent, as well as operations such as counting, striding,
intersection, shifting, and comparisons.  Ranges form the basis for
defining rectangular domains (:ref:`Chapter-Domains`) and arrays
(:ref:`Chapter-Arrays`) in Chapel.

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
:math:`low \leq ix \leq high`                                               if :math:`stride = 1` or :math:`stride = -1`
:math:`low \leq ix \leq high` and :math:`ix \equiv alignmt \pmod{|stride|}` if :math:`alignmt` is not ambiguous
the represented sequence is undefined                                       otherwise
=========================================================================== ============================================

The sequence, if defined, is increasing if :math:`stride > 0` and
decreasing if :math:`stride < 0`.

If the represented sequence is defined but there are no indices
satisfying the applicable equation(s) above, the range and its
represented sequence are *empty*. A common case of this occurs when the
low bound is greater than the high bound.

We say that a value :math:`ix` is *aligned* w.r.t. the range
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

The type of a range is characterized by three properties:

-  ``idxType`` is the type of the values in the range’s represented
   sequence. However, when the range’s low and/or high bound is
   :math:`\infty`, the represented sequence also contains indices that
   are not representable by ``idxType``.

   ``idxType`` must be an integral, boolean, or enumerated type and is
   ``int`` by default. The range’s low bound and high bound (when they
   are not :math:`\infty`) and alignment are of the type ``idxType``.
   The range’s stride is of the signed integer type that has the same
   bit size as ``idxType`` for integral ranges; for boolean and
   enumerated ranges, it is simply ``int``.

-  ``bounds`` indicates which of the range’s bounds are not
   :math:`\infty`. ``bounds`` is an enumeration constant of the
   type ``BoundedRangeType``. It is discussed further below.

-  ``stridable`` is a boolean that determines whether the range’s stride
   can take on values other than 1. ``stridable`` is ``false`` by
   default. A range is called *stridable* if its type’s ``stridable``
   field is ``true``.

``bounds`` is one of the constants of the following enumeration:

.. enum::  enum boundKind { both, low, high, neither };

The value of ``bounds`` determines which bound(s) of the range are
specified (making the range “bounded”, as opposed to infinite, in the
corresponding direction(s)) as follows:

-  ``both``: both bounds are specified.
   Such ranges are called *bounded*.

-  ``low``: the low bound is specified, the high bound is +\ :math:`\infty`.

-  ``high``: the high bound is specified, the low bound is -:math:`\infty`.

-  ``neither``: neither bound is specified, both bounds are :math:`\infty`.
   Such ranges are called *unbounded*.

``bounds`` is ``boundKind.both`` by default. 

The parameters ``idxType``, ``bounds``, and ``stridable`` affect
all values of the corresponding range type. For example, the range’s low
bound is -:math:`\infty` if and only if the ``bounds`` of that
range’s type is either ``high`` or ``neither``.

   *Rationale*.

   Providing ``bounds`` and ``stridable`` in a range’s type
   allows the compiler to identify and optimize the common cases where
   the range is bounded in both directions and/or its stride is 1.

A range type has the following syntax: 

.. code-block:: syntax

   range-type:
     'range' ( named-expression-list )

That is, a range type is obtained as if by invoking the range type
constructor (:ref:`Type_Constructors`) that has the following
header:

.. code-block:: chapel

     proc range(type idxType = int,
                param bounds = boundKind.both,
                param stridable = false) type

As a special case, the keyword ``range`` without a parenthesized
argument list refers to the range type with the default values of all
its parameters, i.e., ``range(int, boundKind.both, false)``.

   *Example (rangeVariable.chpl)*.

   The following declaration declares a variable ``r`` that can
   represent ranges of 32-bit integers, with both low and high bounds
   specified, and the ability to have a stride other than 1.

   .. code-block:: chapel

      var r: range(int(32), boundKind.both, stridable=true);

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

-  ``bounds`` is a value of the type ``boundKind`` that is
   determined as follows:

   -  ``both``, if both the lower bound and the upper bound expressions
      are given,

   -  ``low``, if only the upper bound expression is given,

   -  ``high``, if only the lower bound expression is given,

   -  ``neither``, if neither bound expression is given.

-  ``stridable`` is ``false``.

The value of a range literal is as follows:

- The low bound is given by the lower bound expression, if present, and
  is -:math:`\infty` otherwise.

- When the range has an upper bound expression, a closed-interval
  range (``..``) takes the expression's value as its high bound;
  whereas the high bound of a half-open interval range (``..<``)
  excludes the upper bound and is one less than the upper
  bound expression.  If there is no upper bound expression, the high
  bound is +\ :math:`\infty`.

-  The stride is 1.

-  The alignment is ambiguous.

.. _Range_Default_Values:

Default Values
~~~~~~~~~~~~~~

The default value for a range with an integral ``idxType`` depends on
the type’s ``bounds`` parameter as follows:

-  ``1..0`` (an empty range) if ``bounds`` is ``both``

-  ``1..`` if ``bounds`` is ``low``

-  ``..0`` if ``bounds`` is ``high``

-  ``..`` if ``bounds`` is ``neither``

..

   *Rationale*.

   We use 0 and 1 to represent an empty range because these values are
   available for any integer ``idxType`` with more than one value.

   We have not found the natural choice of the default value for
   ranges with ``low`` and ``high`` ``bounds``. The values indicated above
   are distinguished by the following property. Slicing the default
   value for a ``low``-bounded range with the default value for a
   ``high``-bounded range (or visa versa) produces an empty range,
   matching the default value for a ``both``-bounded range

Default values of ranges with boolean ``idxType`` are similar, but
substituting ``false`` and ``true`` for 0 and 1 above.  Ranges with
``enum`` ``idxType`` use the 0th and 1st values in the enumeration in
place of 0 and 1 above.  If the enum only has a single value, the
default value uses the 0th value as the low bound and has an undefined
high bound; the ``.size`` query should be used with such ranges before
querying the high bound to determine whether or not it is valid.

.. _Ranges_Common_Operations:

Common Operations
-----------------

All operations on a range return a new range rather than modifying the
existing one. This supports a coding style in which all range values are
*immutable*.

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

-  the two range types have the same ``bounds``, and

-  either the destination range is stridable or the source range is not
   stridable.

.. _Range_Comparisons:

Range Comparisons
~~~~~~~~~~~~~~~~~

Ranges can be compared using equality and inequality.

.. function:: operator ==(r1: range(?), r2: range(?)): bool

   Returns ``true`` if the two ranges have the same represented sequence or
   the same four primary properties, and ``false`` otherwise.

.. function:: operator !=(r1: range(?), r2: range(?)): bool

   Returns ``false`` if the two ranges have the same represented sequence or
   the same four primary properties, and ``true`` otherwise.

.. _Iterating_over_Ranges:

Iterating over Ranges
~~~~~~~~~~~~~~~~~~~~~

A range can be used as an iterator expression in a loop. This is legal
only if the range is iterable. In this case, the loop iterates over the
members of the range’s represented sequence in the order defined by the
sequence. If the range is empty, no iterations are executed.

Overflow of the index variable while iterating over an unbounded range
leads to undefined behavior.  For unbounded ranges of bool or enum
index type, the iteration will stop at the last value represented
by the type.

In order for it to be possible to iterate over a range with a last
index, it needs to be possible to add the stride to the range's last
index without overflowing the index type. In other words, the last index
plus the stride must be between the index type's minimum and maximum
value (inclusive). If this property is not met, the program will have
undefined behavior.

   *Implementation Notes*.

   When bounds checking is enabled, the case in the above paragraph is
   checked at runtime and the program will halt if the range iteration is
   invalid.

.. _Iterating_over_Unbounded_Ranges:

Iterating over Unbounded Ranges
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When an unbounded range of integer values is used to drive a loop,
either by being the only iterand, or by serving as the leader iterand
of a zippered iteration, it will generate a conceptually infinite
number of iterations.  In order for such loops to be useful in
practice, they must typically contain a ``break`` or ``return``
statement.  Of course, in practice, the values representable by
``idxType`` are finite; as a result, when the loop reaches its extreme
values, the behavior of the loop is undefined.

   *Implementation Notes*.

   In the current implementation of Chapel, the loop will halt with an
   error once it yields a value within ``stride`` of the maximal
   ``idxType`` value.

When an unbounded range of integer values serves as a follower iterand
in a zippered context ( :ref:`Zippered_Iteration`), it will generate as
many indices as are needed to match its leader iterand.

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

When an unbounded range of ``bool`` or ``enum`` values is used in a
loop context, it is equivalent to a bounded range where the omitted
low/high bound is taken to be the ``false``/``true`` for a ``bool``
range or the type's initial/final value for an ``enum`` range.

.. _Range_Promotion_of_Scalar_Functions:

Range Promotion of Scalar Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Range values may be passed to a scalar function argument whose type
matches the range’s index type. This results in a promotion of the
scalar function as described in :ref:`Promotion`.

   *Example (rangePromotion.chpl)*.

   Given a function ``addOne(x: int)`` that accepts ``int`` values,
   the function ``addOne()`` can be called with the range ``1..10`` as
   its actual argument, which will result in the function being
   invoked for each value in the range in a data-parallel manner.

   .. code-block:: chapel

      proc addOne(x: int) {
        return x + 1;
      }
      var A: [1..10] int;
      A = addOne(1..10);

   .. BLOCK-test-chapelpost

      writeln(A);

   .. BLOCK-test-chapeloutput

      2 3 4 5 6 7 8 9 10 11

   The last statement is equivalent to: 

   .. code-block:: chapel

      forall (a, i) in zip(A, 1..10) do
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
     range-expression 'by' step-expression

   step-expression:
     expression

The step expression can be any integral or boolean type.  Boolean
values are interpreted as 0 or 1, respectively.  It is an error for
the step to be zero.

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
   expression.  We have also found that using a strict ordering of low
   and high bounds can be clearer when using ranges to slice
   arrays.

.. _Align_Operator_For_Ranges:

Align Operator
~~~~~~~~~~~~~~

The ``align`` operator can be applied to any range, and creates a new
range with the given alignment.

The syntax for the ``align`` operator is: 

.. code-block:: syntax

   aligned-range-expression:
     range-expression 'align' expression

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

To create a range aligned relative to its ``first`` index, see
``range.offset`` below.

.. _Count_Operator:

Count Operator
~~~~~~~~~~~~~~

The ``#`` operator takes a range and an integral count and creates a
new range containing the specified number of indices. Specifically:

-  If the count is positive, :math:`count` indices are taken starting from
   the first index of the range argument.

-  If the count is negative, :math:`-count` indices are taken starting from
   the last index of the range argument.

-  If the count is zero, the result is an empty range.

.. code-block:: syntax

   counted-range-expression:
     range-expression # expression

The count expression can be any integral or boolean value, where
boolean values are interpreted as 0 or 1, respectively.

In detail, the result of the ``#`` operator is calculated as follows.
The type, stride, and alignment of the result are those of the range
argument, except the result is always bounded.

Depending on the sign of the count and the stride, the high or low bound
is unchanged and the other bound is adjusted to provide
the specified number of indices. Specifically:

-  If the count and the stride are both positive or both negative,
   the indices are taken starting from the range's low bound.
   In this case, the low bound is preserved and the high bound is set to
   :math:`low bound + count * stride - 1`.

-  If the count and the stride have opposite signs,
   the indices are taken starting from the range's high bound.
   In this case, the high bound is preserved and the low bound is set to
   :math:`high bound + count * stride + 1`.

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
It is an error if the count is greater than the ``size`` of the range.

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
with the range’s index type. The ``bounds`` and ``stridable`` parameters for
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
represented sequences. The stride, alignment, and bounds of the
resulting range are adjusted as needed to make this true. ``idxType``
of the result is determined by the first operand.
The stride of the result is of the same sign as the stride
of the first operand if the second operand's stride is positive,
and of the opposite sign otherwise.

Range slicing is specified by the syntax: 

.. code-block:: syntax

   sliced-range-expression:
     range-expression ( range-expression )
     range-expression [ range-expression ]

..

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

It is an error for the first operand to be ambiguously aligned.
If the second operand is ambiguously aligned, it is replaced
with a range that is identical except it is given an alignment
in such a way that that the intersection of the two ranges'
represented sequences is non-empty, if possible.
How this substitute alignment is chosen when multiple possibilities
are available is implementation-dependent.

If the resulting sequence cannot be expressed as a range with the
original ``idxType``, the slice expression evaluates to the empty
range. This can happen, for example, when the first operand is an
unbounded range with unsigned ``idxType`` and the second operand
represents only negative numbers.

If the resulting sequence is empty, the bounds, stride, and alignment
of the resulting range are implementation-dependent.
If the resulting sequence is empty and both operands are unbounded
in the same direction, it is an error.


.. _Predefined_Range_Functions:

Predefined Routines on Ranges
-----------------------------

.. _Range_Type_Accessors:

Range Type Queries
~~~~~~~~~~~~~~~~~~



.. function:: proc range.idxType type

   Returns the type of the range's indices (its ``idxType``).



.. function:: proc range.bounds param : BoundedRangeType

   Returns which bounds the range explicitly represents
   (its ``bounds`` value).



.. function:: proc range.stridable param : bool

   Returns whether or not the range can have non-unit strides (its
   ``stridable`` value).


.. include:: ../../builtins/ChapelRange.rst
