:title: Users Guide: Zippered Iteration

.. _ug-zip:

Zippered Iteration
==================

Chapel's for-loops (and their parallel counterparts, :ref:`coforall
<ug-coforall>` and forall) can be driven by multiple iterands in a
coordinated manner known as *zippered iteration*.  This is expressed
using the ``zip`` keyword which is followed by a parenthesized list of
the iterands.  

As a simple example, the following loop iterates over an array and a
range simultaneously, referring to the values yielded by each using a
separate index variable:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :caption:
  :language: chapel
  :lines: 1-3

As the name suggests, zippered iteration causes the loop body to be
executed once per corresponding set of values yielded by the iterands.
Thus, in the first iteration of the loop above, *a* will refer to the
first element of *A* and *i* to 1; in the second, *a* will be *A*'s
second element and *i* will be 2; etc.  For this reason, zippered
iterands must typically have the same size (yield the same number of
things), though we'll discuss some exceptions below.

Due to this behavior, printing the array after this loop executes
results in:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 1

Naturally, more than two iterands may be zippered together as well:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 7-8

Where here, the array ends up storing:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 3

Of course, this gets more interesting once we start defining
more general iterators...


Index Variables for Zippered Iteration
--------------------------------------

In the example above, we captured the results yielded by the iterands
using distinct index variables.  A zippered iteration's results can
also be captured using a single index variable which represents a
tuple of the iterands' values:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 12-13

The output of this loop is:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 5-9

In fact, the original loop can be interpreted as de-tupling the
zippered index variable in-place, as is permitted in other declaration
contexts.  Taking this notion further, the following loops show three
ways of decomposing the index variables for a zippered iteration over
a range and an array of 2-tuples:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 16-17, 19-21, 23-25

The output of these loops is as follows:

.. literalinclude:: examples/users-guide/base/zip.good
  :language: text
  :lines: 11-21

As in other de-tupling contexts, this also means that if the
results of one of the iterands is not needed, an underscore can
be used to drop the corresponding value on the floor:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 28-29

And finally, as with non-zippered for-loops, if (for some reason) none
of the iterands' results are needed, the entire clause introducing the
index variables can be omitted:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 32-34


Zippered Iterand Size Matching
------------------------------

As mentioned above, zippered iterands must typically have the same
size and shape.  We've just seen one important exception to this rule
in passing, which is that unbounded ranges can be zipped with other
things in spite of being conceptually infinite.  As an example, the
following loop assigns an array's elements their ordinal number
without needing to know how large the array is a priori:

.. literalinclude:: examples/users-guide/base/zip.chpl
  :language: chapel
  :lines: 37-38

At present, there is no way for users to define their own iterator
functions with a similar unbounded-yet-zipperable property, though we
intend to support this in the future.  Today, unbounded range iterands
can occur any place in a zippered iteration, but to plan for this
future, it is suggested that by convention, they should follow a
bounded iterand (like *A* in the example above).

