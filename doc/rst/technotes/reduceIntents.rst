.. _readme-reduceIntents:

==============
Reduce Intents
==============

Note: this is work in progress and is subject to change.


--------
Overview
--------

Reduce intents are a kind of forall intent - see Section 26.3
"Forall Intents" of the Chapel Language Specification.

As with any forall intent, a reduce intent can be specified on any
*outer variable* - that is, a variable used within the body of a
forall loop and declared outside that loop.  References to such a
variable within the loop implicitly refer to the corresponding formal
argument of the task function created by the parallel iterator.
If/when the parallel iterator executes a yield outside any parallel
constructs, the reference is implicitly to the corresponding formal
argument of the parallel iterator itself. In both cases, these formals
are added implicitly by the compiler.

Reduce intents are distinct:

* The references, within the loop body, to a reduce-intented outer variable
  implicitly refer to the reduction state corresponding to the task
  function's formal. At the beginning of the task, this reduction state
  is initialized to the identity value for the corresponding reduction.

* The value of the outer variable immediately after the forall loop is a
  reduction of the values of the corresponding formals at the end of
  their tasks and the value of the outer variable immediately before
  the forall/coforall loop.

Reduce intents are currently available for forall and coforall statements.


------
Syntax
------

The syntax of ``task-intent-list`` is extended to allow ``reduce-intent``:

  ::

    task-intent-list:
      // no change with these
      formal-intent identifier
      formal-intent identifier, task-intent-list
      // added for reduce intents:
      reduce-intent
      reduce-intent, task-intent-list

    reduce-intent:
      reduce-operator 'reduce' identifier
      reduce-class    'reduce' identifier
      reduce-expr     'reduce' identifier

    reduce-operator: one of
       // these have the same meaning as in a reduction expression
       +  *  &&  ||  &  |  ^  min  max

    reduce-class:
       // the name of the class that implements a user-defined reduction
       identifier

    reduce-expr:
       // an expression producing an instance of a user-defined reduction class
       expr


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
    proc combine(other)   { value = value + other.value; }

    /* Convert the accumulation into the value of the reduction
       that is reported to the user. This is trivial in our case. */
    proc generate()       return value;

    /* produce a new instance of this class */
    proc clone()          return new PlusReduceOp(eltType=eltType);
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

* Implement reduce intents for cobegin statements.

* Provide the other predefined reduction operators as reduce intents:

  .. code-block:: chapel

    minloc maxloc

* We are working on a new interface for user-defined reductions,
  addressing the need for user-defined synchronization choices
  and the ability to provide reduction state without the overhead
  of synchronization support for partial reductions.

* We are also considering replacing classes with records for user-defined
  reductions. The goal is to eliminate the required malloc+free,
  which is possible because the lifetime of a reduction class instance
  matches the forall or coforall statement.
