/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
This Cyclic distribution maps indices to locales in a round-robin pattern
starting at a given index.

Formally, consider a Cyclic distribution with:

  =============  ====================================================
  rank           ``d``
  start index    ``(s_1, ...., s_d)``
  over locales   ``targetLocales: [0..N_1-1, ...., 0..N_d-1] locale``
  =============  ====================================================

It maps an index ``(i_1, ...., i_d)``
to the locale ``targetLocales[j_1, ...., j_d]``
where, for each ``k`` in ``1..d``,
we have:

  ``j_k = (i_k - s_k) (mod N_k)``


**Example**

The following code declares a domain ``D`` distributed
using a Cyclic distribution with a start index of ``(1,1)``,
and declares an array ``A`` over that domain.
The `forall` loop sets each array element
to the ID of the locale to which it is mapped.

  .. code-block:: chapel

    use CyclicDist;

    const Space = {1..8, 1..8};
    const D: domain(2) dmapped Cyclic(startIdx=Space.low) = Space;
    var A: [D] int;

    forall a in A do
      a = a.locale.id;

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


**Initializer Arguments**

The ``Cyclic`` class initializer is defined as follows:

  .. code-block:: chapel

    proc Cyclic.init(
      startIdx,
      targetLocales: [] locale = Locales,
      dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
      dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
      dataParMinGranularity     = // value of  dataParMinGranularity      config const,
      param rank: int  = // inferred from startIdx argument,
      type idxType     = // inferred from startIdx argument )

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

The arguments ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``,
and ``dataParMinGranularity`` set the knobs that are used to
control intra-locale data parallelism for Cyclic-distributed domains
and arrays in the same way that the like-named config constants
control data parallelism for ranges and default-distributed domains
and arrays.

The ``rank`` and ``idxType`` arguments are inferred from the
``startIdx`` argument unless explicitly set.
They must match the rank and index type of the domains
"dmapped" using that Cyclic instance.


**Convenience Initializer Functions**

It is common for a ``Cyclic`` distribution to distribute its indices
across all locales. In this case, a convenience function can be used to
declare variables of cyclic-distributed domain or array type.  These functions
take a domain or list of ranges as arguments and return a cyclic-distributed
domain or array.

  .. code-block:: chapel

    use CyclicDist;

    var CyclicDom1 = newCyclicDom({1..5, 1..5});
    var CyclicArr1 = newCyclicArr({1..5, 1..5}, real);
    var CyclicDom2 = newCyclicDom(1..5, 1..5);
    var CyclicArr2 = newCyclicArr(1..5, 1..5, real);


**Data-Parallel Iteration**

A `forall` loop over a Cyclic-distributed domain or array
executes each iteration on the locale where that iteration's index
is mapped to.

Parallelism within each locale is guided by the values of
``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``, and
``dataParMinGranularity`` of the respective Cyclic instance.
Updates to these values, if any, take effect only on the locale
where the updates are made.


**Limitations**

This distribution has not been tuned for performance.
*/
class Cyclic: BaseDist {
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

    var startIdxTemp: rank*idxType;
    for param i in 0..rank-1 {
      const startIdxI = if isTuple(startIdx) then startIdx(i) else startIdx;
      startIdxTemp(i) = chpl__mod(startIdxI, targetLocDom.dim(i).size);
    }
    this.startIdx = startIdxTemp;

    // Instead of 'dummyLC', we could give 'locDistTemp' a nilable element type.
    const dummyLC = new unmanaged LocCyclic(rank, idxType, dummy=true);
    var locDistTemp: [targetLocDom] unmanaged LocCyclic(rank, idxType)
          = dummyLC;
    coforall locid in targetLocDom do
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

    this.complete();

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

  proc dsiEqualDMaps(that: Cyclic(?)) {
    return (this.startIdx == that.startIdx &&
            this.targetLocs.equals(that.targetLocs));
  }

  proc dsiEqualDMaps(that) param {
    return false;
  }

  proc dsiClone() {
    return new unmanaged Cyclic(startIdx, targetLocs,
                      dataParTasksPerLocale,
                      dataParIgnoreRunningTasks,
                      dataParMinGranularity);
  }

  override proc dsiDestroyDist() {
    coforall ld in locDist do {
      on ld do
        delete ld;
    }
  }

}

proc Cyclic.chpl__locToLocIdx(loc: locale) {
  for locIdx in targetLocDom do
    if (targetLocs[locIdx] == loc) then
      return (true, locIdx);
  return (false, targetLocDom.first);
}

proc Cyclic.getChunk(inds, locid) {
  const chunk = locDist(locid).myChunk((...inds.getIndices()));
  return chunk;
}

override proc Cyclic.dsiDisplayRepresentation() {
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

proc Cyclic.init(other: Cyclic, privateData,
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
                 
override proc Cyclic.dsiSupportsPrivatization() param return true;

proc Cyclic.dsiGetPrivatizeData() return (startIdx,
                                          targetLocDom.dims(),
                                          dataParTasksPerLocale,
                                          dataParIgnoreRunningTasks,
                                          dataParMinGranularity);

proc Cyclic.dsiPrivatize(privatizeData) {
  return new unmanaged Cyclic(_to_unmanaged(this), privatizeData);
}

proc Cyclic.dsiGetReprivatizeData() return 0;

proc Cyclic.dsiReprivatize(other, reprivatizeData) {
  targetLocDom = other.targetLocDom;
  targetLocs = other.targetLocs;
  locDist = other.locDist;
  startIdx = other.startIdx;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}

override proc Cyclic.dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Cyclic domain rank does not match distribution's");
  const whole = createWholeDomainForInds(rank, idxType, stridable, inds);

  const dummyLCD = new unmanaged LocCyclicDom(rank, idxType);
  var locDomsTemp: [this.targetLocDom] unmanaged LocCyclicDom(rank, idxType)
        = dummyLCD;
  coforall localeIdx in this.targetLocDom do
    on this.targetLocs(localeIdx) do
      locDomsTemp(localeIdx) = new unmanaged LocCyclicDom(rank, idxType,
                                              this.getChunk(whole, localeIdx));
  delete dummyLCD;

  var dom = new unmanaged CyclicDom(rank, idxType, stridable,
                                    this: unmanaged, locDomsTemp, whole);
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

proc Cyclic.writeThis(x) throws {
  x <~> this.type:string <~> "\n";
  x <~> "------\n";
  for locid in targetLocDom do
    x <~> " [" <~> locid <~> "=" <~> targetLocs(locid) <~> "] owns chunk: " <~>
      locDist(locid).myChunk <~> "\n";
}

proc Cyclic.targetLocsIdx(i: idxType) {
  const numLocs:idxType = targetLocDom.size:idxType;
  // this is wrong if i is less than startIdx
  //return ((i - startIdx(0)) % numLocs):int;
  // this works even if i is less than startIdx
  return chpl__diffMod(i, startIdx(0), numLocs):idxType;
}

proc Cyclic.targetLocsIdx(ind: rank*idxType) {
  var x: rank*int;
  for param i in 0..rank-1 {
    var dimLen = targetLocDom.dim(i).size;
    //x(i) = ((ind(i) - startIdx(i)) % dimLen):int;
    x(i) = chpl__diffMod(ind(i), startIdx(i), dimLen):int;
  }
  if rank == 1 then
    return x(0);
  else
    return x;
}

proc Cyclic.dsiIndexToLocale(i: idxType) where rank == 1 {
  return targetLocs(targetLocsIdx(i));
}

proc Cyclic.dsiIndexToLocale(i: rank*idxType) {
  return targetLocs(targetLocsIdx(i));
}


  proc chpl__computeCyclicDim(type idxType, lo, myloc, numlocs) {
    const lower = min(idxType)..(lo+myloc) by -numlocs;
    const upper = lo+myloc..max(idxType) by numlocs;
    return lower.last..upper.last by numlocs;
  }

proc chpl__computeCyclic(type idxType, locid, targetLocBox, startIdx) {
    type strType = chpl__signedType(idxType);
    param rank = targetLocBox.size;
    var inds: rank*range(idxType, stridable=true);
    for param i in 0..rank-1 {
      // NOTE: Not bothering to check to see if these can fit into idxType
      const lo = chpl__tuplify(startIdx)(i): idxType;
      const myloc = chpl__tuplify(locid)(i): idxType;
      // NOTE: Not checking for overflow here when casting to strType
      const numlocs = targetLocBox(i).size: strType;
      inds(i) = chpl__computeCyclicDim(idxType, lo, myloc, numlocs);
    }
    return inds;
  }

class LocCyclic {
  param rank: int;
  type idxType;

  const myChunk: domain(rank, idxType, true);

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

    var inds: rank*range(idxType, stridable=true);

    inds = chpl__computeCyclic(idxType, locid, distLocDims, startIdx);
    myChunk = {(...inds)};
  }

  // Used to create a dummy instance.
  proc init(param rank, type idxType, param dummy: bool) where dummy {
    this.rank = rank;
    this.idxType = idxType;
  }
}


class CyclicDom : BaseRectangularDom {
  const dist: unmanaged Cyclic(rank, idxType);

  var locDoms: [dist.targetLocDom] unmanaged LocCyclicDom(rank, idxType);

  var whole: domain(rank, idxType, stridable);
}

proc CyclicDom.setup() {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) {
        var chunk = dist.getChunk(whole, localeIdx);
        locDoms(localeIdx).myBlock = chunk;
      }
    }
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
  coforall localeIdx in dom.dist.targetLocDom with (ref myLocArrTemp) {
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
                                    idxType=idxType, stridable=stridable,
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

proc CyclicDom.dsiLow return whole.low;

proc CyclicDom.dsiHigh return whole.high;

proc CyclicDom.dsiAlignedLow return whole.alignedLow;

proc CyclicDom.dsiAlignedHigh return whole.alignedHigh;

proc CyclicDom.dsiAlignment return whole.alignment;

proc CyclicDom.dsiStride return whole.stride;

proc CyclicDom.dsiMember(i) return whole.contains(i);

proc CyclicDom.dsiIndexOrder(i) return whole.indexOrder(i);

proc CyclicDom.dsiDims() return whole.dims();

proc CyclicDom.dsiDim(d: int) return whole.dim(d);

proc CyclicDom.getLocDom(localeIdx) return locDoms(localeIdx);

override proc CyclicDom.dsiMyDist() return dist;



proc CyclicDom.dsiGetIndices() {
  return whole.getIndices();
}

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
    x <~> this.type:string <~> "\n";
    x <~> "------\n";
    for loc in dist.targetLocDom {
      x <~> "[" <~> loc <~> "=" <~> dist.targetLocs(loc) <~> "] owns " <~>
        locDoms(loc).myBlock <~> "\n";
    }
  } else {
    x <~> whole;
  }
}

proc CyclicDom.dsiNumIndices return whole.size;

iter CyclicDom.these() {
  for i in whole do
    yield i;
}

iter CyclicDom.these(param tag: iterKind) where tag == iterKind.leader {
  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;
  const wholeLow = whole.low;
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
                                           myIgnoreRunning, minSize) do {
      // translate the 0-based indices yielded back to our indexing scheme
      const newFollowThis = chpl__followThisToOrig(idxType, followThis, locDom.myBlock);

      // translate the local indices back to 0-based global indices
      // note that we need to go back and forth in order to distinguish
      // between global strides and those that are due to the cyclic
      // distribution (at least, I couldn't figure out a way to not go
      // back and forth without breaking tests)
      const zeroShift = {(...newFollowThis)}.chpl__unTranslate(wholeLow);
      var result: rank*range(idxType=idxType, stridable=true);
      type strType = chpl__signedType(idxType);
      for param i in 0..rank-1 {
        const wholestride = chpl__tuplify(wholeStride)(i);
        const ref dim = zeroShift.dim(i);
        result(i) = (dim.first / wholestride:idxType)..(dim.last / wholestride:idxType) by (dim.stride:strType / wholestride);
      }
      yield result;
    }
  }
}

// Utility routine to convert 0-based indices back to the indexing scheme
// of 'whole'
private proc chpl__followThisToOrig(type idxType, followThis, whole) {
  param rank = followThis.size;
  var t: rank*range(idxType, stridable=true);
  if debugCyclicDist then
    writeln(here.id, ": follower whole is: ", whole,
                     " follower is: ", followThis);
  for param i in 0..rank-1 {
    // NOTE: unsigned idxType with negative stride will not work
    const wholestride = whole.dim(i).stride:chpl__signedType(idxType);
    t(i) = ((followThis(i).low*wholestride:idxType)..(followThis(i).high*wholestride:idxType) by (followThis(i).stride*wholestride)) + whole.dim(i).alignedLow;
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
                                                    stridable=this.stridable),
                                data);
}

override proc CyclicDom.dsiSupportsPrivatization() param return true;

proc CyclicDom.dsiGetPrivatizeData() return 0;

proc CyclicDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, dist.pid);
  return new unmanaged CyclicDom(rank, idxType, stridable,
                                 privdist, locDoms, whole);
}

proc CyclicDom.dsiGetReprivatizeData() return 0;

proc CyclicDom.dsiReprivatize(other, reprivatizeData) {
  locDoms = other.locDoms;
  whole = other.whole;
}

proc CyclicDom.dsiLocalSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}


class LocCyclicDom {
  param rank: int;
  type idxType;

  // The local block type is always stridable
  // (because that's inherent to the cyclic distribution)
  var myBlock: domain(rank, idxType, stridable=true);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocCyclicDom.contains(i) return myBlock.contains(i);


class CyclicArr: BaseRectangularArr {
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: unmanaged CyclicDom(rank, idxType, stridable);

  var locArr: [dom.dist.targetLocDom] unmanaged LocCyclicArr(eltType, rank, idxType);
  var myLocArr: unmanaged LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType)?;
  const SENTINEL = max(rank*idxType);
}

pragma "no copy return"
proc CyclicArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 0..rank-1 {
    low(i) = ranges(i).alignedLow;
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

override proc CyclicArr.dsiGetBaseDom() return dom;

//
// NOTE: Each locale's myElems array be initialized prior to setting up
// the RAD cache.
//
proc CyclicArr.setupRADOpt() {
  if !stridable { // for now, no support for strided cyclic arrays
    for localeIdx in dom.dist.targetLocDom {
      on dom.dist.targetLocs(localeIdx) {
        const myLocArr = locArr(localeIdx);
        if myLocArr.locRAD != nil {
          delete myLocArr.locRAD;
          myLocArr.locRAD = nil;
        }
        if disableCyclicLazyRAD {
          myLocArr.locRAD = new unmanaged LocRADCache(eltType, rank, idxType,
              stridable=true, dom.dist.targetLocDom);
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

proc CyclicArr.chpl__serialize() {
  return pid;
}

proc type CyclicArr.chpl__deserialize(data) {
  return chpl_getPrivatizedCopy(unmanaged CyclicArr(rank=this.rank,
                                                    idxType=this.idxType,
                                                    stridable=this.stridable,
                                                    eltType=this.eltType),
                                data);
}

override proc CyclicArr.dsiSupportsPrivatization() param return true;

proc CyclicArr.dsiGetPrivatizeData() return 0;

proc CyclicArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  var c = new unmanaged CyclicArr(eltType=eltType, rank=rank, idxType=idxType,
                              stridable=stridable, dom=privdom, locArr=locArr);
  for localeIdx in dom.dist.targetLocDom do
    if c.locArr(localeIdx).locale == here then
      c.myLocArr = c.locArr(localeIdx);
  return c;
}


inline proc _remoteAccessData.getDataIndex(
    param stridable,
    myStr: rank*chpl__signedType(idxType),
    ind: rank*idxType,
    startIdx,
    dimLen) {
  // modified from DefaultRectangularArr
  var sum = origin;
  if stridable {
    halt("RADOpt not supported for strided cyclic arrays.");
  } else {
    for param i in 0..rank-1 do {
      sum += (((ind(i) - off(i)) * blk(i))-startIdx(i))/dimLen(i);
    }
  }
  return sum;
}

inline proc CyclicArr.dsiLocalAccess(i: rank*idxType) ref {
  return _to_nonnil(myLocArr).this(i);
}

proc CyclicArr.dsiAccess(i:rank*idxType) ref {
  local {
    if myLocArr != nil && _to_nonnil(myLocArr).locDom.contains(i) then
      return _to_nonnil(myLocArr).this(i);
  }
  if doRADOpt && !stridable {
    if this.myLocArr {
      const myLocArr = _to_nonnil(this.myLocArr);
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableCyclicLazyRAD {
        if myLocArr.locRAD == nil {
          myLocArr.locRADLock.lock();
          if myLocArr.locRAD == nil {
            var tempLocRAD = new unmanaged LocRADCache(eltType, rank, idxType,
                stridable=true, dom.dist.targetLocDom);
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
        var dataIdx = radata(rlocIdx).getDataIndex(stridable, str, i, startIdx, dimLength);
        return radata(rlocIdx).dataElem(dataIdx);
      }
    }
  }
  return locArr(dom.dist.targetLocsIdx(i))(i);
}

proc CyclicArr.dsiAccess(i: idxType...rank) ref
  return dsiAccess(i);

proc CyclicArr.dsiBoundsCheck(i: rank*idxType) {
  return dom.dsiMember(i);
}


iter CyclicArr.these() ref {
  for i in dom do
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

proc CyclicArr.dsiDynamicFastFollowCheck(lead: [])
  return this.dsiDynamicFastFollowCheck(lead.domain);

proc CyclicArr.dsiDynamicFastFollowCheck(lead: domain) {
  return lead.dist.dsiEqualDMaps(this.dom.dist) && lead._value.whole == this.dom.whole;
}

iter CyclicArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for Cyclic array");

  var t: rank*range(idxType=idxType, stridable=true);
  for param i in 0..rank-1 {
    type strType = chpl__signedType(idxType);
    const wholestride = dom.whole.dim(i).stride:chpl__signedType(idxType);
    if wholestride < 0 && idxType != strType then
      halt("negative stride with unsigned idxType not supported");
    const iStride = wholestride:idxType;
    const      lo = (followThis(i).low * iStride):idxType,
               hi = (followThis(i).high * iStride):idxType,
           stride = (followThis(i).stride*wholestride):strType;
    t(i) = (lo..hi by stride) + dom.whole.dim(i).alignedLow;
  }
  const myFollowThisDom = {(...t)};
  if fast {
    const arrSection = locArr(dom.dist.targetLocsIdx(myFollowThisDom.low));

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
        for i in chunk do yield i;
      }
    } else {
      for i in chunk do yield i;
    }
  } else {
    proc accessHelper(i) ref {
      if myLocArr then local {
        if _to_nonnil(myLocArr).locDom.contains(i) then
          return _to_nonnil(myLocArr).this(i);
      }
      return dsiAccess(i);
    }

    for i in myFollowThisDom {
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

override proc CyclicArr.dsiReallocate(bounds:rank*range(idxType,BoundedRangeType.bounded,stridable)) {
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

class LocCyclicArr {
  type eltType;
  param rank: int;
  type idxType;

  const locDom: unmanaged LocCyclicDom(rank, idxType);

  var locRAD: unmanaged LocRADCache(eltType, rank, idxType, stridable=true)?; // non-nil if doRADOpt=true
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
  override proc writeThis(f) throws {
    halt("LocCyclicArr.writeThis() is not implemented / should not be needed");
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
  var targetLocDomDimLength: rank*idxType;

  proc init(param rank: int, type idxType, startIdx, targetLocDom) {
    this.rank = rank;
    this.idxType = idxType;

    this.complete();

    for param i in 0..rank-1 do
      // NOTE: Not bothering to check to see if length can fit into idxType
      targetLocDomDimLength(i) = targetLocDom.dim(i).size:idxType;
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
  if debugCyclicDistBulkTransfer then
    writeln("In CyclicDist.doiBulkTransferFromAny");

  const Dest = this;
  type el    = Dest.idxType;

  coforall i in Dest.dom.dist.targetLocDom {
    on Dest.dom.dist.targetLocs(i) {
      const regionDest = Dest.dom.locDoms(i).myBlock[destDom];
      const regionSrc = Src.dom.locDoms(i).myBlock[srcDom];
      if regionDest.size > 0 {
        const ini = bulkCommConvertCoordinate(regionDest.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(regionDest.last, destDom, srcDom);
        const sb  = chpl__tuplify(regionSrc.stride);

        var r1,r2: rank * range(idxType = el,stridable = true);
        r2 = regionDest.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 0..rank-1 {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]);
          if r1[t].size != r2[t].size then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].size-1));
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

  if debugCyclicDistBulkTransfer then
    writeln("In CyclicDist.doiBulkTransferToKnown(DefaultRectangular)");

  const Src = this;
  type el   = Src.idxType;

  coforall j in Src.dom.dist.targetLocDom {
    on Src.dom.dist.targetLocs(j) {
      const inters = Src.dom.locDoms(j).myBlock[srcDom];
      if inters.size > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, srcDom, destDom);
        const end = bulkCommConvertCoordinate(inters.last, srcDom, destDom);
        const sa  = chpl__tuplify(destDom.stride);

        //r2 is the domain to refer the elements of A in locale j
        //r1 is the domain to refer the corresponding elements of Dest
        var r1,r2: rank * range(idxType = el,stridable = true);
        r2 = inters.dims();

        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 0..rank-1 {
          r1[t] = (ini[t]:el..end[t]:el by sa[t]);
          if r1[t].size != r2[t].size then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].size-1));
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

  if debugCyclicDistBulkTransfer then
    writeln("In CyclicArr.doiBulkTransferFromKnown(DefaultRectangular)");

  const Dest = this;
  type el    = Dest.idxType;

  coforall j in Dest.dom.dist.targetLocDom {
    on Dest.dom.dist.targetLocs(j) {
      const inters = Dest.dom.locDoms(j).myBlock[destDom];
      if inters.size > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(inters.last, destDom, srcDom);
        const sb  = chpl__tuplify(srcDom.stride);

        var r1,r2: rank * range(idxType = el,stridable = true);
        r2 = inters.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 0..rank-1 {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]);
          if r1[t].size != r2[t].size then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].size-1));
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

proc CyclicArr.dsiTargetLocales() {
  return dom.dist.targetLocs;
}
proc CyclicDom.dsiTargetLocales() {
  return dist.targetLocs;
}
proc Cyclic.dsiTargetLocales() {
  return targetLocs;
}

// Cyclic subdomains are represented as a single domain

proc CyclicArr.dsiHasSingleLocalSubdomain() param return true;
proc CyclicDom.dsiHasSingleLocalSubdomain() param return true;

proc CyclicArr.dsiLocalSubdomain(loc: locale) {
  if (loc == here) {
    // quick solution if we have a local array
    if myLocArr != nil then
      return _to_nonnil(myLocArr).locDom.myBlock;
    // if not, we must not own anything
    var d: domain(rank, idxType, stridable=true);
    return d;
  } else {
    return dom.dsiLocalSubdomain(loc);
  }
}
proc CyclicDom.dsiLocalSubdomain(loc: locale) {
  const (gotit, locid) = dist.chpl__locToLocIdx(loc);
  if (gotit) {
    return whole[(...(chpl__computeCyclic(this.idxType, locid, dist.targetLocDom.dims(), dist.startIdx)))];
  } else {
    var d: domain(rank, idxType, stridable=true);
    return d;
  }
}

proc newCyclicDom(dom: domain) {
  return dom dmapped Cyclic(startIdx=dom.low);
}

proc newCyclicArr(dom: domain, type eltType) {
  var D = newCyclicDom(dom);
  var A: [D] eltType;
  return A;
}

proc newCyclicDom(rng: range...) {
  return newCyclicDom({(...rng)});
}

proc newCyclicArr(rng: range..., type eltType) {
  return newCyclicArr({(...rng)}, eltType);
}
