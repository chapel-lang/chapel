/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

/* Support for round-robin distribution of domains/arrays to target locales. */

module CyclicDist {

private use DSIUtil;
private use ChapelLocks;

proc _determineRankFromStartIdx(startIdx) param {
  return if isTuple(startIdx) then startIdx.size else 1;
}

proc _determineIdxTypeFromStartIdx(startIdx) type {
  return if isTuple(startIdx) then startIdx(0).type else startIdx.type;
}

config param debugCyclicDist = false;
config param verboseCyclicDistWriters = false;
config param debugCyclicDistBulkTransfer = false;

private config param allowDuplicateTargetLocales = false;
//
// If the testFastFollowerOptimization flag is set to true, the
// follower will write output to indicate whether the fast follower is
// used or not.  This is used in regression testing to ensure that the
// 'fast follower' optimization is working.
//
config param testFastFollowerOptimization = false;

//
// This flag is used to disable lazy initialization of the RAD cache.
//
config param disableCyclicLazyRAD = defaultDisableLazyRADOpt;

// chpldoc TODO:
//   a good reference to
//     dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity
//   supports RAD opt, Bulk Transfer optimization, localSubdomain
//   disableCyclicLazyRAD
//
/*

The ``cyclicDist`` distribution uses a round-robin partitioning to map
d-dimensional indices to a d-dimensional array of locales, starting
from a given index.

.. Warning::

  The ``cyclicDist`` distribution was, until recently, a class named
  ``Cyclic``.  Today, ``Cyclic`` is still supported in a deprecated
  form, yet is an alias to the ``cyclicDist`` record here.  In our
  experience, most uses of ``Cyclic`` in distribution contexts should
  continue to work, but updating to ``cyclicDist`` is requested going
  forward due to the deprecation.

More precisely, for a ``cyclicDist`` distribution with:

  =============  ====================================================
  rank           ``d``
  start index    ``(s_1, ...., s_d)``
  over locales   ``targetLocales: [0..<N_1, ...., 0..<N_d] locale``
  =============  ====================================================

Each index ``(i_1, ...., i_d)``
is mapped to the locale ``targetLocales[j_1, ...., j_d]``
where, for each ``k`` in ``1..d``,
we have:

  ``j_k = (i_k - s_k) (mod N_k)``


**Example**

The following code declares a domain ``D`` distributed
using a ``cyclicDist`` distribution with a start index of ``(1,1)``,
and declares an array ``A`` over that domain.
The `forall` loop sets each array element
to the ID of the locale to which it is mapped.

  .. code-block:: chapel

    use CyclicDist;

    const Dist = new cyclicDist(startIdx=(1,1));
    const D = Dist.createDomain({1..8, 1..8});
    var A: [D] int;

    forall a in A do
      a = here.id;

    writeln(A);

When run on 6 locales, the output is:

  ::

    0 1 0 1 0 1 0 1
    2 3 2 3 2 3 2 3
    4 5 4 5 4 5 4 5
    0 1 0 1 0 1 0 1
    2 3 2 3 2 3 2 3
    4 5 4 5 4 5 4 5
    0 1 0 1 0 1 0 1
    2 3 2 3 2 3 2 3


**Data-Parallel Iteration**

As demonstrated by the above example, a `forall` loop over a
``cyclicDist``-distributed domain or array executes each iteration on
the locale owning the index in question.

By default, parallelism within each locale is applied to that locale's
local, strided block of indices by creating a task for each available
processor core (or the number of local indices if it is less than the
number of cores). The local domain indices are then statically divided
as evenly as possible between those tasks.

**Initializer Arguments**

The ``cyclicDist`` initializer is defined as follows:

  .. code-block:: chapel

    proc cyclicDist.init(
      startIdx,
      targetLocales: [] locale = Locales)

The argument ``startIdx`` is a tuple of integers defining an index that
will be distributed to the first locale in ``targetLocales``.
In the 1-dimensional case, ``startIdx`` can be an integer
or a tuple with a single element.

The argument ``targetLocales`` is an array containing the target
locales to which this distribution maps indices and data.
The rank of ``targetLocales`` must match the rank of the distribution,
or be ``1``.  If the rank of ``targetLocales`` is ``1``, a greedy
heuristic is used to reshape the array of target locales so that it
matches the rank of the distribution and each dimension contains an
approximately equal number of indices.

**Convenience Factory Methods**

It is common for a ``cyclicDist``-distributed domain or array to use
its first index as the start Index in a Cyclic distribution.  It is
also common not to override any of the other defaulted initializer
arguments.  In such cases, factory methods are provided for
convenience.

These methods take a domain or series of ranges as arguments and
return a cyclic-distributed domain or array.  For example, the
following declarations create new ``5 x 5`` cyclic-distributed domains
and arrays using `(1, 1)` as the starting index:

  .. code-block:: chapel

    use CyclicDist;

    var CyclicDom1 = cyclicDist.createDomain({1..5, 1..5});
    var CyclicArr1 = cyclicDist.createArray({1..5, 1..5}, real);
    var CyclicDom2 = cyclicDist.createDomain(1..5, 1..5);
    var CyclicArr2 = cyclicDist.createArray(1..5, 1..5, real);

The helper methods on ``Cyclic`` have the following signatures:

  .. function:: proc type cyclicDist.createDomain(dom: domain(?), targetLocales = Locales)

    Create a cyclic-distributed domain. The lower bounds of the domain are used
    as the starting indices.

  .. function:: proc type cyclicDist.createDomain(rng: range(?)..., targetLocales = Locales)

    Create a cyclic-distributed domain from a series of ranges. The lower
    bounds of the ranges are used as the starting indices.

  .. function:: proc type cyclicDist.createArray(dom: domain(?), type eltType, targetLocales = Locales)

    Create a default-initialized cyclic-distributed array whose indices
    match those of the given domain.

  .. function:: proc type cyclicDist.createArray(rng: range(?)..., type eltType, targetLocales = Locales)

    Create a default-initialized cyclic-distributed array using a
    domain constructed from the series of ranges.

  .. function:: proc type cyclicDist.createArray(dom: domain(?), type eltType, initExpr, targetLocales = Locales)

    Create a cyclic-distributed array whose indices match those of the
    given domain.

    The array's values are initialized using ``initExpr`` which can be any of
    the following:

    * a value coercible to ``eltType`` — all elements of the array will be
      assigned with this value
    * an iterator expression with compatible size and type — the array elements
      will be initialized with the values yielded by the iterator
    * an array of compatible size and type — the array will be assigned into
      the distributed array

  .. function:: proc type cyclicDist.createArray(rng: range(?)..., type eltType, initExpr, targetLocales = Locales)

    Create a cyclic-distributed array using a domain constructed from
    the series of ranges.

    The array's values are initialized using ``initExpr`` which can be any of
    the following:

    * a value coercible to ``eltType`` — all elements of the array will be
      assigned with this value
    * an iterator expression with compatible size and type — the array elements
      will be initialized with the values yielded by the iterator
    * an array of compatible size and type — the array will be assigned into
      the distributed array

  .. function:: proc cyclicDist.createDomain(dom: domain(?))

    Create a cyclic-distributed domain over an existing ``cyclicDist`` by copying
    the index space from the passed domain.

  .. function:: proc cyclicDist.createDomain(rng: range(?)...)

    Create a cyclic-distributed domain from a series of ranges over an existing
    ``cyclicDist``.

**Limitations**

This distribution has not been tuned for performance.
*/

pragma "ignore noinit"
record cyclicDist : writeSerializable {
  param rank: int;
  type idxType = int;


  forwarding const chpl_distHelp: chpl_PrivatizedDistHelper(unmanaged CyclicImpl(rank, idxType));

  pragma "last resort"
  @unstable("passing arguments other than 'boundingBox' and 'targetLocales' to 'cyclicDist' is currently unstable")
  proc init(startIdx,
            targetLocales: [] locale = Locales,
            dataParTasksPerLocale=getDataParTasksPerLocale(),
            dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
            dataParMinGranularity=getDataParMinGranularity(),
            param rank = _determineRankFromStartIdx(startIdx),
            type idxType = _determineIdxTypeFromStartIdx(startIdx))
    where isTuple(startIdx) || isIntegral(startIdx)
  {
    const value = new unmanaged CyclicImpl(startIdx, targetLocales,
                                           dataParTasksPerLocale,
                                           dataParIgnoreRunningTasks,
                                           dataParMinGranularity,
                                           rank, idxType
                                          );
    this.rank = rank;
    this.idxType = idxType;

    this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                          if _isPrivatized(value)
                            then _newPrivatizedClass(value)
                            else nullPid,
                          value);
  }

  proc init(startIdx,
            targetLocales: [] locale = Locales)
    where isTuple(startIdx) || isIntegral(startIdx)
  {
    this.init(startIdx, targetLocales,
              /* by specifying even one unstable argument, this should select
                 the whole unstable constructor, which has defaults for everything
                 else. */
              dataParTasksPerLocale=getDataParTasksPerLocale());
  }

  proc init(_pid : int, _instance, _unowned : bool) {
    this.rank = _instance.rank;
    this.idxType = _instance.idxType;

    this.chpl_distHelp = new chpl_PrivatizedDistHelper(_pid,
                                                       _instance,
                                                       _unowned);
  }

  proc init(value) {
    this.rank = value.rank;
    this.idxType = value.idxType;

    this.chpl_distHelp = new chpl_PrivatizedDistHelper(
                           if _isPrivatized(value)
                             then _newPrivatizedClass(value)
                             else nullPid,
                           _to_unmanaged(value));
  }

  // Note: This does not handle the case where the desired type of 'this'
  // does not match the type of 'other'. That case is handled by the compiler
  // via coercions.
  proc init=(const ref other : cyclicDist(?)) {
    this.init(other._value.dsiClone());
  }

  proc clone() {
    return new cyclicDist(this._value.dsiClone());
  }

  @chpldoc.nodoc
  inline operator ==(d1: cyclicDist(?), d2: cyclicDist(?)) {
    if (d1._value == d2._value) then
      return true;
    return d1._value.dsiEqualDMaps(d2._value);
  }

  @chpldoc.nodoc
  inline operator !=(d1: cyclicDist(?), d2: cyclicDist(?)) {
    return !(d1 == d2);
  }

  proc serialize(writer, ref serializer) throws {
    chpl_distHelp.serialize(writer, serializer);
  }
}


@chpldoc.nodoc
@unstable(category="experimental", reason="assignment between distributions is currently unstable due to lack of testing")
operator =(ref a: cyclicDist(?), b: cyclicDist(?)) {
  if a._value == nil {
    __primitive("move", a, chpl__autoCopy(b.clone(), definedConst=false));
  } else {
    if a._value.type != b._value.type then
      compilerError("type mismatch in distribution assignment");
    if a._value == b._value {
      // do nothing
    } else
        a._value.dsiAssign(b._value);
    if _isPrivatized(a._instance) then
      _reprivatize(a._value);
  }
}

@deprecated("'Cyclic' is deprecated, please use 'cyclicDist' instead")
type Cyclic = cyclicDist;

@chpldoc.nodoc
class CyclicImpl: BaseDist, writeSerializable {
  param rank: int;
  type idxType = int;

  const targetLocDom: domain(rank);
  var targetLocs: [targetLocDom] locale;
  var startIdx: rank*idxType;

  var locDist: [targetLocDom] unmanaged LocCyclic(rank, idxType);

  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;

  proc init(startIdx,
            targetLocales: [] locale = Locales,
            dataParTasksPerLocale=getDataParTasksPerLocale(),
            dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
            dataParMinGranularity=getDataParMinGranularity(),
            param rank: int = _determineRankFromStartIdx(startIdx),
            type idxType = _determineIdxTypeFromStartIdx(startIdx))
    where isTuple(startIdx) || isIntegral(startIdx)
  {
    this.rank = rank;
    this.idxType = idxType;

    const ranges = setupTargetLocRanges(rank, targetLocales);
    this.targetLocDom = {(...ranges)};
    this.targetLocs = reshape(targetLocales, this.targetLocDom);
    if !allowDuplicateTargetLocales {
      var checkArr: [LocaleSpace] bool;
      for loc in targetLocs {
        if checkArr[loc.id] then
          halt("CyclicDist does not allow duplicate targetLocales");
        checkArr[loc.id] = true;
      }
    }
    var startIdxTemp: rank*idxType;
    for param i in 0..rank-1 {
      const startIdxI = if isTuple(startIdx) then startIdx(i) else startIdx;
      startIdxTemp(i) = chpl__mod(startIdxI, targetLocDom.dim(i).sizeAs(int));
    }
    this.startIdx = startIdxTemp;

    // Instead of 'dummyLC', we could give 'locDistTemp' a nilable element type.
    const dummyLC = new unmanaged LocCyclic(rank, idxType, dummy=true);
    var locDistTemp: [targetLocDom] unmanaged LocCyclic(rank, idxType)
          = dummyLC;
    coforall locid in targetLocDom with (ref locDistTemp) do
      on targetLocs(locid) do
       locDistTemp(locid) =
         new unmanaged LocCyclic(rank, idxType, locid, startIdxTemp, ranges);

    delete dummyLC;
    this.locDist = locDistTemp;

    // NOTE: When these knobs stop using the global defaults, we will need
    // to add checks to make sure dataParTasksPerLocale<0 and
    // dataParMinGranularity<0
    this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                                 then here.maxTaskPar
                                 else dataParTasksPerLocale;
    this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
    this.dataParMinGranularity = dataParMinGranularity;

    init this;

    if debugCyclicDist then
      for loc in locDist do writeln(loc);
  }

  proc dsiAssign(other: this.type) {
    coforall locid in targetLocDom do
      on targetLocs(locid) do
        delete locDist(locid);
    startIdx = other.startIdx;
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    dataParTasksPerLocale = other.dataParTasksPerLocale;
    dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
    dataParMinGranularity = other.dataParMinGranularity;
    coforall locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new unmanaged LocCyclic(rank, idxType, locid, this);
  }

  proc dsiEqualDMaps(that: CyclicImpl(?)) {
    return (this.startIdx == that.startIdx &&
            this.targetLocs.equals(that.targetLocs));
  }

  proc dsiEqualDMaps(that) param {
    return false;
  }

  proc dsiClone() {
    return new unmanaged CyclicImpl(startIdx, targetLocs,
                      dataParTasksPerLocale,
                      dataParIgnoreRunningTasks,
                      dataParMinGranularity);
  }

  override proc dsiDestroyDist() {
    coforall ld in locDist {
      on ld do
        delete ld;
    }
  }

}

proc CyclicImpl.chpl__locToLocIdx(loc: locale) {
  for locIdx in targetLocDom do
    if (targetLocs[locIdx] == loc) then
      return (true, locIdx);
  return (false, targetLocDom.first);
}

proc CyclicImpl.getChunk(inds, locid) {
  const chunk = locDist(locid).myChunk((...inds.getIndices()));
  return chunk;
}

override proc CyclicImpl.dsiDisplayRepresentation() {
  writeln("startIdx = ", startIdx);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocs = ", for tl in targetLocs do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

override proc CyclicDom.dsiSupportsAutoLocalAccess() param { return true; }

proc CyclicImpl.init(other: CyclicImpl, privateData,
                 param rank = other.rank,
                 type idxType = other.idxType) {
  this.rank = rank;
  this.idxType = idxType;
  targetLocDom = {(...privateData[1])};
  targetLocs = other.targetLocs;
  startIdx = privateData[0];
  locDist = other.locDist;
  dataParTasksPerLocale = privateData[2];
  dataParIgnoreRunningTasks = privateData[3];
  dataParMinGranularity = privateData[4];
}

override proc CyclicImpl.dsiSupportsPrivatization() param do return true;

proc CyclicImpl.dsiGetPrivatizeData() do return (startIdx,
                                          targetLocDom.dims(),
                                          dataParTasksPerLocale,
                                          dataParIgnoreRunningTasks,
                                          dataParMinGranularity);

proc CyclicImpl.dsiPrivatize(privatizeData) {
  return new unmanaged CyclicImpl(_to_unmanaged(this), privatizeData);
}

proc CyclicImpl.dsiGetReprivatizeData() do return 0;

proc CyclicImpl.dsiReprivatize(other, reprivatizeData) {
  targetLocDom = other.targetLocDom;
  targetLocs = other.targetLocs;
  locDist = other.locDist;
  startIdx = other.startIdx;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}

override proc CyclicImpl.dsiNewRectangularDom(param rank: int, type idxType, param strides: strideKind, inds) {
  if idxType != this.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Cyclic domain rank does not match distribution's");
  const whole = createWholeDomainForInds(rank, idxType, strides, inds);

  const dummyLCD = new unmanaged LocCyclicDom(rank, idxType);
  var locDomsTemp: [this.targetLocDom] unmanaged LocCyclicDom(rank, idxType)
        = dummyLCD;
  coforall localeIdx in this.targetLocDom with (ref locDomsTemp) do
    on this.targetLocs(localeIdx) do
      locDomsTemp(localeIdx) = new unmanaged LocCyclicDom(rank, idxType,
             this.getChunk(whole, localeIdx): myBlockType(rank, idxType));
  delete dummyLCD;

  var dom = new unmanaged CyclicDom(rank, idxType, strides,
                                    this:unmanaged, locDomsTemp, whole);
  return dom;
}

//
// Given a tuple of scalars of type t or range(t) match the shape but
// using types rangeType and scalarType e.g. the call:
// _matchArgsShape(range(int(32)), int(32), (1:int(64), 1:int(64)..5, 1:int(64)..5))
// returns the type: (int(32), range(int(32)), range(int(32)))
//
proc _cyclic_matchArgsShape(type rangeType, type scalarType, args) type {
  proc helper(param i: int) type {
    if i == args.size-1 {
      if isCollapsedDimension(args(i)) then
        return (scalarType,);
      else
        return (rangeType,);
    } else {
      if isCollapsedDimension(args(i)) then
        return (scalarType, (... helper(i+1)));
      else
        return (rangeType, (... helper(i+1)));
    }
  }
  return helper(0);
}

override proc CyclicImpl.serialize(writer, ref serializer) throws {
  writer.writeln("cyclicDist");
  writer.writeln("----------");
  for locid in targetLocDom do
    writer.writeln(" [", locid, "=", targetLocs(locid), "] owns chunk: ",
      locDist(locid).myChunk);
}

proc CyclicImpl.targetLocsIdx(i: idxType) {
  const numLocs:idxType = targetLocDom.sizeAs(idxType);
  // this is wrong if i is less than startIdx
  //return ((i - startIdx(0)) % numLocs):int;
  // this works even if i is less than startIdx
  return chpl__diffMod(i, startIdx(0), numLocs):idxType;
}

proc CyclicImpl.targetLocsIdx(ind: rank*idxType) {
  var x: rank*int;
  for param i in 0..rank-1 {
    var dimLen = targetLocDom.dim(i).sizeAs(int);
    //x(i) = ((ind(i) - startIdx(i)) % dimLen):int;
    x(i) = chpl__diffMod(ind(i), startIdx(i), dimLen):int;
  }
  if rank == 1 then
    return x(0);
  else
    return x;
}

proc CyclicImpl.dsiIndexToLocale(i: idxType) where rank == 1 {
  return targetLocs(targetLocsIdx(i));
}

proc CyclicImpl.dsiIndexToLocale(i: rank*idxType) {
  return targetLocs(targetLocsIdx(i));
}

  proc chpl__computeCyclicDim(type idxType, lo, myloc, numlocs) {
    const lower = min(idxType)..(lo+myloc) by -numlocs;
    const upper = lo+myloc..max(idxType) by numlocs;
    return lower.last..upper.last by numlocs:uint;
  }

proc chpl__computeCyclic(type idxType, locid, targetLocBox, startIdx) {
    type strType = chpl__signedType(idxType);
    param rank = targetLocBox.size;
    var inds: rank*range(idxType, strides=strideKind.positive);
    for param i in 0..rank-1 {
      // NOTE: Not bothering to check to see if these can fit into idxType
      const lo = chpl__tuplify(startIdx)(i): idxType;
      const myloc = chpl__tuplify(locid)(i): idxType;
      // NOTE: Not checking for overflow here when casting to strType
      const numlocs = targetLocBox(i).sizeAs(strType);
      inds(i) = chpl__computeCyclicDim(idxType, lo, myloc, numlocs);
    }
    return inds;
  }

class LocCyclic {
  param rank: int;
  type idxType;

  const myChunk: domain(rank, idxType, strideKind.positive);

  proc init(param rank, type idxType, locid,
            distStartIdx: rank*idxType, distLocDims) {
    this.rank = rank;
    this.idxType = idxType;

    var locidx: rank*idxType;
    var startIdx = distStartIdx;

    // NOTE: Not bothering to check to see if these can fit into idxType
    if rank == 1 then
      locidx(0) = locid:idxType;
    else
      for param i in 0..rank-1 do locidx(i) = locid(i):idxType;

    var inds: rank*range(idxType, strides=strideKind.positive);

    inds = chpl__computeCyclic(idxType, locid, distLocDims, startIdx);
    myChunk = {(...inds)};
  }

  // Used to create a dummy instance.
  proc init(param rank, type idxType, param dummy: bool) where dummy {
    this.rank = rank;
    this.idxType = idxType;
  }
}


class CyclicDom : BaseRectangularDom(?) {
  const dist: unmanaged CyclicImpl(rank, idxType);

  var locDoms: [dist.targetLocDom] unmanaged LocCyclicDom(rank, idxType);

  var whole: domain(rank, idxType, strides);
}

proc CyclicDom.setup() {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) {
        var chunk = dist.getChunk(whole, localeIdx);
        locDoms(localeIdx).myBlock = chunk;
      }
    }
}

proc CyclicDom.dsiGetDist() {
  if _isPrivatized(dist) then
    return new cyclicDist(dist.pid, dist, _unowned=true);
  else
    return new cyclicDist(nullPid, dist, _unowned=true);
}

override proc CyclicDom.dsiDestroyDom() {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) do
        delete locDoms(localeIdx);
    }
}

proc CyclicDom.dsiBuildArray(type eltType, param initElts:bool) {
  const dom = this;
  const creationLocale = here.id;
  const dummyLCD = new unmanaged LocCyclicDom(rank, idxType);
  const dummyLCA = new unmanaged LocCyclicArr(eltType, rank, idxType,
                                              dummyLCD, false);
  var locArrTemp: [dom.dist.targetLocDom]
                    unmanaged LocCyclicArr(eltType, rank, idxType) = dummyLCA;
  var myLocArrTemp: unmanaged LocCyclicArr(eltType, rank, idxType)?;

  // formerly in CyclicArr.setup()
  coforall localeIdx in dom.dist.targetLocDom with (ref locArrTemp, ref myLocArrTemp) {
    on dom.dist.targetLocs(localeIdx) {
      const LCA = new unmanaged LocCyclicArr(eltType, rank, idxType,
                                             dom.locDoms(localeIdx),
                                             initElts=initElts);
      locArrTemp(localeIdx) = LCA;
      if here.id == creationLocale then
        myLocArrTemp = LCA;
    }
  }
  delete dummyLCA, dummyLCD;

  var arr = new unmanaged CyclicArr(eltType=eltType, rank=rank,
                                    idxType=idxType, strides=strides,
         dom=_to_unmanaged(dom), locArr=locArrTemp, myLocArr=myLocArrTemp);

  // formerly in CyclicArr.setup()
  if arr.doRADOpt && disableCyclicLazyRAD then arr.setupRADOpt();

  return arr;
}

override proc CyclicDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
  dist.dsiDisplayRepresentation();
}

// common redirects
proc CyclicDom.parSafe param {
  compilerError("this domain type does not support 'parSafe'");
}
override proc CyclicDom.dsiLow do           return whole.lowBound;
override proc CyclicDom.dsiHigh do          return whole.highBound;
override proc CyclicDom.dsiAlignedLow do    return whole.low;
override proc CyclicDom.dsiAlignedHigh do   return whole.high;
override proc CyclicDom.dsiFirst do         return whole.first;
override proc CyclicDom.dsiLast do          return whole.last;
override proc CyclicDom.dsiStride do        return whole.stride;
override proc CyclicDom.dsiAlignment do     return whole.alignment;
proc CyclicDom.dsiNumIndices do    return whole.sizeAs(uint);
proc CyclicDom.dsiDim(d) do        return whole.dim(d);
proc CyclicDom.dsiDim(param d) do  return whole.dim(d);
proc CyclicDom.dsiDims() do        return whole.dims();
proc CyclicDom.dsiGetIndices() do  return whole.getIndices();
proc CyclicDom.dsiMember(i) do     return whole.contains(i);
proc CyclicDom.doiToString() do    return whole:string;
//proc CyclicDom.dsiSerialWrite(x) throws { x.write(whole); }
proc CyclicDom.dsiLocalSlice(param strides, ranges) do return whole((...ranges));
override proc CyclicDom.dsiIndexOrder(i) do              return whole.indexOrder(i);
override proc CyclicDom.dsiMyDist() do                   return dist;

proc CyclicDom.getLocDom(localeIdx) do return locDoms(localeIdx);

proc CyclicDom.dsiSetIndices(x: domain) {
  whole = x;
  setup();
}

proc CyclicDom.dsiSetIndices(x) {
  whole.setIndices(x);
  setup();
}

proc CyclicDom.dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
  chpl_assignDomainWithGetSetIndices(this, rhs);
}

proc CyclicDom.dsiSerialWrite(x) {
  if verboseCyclicDistWriters {
    x.writeln(this.type:string);
    x.writeln("------");
    for loc in dist.targetLocDom {
      x.writeln("[", loc, "=", dist.targetLocs(loc), "] owns ",
        locDoms(loc).myBlock);
    }
  } else {
    x.write(whole);
  }
}

iter CyclicDom.these() {
  for i in whole do
    yield i;
}

iter CyclicDom.these(param tag: iterKind) where tag == iterKind.leader {
  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;
  const wholeLow = whole.lowBound;
  const wholeStride = whole.stride;

  // If this is the only task running on this locale, we don't want to
  // count it when we try to determine how many tasks to use.  Here we
  // check if we are the only one running, and if so, use
  // ignoreRunning=true for this locale only.  Obviously there's a bit
  // of a race condition if some other task starts after we check, but
  // in that case there is no correct answer anyways.
  //
  // Note that this code assumes that any locale will only be in the
  // targetLocales array once.  If this is not the case, then the
  // tasks on this locale will *all* ignoreRunning, which may have
  // performance implications.
  const hereId = here.id;
  const hereIgnoreRunning = if here.runningTasks() == 1 then true
                            else ignoreRunning;
  coforall locDom in locDoms do on locDom {
    const myIgnoreRunning = if here.id == hereId then hereIgnoreRunning
      else ignoreRunning;

    // Forward to defaultRectangular to iterate over the indices we own locally
    for followThis in locDom.myBlock.these(iterKind.leader, maxTasks,
                                           myIgnoreRunning, minSize) {
      // translate the 0-based indices yielded back to our indexing scheme
      const newFollowThis = chpl__followThisToOrig(idxType, followThis, locDom.myBlock);

      // translate the local indices back to 0-based global indices
      // note that we need to go back and forth in order to distinguish
      // between global strides and those that are due to the cyclic
      // distribution (at least, I couldn't figure out a way to not go
      // back and forth without breaking tests)
      const zeroShift = {(...newFollowThis)}.chpl__unTranslate(wholeLow);
      var result: rank*range(idxType=idxType, strides=chpl_strideProduct(
                              whole.strides, chpl_strideUnion(zeroShift) ));
      type strType = chpl__signedType(idxType);
      for param i in 0..rank-1 {
        const wholestride = chpl__tuplify(wholeStride)(i);
        const ref dim = zeroShift.dim(i);
        result(i).chpl_setFields(dim.first / wholestride:idxType,
                                 dim.last / wholestride:idxType,
                                 dim.stride:strType / wholestride);
      }
      yield result;
    }
  }
}

// Utility routine to convert 0-based indices back to the indexing scheme
// of 'whole'
private proc chpl__followThisToOrig(type idxType, followThis, whole) {
  param rank = followThis.size;
  var t: rank*range(idxType, strides=strideKind.any);
  if debugCyclicDist then
    writeln(here.id, ": follower whole is: ", whole,
                     " follower is: ", followThis);
  for param i in 0..rank-1 {
    // NOTE: unsigned idxType with negative stride will not work
    const wholestride = whole.dim(i).stride:chpl__signedType(idxType);
    t(i) = ((followThis(i).lowBound*wholestride:idxType)..(followThis(i).highBound*wholestride:idxType) by (followThis(i).stride*wholestride)) + whole.dim(i).low;
  }
  return t;
}

iter CyclicDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  const t = chpl__followThisToOrig(idxType, followThis, whole);
  if debugCyclicDist then
    writeln(here.id, ": follower maps to: ", t);
  for i in {(...t)} do
    yield i;
}

proc CyclicDom.chpl__serialize() {
  return pid;
}

// TODO: What happens when we try to deserialize on a locale that doesn't
// own a copy of the privatized class?  (can that happen?)  Could this
// be a way to lazily privatize by also making the originating locale part
// of the 'data'?
proc type CyclicDom.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(unmanaged CyclicDom(rank=this.rank,
                                                    idxType=this.idxType,
                                                    strides=this.strides),
                                data);
}

override proc CyclicDom.dsiSupportsPrivatization() param do return true;

proc CyclicDom.dsiGetPrivatizeData() do return 0;

proc CyclicDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, dist.pid);
  return new unmanaged CyclicDom(rank, idxType, strides,
                                 privdist, locDoms, whole);
}

proc CyclicDom.dsiGetReprivatizeData() do return 0;

proc CyclicDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = other.whole;
}


class LocCyclicDom {
  param rank: int;
  type idxType;

  // The local block type is always stridable
  // (because that's inherent to the cyclic distribution)
  var myBlock: myBlockType(rank, idxType);
}

private proc myBlockType(param rank, type idxType) type do
  return domain(rank, idxType, strides=strideKind.any);

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocCyclicDom.contains(i) do return myBlock.contains(i);


class CyclicArr: BaseRectangularArr(?) {
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: unmanaged CyclicDom(rank, idxType, strides);

  var locArr: [dom.dist.targetLocDom] unmanaged LocCyclicArr(eltType, rank, idxType);
  var myLocArr: unmanaged LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType)?;
  const SENTINEL = max(rank*int);
}

pragma "no copy return"
proc CyclicArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 0..rank-1 {
    low(i) = ranges(i).low;
  }

  return locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
}

override proc CyclicArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD.RAD);
    writeln("locArr[", tli, "].locCyclicRAD = ", locArr[tli].locCyclicRAD);
  }
  dom.dsiDisplayRepresentation();
}

override proc CyclicArr.dsiGetBaseDom() do return dom;

override proc CyclicArr.dsiIteratorYieldsLocalElements() param {
  return true;
}


//
// NOTE: Each locale's myElems array be initialized prior to setting up
// the RAD cache.
//
proc CyclicArr.setupRADOpt() {
  if hasUnitStride() { // for now, no support for strided cyclic arrays
    for localeIdx in dom.dist.targetLocDom {
      on dom.dist.targetLocs(localeIdx) {
        const myLocArr = locArr(localeIdx);
        if myLocArr.locRAD != nil {
          delete myLocArr.locRAD;
          myLocArr.locRAD = nil;
        }
        if disableCyclicLazyRAD {
          myLocArr.locRAD = new unmanaged LocRADCache(eltType, rank, idxType,
              strideKind.any, dom.dist.targetLocDom);
          myLocArr.locCyclicRAD = new unmanaged LocCyclicRADCache(rank, idxType, dom.dist.startIdx, dom.dist.targetLocDom);
          for l in dom.dist.targetLocDom {
            if l != localeIdx {
              myLocArr.locRAD!.RAD(l) = locArr(l).myElems._value.dsiGetRAD();
            }
          }
        }
      }
    }
  }
}

override proc CyclicArr.dsiElementInitializationComplete() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      var arr = locArr(localeIdx);
      arr.myElems.dsiElementInitializationComplete();
    }
  }
}

override proc CyclicArr.dsiElementDeinitializationComplete() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      var arr = locArr(localeIdx);
      arr.myElems.dsiElementDeinitializationComplete();
    }
  }
}

override proc CyclicArr.dsiDestroyArr(deinitElts:bool) {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      var arr = locArr(localeIdx);
      if deinitElts then
        _deinitElements(arr.myElems);
      arr.myElems.dsiElementDeinitializationComplete();
      delete arr;
    }
  }
}

proc CyclicArr.chpl__serialize()
    where !(isDomainType(eltType) || isArrayType(eltType)) {
  return pid;
}

proc type CyclicArr.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(unmanaged CyclicArr(rank=this.rank,
                                                    idxType=this.idxType,
                                                    strides=this.strides,
                                                    eltType=this.eltType),
                                data);
}

override proc CyclicArr.dsiSupportsPrivatization() param do return true;

proc CyclicArr.dsiGetPrivatizeData() do return 0;

proc CyclicArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  var c = new unmanaged CyclicArr(eltType=eltType, rank=rank, idxType=idxType,
                              strides=strides, dom=privdom, locArr=locArr);
  for localeIdx in dom.dist.targetLocDom do
    if c.locArr(localeIdx).locale == here then
      c.myLocArr = c.locArr(localeIdx);
  return c;
}


inline proc _remoteAccessData.getDataIndex(
    param strides,
    myStr: rank*chpl__signedType(idxType),
    ind: rank*idxType,
    startIdx,
    dimLen) {
  // modified from DefaultRectangularArr
  var sum = origin;
  if ! strides.isOne() {
    compilerError("RADOpt not supported for strided cyclic arrays.");
  } else {
    for param i in 0..rank-1 {
      sum += (((ind(i) - off(i)):int * blk(i))-startIdx(i):int)/dimLen(i);
    }
  }
  return sum;
}

inline proc CyclicArr.dsiLocalAccess(i: rank*idxType) ref {
  return if allowDuplicateTargetLocales then this.dsiAccess(i)
                                        else _to_nonnil(myLocArr)(i);
}

proc CyclicArr.dsiAccess(i:rank*idxType) ref {
  local {
    if const myLocArrNN = myLocArr then
      if myLocArrNN.locDom.contains(i) then
        return myLocArrNN(i);
  }
  if hasUnitStride() && doRADOpt {
    if const myLocArr = this.myLocArr {
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableCyclicLazyRAD {
        if myLocArr.locRAD == nil {
          myLocArr.locRADLock.lock();
          if myLocArr.locRAD == nil {
            var tempLocRAD = new unmanaged LocRADCache(eltType, rank, idxType,
                strideKind.any, dom.dist.targetLocDom);
            if myLocArr.locCyclicRAD != nil {
              delete myLocArr.locCyclicRAD;
              myLocArr.locCyclicRAD = nil;
            }
            myLocArr.locCyclicRAD = new unmanaged LocCyclicRADCache(rank, idxType, dom.dist.startIdx, dom.dist.targetLocDom);
            tempLocRAD.RAD.blk = SENTINEL;
            myLocArr.locRAD = tempLocRAD;
          }
          myLocArr.locRADLock.unlock();
        }
        const locRAD = _to_nonnil(myLocArr.locRAD);
        if locRAD.RAD(rlocIdx).blk == SENTINEL {
          locRAD.lockRAD(rlocIdx);
          if locRAD.RAD(rlocIdx).blk == SENTINEL {
            locRAD.RAD(rlocIdx) =
              locArr(rlocIdx).myElems._value.dsiGetRAD();
          }
          locRAD.unlockRAD(rlocIdx);
        }
      }
      pragma "no copy" pragma "no auto destroy" var myLocRAD = myLocArr.locRAD;
      pragma "no copy" pragma "no auto destroy" var radata = _to_nonnil(myLocRAD).RAD;
      if radata(rlocIdx).data != nil {
        const startIdx = _to_nonnil(myLocArr.locCyclicRAD).startIdx;
        const dimLength = _to_nonnil(myLocArr.locCyclicRAD).targetLocDomDimLength;
        type strType = chpl__signedType(idxType);
        var str: rank*strType;
        for param i in 0..rank-1 {
          pragma "no copy" pragma "no auto destroy" var whole = dom.whole;
          str(i) = whole.dim(i).stride;
        }
        var dataIdx = radata(rlocIdx).getDataIndex(strides, str, i, startIdx, dimLength);
        return radata(rlocIdx).dataElem(dataIdx);
      }
    }
  }
  return locArr(dom.dist.targetLocsIdx(i))(i);
}

proc CyclicArr.dsiAccess(i: idxType...rank) ref do
  return dsiAccess(i);

proc CyclicArr.dsiBoundsCheck(i: rank*idxType) {
  return dom.dsiMember(i);
}

iter CyclicArr.these() ref {
  foreach i in dom do
    yield dsiAccess(i);
}

iter CyclicArr.these(param tag: iterKind) where tag == iterKind.leader {
  for followThis in dom.these(tag) do
    yield followThis;
}

override proc CyclicArr.dsiStaticFastFollowCheck(type leadType) param {
  if isSubtype(leadType, CyclicArr) {
    var x : leadType?;
    return _to_borrowed(x!.dom.type) == _to_borrowed(this.dom.type);
  } else {
    return _to_borrowed(leadType) == _to_borrowed(this.dom.type);
  }
}

proc CyclicArr.dsiDynamicFastFollowCheck(lead: []) do
  return this.dsiDynamicFastFollowCheck(lead.domain);

proc CyclicArr.dsiDynamicFastFollowCheck(lead: domain) {
  return lead.distribution.dsiEqualDMaps(this.dom.dist) && lead._value.whole == this.dom.whole;
}

iter CyclicArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for Cyclic array");

  var t: rank*range(idxType=idxType, strides=chpl_strideProduct(
                      chpl_strideUnion(followThis), dom.whole.strides));
  for param i in 0..rank-1 {
    type strType = chpl__signedType(idxType);
    const wholestride = dom.whole.dim(i).stride;
    if !dom.whole.strides.isPositive() && idxType != strType then
     if wholestride < 0 then
      halt("negative stride with unsigned idxType not supported");
    const iStride = wholestride:idxType;
    const      lo = (followThis(i).lowBound * iStride):idxType,
               hi = (followThis(i).highBound * iStride):idxType,
           stride = (followThis(i).stride*wholestride):strType;
    t(i).chpl_setFields(lo, hi, stride);
    t(i) = t(i) + dom.whole.dim(i).low;
  }
  const myFollowThisDom = {(...t)};
  if fast {
    const arrSection = locArr(dom.dist.targetLocsIdx(myFollowThisDom.lowBound));

    //
    // Slicing arrSection.myElems will require reference counts to be updated.
    // If myElems is an array of arrays, the inner array's domain or dist may
    // live on a different locale and require communication for reference
    // counting. Simply put: don't slice inside a local block.
    //
    // TODO: Can myLocArr be used here to simplify things?
    //
    // MPF: Why doesn't this just slice the *domain* ?
    ref chunk = arrSection.myElems(myFollowThisDom);

    if arrSection.locale.id == here.id {
      local {
        foreach i in chunk do yield i;
      }
    } else {
      foreach i in chunk do yield i;
    }
  } else {
    proc accessHelper(i) ref {
      if const myLocArrNN = myLocArr then local {
        if myLocArrNN.locDom.contains(i) then
          return myLocArrNN(i);
      }
      return dsiAccess(i);
    }

    foreach i in myFollowThisDom {
      yield accessHelper(i);
    }
  }
}

proc CyclicArr.dsiSerialRead(f) {
  chpl_serialReadWriteRectangular(f, this);
}

proc CyclicArr.dsiSerialWrite(f) {
  chpl_serialReadWriteRectangular(f, this);
}

override proc CyclicArr.dsiReallocate(bounds:rank*range(
                                        idxType, boundKind.both, strides)) {
  // The reallocation happens when the LocCyclicDom.myBlock field is changed
  // in CyclicDom.setup(). Nothing more needs to happen here.
}

override proc CyclicArr.dsiPostReallocate() {
  // Call this *after* the domain has been reallocated
  if doRADOpt then setupRADOpt();
}

proc CyclicArr.setRADOpt(val=true) {
  doRADOpt = val;
  if doRADOpt then setupRADOpt();
}

class LocCyclicArr : writeSerializable {
  type eltType;
  param rank: int;
  type idxType;

  const locDom: unmanaged LocCyclicDom(rank, idxType);

  var locRAD: unmanaged LocRADCache(eltType, rank, idxType, strideKind.any)?; // non-nil if doRADOpt=true
  var locCyclicRAD: unmanaged LocCyclicRADCache(rank, idxType)?; // see below for why
  pragma "local field" pragma "unsafe"
  // may be initialized separately
  var myElems: [locDom.myBlock] eltType;
  var locRADLock: chpl_LocalSpinlock;

  proc init(type eltType,
            param rank: int,
            type idxType,
            const locDom: unmanaged LocCyclicDom(rank, idxType),
            param initElts: bool) {
    this.eltType = eltType;
    this.rank = rank;
    this.idxType = idxType;
    this.locDom = locDom;
    this.myElems = this.locDom.myBlock.buildArray(eltType, initElts=initElts);
  }

  proc deinit() {
    // Elements in myElems are deinited in dsiDestroyArr if necessary.
    if locRAD != nil then
      delete locRAD;
    if locCyclicRAD != nil then
      delete locCyclicRAD;
  }

  // guard against dynamic dispatch resolution trying to resolve
  // write()ing out an array of sync vars and hitting the sync var
  // type's compilerError()
  override proc serialize(writer, ref serializer) throws {
    halt("LocCyclicArr.serialize() is not implemented / should not be needed");
  }
}

proc LocCyclicArr.this(i) ref {
  return myElems(i);
}

iter LocCyclicArr.these() ref {
  for elem in myElems {
    yield elem;
  }
}

// NOTE: I'd rather have this be a subclass of LocRADCache, but I
// couldn't find a way to use rank (param) and idxType (type) from the
// parent class in declarations in the subclass does not work.
class LocCyclicRADCache /* : LocRADCache */ {
  param rank: int;
  type idxType;
  var startIdx: rank*idxType;
  var targetLocDomDimLength: rank*int;

  proc init(param rank: int, type idxType, startIdx, targetLocDom) {
    this.rank = rank;
    this.idxType = idxType;

    init this;

    for param i in 0..rank-1 do
      // NOTE: Not bothering to check to see if length can fit into idxType
      targetLocDomDimLength(i) = targetLocDom.dim(i).sizeAs(int);
  }
}

private proc canDoAnyToCyclic(A, aView, B, bView) param : bool {
  if A.rank != B.rank then return false;
  use Reflection;

  if !canResolveMethod(B, "doiBulkTransferToKnown", bView,
                       A.locArr[A.locArr.domain.first].myElems._value, aView) {
    return false;
  }

  return useBulkTransferDist;
}

// Overload for any transfer *to* Cyclic, if the RHS supports transfers to a
// DefaultRectangular
proc CyclicArr.doiBulkTransferFromAny(destDom, Src, srcDom) : bool
where canDoAnyToCyclic(this, destDom, Src, srcDom) {
  if !chpl_allStridesArePositive(this, destDom, Src, srcDom) then return false;

  if debugCyclicDistBulkTransfer then
    writeln("In CyclicDist.doiBulkTransferFromAny");

  const Dest = this;
  type el    = Dest.idxType;

  coforall i in Dest.dom.dist.targetLocDom {
    on Dest.dom.dist.targetLocs(i) {
      const regionDest = Dest.dom.locDoms(i).myBlock[destDom];
      const regionSrc = Src.dom.locDoms(i).myBlock[srcDom];
      if regionDest.sizeAs(int) > 0 {
        const ini = bulkCommConvertCoordinate(regionDest.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(regionDest.last, destDom, srcDom);
        const sb  = chpl__tuplify(regionSrc.stride);

        var r1: rank * range(idxType = el, strides = strideKind.any);
        const r2 = regionDest.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 0..rank-1 {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]);
          if r1[t].sizeAs(int) != r2[t].sizeAs(int) then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].sizeAs(int)-1));
        }

        if debugCyclicDistBulkTransfer then
          writeln("A.locArr[i][", regionDest, "] = B[", (...r1), "]");

        chpl__bulkTransferArray(Dest.locArr[i].myElems._value, regionDest, Src, {(...r1)});
      }
    }
  }

  return true;
}

// For assignments of the form: DefaultRectangular = Cyclic
proc CyclicArr.doiBulkTransferToKnown(srcDom, Dest:DefaultRectangularArr, destDom) : bool
where useBulkTransferDist {
  if !chpl_allStridesArePositive(this, srcDom, Dest, destDom) then return false;

  if debugCyclicDistBulkTransfer then
    writeln("In CyclicDist.doiBulkTransferToKnown(DefaultRectangular)");

  const Src = this;
  type el   = Src.idxType;

  coforall j in Src.dom.dist.targetLocDom {
    on Src.dom.dist.targetLocs(j) {
      const inters = Src.dom.locDoms(j).myBlock[srcDom];
      if inters.sizeAs(int) > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, srcDom, destDom);
        const end = bulkCommConvertCoordinate(inters.last, srcDom, destDom);
        const sa  = chpl__tuplify(destDom.stride);

        //r2 is the domain to refer the elements of A in locale j
        //r1 is the domain to refer the corresponding elements of Dest
        var r1: rank * range(idxType = el, strides = strideKind.any);
        const r2 = inters.dims();

        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 0..rank-1 {
          r1[t] = (ini[t]:el..end[t]:el by sa[t]);
          if r1[t].sizeAs(int) != r2[t].sizeAs(int) then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].sizeAs(int)-1));
        }

        if debugCyclicDistBulkTransfer then
          writeln("A[",(...r1),"] = B[",(...r2), "]");

        const elemActual = Src.locArr[j].myElems._value;
        chpl__bulkTransferArray(Dest, {(...r1)}, elemActual, {(...r2)});
      }
    }
  }

  return true;
}

// For assignments of the form: Cyclic = DefaultRectangular
proc CyclicArr.doiBulkTransferFromKnown(destDom, Src:DefaultRectangularArr, srcDom) : bool
where useBulkTransferDist {
  if !chpl_allStridesArePositive(this, destDom, Src, srcDom) then return false;

  if debugCyclicDistBulkTransfer then
    writeln("In CyclicArr.doiBulkTransferFromKnown(DefaultRectangular)");

  const Dest = this;
  type el    = Dest.idxType;

  coforall j in Dest.dom.dist.targetLocDom {
    on Dest.dom.dist.targetLocs(j) {
      const inters = Dest.dom.locDoms(j).myBlock[destDom];
      if inters.sizeAs(int) > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(inters.last, destDom, srcDom);
        const sb  = chpl__tuplify(srcDom.stride);

        var r1: rank * range(idxType = el, strides = strideKind.any);
        const r2 = inters.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 0..rank-1 {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]);
          if r1[t].sizeAs(int) != r2[t].sizeAs(int) then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].sizeAs(int)-1));
        }

        if debugCyclicDistBulkTransfer then
            writeln("A[",(...r2),"] = B[",(...r1), "] ");

        const elemActual = Dest.locArr[j].myElems._value;
        chpl__bulkTransferArray(elemActual, {(...r2)}, Src, {(...r1)});
      }
    }
  }

  return true;
}


proc CyclicArr.dsiTargetLocales() const ref {
  return dom.dist.targetLocs;
}
proc CyclicDom.dsiTargetLocales() const ref {
  return dist.targetLocs;
}
proc CyclicImpl.dsiTargetLocales() const ref {
  return targetLocs;
}

// create a domain over an existing Cyclic Distribution
proc cyclicDist.createDomain(dom: domain(?)) {
  return dom dmapped this;
}

// create a domain over an existing Cyclic Distribution constructed from a series of ranges
proc cyclicDist.createDomain(rng: range(?)...) {
  return this.createDomain({(...rng)});
}

// create a domain over a Cyclic Distribution
proc type cyclicDist.createDomain(dom: domain(?), targetLocales: [] locale = Locales)
{
  return dom dmapped new cyclicDist(startIdx=dom.lowBound, targetLocales);
}

// create a domain over a Cyclic Distribution constructed from a series of ranges
proc type cyclicDist.createDomain(rng: range(?)..., targetLocales: [] locale = Locales) {
  return createDomain({(...rng)}, targetLocales);
}

proc type cyclicDist.createDomain(rng: range(?)...) {
  return createDomain({(...rng)});
}


// create an array over a Cyclic Distribution, default initialized
pragma "no copy return"
proc type cyclicDist.createArray(
  dom: domain(?),
  type eltType,
  targetLocales: [] locale = Locales
) {
  var D = createDomain(dom, targetLocales);
  var A: [D] eltType;
  return A;
}

// create an array over a Cyclic Distribution, initialized with the given value or iterator
pragma "no copy return"
proc type cyclicDist.createArray(
  dom: domain(?),
  type eltType,
  initExpr: ?t,
  targetLocales: [] locale = Locales
) where isSubtype(t, _iteratorRecord) || isCoercible(t, eltType)
{
  var D = createDomain(dom, targetLocales);
  var A: [D] eltType;
  A = initExpr;
  return A;
}

// create an array over a Cyclic Distribution, initialized from the given array
pragma "no copy return"
proc type cyclicDist.createArray(
  dom: domain(?),
  type eltType,
  initExpr: [?arrayDom] ?arrayEltType,
  targetLocales: [] locale = Locales
) where dom.rank == arrayDom.rank && isCoercible(arrayEltType, eltType)
{
  if boundsChecking then
    for (d, ad, i) in zip(dom.dims(), arrayDom.dims(), 0..) do
      if d.size != ad.size then halt("Domain size mismatch in 'cyclicDist.createArray' dimension " + i:string);
  var D = createDomain(dom, targetLocales);
  var A: [D] eltType;
  A = initExpr;
  return A;
}

// create an array over a Cyclic Distribution constructed from a series of ranges, default initialized
pragma "no copy return"
proc type cyclicDist.createArray(
  rng: range(?)...,
  type eltType,
  targetLocales: [] locale = Locales
) {
  return createArray({(...rng)}, eltType, targetLocales);
}

pragma "no copy return"
proc type cyclicDist.createArray(rng: range(?)..., type eltType) {
  return createArray({(...rng)}, eltType);
}

// create an array over a Cyclic Distribution constructed from a series of ranges, initialized with the given value or iterator
pragma "no copy return"
proc type cyclicDist.createArray(
  rng: range(?)...,
  type eltType,
  initExpr: ?t,
  targetLocales: [] locale = Locales
) where isSubtype(t, _iteratorRecord) || isCoercible(t, eltType)
{
  return createArray({(...rng)}, eltType, initExpr, targetLocales);
}

pragma "no copy return"
proc type cyclicDist.createArray(rng: range(?)..., type eltType, initExpr: ?t)
  where isSubtype(t, _iteratorRecord) || isCoercible(t, eltType)
{
  return createArray({(...rng)}, eltType, initExpr);
}

// create an array over a Cyclic Distribution constructed from a series of ranges, initialized from the given array
pragma "no copy return"
proc type cyclicDist.createArray(
  rng: range(?)...,
  type eltType,
  initExpr: [?arrayDom] ?arrayEltType,
  targetLocales: [] locale = Locales
) where rng.size == arrayDom.rank && isCoercible(arrayEltType, eltType)
{
  return createArray({(...rng)}, eltType, initExpr, targetLocales);
}

pragma "no copy return"
proc type cyclicDist.createArray(
  rng: range(?)...,
  type eltType,
  initExpr: [?arrayDom] ?arrayEltType
) where rng.size == arrayDom.rank && isCoercible(arrayEltType, eltType)
{
  return createArray({(...rng)}, eltType, initExpr);
}

// Cyclic subdomains are represented as a single domain

proc CyclicArr.dsiHasSingleLocalSubdomain() param do return !allowDuplicateTargetLocales;
proc CyclicDom.dsiHasSingleLocalSubdomain() param do return !allowDuplicateTargetLocales;

proc CyclicArr.dsiLocalSubdomain(loc: locale) {
  if (loc == here) {
    // quick solution if we have a local array
    if const myLocArrNN = myLocArr then
      return myLocArrNN.locDom.myBlock;
    // if not, we must not own anything
    var d: myBlockType(rank, idxType);
    return d;
  } else {
    return dom.dsiLocalSubdomain(loc);
  }
}
proc CyclicDom.dsiLocalSubdomain(loc: locale) {
  const (gotit, locid) = dist.chpl__locToLocIdx(loc);
  if (gotit) {
    return whole[(...(chpl__computeCyclic(idxType, locid, dist.targetLocDom.dims(), dist.startIdx)))] : myBlockType(rank, idxType);
  } else {
    var d: myBlockType(rank, idxType);
    return d;
  }
}

proc CyclicArr.canDoOptimizedSwap(other) {
  var domsMatch = true;

  if this.dom != other.dom { // no need to check if this is true
    for param i in 0..this.dom.rank-1 {
      if this.dom.whole.dim(i) != other.dom.whole.dim(i) {
        domsMatch = false;
      }
    }
  }

  if domsMatch {
    // distributions must be equal, too
    return this.dom.dist.dsiEqualDMaps(other.dom.dist);
  }
  return false;
}

// A helper routine that will perform a pointer swap on an array
// instead of doing a deep copy of that array. Returns true
// if used the optimized swap, false otherwise
//
// TODO: stridability causes issues with RAD swap, and somehow isn't captured by
// the formal type when we check whether this resolves.
proc CyclicArr.doiOptimizedSwap(other: this.type)
  where this.strides == other.strides {

  if(canDoOptimizedSwap(other)) {
    if debugOptimizedSwap {
      writeln("CyclicArr doing optimized swap. Domains: ",
              this.dom.whole, " ", other.dom.whole, " Bounding boxes: ",
              this.dom.dist.startIdx, " ", other.dom.dist.startIdx);
    }
    coforall (locarr1, locarr2) in zip(this.locArr, other.locArr) {
      on locarr1 {
        locarr1.myElems <=> locarr2.myElems;
        locarr1.locRAD <=> locarr2.locRAD;
      }
    }
    return true;
  } else {
    if debugOptimizedSwap {
      writeln("CyclicArr doing unoptimized swap. Domains: ",
              this.dom.whole, " ", other.dom.whole, " Bounding boxes: ",
              this.dom.dist.startIdx, " ", other.dom.dist.startIdx);
    }
    return false;
  }
}


// The purpose of this overload is to provide debugging output in the event that
// debugOptimizedSwap is on and the main routine doesn't resolve (e.g., due to a
// type, stridability, or rank mismatch in the other argument). When
// debugOptimizedSwap is off, this overload will be ignored due to its where
// clause.
pragma "last resort"
proc CyclicArr.doiOptimizedSwap(other) where debugOptimizedSwap {
  writeln("CyclicArr doing unoptimized swap. Type mismatch");
  return false;
}

}  // module CyclicDist
