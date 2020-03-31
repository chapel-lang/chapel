.. default-domain:: chpl

.. _primers-taskParallel:

Task Parallelism
================

`View taskParallel.chpl on GitHub <https://github.com/chapel-lang/chapel/blob/master/test/release/examples/primers/taskParallel.chpl>`_




This primer illustrates Chapel's parallel tasking features,
namely the ``begin``, ``cobegin``, and ``coforall`` statements.


.. code-block:: chapel

    config const n = 10; // Used for the coforall loop


.. _primers-taskparallel-begin:

Begin Statements
----------------
The ``begin`` statement spawns a thread of execution that is independent
of the current (main) thread of execution.

.. code-block:: chapel

    writeln("1: ### The begin statement ###");

    begin writeln("1: output from spawned task");


The main thread of execution continues on to the next statement.
There is no guarantee as to which statement will execute first.

.. code-block:: chapel

    writeln("1: output from main task");




.. _primers-taskparallel-cobegin:

Cobegin Statements
------------------
For more structured behavior, the ``cobegin`` statement can be used to
spawn a block of tasks, one for each statement.  Control continues
after the ``cobegin`` block, but only after all the tasks within the
``cobegin`` block have completed.

.. code-block:: chapel

    writeln("2: ### The cobegin statement ###");

    cobegin {
      writeln("2: output from spawned task 1");
      writeln("2: output from spawned task 2");
    }


The output from within the ``cobegin`` statement will always precede the
following output from the main thread of execution.

.. code-block:: chapel

    writeln("2: output from main task");



If any ``begin`` statements are used within a ``cobegin`` statement,
the thread of execution does not wait for those ``begin`` statements
to complete.


.. code-block:: chapel

    writeln("3: ### The cobegin statement with nested begin statements ###");

    cobegin {
      begin writeln("3: output from spawned task 1");
      begin writeln("3: output from spawned task 2");
    }


The order of the output is again undefined because the ``begin``
statements in the above ``cobegin`` statement are not guaranteed to
have been executed before control reaches the following statement.

.. code-block:: chapel

    writeln("3: output from main task");




.. _primers-taskparallel-coforall:

Coforall Statements
-------------------
Another more structured form of task parallelism is the ``coforall``
statement.  The ``coforall`` statement is a loop variant of the ``cobegin``
statement where each iteration of the loop is a separate task.
Similar to the ``cobegin`` statement, every iteration of the ``coforall``
loop is a separate task and the main thread of execution does not
continue until every iteration is complete.

.. code-block:: chapel

    writeln("4: ### The coforall statement ###");

    coforall i in 1..n {
      writeln("4: output from spawned task 1 (iteration ", i, ")");
      writeln("4: output from spawned task 2 (iteration ", i, ")");
    }


While the order of output within an iteration is deterministic (``1``
executes before ``2``), the order of output relative to other
iterations is not defined.  As with the ``cobegin`` statement, the output
from within the ``coforall`` statement will always precede the following
output.

.. code-block:: chapel

    writeln("4: output from main task");




As with the ``cobegin`` statement, any ``begin`` statements spawned within
a ``coforall`` loop are not guaranteed to be complete before the main
thread of execution continues.

.. code-block:: chapel

    writeln("5: ### The coforall statement with nested begin statements ###");
    coforall i in 1..n {
      begin writeln("5: output from spawned task 1 (iteration ", i, ")");
      begin writeln("5: output from spawned task 2 (iteration ", i, ")");
    }

The order of output is undefined.

.. code-block:: chapel

    writeln("5: output from main task");
