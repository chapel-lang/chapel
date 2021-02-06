// Parallel Iterators

/*
  This primer explains how to write parallel iterators in Chapel,
  which can be used to drive parallel ``forall`` loops.  It assumes
  that the reader already knows how to define serial iterators in
  Chapel, as summarized in the :ref:`iterators primer (iterators.chpl)
  <primers-iterators>` for example.

  Chapel has two main flavors of parallel iterators: `Standalone`
  parallel iterators are the simpler form and can be used to define
  parallelism for a simple forall loop like ``forall i in
  myIter(...)``.  `Leader-follower` iterators are a more involved form
  that support zippered forall loops, like ``forall (...,i,...) in
  zip(..., myIter(...), ...)``.  Note that only defining
  leader-follower iterators is sufficient for use in a non-zippered
  forall loop, though often with additional overhead.

  For a more thorough introduction to leader-follower iterators, refer
  to our PGAS 2011 paper, `User-Defined Parallel Zippered Iterators in
  Chapel`_. Note that there are known lacks and issues with Chapel's
  current definition of parallel iterators, and that we anticipate
  addressing these and improving them over time.  To that end, this
  primer should be considered a snapshot of their status in the
  current implementation.
*/

/*
Motivating Example: a `count` iterator
--------------------------------------
*/

// In this primer, we're going to create a simple iterator named
// ``count`` that will be able to be invoked in either ``for`` or
// ``forall`` loops.  ``count`` will be defined to take an argument
// ``n`` as input and an optional argument ``low`` (set to 1 by
// default), and it will yield ``n`` integers starting with ``low``.
//
// We'll use the following config const ``numTasks`` to indicate the
// degree of parallelism that ``count()`` should use in its forall
// loops.  By default, we've set it to the maximum amount of
// parallelism expected on the current locale, but this can be
// overridden on the executable command-line using the
// ``--numTasks=<n>`` option.
//
config const numTasks = here.maxTaskPar;
if numTasks < 1 then
  halt("numTasks must be a positive integer");

//
// If compiled with ``verbose`` set to ``true``, the parallel
// iterators in this primer will print indications of what they're
// doing under the surface.
//
config param verbose = false;

//
// Next, we declare a problem size for this test.  By default we use a
// small problem size to make the output readable.  Of course, to use
// the parallelism effectively you'd want to use a much larger problem
// size (override on the execution command-line using the
// ``--probSize=<n>`` option).
//
config const probSize = 15;
var A: [1..probSize] real;


//
// To get started, we'll define a traditional serial iterator for
// ``count``.  In part, this is for purposes of illustration in this
// primer.  However, it is also a necessity in that Chapel's current
// implementation of parallel iterators requires there to be a serial
// overload of the iterator as well, to model the expected signature
// and yielded type.
//
iter count(n: int, low: int=1) {
  for i in low..#n do
    yield i;
}


//
// Here are some simple loops using the serial iterator to demonstrate
// it.  First we iterate over all indices in our problem size to
// initialize array ``A``:
//
for i in count(probSize) do
  A[i] = i:real;

writeln("After serial initialization, A is:");
writeln(A);
writeln();

//
// Then we override the default value of ``low`` in order to negate
// the "middle" elements of ``A``:
//
for i in count(n=probSize/2, low=probSize/4) do
  A[i] = -A[i];

writeln("After negating the middle of A:");
writeln(A);
writeln();

//
// For serial zippered iteration, nothing is required other than
// this single iterator:
//
for (i, a) in zip(count(probSize), A) do
  a = i/10.0;

writeln("After re-assigning A using zippering:");
writeln(A);
writeln();

/*
.. primers-parIters-standalone-parallel

A standalone parallel `count` iterator
--------------------------------------
*/


// To create a parallel version of ``count``, we will declare a second
// overload of the iterator with the same signature, but an additional
// ``param`` argument named ``tag`` of built-in enumerated type
// ``iterKind``, to distinguish it from the serial version.  The
// author of a standalone parallel iterator should use a ``where``
// clause to distinguish this overload from others.  Specifically,
// when the Chapel compiler attempts to implement a forall loop like
// ``forall i in count(...)``, it will attempt to resolve the iterator
// by passing in ``iterKind.standalone`` as its value, to distinguish
// it from the serial iterator above.  This argument is what marks
// this version of the iterator as a parallel iterator.  After the
// ``tag`` argument, the rest of the argument list should exactly
// match that of the serial iterator.  For the ``count()`` example,
// this means providing the same ``n`` and ``low`` arguments as
// before.
//
// Unlike serial iterators, parallel iterators are allowed to contain
// ``yield`` statements within parallel constructs like ``coforall``,
// ``cobegin``, and ``forall``.  In our implementation of the
// standalone parallel version of ``count`` here, we use a
// ``coforall`` loop to define ``numTasks`` tasks and then divide the
// iteration space up amongst them.  Specifically, each task calls
// into a helper routine defined at the bottom of this file,
// ``computeChunk()`` that computes its sub-range of the values to be
// counted as a function of its task ID (``tid``) and the total number
// of tasks (``numTasks``).  The iterator also contains debugging
// output which can be enabled by compiling with ``-sverbose=true``.
//
iter count(param tag: iterKind, n: int, low: int = 1)
       where tag == iterKind.standalone {
  if verbose then
    writeln("Standalone parallel count() is creating ", numTasks, " tasks");

  coforall tid in 0..#numTasks {
    const myIters = computeChunk(low..#n, tid, numTasks);

    if verbose then
      writeln("task ", tid, " owns ", myIters);

    for i in myIters do
      yield i;
  }
}

// Though not shown in this example, standalone parallel iterators may
// also target multiple locales using features like ``on`` statements
// or distributed arrays.

/*
.. primers-parIters-standalone-usage

Using the standalone parallel 'count' iterator
----------------------------------------------
*/

// Having defined a standalone parallel iterator, we can execute the
// same loops as before, but using forall loops to make the execution
// parallel.  Since these forall loops are not using zippered
// iteration, the standalone version of the ``count()`` iterator is
// used.
//
forall i in count(probSize) do
  A[i] = i:real;

writeln("After parallel initialization, A is:");
writeln(A);
writeln();

//
// Invoking it again with a different low value:
//
forall i in count(n=probSize/2, low=probSize/4) do
  A[i] = -A[i];

writeln("After negating the middle of A in parallel:");
writeln(A);
writeln();


/*
.. primers-parIters-leader-follower:

Leader-follower iterators
-------------------------
*/

// The parallel iterators for zippered forall loops are necessarily
// more involved since each iterand expression may have its own
// preferred way of doing parallel iteration, yet its yielded values
// must be combined with the other iterands in a way that generates a
// coherent result tuple.  To deal with this challenge, given a
// forall loop of the form:

/* .. code-block:: chapel

      forall (a, b, c) in zip(A, B, C) do
         // ...loop body...
*/

// Chapel's definition designates the first iterand -- in this case,
// ``A`` -- as the 'leader'.  In addition, all iterands in the
// zippering are considered 'followers' (so for this loop, ``A``,
// ``B``, and ``C`` would be).
//

// Given such a loop, the compiler will roughly translate it into
// the following loop structure:

/* .. code-block:: chapel

      for work in A.lead() do   // implemented by inlining the leader
        for (a, b, c) in zip(A.follow(work), B.follow(work), C.follow(work)) do
          // ...loop body...
*/

// where ``.lead()`` and ``.follow()`` represent the leader-follower
// iterators using a simplified naming scheme.
//
// Note that since Chapel's implicitly parallel features are defined
// in terms of zippered iteration, they are also implemented using
// leader-follower iterators.  For example, ``A = B + C;`` will be
// converted to an equivalent zippered parallel loop and then to the
// leader-follower idiom shown above. ``foo(A, B, C)`` goes through a
// similar transformation, where ``foo()`` is defined to take scalar
// arguments and is promoted in this call.  In both cases, ``A``
// serves as the leader and ``A``, ``B``, and ``C`` are all followers.

/*
.. primers-parIters-roles:

Leader-follower Roles
---------------------
*/

// At a high level, the role of a leader iterator is to:
//
//   a) create the parallel tasks used to implement the forall loop,
//   b) associate the tasks with specific locales, if desired,
//   c) assign work (e.g., iterations) to each parallel task
//
// The leader typically creates the parallelism using task parallel
// features like coforall loops; and it associates tasks with locales
// using locality features like on-clauses.  The leader specifies work
// for tasks by having each task it creates yield some representation
// of the work it owns.
//
// The role of the follower iterator is to take as an input argument
// a chunk of work (as yielded by a leader) and to serially iterate
// over and yield the elements/values corresponding to those
// iterations in order.
//
// Let's consider how these roles might play out for our ``count()``
// iterator:


/*
.. primers-parIters-leader

count: leader
-------------
*/

// As with the standalone parallel iterator, leader and follower
// iterators are defined as overloads of the serial version of the
// iterator, once again distinguished by an initial
// ``param`` argument of type ``iterKind``.  To
// invoke the leader iterator and differentiate it from the other
// overloads, the compiler will pass in the value ``iterKind.leader`` to
// this argument.  The author of the leader iterator should use a
// ``where`` clause to distinguish this overload from the others.  As
// with the standalone iterator, the rest of the argument list should
// match that of the serial iterator exactly.
//
// The implementation of our ``count()`` leader iterator is relatively
// similar to the standalone case.  It again uses a coforall loop to
// create a number of tasks equal to the number specified by our
// ``numTasks`` configuration constant.  However, rather than iterating
// over and yielding the values owned by each task, it will instead
// yield a version of the range itself as a means of telling the follower
// iterators what to do.
//
// To be a legal leader iterator, we could simply have each task yield
// its range as the representation of the work we want the follower to
// perform.  However, to support zippering our leader with follower
// iterators written by others, we typically take the convention of
// having iterators over 1D or dense rectangular index spaces yield
// tuples of ranges shifted to a 0-based coordinate system.  In this
// way, the leader-follower iterators have a common representation for
// the work even though each may use its own indexing system.  This
// permits, for example, arrays of the same size/shape to be zippered
// together even if they have different indices.
//
// For this reason, rather than yielding subranges of ``low..#n``,
// we'll yield subranges of ``0..n-1`` and rely on the follower to
// shift the values back to their preferred coordinate system.  As
// a result, we translate each task's range by ``-low`` to shift it
// from ``low``-based coordinates to 0-based coordinates; and then we
// make a 1-tuple out of it.
//
// Note the debugging output inserted into this iterator.  While
// learning about leader-follower iterators, it's useful to turn
// this debugging output on by compiling with ``-sverbose=true``
//
iter count(param tag: iterKind, n: int, low: int=1)
  where tag == iterKind.leader {

  if verbose then
    writeln("In count() leader, creating ", numTasks, " tasks");

  coforall tid in 0..#numTasks {
    const myIters = computeChunk(low..#n, tid, numTasks);
    const zeroBasedIters = myIters.translate(-low);

    if verbose then
      writeln("task ", tid, " owns ", myIters, " yielded as: ", zeroBasedIters);

    yield (zeroBasedIters,); // yield a 1-tuple of our sub-range
  }
}

//
// As mentioned at the outset, this leader is fairly static and
// simple.  More generally, a leader can introduce tasks more
// dynamically, partition work between the tasks more dynamically,
// etc.  See :mod:`DynamicIters` for some more interesting examples of
// leader iterators, including those that use dynamic partitioning.
//

/*
.. primers-parIters-follower

count: follower
---------------
*/

// The follower is another overload of the same iterator name, this
// time taking the ``iterKind.follower`` param enumeration as its
// first argument.  The subsequent arguments should match the leader
// and serial iterators exactly again (so, ``n`` and ``low`` for our
// example).  The final argument must be called ``followThis`` which
// represents the data yielded by the leader (in our case, the 1-tuple
// of 0-based ranges).
//
// The goal of the follower is to do the iteration specified by the
// ``followThis`` argument, serially yielding the elements
// corresponding to those iterations.  In our case, this involves
// plucking the range back out of the 1-tuple of ranges, and shifting
// it back to our ``low``-based coordinate system.  We then use a
// standard for-loop to iterate over that range and yield the
// corresponding indices.  Followers, as the name suggests, tend not
// to be very sophisticated, and simply do what the leader tells them
// to.
//
// As with the leader, this follower has been authored to support
// debugging output when compiled with ``-sverbose=true``.
//
iter count(param tag: iterKind, n: int, low: int=1, followThis)
       where tag == iterKind.follower && followThis.size == 1 {
  const (followInds,) = followThis;
  const lowBasedIters = followInds.translate(low);

  if (verbose) then
    writeln("Follower received ", followThis, " as work chunk; shifting to ",
            lowBasedIters);

  for i in lowBasedIters do
    yield i;
}

//
// Now let's use our leader/follower iterators.  In the following
// loop, ``count()`` serves as the leader and follower while the ``A``
// array is just a follower.  This works because ``A`` is a
// rectangular array whose follower iterator also accepts tuples of
// 0-based ranges like the ones ``count()``'s leader is yielding.  If
// we were to have ``count()`` yield something else (like a raw
// subrange of ``low..#n``), then the two things could not be zippered
// correctly because they wouldn't be speaking the same language --
// either in terms of the type of work being yielded (range
// vs. 1-tuple of range), nor the description of the work
// (``low``-based indices vs. 0-based indices).
//
forall (i, a) in zip(count(probSize), A) do
  a = i/10.0;

writeln("After re-assigning A using parallel zippering:");
writeln(A);
writeln();

//
// We can also zipper in the opposite order, making ``A`` the leader,
// in which case ``count()`` no longer controls the degree of parallelism
// and work assignment since it is no longer the leader.  Instead,
// ``A``'s leader iterator (defined as part of its domain map) is invoked.
// For standard Chapel arrays and domain maps, these leader-follower
// iterators are controlled by the ``dataPar*`` configuration constants
// described in doc/rst/usingchapel/executing.rst.
//
forall (a, i) in zip(A, count(probSize)) do
  a = i/100.0;

writeln("After re-assigning A using parallel zippering and A as the leader:");
writeln(A);
writeln();

//
// Finally, as mentioned at the outset, operations that are equivalent
// to zippering also use leader-follower iterators, so for example
// the following whole-array assignment will use ``A``'s leader and
// ``count()``'s follower:
//
A = count(probSize, low=100);

writeln("After re-assigning A using whole-array assignment:");
writeln(A);
writeln();

/*
.. primers-parIters-closing-notes

Closing notes
-------------
*/

// Chapel data types like records and classes can support iteration by
// defining iterator methods (invoked by name) or ``these()``
// iterators which support iterating over variables of that type
// directly.  Such iterator methods can be overloaded to support
// standalone and/or leader-follower versions to support parallel
// iteration over the variable.
//
// As mentioned at the outset, our leader-follower scheme has a number
// of changes planned for it such as interface improvements and better
// error checking.  We'll update this primer over time as we improve
// these features.
//
// **Definition of helper function used above:**
//
// The following utility function partitions a range into
// ``numChunks`` sub-ranges and returns a range representing the
// indices for sub-range ``myChunk`` (counting from 0).  The absolute
// difference between the size of the ranges returned is at most 1
// (either 0 or 1). If the value of remainder ``rem`` is equal to 0,
// then each sub-range contains ``elemsPerChunk`` indices, equal to
// ``floor(numElements/numChunks)`` work items. But if ``rem`` is not
// equal to 0, then the first ``rem`` sub-ranges get
// (``elemsPerChunk+ 1``) indices and the rest (chunks ``rem`` to
// ``numChunks-1``) get ``elemsPerChunk`` indices. For simplicity, this
// routine works only for unstrided ranges with the default index type
// of ``int``.  These contraints could be relaxed with more effort.
// 
proc computeChunk(r: range, myChunk, numChunks) where r.stridable == false {
  const numElems = r.size;
  const elemsperChunk = numElems/numChunks;
  const rem = numElems%numChunks;
  var mylow = r.low;
  if myChunk < rem {
    mylow += (elemsperChunk+1)*myChunk;
    return mylow..#(elemsperChunk + 1);
  } else {
    mylow += ((elemsperChunk+1)*rem + (elemsperChunk)*(myChunk-rem));
    return mylow..#elemsperChunk;
  }
}

// .. _User-Defined Parallel Zippered Iterators in Chapel: http://pgas11.rice.edu/papers/ChamberlainEtAl-Chapel-Iterators-PGAS11.pdf
