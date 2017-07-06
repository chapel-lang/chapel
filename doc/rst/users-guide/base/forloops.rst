:title: Users Guide: **for**-loops

.. _ug-forloops:

**for**-loops: structured serial iteration
==========================================

*For-loops* are Chapel's means of expressing serial iteration as
driven by an *iterand*—an iterator function or iterable expression.
As a simple example, the following code loops over the integers 1
through 5, printing a message per iteration:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :caption:
  :language: chapel
  :lines: 3-4

More specifically, this for-loop invokes the default iterator for the
*range* expression ``1..5``, storing the values that it yields in a
per-iteration variable named *i*.  The body of the loop then refers to
*i* as a means of customizing the messages that are printed.  The
resulting output is:

.. literalinclude:: examples/users-guide/base/forloops.good
  :language: text
  :lines: 1-5

For-loop Index Variables
------------------------

A for-loop's index variable (like *i* above) is a new symbol that the
for-loop defines, permitting its body to refer to the values generated
by its iterand expression.  Thus, the following code actually declares
two variables named *i* where the for-loop's index variable
temporarily shadows the original *i* and has no relation to it:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 7-10

The original variable *i* remains unchanged by the loop, so it retains
its default value once it is no longer shadowed.  Thus, the output for
the program is:

.. literalinclude:: examples/users-guide/base/forloops.good
  :language: text
  :lines: 7-12


A for-loop's index variable may or may not be mutable, depending on
the expression that drives the loop.  For example, range iterators
yield ``const`` integer values, making it illegal to modify *i* within
the loop body:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 14-15
  :dedent: 2

In contrast, an array's default iterator yields references to the
array's elements.  As a result, the loop's index variable can be used
as a means of modifying the array's values:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 17-19

Thus, printing out the array after this loop terminates would reflect
the updated values:

.. literalinclude:: examples/users-guide/base/forloops.good
  :language: text
  :lines: 14

If the values yielded by the iterand are not required by the loop
body, the index variable and ``in`` keyword can be omitted altogether.
For example, the following loop iterates five times, but since the
index variable is not required by the body, it has been omitted:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 23-25


For-loop Bodies
---------------

If the body of a for-loop is a single statement, as in all of the
examples above, the ``do`` keyword can be used after the iterand to
specify the loop body.  Multi-statement loop bodies can be defined
using curly brackets:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 28-31

Of course, since a compound statement is a single statement itself,
this also means that the two styles can be combined (though with no
real benefit):

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 35-38


Additional For-loop Capabilities
--------------------------------

For loops can be used to iterate over multiple expressions in a
coordinated or *zippered* manner, described on the next page.
Subsequent pages will also describe how to write iterator functions.
Note that it is also possible to exit early from a for-loop via the
``break`` statement, covered later.

.. TODO: add links for these things as they come on-line
