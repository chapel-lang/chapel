// Forall Loops

/*
This primer illustrates forall loops, which are a way to leverage
data parallelism or engage :ref:`user-defined parallel iterators
<primers-parIters>`. 

Like serial for-loops, forall loops can iterate over a data structure,
an iterator, or a zippered combination of these. Unlike for-loops,
multiple iterations of a forall-loop can potentially execute in parallel.
Parallelism is determined by the data structure or iterator being iterated
over. For a zippered forall loop, parallelism is determined by the "leader",
which is the first data structure or iterator in the zippered list.

Chapel has forall statements and forall expressions. Each form has
two varieties, "must-parallel" and "may-parallel".

 - The must-parallel forms are written using the ``forall`` keyword.
   They require that the leader iterable provide a parallel iterator.
   Note that there are no requirements on the behavior of the parallel
   iterator. For example, it can execute serially, in which case
   the "must-parallel" loop that invokes it also executes serially.

 - The may-parallel forms are written using ``[ ]``.
   They invoke the parallel iterator if the leader provides it,
   otherwise fall back on the serial iterator.

As with for-loops, the body of a forall statement is a statement or a
a block statement, whereas the body of a forall expression is an expression.

"Must-parallel" forall statement
--------------------------------

In the following example, the forall loop iterates over the array indices
in parallel:
*/

config const n = 9;
var A: [1..n] real;

forall i in 1..n {
  A[i] = i;
}

writeln("After setting up, A is:");
writeln(A);
writeln();

/*
If ``A`` were a distributed array, each loop iteration would be
executed on the locale where the corresponding array element resides.

"Must-parallel" forall expression
---------------------------------

The following forall-expression produces new values in parallel.
We store these values in a new array.
*/

var B = forall a in A do a * 3;

writeln("After initialization, B is:");
writeln(B);
writeln();

/*
Zippered "must-parallel" forall statement
-----------------------------------------

We can iterate over multiple arrays in parallel with a zippered loop.
Here we illustrate zippering arrays and domains:
*/

var C: [1..n] real;
forall (a, b, i) in zip(A, B, C.domain) do
  C[i] = a * 10 + b / 10;

writeln("After a zippered loop, C is:");
writeln(C);
writeln();

/*
"May-parallel" forall statement
-------------------------------

The iterator ``onlySerial`` defined below does not have any parallel
forms. Therefore the may-parallel loop ``[i in onlySerial(n)]``
will accept it, executing its iterations serially:
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
Using the following must-parallel loop instead would cause
a "parallel iterator is not found" error:

.. code-block:: chapel

    forall i in onlySerial(n) {
      writeln("in iteration #", i);
    }

"May-parallel" forall expression
--------------------------------

Given that these arrays provide parallel iteration, the following
may-parallel expression will be computed in parallel:
*/

var D = [(a,b,c) in zip(A,B,C)] a + c - b;

writeln("The result of may-parallel expression, D is:");
writeln(D);
writeln();

/*
As with must-parallel loops, if A were a distributed array,
iteration locality while computing the above expression
would be determined by A's domain map.

Forall Intents and Shadow Variables
-----------------------------------

A forall loop may name some variables declared outside the loop,
or "outer variables". If so, "shadow variables" are introduced.
Each task created by the parallel iterator gets its own set of
shadow variables, one per outer variable.

 - Each shadow variable behaves as if it were a formal argument
   of the task function for the task. The outer variable is passed
   to this formal argument according to the argument intent
   associated with the shadow variable, which is called a
   "forall intent".

 - The name of an outer variable lexically in the loop body implicitly
   refers to the corresonding shadow variable. If the parallel iterator
   causes multiple iterations of the loop to be executed by the
   same task, these iterations refer to the same set of shadow
   variables.

 - Each shadow variable is deallocated at the end of its task.

The default argument intent is used by default. For numeric types,
this implies capturing the value of the outer variable by the time
the task starts executing. Arrays are passed by reference, and so are
sync, single, and atomic variables.
*/

var outerIntVariable = 1;
proc updateOuterVariable() {
  outerIntVariable += 1;  // always refers to the outer variable
}
var outerAtomicVariable: atomic int;

forall i in 1..n {
  if i == 1 then
    updateOuterVariable();     // beware of potential for data races

  // the shadow variable always contains the value as of loop start
  writeln("shadow outerIntVariable is: ", outerIntVariable);

  D[i] += 0.5;                 // beware of potential for data races

  outerAtomicVariable.add(3);  // ok: concurrent updates are atomic
}

writeln();
writeln("After a loop with default intents, D is:");
writeln(D);
writeln("outerIntVariable is: ", outerIntVariable);
writeln("outerAtomicVariable is: ", outerAtomicVariable.read());
writeln();

/*
The forall intents ``in``, ``const in``, ``ref``, ``const ref``,
and ``reduce`` can be specified explicitly using a ``with`` clause.

An ``in`` or ``const in`` intent creates a copy of the outer variable
for each task. A ``ref`` or ``const ref`` makes the
shadow variable an aliass for the outer variable.
*/

var outerRealVariable = 1.0;

forall i in 1..n with (in outerIntVariable,
                       ref outerRealVariable) {
  outerIntVariable += 1; // a per-task copy, never accessed concurrently

  if i == 1 then
    outerRealVariable *= 2;  // beware of potential for data races
}

writeln("After a loop with explicit intents:");
writeln("outerIntVariable is: ", outerIntVariable);
writeln("outerRealVariable is: ", outerRealVariable);
writeln();

/*
A reduce intent can be used to compute reductions.
The values of each reduce-intent shadow variable at the end of its task
is combined onto its outer variable according to the specified reduction
operation. Within loop body, the shadow variable represents the
accumulation state produced so far by this task, starting from
the reduction identity value at task startup. Values can be
combined onto this accumulation state using the reduction-specific
operation or the ``reduce=`` operator.
*/

var outerMaxVariable = 0;
// outerIntVariable's value before the loop will be included in the sum

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
task, shared by all loop iterations executed by the task, and
deallocated at the end of the task. However, a task-private
variable is initialized without regard to any outer variable.

A task-private variable is introduce using a with-clause
in a way similar to a regular ``var``, ``const``, ``ref``,
or ``const ref`` variable. A ``var`` or ``const`` variable
must provide its type or initializing expression or both.
As with a regular variable, it will be initialized
to the default value of its type the initializing expression
is not given. A ``ref`` or ``const ref`` variable must
have the initializing expression and cannot declare its type.

A ``var`` task-private variable could be used, for example,
as a per-task scratch space that is never accessed concurrently.
*/

forall i in 1..n with (var myReal: real,  // starts at 0 for each task
                       ref outerIntVariable, // a shadow variable
                       ref myRef = outerIntVariable) {

  myReal += 0.1;   // ok: never accessed concurrently

  if i == 1 then
    myRef *= 3;    // beware of potential for data races
}

writeln("After a loop with task-private variables:");
writeln("outerIntVariable is: ", outerIntVariable);
writeln();

/*
Forall Intents Inside Record Methods
------------------------------------

When the forall loop occurs inside a method on a record,
the fields of the receiver record are represented in the loop body
with shadow variables as if they were outer variables.

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
