// Forall Loops

/*
This primer illustrates forall loops, which are a way to leverage
data parallelism or engage :ref:`user-defined parallel iterators
<primers-parIters>`. 

Like serial for-loops, forall loops can iterate over a data structure,
an iterator, or a zippered combination of these. Unlike for-loops,
multiple iterations of a forall loop can potentially execute in parallel.
Parallelism is determined by the data structure or iterator being iterated
over.

Chapel has forall statements and forall expressions. Each form has
two varieties: "must-parallel" and "may-parallel".

 - The must-parallel forms are written using the ``forall`` keyword.
   They require that the iterable provide a parallel iterator.
   Note that there are no requirements on the behavior of the parallel
   iterator. For example, it can execute serially, in which case
   the "must-parallel" loop that invokes it also executes serially.

 - The may-parallel forms are written using brackets ``[ ]``
   (:ref:`primers-forallLoops-may-parallel`).
   They invoke the parallel iterator if the iterable provides it,
   and otherwise will fall back on the serial iterator.

As with for-loops, the body of a forall statement is a statement or a
a block statement, whereas the body of a forall expression is an expression.
Both kinds are shown in the following sections.

"Must-parallel" forall statement
--------------------------------

In the following example, the forall loop iterates over the array indices
in parallel:
*/

config const n = 5;
var A: [1..n] real;

forall i in 1..n {
  A[i] = i;
}

writeln("After setting up, A is:");
writeln(A);
writeln();

/*
If ``A`` were a distributed array (:ref:`primers-distributions`),
each loop iteration would typically be executed on the locale where
the corresponding array element resides.

"Must-parallel" forall expression
---------------------------------

The following forall expression produces new values in parallel.
We store these values in a new array.
*/

var B = forall a in A do a * 3;

writeln("After initialization, B is:");
writeln(B);
writeln();

/*
.. _primers-forallLoops-must-parallel-zippered:

Zippered "must-parallel" forall statement
-----------------------------------------

Forall loops support zippered iteration over multiple iterables
similarly to serial for-loops. For a zippered forall loop, parallelism
is determined by the "leader" iterable, which is the first data structure
or iterator in the zippered list.

A zippered "must-parallel" forall loop requires that the leader iterable
provide a "leader" iterator and all iterables provide "follower" iterators.
These are described in the :ref:`parallel iterators primer
<primers-parIters-leader-follower>`.

Here we illustrate zippering arrays and domains:
*/

var C: [1..n] real;
forall (a, b, i) in zip(A, B, C.domain) do
  C[i] = a * 10 + b / 10 + i * 0.001;

writeln("After a zippered loop, C is:");
writeln(C);
writeln();

/*
The leader iterable in this example is ``A``. Since this array is not
distributed, all loop iterations will be executed on the current locale.

.. _primers-forallLoops-may-parallel:

"May-parallel" forall statement
-------------------------------

The iterator ``onlySerial`` defined below does not have any parallel
forms. Since ``[i in onlySerial(n)]`` is a may-parallel loop, it
will accept the iterator, executing its iterations serially:
*/

iter onlySerial(m: int) {
  for j in 1..m do
    yield j;
}

[i in onlySerial(n)] {
  writeln("in onlySerial iteration #", i);
}
writeln();

/*
If the user had supplied a parallel overload of the onlySerial() iterator,
the above loop would invoke it instead.

Using the following must-parallel loop would cause an error
if onlySerial() does not have any parallel overloads:

  .. code-block:: chapel

      forall i in onlySerial(n) { // error: a parallel iterator is not found
        writeln("in iteration #", i);
      }

"May-parallel" forall expression
--------------------------------

Given that these are default rectangular arrays and therefore
provide parallel iterators, the following may-parallel expression
will be computed in parallel:
*/

var D = [(a,b,c) in zip(A,B,C)] a + c - b;

writeln("The result of may-parallel expression, D is:");
writeln(D);
writeln();

/*
As with must-parallel zippered loops, here A is the leader iterable
(:ref:`primers-forallLoops-must-parallel-zippered`).
Its parallel iterator will determine how this loop is parallelized.
if A were a distributed array, its leader iterator, if defined,
would also determine iteration locality.

Domains declared without a ``dmapped`` clause, including
default rectangular and default associative domains, as well as
arrays over such domains, provide both serial and parallel
iterators. So do domains distributed over standard distributions,
such as Block and Cyclic (:ref:`primers-distributions`), and
arrays over such domains. The parallel iterators provided
by standard distributions place each loop iteration on the
locale where the corresponding index or array element is placed.

Task Intents and Shadow Variables
---------------------------------

A forall loop may refer to some variables declared outside the loop,
known as "outer variables". If so, "shadow variables" are introduced.
Each task created by the parallel iterator gets its own set of
shadow variables, one per outer variable.

 - Each shadow variable behaves as if it were a formal argument
   of the task function for the task. (Task functions are described in
   :ref:`the language spec <Chapter-Task_Parallelism_and_Synchronization>`).
   The outer variable is passed to this formal argument according to
   the :ref:`argument intent <Argument_Intents>`  associated with
   the shadow variable, which is called a "task intent".

 - The name of an outer variable within the lexical scope of the loop
   body refers implicitly to the corresponding shadow variable. If the
   parallel iterator causes multiple iterations of the loop to be
   executed by the same task, these iterations refer to the same set
   of shadow variables.

 - Each shadow variable is deallocated at the end of its task.

The default argument intent (:ref:`The_Default_Intent`) is used by default.
For numeric types, this implies capturing the value of the outer
variable by the time the task starts executing. Arrays are passed by
reference, and so are sync, single, and atomic variables
(:ref:`primers-syncsingle`, :ref:`primers-atomics`).

*/

var outerIntVariable = 0;
proc updateOuterVariable() {
  outerIntVariable += 1;  // always refers to the outer variable
}
var outerAtomicVariable: atomic int;

forall i in 1..n {

  D[i] += 0.5; // if multiple iterations of the loop update the same
               // array element, it could lead to a data race

  outerAtomicVariable.add(1);  // ok: concurrent updates are atomic

  if i == 1 then           // ensure only one task updates outerIntVariable
    updateOuterVariable(); // to avoid the risk of a data race

  // the shadow variable always contains the value as of loop start
  writeln("shadow outerIntVariable is: ", outerIntVariable);
}

writeln();
writeln("After a loop with default intents, D is:");
writeln(D);
 // This variable is updated exactly once, so its value is 1.
writeln("outerIntVariable is: ", outerIntVariable);
 // This variable is incremented atomically n times, so its value is n.
writeln("outerAtomicVariable is: ", outerAtomicVariable.read());
writeln();

/*
The task intents ``in``, ``const in``, ``ref``, ``const ref``,
and ``reduce`` can be specified explicitly using a ``with`` clause.

An ``in`` or ``const in`` intent creates a copy of the outer variable
for each task. A ``ref`` or ``const ref`` makes the
shadow variable an alias for the outer variable. Updates to a ``ref``
shadow variable are reflected in the corresponding outer variable.
*/

var outerRealVariable = 1.0;

forall i in 1..n with (in outerIntVariable,
                       ref outerRealVariable) {
  outerIntVariable += 1;    // a per-task copy, never accessed concurrently

  if i == 1 then            // ensure only one task accesses outerIntVariable
    outerRealVariable *= 2; // to avoid the risk of a data race
}

writeln("After a loop with explicit intents:");
 // This outer variable's value is unaffected by the loop
 // because its shadow variables have the 'in' intent.
writeln("outerIntVariable is: ", outerIntVariable);
writeln("outerRealVariable is: ", outerRealVariable);
writeln();

/*
A reduce intent can be used to compute reductions.
The value of each reduce-intent shadow variable at the end of its task
is combined into its outer variable according to the specified reduction
operation. Within loop body, the shadow variable represents the
accumulation state produced by this task so far, starting from
the reduction identity value at task startup. Values can be
combined onto this accumulation state using the reduction-specific
operation or the ``reduce=`` operator.
*/

 // The values of the outer variables before the loop will be included
 // in the reduction result.
writeln("outerIntVariable before the loop is: ", outerIntVariable);
var outerMaxVariable = 0;

forall i in 1..n with (+ reduce outerIntVariable,
                       max reduce outerMaxVariable) {
  outerIntVariable += i;
  if i % 2 == 0 then
    outerMaxVariable reduce= i;

  // The loop body can contain other code
  // regardless of reduce-related operations.
}

writeln("After a loop with reduce intents:");
writeln("outerIntVariable = ", outerIntVariable);
writeln("outerMaxVariable = ", outerMaxVariable);
writeln();

/*
A with-clause can be used in a similar fashion with any flavor
of forall loop.

Task-Private Variables
----------------------

A task-private variable is similar to an in-intent or ref-intent
shadow variable in that it is initialized at the beginning of its
task and deallocated at the end of the task. However, a task-private
variable is initialized without regard to any outer variable.

A task-private variable is introduced using a with-clause
in a way similar to a regular ``var``, ``const``, ``ref``,
or ``const ref`` variable. A ``var`` or ``const`` variable
must provide either its type or initializing expression, or both.
As with a regular variable, it will be initialized
to the default value of its type if the initializing expression
is not given. A ``ref`` or ``const ref`` variable must
have the initializing expression and cannot declare its type.

A ``var`` task-private variable could be used, for example,
as a per-task scratch space that is never accessed concurrently.
*/

forall i in 1..n with (var myReal: real,  // starts at 0 for each task
                       ref outerIntVariable, // a shadow variable
                       ref myRef = outerIntVariable) {

  myReal += 0.1;   // ok: never accessed concurrently

  if i == 1 then   // ensure only one task accesses outerIntVariable
    myRef *= 3;    // to avoid the risk of a data race
}

writeln("After a loop with task-private variables:");
 // outerIntVariable was updated through the task-private reference 'myRef'
writeln("outerIntVariable is: ", outerIntVariable);
writeln();

/*
Task Intents Inside Record Methods
----------------------------------

When the forall loop occurs inside a method on a record,
the fields of the receiver record are represented in the loop body
with shadow variables as if they were outer variables.
This, for example, allows the forall loop body to update
record fields of array types.

At present, the record fields, as well as the method receiver ``this``,
are always passed by default intent and cannot be listed in a with-clause.
*/

record MyRecord {
  var arrField: [1..n] int;
  var intField: int;
}

proc MyRecord.myMethod() {
  forall i in 1..n {
    arrField[i] = i * 2;  // beware of potential for data races
    // intField += 1;     // would cause "illegal assignment" error
  }
}

var myR = new MyRecord();
myR.myMethod();

writeln("After MyRecord.myMethod, myR is:");
writeln(myR);
writeln();
