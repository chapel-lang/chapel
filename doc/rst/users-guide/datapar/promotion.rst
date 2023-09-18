:title: Users Guide: Promotion

.. _ug-promotion:

Promotion: implicit data parallelism
====================================

In Chapel, *function promotion* (or simply *promotion*) refers to an
implicit form of data parallelism that is triggered by passing a
collection of values to an argument that expects a single value.  More
precisely, when passing:

* an array of type *t*,
* a range or domain whose index type is *t*, or
* a forall expression yielding type *t*,

to a function argument expecting type *t*, that function will be
called in a data-parallel manner for all values in the expression.

As a simple example, consider the following function, ``negate()``
which takes a ``real`` value by reference and negates it:

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
promote ``negate()`` by passing it a multidimensional array of reals:

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
``ref`` and sorts them relative to one another using a swap
assignment:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 30-33

This function can be promoted using two arrays of reals as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 35-38

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
function which assigns its *y* argument into its *x* argument only if
the third argument, *b*, is true:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 51-54

The following calls demonstrate that various combinations of arguments
may be promoted, where *A* and *B* are as above, and *Mask* is an
array of ``bool`` values:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 63-66,85,76

These calls are equivalent to the following forall-loops:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 89-90,80-81

So, after the respective calls above, *A* will store the following
values:

.. literalinclude:: examples/users-guide/datapar/promotion.good
  :language: text
  :lines: 27,25

Any non-promoted arguments to a promoted call are only promoted once. Consider
a promoted call to ``maybeCopy()`` where the first two arguments are promoted
and the third argument is not.

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 199-202,206

The third argument, the result of a call to ``computeMask()``, will be resolved
before the promotion and the message will only be printed once. The equivalent
forall-loop is:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 210-212


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

On the way into the loop, a ``const`` shadow copy of *r* is created,
as with any other forall-loop.  But since constants can't be passed by
``ref`` argument intent, an error is triggered.  As with forall
intents on explicit loops, these rules prevent common race conditions.
For example, if the call *had* been executed, it would be a race as to
which of *B*'s values would be stored into *r*.

Promoted calls do not support a mechanism to override default forall
intents.  Thus, in order to override the default intents, users need
to write out the equivalent loop explicitly and add a ``with`` clause.
For example, the following would yield the equivalent of the promoted
call to ``maybeCopy()`` with the race enabled:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 114-115


Promoting Using Ranges, Domains, Forall Expressions
---------------------------------------------------

As mentioned at the outset, not only can arrays be used to promote
functions, but ranges, domains, and forall expressions can as well.
For example, the following calls promote ``maybeCopy()`` using a
range, a domain, and a forall expression respectively:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 185,189,193

The contents of *A* after these calls is as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.good
  :language: text
  :lines: 45-47

At the time of this writing, Chapel does not support an official way
for users to create their own collection types that support promotion.
We expect to support this capability in the future by having the
collection type support certain well-defined iterator methods.


Promoted Library Routines and Operators
---------------------------------------

All of the examples above illustrate promotion on procedures written
by a user.  Yet any procedure is a candidate for promotion in Chapel,
including library routines and operators.  As an example, consider the
following lines of code:

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

Note that Chapel does not support an explicit ``+`` operator for
arrays.  Rather, it implements this operation by promoting the
standard ``+`` operator for the array's element type.  Similarly,
applying ``*`` to arrays promotes the scalar ``*`` operator.  For this
reason, applying ``*`` to Chapel arrays results in an elementwise
multiplication of the arrays' elements by default, rather than a
matrix multiplication operation.

Even the assignment operations in these statements can be considered
to be promotions of scalar assignment for ``real`` values.  Thus, the
statements above can be considered to be equivalent to:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 123-124,130-131


Promotion vs. Whole-Array Operations
------------------------------------

Chapel's promoted operators result in different behavior than you
might expect from a typical array language.  To understand the
difference, let's look at an example:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 136

As expected, this statement doubles each element of *B*, adds the
result to its corresponding value in *A*, and then assigns that result
to the corresponding value in *C*.  However, most array languages
would define the semantics of this statement by evaluating an operator
at a time, as follows:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 139-140

In contrast, Chapel defines it using zippered iteration:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 143-144

In this case, the two approaches compute the same values, but the
Chapel approach has the benefit of avoiding the need for any temporary
arrays to store intermediate array results.  This makes memory
requirements of Chapel programs more explicit to users while also
tending to make better use of memory caches in modern architectures.

For other computations, Chapel's promotion semantics generate a
different result than most array languages would.  For example,
consider the following computation which attempts to replace each
interior element of *V* with the average of its neighbors:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 148-149

Again, in an array language, this computation would typically be
evaluated by applying each operator in turn:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 163-164

However, Chapel's zippered promotion semantics result in the following
interpretation:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 152-153

which is equivalent to:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 156-157

Both of these loops have a race condition, and therefore so does the
original whole-array computation in Chapel.  Specifically, the tasks
used to execute the forall-loop may try to read and write overlapping
values of *V* simultaneously.  Thus, where the author likely intended
for all the original values of *V* to be averaged, in reality one or
more tasks are likely to end up reading the new values that were
computed by their sibling tasks.

For this reason, it is important that users of promotion keep the
underlying zippered interpretation in mind and ensure that they are
not introducing race conditions between the iterations.  For example,
to write the previous computation safely in Chapel using promotion,
users could introduce a temporary array:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 168-169

Alternatively, they could create an explicit array-oriented
implementation of ``+`` to avoid the promotion.  Note that this still
requires a temporary array in which to store and return the result:

.. literalinclude:: examples/users-guide/datapar/promotion.chpl
  :language: chapel
  :lines: 173-178
