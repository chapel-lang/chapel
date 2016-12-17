:title: Users Guide: Promotion

.. _ug-promotion:

Promotion: implicit data parallelism
====================================

In Chapel, *function promotion* (or simply *promotion*) refers to an
implicit form of data parallelism that occurs when passing a
collection of values to an argument that expects a single value.  More
precisely, when an array of type *t* (or a range or domain whose index
type is *t*) is passed to a function argument of type *t*, that
function will be called in a data-parallel manner for all values in
the collection.

As a simple example, consider the following function, *negate()* which
takes a ``real`` value by reference and negates it:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :caption:
  :language: chapel
  :lines: 1-3

In addition to calling the function with a single ``real`` variable,
we can also call the function in a promoted manner using an array of
reals, as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 9-10

If we print out the array after making this call, we can see that each
element has been negated, as expected:

.. literalinclude:: examples/users-guide/datapar/promotion.good
   :language: text
   :lines: 3

Moreover, the execution will be equivalent to the following
data-parallel forall-loop:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 14-15

Nothing about the above requires using a 1D array.  We could also
promote *negate()* by passing it a multidimensional array of reals:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 19-22

which would again be equivalent to the loop:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 25-26


Multi-Argument Promotion
------------------------

When multiple function arguments are promoted in a single call, the
result is equivalent to a zippered forall-loop.  As an example,
consider the following function which takes two ``real`` arguments by
**ref** and sorts them relative to one another using swap assignment:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 30-33

This function can be promoted using two arrays of reals as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 34-37

This promotion is equivalent to the following zippered forall-loop:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 45-46

Either form will leave the arrays as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.good
   :language: text
   :lines: 15-16

Promoting multiple arguments simultaneously requires that the
arguments can legally be zippered together, as in an explicit
forall-loop.

All of a function's arguments need not be promoted simultaneously.
Any arguments that are promoted define the implicit, potentially
zippered forall-loop.  All others are simply passed through to their
formal argument normally.  For example, consider the following
function which assigns its *y* argument into its *x* argument if the
third argument, *b*, is true:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 51-54

The following four calls demonstrate that various combinations of
arguments may be promoted, where *Mask* is an array of ``bool``
values:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 67,76,85,94

These calls are equivalent to the following forall-loops:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 71-72,80-81,89-90,98-99

If *A*, *B*, and *Mask* hold the following values before each of
these calls:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 63-65

Then *A* will store the following values afterward:

.. literalinclude:: examples/users-guide/datapar/promotion.good
  :language: text
  :lines: 23,25,27,29

Promoted Calls and Forall Intents
---------------------------------

Note that default forall intents apply to promoted calls as they would
to the equivalent forall-loop.  As an example, consider the following
call.  It is promoted due to its second and third arguments, and tries
to pass a scalar *r* into the first argument which has ``ref`` intent:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 105-106
  :dedent: 2

Attempting to compile this promoted call results in a compile-time
error.  To see the reason, consider the equivalent forall-loop:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 108-109
  :dedent: 2

On the way into the loop, a ``const`` shadow copy of *r* is created as
with any other forall-loop.  But since constants can't be passed by
``ref`` argument intent, an error is triggered.  As with forall
intents on explicit loops, these rules prevent common race conditions.
For example, if the call had been executed, it would be a race as to
which of *B*'s values would be stored into *r*.

Promoted calls do not support a mechanism to override default forall
intents.  Thus, in order to override the default and enable the race,
a user would need to write the loop out explicitly and add a ``with``
clause as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 114-115


Promoted Library Routines and Operators
---------------------------------------

All of the examples above illustrate promotion on procedures written
by a user.  Yet any procedure may be promoted in Chapel, including
library routines and operators.  As an example, consider the following
lines of code:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 119-120,127

The second statement promotes the standard ``sqrt()`` library routine,
resulting in an array of results, stored in *B*:

.. literalinclude:: examples/users-guide/datapar/promotion.good
  :language: text
  :lines: 32

The third statement doubles the array *A* and stores the result in *B*
as expected:

.. literalinclude:: examples/users-guide/datapar/promotion.good
  :language: text
  :lines: 34

Yet rather than supporting an explicit ``+`` operator on arrays of
reals, Chapel implements this statement by promoting the standard
``+`` operator for scalar values of type ``real``.

Even the assignment operations in these statements can be considered
to be promotions of scalar assignment between ``real`` values.  Thus,
the statements above can be considered to be equivalent to:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 123-124,130-131


Promotion vs. Whole-Array Operations
------------------------------------

A key distinction should be made between Chapel's use of promotion
versus whole-array operations as supported in traditional array
languages.  Specifically, consider the following statement:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 136

As expected, this statement doubles each element of *B*, adds it to
its corresponding value in *A*, and then assigns the result to the
corresponding value in *C*.  However, where most array languages would
semantically define this statement by evaluating an operator at a time,
as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 139-140

Chapel defines it using zippered iteration:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 143-144

In this case, the two approaches compute the same values, but the
Chapel approach has the benefit of avoiding temporary arrays to store
intermediate array values.  This makes Chapel's memory utilization
more explicit to the user and also tends to make better use of memory
caches in modern architectures.

For other computations, Chapel's promotion semantics can generate a
different result than most array languages would.  For example,
consider the following computation which attempts to replace each
interior element of *V* with the average of its neighbors:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 148-149

Again, in an array language, this computation would typically proceed
by evaluating each operator in turn:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 163-164

However, Chapel's zippered promotion semantics result in the following
interpretation:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 152-153

which is effectively the same as:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 156-157

These loops have a race condition, and therefore so does the original
whole-array computation in Chapel.  Specifically, when the parallel
loop is executed using multiple tasks, they may try to read and write
the same values of *V* simultaneously.  Thus, where the author likely
intended for all the original values of *V* to be averaged, a given
task may end up reading a value that was computed by one of its
siblings that owned an adjacent iteration.

For this reason, it is important that users of promotion keep the
underlying zippered interpretation in mind and ensure that they are
not writing to values that other iterations may be reading
simultaneously.  For example, the previous computation could be
written safely in Chapel by introducing a temporary array:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 168-169

Alternatively, the user could create an explicit array-oriented
implementation of ``+`` to avoid the promotion.  Of course, such a
procedure would itself need to use a temporary array to store and
return the result:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 173-178
          


Promoting Using Ranges and Domains
----------------------------------

Future:
* shape preservation
* user-defined promotable collections
