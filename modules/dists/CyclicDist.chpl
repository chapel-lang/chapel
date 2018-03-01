/*
 * Copyright 2004-2018 Cray Inc.
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

use DSIUtil;

proc _determineRankFromStartIdx(startIdx) param {
  return if isTuple(startIdx) then startIdx.size else 1;
}

proc _determineIdxTypeFromStartIdx(startIdx) type {
  return if isTuple(startIdx) then startIdx(1).type else startIdx.type;
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


**Constructor Arguments**

The ``Cyclic`` class constructor is defined as follows:

  .. code-block:: chapel

    proc Cyclic(
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

  var startIdx: rank*idxType;
  var targetLocDom: domain(rank);
  var targetLocs: [targetLocDom] locale;

  var locDist: [targetLocDom] LocCyclic(rank, idxType);

  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;

  proc Cyclic(startIdx,
             targetLocales: [] locale = Locales,
             dataParTasksPerLocale=getDataParTasksPerLocale(),
             dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
             dataParMinGranularity=getDataParMinGranularity(),
             param rank: int = _determineRankFromStartIdx(startIdx),
             type idxType = _determineIdxTypeFromStartIdx(startIdx))
    where isTuple(startIdx) || isIntegralType(startIdx.type) {
    var tupleStartIdx: rank*idxType;
    if isTuple(startIdx) then tupleStartIdx = startIdx;
                         else tupleStartIdx(1) = startIdx;

    // MPF - why isn't it:
    //setupTargetLocalesArray(targetLocDom, targetLocs, targetLocales);

    if rank == 1  {
      targetLocDom = {0..#targetLocales.numElements};
      targetLocs = targetLocales;
    } else if targetLocales.rank == 1 {
      const factors = _factor(rank, targetLocales.numElements);
      var ranges: rank*range;
      for param i in 1..rank {
        ranges(i) = 0..#factors(i);
      }
      targetLocDom = {(...ranges)};
      for (loc1, loc2) in zip(targetLocs, targetLocales) {
        loc1 = loc2;
      }
    } else {
      if targetLocales.rank != rank then
        compilerError("locales array rank must be one or match distribution rank");
      var ranges: rank*range;
      for param i in 1..rank do {
        var thisRange = targetLocales.domain.dim(i);
        ranges(i) = 0..#thisRange.length;
      }
      targetLocDom = {(...ranges)};
      targetLocs = reshape(targetLocales, targetLocDom);
    }

    for param i in 1..rank do
      this.startIdx(i) = chpl__mod(tupleStartIdx(i), targetLocDom.dim(i).length);

    // NOTE: When these knobs stop using the global defaults, we will need
    // to add checks to make sure dataParTasksPerLocale<0 and
    // dataParMinGranularity<0
    this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                                 then here.maxTaskPar
                                 else dataParTasksPerLocale;
    this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
    this.dataParMinGranularity = dataParMinGranularity;

    coforall locid in targetLocDom {
      on targetLocs(locid) {
        locDist(locid) = new LocCyclic(rank, idxType, locid, this);
      }
    }
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
        locDist(locid) = new LocCyclic(rank, idxType, locid, this);
  }

  proc dsiEqualDMaps(that: Cyclic(?)) {
    return (this.startIdx == that.startIdx &&
            this.targetLocs.equals(that.targetLocs));
  }

  proc dsiEqualDMaps(that) param {
    return false;
  }

  proc dsiClone() {
    return new Cyclic(startIdx, targetLocs,
                      dataParTasksPerLocale,
                      dataParIgnoreRunningTasks,
                      dataParMinGranularity);
  }

  proc dsiDestroyDist() {
    coforall ld in locDist do {
      on ld do
        delete ld;
    }
  }

}


proc Cyclic.getChunk(inds, locid) {
  var sliceBy: rank*range(idxType=idxType, stridable=true);
  var locidtup: rank*idxType;
  // NOTE: Not bothering to check to see if these can fit into idxType
  if rank == 1 then
    locidtup(1) = locid:idxType;
  else
    for param i in 1..rank do locidtup(i) = locid(i):idxType;
  for param i in 1..rank {
    var distStride = targetLocDom.dim(i).length:chpl__signedType(idxType);
    var offset = chpl__diffMod(startIdx(i) + locidtup(i), inds.dim(i).low, distStride);
    sliceBy(i) = inds.dim(i).low + offset..inds.dim(i).high by distStride;
    // remove alignment
    sliceBy(i).alignHigh();
  }
  return inds((...sliceBy));
  //
  // WANT:
  //var distWhole = locDist(locid).myChunk.getIndices();
  //return inds((...distWhole));
}

proc Cyclic.dsiDisplayRepresentation() {
  writeln("startIdx = ", startIdx);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocs = ", for tl in targetLocs do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

proc Cyclic.dsiSupportsPrivatization() param return true;

proc Cyclic.dsiGetPrivatizeData() return 0;

proc Cyclic.dsiPrivatize(privatizeData) {
  return new Cyclic(startIdx, targetLocs,
                    dataParTasksPerLocale,
                    dataParIgnoreRunningTasks,
                    dataParMinGranularity);
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

proc Cyclic.dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Cyclic domain rank does not match distribution's");
  var dom = new CyclicDom(rank=rank, idxType=idxType, dist = this, stridable=stridable);
  dom.dsiSetIndices(inds);
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
    if i == args.size {
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
  return helper(1);
}

proc Cyclic.writeThis(x) {
  x <~> this.type:string <~> "\n";
  x <~> "------\n";
  for locid in targetLocDom do
    x <~> " [" <~> locid <~> "=" <~> targetLocs(locid) <~> "] owns chunk: " <~>
      locDist(locid).myChunk <~> "\n";
}

proc Cyclic.targetLocsIdx(i: idxType) {
  const numLocs:idxType = targetLocDom.numIndices:idxType;
  // this is wrong if i is less than startIdx
  //return ((i - startIdx(1)) % numLocs):int;
  // this works even if i is less than startIdx
  return chpl__diffMod(i, startIdx(1), numLocs):idxType;
}

proc Cyclic.targetLocsIdx(ind: rank*idxType) {
  var x: rank*int;
  for param i in 1..rank {
    var dimLen = targetLocDom.dim(i).length;
    //x(i) = ((ind(i) - startIdx(i)) % dimLen):int;
    x(i) = chpl__diffMod(ind(i), startIdx(i), dimLen):int;
  }
  if rank == 1 then
    return x(1);
  else
    return x;
}

proc Cyclic.dsiIndexToLocale(i: idxType) where rank == 1 {
  return targetLocs(targetLocsIdx(i));
}

proc Cyclic.dsiIndexToLocale(i: rank*idxType) {
  return targetLocs(targetLocsIdx(i));
}


class LocCyclic {
  param rank: int;
  type idxType;

  const myChunk: domain(rank, idxType, true);

  proc LocCyclic(param rank, type idxType, locid, dist: Cyclic(rank, idxType)) {
    var locidx: rank*idxType;
    var startIdx = dist.startIdx;

    // NOTE: Not bothering to check to see if these can fit into idxType
    if rank == 1 then
      locidx(1) = locid:idxType;
    else
      for param i in 1..rank do locidx(i) = locid(i):idxType;

    var inds: rank*range(idxType, stridable=true);

    type strType = chpl__signedType(idxType);
    // NOTE: Not checking for overflow here when casting to strType
    for param i in 1..rank {
      const lower = min(idxType)..(startIdx(i)+locidx(i)) by -dist.targetLocDom.dim(i).length:strType;
      const upper = (startIdx(i) + locidx(i))..max(idxType) by dist.targetLocDom.dim(i).length:strType;
      const lo = lower.last, hi = upper.last;
      inds(i) = lo..hi by dist.targetLocDom.dim(i).length:strType;
    }
    myChunk = {(...inds)};
  }
}


class CyclicDom : BaseRectangularDom {
  const dist: Cyclic(rank, idxType);

  var locDoms: [dist.targetLocDom] LocCyclicDom(rank, idxType, stridable);

  var whole: domain(rank, idxType, stridable);
}


proc CyclicDom.setup() {
  if locDoms(dist.targetLocDom.low) == nil {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) do
        locDoms(localeIdx) = new LocCyclicDom(rank, idxType, stridable, dist.getChunk(whole, localeIdx));
    }
  } else {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) {
        var chunk = dist.getChunk(whole, localeIdx);
        locDoms(localeIdx).myBlock = chunk;
      }
    }
  }
}

proc CyclicDom.dsiDestroyDom() {
    coforall localeIdx in dist.targetLocDom {
      on dist.targetLocs(localeIdx) do
        delete locDoms(localeIdx);
    }
}

proc CyclicDom.dsiBuildArray(type eltType) {
  var arr = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}

proc CyclicDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
  dist.dsiDisplayRepresentation();
}

proc CyclicDom.dsiLow return whole.low;

proc CyclicDom.dsiHigh return whole.high;

proc CyclicDom.dsiStride return whole.stride;

proc CyclicDom.dsiMember(i) return whole.member(i);

proc CyclicDom.dsiIndexOrder(i) return whole.indexOrder(i);

proc CyclicDom.dsiDims() return whole.dims();

proc CyclicDom.dsiDim(d: int) return whole.dim(d);

proc CyclicDom.getLocDom(localeIdx) return locDoms(localeIdx);

proc CyclicDom.dsiMyDist() return dist;



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

proc CyclicDom.dsiNumIndices return whole.numIndices;

iter CyclicDom.these() {
  for i in whole do
    yield i;
}

iter CyclicDom.these(param tag: iterKind) where tag == iterKind.leader {
  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;
  const wholeLow = whole.low;
  coforall locDom in locDoms do on locDom {
    const (numTasks, parDim) = _computeChunkStuff(maxTasks, ignoreRunning,
                                                  minSize,
                                                  locDom.myBlock.dims());

    var result: rank*range(idxType=idxType, stridable=true);
    // Use the internal function for untranslate to avoid having to do
    // extra work to negate the offset
    var zeroedLocalPart = whole((...locDom.myBlock.getIndices())).chpl__unTranslate(wholeLow);
    for param i in 1..rank {
      var dim = zeroedLocalPart.dim(i);
      type strType = chpl__signedType(idxType);
      // NOTE: unsigned idxType with negative stride will not work
      const wholestride = whole.dim(i).stride:strType;
      if dim.last >= dim.first then
        result(i) = (dim.first / wholestride:idxType)..(dim.last / wholestride:idxType) by (dim.stride:strType / wholestride);
      else
        // _computeChunkStuff should have produced no tasks for this
        // If this ain't going to happen, could force numTasks=0 here instead.
        assert(numTasks == 0);
    }
    if numTasks == 1 {
      if debugCyclicDist then
        writeln(here.id, ": leader whole: ", whole,
                         " result: ", result,
                         " myblock: ", locDom.myBlock);
      yield result;
    } else {

      coforall taskid in 0..#numTasks {
        var splitRanges: rank*range(idxType=idxType, stridable=true) = result;
        const low = result(parDim).first, high = result(parDim).high;
        const (lo,hi) = _computeBlock(high - low + 1, numTasks, taskid,
                                      high, low, low);
        // similar to BlockDist
        assert(lo <= hi);
        splitRanges(parDim) = result(parDim)(lo..hi);
        if debugCyclicDist then
          writeln(here.id, ": leader whole: ", whole,
                           " result: ", result,
                           " splitRanges: ", splitRanges);
        // remove alignment
        splitRanges(parDim) = splitRanges(parDim).first..splitRanges(parDim).last by splitRanges(parDim).stride;
        yield splitRanges;
      }
    }
  }
}

iter CyclicDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  var t: rank*range(idxType, stridable=true);
  if debugCyclicDist then
    writeln(here.id, ": follower whole is: ", whole,
                     " follower is: ", followThis);
  for param i in 1..rank {
    // NOTE: unsigned idxType with negative stride will not work
    const wholestride = whole.dim(i).stride:chpl__signedType(idxType);
    t(i) = ((followThis(i).low*wholestride:idxType)..(followThis(i).high*wholestride:idxType) by (followThis(i).stride*wholestride)) + whole.dim(i).alignedLow;
  }
  if debugCyclicDist then
    writeln(here.id, ": follower maps to: ", t);
  for i in {(...t)} do
    yield i;
}

proc CyclicDom.dsiSupportsPrivatization() param return true;

proc CyclicDom.dsiGetPrivatizeData() return 0;

proc CyclicDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, dist.pid);
  var c = new CyclicDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist);
  c.locDoms = locDoms;
  c.whole = whole;
  return c;
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
  param stridable: bool;

  var myBlock: domain(rank, idxType, true);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocCyclicDom.member(i) return myBlock.member(i);


class CyclicArr: BaseRectangularArr {
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: CyclicDom(rank, idxType, stridable);

  var locArr: [dom.dist.targetLocDom] LocCyclicArr(eltType, rank, idxType, stridable);
  var myLocArr: LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable);
  const SENTINEL = max(rank*idxType);
}

pragma "no copy return"
proc CyclicArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 1..rank {
    low(i) = ranges(i).low;
  }

  return locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
}

proc CyclicArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD.RAD);
    writeln("locArr[", tli, "].locCyclicRAD = ", locArr[tli].locCyclicRAD);
  }
  dom.dsiDisplayRepresentation();
}

proc CyclicArr.dsiGetBaseDom() return dom;

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
          myLocArr.locRAD = new LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
          myLocArr.locCyclicRAD = new LocCyclicRADCache(rank, idxType, dom.dist.startIdx, dom.dist.targetLocDom);
          for l in dom.dist.targetLocDom {
            if l != localeIdx {
              myLocArr.locRAD.RAD(l) = locArr(l).myElems._value.dsiGetRAD();
            }
          }
        }
      }
    }
  }
}

proc CyclicArr.setup() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      locArr(localeIdx) = new LocCyclicArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx));
      if this.locale == here then
        myLocArr = locArr(localeIdx);
    }
  }
  if doRADOpt && disableCyclicLazyRAD then setupRADOpt();
}

proc CyclicArr.dsiDestroyArr() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      delete locArr(localeIdx);
    }
  }
}

proc CyclicArr.dsiSupportsPrivatization() param return true;

proc CyclicArr.dsiGetPrivatizeData() return 0;

proc CyclicArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  var c = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
  c.locArr = locArr;
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
    for param i in 1..rank do {
      sum += (((ind(i) - off(i)) * blk(i))-startIdx(i))/dimLen(i);
    }
  }
  return sum;
}

proc CyclicArr.dsiAccess(i:rank*idxType) ref {
  local {
    if myLocArr != nil && myLocArr.locDom.member(i) then
      return myLocArr.this(i);
  }
  if doRADOpt && !stridable {
    if myLocArr {
      if boundsChecking then
        if !dom.dsiMember(i) then
          halt("array index out of bounds: ", i);
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableCyclicLazyRAD {
        if myLocArr.locRAD == nil {
          myLocArr.lockLocRAD();
          if myLocArr.locRAD == nil {
            var tempLocRAD = new LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
            myLocArr.locCyclicRAD = new LocCyclicRADCache(rank, idxType, dom.dist.startIdx, dom.dist.targetLocDom);
            tempLocRAD.RAD.blk = SENTINEL;
            myLocArr.locRAD = tempLocRAD;
          }
          myLocArr.unlockLocRAD();
        }
        if myLocArr.locRAD.RAD(rlocIdx).blk == SENTINEL {
          myLocArr.locRAD.lockRAD(rlocIdx);
          if myLocArr.locRAD.RAD(rlocIdx).blk == SENTINEL {
            myLocArr.locRAD.RAD(rlocIdx) =
              locArr(rlocIdx).myElems._value.dsiGetRAD();
          }
          myLocArr.locRAD.unlockRAD(rlocIdx);
        }
      }
      pragma "no copy" pragma "no auto destroy" var myLocRAD = myLocArr.locRAD;
      pragma "no copy" pragma "no auto destroy" var radata = myLocRAD.RAD;
      if radata(rlocIdx).data != nil {
        const startIdx = myLocArr.locCyclicRAD.startIdx;
        const dimLength = myLocArr.locCyclicRAD.targetLocDomDimLength;
        type strType = chpl__signedType(idxType);
        var str: rank*strType;
        for param i in 1..rank {
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

iter CyclicArr.these() ref {
  for i in dom do
    yield dsiAccess(i);
}

iter CyclicArr.these(param tag: iterKind) where tag == iterKind.leader {
  for followThis in dom.these(tag) do
    yield followThis;
}

proc CyclicArr.dsiStaticFastFollowCheck(type leadType) param
  return leadType == this.type || leadType == this.dom.type;

proc CyclicArr.dsiDynamicFastFollowCheck(lead: [])
  return lead.domain._value == this.dom;

proc CyclicArr.dsiDynamicFastFollowCheck(lead: domain)
  return lead._value == this.dom;

iter CyclicArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for Cyclic array");

  var t: rank*range(idxType=idxType, stridable=true);
  for param i in 1..rank {
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
  const myFollowThis = {(...t)};
  if fast {
    const arrSection = locArr(dom.dist.targetLocsIdx(myFollowThis.low));

    //
    // Slicing arrSection.myElems will require reference counts to be updated.
    // If myElems is an array of arrays, the inner array's domain or dist may
    // live on a different locale and require communication for reference
    // counting. Simply put: don't slice inside a local block.
    //
    // TODO: Can myLocArr be used here to simplify things?
    //
    ref chunk = arrSection.myElems(myFollowThis);
    if arrSection.locale.id == here.id then local {
      for i in chunk do yield i;
    } else {
      for i in chunk do yield i;
    }
  } else {
    proc accessHelper(i) ref {
      if myLocArr then local {
        if myLocArr.locDom.member(i) then
          return myLocArr.this(i);
      }
      return dsiAccess(i);
    }

    for i in myFollowThis {
      yield accessHelper(i);
    }
  }
}

proc CyclicArr.dsiSerialWrite(f) {
  if verboseCyclicDistWriters {
    writeln(this.type:string);
    writeln("------");
  }
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:idxType) {
      f.write(" ");
      i(rank) += dom.dsiDim(rank).stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:idxType) {
          i(dim) += dom.dsiDim(dim).stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dsiDim(dim2).low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

proc CyclicArr.dsiReallocate(bounds:rank*range(idxType,BoundedRangeType.bounded,stridable)) {
  // The reallocation happens when the LocCyclicDom.myBlock field is changed
  // in CyclicDom.setup(). Nothing more needs to happen here.
}

proc CyclicArr.dsiPostReallocate() {
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
  param stridable: bool;

  const locDom: LocCyclicDom(rank, idxType, stridable);

  var locRAD: LocRADCache(eltType, rank, idxType, stridable); // non-nil if doRADOpt=true
  var locCyclicRAD: LocCyclicRADCache(rank, idxType); // see below for why
  var myElems: [locDom.myBlock] eltType;
  var locRADLock: atomicbool; // This will only be accessed locally, so
                              // force the use of processor atomics

  // These functions will always be called on this.locale, and so we do
  // not have an on statement around the while loop below (to avoid
  // the repeated on's from calling testAndSet()).
  inline proc lockLocRAD() {
    while locRADLock.testAndSet() do chpl_task_yield();
  }

  inline proc unlockLocRAD() {
    locRADLock.clear();
  }

  proc deinit() {
    if locRAD != nil then
      delete locRAD;
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

  proc LocCyclicRADCache(param rank: int, type idxType, startIdx, targetLocDom) {
    for param i in 1..rank do
      // NOTE: Not bothering to check to see if length can fit into idxType
      targetLocDomDimLength(i) = targetLocDom.dim(i).length:idxType;
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
      if regionDest.numIndices > 0 {
        const ini = bulkCommConvertCoordinate(regionDest.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(regionDest.last, destDom, srcDom);
        const sb  = chpl__tuplify(regionSrc.stride);

        var r1,r2: rank * range(idxType = el,stridable = true);
        r2 = regionDest.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 1..rank {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]);
          if r1[t].length != r2[t].length then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
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
      if inters.numIndices > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, srcDom, destDom);
        const end = bulkCommConvertCoordinate(inters.last, srcDom, destDom);
        const sa  = chpl__tuplify(destDom.stride);

        //r2 is the domain to refer the elements of A in locale j
        //r1 is the domain to refer the corresponding elements of Dest
        var r1,r2: rank * range(idxType = el,stridable = true);
        r2 = inters.dims();

        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 1..rank {
          r1[t] = (ini[t]:el..end[t]:el by sa[t]);
          if r1[t].length != r2[t].length then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
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
      if inters.numIndices > 0 {
        const ini = bulkCommConvertCoordinate(inters.first, destDom, srcDom);
        const end = bulkCommConvertCoordinate(inters.last, destDom, srcDom);
        const sb  = chpl__tuplify(srcDom.stride);

        var r1,r2: rank * range(idxType = el,stridable = true);
        r2 = inters.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 1..rank {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]);
          if r1[t].length != r2[t].length then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
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

proc CyclicArr.dsiLocalSubdomain() {
  return myLocArr.locDom.myBlock;
}
proc CyclicDom.dsiLocalSubdomain() {
  // TODO -- could be replaced by a privatized myLocDom in CyclicDom
  // as it is with CyclicArr
  var myLocDom:LocCyclicDom(rank, idxType, stridable) = nil;
  for (loc, locDom) in zip(dist.targetLocs, locDoms) {
    if loc == here then
      myLocDom = locDom;
  }
  return myLocDom.myBlock;
}
