// Loops

/*
  This primer teaches about Chapel's various loop styles, serial and
  parallel.
*/

use IO;

/*

  Like most imperative programming languages, Chapel supports loops,
  designed to run a piece of code zero, one, or multiple times.
  Chapel supports traditional serial loops, which execute the loop's
  iterations one at a time.  It also supports parallel loops in which
  the iterations of the loop may run simultaneously using parallelism
  available in the target platform's processors.  This primer is
  designed to teach these loop forms and to provide guidance as to
  when each might be used.

  Serial Loops
  ============

  While-Loops
  -----------

  We'll start with Chapel's _while-loops_, which execute as long as aa
  boolean condition remains true.  While loops come in two forms, the
  ``while...`` loop and the ``do...while`` loop.  These are much like
  their counterparts in other languages.

  Here is a ``while...`` loop that will print and double an integer
  `i` until its value exceeds 100:

*/

  var i = 1;
  while i < 100 {
    writeln("Within this loop, i is ", i);
    i *= 2;
  }

/*

  In the event that the loop body only consists of a single statement,
  you can use the ``do`` keyword rather than curly brackets (a
  _compound statement_) to define the loop body.  For example:

*/

  var j = 1;
  while j < 100 do
    j *= 3;
  writeln("At the end of this loop, j is ", j);

/*

  If you want to be sure to execute the loop body at least once,
  you'll typically want to use the ``do...while`` form:

*/

  var k = 1;
  do {
    writeln("Within this loop, k is ", k);
    k *= 4;
  } while k < 100;

/*

  One way in which Chapel's ``do...while`` loops may differ from those
  in other languages you've used is that the while condition can refer
  to symbols declared within the loop.  For example, the test against
  ``i`` here refers to the constant declared within the current
  iteration's body rather than the variable defined in our original
  loop above.

*/

  do {
    const i = read(int);
    writeln("Within this loop, i is ", i);
  } while i < 100;

/*

  For-Loops
  ---------

  Chapel's other serial loop form is the _for-loop_.  Here is a sample
  for-loop that iterates over the integers 1 through 10, inclusive:

*/

  for i in 1..3 {
    writeln("Within this for-loop, i is ", i);
  }

/*

  Though this example may act and look a lot like the C loop ``for
  (i=1; i<=10; i++)``, the way it works is quite different.
  Specifically, a Chapel for-loop is a common way to invoke a _serial
  iterator_.

  For-loops generally take the form: ``for [inds] in [expr]`` where
  ``[expr]`` is the _iterand_ expression that the loop is traversing.
  If this iterand is a variable or value of a given type, `it invokes
  that type's default serial iterator.  The iterand in the loop above
  is the range ``1..10``, so it invokes the default serial iterator
  for the ``range`` type, which yields the indices represented by the
  range one at a time.  For more about ranges, see the TODO ranges
  primer.

  As values are yielded back to a for-loop, they are bound to the
  loop's index (or indices)—in this case ``i``.  The indices of a
  for-loop are new identifiers introduced by the loop, and each
  iteration of the loop can be thought of as getting its own copy of
  the variable.

  Specifically, in the loop above, each iteartion will get its own
  index ``i``, distinct from the other iterations and our previous
  variables named ``i`` in the while-loop examples above.  The range
  iterator yields ``const`` indices, so the ``i`` indices cannot be
  assigned within the loop body.  In effect, the loop above can be
  thought of as being equivalent to:

*/

  {
    const i = 1;
    writeln("Within this for-loop, i is ", i);
  }
  {
    const i = 2;
    writeln("Within this for-loop, i is ", i);
  }
  {
    const i = 3;
    writeln("Within this for-loop, i is ", i);
  }

/*

  A for-loop's iterand can also be a call to a serial iterator, in
  which case the loop invokes the iterator and creates its
  indices in terms of the values yielded by the iterator.  FOr
  omre about Chapel iterators, see the TODO iterators primer.

  Zippered For-Loops
  ------------------

  For loops support _zippered_ iteration, in which multiple iterand
  expressions are invoked cooperatively, yielding multiple indices.
  For example, the following loop iterates over two ranges in a
  zippered manner:

*/

  for ij in zip(1..3, 1..6 by 2) {
    writeln("Within this for-loop, ij is ", ij);
  }

/*

  Note that the iterands in a zippered loop need to have compatible
  sizes and shapes.  In this case, each of the two ranges represent
  three indices, so this is a legal zippering.

  A zippered loop generates a tuple index, storing one component for
  each iterand expression being zipped together.  As a result, in the
  loop above, `ij` is a 2-tuple of integers, where the first
  represents those yielded by ``1..3`` and the second ``1..6 by 2``.

  Like other tuples, such indices can be de-tuples into their separate
  components by de-tupling the index into its distinct components:

*/

  for (i,j,k) in zip(1..3, 1..6 by 2, 1.9 by 3) do
    writeln("Within this loop, i is ", i, " j is ", j, " and k is ", k);

/*

  This loop also demonstrates that zippered for loops can iterate over
  more than just two iterands (an arbitrary number), and that
  single-statement for-loops can be declared using the ``do`` keyword,
  as with while-loops above.

  Fully-Unrolled For-loops
  ------------------------

  One last case to know about is that Chapel supports a few loop forms
  that are fully unrolled at compile-time.  Such loops permit their
  indices and other variables within the loop to take on different
  values in different iterations.  The two primary ways to get fully
  unrolled for-loops today are by iterating over a heterogeneous
  tuple, or a bounded range whose bounds are both ``param`` values and
  whose index is also declared as a ``param``:

*/

  const tup = (1, 2.3, "four");

  for t in tup {
    writeln("The next component of 'tup' has type ", t.type:string);
  }

  for param i in 0..<tup.size {
    writeln("Component ", i, " of 'tup' has type ", tup(i).type:string);
  }

/*

  Such loops can be particularly helpful when you need a loop whose
  body requires different types on different iterations.
  Performance-minded programmers might also be inclined to use
  fully-unrolled ``param`` loops as a means of enabling vectorization
  in the compiler.  While this is a time-honored technique, Chapel's
  parallel loops provide alternative approaches.




  Data-Parallel Loops
  ===================

  foreach-loops
  -------------

  forall-loops
  ------------


  Task-Parallel Loops
  ===================

  coforall loops
  --------------


  Nesting Loops
  =============

*/
