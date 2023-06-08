// Loops

/*

  This primer teaches about Chapel's various loop forms, both serial
  and parallel.

*/

use IO;

/*

  Like most imperative programming languages, Chapel supports loops.
  A loop is designed to run its code body a number of times (where
  that number could be once, or even zero times).  Chapel supports
  traditional serial loops, which execute the loop's iterations one
  after the other.  It also supports parallel loops in which the
  loop's iterations may run simultaneously using the parallelism
  available from the target system's processors.  This primer is
  designed to introduce these loop forms and to provide guidance as to
  when each might be used.

  Serial Loops
  ============

  While-Loops
  -----------

  We'll start with Chapel's _while-loops_, which execute as long as a
  boolean condition remains true.  While loops come in two forms, the
  ``while...`` loop and the ``do...while`` loop.  These are much like
  their counterparts in other languages.

  Here is a ``while...`` loop that will print and double an integer
  `i` until its value exceeds 100:

*/

  var i = 1;
  while i < 100 {
    writeln("Within the first while-loop, i is ", i);
    i *= 2;
  }

/*

  In the event that the loop body only consists of a single statement,
  you can use the ``do`` keyword to define the loop body rather than
  curly brackets (a _compound statement_).  For example:

*/

  var j = 1;
  while j < 100 do
    j *= 3;
  writeln("At the end of the second while-loop, j is ", j);

/*

  If you want to be sure to execute the loop body at least once,
  you'll typically want to use the ``do...while`` form:

*/

  var k = 1;
  do {
    writeln("Within the third while-loop, k is ", k);
    k *= 4;
  } while k < 100;

/*

  One way in which Chapel's ``do...while`` loops may differ from those
  you've used in other languages is that the while condition can refer
  to symbols declared within the loop.  For example, the test against
  ``i`` in the following loop refers to the local constant declared
  within the loop's body rather than the variable defined in our
  original loop above.

*/

  do {
    const i = read(int);
    writeln("Within the fourth while-loop, i is ", i);
  } while i < 100;

/*

  For-Loops
  ---------

  Chapel's other serial loop form is the _for-loop_.  Here is a sample
  for-loop that iterates over the integers 1 through 10, inclusive:

*/

  for i in 1..3 {
    writeln("Within the first for-loop, i is ", i);
  }

/*

  Though this example may look and act a lot like the C loop ``for
  (i=1; i<=10; i++)``, the way it works is quite different.
  Specifically, a Chapel for-loop is a common way to invoke a _serial
  iterator_.  In this case, it is invoking the default serial iterator
  for range values.

  For-loops generally take the form: ``for [inds] in [expr]`` where
  ``[expr]`` is the _iterand_ expression that the loop is traversing.
  If this iterand is a variable or value of a given type, `it invokes
  that type's default serial iterator.  The iterand in the loop above
  is the range ``1..10``, so the loop invokes the default serial
  iterator for the ``range`` type, which yields the indices
  represented by the range one at a time.  For more about ranges, see
  the TODO ranges primer.

  As values are yielded back to a for-loop, they are bound to the
  loop's _index variable_ (or _indices_)—in this case ``i``.  The
  indices of a for-loop are new identifiers introduced by the loop,
  and each iteration of the loop can be thought of as getting its own
  copy of the variable.

  Specifically, in the loop above, each iteration will get its own
  index variable ``i``, distinct from the other iterations and our
  previous variables named ``i`` in the while-loop examples above.
  The range iterator yields ``const`` indices, so the ``i`` indices
  cannot be assigned within the loop body.  In effect, the loop above
  can be thought of as being equivalent to:

*/

  {
    const i = 1;
    writeln("Within the first for-loop, i is ", i);
  }
  {
    const i = 2;
    writeln("Within the first for-loop, i is ", i);
  }
  {
    const i = 3;
    writeln("Within the first for-loop, i is ", i);
  }

/*

  A for-loop's iterand can also be a call to a serial iterator, in
  which case the loop invokes the iterator and creates its
  indices in terms of the values yielded by the iterator.  For
  more about Chapel iterators, see the TODO iterators primer.

  For-Loops over Arrays and Domains
  ---------------------------------

  In addition to ranges and iterators, it is common for Chapel
  for-loops to iterate over arrays or domains (see TODO for more
  information about these types).  When iterating over an array, the
  corresponding index variable is a reference to the array's elements,
  permitting its values to be read or modified.  For example:

*/

  var A = [1.2, 3.4, 5.6, 7.8];

  for a in A {
    writeln("Doubling ", a, " within the second for-loop");
    a *= 2;
  }

  writeln("After the second for-loop, A is: ", A);

/*

  When iterating over a domain, the corresponding index variable
  represents a read-only copy of the domain's indices:

*/

  for i in A.domain {
    writeln("In the third for-loop, element ", i, " of A is ", A[i]);
  }

/*

  For a multidimensional domain, the index variable will be a tuple:

*/

  const Dom2D = {1..3, 1..3};

  for idx in Dom2D {
    writeln("The fourth for-loop got index ", idx, " from Dom2D");
  }

/*

  Like other tuples in Chapel, such indices can be de-tupled into
  their distinct components:

*/

  for (row, col) in Dom2D do
    writeln("The fifth for-loop got row = ", row, " and col = ", col);

/*

  This last example also demonstrates that single-statement for-loops
  can be declared using the ``do`` keyword, similar to while-loops
  above.


  Zippered For-Loops
  ------------------

  For-loops support _zippered_ iteration, in which multiple iterand
  expressions are invoked cooperatively, yielding multiple indices.
  For example, the following loop iterates over two ranges and an
  array in a zippered manner:

*/

  for idx in zip(1..4, 1..8 by 2, A) do
    writeln("Within the first zippered for-loop, idx is ", idx);

/*

  Note that the iterands in a zippered loop need to have compatible
  sizes and shapes.  In this case, each of the two ranges represent
  four indices, and the array is 1-dimensional and has four elements,
  so this is a legal zippering.

  A zippered loop generates a tuple index, storing one component for
  each iterand expression being zipped together.  As a result, in the
  loop above, `idx` is a 3-tuple, where the first two components are
  integers representing the indices yielded by ``1..4`` and ``1..8 by
  2``, respectively; and the third element is a reference to the
  elements of `A`.

  Like other tuples, such indices can be de-tupled into their distiinct
  components by de-tupling the index into its distinct components:

*/

  for (i, j, a) in zip(1..3, 1..6 by 2, A) do
    writeln("Within the second zippered for-loop, i, j, and a are: " (i,j,a));


/*

  Zippered for-loops can iterate over an arbitrary number of iterand
  expressions.

  Statically Varying (Unrolled) For-Loops
  ---------------------------------------

  One last case to know about is that Chapel supports a few for-loop
  forms that support distinct static types or values in each
  iteration.  This is achieved by unrolling the for-loop at
  compile-time to create distinct copies of the loop body and support
  different static signatures without loss of performance.

  The two primary ways to such for-loops today are by iterating over a
  heterogeneous tuple, or a range literal whose low and high bounds
  are both ``param`` expressions and whose index is also declared as a
  ``param``:

*/

  const tup = (1, 2.3, "four");

  for t in tup do
    writeln("A component of 'tup' has type ", t.type:string);

  for param i in 0..<tup.size do
    writeln("Component ", i, " of 'tup' has type ", tup(i).type:string);

/*

  For each of these loops, the compiler will fully unroll the loop,
  where each copy of the loop body will be specialized to the types of
  the tuple components (``int``, ``real``, and ``string``,
  respectively).  The second loop will also be specialized to the
  ``param`` values of `i` (0, 1, and 2, respectively).

  That concludes this primer's summary of Chapel's serial loop forms.

  Parallel Loops
  ==============

  Next, let's look at Chapel's parallel loop forms, all of which are
  syntactically similar to the serial for-loops shown above.  For this
  discussion, we'll divide Chapel's parallel loop forms into two
  categories: data-parallel loops and task-parallel loops.

  Data-Parallel Loops
  ===================

  Data-parallel loops in Chapel can be thought of as those that say
  "the iterations of this loop can, and should, be performed in
  parallel", yet in a way that avoids talking about the specifics of
  how that parallelism is implemented.  This is in contrast with
  task-parallel loops, where the loop is explicitly creating distinct
  parallel tasks.

  A key property of data-parallel loops in Chapel is that the
  programmer is essentially asserting that the loop is safe to run in
  parallel—specifically, that the iterations of the loop are
  order-independent.

  Because the parallel implementation of the loop is unspecified by
  the language, the programmer shouldn't assume anything about the
  amount of parallelism that will be used to implement the loop, nor
  how iterations will be parallelized.  The loop could even be
  executed serially like a for-loop.  For that reason, performing some
  sort of blocking synchronized operation between distinct iterations
  of a single loop would violate the order-independent property (and
  would suggest using a task-parallel loop instead).

  foreach-loops
  -------------

  The first, and simplest, data-parallel loop is the `foreach` loop.
  This loop form asserts that the loop meets our data-parallel
  criteria above, and says that it should be implemented using a
  processor's hardware-level parallelism.  Such loops enable the use
  of vector instructions on traditional CPUs or the use of multiple
  thread blocks when executing on a GPU.  Notably, a foreach-loop will
  not distribute its iterations across multiple Chapel tasks or
  software threads.

  Syntactically, foreach-loops are identical to for-loops, simply
  using the ``foreach`` keyword.  For example, the following
  ``foreach`` loop will double the values of the array `A` declared
  above:

*/

  foreach a in A do
    a *= 2;

  writeln("After our first foreach-loop, A is:\n", A);

/*

  Because each iteration is performing its own operations on its own
  elements of A, this loop is trivially parallelizable, and completely
  reasonable to write as a ``foreach``.  When running the computation
  on a processor that could vectorize four consecutive floating point
  multiplications, it would be reasonable to assume that this loop
  will be vectorized.  (TODO: Do we believe that?  Is this loop so
  small as to be silly?)

  Like the for-loops above, Chapel's foreach-loops support zippered
  iteration.  For example, given a second array, B, we could write:

*/

  foreach (a, i) in zip(A, 1..) do
    a += (i / 100.0);

   writeln("After our zippered foreach-loop, A is:\n", A);

/*

  Note that if a Chapel program were only ever to use foreach-loops to
  express its parallelism, it would never execute in parallel across
  the multiple processor cores of a modern processor or the distinct
  compute nodes of a cluster or HPC.  This is because foreach-loops
  don't ever introduce new Chapel tasks, and tasks are the only way to
  run in parallel at larger scales.  As a result, to leverage the full
  power of most parallel platforms, we need to look to Chapel's other
  parallel loop forms.

  Before that, though, is the implication that foreach-loops are not
  important or useful?  Not at all!  They are very useful for
  expressing parallelism in contexts where the program has already
  created as many tasks as it wants or needs to to keep all available
  hardware processors busy.  In such contexts, if a loop's work is
  parallel, using foreach loops avoids the overhead of creating
  additional tasks while still enabling the compiler to leverage any
  local hardware parallelism available to the current task (such as
  vector instructions).

  forall-loops
  ------------

  Forall-loops are like foreach-loops except that they have the
  potential to create new Chapel tasks that can make use of multiple
  cores and/or compute nodes in parallel.

  Similar to how a Chapel for-loop invokes a serial iterator, the
  forall-loop invokes a parallel iterator.  And where serial iterators
  may only yield values sequentially, a parallel iterator's yield
  statements may occur within parallel loops, resulting in parallel
  execution.

  A parallel iterator can create as many tasks as it wants, but by
  convention, most will create an amount that is appropriate for the
  available hardware parallelism that it is targeting.


  foreach a in A do
    a *= 2;

  writeln("After our first foreach-loop, A is:\n", A);


  square-bracket loops
  --------------------

  promotion and loops
  -------------------

  A final note on data-parallel loops and legality / races
  --------------------------------------------------------

  Note that the Chapel compiler and language are not responsible for
  making sure a data-parallel loop meets these properties.  If the
  programmer writes a data-parallel loop that is not parallel-safe or
  creates a race, that is on them.  As an example, the following loop
  may appear to replace the interior elements of an array with the
  average of its neighbors, but because the same elements may get read
  and written simultaneously by distinct parallel iterations, the
  results will be unpredictable depending on the scheduling of the
  computation:

*/

  var A = [i in 1..100] i: real;

  writeln("Before the race-y averaging loop, A is:\n", A);
  forall i in 2..99 do
    A[i] = (A[i-1] + A[i+1]) / 2;

  // if this were uncommented, you may see different results on each
  // execution of the program:
  //
  writeln("After the race-y averaging loop, A is:\n", A);

/*

  The programmer is still permitted to write such loops, and Chapel
  will dutifully implement it as requested, but will not protect the
  user against such races.

  Note that a way to write the above computation safely would be to
  store the results into a distinct array so that we're not reading
  and writing the same elements within the same parallel loop:

*/

  var B = [i in 1..100] i: real;
  var C: [1..100] real;

  writeln("Before the safe averaging loop, B is:\n", B);
  forall i in 2..99 do
    C[i] = (B[i-1] + B[i+1]) / 2;

  writeln("After the safe averaging loop, C is:\n", C);

/* 

  Part of the reason that the Chapel compiler does not guard against
  such races is that it can be incredibly subtle whether a given loop
  is safe to parallelize or not.  For example, a variation on the
  original loop would be safe if we only wrote to even elements and
  read from odd ones:

*/

  var D = [i in 1..100] i: real;
  
  writeln("Before the third averaging loop, D is:\n", A);
  forall i in 2..99 by 2 do
    D[i] = (D[i-1] + D[i+1]) / 2;

  // if this were uncommented, you may see different results on each
  // execution of the program:
  //
  writeln("After the third averaging loop, D is:\n", D);

/*

  Task-Parallel Loops
  ===================

  coforall loops
  --------------


  Nesting Loops
  =============

*/
