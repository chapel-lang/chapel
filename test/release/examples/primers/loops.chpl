// Loops

/*

  This primer teaches about Chapel's various loop forms, both serial
  and parallel.

*/

use IO;

/*

  Like most imperative programming languages, Chapel supports loops.
  A loop is designed to run the statement or statements making up its
  code body a number of times (where that number could be one time, or
  even zero).  Chapel supports traditional serial loops, which execute
  the loop's iterations one after another.  It also supports parallel
  loops in which the loop's iterations may run simultaneously using
  parallelism available in the target system.  This primer is designed
  to introduce these loop forms and to provide guidance as to when
  each might be used.

  Serial Loops
  ============

  While-Loops
  -----------

  We'll start with Chapel's _while-loops_, which execute as long as a
  boolean condition remains true.  While loops come in two forms, the
  ``while...`` loop and the ``do...while`` loop.  These are similar to
  their counterparts in other languages.

  Here is a ``while...`` loop that will print and double an integer
  ``i`` until its value exceeds 100:

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
  you've used in other languages is that the condition expression can
  refer to symbols declared within the loop.  For example, the test
  against ``i`` in the following loop refers to the local
  per-iteration constant declared within the loop's body rather than
  the variable defined earlier to drive the original loop.

*/

  do {
    const i = readln(int);
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
  (i=1; i<=10; i++)``, the way it works is somewhat different.
  Specifically, in Chapel a for-loop always invokes a _serial
  iterator_.  In more detail:

  Chapel for-loops generally take the form: ``for [inds] in [expr]``
  where ``[expr]`` is the _iterand_ expression that the loop is
  traversing.  When this iterand expression is a call to a Chapel
  iterator, the loop will invoke that iterator (see the
  :ref:`Iterators Primer <primers-iterators>` for more on defining
  iterators).  If the iterand is a variable or value of a given type,
  the loop invokes that type's default serial iterator method.  The
  iterand in the loop above is the range value ``1..3``, so the loop
  invokes the ``range`` type's default serial iterator method, which
  yields the range's indices one at a time.  For more about ranges,
  see the :ref:`Ranges Primer <primers-ranges>`.

  As values are yielded back to a for-loop, they are bound to the
  loop's _index variable(s)_.  In this case, the index variable is
  ``i``.  A for-loop's index or indices are brand new identifiers
  introduced by the loop, and each iteration of the loop can be
  thought of as getting its own private copy of the index variable.

  An implication of this is that the ``i`` variable in the loop above
  is new and distinct from previous ``i`` variables that appeared
  earlier in this primer.  Another implication is that a for-loop's
  index values will not be carried from one iteration of the loop to
  the next, nor persist after the loop completes.  If you want this
  behavior, use additional variables declared manually outside of the
  for-loop or a while-loop.

  Moreover, the ``range`` iterator yields its indices by ``const``
  intent, so the loop's ``i`` index variable cannot be re-assigned
  within the loop body.  In effect, the loop above can be thought of
  as being equivalent to:

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

  Other iterators may yield their indices using other intents, such as
  ``ref``, which would permit the loop index variables to be modified.
  We'll see an example of this in the next section.


  For-Loops over Arrays and Domains
  ---------------------------------

  In addition to looping over ranges and explicit iterators, it is
  common for Chapel for-loops to iterate over arrays or domains (see
  the :ref:`Arrays <primers-arrays>` and :ref:`Domains
  <primers-domains>` Primers for details on these types).  When
  iterating over an array variable, its serial iterator yields
  references to the array's elements, permitting them to be read or
  modified within the loop.  For example:

*/

  var A = [1.2, 3.4, 5.6, 7.8];

  for a in A {
    write("The second for-loop is Doubling ", a);
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

  For a multidimensional domain, the index variable will be a tuple,
  and indices will be yielded in row-major order:

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
  can be declared using the ``do`` keyword, similar to what we saw
  with while-loops above.


  Zippered For-Loops
  ------------------

  For-loops support _zippered_ iteration, in which multiple iterand
  expressions are invoked in a coordinated manner, yielding multiple
  indices.  For example, the following loop iterates over two ranges
  and an array in a zippered manner:

*/

  for idx in zip(1..4, 1..8 by 2, A) do
    writeln("Within the first zippered for-loop, idx is ", idx);

/*

  Note that the iterands in a zippered loop need to have compatible
  sizes and shapes.  In this case, each of the two ranges represent
  four indices, and the array has four elements and is similarly
  1-dimensional, so this is a legal zippering.

  A zippered loop generates a tuple index, storing one component for
  each iterand expression being zipped together.  As a result, in the
  loop above, ``idx`` is a 3-tuple, where the first two components are
  integers representing the indices yielded by ``1..4`` and ``1..8 by
  2``, respectively; the third element refers to the elements of ``A``.

  Like other tuples, such indices can be de-tupled into their
  distinct components in the loop header:

*/

  for (i, j, a) in zip(1..4, 1..8 by 2, A) do
    writeln("Within the second zippered for-loop, i, j, and a are: ", (i,j,a));


/*

  Zippered for-loops can iterate over an arbitrary number of iterand
  expressions.

  Statically Varying (Unrolled) For-Loops
  ---------------------------------------

  One last case to know about is that Chapel has a few for-loop forms
  that support distinct static types or values in each iteration.
  This is achieved by unrolling the for-loop at compile-time to create
  distinct copies of the loop body to represent the different static
  properties.

  The two primary ways to write such for-loops today are by iterating
  over:

  * a heterogeneous tuple

  * a range value whose low and high bounds are both ``param`` expressions and whose index variable is also declared as a ``param``:

*/

  const tup = (1, 2.3, "four");

  for t in tup do
    writeln("One component of 'tup' has type ", t.type:string);

  for param i in 0..<tup.size do
    writeln("Component ", i, " of 'tup' has type ", tup(i).type:string);

/*

  For each of these loops, the compiler will fully unroll the loop,
  where each copy of the loop body will be specialized to the types of
  the tuple components (``int``, ``real``, and ``string``,
  respectively).  The second loop will also be specialized to the
  ``param`` values of ``i`` (0, 1, and 2, respectively).

  This concludes this primer's summary of Chapel's serial loop forms.
  Next, let's look at Chapel's parallel loop forms, all of which are
  written very similarly to the serial for-loops shown here, yet using
  different keywords.  Each supports per-iteration index variables,
  zippered iteration, a ``do`` keyword form for single-statement
  bodies, etc.

  In the following discussion, we'll divide Chapel's parallel loop
  forms into two categories: data-parallel loops (e.g., ``foreach``
  and ``forall``) and task-parallel loops (``coforall``).


  Data-Parallel Loops
  ===================

  Data-parallel loops in Chapel can be thought of as indicating "the
  iterations of this loop can, and should, be performed in parallel",
  yet in a way that avoids specifying how that parallelism is
  implemented.  This is in contrast with task-parallel loops, where
  the loop is explicitly creating distinct parallel tasks.

  A key property of data-parallel loops in Chapel is that the
  programmer is essentially asserting that the loop is safe to run in
  parallel—specifically, that the iterations of the loop are
  order-independent.  The Chapel compiler will take the programmer's
  word for it and do its best to implement the loop in parallel rather
  than complaining or balking if the loop contains a race.

  Because the parallel implementation of data-parallel loops is
  unspecified by the language, the programmer shouldn't assume
  anything about the amount of parallelism that will be used to
  implement the loop, nor how its iterations will be parallelized.
  The loop could even be executed completely serially like a for-loop.
  For these reasons, performing any sort of blocking/synchronized
  operation between distinct iterations of a single data-parallel loop
  would violate the order-independent property and not be considered
  legal.  Using a task-parallel loop in such cases would be more
  appropriate.

  foreach-loops
  -------------

  The first, and simplest, data-parallel loop is the ``foreach`` loop.
  This loop form asserts that the loop meets the data-parallel
  criteria above, and specifies that its iterations should be
  implemented using hardware parallelism if possible.  When executing
  a foreach-loop on a conventional processor or GPU, the compiler will
  attempt to implement its iteration using any SIMD/SIMT parallelism
  that's available.  For example, if executing the loop on a processor
  with vector instructions, it will attempt to implement the loop
  using those instructions if possible.  Notably, a foreach-loop will
  not implement its iterations using multiple Chapel tasks or software
  threads (the forall-loop, below, does this).

  Syntactically, foreach-loops are identical to for-loops, simply
  using the ``foreach`` keyword.  For example, the following
  ``foreach`` loop will double the values of the array ``A`` declared
  above:

*/

  foreach a in A do
    a *= 2;

  writeln("After our first foreach-loop, A is: ", A);

/*

  Because each iteration is performing its own operations on its own
  elements of A, this loop is trivially parallel-safe, and completely
  reasonable to write using ``foreach``.  When running the computation
  on a processor that can vectorize floating point multiplications,
  the ``foreach`` improves the compiler's ability to implement the
  loop using those instructions, by asserting that the loop is legal
  to parallelize.

  Like the for-loops above, Chapel's foreach-loops support zippered
  iteration.  For example, given a second array, B, we could write:

*/

  foreach (a, i) in zip(A, 1..) do
    a += (i / 100.0);

   writeln("After our first zippered foreach-loop, A is: ", A);

/*

  Note that if a Chapel program were only ever to use foreach-loops to
  express its parallelism, it would never make use of the multiple
  processor cores of a modern processor nor the distinct compute nodes
  of a cluster or HPC system.  This is because foreach-loops don't
  ever introduce new Chapel tasks, and tasks are the only way to run
  in parallel at system scales beyond a single processor.  As a
  result, to leverage the full power of most parallel platforms, we
  need to look to Chapel's other parallel loop forms.

  Before doing so, though, does that imply that foreach-loops are not
  important or useful?  Not at all!  They are very useful for
  expressing parallelism in contexts where the program has already
  created as many tasks as it wants or needs to to make use of the
  available hardware processors.  In such cases, it may not be wise to
  create additional tasks, so the foreach-loop represents a way to
  take advantage of processor-level parallelism in the hardware
  without the overhead of potentially creating additional software
  tasks.


  forall-loops
  ------------

  Forall-loops are very similar to foreach-loops except that they have
  the potential to be implemented using multiple Chapel tasks.  This
  permits them to use multiple cores and/or compute nodes to execute
  the loop's iterations.

  Similar to how a Chapel for-loop invokes a serial iterator, the
  forall-loop invokes a parallel iterator.  Where serial iterators may
  only yield values sequentially, a parallel iterator's yield
  statements may occur within parallel loops and constructrs,
  resulting in parallel execution.  For details about writing a
  parallel iterator, see the :ref:`Parallel Iterators Primer
  <primers-parIter>`.

  A parallel iterator can create as many tasks as it wants, and to run
  them where it wants, but by convention, most will create an amount
  that is appropriate for the available hardware parallelism that it
  is targeting.  For example, the default parallel iterator for a
  range, local domain, or local array will typically implement its
  iterations using a number of tasks equal to the number of local
  available processor cores, since those data structures are stored by
  a single locale.  Meanwhile, the default parallel iterator for a
  distributed domain or array will typically implement the iterations
  using all of the available processor cores on all of the locales
  that own a piece of the domain or array.

  Looking at some simple examples, when run on a k-core processor, for
  large enough values of ``n``, each of the following loops will
  typically use ``k`` tasks to implement the loop's iterations in
  parallel:

*/

  config const n = 1000;

  var B: [1..n] real;


  forall i in 1..n do
    B[i] = i: real;

  writeln("After the first forall loop over a range, B is: ", B);


  forall i in B.domain do
    B[i] = A[i % A.size];

  writeln("After the second forall loop over a domain, B is: ", B);

  forall b in B do
    b = -b;

  writeln("After the third forall loop over an array, B is: ", B);

/*

  Similarly, if we iterate over a domain or array that's distributed
  across ``nl`` locales with ``k`` cores each, each locale will tend
  to use its ``k`` cores to iterate over the subset of the domain or
  array that it owns locally:
  
*/

  use BlockDist;

  const BlockDom = Block.createDomain({1..n, 1..n});
  var C: [BlockDom] real;

  forall (i,j) in BlockDom do
    C[i,j] = (100 * here.id) + i + j/1000.0;

  writeln("After the fourth forall loop over a distributed domain, C is:\n",C);

  forall c in C do
    c *= 2;

  writeln("After the fourth forall loop over a distributed array, C is:\n", C);

/*

  Because forall-loops invoke parallel iterators, the tasks they
  create and where they run are not defined by the Chapel language,
  but by the iterators themselves.  Any type supporting parallel
  iteration should describe how its parallel iterators work as part of
  its user-facing documentation.

  Square Bracket Loops
  --------------------

  A third data-parallel loop form uses square brackets to define the
  loop instead of the ``foreach`` or ``forall`` keywords.  For
  example, such a loop may look like:

*/
  
  [c in C] c -= 0.001;

  writeln("After the first square bracket loop, C is:\n", C);

/*

  In this loop form, the square brackets can be thought of as
  replacing the ``for[each|all]`` and ``do`` keywords, respectively.
  This loop is both a shorthand for a data parallel loop, while also
  supporting a "sliding scale" of parallelism.  Specifically, it will
  be equivalent to a ``forall`` loop if its iterand has (or is) a
  parallel iterator, and a ``foreach`` loop otherwise.

  
  Promotion and Parallel Loops
  ----------------------------

  In Chapel, an operator or procedure accepting a formal argument of
  type ``t`` can be _promoted_ by invoking the procedure with:

  * an array whose elements are of type ``t``

  * a range or domain whose indices are of type ``t``

  Such promotions are equivalent to ``forall`` loops that iterate over
  each of the promoting actual arguemnts in a zippered manner, passing
  the respective elements into the operator or procedure.  For
  example, given the procedure:

*/

  proc foo(ref x: real, t: (int, int), d: real) {
    x = t(0) + t(1)/d;
  }

/*

  The call:
*/

  foo(C, BlockDom, 100.0);

  writeln("After the promoted call to foo(), C is:\n", C);

/*

  is equivalent to the forall-loop:

*/

  forall (c, ij) in zip(C, BlockDom) do
    foo(c, ij, 100.0);

  writeln("After the equivalent zippered forall loop, C is:\n", C);

/*

  As a result, the parallel calls to ``foo()`` will be executed using
  the available processor cores on each of the locales that own a
  portion of ``C``.
  

  A final note on data-parallel loops and legality / races
  --------------------------------------------------------

  As mentioned previously, the Chapel compiler and language are not
  responsible for making sure that a data-parallel loop is safe to
  execute in parallel.  If the programmer writes a data-parallel loop
  that is not parallel-safe or that creates a race, the outcome is
  their responsibility rather than Chapel's.  As an example, the
  following loop may appear to replace the interior elements of an
  array with the average of its neighbors, yet because the same
  elements may be read and written simultaneously by distinct parallel
  iterations, the results will be unpredictable depending on how
  the iterations are executed:

*/

  var D = [i in 1..n] (i**2): real;

  writeln("Before the race-y averaging loop, D is: ", D);
  forall i in 2..<n do
    D[i] = (D[i-1] + D[i+1]) / 2;

  // if the following line were uncommented, you would likely see
  // different results after each execution of the program:
  //
  // writeln("After the race-y averaging loop, D is: ", D);

/*

  The programmer is still permitted to write such loops in Chapel, and
  the compiler will dutifully implement it as requested; but it will
  not protect the user from such races.

  Note that one way to write the above computation safely would be to
  store the results into a distinct array to avoid reading and writing
  and writing the same elements within a single parallel loop:

*/

  var E = [i in 1..n] (i**2): real;
  var F: [1..n] real;

  writeln("Before the safe averaging loop, E is: ", E);
  forall i in 2..<n do
    F[i] = (E[i-1] + E[i+1]) / 2;

  writeln("After the safe averaging loop, F is: ", F);

/* 

  Part of the reason that Chapel compiler does not prevent writing
  parallel loops with races is that it can be difficult to determine
  whether a given loop is safe to parallelize or not.  For example,
  the following variation on the original loop would be safe since
  it only writes to even elemnents and reads from odd ones:

*/

  D = [i in 1..n] (i**2): real;

  writeln("Before the third averaging loop, D is: ", D);
  forall i in 2..<n by 2 do
    D[i] = (D[i-1] + D[i+1]) / 2;

  writeln("After the third averaging loop, D is: ", D);

/*

  Distinguishing between loops that are parallel-safe versus not is
  intractable in general, so rather than attempting to make that
  judgemeent, Chapel trusts the programmer to use the loop form they
  want.  Finally, in some cases, loops with benign or acceptable races
  can be valuable to parallel computations, so Chapel does not want to
  outlaw such computations.

  At the end of the day, an important thing to remember about Chapel's
  forall-loops is that they are essentially invocations of parallel
  iterators.  In practice, those parallel iterators are themselves
  often written in terms of forall-loops, or the more explicit
  task-parallel coforall-loops.


  Task-Parallel Loops
  ===================

  Chapel has a single task-parallel loop form, the ``coforall`` loop:
  
  coforall loops
  --------------

  In most respects, the coforall-loop is the simplest parallel loop
  form to explain in Chapel.  It literally creates a distinct Chapel
  task for each iteration of the loop and then waits to proceed until
  those tasks have all completed executing their copy of the loop
  body.  Mnemonically, ``coforall` can be thought of as a "concurrent
  forall-loop".  For example, the following coforall loop will create
  four tasks, one for each iteration of the loop:

*/

  config const numTasks = 4;
  var total: atomic int;
  
  coforall tid in 1..numTasks do
    total.add(tid);

  writeln("The total of the integers 1..", numTasks, " is ", total.read());

/*

  Because each iteration is executed by a separate task,
  coforall-loops can synchronize between distinct iterations of the
  loop, unlike the data-parallel loop forms.  This makes
  coforall-loops useful as a means of creating an arbitrary number of
  tasks that are independently doing the same, or similar, things.  In
  practice, the parallel iterators used to define ``forall`` loops are
  often implemented in terms of coforall-loops that create a task per
  available processor core and/or locale.
  
  For further information about tasks in Chapel, see the :ref:`Task
  Parallelism Primer <primers-taskParallel>`.


  Closing Discussions
  ===================

  At this point, you've learned about all of Chapel's loop forms.  The
  remaining sections cover some loop-related discussion topics that
  often come up in practice.


  Nesting Loops
  -------------

  The loop forms discussed here can be nested arbitrarily, and their
  definitions are the same whether they are an outer or inner loop.  A
  nested for-loop will perform nested serial iterations as in other
  languages.  A nested coforall-loop will create a number of tasks
  equal to outer loop's trip count and the sum of all the inner loops'
  counts.  For example, the following loop would logically create
  `x + x*(x+1)/2` tasks:

*/

  config const x = 4;

  writeln();
  coforall i in 1..x do
    coforall j in 1..#i do
      writeln("Here's a message from one of the nested coforall tasks");

/*

  That said, since the outer tasks effectively have nothing to do but
  wait once they've created the inner ``coforall`` tasks, the
  implementation can potentially optimize things by re-using them to
  serve as one of the inner-loop tasks, reducing the total number of
  tasks to `x*(x+1)/2`.

  The most tricky case to reason about in a nested loop situation is
  the forall-loop since its implementation is essentially "Do whatever
  the parallel iterator says to."  If a parallel iterator were to
  always create a number of tasks equal to the local node's number of
  cores then a nested loop that invoked that iterator would create
  ``numCores**2`` tasks.  In practice, the default iterators for
  ranges, domains, and arrays attempt to take stock of the number of
  tasks running on the current locale and to throttle the number of
  tasks created to not overwhelm the node.  As a result, a nested
  forall-loop over a pair of ranges, like:
*/

  forall i in 1..500 do
    forall j in 1..500 do
      /* do some computation here */;

/*      

  will typically create only ``numCores`` tasks since the outer loop
  will create ``numCores`` tasks, then each of the inner loops will
  see that all the cores are busy and execute sequentially to avoid
  creating more tasks than the hardware can run concurrently.  If the
  inner loop body was not computationally intensive, it might make
  sense to rewrite the inner loop using ``foreach`` in order to avoid
  the overhead of determining whether or not to create tasks at all:

*/

  forall i in 1..500 do
    foreach j in 1..500 do
      /* do some computation here */;

/*

  That said, such overheads are modest, so for many computations,
  the penalty for using nested forall-loops will be modest in practice.


  When to Use Which Loop Form?
  ----------------------------

  Given these various loop forms, which ones should you use when?

  Starting with the obvious, if you have a loop that wants or needs to
  be serial, such as a loop spanning the time steps of a simulation,
  you should use one of the serial loop forms.  If you are invoking an
  existing iterator or iterating over a type that supports iteration,
  like an array, domain, range, or list, the for-loop can often be the
  clearest and most elegant loop form.  But if you want to do
  something more general, or not linked to an iterator, the while-loop
  can serve as a more general fallback.  Or, a hybrid solution would
  be to abstract the while-loop into a new iterator of your own and
  then use a for-loop to invoke the iterator.

  When choosing between the parallel loop forms, one consideration
  should be how many iterations the loop has.  For example, if you're
  iterating over an array with a million elements, you typically
  wouldn't want to use a coforall-loop, since that would literally
  create a million tasks.  Unless you happen to have a million
  processor cores, this is probably overkill; and even if you do, each
  task will only own one element, so unless your loop body has massive
  computational intensity, you may spend more time creating and
  destroying tasks than actually getting useful work done.  In such
  cases, the forall- or foreach-loops would be a better choice since
  they will create parallelism proportional to the computational
  resources that are storing the array.

  Another consideration is whether you require synchronization or
  coordination between distinct iterations of the loop.  If you do,
  the coforall-loop is probably the right choice since it's the only
  one that guarantees that each iteration will be executed with a
  distinct task.  When you do not require synchronization between
  iterations, the forall-loop or square-bracket loop are generally
  good defaults to reach for since they will make best use of the
  hardware parallelism corresponding to the iterand expression.  The
  foreach loop can also be used as an alternative if you either know
  that you're running a number of tasks that will saturate your
  hardware parallelism, or the loop itself is of sufficiently small
  size or computational intensity that creating tasks to execute it
  would likely be overkill.
  
  A Common Mistake
  ----------------

  Wrapping up, one of Chapel's most powerful features — the fact that
  forall loops can generate distributed memory parallelism in addition
  to local, multi-core parallelism — can also be a place for simple
  errors.  It is important, when writing forall-loops, to think about
  the iterand expression and what computational resources it will use.
  For example, here is an example of the sort of thing that can go
  wrong:

*/

  // create a block-distributed array G
  var G = Block.createArray(1..10, int);

  // attempt (but fail) to iterate over G's elements in a parallel,
  // distributed manner
  forall i in 1..10 do
    G[i] = i;

  writeln("After the incorrect forall, G is: ", G);
  

/*

  While the code, as written, will work properly, the comment is
  incorrect in stating that the computation will be distributed.
  Specifically, even though ``G`` is distributed and accessed within
  the loop, the forall loop's iterand is a range and ranges are not
  distributed.  As a result, the range's default iterator method will
  only consider the local cores when deciding how many tasks to create
  and where to run them.  We can see this by storing the ID of the
  locale that executes each iteration into G:

*/
  
  forall i in 1..10 do
    G[i] = here.id;

  writeln("The locales assigning to G (range version) were: ", G);

/*

  In order to get the correct behavior, we'd need to iterate over
  something distributed instead, like G itself:

*/

  forall g in G do
    g = here.id;

  writeln("The locales assigning to G (array version) were: ", G);

/*

  Or its domain, which is also distributed:

*/

  G = -1;

  forall i in G.domain do
    G[i] = here.id;

  writeln("The locales assigning to G (domain version) were: ", G);

/*

  In addition to iterating over arrays and domains, iterating over
  slices of arrays and domains is another technique for making sure
  your forall-loop computations maintain locality and affinity.
  
*/
