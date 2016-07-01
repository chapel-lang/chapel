:title: Users Guide: Zippered Iteration

.. _ug-zip:

Zippered Iteration
==================

Chapel's :ref:`for-loops <ug-forloops>` (and their parallel
counterparts, :ref:`coforall <ug-coforall>` and forall) can be driven
by multiple iterands in a coordinated manner known as *zippered
iteration*.  This is expressed using the ``zip`` keyword, followed by
a parenthesized list of the iterands.

As a simple example, the following loop iterates over an array and a
range simultaneously, referring to the values yielded by each using
distinct index variables, *a* and *i*:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :caption:
  :language: chapel
  :lines: 1-3

As the name suggests, zippered iteration causes the loop body to be
executed once per corresponding set of values yielded by the iterands.
Thus, in the first iteration of the loop above, *a* will refer to the
first element of *A* and *i* to 1; in the second, *a* refers to *A*'s
second element and *i* to 2; etc.  For this reason, zippered iterands
must typically have the same "size" (yield the same number of things),
though we'll discuss exceptional cases below.

As a result of this behavior, printing the array after this loop has
executed will result in:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 1

Naturally, zippering may involve more than two iterands as well:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 7-8

Here, the array ends up as:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 3



Index Variables for Zippered Iteration
--------------------------------------

In the example above, we captured the results of the zippering using
an index variable per iterand.  Zippered iterations can also be
represented using a single index variable storing a tuple of the
iterands' values:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 12-13

The output of this loop is:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 5-9

In fact, the original case can be thought of as de-tupling the
zippered index variable in-place, as is permitted in other declaration
contexts.  Taking this notion further, the following loops show three
ways of decomposing the index variable for a zippered iteration over a
range and an array of 2-tuples:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 16-17, 19-21, 23-25

The output of these loops is as follows:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 11-21

As in other de-tupling contexts, this also means that if the results
of one of the iterands is not needed, an underscore can be used to
drop its values on the floor:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 28-29

And finally, as with non-zippered for-loops, if none of the iterands'
results are needed, the entire clause introducing the index variables
can be omitted:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 32-34


Zippered Iterand Size Matching
------------------------------

As mentioned above, zippered iterands must typically have the same
size, which is to say they should yield the same number of things.
Just above, we saw an important exception to this rule, which is that
unbounded ranges can be zipped with other things in spite of being
conceptually infinite.  

As a second example, the following loop assigns an array's elements
their ordinal values without needing to know the array's size *a
priori*:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 37-38

At present, there is unfortunately no way for users to define their
own iterator functions in a way that permits them to be flexible with
respect to size like unbounded ranges are.  However, this is planned
for the future to support idioms like consuming *n* values from a
random sequence or a data stream in a zippered context.  In the
meantime, a typical workaround is to write such iterators to take the
number of items to generate as an argument.

Related: Today, unbounded range iterands can occur anywhere in a
zippered iterand list.  However, in anticipating future support for
zippering unbounded user iterators, it is recommended that users
follow the convention of having unbounded range iterands follow a
bounded iterand in zippered contexts (as ``1..`` follows *A* in the
examples above).
