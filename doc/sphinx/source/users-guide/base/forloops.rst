:title: Users Guide: For loops

.. _ug-forloops:

For-loops
=========

*For-loops* are Chapel's means of expressing serial iteration as
driven by an iterator function or iterable expression.  As a simple
example, the following for-loop loops over the integers 1 through 5,
printing a message per iteration:

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

Note that for-loop index variables, like *i* above, are new symbols
that are defined by the for-loop and only available within its body.
Thus, the following code actually declares two variables named *i*
where the for-loop's index variable temporarily shadows the original
*i*:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 7-10

Meanwhile, the original *i* is unchanged and so retains its default
value once it comes back into scope.  Thus, the output for the program
is:

.. literalinclude:: examples/users-guide/base/forloops.good
  :language: text
  :lines: 7-12



A for-loop's index variable may or may not be modifiable, depending on
the expression that drives the loop.  For example, range iterators
yield ``const`` integer values, making it illegal to modify *i* within
the loop body:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 14-15

In contrast, the default iterator for an array yields references to
the array's elements.  As a result, the loop's index variable can be
used to modify the array:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 17-19

As a result, printing out the array after the loop terminates reflects
the updated values:

.. literalinclude:: examples/users-guide/base/forloops.good
  :language: text
  :lines: 14

If the yielded values are not required within the loop, the index
variable and ``in`` keyword can be omitted altogether.  For example,
the following loop iterates five times but since the index is not used
by the body, it does not need to be named:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 23-25


For-loop Bodies
---------------

If the body of a for-loop is a single statement, as in all of the
examples above, the ``do`` keyword can be used after the iterand to
specify the loop body.  For multi-statement loop bodies, curly
brackets are used:

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 28-31

Of course, since a compound statement is itself a single statement,
this also means that the two styles can be combined (though with no
benefit):

.. literalinclude:: examples/users-guide/base/forloops.chpl
  :language: chapel
  :lines: 35-38


Additional For-loop Capabilities
--------------------------------

For loops can be iterate over multiple iterands in a *zippered*
manner, described on the next page.  Note that it is also possible to
exit early from for loops via the ``break`` statement, covered in a
later section.

