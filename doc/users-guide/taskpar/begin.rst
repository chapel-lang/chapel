:title: Users Guide: **begin** Statements

**begin** Statements: unstructured tasking
==========================================

The simplest way to create a task in Chapel is with the **begin**
statement.  It prefixes another statement and indicates that a task
should be created to execute the statement while the original task
goes on to the statement following the **begin**.

As an example, the following program creates a task to execute the
call to ``writeln()`` in the first statement while the original task
goes on to execute the second call to ``writeln()``.

.. literalinclude:: examples/users-guide/taskpar/begin.chpl
  :caption:
  :language: chapel

Because these two tasks are not synchronized in any way, the order in
which the messages appear on the console is not guaranteed.  As a
result, when running this program, the console could display the
messages either like this:

.. literalinclude:: examples/users-guide/taskpar/begin-alt.good

or this:

.. literalinclude:: examples/users-guide/taskpar/begin.good

Happily, the ``writeln()`` routine is written in a parallel-safe
manner, so there is no danger of the characters from the two strings
getting mixed up with each other even though the tasks are executing
in parallel.


**begin**-ing More Complex Statements
-------------------------------------

The statement defining a task can be a compound statement or a
function call, so can specify an arbitrary amount of code for the task
to run.  For example, the following program creates one task that
prints some messages using a compound statement and a second that
prints other messages using a function call, while the original task
prints messages of its own.

.. literalinclude:: examples/users-guide/taskpar/beginBiggerStatements.chpl
  :caption:
  :language: chapel


Nested Tasks
------------

Since Chapel supports nested parallelism, a task created with **begin**
may itself create other tasks.  For example, the following routine
uses recursion and **begin** statements to create a distinct task to
process each node in a binary tree:

.. literalinclude:: examples/users-guide/taskpar/walkTreeUsingBegins.chpl
  :caption:
  :language: chapel
  :lines: 24-31

(Whether or not this is an intelligent use of tasks depends on the
size of the tree, the ``processTasks()`` computation, what else the
program is doing, the system, etc.)
