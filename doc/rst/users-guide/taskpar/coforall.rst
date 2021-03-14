:title: Users Guide: **coforall**-loops

.. _ug-coforall:

**coforall**-loops: loop-based tasking
======================================

The **coforall statement** can be used to create an arbitrary number
of related, homogeneous tasks.  Syntactically, it mirrors the for-loop
statement, but uses the ``coforall`` keyword in place of ``for``.

Operationally, a coforall loop creates a distinct task per loop
iteration, each of which executes a copy of the loop body.
Mnemonically, the coforall loop can be thought of as a *concurrent
forall*—that is, a parallel loop in which each iteration is a
concurrent task.

As with the :ref:`cobegin statement <ug-cobegin>`, the original task
does not proceed until the child tasks corresponding to the coforall's
iterations have completed.  And, as with ``cobegin``, the original
task waits only for its immediate children, not their descendents.

The following code illustrates a simple use of the coforall loop:

.. literalinclude:: examples/users-guide/taskpar/coforall.chpl
  :caption:
  :language: chapel

This program will create a number of tasks equal to the configuration
constant *numTasks*.  Each task executes the loop body, printing a
hello message indicating the value of its unique, private copy of the
loop index variable *tid* (think "task ID") and the total number of
tasks.  As in previous examples, since the tasks are not coordinating
with one another, their "Hello" messages will print out in an
arbitrary order.  However, the "Signing off..." message will not print
until all the "Hello" messages have, since it will be executed by the
original task only once the per-iteration tasks are done.  Thus, the
following shows a possible output of the test:

.. literalinclude:: examples/users-guide/taskpar/coforall-alt.good
  :language: text
