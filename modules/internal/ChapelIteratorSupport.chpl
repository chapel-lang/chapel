/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

// This comment is a lie for chpl-docs sake. It only applies to the
// vectorizeOnly iterators found at the bottom of this file.
/*
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
pragma "error mode fatal" // avoid compiler errors here
pragma "unsafe"
module ChapelIteratorSupport {
  private use ChapelStandard;
  private use Reflection;

  //
  // module support for iterators
  //
  pragma "no doc"
  pragma "allow ref" // needs to to return tuples with refs
  pragma "fn returns iterator"
  proc iteratorIndex(ic: _iteratorClass) {
    ic.advance();
    return ic.getValue();
  }

  pragma "no doc"
  pragma "expand tuples with values"  // needs to return tuples with refs
  pragma "fn returns iterator"
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

  pragma "no doc"
  proc iteratorIndexType(x) type {
   // If the result is a runtime type, we do not want to call iteratorIndex()
   // - to avoid ic.advance(). We can do this for an array or a domain.
   if (isArray(x)) {
    return x.eltType;
   } else if isDomain(x) {
    return if x.rank == 1 then x.idxType else x.rank * x.idxType;
   } else {
    pragma "no copy" var ic = _getIterator(x);
    pragma "no copy" var i = iteratorIndex(ic);
    _freeIterator(ic);
    return i.type;
   }
  }

  pragma "no doc"
  proc iteratorIndexTypeZip(xs...) type {
    proc iteratorIndexTypeZipHelp(param dim) type {
      if dim == xs.size then
        return (iteratorIndexType(xs(dim)),);
      else
        return (iteratorIndexType(xs(dim)),
                (...iteratorIndexTypeZipHelp(dim+1)));
    }
    return iteratorIndexTypeZipHelp(1);
  }

  proc iteratorToArrayElementType(type t:_iteratorRecord) type {
    // Todo: chpl__unref() may be unnecessary. Ensure this test passes:
    //   test/expressions/loop-expr/zip-arrays.chpl
    return chpl__unref(
      chpl_buildStandInRTT(__primitive("scalar promotion type", t)) );
  }

  //
  // The two chpl_buildStandInRTT(type) functions accept, at run time,
  // the _RuntimeTypeInfo for domType/arrType that is **uninitialized**.
  // They returns a fresh domain/array type of the same kind, whose
  // RTT is **initialized**. Important: no accessing the uninitialized RTTs.
  //
  // It took some acrobatics to get the domain's distribution type,
  // rank, idxType, stridable from 'domType', and the same plus
  // (even more acrobatics) eltType from 'arrType'.
  // Ideally we'd get them **directly** from domType/arrType.
  //

  pragma "unsafe"
  proc chpl_buildStandInRTT(type domType: domain) type
  { //
    // domType._instance has no runtime type, so accessing its type
    // does not execute any code at run time. So it does not access
    // any contents of domType's _RuntimeTypeInfo. This is good -
    // because the _RuntimeTypeInfo is uninitialized.
    //
    var instanceObj: __primitive("static field type", domType, "_instance");

    return chpl_buildStandInRTT(instanceObj);
  }

  pragma "unsafe"
  proc chpl_buildStandInRTT(type arrType: []) type
  {
    // Analogously to instanceObj in chpl_buildStandInRTT(domType).
    type arrInstType = __primitive("static field type", arrType, "_instance");
    type domInstType = __primitive("static field type", arrInstType, "dom");

    // No runtime types - no code is executed at run time here.
    var domInstance: domInstType;

    // This is a domain built from properly-initialized _RuntimeTypeInfo.
    pragma "no auto destroy"
    var standinDomain: chpl_buildStandInRTT(domInstance);

    var instanceObj: arrInstType;

    // Luckily, "static typeof" shields us from accessing the field
    // instanceObj.eltType at run time - even when it is a run-time type.
    // Without "static typeof", it would access a field of instanceObj
    // and crash because the latter, by design, is nil.
    //
    // If 'instanceEltType' is a runtime type, its _RuntimeTypeInfo
    // is uninitialized.
    type instanceEltType = __primitive("static typeof", instanceObj.eltType);

    return chpl__buildArrayRuntimeType(standinDomain,
                                       chpl_buildStandInRTT(instanceEltType));
  }

  //
  // When the argument is an _iteratorRecord, return the type of the array
  // that is created when this iterator is promoted to an array.
  // Its domain is ir._shape_ or an empty 1-d domain if there is no shape.
  // Its element type is given by the iterator's "scalar promotion type".
  //
  proc chpl_buildStandInRTT(type irType: _iteratorRecord) type
  {
    type shapeType = chpl_iteratorShapeStaticTypeOrNothing(irType);

    proc standinType() type {
      if shapeType == nothing {
        // shapeless case
        return domain(1);

      } else if isRange(shapeType) {
        // the shape is given by a range
        return domain(1);

    // The rest are pieces from chpl_buildStandInRTT(arrType).
      } else {
        // shapeful case

        // No runtime types - no code is executed at run time here.
        var domInstance: shapeType;

        // Verify that there are no runtime types so far.
        compilerAssert(!isDomain(domInstance) && !isArray(domInstance));

        return chpl_buildStandInRTT(domInstance);
      }
    }

    // This is a domain built using properly-initialized _RuntimeTypeInfo.
    pragma "no auto destroy"
    var standinDomain: standinType();

    return chpl__buildArrayRuntimeType(standinDomain,
      chpl_buildStandInRTT(__primitive("scalar promotion type", irType)) );
  }

  //
  // If the type is neither an array nor a domain, then there is no run-time
  // content - nothing that can be uninitialized. Use the type directly.
  //
  proc chpl_buildStandInRTT(type nonRTtype) type
  {
    return nonRTtype;
  }

  //
  // The following overloads accept BaseDom subclasses.
  // The argument is always 'nil', so we cannot get any
  // run-time types from it. So we create them from scratch.
  //
  proc chpl_buildStandInRTT(domInst) type
    where domInst.type <= unmanaged DefaultRectangularDom
  {
    // The only _RuntimeTypeInfo component for a domain type is
    // a BaseDist subclass. We use 'defaultDist' for that.
    // The other args are always compile-time only.
    return chpl__buildDomainRuntimeType(defaultDist, domInst.rank,
                                        domInst.idxType, domInst.stridable);
  }

  // Other kinds of arrays/domains are not supported.
  proc chpl_buildStandInRTT(domInst) type
  {
    if domInst.type <= unmanaged BaseDom then
      compilerError("for/forall/promoted expressions are not implemented when the elements are or contain non-DefaultRectangular domains or arrays");
    else
      compilerError("unexpected argument of type ", domInst.type:string, " for chpl_buildStandInRTT()");
  }

  iter chpl_trivialLeader() {
    yield 0;
  }

  inline proc chpl_computeIteratorShape(arg: []) {
    return chpl_computeIteratorShape(arg.domain);
  }
  inline proc chpl_computeIteratorShape(arg: domain) {
    return arg._instance;
  }
  inline proc chpl_computeIteratorShape(arg: range(?)) {
    return arg;
  }
  inline proc chpl_computeIteratorShape(arg: _iteratorRecord) {
    if chpl_iteratorHasShape(arg) then
      return arg._shape_;
    else {
      const myvoid = none; // workaround for #9152
      return myvoid;
    }
  }
  inline proc chpl_computeIteratorShape(arg) {
    // none of the above cases
    return none;
  }

  proc chpl_iteratorHasShape(ir: _iteratorRecord) param {
    if hasField(ir.type, "_shape_") then
      return ir._shape_.type != void;
    else
      return false;
  }
  inline proc chpl_iteratorHasDomainShape(ir: _iteratorRecord) param {
    if hasField(ir.type, "_shape_") then
      return isSubtype(ir._shape_.type, BaseDom);
    else
      return false;
  }
  inline proc chpl_iteratorHasRangeShape(ir: _iteratorRecord) param {
    if hasField(ir.type, "_shape_") then
      return isRange(ir._shape_.type);
    else
      return false;
  }

  // This is the static type of chpl_computeIteratorShape(ir).
  proc chpl_iteratorShapeStaticTypeOrNothing(type ir: _iteratorRecord) type
  {
    if hasField(ir, "_shape_") then
      return __primitive("static field type", ir, "_shape_");
    else
      return nothing;
  }

  proc chpl_iteratorFromForExpr(ir: _iteratorRecord) param {
    if Reflection.canResolveMethod(ir, "_fromForExpr_") then
      return ir._fromForExpr_;
    else
      return false;
  }
  proc chpl_iteratorFromForExpr(arg) param {
    // non-iterator-record cases are always parallel
    // Todo: what if it is an array or domain whose domain map
    // that does not provide parallel iterators?
    return false;
  }

  proc _iteratorRecord.writeThis(f) throws {
    var first: bool = true;
    for e in this {
      if !first then
        f <~> " ";
      else
        first = false;
      f <~> e;
    }
  }

  proc =(ref ic: _iteratorRecord, xs) {
    for (e, x) in zip(ic, xs) do
      e = x;
  }

  // TODO: replace use of iteratorIndexType?
  pragma "suppress lvalue error"
  proc =(ref ic: _iteratorRecord, x: iteratorIndexType(ic)) {
    for e in ic do
      e = x;
  }

  pragma "suppress lvalue error"
  pragma "no borrow convert" // e.g. iteration over tuple of owned
  // argument is const ref for e.g. for x in (someSharedThing1, someSharedThing2)
  inline proc _getIterator(const ref x) {
    return _getIterator(x.these());
  }

  proc _getIterator(type t) {
    return _getIterator(t.these());
  }

  inline proc _getIteratorZip(x) {
    return _getIterator(x);
  }

  inline proc _getIteratorZip(type t) {
    return _getIterator(t);
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

  inline proc _getIteratorZip(type t: _tuple) {
    inline proc _getIteratorZipInternal(type t: _tuple, param dim: int) {
      if dim == t.size then
        return (_getIterator(t(dim)),);
      else
        return (_getIterator(t(dim)), (..._getIteratorZipInternal(t, dim+1)));
    }
    if t.size == 1 then
      return _getIterator(t(1));
    else
      return _getIteratorZipInternal(t, 1);
  }

  inline proc _freeIterator(ic: _iteratorClass) {
    chpl_here_free(__primitive("cast_to_void_star", ic));
  }

  inline proc _freeIterator(x: _tuple) {
    for param i in 1..x.size do
      _freeIterator(x(i));
  }

  pragma "fn returns iterator"
  pragma "no implicit copy"
  inline proc _toLeader(ir: _iteratorRecord)
    where __primitive("has leader", ir)
    return chpl__autoCopy(__primitive("to leader", ir));

  pragma "suppress lvalue error"
  pragma "fn returns iterator"
  inline proc _toLeader(x)
    where !isSubtype(x.type, _iteratorRecord) && __primitive("has leader", x.these())
    return _toLeader(x.these());

  pragma "fn returns iterator"
  inline proc _toLeaderZip(x)
    where !isTuple(x) && Reflection.canResolve("_toLeader", x)
    return _toLeader(x);

  pragma "fn returns iterator"
  inline proc _toLeaderZip(x: _tuple)
    where Reflection.canResolve("_toLeader", x(1))
    return _toLeader(x(1));

  pragma "no implicit copy"
  pragma "fn returns iterator"
  inline proc _toStandalone(iterator: _iteratorClass)
    return chpl__autoCopy(__primitive("to standalone", iterator));

  pragma "fn returns iterator"
  inline proc _toStandalone(ir: _iteratorRecord) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var standalone = _toStandalone(ic);
    _freeIterator(ic);
    return standalone;
  }

  pragma "suppress lvalue error"
  pragma "fn returns iterator"
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
  pragma "fn returns iterator"
  inline proc _toLeader(ir: _iteratorRecord, args...) {
    return chpl__autoCopy(__primitive("to leader", ir, (...args)));
  }

  pragma "suppress lvalue error"
  pragma "expand tuples with values"
  pragma "fn returns iterator"
  inline proc _toLeader(x, args...)
    return _toLeader(x.these(), (...args));

  pragma "expand tuples with values"
  pragma "fn returns iterator"
  inline proc _toLeaderZip(x, args...)
    return _toLeader(x, (...args));

  pragma "expand tuples with values"
  pragma "fn returns iterator"
  inline proc _toLeaderZip(x: _tuple, args...)
    return _toLeader(x(1), (...args));

  pragma "no implicit copy"
  pragma "expand tuples with values"
  pragma "fn returns iterator"
  inline proc _toStandalone(iterator: _iteratorClass, args...)
    return chpl__autoCopy(__primitive("to standalone", iterator, (...args)));

  pragma "expand tuples with values"
  pragma "fn returns iterator"
  inline proc _toStandalone(ir: _iteratorRecord, args...) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var standalone = _toStandalone(ic, (...args));
    _freeIterator(ic);
    return standalone;
  }

  pragma "suppress lvalue error"
  pragma "expand tuples with values"
  pragma "fn returns iterator"
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
  pragma "fn returns iterator"
  inline proc _toFollower(iterator: _iteratorClass, leaderIndex)
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex));

  pragma "fn returns iterator"
  inline proc _toFollower(ir: _iteratorRecord, leaderIndex) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFollower(ic, leaderIndex);
    _freeIterator(ic);
    return follower;
  }

  pragma "suppress lvalue error"
  pragma "fn returns iterator"
  inline proc _toFollower(x, leaderIndex) {
    return _toFollower(x.these(), leaderIndex);
  }

  pragma "fn returns iterator"
  inline proc _toFollowerZip(x, leaderIndex) {
    return _toFollower(x, leaderIndex);
  }

  pragma "fn returns iterator"
  inline proc _toFollowerZip(x: _tuple, leaderIndex) {
    return _toFollowerZipInternal(x, leaderIndex, 1);
  }

  pragma "fn returns iterator"
  inline proc _toFollowerZipInternal(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size then
      return (_toFollower(x(dim), leaderIndex),);
    else
      return (_toFollower(x(dim), leaderIndex),
              (..._toFollowerZipInternal(x, leaderIndex, dim+1)));
  }

  pragma "no implicit copy"
  pragma "fn returns iterator"
  inline proc _toFastFollower(iterator: _iteratorClass, leaderIndex, fast: bool) {
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex, true));
  }

  pragma "fn returns iterator"
  inline proc _toFastFollower(ir: _iteratorRecord, leaderIndex, fast: bool) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFastFollower(ic, leaderIndex, fast=true);
    _freeIterator(ic);
    return follower;
  }

  pragma "fn returns iterator"
  inline proc _toFastFollower(x, leaderIndex) {
    if chpl__staticFastFollowCheck(x) then
      return _toFastFollower(_getIterator(x), leaderIndex, fast=true);
    else
      return _toFollower(_getIterator(x), leaderIndex);
  }

  pragma "fn returns iterator"
  inline proc _toFastFollowerZip(x, leaderIndex) {
    return _toFastFollower(x, leaderIndex);
  }

  pragma "fn returns iterator"
  inline proc _toFastFollowerZip(x: _tuple, leaderIndex) {
    return _toFastFollowerZip(x, leaderIndex, 1);
  }

  pragma "fn returns iterator"
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
  pragma "no doc"
  proc singleValIter(iterables: _tuple) param {
    return iterables.size == 1 && !isRefIter(_getIterator(iterables(1)));
  }

  pragma "no doc"
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

         CHPL_PRAGMA_IVDEP
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

  pragma "no doc"
  pragma "vectorize yielding loops"
  iter vectorizeOnly(iterables...) ref where singleRefIter(iterables) {
    for i in iterables(1) do yield i;
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
  iter vectorizeOnly(param tag: iterKind, iterables...)
    where tag == iterKind.standalone && singleValIter(iterables) {
    for i in iterables(1) do yield i;
  }

  pragma "no doc"
  iter vectorizeOnly(param tag: iterKind, iterables...) ref
    where tag == iterKind.standalone && singleRefIter(iterables) {
    for i in iterables(1) do yield i;
  }

  pragma "no doc"
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
      yield iterables(1);
  }

  pragma "no doc"
  iter vectorizeOnly(param tag: iterKind, iterables...) ref
    where tag == iterKind.leader && singleRefIter(iterables) {
      yield iterables(1);
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
  iter vectorizeOnly(param tag: iterKind, followThis, iterables...)
    where tag == iterKind.follower && singleValIter(iterables) {
      for i in iterables(1) do yield i;
  }

  pragma "no doc"
  iter vectorizeOnly(param tag: iterKind, followThis, iterables...) ref
    where tag == iterKind.follower && singleRefIter(iterables) {
      for i in iterables(1) do yield i;
  }

  pragma "no doc"
  iter vectorizeOnly(param tag: iterKind, followThis, iterables...?numiterables)
    where tag == iterKind.follower && numiterables > 1 {
    for i in zip((...iterables)) do yield i;
  }
}
