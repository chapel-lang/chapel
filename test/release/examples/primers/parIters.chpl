// Parallel Iterators

/*
  Parallel Iterators Primer
   
  This primer shows how to use parallel iterators in Chapel.
  Leader-follower iterators are used to implement zippered
  forall loops in Chapel over data structures or iterators.
  Standalone parallel iterators are used to implement non-zippered
  forall loops when they exist, falling back to leader-follower when
  standalone is not available. 

  For a more thorough introduction to leader-follower iterators,
  refer to our PGAS 2011 paper, `User-Defined Parallel Zippered
  Iterators in Chapel`_. We expect the parallel iterator interface
  to change and improve over time, so this should be considered a
  snapshot of their use in the current implementation.
*/

/*
.. primers-parIters-leader-follower:

Leader-follower
---------------
*/

// Any zippered forall loop in Chapel will be implemented using
// leader-follower iterators.  Generally speaking, a forall loop
// of the following form:

/* .. code-block:: chapel

      forall (a, b, c) in zip(A, B, C) do
         // ...loop body...
*/

// is semantically defined such that the first thing being iterated
// over -- in this case, ``A`` -- is designated the 'leader.'  All things
// being iterated over are 'followers' (so for this loop, ``A``,
// ``B``, and ``C`` would be).
//

/*
.. primers-parIters-semantics:

Semantics
---------
*/

// Given such a loop, the compiler will roughly translate it into
// the following loop structure:

/* .. code-block:: chapel

      for work in A.lead() do   // implemented by inlining the leader
        for (a, b, c) in zip(A.follow(work), B.follow(work), C.follow(work)) do
          // ...loop body...
*/

// where ``.lead()`` and ``.follow()`` represent the leader-follower iterators
// using a simplified naming scheme.
//
// Note that since Chapel's implicitly parallel semantics are defined
// in terms of zippered iteration, such cases are also implemented using
// leader-follower iterators.  For example, ``A = B + C;`` will be converted to
// an equivalent zippered parallel loop and then to the leader-follower idiom
// shown above. ``foo(A, B, C)`` goes through a similar transformation, where
// ``foo()`` is defined to take scalar arguments and is promoted in this call.

/*
.. primers-parIters-roles:

Roles
-----
*/

// At a high level, the role of a leader iterator is to:
//
//   a) create the parallel tasks used to implement the forall loop,
//   b) associate the parallel tasks with specific locales as required/desired.
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

/*
.. primers-parIters-count:

Example: count
--------------
*/

// For this example, we're going to create a simple iterator named
// ``count`` that will be able to be invoked in for or forall loops.
// ``count`` will be defined to take an argument ``n`` as input and an
// optional argument ``low`` (set to 1 by default), and it will yield ``n``
// integers starting with ``low``.
//
// We'll use the following config const ``numTasks`` to indicate the degree
// of parallelism to use in the leader to implement forall loops. By default
// we've set it to the tasking layer estimate of maximum parallelism on the
// current locale, but it can be overridden on the executable command-line
// using the ``--numTasks=<n>`` option.
//
config const numTasks = here.maxTaskPar;
if (numTasks < 1) then
  halt("numTasks must be a positive integer");

//
// If compiled with ``verbose`` set to ``true``, the leader and follower will
// print out some messages indicating what they're doing under the
// covers.
//
config param verbose = false;

//
// Declare a problem size for this test.  By default we use a small
// problem size to make the output readable.  Of course, to use the
// parallelism effectively you'd want to use a much larger problem
// size (override on the execution command-line using the
// ``--probSize=<n>`` option).
//
config const probSize = 15;
var A: [1..probSize] real;

//
// When defining a leader-follower iterator pair, our current
// implementation requires that you also define a serial iterator
// of the same name that yields the same type as the follower iterator.
// In this case, the serial iterator is simple:  We simply yield the
// integers ``low..low+n-1`` (computed using the count operator, ``#``):
//
iter count(n: int, low: int=1) {
  for i in low..#n do
    yield i;
}

//
// Here are some simple loops using this iterator to demonstrate
// it.  First we iterate over all indices in our problem size to
// initialize ``A``:
//
for i in count(probSize) do
  A[i] = i:real;

writeln("After serial initialization, A is:");
writeln(A);
writeln();

//
// Then we override the default value of low in order to negate
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

writeln("After reassigning A using zippering:");
writeln(A);
writeln();

/*
.. primers-parIters-leader

count: leader
-------------
*/

// The leader and follower iterators are defined as overloads of the
// serial version of the iterator, distinguished by an initial
// param argument of the built-in enumerated type ``iterKind``.  To
// invoke the leader iterator and differentiate it from the other
// overloads, the compiler will pass in the value ``iterKind.leader`` to
// this argument.  The author of the leader iterator should use a
// ``where`` clause to distinguish this overload from the others.  After
// this ``tag`` argument, the rest of the argument list should match
// that of the serial iterator exactly.  For our example, this means
// providing the same n and low arguments as before.
//
// The implementation of this leader iterator is relatively simple and
// static.  It uses a coforall loop to create a number of tasks equal
// to the number specified by our ``numTasks`` config const and then has
// each yield a subset of the total work to perform.
//
// We compute the work that a task should yield by calling into the
// ``computeChunk()`` helper function (defined at the bottom of this file)
// to compute its subrange of the range ``low..#n`` owned by the task,
// storing it in a variable called ``myIters.``
//
// To be a legal leader iterator, we could simply yield this range as
// a representation of the work we want the follower to perform.
// However, to support zippering our leader with follower iterators
// written by others, we typically take the convention of having
// iterators over 1D or dense rectangular index spaces yield tuples
// of ranges shifted to a 0-based coordinate system.  In this way, the
// leader-follower iterators have a common representation for the
// work even though each may use its own indexing system.  This
// permits, for example, arrays of the same size/shape to be zippered
// together even if they have different domains.
//
// To this end, rather than yielding subranges of ``low..#n``, we'll yield
// subranges of ``0..n-1`` and rely on the follower to shift it back to
// the original coordinate system.  For this reason, we translate the
// range by ``-low`` to shift it from low-based coordinates to 0-based
// coordinates; and then we make a 1-tuple out of it.
//
// Note the debugging output inserted into this iterator.  While
// learning about leader-follower iterators, it's useful to turn
// this debugging output on by compiling with ``-sverbose=true``
//
iter count(param tag: iterKind, n: int, low: int=1)
  where tag == iterKind.leader {

  if (verbose) then
    writeln("In count() leader, creating ", numTasks, " tasks");

  coforall tid in 0..#numTasks {
    const myIters = computeChunk(low..#n, tid, numTasks);
    const zeroBasedIters = myIters.translate(-low);

    if (verbose) then
      writeln("task ", tid, " owns ", myIters, " yielded as: ", zeroBasedIters);

    yield (zeroBasedIters,);
  }
}

//
// As mentioned at the outset, this leader is fairly static and
// simple.  More generally, a leader can introduce tasks more
// dynamically, partition work between the tasks more dynamically,
// etc.  See :mod:`DynamicIters` for some more interesting examples
// of leader iterators, including those that use dynamic partitioning.
//

/*
.. primers-parIters-follower

count: follower
---------------
*/

// The follower is another overload of the same iterator name, this
// time taking the iterKind.follower param enumeration as its first
// argument.  The next arguments should match the leader and serial
// iterators exactly again (so, ``n`` and ``low`` for our example).  The
// final argument must be called ``followThis`` which represents the data
// yielded by the leader (in our case, the 1-tuple of ranges).
//
// The goal of the follower is to do the iteration specified by the
// ``followThis`` argument, serially yielding the elements corresponding
// to those iterations.  In our case, this involves plucking the
// range back out of the tuple of ranges, and shifting it back to
// our low-based coordinate system.  We then use a standard for loop
// to iterate over that range and yield the corresponding indices.
// Followers, as the name suggests, tend not to be very sophisticated,
// and simply do what the leader tells them to.
//
// As with the leader, this follower has been authored to support
// debugging output when compiled with ``-sverbose=true``.
//
iter count(param tag: iterKind, n: int, low: int=1, followThis)
       where tag == iterKind.follower && followThis.size == 1 {
  const lowBasedIters = followThis(1).translate(low);

  if (verbose) then
    writeln("Follower received ", followThis, " as work chunk; shifting to ",
            lowBasedIters);

  for i in lowBasedIters do
    yield i;
}

/*
.. primers-parIters-standalone-parallel

count: standalone parallel
--------------------------
*/

// The standalone parallel iterator is another overload of the same name,
// taking the ``iterKind.standalone`` param enumeration as its first argument.
// The next arguments again match the serial iterator exactly. This iterator
// generates parallelism and yields single elements in the low-based
// coordinate system. The standalone parallel iterator is invoked in
// forall loops that are not zippered.  Because this iterator will not
// be zippered with others, it doesn't need to go to the trouble of
// zero-shifting indices and putting them into a 1-tuple.
//
// This iterator has also been authored to include debugging output when
// compiled with ``-sverbose=true``.
//
iter count(param tag: iterKind, n: int, low: int = 1)
       where tag == iterKind.standalone {
  if (verbose) then
    writeln("In count() standalone, creating ", numTasks, " tasks");
  coforall tid in 0..#numTasks {
    const myIters = computeChunk(low..#n, tid, numTasks);
    if (verbose) then
      writeln("task ", tid, " owns ", myIters);
    for i in myIters do
      yield i;
  }
}

/*
.. primers-parIters-usage

count: usage
------------
*/

// Now that we've defined leader-follower and standalone iterators, we can
// execute the same loops we did before, only this time using forall loops
// to make the execution parallel.  We start with some simple invocations
// as before.  In these invocations, the ``count()`` standalone parallel
// iterator is used since it is the only thing being iterated over (``A`` is
// being randomly accessed within the loop.)
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

//
// Zippered iteration is now a bit more interesting.  In this
// first loop, ``count()`` serves as the leader and follower while
// the ``A`` array is a follower.  This works because ``A`` is a
// rectangular array whose follower iterator accepts tuples of
// 0-based ranges like the ones ``count()``'s leader is yielding.
// If we were to have ``count()`` yield something else (like a raw
// subrange of ``low..#n``), then the two things could not be
// zippered correctly because they wouldn't be speaking the
// same language -- either in terms of the type of work being
// yielded (range vs. 1-tuple of range), nor the description of
// the work (low-based indices vs. 0-based indices).
//
forall (i, a) in zip(count(probSize), A) do
  a = i/10.0;

writeln("After reassigning A using parallel zippering:");
writeln(A);
writeln();

//
// We can also zipper in the opposite order, making ``A`` the leader,
// in which case ``count()`` no longer controls the degree of parallelism
// and work assignment since it is no longer the leader.  Instead,
// ``A``'s leader iterator (defined as part of its domain map) is invoked.
// For standard Chapel arrays and domain maps, these leader-follower
// iterators are controlled by the ``dataPar*`` configuration constants
// as described in doc/rst/usingchapel/executing.rst.
//
forall (a, i) in zip(A, count(probSize)) do
  a = i/100.0;

writeln("After reassigning A using parallel zippering and A as the leader:");
writeln(A);
writeln();

//
// Finally, as mentioned at the outset, operations that are equivalent
// to zippering also use leader-follower iterators, so for example
// the following whole-array assignment will use ``A``'s leader and
// ``count()``'s follower:
//
A = count(probSize, low=100);

writeln("After reassigning A using whole-array assignment:");
writeln(A);
writeln();

/*
.. primers-parIters-closing-notes

Closing notes
-------------
*/

// Chapel data types like records and classes can support iteration
// by defining iterator methods (invoked by name) or ``these()`` iterators
// which support iterating over variables of that type directly.  Such
// iterator methods can be overloaded to support leader-follower
// versions as well to permit parallel iteration over the variable.
//
// As mentioned at the outset, our leader-follower scheme has a number
// of changes planned for it such as interface improvements and better
// error checking.  We'll update this primer as we improve these features.
//
// Definitions of functions used above:
//
// This is a poor-man's partitioning algorithm.  It gives
// ``floor(numElements/NumChunks)`` work items to the first ``numChunks-1``
// chunks and the remainder to the last chunk.  For simplicity it only
// works for non-strided, default index type ranges.  More work would
// be required to generalize it for strided or unbounded ranges.
//
proc computeChunk(r: range, myChunk, numChunks) where r.stridable == false {
  const numElems = r.size;
  const elemsPerChunk = numElems/numChunks;
  const mylow = r.low + elemsPerChunk*myChunk;
  if (myChunk != numChunks - 1) {
    return mylow..#elemsPerChunk;
  } else {
    return mylow..r.high;
  }
}

// .. _User-Defined Parallel Zippered Iterators in Chapel: http://pgas11.rice.edu/papers/ChamberlainEtAl-Chapel-Iterators-PGAS11.pdf 
