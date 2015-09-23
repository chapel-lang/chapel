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

    reduce-operator: one of
       // these have the same meaning as in a reduction expression
       +  *  &&  ||  &  |  ^


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

* Provide reduce intents as task intents.

* Provide the other predefined reduction operators as reduce intents:

  .. code-block:: chapel

    min max minloc maxloc

* Allow user-defined reductions to be used with reduce intents.

* Implement reduction expressions using forall loops and reduce intents.
