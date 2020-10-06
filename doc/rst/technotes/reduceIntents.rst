.. _readme-reduceIntents:

==============
Reduce Intents
==============

Note: this is work in progress and is subject to change.


--------
Overview
--------

Reduce intents are defined as :ref:`a forall intent <Forall_Intents>`
for forall loops and :ref:`a task intent <Task_Intents>` for
task-parallel constructs.

Reduce intents are distinct from other forall/task intents:

* The references, within the loop body, to a reduce-intented outer variable
  implicitly refer to the reduction state corresponding to the task
  function's formal. At the beginning of the task, this reduction state
  is initialized to the identity value for the corresponding reduction.

* The value of the outer variable immediately after the forall loop is a
  reduction of the values of the corresponding formals at the end of
  their tasks and the value of the outer variable immediately before
  the forall/coforall loop.

Reduce intents are currently available for forall and coforall statements.


--------
Examples
--------

Increment ``x`` in the loop -- counts the number of iterations:

 .. code-block:: chapel

  var x = 0;
  forall myIterator() with (+ reduce x) {
    x += 1;
  }
  writeln("The number of loop iterations is: ", x);

Set ``x`` in the loop -- counts the number of tasks:

 .. code-block:: chapel

  var x = 0;
  forall myIterator() with (+ reduce x) {
    x = 1;
  }
  writeln("The number of tasks is: ", x);

For a user-defined reduction, there is a task-private instance
of the reduction class for each task created for the forall
or coforall loop. Here is an example of such a class:

 .. code-block:: chapel

  /* Implements + reduction over numeric data. */
  class PlusReduceOp: ReduceScanOp {

    /* the type of the elements to be reduced */
    type eltType;

    /* task-private accumulator/reduction state */
    var value: eltType;

    /* identity w.r.t. the reduction operation */
    proc identity         return 0: eltType;

    /* accumulate a single element onto the accumulator */
    proc accumulate(elm)  { value = value + elm; }

    /* accumulate a single element onto the state */
    proc accumulateOntoState(ref state, elm)  { state = state + elm; }

    /* accumulate the value of the outer variable at the entry to the loop */
    // Note: this method is optional. If it is not provided,
    // accumulate(outerVar) is used instead.
    proc initialAccumulate(outerVar) { value = value + outerVar: eltType; }

    // Note: 'this' can be accessed by multiple calls to combine()
    // concurrently. The Chapel implementation serializes such calls
    // with a lock on 'this'.
    // 'other' will not be accessed concurrently.
    /* combine the accumulations in 'this' and 'other' */
    proc combine(other: borrowed PlusReduceOp)   { value = value + other.value; }

    /* Convert the accumulation into the value of the reduction
       that is reported to the user. This is trivial in our case. */
    proc generate()       return value;

    /* produce a new instance of this class */
    proc clone()          return new unmanaged PlusReduceOp(eltType=eltType);
  }


  // Use the above class.
  var A = [1000, 200, 30, 4];
  var sum: int;
  forall elm in A with (PlusReduceOp reduce sum) {
    sum += elm;  // equivalently:  sum reduce= elm;
  }
  writeln(sum);

  // To have different input/accumulator/result types of the reduction,
  // specify the input type explicitly, e.g. PlusReduceOp(int) below:
  var A = [false, false, true, false, true];
  var sum: real;
  forall elm in A with (PlusReduceOp(int) reduce sum) {
    sum reduce= elm;   // bools are implicitly coerced to 'int' input type
    writeln(sum);      // accumulation state: int
  }
  writeln(sum);        // result: real


-----------
Future Work
-----------

* Switch to a light-weight interface for user-defined reductions.
  The current proposal is discussed in
  `GitHub issue 9879 <https://github.com/chapel-lang/chapel/issues/9879>`_.

* Implement ``reduce=`` for task intents.

* Implement reduce intents for cobegin statements.
  Consider reduce intents for begin statements.
