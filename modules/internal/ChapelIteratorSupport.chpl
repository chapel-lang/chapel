/*
 * Copyright 2004-2015 Cray Inc.
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

module ChapelIteratorSupport {
  //
  // module support for iterators
  //
  proc iteratorIndex(ic: _iteratorClass) {
    ic.advance();
    return ic.getValue();
  }

  pragma "expand tuples with values"
  proc iteratorIndex(t: _tuple) {
    pragma "expand tuples with values"
    proc iteratorIndexHelp(t: _tuple, param dim: int) {
      if dim == t.size then
        return _build_tuple_always_allow_ref(iteratorIndex(t(dim)));
      else
        return _build_tuple_always_allow_ref(iteratorIndex(t(dim)),
                                             (...iteratorIndexHelp(t, dim+1)));
    }

    return iteratorIndexHelp(t, 1);
  }

  proc iteratorIndexType(x) type {
    pragma "no copy" var ic = _getIterator(x);
    pragma "no copy" var i = iteratorIndex(ic);
    _freeIterator(ic);
    return i.type;
  }

  proc _iteratorRecord.writeThis(f: Writer) {
    var first: bool = true;
    for e in this {
      if !first then
        f.write(" ");
      else
        first = false;
      f.write(e);
    }
  }

  proc =(ref ic: _iteratorRecord, xs) {
    for (e, x) in zip(ic, xs) do
      e = x;
  }

  pragma "suppress lvalue error"
  proc =(ref ic: _iteratorRecord, x: iteratorIndexType(ic)) {
    for e in ic do
      e = x;
  }

  inline proc _getIterator(x) {
    return _getIterator(x.these());
  }

  inline proc _getIterator(ic: _iteratorClass)
    return ic;

  proc _getIterator(type t) {
    compilerError("cannot iterate over a type");
  }

  inline proc _getIteratorZip(x) {
    return _getIterator(x);
  }

  inline proc _getIteratorZip(x: _tuple) {
    inline proc _getIteratorZipInternal(x: _tuple, param dim: int) {
      if dim == x.size then
        return (_getIterator(x(dim)),);
      else
        return (_getIterator(x(dim)), (..._getIteratorZipInternal(x, dim+1)));
    }
    if x.size == 1 then
      return _getIterator(x(1));
    else
      return _getIteratorZipInternal(x, 1);
  }

  proc _checkIterator(type t) {
    compilerError("cannot iterate over a type");
  }

  inline proc _checkIterator(x) {
    return x;
  }

  inline proc _freeIterator(ic: _iteratorClass) {
    chpl_here_free(__primitive("cast_to_void_star", ic));
  }

  inline proc _freeIterator(x: _tuple) {
    for param i in 1..x.size do
      _freeIterator(x(i));
  }

  pragma "no implicit copy"
  inline proc _toLeader(iterator: _iteratorClass)
    return chpl__autoCopy(__primitive("to leader", iterator));

  inline proc _toLeader(ir: _iteratorRecord) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var leader = _toLeader(ic);
    _freeIterator(ic);
    return leader;
  }

  inline proc _toLeader(x)
    return _toLeader(x.these());

  inline proc _toLeaderZip(x)
    return _toLeader(x);

  inline proc _toLeaderZip(x: _tuple)
    return _toLeader(x(1));

  pragma "no implicit copy"
  inline proc _toStandalone(iterator: _iteratorClass)
    return chpl__autoCopy(__primitive("to standalone", iterator));

  inline proc _toStandalone(ir: _iteratorRecord) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var standalone = _toStandalone(ic);
    _freeIterator(ic);
    return standalone;
  }

  inline proc _toStandalone(x) {
    return _toStandalone(x.these());
  }


  //
  // additional _toLeader/_toLeaderZip for forall intents
  //
  // The extra args... are used to pass the outer variables of the
  // forall loop body, which are subject to forall intents.
  //

  pragma "no implicit copy"
  pragma "expand tuples with values"
  inline proc _toLeader(iterator: _iteratorClass, args...)
    return chpl__autoCopy(__primitive("to leader", iterator, (...args)));

  pragma "expand tuples with values"
  inline proc _toLeader(ir: _iteratorRecord, args...) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var leader = _toLeader(ic, (...args));
    _freeIterator(ic);
    return leader;
  }

  pragma "expand tuples with values"
  inline proc _toLeader(x, args...)
    return _toLeader(x.these(), (...args));

  pragma "expand tuples with values"
  inline proc _toLeaderZip(x, args...)
    return _toLeader(x, (...args));

  pragma "expand tuples with values"
  inline proc _toLeaderZip(x: _tuple, args...)
    return _toLeader(x(1), (...args));

  pragma "no implicit copy"
  pragma "expand tuples with values"
  inline proc _toStandalone(iterator: _iteratorClass, args...)
    return chpl__autoCopy(__primitive("to standalone", iterator, (...args)));

  pragma "expand tuples with values"
  inline proc _toStandalone(ir: _iteratorRecord, args...) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var standalone = _toStandalone(ic, (...args));
    _freeIterator(ic);
    return standalone;
  }

  pragma "expand tuples with values"
  inline proc _toStandalone(x, args...) {
    return _toStandalone(x.these(), (...args));
  }


  //
  // return true if any iterator supports fast followers
  //
  proc chpl__staticFastFollowCheck(x) param {
    pragma "no copy" const lead = x;
    if isDomain(lead) || isArray(lead) then
      return chpl__staticFastFollowCheck(x, lead);
    else
      return false;
  }

  proc chpl__staticFastFollowCheck(x, lead) param {
    return false;
  }

  proc chpl__staticFastFollowCheck(x: [], lead) param {
    return x._value.dsiStaticFastFollowCheck(lead._value.type);
  }

  proc chpl__staticFastFollowCheckZip(x: _tuple) param {
    pragma "no copy" const lead = x(1);
    if isDomain(lead) || isArray(lead) then
      return chpl__staticFastFollowCheckZip(x, lead);
    else
      return false;
  }

  proc chpl__staticFastFollowCheckZip(x, lead) param {
    return chpl__staticFastFollowCheck(x, lead);
  }

  proc chpl__staticFastFollowCheckZip(x: _tuple, lead, param dim = 1) param {
    if x.size == dim then
      return chpl__staticFastFollowCheckZip(x(dim), lead);
    else
      return chpl__staticFastFollowCheckZip(x(dim), lead) || chpl__staticFastFollowCheckZip(x, lead, dim+1);
  }

  //
  // return true if all iterators that support fast followers can use
  // their fast followers
  //
  proc chpl__dynamicFastFollowCheck(x) {
    return chpl__dynamicFastFollowCheck(x, x);
  }

  proc chpl__dynamicFastFollowCheck(x, lead) {
    return true;
  }

  proc chpl__dynamicFastFollowCheck(x: [], lead) {
    if chpl__staticFastFollowCheck(x, lead) then
      return x._value.dsiDynamicFastFollowCheck(lead);
    else
      return false;
  }

  proc chpl__dynamicFastFollowCheckZip(x: _tuple) {
    return chpl__dynamicFastFollowCheckZip(x, x(1));
  }

  proc chpl__dynamicFastFollowCheckZip(x, lead) {
    return chpl__dynamicFastFollowCheck(x, lead);
  }

  proc chpl__dynamicFastFollowCheckZip(x: _tuple, lead, param dim = 1) {
    if x.size == dim then
      return chpl__dynamicFastFollowCheckZip(x(dim), lead);
    else
      return chpl__dynamicFastFollowCheckZip(x(dim), lead) && chpl__dynamicFastFollowCheckZip(x, lead, dim+1);
  }

  pragma "no implicit copy"
  inline proc _toFollower(iterator: _iteratorClass, leaderIndex)
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex));

  inline proc _toFollower(ir: _iteratorRecord, leaderIndex) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFollower(ic, leaderIndex);
    _freeIterator(ic);
    return follower;
  }

  inline proc _toFollower(x, leaderIndex) {
    return _toFollower(x.these(), leaderIndex);
  }

  inline proc _toFollowerZip(x, leaderIndex) {
    return _toFollower(x, leaderIndex);
  }

  inline proc _toFollowerZip(x: _tuple, leaderIndex) {
    return _toFollowerZipInternal(x, leaderIndex, 1);
  }

  inline proc _toFollowerZipInternal(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size then
      return (_toFollower(x(dim), leaderIndex),);
    else
      return (_toFollower(x(dim), leaderIndex),
              (..._toFollowerZipInternal(x, leaderIndex, dim+1)));
  }

  pragma "no implicit copy"
  inline proc _toFastFollower(iterator: _iteratorClass, leaderIndex, fast: bool) {
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex, true));
  }

  inline proc _toFastFollower(ir: _iteratorRecord, leaderIndex, fast: bool) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFastFollower(ic, leaderIndex, fast=true);
    _freeIterator(ic);
    return follower;
  }

  pragma "no implicit copy"
  inline proc _toFastFollower(iterator: _iteratorClass, leaderIndex) {
    return _toFollower(iterator, leaderIndex);
  }

  inline proc _toFastFollower(ir: _iteratorRecord, leaderIndex) {
    return _toFollower(ir, leaderIndex);
  }

  inline proc _toFastFollower(x, leaderIndex) {
    if chpl__staticFastFollowCheck(x) then
      return _toFastFollower(x.these(), leaderIndex, fast=true);
    else
      return _toFollower(x.these(), leaderIndex);
  }

  inline proc _toFastFollowerZip(x, leaderIndex) {
    return _toFastFollower(x, leaderIndex);
  }

  inline proc _toFastFollowerZip(x: _tuple, leaderIndex) {
    return _toFastFollowerZip(x, leaderIndex, 1);
  }

  inline proc _toFastFollowerZip(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size-1 then
      return (_toFastFollowerZip(x(dim), leaderIndex),
              _toFastFollowerZip(x(dim+1), leaderIndex));
    else
      return (_toFastFollowerZip(x(dim), leaderIndex),
              (..._toFastFollowerZip(x, leaderIndex, dim+1)));
  }



  // helper functions used by the below iterators to check if the argument is a
  // value or reference iterator.
  proc singleValIter(iterables: _tuple) param {
    return iterables.size == 1 && !isRefIter(_getIterator(iterables(1)));
  }

  proc singleRefIter(iterables: _tuple) param  {
    return iterables.size == 1 && isRefIter(_getIterator(iterables(1)));
  }

  /*
     Vectorize only "wrapper" iterator:

     This iterator wraps and vectorizes other iterators. It takes one or more
     iterables (an iterator or class/record with a these() iterator) and yields
     the same elements as the wrapped iterables.

     This iterator exists to provide a way to vectorize data parallel loops
     without invoking a parallel iterator with the goal of avoiding task
     creation for loops with small trip counts or where task creation isn't
     desirable.

     Data parallel operations in Chapel such as forall loops are order
     independent. However, a forall is implemented in terms of either
     leader/follower or standalone iterators which typically create tasks.
     This iterator exists to allow vectorization of order independent loops
     without requiring task creation. By using this wrapper iterator you are
     asserting that the loop is order independent (and thus a candidate for
     vectorization) just as you are when using a forall loop.

     When invoked from a serial for loop, this iterator will simply mark your
     iterator(s) as order independent. When invoked from a parallel forall loop
     this iterator will implicitly be order independent because of the
     semantics of a forall, and additionally it will invoke the serial
     iterator instead of the parallel iterators. For instance:

       `forall i in vectorizeOnly(1..10) do;`
       `for    i in vectorizeOnly(1..10) do;`

     will both effectively generate:

       `CHPL_PRAGMA_IVDEP`
       `for (i=0; i<=10; i+=1) {}`

     The vectorizeOnly automatically handles zippering, so the "zip" keyword is
     not needed. For instance, to vectorize:

       `for (i, j) in zip(1..10, 1..10) do;`

     simply write:

       `for (i, j) in vectorizeOnly(1..10, 1..10) do;`
  */

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
  pragma "vectorize yielding loops"
  iter vectorizeOnly(iterables...) where singleValIter(iterables) {
    for i in iterables(1) do yield i;
  }

  pragma "vectorize yielding loops"
  iter vectorizeOnly(iterables...) ref where singleRefIter(iterables) {
    for i in iterables(1) do yield i;
  }

  pragma "vectorize yielding loops"
  iter vectorizeOnly(iterables...?numiterables) where numiterables > 1 {
    for i in zip((...iterables)) do yield i;
  }


  //
  // standalone versions
  //
  iter vectorizeOnly(param tag: iterKind, iterables...)
    where tag == iterKind.standalone && singleValIter(iterables) {
    for i in iterables(1) do yield i;
  }

  iter vectorizeOnly(param tag: iterKind, iterables...) ref
    where tag == iterKind.standalone && singleRefIter(iterables) {
    for i in iterables(1) do yield i;
  }

  iter vectorizeOnly(param tag: iterKind, iterables...?numiterables)
    where tag == iterKind.standalone && numiterables > 1  {
    for i in zip((...iterables)) do yield i;
  }


  //
  // leader versions
  //
  iter vectorizeOnly(param tag: iterKind, iterables...)
    where tag == iterKind.leader && singleValIter(iterables) {
      yield iterables(1);
  }

  iter vectorizeOnly(param tag: iterKind, iterables...) ref
    where tag == iterKind.leader && singleRefIter(iterables) {
      yield iterables(1);
  }

  iter vectorizeOnly(param tag: iterKind, iterables...?numiterables)
    where tag == iterKind.leader && numiterables > 1  {
      yield iterables;
  }


  //
  // follower versions
  //
  iter vectorizeOnly(param tag: iterKind, followThis, iterables...)
    where tag == iterKind.follower && singleValIter(iterables) {
      for i in iterables(1) do yield i;
  }

  iter vectorizeOnly(param tag: iterKind, followThis, iterables...) ref
    where tag == iterKind.follower && singleRefIter(iterables) {
      for i in iterables(1) do yield i;
  }

  iter vectorizeOnly(param tag: iterKind, followThis, iterables...?numiterables)
    where tag == iterKind.follower && numiterables > 1 {
    for i in zip((...iterables)) do yield i;
  }
}
