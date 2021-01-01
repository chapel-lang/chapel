/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

  Iterators supporting vectorization without creating tasks.

  Data parallel constructs (such as ``forall`` loops) are implicitly
  vectorizable. If the ``--vectorize`` compiler flag is thrown the Chapel
  compiler will emit vectorization hints to the backend compiler, though the
  effects will vary based on the target compiler.

  In order to allow users to explicitly request vectorization, this prototype
  vectorizing iterator is being provided. Loops that invoke this iterator will
  be marked with vectorization hints, provided the ``--vectorize`` flag is
  thrown.

  This iterator is currently available for all Chapel programs and does not
  require a ``use`` statement to make it available. In future releases it will
  be moved to a standard module and will likely require a ``use`` statement to
  make it available.
 */
pragma "error mode fatal"
pragma "unsafe"
pragma "module included by default"
module VectorizingIterator {
  private use ChapelStandard;

  // DEV NOTES:
  //
  // 3 versions of the iterators exist for each iterKind: a ref and a val
  // iterator that take a single iterable and a version that takes a tuple of
  // iterables. The refness of a tuple of iterables is handled automatically by
  // tuple semantics, and it's only for the single iterable that we need an
  // explicit ref and non-ref version. It would be ideal if there was an easier
  // way to provide a wrapper iterator. Something to keep in mind of L/F 2.0?
  //
  // Note that no type checking is done on the argument since if a non-iterable
  // is called it will result in the same error message to the user anyways
  // since these are just wrapper iterators.
  //
  // Also note that we can currently rely on all parallel iterators having a
  // serial version as well since we always resolve the serial iterator.
  //
  // It would be nice to warn the user that the "zip" keyword isn't needed.
  // That's easy to do for the parallel case since the leader/follower would do
  // the warning, but there's no way without compiler involvement to do that
  // for the serial case that I can think of. Perhaps a new flag on the iter
  // such as "non-zipperable iterator"?

  //
  // serial versions.
  //

  /*
     Vectorize only "wrapper" iterator:

     This iterator wraps and vectorizes other iterators. It takes one or more
     iterables (an iterator or class/record with a these() iterator) and yields
     the same elements as the wrapped iterables.

     This iterator exists to provide a way to vectorize data parallel loops
     without invoking a parallel iterator with the goal of avoiding task
     creation for loops with small trip counts or where task creation isn't
     desirable.

     Data parallel operations in Chapel such as forall loops are
     order-independent. However, a forall is implemented in terms of either
     leader/follower or standalone iterators which typically create tasks.
     This iterator exists to allow vectorization of order-independent loops
     without requiring task creation. By using this wrapper iterator you are
     asserting that the loop is order-independent (and thus a candidate for
     vectorization) just as you are when using a forall loop.

     When invoked from a serial for loop, this iterator will simply mark your
     iterator(s) as order-independent. When invoked from a parallel forall loop
     this iterator will implicitly be order-independent because of the
     semantics of a forall, and additionally it will invoke the serial
     iterator instead of the parallel iterators. For instance:

     .. code-block:: chapel

         forall i in vectorizeOnly(1..10) do;
         for    i in vectorizeOnly(1..10) do;

     will both effectively generate:

     .. code-block:: c

         // this loop hinted as order-independent
         for (i=0; i<=10; i+=1) {}

     The ``vectorizeOnly`` iterator  automatically handles zippering, so the
     ``zip`` keyword is not needed. For instance, to vectorize:

     .. code-block:: chapel

         for (i, j) in zip(1..10, 1..10) do;

     simply write:

     .. code-block:: chapel

         for (i, j) in vectorizeOnly(1..10, 1..10) do;

     Note that the use of ``zip`` is not explicitly prevented, but all
     iterators being zipped must be wrapped by a ``vectorizeOnly`` iterator.
     Future releases may explicitly prevent the use ``zip`` with this iterator.
  */
  pragma "vectorize yielding loops"
  iter vectorizeOnly(iterables...) where singleValIter(iterables) {
    for i in iterables(0) do yield i;
  }

  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(iterables...) ref where singleRefIter(iterables) {
    for i in iterables(0) do yield i;
  }

  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(iterables...?numiterables) where numiterables > 1 {
    for i in zip((...iterables)) do yield i;
  }


  //
  // standalone versions
  //
  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(param tag: iterKind, iterables...)
    where tag == iterKind.standalone && singleValIter(iterables) {
    for i in iterables(0) do yield i;
  }

  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(param tag: iterKind, iterables...) ref
    where tag == iterKind.standalone && singleRefIter(iterables) {
    for i in iterables(0) do yield i;
  }

  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(param tag: iterKind, iterables...?numiterables)
    where tag == iterKind.standalone && numiterables > 1  {
    for i in zip((...iterables)) do yield i;
  }


  //
  // leader versions
  //
  pragma "no doc"
  iter vectorizeOnly(param tag: iterKind, iterables...)
    where tag == iterKind.leader && singleValIter(iterables) {
      yield iterables(0);
  }

  pragma "no doc"
  iter vectorizeOnly(param tag: iterKind, iterables...) ref
    where tag == iterKind.leader && singleRefIter(iterables) {
      yield iterables(0);
  }

  pragma "no doc"
  iter vectorizeOnly(param tag: iterKind, iterables...?numiterables)
    where tag == iterKind.leader && numiterables > 1  {
      yield iterables;
  }


  //
  // follower versions
  //
  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(param tag: iterKind, followThis, iterables...)
    where tag == iterKind.follower && singleValIter(iterables) {
      for i in iterables(0) do yield i;
  }

  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(param tag: iterKind, followThis, iterables...) ref
    where tag == iterKind.follower && singleRefIter(iterables) {
      for i in iterables(0) do yield i;
  }

  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(param tag: iterKind, followThis, iterables...?numiterables)
    where tag == iterKind.follower && numiterables > 1 {
    for i in zip((...iterables)) do yield i;
  }
}
