// Loops

/*

  This primer teaches the loop forms supported by Chapel, both serial
  and parallel.

*/

use IO;  // enable access to the readln() call that we use below

/*

  Like most imperative programming languages, Chapel supports loops.
  A loop is designed to run the statement or statements making up its
  body a number of times, where that number could be one or even zero.
  Chapel supports traditional serial loops, which execute the loop's
  iterations one after another.  It also supports parallel loops in
  which the loop's iterations may run simultaneously using hardware
  parallelism available in the target system.  This primer is designed
  to introduce these loop forms and to provide guidance as to when
  each might be appropriate.

  First, let's start with a quick survey of the loop forms before
  going through them in detail.  Chapel supports:

  * **while loops:** :ref:`for arbitrary serial loops <While Loops>`

    .. code-block:: chapel

      while cond do ...body...

  * **for loops:** :ref:`for iterator-driven serial loops <For Loops>`

    .. code-block:: chapel

      for i in 1..n do ...body...

  * **foreach loops:** :ref:`for parallel loops implemented using hardware parallelism (e.g., vectorization) <Foreach Loops>`

    .. code-block:: chapel

      foreach i in 1..n do ...body...

  * **forall loops:** :ref:`for parallel loops implemented using software parallelism (e.g., threads) <Forall Loops>`

    .. code-block:: chapel

      forall i in 1..n do ...body...

  * **square-bracket loops:** :ref:`as a shorthand for forall- or foreach-loops <Square-Bracket Loops>`
  
    .. code-block:: chapel

      [i in 1..n] ...body...

  * **coforall loops:** :ref:`for concurrent loops in which each iteration is executed by a distinct task <Coforall Loops>`
  
    .. code-block:: chapel

      coforall i in 1..n do ...body...

  In addition to going through these loop forms in detail, this primer
  also covers :ref:`loops over arrays and domains <loops-arrs-doms>`,
  :ref:`zippered iteration <zip-iter>`, :ref:`fully unrolled for-loops
  <param-for-loops>`, :ref:`promotion <loops-promotion>`, :ref:`race
  conditions <race-conditions>`, :ref:`loop nests <loop-nests>`,
  :ref:`when to use which loop form <loop-choice>`, and :ref:`a common
  performance mistake <common-loop-mistake>`.
      
  Serial Loops
  ------------

  .. _While Loops:
  
  While Loops
  ~~~~~~~~~~~

  We'll start with Chapel's *while-loops*, which execute as long as a
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

  In the event that the loop body consists of only a single statement,
  you can use the ``do`` keyword to define it rather than curly
  brackets (a *compound statement*).  For example:

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
  the variable defined above to drive our first while-loop.

*/

  do {
    const i = readln(int);
    writeln("Within the fourth while-loop, i is ", i);
  } while i < 100;

/*

  .. _For Loops:

  For Loops
  ~~~~~~~~~

  Chapel's other serial loop form is the *for-loop*.  Here is a simple
  for-loop that iterates over the integers 1 through 3, inclusive:

*/

  for i in 1..3 {
    writeln("Within the first for-loop, i is ", i);
  }

/*

  Though this example may look and act a lot like the C loop ``for
  (i=1; i<=3; i++)``, the way it works is somewhat different.
  Specifically, in Chapel a for-loop always invokes a *serial
  iterator*.  In more detail:

  Chapel for-loops generally take the form: ``for [inds] in [expr]``,
  where ``[expr]`` is the *iterand expression* that the loop is
  traversing.  When this iterand is a call to a Chapel iterator, the
  loop will invoke that iterator.  If the iterand is a variable or
  value of a given type, the loop invokes that type's default serial
  iterator method.  See the :ref:`Iterators Primer
  <primers-iterators>` for more about defining iterators.

  The iterand in the loop above is the range value ``1..3``, so the
  loop invokes the ``range`` type's default serial iterator method,
  which yields the range's indices one at a time.  For more about
  ranges, see the :ref:`Ranges Primer <primers-ranges>`.

  As values are yielded back to a for-loop, they are bound to the
  loop's *index variable(s)*.  In this case, the index variable is
  ``i``.  A for-loop's index or indices are brand new identifiers
  introduced by the loop, and each iteration of the loop can be
  thought of as getting its own private copy of the index variable.

  An implication of this is that the ``i`` variable in the loop above
  is new and distinct from previous ``i`` variables that appeared
  earlier in this primer.  Another implication is that a for-loop's
  index values will not be carried from one iteration of the loop to
  the next, nor persist after the loop completes.  If you want such
  behaviors, you'll need to use a while-loop, or to declare additional
  variables outside of the for-loop that will persist across
  iterations.

  The ``range`` type's iterator yields its indices by ``out`` intent,
  preventing the loop's index variable ``i`` from being assigned
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

  Other iterators may yield their indices using the ``ref`` intent,
  permits the loop index variable to be modified.  We'll see an
  example of this in the next section.

  .. _loops-arrs-doms:

  Loops over Arrays and Domains
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  In addition to looping over ranges and explicit iterators, loops in
  Chapel are commonly used to iterate over arrays or domains (see the
  :ref:`Arrays <primers-arrays>` and :ref:`Domains <primers-domains>`
  Primers for details on these types).  When iterating over an array
  variable, its serial iterator yields references to the array's
  elements, permitting them to be read or modified within the loop.
  For example:

*/

  var A = [1.2, 3.4, 5.6, 7.8];

  for a in A {
    writeln("The second for-loop is doubling ", a);
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

  .. _zip-iter:

  Zippered For-Loops
  ~~~~~~~~~~~~~~~~~~

  For-loops also support *zippered* iteration, in which multiple
  iterand expressions are invoked in a coordinated manner, yielding
  multiple indices.  For example, the following loop iterates over two
  ranges and an array in a zippered manner:

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

  .. _param-for-loops:
  
  Statically Varying (Unrolled) For-Loops
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  One last case to know about is that Chapel has a few for-loop forms
  that support the ability to have distinct static types or values per
  iteration.  This is achieved by unrolling the for-loop at
  compile-time to create distinct copies of the loop body that
  represent the different static properties.

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
  and each copy of the loop body will be specialized to the types of
  the tuple components (``int``, ``real``, and ``string``,
  respectively).  The second loop will also be specialized to the
  ``param`` values of ``i`` (0, 1, and 2, respectively).

  This concludes this primer's introduction to Chapel's serial loop forms.

  Parallel Loops
  --------------

  Next, let's look at Chapel's parallel loop forms, all of which are
  written very similarly to the serial for-loops shown above, simply
  using different keywords.  Specifically, each parallel loop form
  supports per-iteration index variables, zippered iteration, a ``do``
  keyword form for single-statement bodies, etc., just like the
  for-loop.

  A key property of parallel loops in Chapel is that the programmer is
  asserting that the loop's iterations are order-independent and that
  they can/should execute in parallel.  The Chapel compiler will take
  the programmer's word for this and do its best to implement the loop
  in parallel rather than trying to prove that the loop is
  parallel-safe or race-free before doing so.  As a result, it is
  possible for a user to write parallel loops that contain races or
  are otherwise unsafe, though Chapel's design reduces the chances of
  inadvertently doing so.

  In the following discussion, we'll divide Chapel's parallel loop
  forms into two categories: data-parallel loops (e.g., ``foreach``
  and ``forall``) and task-parallel loops (``coforall``).


  Data-Parallel Loops
  -------------------

  Data-parallel loops in Chapel can be thought of as indicating "the
  iterations of this loop can, and should, be performed in parallel."
  Unlike task-parallel loops, the specifics of how a data-parallel
  loop will be parallelized are abstracted away from the loop, as we
  will see.

  Because the specific implementation of a data-parallel loop is
  abstract, the programmer shouldn't assume anything about the amount
  of parallelism that will be used to implement the loop, nor how its
  iterations will be parallelized or scheduled.  The loop could even
  be executed completely serially like a for-loop.  For these reasons,
  performing any sort of blocking or synchronized operation between
  distinct iterations of a single data-parallel loop would violate the
  order-independent property and not be a legal use case (however,
  using a task-parallel loop in such cases may be more appropriate, as
  we will see).


  .. _Foreach Loops:

  Foreach Loops
  ~~~~~~~~~~~~~

  The first, and simplest, data-parallel loop is the ``foreach`` loop.
  This loop form asserts that the loop meets the order-independent and
  unsynchronized properties above, and specifies that its iterations
  should be implemented using hardware parallelism if possible.  When
  executing a foreach-loop on a conventional processor or GPU, the
  compiler will attempt to implement its iterations using any hardware
  SIMD/SIMT parallelism that's available.  For example, if executing
  the loop on a processor with vector instructions, it will attempt to
  implement the loop using those instructions if possible.  Notably, a
  foreach-loop will not implement its iterations using multiple Chapel
  tasks or software threads (the forall-loop, below, does this).

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
  on a processor that supports vector instructions for performing
  floating point multiplications, the ``foreach`` keyword's assertion
  that the loop is legal to parallelize improves the compiler's
  ability to implement the loop using those instructions.

  Like the for-loops above, Chapel's foreach-loops support zippered
  iteration.  For example, the following loop performs a zippered
  iteration over the array `A` and an unbounded range `1..`.

*/

  foreach (a, i) in zip(A, 1..) do
    a += (i / 100.0);

   writeln("After our first zippered foreach-loop, A is: ", A);

/*

  Note that if a Chapel program only used foreach-loops to express its
  parallelism, it would never make use of the multiple processor cores
  of a modern processor nor the distinct compute nodes of a cluster or
  HPC system.  This is because foreach-loops don't ever introduce new
  Chapel tasks, and tasks are the only way to run in parallel at
  system scales beyond a single processor core.  As a result, to
  leverage the full power of most parallel platforms, we need to look
  to Chapel's other parallel loop forms.

  .. _Forall Loops:

  Forall Loops
  ~~~~~~~~~~~~

  Forall-loops are similar to foreach-loops, except that they have the
  potential to be implemented using multiple Chapel tasks.  This
  permits them to use multiple cores and/or compute nodes to execute
  the loop's iterations.

  Just as Chapel's for-loops invoke a serial iterator, its
  forall-loops invoke a parallel iterator.  Where serial iterators may
  only yield values sequentially, a parallel iterator's yield
  statements may occur within parallel loops and constructs, resulting
  in parallel execution.  When using a forall-loop with zippered
  iterands, the first iterand in the zippering controls the loop's
  parallelism.  For details about writing such parallel iterators, see
  the :ref:`Parallel Iterators Primer <primers-parIters>`.

  A parallel iterator can create as many tasks as it wants, and can
  specify where they should run.  By convention, most will create as
  many tasks as are appropriate for the hardware parallelism that the
  loop iterand targets.  For example, the default parallel iterator
  for a range, local domain, or local array typically implements its
  iterations using a number of tasks equal to the number of local
  processor cores that are available, since those data structures are
  stored on a single locale.  In contrast, the default parallel
  iterator for a distributed domain or array will typically implement
  the iterations using all of the available processor cores on all of
  the locales that own a subset of the domain's indices or array's
  elements.

  The task that originally encountered the forall-loop will not
  proceed past the loop until all tasks created by the parallel
  iterator to run the loop's iterations have completed.  Logically,
  you can think of there as being a *join* operation on all tasks that
  are helping to implement the forall-loop.

  Looking at some simple examples, when run on a k-core processor,
  each of the following loops will typically use ``k`` tasks to
  implement the loop's iterations in parallel (at least, when ``k >
  n``):

*/

  config const n = 1000;

  var B: [1..n] real;


  forall i in 1..n do
    B[i] = i: real;

  writeln("After the forall loop over a range, B is: ", B);


  forall i in B.domain do
    B[i] = A[i % A.size];

  writeln("After the forall loop over a domain, B is: ", B);


  forall b in B do
    b = -b;

  writeln("After the forall loop over an array, B is: ", B);

/*

  Note the presence of the ``with``-clause in the first two loops
  above.  By default, variables declared outside of a parallel loop in
  Chapel, like ``B`` here, will be represented by a ``const`` shadow
  version of the variable within the loop itself.  This is designed to
  avoid inadvertent race conditions within the loop, by preventing
  multiple iterations of the loop from writing to the same variable
  simultaneously.  As a result, if it is your intention to modify the
  variable, using a ``with``-clause is a way to say how that variable
  should be made available to the loop.

  In the cases above that want to write to ``B``'s elements using
  ``B[i]``, the shadow variable for ``B`` would prevent such
  assignments since it is ``const``.  So we override that behavior
  using the ``with``-clauses to say that ``B`` should be made
  available to the loop body by reference (``ref``).  For more
  information on task intents and ``with``-clauses, refer to the
  :ref:`primers-forallLoops` and :ref:`primers-taskParallel` primers.

  Note, however, that when we're looping over the array itself, as in
  the last forall-loop above or the foreach-loops in the previous
  section, there's no need for such an intent because we're not
  modifying the shadow variable of something declared outside the
  loop; instead, it's just the loop's indices themselves, which do not
  receive shadow variables since they are already private to the
  iteration, and therefore not amenable to races.

  Next, let's consider some forall-loops over distributed domains and
  arrays.  When we iterate over a domain or array that's distributed
  across multiple locales, each with ``k`` cores, each locale will
  tend to use its ``k`` cores to iterate over the subset of the domain
  or array that it owns locally:
  
*/

  use BlockDist;

  const BlockDom = blockDist.createDomain({1..n, 1..n});
  var C: [BlockDom] real;

  forall (i,j) in BlockDom do
    C[i,j] = (100 * here.id) + i + j/1000.0;

  writeln("After the forall loop over a distributed domain, C is:\n",C);

  forall c in C do
    c *= 2;

  writeln("After the forall loop over a distributed array, C is:\n", C);

/*

  Because forall-loops invoke parallel iterators, the tasks they
  create and where they run are not defined by the Chapel language,
  but by the iterators themselves.  Any type supporting parallel
  iteration should describe how its parallel iterators work as part of
  its user-facing documentation.  For more about distributed domains
  and arrays or parallel iterators, refer to the
  :ref:`primers-distributions` and :ref:`primers-parIters` primers.

  .. _Square-Bracket Loops:
  
  Square-Bracket Loops
  ~~~~~~~~~~~~~~~~~~~~

  A third data-parallel loop form uses square brackets to define the
  loop instead of the ``foreach`` or ``forall`` keywords.  For
  example, such a loop may look like:

*/

  [i in 1..n] B[i] -= 0.001;

  writeln("After the first square bracket loop, B is:\n", B);

  [c in C] c -= 0.001;

  writeln("After the second square bracket loop, C is:\n", C);

/*

  In this loop form, the square brackets can be thought of as
  replacing the ``for[each|all]`` and ``do`` keywords, respectively.
  This loop is both a shorthand for a data parallel loop, while also
  supporting a "sliding scale" of parallelism.  Specifically, it will
  be equivalent to a ``forall`` loop if its iterand has/is a parallel
  iterator, and a ``foreach`` loop otherwise.

  .. _loops-promotion:
  
  Promotion and Data-Parallel Loops
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  In Chapel, an operator or procedure accepting a formal argument of
  type ``t`` can be *promoted* by invoking the procedure with:

  * an array whose elements are of type ``t``

  * a range or domain whose indices are of type ``t``

  Such promotions are equivalent to ``forall`` loops that iterate over
  each of the promoted actual arguments in a zippered manner, passing
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
  portion of ``C`` since ``C`` is the first iterand in the `zip`
  expression.

  .. _race-conditions:

  A final note on data-parallel loops and legality / races
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  As mentioned previously, the Chapel compiler and language are not
  responsible for making sure that a data-parallel loop is safe to
  execute in parallel.  Shadow variables reduce the chances of an
  accidental race, but do not protect against them.  If a programmer
  writes a data-parallel loop that is not parallel-safe or that
  creates a race, the outcome is their responsibility, not Chapel's.

  As an example, the following loop may appear to replace the interior
  elements of an array with the average of their neighbors; yet,
  because the same elements may be read and written simultaneously by
  distinct parallel iterations, the results will be unpredictable
  depending on how the iterations are scheduled at execution-time:

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
  the compiler will dutifully implement them as requested; but it will
  not protect the user from such races.

  One way to write the averaging computation above in a correct, but
  still parallel, manner would be to store the results into a distinct
  array to avoid reading and writing the same elements within a single
  parallel loop:

*/

  var E = [i in 1..n] (i**2): real;
  var F: [1..n] real;

  writeln("Before the safe averaging loop, E is: ", E);

  forall i in 2..<n do
    F[i] = (E[i-1] + E[i+1]) / 2;

  writeln("After the safe averaging loop, F is: ", F);

/* 

  Part of the reason the Chapel compiler does not prevent writing
  parallel loops with races is that it can be difficult to determine
  whether a given loop is safe to parallelize or not.  For example,
  the following variation on the original loop would be safe since it
  only writes to even elements and reads from odd ones:

*/

  D = [i in 1..n] (i**2): real;

  writeln("Before the third averaging loop, D is: ", D);
  forall i in 2..<n by 2 do
    D[i] = (D[i-1] + D[i+1]) / 2;

  writeln("After the third averaging loop, D is: ", D);

/*

  Distinguishing between loops that are parallel-safe versus not is
  generally intractable, so rather than attempting to make that
  judgment, Chapel trusts the programmer to use the loop form they
  want.  Moreover, for some parallel computations, race conditions can
  be benign, acceptable, or desirable; and Chapel does not want to
  prevent users from writing such computations.

  An important thing to remember about Chapel's forall-loops is that
  they are essentially invocations of parallel iterators.  In
  practice, those parallel iterators are themselves often written in
  terms of the data-parallel loop forms above, or the more explicit
  task-parallel coforall-loops that we'll cover next.


  Task-Parallel Loops
  -------------------

  Chapel has a single task-parallel loop form, the ``coforall`` loop:

  .. _Coforall Loops:

  Coforall Loops
  ~~~~~~~~~~~~~~

  In most respects, the coforall-loop is the simplest parallel loop
  form to explain in Chapel.  It literally creates a distinct Chapel
  task for each iteration of the loop; it then waits until each of
  those tasks has completed executing its copy of the loop body before
  proceeding.  Mnemonically, ``coforall`` can be thought of as a
  "concurrent forall loop".  For example, the following coforall loop
  will create four tasks, one for each iteration of the loop:

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
  processor core and/or locale.  As an example, the following
  coforall-loop creates a task per processor core across all locales:

*/


  coforall loc in Locales {
    on loc {
      coforall tid in 1..here.maxTaskPar {
        // ``here.maxTaskPar`` queries the number of parallel tasks
        // that can run on this locale, and it is typically equal to the
        // number of local processor cores.  So this loop body will be
        // executed once per core per locale.
      }
    }
  }

/*

  For further information about tasks in Chapel, see the :ref:`Task
  Parallelism Primer <primers-taskParallel>`.

  When using coforall loops, keep in mind that a task will be created
  for each iteration, and that each task will consume memory and
  processing resources.  For example, a coforall loop with a million
  iterations will literally create a million tasks.  If you don't have
  a million cores to run them on, this is likely to be overkill,
  requiring more memory and processing power than is warranted..  If
  there is no explicit synchronization between the iterations, a
  forall loop is typically a better choice, since it would use a
  number of tasks proportional to the targeted hardware parallelism.


  Closing Discussions
  -------------------

  At this point, you've learned about all of Chapel's loop forms.  The
  remaining sections cover some loop-related topics that may come up
  in practice.

  .. _loop-nests:

  Nesting Loops
  ~~~~~~~~~~~~~

  The loop forms discussed here can be nested arbitrarily, and their
  definitions are the same whether they are an outer or inner loop.  A
  nested for-loop will perform nested serial iterations as in other
  languages.  A nested coforall-loop will create a number of tasks
  equal to the outer loop's trip count and the sum of all the inner
  loops' counts.  For example, the following loop will create around
  ``x**2`` tasks, since each iteration of each loop will create its
  own task.

*/

  config const x = 4;

  writeln();
  coforall i in 1..x do
    coforall j in 1..x do
      writeln("Here's a message from one of the nested coforall tasks");

/*

  A tricky case to reason about in a nested loop situation is the
  forall-loop since its implementation is essentially "Do whatever the
  parallel iterator says."  If a parallel iterator were to always
  create ``x`` tasks (say), then a nested forall loop invoking that
  iterator in both loops would create roughly ``x**2`` tasks as in the
  coforall example above.

  In practice, however, most parallel iterators (including those
  defined on ranges, domains, and arrays) will take stock of the
  number of tasks running on the current locale and then throttle the
  number of tasks they create to avoid overwhelming the node.  As a
  result, a nested forall-loop over a pair of ranges, like:
  
*/

  forall i in 1..500 {
    forall j in 1..500 {
      // do some computation here
    }
  }

/*      

  will typically create only ``numCores`` tasks in total.
  Specifically, the outer loop will create ``numCores`` tasks, then
  each of the inner loops will see that all the cores are busy and
  avoid creating additional tasks since there is nowhere to run them.
  
  In such cases, if the inner loop body was not computationally
  intensive, it could make sense to rewrite the inner loop as a
  ``foreach`` in order to avoid the overhead of having the iterator
  determine whether or not to create tasks at all:

*/

  forall i in 1..500 {
    foreach j in 1..500 {
      // do some computation here
    }
  }

/*

  That said, such overheads are relatively modest, so for loop bodies
  that are computationally intensive, the benefit of changing the
  inner loop from ``forall`` to ``foreach`` may be negligible.

  In summary, there is nothing magical about nested loops.  When
  reasoning about what a given loop nest does, consider the loops one
  at a time.  For example, what does the outer loop do?  ("It's a
  ``forall``, so it will invoke the parallel iterator specified by its
  iterand.")  OK, what about the next loop?  ("It's a `coforall`, so
  it will literally create a task per iteration regardless of how many
  are already running).  What about the next loop?  ("It's a
  ``foreach``, so it will try to use hardware features in the task's
  current target processor to implement its iterations").  Chapel's
  implementation of parallel loops is very imperative, where the most
  complex case is being familiar with the parallelism implemented by
  any iterand expressions of a ``forall`` loop.

  .. _loop-choice:

  When to Use Which Loop Form?
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Given these various loop forms, which ones should you use when?

  Starting with the obvious, if you have a loop that wants or needs to
  be serial, such as a loop spanning the time steps of a simulation,
  you should use one of the serial loop forms.  When writing a serial
  loop, if you are iterating over a type that supports iteration, like
  an array, domain, range, or list, the for-loop can often be the
  clearest and most elegant loop form.  Or, if a serial iterator
  exists that does what you want, invoking it with a for-loop is also
  an obvious choice.  But if you want to do something more general
  that is not currently supported by an iterator, the while-loop can
  serve as a more general fallback.  Or you might want to write an
  iterator of your own that wraps your unique serial loop structure,
  and then use a for-loop to invoke it.  Refer to the :ref:`Iterators
  Primer <primers-iterators>` for more information about doing this.

  When choosing between the parallel loop forms, one consideration
  should be how many iterations the loop has.  For example, if you're
  iterating over an array with a million elements, you typically
  wouldn't want to use a coforall-loop, since that would literally
  create a million tasks.  Unless you happen to have a million
  processor cores, this is probably overkill.  And even if you do have
  a million cores, each would only own one element; so if your loop's
  body was not computationally intensive, you may spend more time
  creating and destroying tasks than actually getting useful work
  done.  In such cases, the forall- or foreach-loops could be a better
  choice since they will create parallelism proportional to the
  computational resources that are storing the array.

  Another consideration is whether you require synchronization or
  coordination between distinct iterations of the loop.  If you do,
  the coforall-loop is probably the right choice since it's the only
  one that permits inter-iteration coordination, since each iteration
  will be executed by a distinct task.  When you do not require
  synchronization between iterations, the forall-loop or
  square-bracket loop are generally good defaults to reach for since
  they will make best use of the hardware parallelism corresponding to
  the iterand expression.  The foreach loop can serve as an
  alternative if you know that you're already running a number of
  tasks that will saturate your hardware parallelism, or if the loop
  itself is of sufficiently modest size or computational intensity
  that creating new tasks to execute it would be overkill.

  .. _common-loop-mistake:
  
  A Common Performance Mistake
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Wrapping up, one of Chapel's most powerful features — the fact that
  forall loops can generate distributed memory parallelism in addition
  to local, multi-core parallelism — can also be the cause of simple
  errors that can kill performance.  When writing forall-loops, it is
  important to consider the iterand expression and what computational
  resources it will use.  Here is an example that illustrates how
  things can go wrong:

*/

  // create a block-distributed array G
  var G = blockDist.createArray(1..10, int);

  // attempt (but fail) to iterate over G's elements in a parallel,
  // distributed manner
  forall i in 1..10 do
    G[i] = i;

  writeln("After the non-distributed forall, G is: ", G);
  

/*

  While the code, as written, will work properly, the comment is
  incorrect in expecting that the computation will be distributed.
  Specifically, even though ``G`` is distributed and accessed within
  the loop, the forall loop's iterand is a range and ranges are not
  distributed.  As a result, the range's default iterator method will
  only consider the local cores when deciding how many tasks to create
  and where to run them.  The loop's body will still be able to update
  the remote elements of ``G`` by virtue of Chapel's global namespace.
  
  We can see that this is the case by storing the ID of the locale
  that executes each iteration into G:

*/
  
  forall i in 1..10 do
    G[i] = here.id;

  writeln("The locales assigning to G (range version) were: ", G);

/*

  In order to get the correct behavior, we'd need to iterate over
  something distributed instead, like G itself:

*/

  G = -1;  // reset G

  forall g in G do
    g = here.id;

  writeln("The locales assigning to G (array version) were: ", G);

/*

  Or its domain, which is also distributed:

*/

  G = -1;  // reset G

  forall i in G.domain do
    G[i] = here.id;

  writeln("The locales assigning to G (domain version) were: ", G);

/*

  In addition to iterating over arrays and domains, iterating over
  slices of arrays and domains is another technique for making sure
  your forall-loop computations maintain locality and affinity.  For
  example:

*/

  G = -1;  // reset G

  forall g in G[2..G.size-1] do
    g = here.id;

  writeln("The locales assigning to a slice of G were: ", G[2..G.size-1]);

/*

  As a final note, the following pattern can be a particularly
  surprising instance of the above:

*/

  forall loc in Locales {
    on loc {
      // do some computation
    }
  }

/*

  Although the `Locales` array represents the set of distributed
  locales on which the program is running, it is implemented using a
  local array on each locale.  As a result, the parallelism generated
  by this loop structure will once again be based on the number of
  local cores, implying that if ``numLocales >> here.maxTaskPar``, you
  will not end up executing on all the locales simultaneously.

  A better approach would be to use a coforall-loop:

*/

  coforall loc in Locales {
    on loc {
      // do some computation
    }
  }

/*

  This will create a task per locale regardless of the number of local
  cores, ensuring that all locales end up computing simultaneously.
  
  The lesson here is to make sure you're iterating over a distributed
  expression when you want your forall-loop to parallelize across a
  number of locales greater than the number of local cores.

  Conclusion
  ----------

  That wraps up this primer introducing Chapel's various loop types.
  For further details, refer to the Chapel language specification
  or ask questions in our user support channels.

*/
