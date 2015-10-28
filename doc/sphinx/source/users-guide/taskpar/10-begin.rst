:title: Users Guide: The 'begin' statement

The 'begin' statement
=====================

The simplest way to create a task in Chapel is the 'begin' statement.
It prefixes another statement and indicates that a task should be
created to execute that statement while the original task goes on to
the next statement.  For example, the following program creates a task
to execute the first call to writeln() while the original task goes on
to execute the second call.

.. code-include::../code/10-begin.chpl
  :lexer: chapel

Because these two tasks are not synchronized in any way, their calls
to writeln() could execute in any order.  As a result, their messages
could be printed to the console in either order:

.. code-include::../code/10-begin-alt.good

or:

.. code-include::../code/10-begin.good

Happily, the writeln() routine is written in a parallel-safe manner,
so there is no danger of the two messages getting jumbled up with one
another.

'begin'ing Larger Statements
----------------------------

The statement specifying a task can be a compound statement or a
function call, so can be defined by an arbitrary amount of code.  For
example, the following program creates one task that prints some
messages using a compound statement and a second that prints some
using a procedure call, while the original task prints messages of its
own.

.. code-include:../code/10-beginBiggerStatements.chpl
  :lexer: chapel


Nested Tasks
------------

Since Chapel supports nested parallelism, a task created with 'begin'
may itself create other tasks.  For example, the following routine
uses recursion and 'begin' statements to create a distinct task to
process each node in a binary tree:

.. code-include:../code/10-walkTreeUsingBegins.chpl
  :lexer: chapel
  :start-line: 23
  :end-line: 32

(Whether or not this is an appropriate use of tasks depends on the
size of the tree, the processTasks() computation, the system, etc.)
