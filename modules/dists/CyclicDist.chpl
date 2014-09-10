/*
 * Copyright 2004-2014 Cray Inc.
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

  var pid: int = -1;

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

  proc Cyclic(param rank, type idxType, other: Cyclic(rank, idxType)) {
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    startIdx = other.startIdx;
    locDist = other.locDist;
    dataParTasksPerLocale = other.dataParTasksPerLocale;
    dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
    dataParMinGranularity = other.dataParMinGranularity;
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

  proc dsiClone() return new Cyclic(rank=rank, idxType=idxType, other=this);
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
  return new Cyclic(rank=rank, idxType=idxType, other=this);
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

proc Cyclic.dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool) {
  if idxType != this.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Cyclic domain rank does not match distribution's");
  var dom = new CyclicDom(rank=rank, idxType=idxType, dist = this, stridable=stridable);
  dom.setup();
  return dom;
} 

proc Cyclic.dsiCreateReindexDist(newSpace, oldSpace) {
  proc anyStridable(space, param i=1) param
    return if i == space.size
      then space(i).stridable
      else space(i).stridable || anyStridable(space, i+1);

  if anyStridable(newSpace) || anyStridable(oldSpace) then
    compilerWarning("reindexing stridable Cyclic arrays is not yet fully supported");

  var newLow: rank*idxType;
  for param i in 1..rank {
    newLow(i) = newSpace(i).low - oldSpace(i).low + startIdx(i);
  }
  var newDist = new Cyclic(rank=rank, idxType=idxType, startIdx=newLow,
                           targetLocales=targetLocs,
                           dataParTasksPerLocale=dataParTasksPerLocale,
                           dataParIgnoreRunningTasks=dataParIgnoreRunningTasks,
                           dataParMinGranularity=dataParMinGranularity);
  return newDist;
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

proc Cyclic.dsiCreateRankChangeDist(param newRank: int, args) {
  var collapsedDimInds: rank*idxType;
  var collapsedLocs: _cyclic_matchArgsShape(range(int), int, args);
  var newLow: newRank*idxType;

  var j: int = 1;
  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      collapsedDimInds(i) = args(i);
    else {
      newLow(j) = startIdx(i);
      j += 1;
    }
  }
  const partialLocIdx = targetLocsIdx(collapsedDimInds);


  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) {
      collapsedLocs(i) = partialLocIdx(i);
} else {
      collapsedLocs(i) = targetLocDom.dim(i);
    }
  }
  var newTargetLocales = targetLocs[(...collapsedLocs)];
  return new Cyclic(rank=newRank, idxType=idxType, startIdx=newLow, targetLocales=newTargetLocales);
}

proc Cyclic.writeThis(x: Writer) {
  x.writeln(typeToString(this.type));
  x.writeln("------");
  for locid in targetLocDom do
    x.writeln(" [", locid, "=", targetLocs(locid), "] owns chunk: ", locDist(locid).myChunk); 
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
  param rank: int;
  type idxType;
  param stridable: bool;

  const dist: Cyclic(rank, idxType);

  var locDoms: [dist.targetLocDom] LocCyclicDom(rank, idxType, stridable);

  var whole: domain(rank, idxType, stridable);

  var pid: int = -1;
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

proc CyclicDom.dsiSerialWrite(x: Writer) {
  if verboseCyclicDistWriters {
    x.writeln(typeToString(this.type));
    x.writeln("------");
    for loc in dist.targetLocDom {
      x.writeln("[", loc, "=", dist.targetLocs(loc), "] owns ", locDoms(loc).myBlock);
    }
  } else {
    x.write(whole);
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
    t(i) = ((followThis(i).low*wholestride:idxType)..(followThis(i).high*wholestride:idxType) by (followThis(i).stride*wholestride)) + whole.dim(i).low;
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

proc CyclicDom.dsiBuildRectangularDom(param rank, type idxType,
                                    param stridable: bool,
                                    ranges: rank*range(idxType,
                                                       BoundedRangeType.bounded,
                                                       stridable)) {
  if idxType != dist.idxType then
    compilerError("Cyclic domain index type does not match distribution's");
  if rank != dist.rank then
    compilerError("Cyclic domain rank does not match distribution's");

  var dom = new CyclicDom(rank=rank, idxType=idxType,
                         dist=dist, stridable=stridable);
  dom.dsiSetIndices(ranges);
  return dom;

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


class CyclicArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: CyclicDom(rank, idxType, stridable);

  var locArr: [dom.dist.targetLocDom] LocCyclicArr(eltType, rank, idxType, stridable);
  var myLocArr: LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable);
  var pid: int = -1;
  const SENTINEL = max(rank*idxType);
}

proc CyclicArr.dsiSlice(d: CyclicDom) {
  var alias = new CyclicArr(eltType=eltType, rank=rank, idxType=idxType,
                            stridable=d.stridable, dom=d);
  var thisid = this.locale.id;
  for i in dom.dist.targetLocDom {
    on dom.dist.targetLocs(i) {
      alias.locArr[i] =
        new LocCyclicArr(eltType=eltType, rank=rank, idxType=idxType,
                         stridable=d.stridable, locDom=d.locDoms[i],
                         myElems=>locArr[i].myElems[d.locDoms[i].myBlock]);
      if thisid == here.id then
        alias.myLocArr = alias.locArr[i];
    }
  }
  if doRADOpt then alias.setupRADOpt();
  return alias;
}

proc CyclicArr.dsiRankChange(d, param newRank: int, param stridable: bool, args) {
  var alias = new CyclicArr(eltType=eltType, rank=newRank, idxType=idxType, stridable=stridable, dom = d);
  var thisid = this.locale.id;
  coforall ind in d.dist.targetLocDom {
    on d.dist.targetLocs(ind) {
      const locDom = d.getLocDom(ind);
      var collapsedDims: rank*idxType;
      var locSlice: _cyclic_matchArgsShape(range(idxType=idxType, stridable=true), idxType, args);
      var locArrInd: rank*int;
      var j = 1;
      for param i in 1..args.size {
        if isCollapsedDimension(args(i)) {
          locSlice(i) = args(i);
          collapsedDims(i) = args(i);
        } else {
          locSlice(i) = locDom.myBlock.dim(j)(args(i));
          j += 1;
        }
      }
      locArrInd = dom.dist.targetLocsIdx(collapsedDims);
      j = 1;
      // Now that the locArrInd values are known for the collapsed dimensions
      // Pull the rest of the dimensions values from ind
      for param i in 1..args.size {
        if !isCollapsedDimension(args(i)) {
          if newRank > 1 then
            locArrInd(i) = ind(j);
          else
            locArrInd(i) = ind;
          j += 1;
        }
      }
      alias.locArr[ind] =
        new LocCyclicArr(eltType=eltType, rank=newRank, idxType=d.idxType,
                         stridable=d.stridable, locDom=locDom,
                         myElems=>locArr[(...locArrInd)].myElems[(...locSlice)]);
      if here.id == thisid then
        alias.myLocArr = alias.locArr[ind];
    }
  }
  if doRADOpt then alias.setupRADOpt();
  return alias;
}

proc CyclicArr.dsiReindex(d: CyclicDom) {
  var alias = new CyclicArr(eltType=eltType, rank=rank, idxType=d.idxType,
                            stridable=d.stridable, dom=d);
  const sameDom = d==dom;
  var thisid = this.locale.id;
  coforall i in dom.dist.targetLocDom {
    on dom.dist.targetLocs(i) {
      const locDom = d.getLocDom(i);
      const locAlias: [locDom.myBlock] => locArr[i].myElems;
      alias.locArr[i] = new LocCyclicArr(eltType=eltType, idxType=idxType,
                                         locDom=locDom, stridable=d.stridable,
                                         rank=rank, myElems=>locAlias);
      if thisid == here.id then
        alias.myLocArr = alias.locArr[i];
      if doRADOpt {
        if sameDom {
          // If we the reindex domain is the same as that of this array,
          //  the RAD cache will be the same you can just copy the values
          //  directly into the alias's RAD cache
          if locArr[i].locRAD {
            alias.locArr[i].locRAD = new LocRADCache(eltType, rank, idxType,
                                                     dom.dist.targetLocDom);
            alias.locArr[i].locCyclicRAD = new LocCyclicRADCache(rank, idxType,
                                                                 dom.dist.startIdx,
                                                                 dom.dist.targetLocDom);
            alias.locArr[i].locRAD.RAD = locArr[i].locRAD.RAD;
          }
        }
      }
    }
  }
  if doRADOpt then
    if !sameDom then alias.setupRADOpt();
  return alias;
}

proc CyclicArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 1..rank {
    low(i) = ranges(i).low;
  }

  var A => locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
  return A;
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
          myLocArr.locRAD = new LocRADCache(eltType, rank, idxType, dom.dist.targetLocDom);
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


inline proc _remoteAccessData.getDataIndex(param stridable, myStr: rank*idxType, ind: rank*idxType, startIdx, dimLen) {
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
            var tempLocRAD = new LocRADCache(eltType, rank, idxType, dom.dist.targetLocDom);
            myLocArr.locCyclicRAD = new LocCyclicRADCache(rank, idxType, dom.dist.startIdx, dom.dist.targetLocDom);
            tempLocRAD.RAD.blk = SENTINEL;
            myLocArr.locRAD = tempLocRAD;
          }
          myLocArr.unlockLocRAD();
        }
        // NOTE: This is a known, benign race.  Multiple tasks may be
        // initializing the RAD cache entries at once, but our belief is
        // that this is infrequent enough that the potential extra gets
        // are worth *not* having to synchronize.  If this turns out to be
        // an incorrect assumption, we can add an atomic variable and use
        // a fetchAdd to decide which task does the update.
        if myLocArr.locRAD.RAD(rlocIdx).blk == SENTINEL {
          myLocArr.locRAD.RAD(rlocIdx) = locArr(rlocIdx).myElems._value.dsiGetRAD();
        }  
      }
      pragma "no copy" pragma "no auto destroy" var myLocRAD = myLocArr.locRAD;
      pragma "no copy" pragma "no auto destroy" var radata = myLocRAD.RAD;
      if radata(rlocIdx).data != nil {
        const startIdx = myLocArr.locCyclicRAD.startIdx;
        const dimLength = myLocArr.locCyclicRAD.targetLocDomDimLength;
        var str: rank*idxType;
        for param i in 1..rank {
          pragma "no copy" pragma "no auto destroy" var whole = dom.whole;
          str(i) = whole.dim(i).stride;
        }
        var dataIdx = radata(rlocIdx).getDataIndex(stridable, str, i, startIdx, dimLength);
        return radata(rlocIdx).data(dataIdx);
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
    // NOTE: unsigned idxType with negative stride will not work
    const wholestride = dom.whole.dim(i).stride:chpl__signedType(idxType);
    t(i) = ((followThis(i).low*wholestride):idxType..(followThis(i).high*wholestride):idxType by (followThis(i).stride*wholestride)) + dom.whole.dim(i).low;
  }
  const myFollowThis = {(...t)};
  if fast {
    const arrSection = locArr(dom.dist.targetLocsIdx(myFollowThis.low));
    if arrSection.locale.id == here.id then local {
      for e in arrSection.myElems(myFollowThis) do
        yield e;
    } else {
      for e in arrSection.myElems(myFollowThis) do
        yield e;
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

proc CyclicArr.dsiSerialWrite(f: Writer) {
  if verboseCyclicDistWriters {
    writeln(typeToString(this.type));
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

proc CyclicArr.dsiReallocate(d: domain) {
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

  var locRAD: LocRADCache(eltType, rank, idxType); // non-nil if doRADOpt=true
  var locCyclicRAD: LocCyclicRADCache(rank, idxType); // see below for why
  var myElems: [locDom.myBlock] eltType;
  var locRADLock: atomicflag; // This will only be accessed locally, so
                              // force the use of processor atomics

  // These function will always be called on this.locale, and so we do
  // not have an on statement around the while loop below (to avoid
  // the repeated on's from calling testAndSet()).
  inline proc lockLocRAD() {
    while locRADLock.testAndSet() do chpl_task_yield();
  }

  inline proc unlockLocRAD() {
    locRADLock.clear();
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

proc CyclicArr.dsiSupportsBulkTransferInterface() param return true;

proc CyclicArr.doiCanBulkTransferStride() param {
  if debugCyclicDistBulkTransfer then
    writeln("In CyclicArr.doiCanBulkTransferStride");

  // A CyclicArr is a bunch of DefaultRectangular arrays,
  // so strided bulk transfer gotta be always possible.
  return true;
}

//For assignments of the form: "any = Cyclic"
//Currently not used, instead we use: doiBulkTransferFrom()
proc CyclicArr.doiBulkTransferTo(Barg)
{
  if debugCyclicDistBulkTransfer then
    writeln("In CyclicArr.doiBulkTransferTo()");
  
  const B = this, A = Barg._value;
  type el = B.idxType;
  coforall i in B.dom.dist.targetLocDom do // for all locales
    on B.dom.dist.targetLocs(i)
      {
        var regionA = B.dom.locDoms(i).myBlock;
        if regionA.numIndices>0
        {
          const ini=bulkCommConvertCoordinate(regionA.first, B, A);
          const end=bulkCommConvertCoordinate(regionA.last, B, A);
          const sb=chpl__tuplify(A.dom.locDoms(i).myBlock.stride);
          
          var r1,r2: rank * range(idxType = el,stridable = true);
          r2=regionA.dims();
           //In the case that the number of elements in dimension t for r1 and r2
           //were different, we need to calculate the correct stride in r1
          for param t in 1..rank
          {
            r1[t] = (ini[t]:el..end[t]:el by sb[t]:el);
            if r1[t].length != r2[t].length then
              r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
          }
        
          if debugCyclicDistBulkTransfer then
            writeln("A",(...r1),".FromDR",regionA);
    
          Barg[(...r1)]._value.doiBulkTransferFromDR(B.locArr[i].myElems);
        }
      }
}


//For assignments of the form: "Cyclic = any" 
//where "any" means any array that implements the bulk transfer interface
proc CyclicArr.doiBulkTransferFrom(Barg)
{
  if debugCyclicDistBulkTransfer then
    writeln("In CyclicArr.doiBulkTransferFrom()");
  
  const A = this, B = Barg._value;
  type el = A.idxType;
  coforall i in A.dom.dist.targetLocDom do // for all locales
    on A.dom.dist.targetLocs(i)
    { 
      var regionA = A.dom.locDoms(i).myBlock;    
      if regionA.numIndices>0
      {
        const ini=bulkCommConvertCoordinate(regionA.first, A, B);
        const end=bulkCommConvertCoordinate(regionA.last, A, B);
        const sb=chpl__tuplify(B.dom.locDoms(i).myBlock.stride);
      
        var r1,r2: rank * range(idxType = el,stridable = true);
        r2=regionA.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 1..rank
        {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]:el);
          if r1[t].length != r2[t].length then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
        }
       
        if debugCyclicDistBulkTransfer then
          writeln("B[",(...r1),"] ToDR A[",regionA, "] ");
        
        Barg[(...r1)]._value.doiBulkTransferToDR(A.locArr[i].myElems[regionA]);
      }
    }
}

//For assignments of the form: DR = Cyclic 
//(default rectangular array = cyclic distributed array)
proc CyclicArr.doiBulkTransferToDR(Barg)
{
  if debugCyclicDistBulkTransfer then
    writeln("In CyclicArr.doiBulkTransferToDR()");
  
  const A = this, B = Barg._value;
  type el = A.idxType;
  coforall j in A.dom.dist.targetLocDom do // for all locales
    on A.dom.dist.targetLocs(j)
    {
      const inters=A.dom.locDoms(j).myBlock;
      if(inters.numIndices>0)
      {
        const ini=bulkCommConvertCoordinate(inters.first, A, B);
        const end=bulkCommConvertCoordinate(inters.last, A, B);
        const sa = chpl__tuplify(B.dom.dsiStride); //return a tuple
        
        //r2 is the domain to refer the elements of A in locale j
        //r1 is the domain to refer the correspondig elements of B
        var r1,r2: rank * range(idxType = el,stridable = true);
        r2=inters.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 1..rank
        {
          r1[t] = (ini[t]:el..end[t]:el by sa[t]:el);
          if r1[t].length != r2[t].length then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
        }
            
        const d ={(...r1)};
        const slice = B.dsiSlice(d._value);
        //Necessary to calculate the value of blk variable in DR
        //with the new domain r1
        slice.adjustBlkOffStrForNewDomain(d._value, slice);
     
        if debugCyclicDistBulkTransfer then 
          writeln(" A[",(...r1),"] = B[",(...r2), "]");
      
        slice.doiBulkTransferStride(A.locArr[j].myElems[(...r2)]._value);
        delete slice;
      }
    }
}

//For assignments of the form: Cyclic = DR 
//(cyclic distributed array = default rectangular)
proc CyclicArr.doiBulkTransferFromDR(Barg)
{
  if debugCyclicDistBulkTransfer then
    writeln("In CyclicArr.doiBulkTransferFromDR()");
  
  const A = this, B = Barg._value;
  type el = A.idxType;
  coforall j in A.dom.dist.targetLocDom do // for all locales
    on A.dom.dist.targetLocs(j)
    {
      const inters=A.dom.locDoms(j).myBlock;
      if(inters.numIndices>0)
      {
        const ini=bulkCommConvertCoordinate(inters.first, A, B);
        const end=bulkCommConvertCoordinate(inters.last, A, B);
        const sb = chpl__tuplify(B.dom.dsiStride); //return a tuple
        
        var r1,r2: rank * range(idxType = el,stridable = true);
        r2=inters.dims();
        //In the case that the number of elements in dimension t for r1 and r2
        //were different, we need to calculate the correct stride in r1
        for param t in 1..rank
        {
          r1[t] = (ini[t]:el..end[t]:el by sb[t]:el);
          if r1[t].length != r2[t].length then
            r1[t] = (ini[t]:el..end[t]:el by (end[t] - ini[t]):el/(r2[t].length-1));
        }
        
        if debugCyclicDistBulkTransfer then
            writeln("A[",(...r2),"] = B[",(...r1), "] ");
        
        const d ={(...r1)};
        const slice = B.dsiSlice(d._value);
        //this step it's necessary to calculate the value of blk variable in DR
        //with the new domain r1
        slice.adjustBlkOffStrForNewDomain(d._value, slice);
      
        A.locArr[j].myElems[(...r2)]._value.doiBulkTransferStride(slice);
        delete slice;
      }
    }
}
    
proc CyclicArr.dsiTargetLocDom() {
  return dom.dist.targetLocDom;
}

proc CyclicArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

// Cyclic subdomains are represented as a single domain

proc CyclicArr.dsiOneLocalSubdomain() param return true;

proc CyclicArr.dsiGetLocalSubdomain() {
  return myLocArr.locDom.myBlock;
}
