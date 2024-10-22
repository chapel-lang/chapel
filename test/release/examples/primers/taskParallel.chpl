// Task Parallelism

//
// This primer illustrates Chapel's task-parallel features,
// namely the ``begin``, ``cobegin``, and ``coforall`` statements.
// In Chapel, a `task` is a computation in the program that can,
// and typically will, run in parallel with respect to other tasks.
// These three statements are the only ways to create new tasks
// within a Chapel program.

/*
.. index::
    single: begin

.. _primers-taskparallel-begin:

Begin Statements
----------------
The ``begin`` statement creates a new parallel task that's independent
of the original one encountering the ``begin``.
*/

writeln("1: ### The begin statement ###");
begin writeln("1: output from spawned task");

// The original task continues on to the next statement.  In this
// example, there is no guarantee which statement will execute first
// since they can run in parallel and are not synchronized.
writeln("1: output from main task");


/*
.. index::
   single: cobegin
.. _primers-taskparallel-cobegin:

Cobegin Statements
------------------
For more structured behavior, the ``cobegin`` statement can be used to
spawn a block of tasks, one for each statement.
*/
writeln("2: ### The cobegin statement ###");

cobegin {
  writeln("2: output from spawned task 1");
  writeln("2: output from spawned task 2");
}

// The original task continues execution after the ``cobegin`` block,
// but only after all the child tasks created by the ``cobegin`` block
// have completed.  As a result, in this example, the output from
// within the ``cobegin`` statement will always precede the following
// output from the original task:

writeln("2: output from main task");


// If any ``begin`` statements are used within a ``cobegin``
// statement's tasks, the original task does not wait for those
// ``begin`` tasks to complete.  That is, the original task only waits
// on the `cobegin`'s child tasks, not all of their descendent tasks.

writeln("3: ### The cobegin statement with nested begin statements ###");

cobegin {
  begin writeln("3: output from spawned task 1");
  begin writeln("3: output from spawned task 2");
}

// The order of the output is again undefined because the ``begin``
// statements in the above ``cobegin`` statement are not guaranteed to
// have been executed before control reaches the following statement.
writeln("3: output from main task");


/*
.. index::
   single: coforall
.. _primers-taskparallel-coforall:

Coforall Loops
--------------
Another structured form of task parallelism is the
``coforall`` loop.  This loop form is like a ``for`` loop, except that
each iteration of the loop is executed by a distinct task.  Similar
to the ``cobegin`` statement, the original task does not
continue until the tasks created for each iteration have completed.
*/
writeln("4: ### The coforall loop ###");

config const n = 10;

coforall i in 1..n {
  writeln("4: output 1 from spawned task ", i);
  writeln("4: output 2 from spawned task ", i);
}

// While the statements within the loop body will execute in the
// normal way (so in this case, the first ``writeln()`` will execute
// before the second), the order of execution relative to other
// iterations is undefined.

// As with the ``cobegin`` statement, the original task will wait
// until the ``coforall``'s child tasks have completed before
// proceeding.  As a result, in this example, all output from within
// the ``coforall`` loop will precede the following output:

writeln("4: output from main task");



// Also like the ``cobegin`` statement, the orignal task will not wait
// for any ``begin`` tasks spawned by its child tasks.

writeln("5: ### The coforall loop with nested begin statements ###");
coforall i in 1..n {
  begin writeln("5: output from spawned task 1 (iteration ", i, ")");
  begin writeln("5: output from spawned task 2 (iteration ", i, ")");
}
// The order of output is undefined.
writeln("5: output from main task");

/*
.. index::
   single: with; task intent
.. _primers-taskparallel-task-intents:

Task Intents
------------

The body of a task construct may refer to some variables declared
outside its lexical scope, known as "outer variables". When it does,
"shadow variables" are introduced. Each task created by the task
construct gets its own set of shadow variables, one per outer variable.

 - Each shadow variable behaves as if it were a formal argument
   of a function that implements the task's work. (These "task
   functions" are described in :ref:`the language spec
   <Chapter-Task_Parallelism_and_Synchronization>`).
   The outer variable is passed to this formal argument according to
   the :ref:`argument intent <Argument_Intents>` associated with
   the shadow variable, which is called a "task intent".

 - References within a task that seem to refer to an outer variable
   will actually be referring to the corresponding shadow variable
   owned by the task.

 - Each shadow variable is deallocated at the end of its task.

For most types, forall intents use the default argument intent
(:ref:`The_Default_Intent`). For numeric types, this implies capturing the
value of the outer variable by the time the task starts executing. Sync and
atomic variables are passed by reference (:ref:`primers-syncs`,
:ref:`primers-atomics`). Arrays infer their default intent based upon the
declaration of the array. Mutable arrays (e.g. declared with ``var`` or passed
by ``ref`` intent) have a default intent of ``ref``, while immutable arrays
(e.g. declared with ``const`` or passed by ``const`` intent) have a default
intent of ``const``. These defaults are described in :ref:`the language spec
<Task_Intents>`.

``begin`` statements currently capture the values of outer variables
of numeric types into their shadow variables at task creation time.
This means that the shadow variables can be accessed even after
the outer variables' scope exits. This is not the case
for some other types such as arrays.
*/
var outerIntVariable = 2;
begin assert(outerIntVariable == 2);

// The task intents ``in``, ``const in``, ``ref``, ``const ref``,
// and ``reduce`` can be specified explicitly using a ``with`` clause.
//
// An ``in`` or ``const in`` intent creates a copy of the outer variable
// for each task. A ``ref`` or ``const ref`` makes the
// shadow variable an alias for the outer variable.
var outerArray = [10, 11, 12];
begin with (in outerArray) assert(outerArray[0] == 10);

var outerRealVariable = 1.0;

coforall i in 1..n with (ref outerRealVariable) {
  if i == 1 then            // ensure only one task updates outerIntVariable
    outerRealVariable *= 2; // to avoid the risk of a data race
}

// A reduce intent can be used to compute reductions with ``coforall`` loops.
// The values of each reduce-intent shadow variable at the end of its task
// is combined onto its outer variable according to the specified reduction
// operation.

 // The values of the outer variables before the loop will be included
 // in the reduction result.
var outerMaxVariable = 0;
var outerMinVariable = 0;

coforall i in 1..n with (+ reduce outerIntVariable,
                         max reduce outerMaxVariable,
                         min reduce outerMinVariable) {
  outerIntVariable = i;

  if i % 2 == 0 then
    outerMaxVariable = i;  // compute the max of even indices
  else
    outerMinVariable = -i; // ... and the min of negated odd ones

  // The loop body can contain other code
  // regardless of reduce-related operations.
}
