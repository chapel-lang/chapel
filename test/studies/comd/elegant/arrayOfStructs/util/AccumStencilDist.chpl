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

use BlockDist;
use DSIUtil;
use ChapelUtil;
use CommDiagnostics;
use ChapelLocks;
use ChapelDebugPrint;
private use HaltWrappers;

//
// These flags are used to output debug information and run extra
// checks when using AccumStencil.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugAccumStencilDist = false;
config param debugAccumStencilDistBulkTransfer = false;

// Re-uses these flags from BlockDist:
//   - disableAliasedBulkTransfer
//   - sanityCheckDistribution
//   - testFastFollowerOptimization

//
// This flag is used to disable lazy initialization of the RAD cache.
//
config param disableAccumStencilLazyRAD = defaultDisableLazyRADOpt;

class AccumStencil : BaseDist {
  param rank: int;
  type idxType = int;
  param ignoreFluff: bool;
  var boundingBox: domain(rank, idxType);
  var fluff: rank*idxType;
  var periodic: bool = false;
  var targetLocDom: domain(rank);
  var targetLocales: [targetLocDom] locale;
  var locDist: [targetLocDom] unmanaged LocAccumStencil(rank, idxType);
  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;
}

//
// Local AccumStencil Distribution Class
//
// rank : generic rank that matches AccumStencil.rank
// idxType: generic index type that matches AccumStencil.idxType
// myChunk: a non-distributed domain that defines this locale's indices
//
class LocAccumStencil {
  param rank: int;
  type idxType;
  const myChunk: domain(rank, idxType);
}

//
// AccumStencil Domain Class
//
// rank:      generic domain rank
// idxType:   generic domain index type
// stridable: generic domain stridable parameter
// dist:      reference to distribution class
// locDoms:   a non-distributed array of local domain classes
// whole:     a non-distributed domain that defines the domain's indices
//
class AccumStencilDom: BaseRectangularDom {
  param ignoreFluff : bool;
  const dist: unmanaged AccumStencil(rank, idxType, ignoreFluff);
  var locDoms: [dist.targetLocDom] unmanaged LocAccumStencilDom(rank, idxType, stridable);
  var whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  var fluff: rank*idxType;
  var periodic: bool = false;
  var wholeFluff : domain(rank=rank, idxType=idxType, stridable=stridable);
}

//
// Local AccumStencil Domain Class
//
// rank: generic domain rank
// idxType: generic domain index type
// stridable: generic domain stridable parameter
// myBlock: a non-distributed domain that defines the local indices
//
// NeighDom will be a rectangular domain where each dimension is the range
// -1..1
//
class LocAccumStencilDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var myBlock, myFluff: domain(rank, idxType, stridable);
  var recvDom : domain(1);
}

//
// AccumStencil Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// dom: reference to domain class
// locArr: a non-distributed array of local array classes
// myLocArr: optimized reference to here's local array class (or nil)
//
class AccumStencilArr: BaseRectangularArr {
  param ignoreFluff: bool;
  var doRADOpt: bool = defaultDoRADOpt;
  var dom: unmanaged AccumStencilDom(rank, idxType, stridable, ignoreFluff);
  var locArr: [dom.dist.targetLocDom] unmanaged LocAccumStencilArr(eltType, rank, idxType, stridable);
  pragma "local field"
  var myLocArr: unmanaged LocAccumStencilArr(eltType, rank, idxType, stridable)?;
  const SENTINEL = max(rank*idxType);
}

//
// Local AccumStencil Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// locDom: reference to local domain class
// myElems: a non-distributed array of local elements
//
class LocAccumStencilArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  const locDom: unmanaged LocAccumStencilDom(rank, idxType, stridable);
  var locRAD: unmanaged LocRADCache(eltType, rank, idxType, stridable)?; // non-nil if doRADOpt=true

  pragma "local field" pragma "unsafe" pragma "no auto destroy"
  // may be initialized separately
  // always destroyed explicitly (to control deiniting elts)
  var myElems: [locDom.myFluff] eltType;

  var locRADLock: chpl__processorAtomicType(bool); // only accessed locally

  var recvM, recvP : [locDom.recvDom] eltType;
  var recvMFlag, recvPFlag : atomic bool;

  proc init(type eltType,
            param rank: int,
            type idxType,
            param stridable: bool,
            const locDom: unmanaged LocAccumStencilDom(rank, idxType, stridable),
            param initElts: bool) {
    this.eltType = eltType;
    this.rank = rank;
    this.idxType = idxType;
    this.stridable = stridable;
    this.locDom = locDom;
    this.myElems = this.locDom.myFluff.buildArray(eltType, initElts=initElts);

    // Even if the array elements don't need to be initialized now,
    // do initialize the fluff.
    if initElts == false {
      if this.locDom.myBlock != this.locDom.myFluff {
        forall i in this.locDom.myFluff {
          if !this.locDom.contains(i) {
            pragma "no auto destroy" pragma "unsafe"
            var def: eltType;
            __primitive("=", myElems[i], def);
          }
        }
      }
    }
  }

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
    // Even if the array elements don't need to be de-initialized now,
    // do de-initialize the fluff.
    param needsDestroy = __primitive("needs auto destroy", eltType);
    if needsDestroy {
      if this.locDom.myBlock != this.locDom.myFluff {
        forall i in this.locDom.myFluff {
          if !this.locDom.contains(i) {
            chpl__autoDestroy(myElems[i]);
          }
        }
      }
    }

    // Elements in myElems are deinited in dsiDestroyArr if necessary.
    // Here we need to clean up the rest of the array.
    _do_destroy_array(myElems, deinitElts=false);

    if locRAD != nil then
      delete locRAD;
  }
}

private proc makeZero(param rank : int, type idxType) {
  var ret : rank*idxType;
  return ret;
}
//
// AccumStencil constructor for clients of the AccumStencil distribution
//
proc AccumStencil.init(boundingBox: domain,
                targetLocales: [] locale = Locales,
                dataParTasksPerLocale=getDataParTasksPerLocale(),
                dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
                dataParMinGranularity=getDataParMinGranularity(),
                param rank = boundingBox.rank,
                type idxType = boundingBox.idxType,
                fluff: rank*idxType = makeZero(rank, idxType),
                periodic: bool = false,
                param ignoreFluff = false) {
  if rank != boundingBox.rank then
    compilerError("specified AccumStencil rank != rank of specified bounding box");
  if idxType != boundingBox.idxType then
    compilerError("specified AccumStencil index type != index type of specified bounding box");

  this.rank = rank;
  this.idxType = idxType;
  this.ignoreFluff = ignoreFluff;

  this.boundingBox = boundingBox : domain(rank, idxType, stridable=false);
  this.fluff = fluff;

  // can't have periodic if there's no fluff
  this.periodic = periodic && !isZeroTuple(fluff);

  const ranges = setupTargetLocRanges(rank, targetLocales);
  this.targetLocDom = {(...ranges)};
  this.targetLocales = reshape(targetLocales, this.targetLocDom);

  const dummyLAS = new unmanaged LocAccumStencil(rank, idxType, dummy=true);
  var locDistTemp: [this.targetLocDom] unmanaged LocAccumStencil(rank, idxType)
        = dummyLAS;

  const boundingBoxDims = this.boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();
  coforall locid in targetLocDom do
    on this.targetLocales(locid) do
      locDistTemp(locid) =  new unmanaged LocAccumStencil(rank, idxType, locid, boundingBoxDims,
                                     targetLocDomDims);
  delete dummyLAS;
  locDist = locDistTemp;

  // NOTE: When these knobs stop using the global defaults, we will need
  // to add checks to make sure dataParTasksPerLocale<0 and
  // dataParMinGranularity<0
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0
                               then here.maxTaskPar
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;

  this.complete();

  if debugAccumStencilDist {
    writeln("Creating new AccumStencil distribution:");
    dsiDisplayRepresentation();
  }
}

proc AccumStencil.dsiAssign(other: _to_unmanaged(this.type)) {
  coforall locid in targetLocDom do
    on targetLocales(locid) do
      delete locDist(locid);
  boundingBox = other.boundingBox;
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
  const boundingBoxDims = boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();

  coforall locid in targetLocDom do
    on targetLocales(locid) do
      locDist(locid) = new unmanaged LocAccumStencil(rank, idxType, locid, boundingBoxDims,
                                    targetLocDomDims);
}

//
// AccumStencil distributions are equivalent if they share the same bounding
// box and target locale set.
//
proc AccumStencil.dsiEqualDMaps(that: unmanaged AccumStencil(?)) {
  return (this.boundingBox == that.boundingBox &&
          this.targetLocales.equals(that.targetLocales) &&
          this.fluff == that.fluff &&
          this.periodic == that.periodic);
}


proc AccumStencil.dsiEqualDMaps(that) param {
  return false;
}



proc AccumStencil.dsiClone() {
  return new unmanaged AccumStencil(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff);
}

override proc AccumStencil.dsiDestroyDist() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

proc AccumStencil.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

override proc AccumStencil.dsiNewRectangularDom(param rank: int, type idxType,
                                  param stridable: bool, inds) {
  if idxType != this.idxType then
    compilerError("AccumStencil domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("AccumStencil domain rank does not match distribution's");

  const dist = this;
  const dummyLACSD = new unmanaged LocAccumStencilDom(rank, idxType, stridable);
  var locDomsTemp: [dist.targetLocDom] unmanaged LocAccumStencilDom(rank, idxType, stridable) = dummyLACSD;
  coforall localeIdx in dist.targetLocDom do
    on dist.targetLocales(localeIdx) do
      locDomsTemp(localeIdx) = new unmanaged LocAccumStencilDom(rank, idxType, stridable);
  delete dummyLACSD;

  var dom = new unmanaged AccumStencilDom(rank=rank, idxType=idxType, dist=_to_unmanaged(this), stridable=stridable, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff, locDoms = locDomsTemp);

  dom.dsiSetIndices(inds);
  if debugAccumStencilDist {
    writeln("Creating new AccumStencil domain:");
    dom.dsiDisplayRepresentation();
  }
  return dom;
}

//
// output distribution
//
proc AccumStencil.writeThis(x) throws {
  x.writeln("AccumStencil");
  x.writeln("-------");
  x.writeln("distributes: ", boundingBox);
  x.writeln("across locales: ", targetLocales);
  x.writeln("indexed via: ", targetLocDom);
  x.writeln("resulting in: ");
  for locid in targetLocDom do
    x.writeln("  [", locid, "] locale ", locDist(locid).locale.id, " owns chunk: ", locDist(locid).myChunk);
}

proc AccumStencil.dsiIndexToLocale(ind: idxType) where rank == 1 {
  return targetLocales(targetLocsIdx(ind));
}

proc AccumStencil.dsiIndexToLocale(ind: rank*idxType) where rank > 1 {
  return targetLocales(targetLocsIdx(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
proc AccumStencil.getChunk(inds, locid) {
  // use domain slicing to get the intersection between what the
  // locale owns and the domain's index set
  //
  // TODO: Should this be able to be written as myChunk[inds] ???
  //
  // TODO: Does using David's detupling trick work here?
  //
  const chunk = locDist(locid).myChunk((...inds.getIndices()));
  if sanityCheckDistribution then
    if chunk.size > 0 {
      if targetLocsIdx(chunk.low) != locid then
        writeln("[", here.id, "] ", chunk.low, " is in my chunk but maps to ",
                targetLocsIdx(chunk.low));
      if targetLocsIdx(chunk.high) != locid then
        writeln("[", here.id, "] ", chunk.high, " is in my chunk but maps to ",
                targetLocsIdx(chunk.high));
    }
  return chunk;
}

//
// get the index into the targetLocales array for a given distributed index
//
proc AccumStencil.targetLocsIdx(ind: idxType) where rank == 1 {
  return targetLocsIdx((ind,));
}

proc AccumStencil.targetLocsIdx(ind: rank*idxType) {
  var result: rank*int;
  for param i in 0..rank-1 do
    result(i) = max(0, min((targetLocDom.dim(i).size-1):int,
                           (((ind(i) - boundingBox.dim(i).low) *
                             targetLocDom.dim(i).size:idxType) /
                            boundingBox.dim(i).size):int));
  return if rank == 1 then result(1) else result;
}

proc AccumStencil.dsiCreateReindexDist(newSpace, oldSpace) {
  proc anyStridable(space, param i=1) param
    return if i == space.size then space(i).stridable
           else space(i).stridable || anyStridable(space, i+1);

  // Should this error be in ChapelArray or not an error at all?
  if newSpace(1).idxType != oldSpace(1).idxType then
    compilerError("index type of reindex domain must match that of original domain");
  if anyStridable(newSpace) || anyStridable(oldSpace) then
    compilerWarning("reindexing stridable AccumStencil arrays is not yet fully supported");

  /* To shift the bounding box, we must perform the following operation for
   *  each dimension:
   *
   *   bbox(r).low - (oldSpace(r).low - newSpace(r).low)
   *   bbox(r).high - (oldSpace(r).low - newSpace(r).low)
   *
   * The following is guaranteed on entry:
   *
   *   oldSpace(r).low-newSpace(r).low = oldSpace(r).high-newSpace(r).high
   *
   * We need to be able to do this without going out of range of the index
   *  type.  The approach we take is to check if there is a way to perform
   *  the calculation without having any of the intermediate results go out
   *  of range.
   *
   *    newBbLow = bbLow - (oldLow - newLow)
   *    newBbLow = bbLow - oldLow + newLow
   *
   * Can be performed as:
   *
   *    t = oldLow-newLow;
   *    newBbLow = bbLow-t;
   * or
   *    t = bbLow-oldLow;
   *    newBbLow = t+newLow;
   * or
   *    t = bbLow+newLow;
   *    newBbLow = t-oldLow;
   *
   */
  proc adjustBound(bbound, oldBound, newBound) {
    var t: bbound.type;
    if safeSub(oldBound, newBound) {
      t = oldBound-newBound;
      if safeSub(bbound, t) {
        return (bbound-t, true);
      }
    }
    if safeSub(bbound, oldBound) {
      t = bbound-oldBound;
      if safeAdd(t, newBound) {
        return (t+newBound, true);
      }
    }
    if safeAdd(bbound, newBound) {
      t = bbound+newBound;
      if safeSub(t, oldBound) {
        return(t-oldBound, true);
      }
    }
    return (bbound, false);
  }

  var myNewBbox = boundingBox.dims();
  for param r in 0..rank-1 {
    var oldLow = oldSpace(r).low;
    var newLow = newSpace(r).low;
    var oldHigh = oldSpace(r).high;
    var newHigh = newSpace(r).high;
    var valid: bool;
    if oldLow != newLow {
      (myNewBbox(r)._low,valid) = adjustBound(myNewBbox(r).low,oldLow,newLow);
      if !valid then // try with high
        (myNewBbox(r)._low,valid) = adjustBound(myNewBbox(r).low,oldHigh,newHigh);
      if !valid then
        halt("invalid reindex for AccumStencil: distribution bounding box (low) out of range in dimension ", r);

      (myNewBbox(r)._high,valid) = adjustBound(myNewBbox(r).high,oldHigh,newHigh);
      if !valid then
        (myNewBbox(r)._high,valid) = adjustBound(myNewBbox(r).high,oldLow,newLow);
      if !valid then // try with low
        halt("invalid reindex for AccumStencil: distribution bounding box (high) out of range in dimension ", r);
    }
  }
  var d = {(...myNewBbox)};
  var newDist = new unmanaged AccumStencil(d, targetLocales,
                          dataParTasksPerLocale, dataParIgnoreRunningTasks,
                          dataParMinGranularity, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff);
  return newDist;
}


proc LocAccumStencil.init(param rank: int,
                          type idxType,
                          locid, // the locale index from the target domain
                          boundingBox: rank*range(idxType),
                          targetLocBox: rank*range) {
  this.rank = rank;
  this.idxType = idxType;
  if rank == 1 {
    const lo = boundingBox(1).low;
    const hi = boundingBox(1).high;
    const numelems = hi - lo + 1;
    const numlocs = targetLocBox(1).size;
    const (blo, bhi) = _computeBlock(numelems, numlocs, locid,
                                     max(idxType), min(idxType), lo);
    myChunk = {blo..bhi};
  } else {
    var inds: rank*range(idxType);
    for param i in 0..rank-1 {
      const lo = boundingBox(i).low;
      const hi = boundingBox(i).high;
      const numelems = hi - lo + 1;
      const numlocs = targetLocBox(i).size;
      const (blo, bhi) = _computeBlock(numelems, numlocs, locid(i),
                                       max(idxType), min(idxType), lo);
      inds(i) = blo..bhi;
    }
    myChunk = {(...inds)};
  }
}

// Used to create a dummy instance.
proc LocAccumStencil.init(param rank, type idxType, param dummy: bool) where dummy {
  this.rank = rank;
  this.idxType = idxType;
}

override proc AccumStencilDom.dsiMyDist() return dist;

proc AccumStencilDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
}

proc AccumStencilDom.dsiDims() return whole.dims();

proc AccumStencilDom.dsiDim(d: int) return whole.dim(d);

// stopgap to avoid accessing locDoms field (and returning an array)
proc AccumStencilDom.getLocDom(localeIdx) return locDoms(localeIdx);


//
// Given a tuple of scalars of type t or range(t) match the shape but
// using types rangeType and scalarType e.g. the call:
// _matchArgsShape(range(int(32)), int(32), (1:int(64), 1:int(64)..5, 1:int(64)..5))
// returns the type: (int(32), range(int(32)), range(int(32)))
//
proc _matchArgsShape(type rangeType, type scalarType, args) type {
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


iter AccumStencilDom.these() {
  for i in whole do
    yield i;
}

iter AccumStencilDom.these(param tag: iterKind) where tag == iterKind.leader {
  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;
  const wholeLow = whole.low;

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
    // Use the internal function for untranslate to avoid having to do
    // extra work to negate the offset
    type strType = chpl__signedType(idxType);
    const tmpAccumStencil = locDom.myBlock.chpl__unTranslate(wholeLow);
    var locOffset: rank*idxType;
    for param i in 0..tmpAccumStencil.rank-1 do
      locOffset(i) = tmpAccumStencil.dim(i).first/tmpAccumStencil.dim(i).stride:strType;
    // Forward to defaultRectangular
    for followThis in tmpAccumStencil.these(iterKind.leader, maxTasks,
                                            myIgnoreRunning, minSize,
                                            locOffset) do
      yield followThis;
  }
}

//
// TODO: Abstract the addition of low into a function?
// Note relationship between this operation and the
// order/position functions -- any chance for creating similar
// support? (esp. given how frequent this seems likely to be?)
//
// TODO: Is there some clever way to invoke the leader/follower
// iterator on the local blocks in here such that the per-core
// parallelism is expressed at that level?  Seems like a nice
// natural composition and might help with my fears about how
// stencil communication will be done on a per-locale basis.
//
iter AccumStencilDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 0) param
      return if i == rangeTuple.size-1 then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag then
    chpl__testPar("AccumStencil domain follower invoked on ", followThis);

  var t: rank*range(idxType, stridable=stridable||anyStridable(followThis));
  type strType = chpl__signedType(idxType);
  for param i in 0..rank-1 {
    var stride = whole.dim(i).stride: strType;
    // not checking here whether the new low and high fit into idxType
    var low = (stride * followThis(i).low:strType):idxType;
    var high = (stride * followThis(i).high:strType):idxType;
    t(i) = ((low..high by stride:strType) + whole.dim(i).alignedLow by followThis(i).stride:strType).safeCast(t(i).type);
  }
  for i in {(...t)} {
    yield i;
  }
}

//
// output domain
//
proc AccumStencilDom.dsiSerialWrite(x) {
  x <~> whole;
}

//
// how to allocate a new array over this domain
//
proc AccumStencilDom.dsiBuildArray(type eltType, param initElts:bool) {
  const dom = this;
  const creationLocale = here.id;
  const dummyLASD = new unmanaged LocAccumStencilDom(rank, idxType, stridable);
  const dummyLASA = new unmanaged LocAccumStencilArr(eltType, rank, idxType,
                                                     stridable, dummyLASD,
                                                     false);
  var locArrTemp: [dom.dist.targetLocDom] unmanaged LocAccumStencilArr(eltType, rank, idxType, stridable) = dummyLASA;
  var myLocArrTemp: unmanaged LocAccumStencilArr(eltType, rank, idxType, stridable)?;
  
  // formerly in AccumStencilArr.setup()
  coforall localeIdx in dom.dist.targetLocDom with (ref myLocArrTemp) {
    on dom.dist.targetLocales(localeIdx) {
      const locDom = dom.getLocDom(localeIdx);
      const LASA = new unmanaged LocAccumStencilArr(eltType, rank, idxType,
                                                    stridable, locDom,
                                                    initElts=initElts);
      locArrTemp(localeIdx) = LASA;
      if here.id == creationLocale then
        myLocArrTemp = LASA;
    }
  }
  delete dummyLASA, dummyLASD;

  var arr = new unmanaged AccumStencilArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=_to_unmanaged(this), ignoreFluff=this.ignoreFluff, locArr=locArrTemp, myLocArr=myLocArrTemp);

  // formerly in AccumStencilArr.setup()
  if arr.doRADOpt && disableAccumStencilLazyRAD then arr.setupRADOpt();

  return arr;
}

proc AccumStencilDom.dsiNumIndices return whole.size;
proc AccumStencilDom.dsiLow return whole.low;
proc AccumStencilDom.dsiHigh return whole.high;
proc AccumStencilDom.dsiStride return whole.stride;

//
// INTERFACE NOTES: Could we make dsiSetIndices() for a rectangular
// domain take a domain rather than something else?
//
proc AccumStencilDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
      absFluff(i) = abs(fluff(i) * x.dim(i).stride);
    }
    wholeFluff = whole.expand(absFluff);
  }
  setup();
  if debugAccumStencilDist {
    writeln("Setting indices of AccumStencil domain:");
    dsiDisplayRepresentation();
  }
}

proc AccumStencilDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(1).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
      absFluff(i) = abs(fluff(i) * whole.dim(i).stride);
    }
    wholeFluff = whole.expand(absFluff);
  }
  setup();
  if debugAccumStencilDist {
    writeln("Setting indices of AccumStencil domain:");
    dsiDisplayRepresentation();
  }
}

proc AccumStencilDom.dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
  chpl_assignDomainWithGetSetIndices(this, rhs);
}

proc AccumStencilDom.dsiGetIndices() {
  return whole.getIndices();
}

// dsiLocalSlice
proc AccumStencilDom.dsiLocalSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}

proc AccumStencilDom.setup() {
  coforall localeIdx in dist.targetLocDom {
    on dist.targetLocales(localeIdx) {
      const myLocDom = locDoms(localeIdx);

      var abstr : rank*whole.dim(0).stride.type;
      for param i in 0..rank-1 {
        abstr(i) = abs(whole.dim(i).stride);
      }

      myLocDom.myBlock = dist.getChunk(whole, localeIdx);

      if !isZeroTuple(fluff) && myLocDom.myBlock.size != 0 then {
        myLocDom.myFluff = myLocDom.myBlock.expand(fluff*abstr);
      } else {
        myLocDom.myFluff = myLocDom.myBlock;
      }

      proc faceSize(dim) {
        var ret = 1;
        for i in 0..rank-1 do
          if i != dim then ret *= myLocDom.myFluff.dim(i).size;
        return ret;
      }

      var maxBufSize = 0;
      for i in 0..rank-1 {
        const fs = faceSize(i);
        // Multiplied by '2' for non-fluff elements to be received
        const len = fs * fluff(i) * 2;
        maxBufSize = max(maxBufSize, len);
      }
      myLocDom.recvDom = {1..maxBufSize};
    }
  }
}

override proc AccumStencilDom.dsiDestroyDom() {
  coforall localeIdx in dist.targetLocDom {
    on locDoms(localeIdx) do
      delete locDoms(localeIdx);
  }
}

proc AccumStencilDom.dsiMember(i) {
  return wholeFluff.contains(i);
}

proc AccumStencilDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocAccumStencilDom.contains(i) return myBlock.contains(i);

proc AccumStencilArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
    if doRADOpt then
      writeln("locArr[", tli, "].locRAD = ", locArr[tli].locRAD!.RAD);
  }
}

override proc AccumStencilArr.dsiGetBaseDom() return dom;

//
// NOTE: Each locale's myElems array must be initialized prior to setting up
// the RAD cache.
//
proc AccumStencilArr.setupRADOpt() {
  for localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      const myLocArr = locArr(localeIdx);
      if myLocArr.locRAD != nil {
        delete myLocArr.locRAD;
        myLocArr.locRAD = nil;
      }
      if disableAccumStencilLazyRAD {
        myLocArr.locRAD = new unmanaged LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
        for l in dom.dist.targetLocDom {
          if l != localeIdx {
            myLocArr.locRAD!.RAD(l) = locArr(l).myElems._value.dsiGetRAD();
          }
        }
      }
    }
  }
}

override proc AccumStencilArr.dsiElementInitializationComplete() {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      locArr(localeIdx).myElems.dsiElementInitializationComplete();
    }
  }
}

override proc AccumStencilArr.dsiDestroyArr(param deinitElts:bool) {
  coforall localeIdx in dom.dist.targetLocDom {
    on locArr(localeIdx) {
      var arr = locArr(localeIdx);
      if !ignoreFluff {
        if deinitElts {
          // only deinitialize non-fluff elements
          // fluff is always deinited in the LocArr deinit
          param needsDestroy = __primitive("needs auto destroy", eltType);
          if needsDestroy {
            if _deinitElementsIsParallel(eltType) {
              forall i in arr.locDom.myBlock {
                chpl__autoDestroy(arr.myElems[i]);
              }
            } else {
              for i in arr.locDom.myBlock {
                chpl__autoDestroy(arr.myElems[i]);
              }
            }
          }
        }
        delete arr;
      }
    }
  }
}

proc AccumStencilArr.fluff ref {
  return dom.dist.fluff;
}

// Re-use _remoteAccessData.getDataIndex from BlockDist


inline proc AccumStencilArr.dsiLocalAccess(i: rank*idxType) ref {
  return myLocArr!.this(i);
}

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in targetLocsIdx?
//
// BHARSH TODO: Like BlockDist, 'idx' should probably have a 'const in' or 'in'
// intent
//
inline
proc AccumStencilArr.do_dsiAccess(param setter, idx: rank*idxType) ref {
  var i = idx;
  local {
    if myLocArr != nil {
      const locArr = myLocArr!;
      if setter || this.ignoreFluff {
        // A write: return from actual data and not fluff
        if locArr.locDom.contains(i) then return locArr.this(i);
      } else {
        // A read: return from fluff if possible
        // If there is no fluff, then myFluff == myBlock
        if locArr.locDom.myFluff.contains(i) then return locArr.this(i);
      }
    }
  }

  return nonLocalAccess(i);
}

proc AccumStencilArr.nonLocalAccess(i: rank*idxType) ref {

  if doRADOpt {
    if myLocArr {
      const mArr = myLocArr!;
      if boundsChecking {
        if !dom.wholeFluff.contains(i) {
          halt("array index out of bounds: ", i);
        }
      }
      var rlocIdx = dom.dist.targetLocsIdx(i);
      if !disableAccumStencilLazyRAD {
        if mArr.locRAD == nil {
          mArr.lockLocRAD();
          if mArr.locRAD == nil {
            var tempLocRAD = new unmanaged LocRADCache(eltType, rank, idxType, stridable, dom.dist.targetLocDom);
            tempLocRAD.RAD.blk = SENTINEL;
            mArr.locRAD = tempLocRAD;
          }
          mArr.unlockLocRAD();
        }
        var mRAD = mArr.locRAD!;
        if mRAD.RAD(rlocIdx).blk == SENTINEL {
          mRAD.lockRAD(rlocIdx);
          if mRAD.RAD(rlocIdx).blk == SENTINEL {
            mRAD.RAD(rlocIdx) = locArr(rlocIdx).myElems._value.dsiGetRAD();
          }
          mRAD.unlockRAD(rlocIdx);
        }
      }
      pragma "no copy" pragma "no auto destroy" var myLocRAD = myLocArr!.locRAD!;
      pragma "no copy" pragma "no auto destroy" var radata = myLocRAD.RAD;
      if radata(rlocIdx).shiftedData != nil {
        var dataIdx = radata(rlocIdx).getDataIndex(i);
        return radata(rlocIdx).getDataElem(dataIdx);
      }
    }
  }
  return locArr(dom.dist.targetLocsIdx(i))(i);
}

// ref version
inline proc AccumStencilArr.dsiAccess(i: rank*idxType) ref {
  return do_dsiAccess(true, i);
}
// value version for POD types
inline proc AccumStencilArr.dsiAccess(i: rank*idxType)
where shouldReturnRvalueByValue(eltType) {
  return do_dsiAccess(false, i);
}
// const ref version for types with copy-ctor
inline proc AccumStencilArr.dsiAccess(i: rank*idxType) const ref
where shouldReturnRvalueByConstRef(eltType) {
  return do_dsiAccess(false, i);
}


// ref version
inline proc AccumStencilArr.dsiAccess(i: idxType...rank) ref
  return dsiAccess(i);
// value version for POD types
inline proc AccumStencilArr.dsiAccess(i: idxType...rank)
where shouldReturnRvalueByValue(eltType)
  return dsiAccess(i);
// const ref version for types with copy-ctor
inline proc AccumStencilArr.dsiAccess(i: idxType...rank) const ref
where shouldReturnRvalueByConstRef(eltType)
  return dsiAccess(i);

inline proc AccumStencilArr.dsiBoundsCheck(i: rank*idxType) {
  return dom.wholeFluff.contains(i);
}

iter AccumStencilArr.these() ref {
  for i in dom do
    yield dsiAccess(i);
}

//
// TODO: Rewrite this to reuse more of the global domain iterator
// logic?  (e.g., can we forward the forall to the global domain
// somehow?
//
iter AccumStencilArr.these(param tag: iterKind) where tag == iterKind.leader {
  for followThis in dom.these(tag) do
    yield followThis;
}

override proc AccumStencilArr.dsiStaticFastFollowCheck(type leadType) param
  return leadType == this.type || leadType == this.dom.type;

proc AccumStencilArr.dsiDynamicFastFollowCheck(lead: [])
  return lead.domain._value == this.dom;

proc AccumStencilArr.dsiDynamicFastFollowCheck(lead: domain)
  return lead._value == this.dom;

iter AccumStencilArr.these(param tag: iterKind, followThis, param fast: bool = false) ref where tag == iterKind.follower {
  proc anyStridable(rangeTuple, param i: int = 0) param
      return if i == rangeTuple.size-1 then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if chpl__testParFlag {
    if fast then
      chpl__testPar("AccumStencil array fast follower invoked on ", followThis);
    else
      chpl__testPar("AccumStencil array non-fast follower invoked on ", followThis);
  }

  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for AccumStencil array");

  var myFollowThis: rank*range(idxType=idxType, stridable=stridable || anyStridable(followThis));
  var lowIdx: rank*idxType;

  for param i in 0..rank-1 {
    var stride = dom.whole.dim(i).stride;
    // NOTE: Not bothering to check to see if these can fit into idxType
    var low = followThis(i).low * abs(stride):idxType;
    var high = followThis(i).high * abs(stride):idxType;
    myFollowThis(i) = ((low..high by stride) + dom.whole.dim(i).alignedLow by followThis(i).stride).safeCast(myFollowThis(i).type);
    lowIdx(i) = myFollowThis(i).low;
  }

  const myFollowThisDom = {(...myFollowThis)};
  if fast {
    //
    // TODO: The following is a buggy hack that will only work when we're
    // distributing across the entire Locales array.  I still think the
    // locArr/locDoms arrays should be associative over locale values.
    //
    var arrSection = locArr(dom.dist.targetLocsIdx(lowIdx));

    //
    // if arrSection is not local and we're using the fast follower,
    // it means that myFollowThisDom is empty; make arrSection local so
    // that we can use the local block below
    //
    if arrSection.locale.id != here.id then
      arrSection = myLocArr!;

    //
    // Slicing arrSection.myElems will require reference counts to be updated.
    // If myElems is an array of arrays, the inner array's domain or dist may
    // live on a different locale and require communication for reference
    // counting. Simply put: don't slice inside a local block.
    //
    ref chunk = arrSection.myElems(myFollowThisDom);
    local {
      for i in chunk do yield i;
    }
  } else {
    //
    // we don't necessarily own all the elements we're following
    //
    for i in myFollowThisDom {
      yield dsiAccess(i);
    }
  }
}

//
// output array
//
proc AccumStencilArr.dsiSerialWrite(f) {
  type strType = chpl__signedType(idxType);
  var binary = f.binary();
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 0..#rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f <~> dsiAccess(i);
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:strType) {
      if ! binary then f <~> " ";
      i(rank) += dom.dsiDim(rank).stride:strType;
    } else {
      for dim in 0..rank-2 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:strType) {
          i(dim) += dom.dsiDim(dim).stride:strType;
          for dim2 in dim+1..rank-1 {
            f <~> "\n";
            i(dim2) = dom.dsiDim(dim2).low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

pragma "no copy return"
proc AccumStencilArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 0..rank-1 {
    low(i) = ranges(i).low;
  }
  return locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
}

proc _extendTuple(type t, idx: _tuple, args) {
  var tup: args.size*t;
  var j: int = 0;

  for param i in 0..args.size-1 {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idx(j);
      j += 1;
    }
  }
  return tup;
}

proc _extendTuple(type t, idx, args) {
  var tup: args.size*t;
  var idxTup = (idx,);
  var j: int = 0;

  for param i in 0..args.size-1 {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idxTup(j);
      j += 1;
    }
  }
  return tup;
}

private inline proc isZeroTuple(t) {
  if isTuple(t) {
    for param i in 0..t.size-1 do
      if t(i) != 0 then return false;
  } else if isIntegral(t) {
    return t == 0;
  }
  else compilerError("Incorrect usage of 'isZeroTuple' utility function.");
  return true;
}

iter _array.boundaries() {
  for d in _value.dsiBoundaries() do yield d;
}

iter _array.boundaries(param tag : iterKind) where tag == iterKind.standalone {
  forall d in _value.dsiBoundaries() do yield d;
}

iter AccumStencilArr.dsiBoundaries() {
  for i in dom.dist.targetLocDom {
    const LSA      = locArr[i];
    const myLocDom = LSA.locDom;

    //
    // if `LSA` has a cache section of the outermost fluff/boundary,
    // then we should yield that so it is updated correctly. To do
    // so, we need to translate the local offset to the global offset.
    // A 2D example:
    //
    //         |-----|-----|
    //  X=>|---|     |     |
    //     |---|-----|-----|
    //         |  A  |     |
    //         |-----|-----|
    //
    // Locale-chunk `A` knows about its cache section `X` by the
    // coordinates (-1, 1). However, `X` is globally on the (-1,0)
    // face of the grid. The logic below performs the translation
    // from the local coordinate to the global coordinate.
    //
    proc toGlobalDirection(neighIdx) {
      const low  = dom.dist.targetLocDom.low;
      const high = dom.dist.targetLocDom.high;
      var ret : neighIdx.type;
      for param r in 0..rank-1 {
        if neighIdx(r) < low(r) {
          ret(r) = -1;
        } else if neighIdx(r) > high(r) {
          ret(r) = 1;
        } // else 0
      }
      return ret;
    }

    proc chunkSlice(LD, neighIdx) {
      var r : rank*LD.myFluff.dim(0).type;
      for i in 0..#rank {
        const F = LD.myFluff.dim(i);
        const B = LD.myBlock.dim(i);

        if neighIdx(i) == -1 then r(i) = F.first..B.first # fluff(i);
        else if neighIdx(i) == 1 then r(i) = B.last..F.last # -fluff(i);
        else r(i) = B;
      }
      return {(...r)};
    }

    var ndr : rank*range;
    for i in 0..rank-1 do ndr(i) = -1..1;
    const ND = {(...ndr)};

    for Off in ND {
      const neighIdx = i + Off;

      if i != neighIdx {
        const dir = toGlobalDirection(neighIdx);
        const C = chunkSlice(myLocDom, Off);

        for el in LSA.myElems[C] do yield (el, dir);
      }
    }
  }
}

//
// Yields any 'fluff' boundary chunks in the AccumStencilArr along with a global coordinate of
// where the chunk lives relative to the core.
//
iter AccumStencilArr.dsiBoundaries(param tag : iterKind) where tag == iterKind.standalone {
  coforall i in dom.dist.targetLocDom do on dom.dist.targetLocales(i) {
    const LSA      = locArr[i];
    const myLocDom = LSA.locDom;

    proc toGlobalDirection(neighIdx) {
      const low  = dom.dist.targetLocDom.low;
      const high = dom.dist.targetLocDom.high;
      var ret : neighIdx.type;
      for param r in 0..rank-1 {
        if neighIdx(r) < low(r) {
          ret(r) = -1;
        } else if neighIdx(r) > high(r) {
          ret(r) = 1;
        } // else 0
      }
      return ret;
    }

    proc chunkSlice(LD, neighIdx) {
      var r : rank*LD.myFluff.dim(0).type;
      for i in 0..#rank {
        const F = LD.myFluff.dim(i);
        const B = LD.myBlock.dim(i);

        if neighIdx(i) == -1 then r(i) = F.first..B.first # fluff(i);
        else if neighIdx(i) == 1 then r(i) = B.last..F.last # -fluff(i);
        else r(i) = B;
      }
      return {(...r)};
    }

    var ndr : rank*range;
    for i in 0..rank-1 do ndr(i) = -1..1;
    const ND = {(...ndr)};

    // TODO: There's probably a better way to utilize parallelism here...
    forall Off in ND {
      const neighIdx = i + Off;

      if i != neighIdx {
        const dir = toGlobalDirection(neighIdx);
        const C = chunkSlice(myLocDom, Off);
        assert(myLocDom.myBlock.contains(C) == false, "Yielding ", C, " in direction ", dir, " from ", myLocDom.myFluff);

        for el in LSA.myElems[C] do yield (el, dir);
      }
    }
  }
}

//
// Returns a view of this AccumStencil-distributed array without any fluff.
// Useful for ensuring that reads/writes always operate on non-cached data
//
// Copies the range slice function in ChapelArray.
//
// TODO: Return an ArrayView instead of something that looks like a normal
// array
//
pragma "no copy return"
proc _array.noFluffView() {
  var a = _value.dsiNoFluffView();
  return _newArray(a);
}

proc AccumStencilArr.dsiNoFluffView() {
  var tempDist = new unmanaged AccumStencil(dom.dist.boundingBox, dom.dist.targetLocales,
                             dom.dist.dataParTasksPerLocale, dom.dist.dataParIgnoreRunningTasks,
                             dom.dist.dataParMinGranularity, ignoreFluff=true);
  pragma "no auto destroy" var newDist = new _distribution(tempDist);
  pragma "no auto destroy" var tempDom = new _domain(newDist, rank, idxType, dom.stridable, dom.whole.dims());
  newDist._value.add_dom(tempDom._value);

  var newDom = tempDom._value;

  var alias = new unmanaged AccumStencilArr(eltType=eltType, rank=rank, idxType=idxType, stridable=newDom.stridable, dom=newDom, ignoreFluff=true, locArr=locArr, myLocArr=myLocArr);

  newDom.add_arr(alias, locking=false);
  return alias;
}

// wrapper
proc _array.updateFluff() {
  _value.dsiUpdateFluff();
}

proc AccumStencilArr._getNeighborIdx(curIdx, dim, direction) {
  var offsetIdx = curIdx;
  const locDim = dom.dist.targetLocDom.dim(dim);
  var off = direction;
  const next = curIdx(dim) + direction;
  if dom.dist.periodic {
    if next < locDim.first then off = locDim.size;
    else if next > locDim.last then off = -locDim.size;
    else off = 0;
  }
  offsetIdx(dim) = next + off;

  return offsetIdx;
}

proc AccumStencilArr._getSendDom(sourceArr, dim, direction) {
  var locDom = sourceArr.locDom;
  var r : rank * locDom.myFluff.dim(0).type;
  for i in 0..rank-1 {
    const cd = locDom.myFluff.dim(i);
    if i == dim {
      const outerBound = if direction == -1 then cd.first else cd.last;
      const ord        = cd.indexOrder(outerBound);
      const ordOff     = ord + -direction * dom.dist.fluff(i) * 2;
      // Add direction (-1 or 1) so the innerBound is inclusive w/ fluff
      const innerBound = cd.orderToIndex(ordOff + direction);
      const chunk      = if direction == -1 then outerBound..innerBound else innerBound..outerBound;

      r(i) = chunk;
    } else {
      r(i) = cd;
    }
  }
  return {(...r)};
}

private proc chopDim(D:domain, dim) {
  compilerAssert(D.rank > 1, "Cannot call 'chopDim' on one-dimensional domain");
  param newRank = D.rank - 1;
  var r : newRank * D.dim(0).type;
  var cur = 0;
  for i in 0..D.rank-1 {
    if i != dim {
      r(cur) = D.dim(i);
      cur += 1;
    }
  }
  return {(...r)};
}

private proc denseTo1D(idx, dims) {
  var blk : dims.size * int;
  blk(blk.size-1) = 1;
  for i in 0..blk.size-2 by -1 do blk(i) = blk(i+1) * dims(i+1).size;
  var sum = 0;
  for i in 0..idx.size-1 do sum += idx(i) * blk(i);
  return sum;
}

proc AccumStencilArr._sendElements(destBuf, sourceArr, dim, direction) {
  const sendDom = _getSendDom(sourceArr, dim, direction);

  if rank == 1 {
    destBuf[1..sendDom.size] = sourceArr.myElems[sendDom];
  } else {
    const chopped = chopDim(sendDom, rank-1);
    const lastDim = sendDom.dim(rank-1);
    const len = lastDim.size;
    forall (idx, dense) in zip(chopped, densify(chopped, chopped)) {
      const low = 1 + denseTo1D(chpl__tuplify(dense), sendDom.dims());
      const bufSlice = low..#len;
      destBuf[bufSlice] = sourceArr.myElems[(...idx), lastDim];
    }
  }
}

proc AccumStencilArr._unpackElements(srcBuf, destArr, dim, direction) {
  use Reflection;
  const destDom = _getSendDom(destArr, dim, direction);

  if rank == 1 {
    forall (elIdx, bufIdx) in (destDom, srcBuf.domain[1..destDom.size]) {
      destArr.myElems[elIdx] += srcBuf[bufIdx];
    }
  } else {
    const chopped = chopDim(destDom, rank-1);
    const lastDim = destDom.dim(rank-1);
    const len = lastDim.size;
    forall (idx, dense) in zip(chopped, densify(chopped, chopped)) {
      const low = 1 + denseTo1D(chpl__tuplify(dense), destDom.dims());
      const bufSlice = low..#len;
      const tupIdx = chpl__tuplify(idx);
      for (el, buf) in zip(lastDim, bufSlice) {
        destArr.myElems[(...idx), el] += srcBuf[buf];
      }
    }
  }
}

proc AccumStencilArr._exchangeHelper(curIdx, dim, direction) {
  const neighIdx = _getNeighborIdx(curIdx, dim, direction);
  if dom.dist.targetLocDom.contains(neighIdx) == false {
    if dom.dist.periodic != false {
      writeln("Error: Failed to find neighboring locale.");
      writeln("\tcurIdx   = ", curIdx);
      writeln("\tdim      = ", dim);
      writeln("\tdir      = ", direction);
      writeln("\tneighIdx = ", neighIdx);
      halt();
    }
    return;
  }

  var sourceArr = locArr[curIdx];
  var sourceDom = sourceArr.locDom;
  var destArr   = locArr[neighIdx];
  ref destBuf   = if direction == -1 then destArr.recvP else destArr.recvM;
  ref destFlag  = if direction == -1 then destArr.recvPFlag else destArr.recvMFlag;

  ref ourFlag   = if direction == -1 then sourceArr.recvMFlag else sourceArr.recvPFlag;
  ref ourBuf    = if direction == -1 then sourceArr.recvM else sourceArr.recvP;

  _sendElements(destBuf, sourceArr, dim, direction);
  destFlag.write(true);

  // wait for other locale to fill our buffer
  ourFlag.waitFor(true);

  // reset for next run
  ourFlag.write(false);

  _unpackElements(ourBuf, sourceArr, dim, direction);
}

proc AccumStencilArr.dsiUpdateFluff() {
  if isZeroTuple(dom.fluff) then return;

  for dim in 0..rank-1 {
    coforall localeIdx in dom.dist.targetLocDom {
      on dom.dist.targetLocales(localeIdx) {
        cobegin {
          _exchangeHelper(localeIdx, dim, -1);
          _exchangeHelper(localeIdx, dim,  1);
        }
      }
    }
  }
}

override proc AccumStencilArr.dsiReallocate(bounds : rank*range(idxType, BoundedRangeType.bounded, stridable)) {
  //
  // For the default rectangular array, this function changes the data
  // vector in the array class so that it is setup once the default
  // rectangular domain is changed.  For this distributed array class,
  // we don't need to do anything, because changing the domain will
  // change the domain in the local array class which will change the
  // data in the local array class.  This will only work if the domain
  // we are reallocating to has the same distribution, but domain
  // assignment is defined so that only the indices are transferred.
  // The distribution remains unchanged.
  //
}

// Call this *after* the domain has been reallocated
override proc AccumStencilArr.dsiPostReallocate() {
  if doRADOpt then setupRADOpt();
}

proc AccumStencilArr.setRADOpt(val=true) {
  doRADOpt = val;
  if doRADOpt then setupRADOpt();
}

//
// the accessor for the local array -- assumes the index is local
//
inline proc LocAccumStencilArr.this(i) ref {
  return myElems(i);
}

//
// Privatization
//
proc AccumStencil.init(other: unmanaged AccumStencil, privateData,
                param rank = other.rank,
                type idxType = other.idxType,
                param ignoreFluff = other.ignoreFluff) {
  this.rank = rank;
  this.idxType = idxType;
  this.ignoreFluff = ignoreFluff;
  boundingBox = {(...privateData(0))};
  fluff = privateData(5);
  periodic = privateData(6);
  targetLocDom = {(...privateData(1))};
  targetLocales = other.targetLocales;  // todo use serial loop for these two?
  locDist = other.locDist;
  dataParTasksPerLocale = privateData(2);
  dataParIgnoreRunningTasks = privateData(3);
  dataParMinGranularity = privateData(4);
}

override proc AccumStencil.dsiSupportsPrivatization() param return true;

proc AccumStencil.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity,
          fluff, periodic);
}

proc AccumStencil.dsiPrivatize(privatizeData) {
  return new unmanaged AccumStencil(_to_unmanaged(this), privatizeData);
}

proc AccumStencil.dsiGetReprivatizeData() return boundingBox.dims();

proc AccumStencil.dsiReprivatize(other, reprivatizeData) {
  boundingBox = {(...reprivatizeData)};
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}

override proc AccumStencilDom.dsiSupportsPrivatization() param return true;

proc AccumStencilDom.dsiGetPrivatizeData() return (dist.pid, whole.dims());

proc AccumStencilDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData(0));
  var c = new unmanaged AccumStencilDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist, fluff=fluff, periodic=periodic, ignoreFluff=this.ignoreFluff, locDoms=locDoms, whole={(...privatizeData(1))});

  if c.whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
      absFluff(i) = abs(fluff(i) * c.whole.dim(i).stride);
    }
    c.wholeFluff = c.whole.expand(absFluff);
  }
  return c;
}

proc AccumStencilDom.dsiGetReprivatizeData() return whole.dims();

proc AccumStencilDom.dsiReprivatize(other, reprivatizeData) {
  for i in dist.targetLocDom do
    locDoms(i) = other.locDoms(i);
  whole = {(...reprivatizeData)};
  if whole.size > 0 {
    var absFluff : fluff.type;
    for param i in 0..rank-1 {
      absFluff(i) = abs(fluff(i) * whole.dim(i).stride);
    }
    wholeFluff = whole.expand(absFluff);
  }
}

override proc AccumStencilArr.dsiSupportsPrivatization() param return true;

proc AccumStencilArr.dsiGetPrivatizeData() return dom.pid;

proc AccumStencilArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData);

  const dummyLASD = new unmanaged LocAccumStencilDom(rank, idxType, stridable);
  const dummyLASA = new unmanaged LocAccumStencilArr(eltType, rank, idxType, stridable, dummyLASD, initElts=false);
  var locArrTemp: [privdom.dist.targetLocDom] unmanaged LocAccumStencilArr(eltType, rank, idxType, stridable) = dummyLASA;
  var myLocArrTemp: unmanaged LocAccumStencilArr(eltType, rank, idxType, stridable)?;
  
  for localeIdx in privdom.dist.targetLocDom {
    const LASA = locArr(localeIdx);
    locArrTemp(localeIdx) = LASA;
    if LASA.locale.id == here.id then
      myLocArrTemp = LASA;
  }
  delete dummyLASA, dummyLASD;

  var c = new unmanaged AccumStencilArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom, ignoreFluff=this.ignoreFluff, locArr=locArrTemp, myLocArr=myLocArrTemp);

  return c;
}

proc AccumStencilArr.dsiTargetLocales() {
  return dom.dist.targetLocales;
}

proc AccumStencilDom.dsiTargetLocales() {
  return dist.targetLocales;
}

proc AccumStencil.dsiTargetLocales() {
  return targetLocales;
}

// AccumStencil subdomains are continuous

proc AccumStencilArr.dsiHasSingleLocalSubdomain() param return true;
proc AccumStencilDom.dsiHasSingleLocalSubdomain() param return true;

// returns the current locale's subdomain

proc AccumStencilArr.dsiLocalSubdomain(loc: locale) {
  if loc != here then
    unimplementedFeatureHalt("AccumStencil", "remote subdomain queries");

  return myLocArr!.locDom.myBlock;
}
proc AccumStencilDom.dsiLocalSubdomain(loc: locale) {
  if loc != here then
    unimplementedFeatureHalt("AccumStencil", "remote subdomain queries");

  // TODO -- could be replaced by a privatized myLocDom in AccumStencilDom
  // as it is with AccumStencilArr
  var myLocDom:unmanaged LocAccumStencilDom(rank, idxType, stridable) = nil;
  for (loc, locDom) in zip(dist.targetLocales, locDoms) {
    if loc == here then
      myLocDom = locDom;
  }
  return myLocDom.myBlock;
}
