:title: Users Guide: **cobegin** Statements

.. _ug-cobegin:

**cobegin** Statements: creating groups of tasks
================================================

The **cobegin statement** creates a fixed number of related, yet
potentially heterogeneous, tasks.  It is expressed by prefixing a
compound statement with the ``cobegin`` keyword, as illustrated by the
following example:

.. literalinclude:: examples/users-guide/taskpar/cobegin.chpl
  :caption:
  :language: chapel

At execution time, a distinct task is created for each child statement
within the cobegin statement.  Thus, in the example above, three tasks
will be created, corresponding to the three statements immediately
within the cobegin's block statement: one for the standalone
writeln(), one for the call to ``sayHi()``, and one for the compound
statement.  Since these tasks are likely to execute in parallel and
don't coordinate with one another, the ordering of their messages
relative to one another is arbitrary.  Note that the two statements
within the compound statement will always be printed in the order
shown since they are executed by a single task.

Cobegin statements also specify that the original task which
encounters it cannot proceed until its child tasks have completed.  In
the example above, this means that "See you later!"  will not be
printed until all of the greetings have been.

As a result of these rules, possible outputs for the code above
include:

.. literalinclude:: examples/users-guide/taskpar/cobegin.good
  :language: text

or

.. literalinclude:: examples/users-guide/taskpar/cobegin-alt.good
  :language: text

but never:

.. literalinclude:: examples/users-guide/taskpar/cobegin-bad.txt
  :language: text

(because this would imply that the third task did not run serially and
that the original task did not wait for its children to complete
before going on).


Child vs. Descendent Tasks
--------------------------

It's worth noting that cobegin statements only wait for their
immediate child tasks to complete before proceeding, ignoring any
further descendent tasks.  Thus, if a cobegin's task creates an
additional task using a ``begin`` statement, that new task executes
asynchronously and has no bearing on the cobegin's completion
semantics.

As an example, consider the following program:

.. literalinclude:: examples/users-guide/taskpar/cobegin+begin.chpl
  :caption:
  :language: chapel

Here, it's possible for the "Sorry, I've gotta leave..." message to be
printed before all of the "Hello!" messages, since they are written by
an asynchronous task.  However, since the original child task printed
the "Beginning..." and "I'm moving on..." messages, those are
guaranteed to have executed and in that order.  For these reasons, the
following would be a possible output:

.. literalinclude:: examples/users-guide/taskpar/cobegin+begin-alt.good
  :language: text

However, the following output could never occur:

.. literalinclude:: examples/users-guide/taskpar/cobegin+begin-bad.txt
  :language: text

First, because it's not possible for "Hello!" messages to print before
the "Beginning..." message since the ``begin`` will not have been
encountered yet.  Second, because the "Sorry" message can't print
prior to "I'm moving on..." since it has to wait for its immediate
children to finish.
