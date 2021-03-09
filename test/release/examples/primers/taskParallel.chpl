// Task Parallelism

//
// This primer illustrates Chapel's parallel tasking features,
// namely the ``begin``, ``cobegin``, and ``coforall`` statements.

config const n = 10; // Used for the coforall loop

// .. _primers-taskparallel-begin:
//
// Begin Statements
// ----------------
// The ``begin`` statement spawns a thread of execution that is independent
// of the current (main) thread of execution.
writeln("1: ### The begin statement ###");

begin writeln("1: output from spawned task");

// The main thread of execution continues on to the next statement.
// There is no guarantee as to which statement will execute first.
writeln("1: output from main task");



// .. _primers-taskparallel-cobegin:
//
// Cobegin Statements
// ------------------
// For more structured behavior, the ``cobegin`` statement can be used to
// spawn a block of tasks, one for each statement.  Control continues
// after the ``cobegin`` block, but only after all the tasks within the
// ``cobegin`` block have completed.
writeln("2: ### The cobegin statement ###");

cobegin {
  writeln("2: output from spawned task 1");
  writeln("2: output from spawned task 2");
}

// The output from within the ``cobegin`` statement will always precede the
// following output from the main thread of execution.
writeln("2: output from main task");


// If any ``begin`` statements are used within a ``cobegin`` statement,
// the thread of execution does not wait for those ``begin`` statements
// to complete.

writeln("3: ### The cobegin statement with nested begin statements ###");

cobegin {
  begin writeln("3: output from spawned task 1");
  begin writeln("3: output from spawned task 2");
}

// The order of the output is again undefined because the ``begin``
// statements in the above ``cobegin`` statement are not guaranteed to
// have been executed before control reaches the following statement.
writeln("3: output from main task");



// .. _primers-taskparallel-coforall:
//
// Coforall Statements
// -------------------
// Another more structured form of task parallelism is the ``coforall``
// statement.  The ``coforall`` statement is a loop variant of the ``cobegin``
// statement where each iteration of the loop is a separate task.
// Similar to the ``cobegin`` statement, every iteration of the ``coforall``
// loop is a separate task and the main thread of execution does not
// continue until every iteration is complete.
writeln("4: ### The coforall statement ###");

coforall i in 1..n {
  writeln("4: output from spawned task 1 (iteration ", i, ")");
  writeln("4: output from spawned task 2 (iteration ", i, ")");
}

// While the order of output within an iteration is deterministic (``1``
// executes before ``2``), the order of output relative to other
// iterations is not defined.  As with the ``cobegin`` statement, the output
// from within the ``coforall`` statement will always precede the following
// output.
writeln("4: output from main task");



// As with the ``cobegin`` statement, any ``begin`` statements spawned within
// a ``coforall`` loop are not guaranteed to be complete before the main
// thread of execution continues.
writeln("5: ### The coforall statement with nested begin statements ###");
coforall i in 1..n {
  begin writeln("5: output from spawned task 1 (iteration ", i, ")");
  begin writeln("5: output from spawned task 2 (iteration ", i, ")");
}
// The order of output is undefined.
writeln("5: output from main task");

/*
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

The default argument intent (:ref:`The_Default_Intent`) is used by default.
For numeric types, this implies capturing the value of the outer
variable by the time the task starts executing. Arrays are passed by
reference, as are sync, single, and atomic variables
(:ref:`primers-syncsingle`, :ref:`primers-atomics`).
For ``begin`` statements, for example, this means that the captured
value of an outer numeric variable can be accessed even after its
scope exits, while an outer array variable cannot.
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
  outerIntVariable += i;

  if i % 2 == 0 then
    outerMaxVariable = i;  // compute the max of even indices
  else
    outerMinVariable = -i; // ... and the min of negated odd ones

  // The loop body can contain other code
  // regardless of reduce-related operations.
}
