.. _readme-reduceIntents:

==============
Reduce Intents
==============

Note: this is work in progress and is subject to change.


--------
Overview
--------

Reduce intents are a kind of forall intent - see Section 25.3
"Forall Intents" of the Chapel Language Specification.

As with any forall intent, a reduce intent can be specified on any
*outer variable* - that is, a variable used within the body of a
forall loop and declared outside that loop.  References to such a
variable within the loop implicitly refer to the corresponding formal
argument of the task function or the leader iterator.

Reduce intents are distinct:

* Each task formal that corresponds to an outer variable with a reduce
  intent is initialized, at the beginning of its task, to the identity
  value for the corresponding reduction.

* The value of the outer variable immediately after the forall loop is a
  reduction of the values of the corresponding formals at the end of
  their tasks.

Note that the value of the outer variable immediately before the forall loop
is discarded.

Reduce intents are also available with coforall statements.


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

    reduce-operator: one of
       // these have the same meaning as in a reduction expression
       +  *  &&  ||  &  |  ^  min  max

    reduce-class:
       // the name of the class that implements a user-defined reduction
       identifier


--------
Examples
--------

Increment ``x`` in the loop -- counts the number of iterations:

 .. code-block:: chapel

  var x = 5;
  forall myIterator() with (+ reduce x) {
    x += 1;
  }
  writeln("The number of loop iterations is: ", x);

Set ``x`` in the loop -- counts the number of tasks:

 .. code-block:: chapel

  var x = 5;
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

    /* task-private accumulator state */
    var value: eltType;

    /* identity w.r.t. the reduction operation */
    proc identity         return 0: eltType;

    /* accumulate a single element onto the accumulator */
    proc accumulate(elm)  { value = value + elm; }

    /* accumulate a single element onto the state */
    proc accumulateOntoState(ref state, elm)  { state = state + elm; }

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
Open Issues
-----------

* Should reduce-intent variables within the loop body
  be task-private or iteration-private?
  I.e. should the variable's value that is reduced into the final result
  be taken at the end of each task or at the end of each loop iteration?

  The current implementation and the above examples provide the former.
  Both above examples would report the number of iterations if the latter.

* How to support reductions where the type of the result is different
  from the type of the values being reduced, e.g. for a ``min-k`` reduction?

* Should the initial value of the reduction variable participate
  in the reduction as well?

* How would we support reductions over nested forall loops, e.g.:

  .. code-block:: chapel

   var global = 0;

   forall i in iterX() with (+ reduce global) do
     forall j in iterY() with (+ reduce global) do
        global += kernel(i,j);

   writeln("result = ", global);

  The current implementation would exclude, from the final result,
  the values of ``kernel(i,j)`` for most ``i``. Indeed, assume that each task
  of the outer forall executes several inner forall loops.
  The reduction result of the inner loop will be stored into
  the outer task's ``global`` formal. Since the value of that formal
  is discarded when entering the inner loop, only the result
  from the last inner loop within the outer task will be retained
  and reduced into the outer ``global``.


-----------
Future Work
-----------

* Provide reduce intents as task intents for cobegin statements.

* Provide the other predefined reduction operators as reduce intents:

  .. code-block:: chapel

    minloc maxloc
