//
// The Block distribution is defined with six classes:
//
//   Block       : distribution class
//   BlockDom    : domain class
//   BlockArr    : array class
//   LocBlock    : local distribution class (per-locale instances)
//   LocBlockDom : local domain class (per-locale instances)
//   LocBlockArr : local array class (per-locale instances)
//
// When a distribution, domain, or array class instance is created, a
// correponding local class instance is created on each locale that is
// mapped to by the distribution.
//

//
// TO DO List
//
// 1. refactor pid fields from distribution, domain, and array classes
//

use Time;
use DSIUtil;
use ChapelUtil;

//
// These flags are used to output debug information and run extra
// checks when using Block.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugBlockDist = false;
config param debugBlockDistBenchmark = true;
config param sanityCheckDistribution = false;

//
// If the testFastFollowerOptimization flag is set to true, the
// follower will write output to indicate whether the fast follower is
// used or not.  This is used in regression testing to ensure that the
// 'fast follower' optimization is working.
//
config param testFastFollowerOptimization = false;

//
// Block Distribution Class
//
//   The fields dataParTasksPerLocale, dataParIgnoreRunningTasks, and
//   dataParMinGranularity can be changed, but changes are
//   not reflected in privatized copies of this distribution.  Perhaps
//   this is a feature, not a bug?!
//
// rank : generic rank that must match the rank of domains and arrays
//        declared over this distribution
//
// idxType: generic index type that must match the index type of
//          domains and arrays declared over this distribution
//
// boundingBox: a non-distributed domain defining a bounding box used
//              to partition the space of all indices across the array
//              of target locales; the indices inside the bounding box
//              are partitioned "evenly" across the locales and
//              indices outside the bounding box are mapped to the
//              same locale as the nearest index inside the bounding
//              box
//
// targetLocDom: a non-distributed domain over which the array of
//               target locales and the array of local distribution
//               classes are defined
//
// targetLocales: a non-distributed array containing the target
//                locales to which this distribution partitions indices
//                and data
//
// locDist: a non-distributed array of local distribution classes
//
// dataParTasksPerLocale: an integer that specifies the number of tasks to
//                        use on each locale when iterating in parallel over
//                        a Block-distributed domain or array
//
// dataParIgnoreRunningTasks: a boolean what dictates whether the number of
//                            task use on each locale should be limited
//                            by the available parallelism
//
// dataParMinGranularity: the minimum required number of elements per
//                        task created
//
class Block : BaseDist {
  param rank: int;
  type idxType = int;
  var boundingBox: domain(rank, idxType);
  var targetLocDom: domain(rank);
  var targetLocales: [targetLocDom] locale;
  var locDist: [targetLocDom] LocBlock(rank, idxType);
  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;
  var pid: int = -1; // privatized object id (this should be factored out)
}

//
// Local Block Distribution Class
//
// rank : generic rank that matches Block.rank
// idxType: generic index type that matches Block.idxType
// myChunk: a non-distributed domain that defines this locale's indices
//
class LocBlock {
  param rank: int;
  type idxType;
  const myChunk: domain(rank, idxType);
}

//
// Block Domain Class
//
// rank:      generic domain rank
// idxType:   generic domain index type
// stridable: generic domain stridable parameter
// dist:      reference to distribution class
// locDoms:   a non-distributed array of local domain classes
// whole:     a non-distributed domain that defines the domain's indices
//
class BlockDom: BaseArithmeticDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  const dist: Block(rank, idxType);
  var locDoms: [dist.targetLocDom] LocBlockDom(rank, idxType, stridable);
  const whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  var pid: int = -1; // privatized object id (this should be factored out)
}

//
// Local Block Domain Class
//
// rank: generic domain rank
// idxType: generic domain index type
// stridable: generic domain stridable parameter
// myBlock: a non-distributed domain that defines the local indices
//
class LocBlockDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var myBlock: domain(rank, idxType, stridable);
}

//
// Block Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// dom: reference to domain class
// locArr: a non-distributed array of local array classes
// myLocArr: optimized reference to here's local array class (or nil)
//
class BlockArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  var dom: BlockDom(rank, idxType, stridable);
  var locArr: [dom.dist.targetLocDom] LocBlockArr(eltType, rank, idxType, stridable);
  var myLocArr: LocBlockArr(eltType, rank, idxType, stridable);
  var pid: int = -1; // privatized object id (this should be factored out)
}

//
// Local Block Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// locDom: reference to local domain class
// myElems: a non-distributed array of local elements
//
class LocBlockArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  const locDom: LocBlockDom(rank, idxType, stridable);
  var myElems: [locDom.myBlock] eltType;
}

//
// Block constructor for clients of the Block distribution
//
def Block.Block(boundingBox: domain,
                targetLocales: [] locale = thisRealm.Locales,
                dataParTasksPerLocale=getDataParTasksPerLocale(),
                dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
                dataParMinGranularity=getDataParMinGranularity(),
                param rank = boundingBox.rank,
                type idxType = boundingBox.dim(1).idxType) {
  if rank != boundingBox.rank then
    compilerError("specified Block rank != rank of specified bounding box");
  if idxType != boundingBox.dim(1).idxType then
    compilerError("specified Block index type != index type of specified bounding box");

  this.boundingBox = boundingBox;

  setupTargetLocalesArray(targetLocDom, this.targetLocales, targetLocales);

  const boundingBoxDims = boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();
  coforall locid in targetLocDom do
    on this.targetLocales(locid) do
      locDist(locid) =  new LocBlock(rank, idxType, locid, boundingBoxDims,
                                     targetLocDomDims);

  // NOTE: When these knobs stop using the global defaults, we will need
  // to add checks to make sure dataParTasksPerLocale<0 and
  // dataParMinGranularity<0
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0 then here.numCores
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;
  
  if debugBlockDist {
    writeln("Creating new Block distribution:");
    dsiDisplayRepresentation();
  }
}

def Block.dsiAssign(other: this.type) {
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
      locDist(locid) = new LocBlock(rank, idxType, locid, boundingBoxDims,
                                    targetLocDomDims);
}

def Block.dsiClone() {
  return new Block(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity);
}

def Block.dsiDestroyDistClass() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

def Block.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

def Block.dsiNewArithmeticDom(param rank: int, type idxType,
                              param stridable: bool) {
  if idxType != this.idxType then
    compilerError("Block domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Block domain rank does not match distribution's");
  
  var dom = new BlockDom(rank=rank, idxType=idxType, dist=this, stridable=stridable);
  dom.setup();
  if debugBlockDist {
    writeln("Creating new Block domain:");
    dom.dsiDisplayRepresentation();
  }
  return dom;
}

//
// output distribution
//
def Block.writeThis(x:Writer) {
  x.writeln("Block");
  x.writeln("-------");
  x.writeln("distributes: ", boundingBox);
  x.writeln("across locales: ", targetLocales);
  x.writeln("indexed via: ", targetLocDom);
  x.writeln("resulting in: ");
  for locid in targetLocDom do
    x.writeln("  [", locid, "] locale ", locDist(locid).locale.uid, " owns chunk: ", locDist(locid).myChunk);
}

def Block.dsiIndexLocale(ind: idxType) where rank == 1 {
  return targetLocales(targetLocsIdx(ind));
}

def Block.dsiIndexLocale(ind: rank*idxType) where rank > 1 {
  return targetLocales(targetLocsIdx(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
def Block.getChunk(inds, locid) {
  // use domain slicing to get the intersection between what the
  // locale owns and the domain's index set
  //
  // TODO: Should this be able to be written as myChunk[inds] ???
  //
  // TODO: Does using David's detupling trick work here?
  //
  const chunk = locDist(locid).myChunk((...inds.getIndices()));
  if sanityCheckDistribution then
    if chunk.numIndices > 0 {
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
def Block.targetLocsIdx(ind: idxType) where rank == 1 {
  return targetLocsIdx(tuple(ind));
}

def Block.targetLocsIdx(ind: rank*idxType) {
  var result: rank*int;
  for param i in 1..rank do
    result(i) = max(0, min((targetLocDom.dim(i).length-1):int,
                           (((ind(i) - boundingBox.dim(i).low) *
                             targetLocDom.dim(i).length:idxType) /
                            boundingBox.dim(i).length):int));
  return if rank == 1 then result(1) else result;
}

def Block.dsiCreateReindexDist(newSpace, oldSpace) {
  def anyStridable(space, param i=1) param
    return if i == space.size then space(i).stridable
           else space(i).stridable || anyStridable(space, i+1);

  // Should this error be in ChapelArray or not an error at all?
  if newSpace(1).idxType != oldSpace(1).idxType then
    compilerError("index type of reindex domain must match that of original domain");
  if anyStridable(newSpace) || anyStridable(oldSpace) then
    compilerWarning("reindexing stridable Block arrays is not yet fully supported");

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
  def adjustBound(bbound, oldBound, newBound) {
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
  for param r in 1..rank {
    var oldLow = oldSpace(r)._low;
    var newLow = newSpace(r)._low;
    var oldHigh = oldSpace(r)._high;
    var newHigh = newSpace(r)._high;
    var valid: bool;
    if oldLow != newLow {
      (myNewBbox(r)._low,valid) = adjustBound(myNewBbox(r).low,oldLow,newLow);
      if !valid then // try with high
        (myNewBbox(r)._low,valid) = adjustBound(myNewBbox(r).low,oldHigh,newHigh);
      if !valid then
        halt("invalid reindex for Block: distribution bounding box (low) out of range in dimension ", r);

      (myNewBbox(r)._high,valid) = adjustBound(myNewBbox(r).high,oldHigh,newHigh);
      if !valid then
        (myNewBbox(r)._high,valid) = adjustBound(myNewBbox(r).high,oldLow,newLow);
      if !valid then // try with low
        halt("invalid reindex for Block: distribution bounding box (high) out of range in dimension ", r);
    }
  }
  var d = [(...myNewBbox)];
  var newDist = new Block(d, targetLocales, 
                          dataParTasksPerLocale, dataParIgnoreRunningTasks,
                          dataParMinGranularity);
  return newDist;
}


def LocBlock.LocBlock(param rank: int,
                      type idxType, 
                      locid, // the locale index from the target domain
                      boundingBox: rank*range(idxType),
                      targetLocBox: rank*range) {
  if rank == 1 {
    const lo = boundingBox(1).low;
    const hi = boundingBox(1).high;
    const numelems = hi - lo + 1;
    const numlocs = targetLocBox(1).length;
    const (blo, bhi) = _computeBlock(numelems, numlocs, locid,
                                     max(idxType), min(idxType), lo);
    myChunk = [blo..bhi];
  } else {
    var tuple: rank*range(idxType);
    for param i in 1..rank {
      const lo = boundingBox(i).low;
      const hi = boundingBox(i).high;
      const numelems = hi - lo + 1;
      const numlocs = targetLocBox(i).length;
      const (blo, bhi) = _computeBlock(numelems, numlocs, locid(i),
                                       max(idxType), min(idxType), lo);
      tuple(i) = blo..bhi;
    }
    myChunk = [(...tuple)];
  }
}

def BlockDom.getBaseDist() return dist;

def BlockDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].myBlock = ", locDoms[tli].myBlock);
}

def BlockDom.dsiDims() return whole.dims();

def BlockDom.dsiDim(d: int) return whole.dim(d);

// stopgap to avoid accessing locDoms field (and returning an array)
def BlockDom.getLocDom(localeIdx) return locDoms(localeIdx);


//
// Given a tuple of scalars of type t or range(t) match the shape but
// using types rangeType and scalarType e.g. the call:
// _matchArgsShape(range(int), int, (1:int(64), 1:int(64)..5, 1:int(64)..5))
// returns the type: (int, range(int), range(int))
//
def _matchArgsShape(type rangeType, type scalarType, args) type {
  def tuple(type t ...) type return t;
  def helper(param i: int) type {
    if i == args.size {
      if isCollapsedDimension(args(i)) then
        return tuple(scalarType);
      else
        return tuple(rangeType);
    } else {
      if isCollapsedDimension(args(i)) then
        return (scalarType, (... helper(i+1)));
      else
        return (rangeType, (... helper(i+1)));
    }
  }
  return helper(1);
}


def Block.dsiCreateRankChangeDist(param newRank: int, args) {
  var collapsedDimLocs: rank*idxType;

  for param i in 1..rank {
    if isCollapsedDimension(args(i)) {
      collapsedDimLocs(i) = args(i);
    } else {
      collapsedDimLocs(i) = 0;
    }
  }
  const collapsedLocInd = gargetLocsIdx(collapsedDimLocs);
  var collapsedBbox: _matchArgsShape(range(idxType=idxType), idxType, args);
  var collapsedLocs: _matchArgsShape(range(idxType=int), int, args);

  for param i in 1..rank {
    if isCollapsedDimension(args(i)) {
      // set indicies that are out of bounds to the bounding box low or high.
      collapsedBbox(i) = if args(i) < boundingBox.dim(i).low then boundingBox.dim(i).low else if args(i) > boundingBox.dim(i).high then boundingBox.dim(i).high else args(i);
      collapsedLocs(i) = collapsedLocInd(i);
    } else {
      collapsedBbox(i) = boundingBox.dim(i);
      collapsedLocs(i) = targetLocDom.dim(i);
    }
  }

  const newBbox = boundingBox[(...collapsedBbox)];
  const newTargetLocs = targetLocales((...collapsedLocs));
  return new Block(newBbox, newTargetLocs,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity);
}

def BlockDom.these() {
  for i in whole do
    yield i;
}

def BlockDom.these(param tag: iterator) where tag == iterator.leader {
  const maxTasks = dist.dataParTasksPerLocale;
  const ignoreRunning = dist.dataParIgnoreRunningTasks;
  const minSize = dist.dataParMinGranularity;
  const precomputedWholeLow = whole.low;
  coforall locDom in locDoms do on locDom {
    var tmpBlock = locDom.myBlock - precomputedWholeLow;
    const (numTasks, parDim) =
      _computeChunkStuff(maxTasks, ignoreRunning, minSize,
                         locDom.myBlock.dims(), rank);
    var locBlock: rank*range(idxType);
    for param i in 1..tmpBlock.rank {
      locBlock(i) = (tmpBlock.dim(i).low/tmpBlock.dim(i).stride:idxType)..#(tmpBlock.dim(i).length);
    }
    if (numTasks == 1) {
      yield locBlock;
    } else {
      coforall taskid in 0:uint(64)..#numTasks {
        var tuple: rank*range(idxType) = locBlock;
        const (lo,hi) = _computeBlock(locBlock(parDim).length, numTasks, taskid,
                                      locBlock(parDim).high,
                                      locBlock(parDim).low,
                                      locBlock(parDim).low);
        tuple(parDim) = lo..hi;
        yield tuple;
      }
    }
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
def BlockDom.these(param tag: iterator, follower) where tag == iterator.follower {
  def anyStridable(rangeTuple, param i: int = 1) param
      return if i == rangeTuple.size then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  chpl__testPar("Block domain follower invoked on ", follower);
  var t: rank*range(idxType, stridable=stridable||anyStridable(follower));
  for param i in 1..rank {
    var stride = whole.dim(i).stride: idxType;
    var low = stride * follower(i).low;
    var high = stride * follower(i).high;
    t(i) = (low..high by stride:int) + whole.dim(i).low by follower(i).stride;
  }
  for i in [(...t)] {
    yield i;
  }
}

//
// output domain
//
def BlockDom.dsiSerialWrite(x:Writer) {
  x.write(whole);
}

//
// how to allocate a new array over this domain
//
def BlockDom.dsiBuildArray(type eltType) {
  var arr = new BlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}

def BlockDom.dsiNumIndices return whole.numIndices;
def BlockDom.dsiLow return whole.low;
def BlockDom.dsiHigh return whole.high;

//
// INTERFACE NOTES: Could we make dsiSetIndices() for an arithmetic
// domain take a domain rather than something else?
//
def BlockDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
  if debugBlockDist {
    writeln("Setting indices of Block domain:");
    dsiDisplayRepresentation();
  }
}

def BlockDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(1).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  setup();
  if debugBlockDist {
    writeln("Setting indices of Block domain:");
    dsiDisplayRepresentation();
  }
}

def BlockDom.dsiGetIndices() {
  return whole.getIndices();
}

// remove all instances of getDist
def BlockDom.getDist(): Block(idxType) {
  return dist;
}

// dsiLocalSlice
def BlockDom.localSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}

def BlockDom.setup() {
  if locDoms(dist.targetLocDom.low) == nil {
    coforall localeIdx in dist.targetLocDom do {
      on dist.targetLocales(localeIdx) do
        locDoms(localeIdx) = new LocBlockDom(rank, idxType, stridable,
                                             dist.getChunk(whole, localeIdx));
    }
  } else {
    coforall localeIdx in dist.targetLocDom do {
      on dist.targetLocales(localeIdx) do
        locDoms(localeIdx).myBlock = dist.getChunk(whole, localeIdx);
    }
  }
}

def BlockDom.dsiMember(i) {
  return whole.member(i);
}

def BlockDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// build a new arithmetic domain using the given range
//
def BlockDom.dsiBuildArithmeticDom(param rank: int, type idxType,
                                   param stridable: bool,
                                   ranges: rank*range(idxType,
                                                      BoundedRangeType.bounded,
                                                      stridable)) {
  if idxType != dist.idxType then
    compilerError("Block domain index type does not match distribution's");
  if rank != dist.rank then
    compilerError("Block domain rank does not match distribution's");
  
  var dom = new BlockDom(rank=rank, idxType=idxType,
                         dist=dist, stridable=stridable);
  dom.dsiSetIndices(ranges);
  return dom;
}

def BlockDom.writeBinDom_real(f: file, in rank : int, in whole:domain) {

  var gres=0:int(64);
  var gerr=0:int;
  var fpos=f.chpl_ftell();
  if debugBlockDist then writeln("writeBinDom, ftell:",fpos);


  binfwrite(rank,4,1,f._fp,gres,gerr);

  for dim in 1..rank do {
    binfwrite(whole.dim(dim)._low,4,1,f._fp,gres,gerr);
    binfwrite(whole.dim(dim)._high,4,1,f._fp,gres,gerr);
    binfwrite(whole.dim(dim)._stride,4,1,f._fp,gres,gerr);
    if debugBlockDist then {
		writeln("Writting Dom dim:",dim," : ",whole.dim(dim)._low);
		writeln("Writting Dom dim:",dim," : ",whole.dim(dim)._high);
		writeln("Writting Dom dim:",dim," : ",whole.dim(dim)._stride);
    }
  }

}


def BlockDom.writeBinDom(f: file) {
   writeBinDom_real(f,rank,whole);
}


def BlockDom.readBinDom_real(f: file, in rank : int, in whole:domain) {

  var gres=0:int(64);
  var gerr=0:int;
  for dim in 1..rank do {
    if debugBlockDist then {
                writeln("before read Dom dim:",dim," : ",whole.dim(dim)._low);
                writeln("before read Dom dim:",dim," : ",whole.dim(dim)._high);
                writeln("before read Dom dim:",dim," : ",whole.dim(dim)._stride);
    }
  }
  var fpos=f.chpl_ftell();
  if debugBlockDist then writeln("readBinDom, ftell:",fpos);

  binfread(rank,4,1,f._fp,gres,gerr);

  var rr:int;
  for dim in 1..rank do {
    binfread(whole.dim(dim)._low,4,1,f._fp,gres,gerr);
    binfread(whole.dim(dim)._high,4,1,f._fp,gres,gerr);
    binfread(whole.dim(dim)._stride,4,1,f._fp,gres,gerr);
    if debugBlockDist then {
                writeln("Read Dom dim:",dim," : ",whole.dim(dim)._low," rr:",rr);
                writeln("Read Dom dim:",dim," : ",whole.dim(dim)._high);
                writeln("Read Dom dim:",dim," : ",whole.dim(dim)._stride);
    }
  }

}


def BlockDom.readBinDom(f: file) {
   readBinDom_real(f,rank,whole);
}


//
// Added as a performance stopgap to avoid returning a domain
//
def LocBlockDom.member(i) return myBlock.member(i);

def BlockArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom do
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
}

def BlockArr.dsiGetBaseDom() return dom;

def BlockArr.setup() {
  var thisid = this.locale.uid;
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
      const locDom = dom.getLocDom(localeIdx);
      locArr(localeIdx) = new LocBlockArr(eltType, rank, idxType, stridable, locDom);
      if thisid == here.uid then
        myLocArr = locArr(localeIdx);
    }
  }
}

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in targetLocsIdx?
//
def BlockArr.dsiAccess(i: rank*idxType) var {
  if myLocArr then local {
    if myLocArr.locDom.member(i) then
      return myLocArr.this(i);
  }
  return locArr(dom.dist.targetLocsIdx(i))(i);
}

def BlockArr.dsiAccess(i: idxType...rank) var
  return dsiAccess(i);

def BlockArr.these() var {
  for i in dom do
    yield dsiAccess(i);
}

/*
_extern def binfwrite (inout ptr:int(64), size:int(64) , nelm:int(64), file:_file, inout res:int(64), inout err:int );
_extern def binfwrite (inout ptr:int, size:int(64) , nelm:int(64), file:_file, inout res:int(64), inout err:int );
_extern def binfwrite (inout ptr:real, size:int(64) , nelm:int(64), file:_file, inout res:int(64), inout err:int );

extern def binfread (inout ptr:int, size:int(64) , nelm:int(64), file:_file, inout res:int(64), inout err:int );
_extern def binfread (inout ptr:int(64), size:int(64) , nelm:int(64), file:_file, inout res:int(64), inout err:int );
_extern def binfread (inout ptr:real, size:int(64) , nelm:int(64), file:_file, inout res:int(64), inout err:int );
*/

def BlockArr.writeBinLocalArray(localeIdx, offset: int(64), f: file)
{
  const privarr=this.dsiPrivatize(dom.pid);

  var status= 0:int(64);
  var err= 0:int;
  var numbytespn=8:int(64);
  var nbyteslin=numbytespn*((1+dom.dsiDim(1)._high-dom.dsiDim(1)._low)/dom.dsiDim(1)._stride);
  var pos=0:int(64);
  var oldpos=-1:int(64);
  var timer: Timer;
  timer.start();
  var to,from, numelem,tmp=0:int(64);
  var outfile = new file(f.filename, FileAccessMode.readwrite);
  var bytes_written=0:int(64);
  outfile.open();

  if debugBlockDist then writeln("BlockArr.writeBinLocalArray in ",here.id," myblock:",locArr(localeIdx).locDom.myBlock.numIndices);
  var ind=privarr.locArr(localeIdx).locDom.myBlock.low;
  if debugBlockDist then writeln("BlockArr.writeBinLocalArray  create ind:",ind);

  for idx in locArr(localeIdx).locDom.myBlock.low(1)..locArr(localeIdx).locDom.myBlock.high(1)  {

    from=privarr.locArr(localeIdx).locDom.myBlock.low(2):int(64);
    to=privarr.locArr(localeIdx).locDom.myBlock.high(2):int(64);
    if debugBlockDist then writeln("BlockArr.writeBinLocalArray  from:",from," to: ",to," line:",idx, " numbytespn:",numbytespn," ",dom.dsiDim(1)._high," ",dom.dsiDim(1)._low," ",dom.dsiDim(1)._stride);

    ind(1)=idx:int;
    ind(2)=from:int;
    numelem=to-from+1;

    pos=((idx-1))*nbyteslin+(from-1)*numbytespn+offset;
    if (pos!=(oldpos+numelem*numbytespn)) then {
//      if debugBlockDistBenchmark then writeln("BlockArr.writeBinLocalArray in ",here.id," line:",idx," to seek to pos:(",from," ",to,") ",pos," old pos:",oldpos+numelem*numbytespn," elapsed:",timer.elapsed(TimeUnits.seconds));
      outfile.fseekset(pos);
    }

    oldpos=pos;
    binfwrite(privarr.locArr(localeIdx)(ind),numbytespn,numelem,outfile._fp, status, err);
    bytes_written=bytes_written+(numbytespn*numelem);
    if status < 0 {
      halt("***Error: Write failed: ", err, "***");
    }
  }
  pos=outfile.chpl_ftell();
  outfile.close();
  timer.stop();
  if debugBlockDist then writeln("BlockArr.writeLocalBinArray locale ",here.id," ftell at the end:",pos);
  if debugBlockDistBenchmark then
    writeln("BlockArr.writeLocalBinArray time to write in locale ",here.id,": ",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",bytes_written/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");
  return pos;
}

def BlockArr.writeBinArray(f: file) 
{
  var timer: Timer;
  timer.start();
  var fpos=f.chpl_ftell();

  if debugBlockDist then writeln("writeBinArray beginning, ftell:",fpos);

  // lastPos and gotoPos used for going to the end of the writes at the end.
  var lastPos=fpos;
  var gotoPos=fpos; 

  if debugBlockDist then writeln("BlockArr.writeBinArray Going to call the real write ,pos:",fpos);

  // We close the file so it is not opened from different locales.
  // It will be opened again after writting, at the right position
  f.close();

  if debugBlockDist then {
    writeln("Inside: ",here.id," Domain1:",dom.dist.targetLocDom.dim(1));
    writeln("Inside: ",here.id," Domain2:",dom.dist.targetLocDom.dim(2));
    for i1 in dom.dist.targetLocDom.dim(1)._low..dom.dist.targetLocDom.dim(1)._high  {
      for i2 in dom.dist.targetLocDom.dim(2)._low..dom.dist.targetLocDom.dim(2)._high {
	writeln("Inside: ",here.id," idx:",i1,",",i2, " locale:",dom.dist.targetLocales((i1,i2)));
  } } }

//  for i1 in dom.dist.targetLocDom.dim(1)._low..dom.dist.targetLocDom.dim(1)._high {
  for i2 in dom.dist.targetLocDom.dim(2)._low..dom.dist.targetLocDom.dim(2)._high {
    coforall i1 in dom.dist.targetLocDom.dim(1)._low..dom.dist.targetLocDom.dim(1)._high {
      on dom.dist.targetLocales((i1,i2)) {
        if debugBlockDist then writeln("Inside: ",here.id," idx:",i1,",",i2, " low:",dom.dist.targetLocDom.dim(1)._low, " high:",dom.dist.targetLocDom.dim(1)._high);
        lastPos=writeBinLocalArray((i1,i2),fpos,f);
	if ( gotoPos < lastPos ) then gotoPos=lastPos;
      }
    }
  }
  timer.stop();
  if debugBlockDistBenchmark then
    writeln("BlockArr.writeBinArray time to write:",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",(gotoPos-fpos)/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");

  f.open();
  f.fseekset(gotoPos);
  if debugBlockDist then writeln("writeBinArray, at end :",gotoPos);
}


// read binary array
def BlockArr.readBinArray(f: file) {
//  if dom.dsiNumIndices == 0 then return;
  var rango:int;
  var gerr:int;
  var gres:int(64);
  var timer: Timer;
  timer.start();

  var fpos=f.chpl_ftell();
  var gotoPos=fpos; 


//  var i : rank*idxType;
  if debugBlockDist then writeln("readBinArray enter locales, ftell:",fpos);
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocales(localeIdx) {
//startCommDiagnosticsHere();
        var infile = new file(f.filename, FileAccessMode.read);
//stopCommDiagnosticsHere();
        const privarr=this.dsiPrivatize(dom.pid);

  	var loc_timer: Timer;
  	loc_timer.start();
        infile.open();


  	var bytes_read=0:int(64);
        var order=here.id*100000;
        var status= 0:int(64);
        var err= 0:int;
// num of bytes per number
        var numbytespn=8:int(64);
// For now, one for the rank and one for each dimension
        var offset=fpos;
        var nbyteslin=numbytespn*((1+dom.dsiDim(1)._high-dom.dsiDim(1)._low)/dom.dsiDim(1)._stride);

        var pos=0:int(64);
        var oldpos=0:int(64);
        var to,from, numelem,tmp=0:int(64);
        var thislocal=localeIdx;
        if debugBlockDist then writeln("BlockArr.readBinArray in ",here.id," myblock.low:",locArr(localeIdx).locDom.myBlock.low);
        var ind=privarr.locArr(localeIdx).locDom.myBlock.low;
        if debugBlockDist then writeln("BlockArr.readBinArray  create ind:",ind," offset:",offset);
        for idx in locArr(localeIdx).locDom.myBlock.low(1)..locArr(localeIdx).locDom.myBlock.high(1)  {
          from=privarr.locArr(localeIdx).locDom.myBlock.low(2);
          to=privarr.locArr(localeIdx).locDom.myBlock.high(2);
          if debugBlockDist then {
            writeln("BlockArr.readBinArray  from:",from," to: ",to," line:",idx);
          }
          ind(1)=idx:int;
          ind(2)=from:int;
          numelem=to-from+1;
	  pos=((idx-1))*nbyteslin+(from-1)*numbytespn+offset;

	  if (pos!=(oldpos+numelem*numbytespn)) then {
		  infile.fseekset(pos);
	  }
	  oldpos=pos;
	  if debugBlockDist then writeln("binfread ",here.id,numbytespn,"numelem:",numelem," pos:",pos );
	  binfread(privarr.locArr(thislocal)(ind),numbytespn,numelem,infile._fp, status, err);
	  bytes_read=bytes_read+numbytespn*numelem;
	  if status < 0 {
		  const err = __primitive("get_errno");
		  halt("***Error: Write failed: ", err, "***");
	  }

	}
        infile.close();
        loc_timer.stop();
        if debugBlockDistBenchmark then
          writeln("BlockArr.readBinArray time to read in locale ",here.id,": ",loc_timer.elapsed(TimeUnits.seconds)," bandwitdh: ",bytes_read/loc_timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");
    }
  }

// Updates the file position, so it can continue to be used as if it had
// been read sequentially.

  var numbytespn=8:int(64);
  var nbyteslin=numbytespn*((1+dom.dsiDim(1)._high-dom.dsiDim(1)._low)/dom.dsiDim(1)._stride);
  var bytes_read=(((dom.dsiDim(2)._high)-dom.dsiDim(2)._low+1)/dom.dsiDim(2)._stride)*nbyteslin:int(64);
  gotoPos=bytes_read+fpos;

  f.fseekset(gotoPos);
  timer.stop();
  if debugBlockDistBenchmark then
    writeln("BlockArr.readBinArray time to read:",timer.elapsed(TimeUnits.seconds)," bandwitdh: ",bytes_read/timer.elapsed(TimeUnits.seconds)/(1000*1000)," MiB/s");


// End of parallel read
}


//
// TODO: Rewrite this to reuse more of the global domain iterator
// logic?  (e.g., can we forward the forall to the global domain
// somehow?
//
def BlockArr.these(param tag: iterator) where tag == iterator.leader {
  const maxTasks = dom.dist.dataParTasksPerLocale;
  const ignoreRunning = dom.dist.dataParIgnoreRunningTasks;
  const minSize = dom.dist.dataParMinGranularity;
  const precomputedWholeLow = dom.whole.low;
  coforall locDom in dom.locDoms do on locDom {
    var tmpBlock = locDom.myBlock - precomputedWholeLow;
    const (numTasks, parDim) =
      _computeChunkStuff(maxTasks, ignoreRunning, minSize,
                         locDom.myBlock.dims(), rank);
    var locBlock: rank*range(idxType);
    for param i in 1..tmpBlock.rank {
      locBlock(i) = (tmpBlock.dim(i).low/tmpBlock.dim(i).stride:idxType)..#(tmpBlock.dim(i).length);
    }


    if (numTasks == 1) {
      yield locBlock;
    } else {
      coforall taskid in 0:uint(64)..#numTasks {
        var tuple: rank*range(idxType) = locBlock;
        const (lo,hi) = _computeBlock(locBlock(parDim).length, numTasks, taskid,
                                      locBlock(parDim).high,
                                      locBlock(parDim).low,
                                      locBlock(parDim).low);
          
        tuple(parDim) = lo..hi;
        yield tuple;

      }
    }
  }
}

def BlockArr.dsiStaticFastFollowCheck(type leadType) param
  return leadType == this.type || leadType == this.dom.type;

def BlockArr.dsiDynamicFastFollowCheck(lead: [])
  return lead.domain._value == this.dom;

def BlockArr.dsiDynamicFastFollowCheck(lead: domain)
  return lead._value == this.dom;

def BlockArr.these(param tag: iterator, follower, param fast: bool = false) var where tag == iterator.follower {
  def anyStridable(rangeTuple, param i: int = 1) param
      return if i == rangeTuple.size then rangeTuple(i).stridable
             else rangeTuple(i).stridable || anyStridable(rangeTuple, i+1);

  if fast then
    chpl__testPar("Block array fast follower invoked on ", follower);
  else
    chpl__testPar("Block array non-fast follower invoked on ", follower);

  if testFastFollowerOptimization then
    writeln((if fast then "fast" else "regular") + " follower invoked for Block array");

  var followThis: rank*range(idxType=idxType, stridable=stridable || anyStridable(follower));
  var lowIdx: rank*idxType;

  for param i in 1..rank {
    var stride = dom.whole.dim(i).stride;
    var low = follower(i).low * stride;
    var high = follower(i).high * stride;
    followThis(i) = (low..high by stride) + dom.whole.dim(i).low by follower(i).stride;
    lowIdx(i) = followThis(i).low;
  }
  const followThisDom = [(...followThis)];

  //
  // TODO: The following is a buggy hack that will only work when we're
  // distributing across the entire Locales array.  I still think the
  // locArr/locDoms arrays should be associative over locale values.
  //
  var arrSection = locArr(dom.dist.targetLocsIdx(lowIdx));
  if fast {
    //
    // if arrSection is not local and we're using the fast follower,
    // it means that followThisDom is empty; make arrSection local so
    // that we can use the local block below
    //
    if arrSection.locale.uid != here.uid then
      arrSection = myLocArr;
    local {
      for e in arrSection.myElems(followThisDom) do
        yield e;
    }
  } else {
    //
    // we don't own all the elements we're following
    //
    def accessHelper(i) var {
      if myLocArr then local {
        if myLocArr.locDom.member(i) then
          return myLocArr.this(i);
      }
      return dsiAccess(i);
    }
    for i in followThisDom {
      yield accessHelper(i);
    }
  }
}

//
// output array
//
def BlockArr.dsiSerialWrite(f: Writer) {
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim)._low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank)._high - dom.dsiDim(rank)._stride:idxType) {
      f.write(" ");
      i(rank) += dom.dsiDim(rank)._stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim)._high - dom.dsiDim(dim)._stride:idxType) {
          i(dim) += dom.dsiDim(dim)._stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dsiDim(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

def BlockArr.dsiSlice(d: BlockDom) {
  var alias = new BlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d, pid=pid);
  var thisid = this.locale.uid;
  coforall i in d.dist.targetLocDom {
    on d.dist.targetLocales(i) {
      alias.locArr[i] = new LocBlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, locDom=d.locDoms[i], myElems=>locArr[i].myElems[d.locDoms[i].myBlock]);
      if thisid == here.uid then
        alias.myLocArr = alias.locArr[i];
    }
  }

  return alias;
}

def BlockArr.localSlice(ranges) {
  var low: rank*idxType;
  for param i in 1..rank {
    low(i) = ranges(i).low;
  }
  var A => locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
  return A;
}

def _extendTuple(type t, idx: _tuple, args) {
  var tup: args.size*t;
  var j: int = 1;
  
  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idx(j);
      j += 1;
    }
  }
  return tup;
}

def _extendTuple(type t, idx, args) {
  var tup: args.size*t;
  var idxTup = tuple(idx);
  var j: int = 1;

  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idxTup(j);
      j += 1;
    }
  }
  return tup;
}


def BlockArr.dsiRankChange(d, param newRank: int, param stridable: bool, args) {
  var alias = new BlockArr(eltType=eltType, rank=newRank, idxType=idxType, stridable=stridable, dom=d);
  var thisid = this.locale.uid;
  coforall ind in d.dist.targetLocDom {
    on d.dist.targetLocales(ind) {
      const locDom = d.getLocDom(ind);
      // locSlice is a tuple of ranges and scalars. It will match the basic
      // shape of the args argument. 
      var locSlice: _matchArgsShape(range(idxType=idxType, stridable=stridable), idxType, args);
      // collapsedDims stores the value any collapsed dimension is down to.
      // For any non-collapsed dimension, that position is ignored.
      // This tuple is then passed to the targetLocsIdx function to build up a
      // partial index into this.targetLocDom with correct values set for all
      // collapsed dimensions. The rest of the dimensions get their values from
      // ind - an index into the new rank changed targetLocDom.
      var collapsedDims: rank*idxType;
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
        new LocBlockArr(eltType=eltType, rank=newRank, idxType=d.idxType,
                        stridable=d.stridable, locDom=locDom,
                        myElems=>locArr[(...locArrInd)].myElems[(...locSlice)]);

      if thisid == here.uid then
        alias.myLocArr = alias.locArr[ind];
    }
  }
  return alias;
}

def BlockArr.dsiReindex(d: BlockDom) {
  var alias = new BlockArr(eltType=eltType, rank=d.rank, idxType=d.idxType,
                           stridable=d.stridable, dom=d);

  var thisid = this.locale.uid;
  coforall i in d.dist.targetLocDom {
    on d.dist.targetLocales(i) {
      const locDom = d.getLocDom(i);
      var locAlias: [locDom.myBlock] => locArr[i].myElems;
      alias.locArr[i] = new LocBlockArr(eltType=eltType,
                                        rank=rank, idxType=d.idxType,
                                        stridable=d.stridable,
                                        locDom=locDom,
                                        myElems=>locAlias);
      if thisid == here.uid then
        alias.myLocArr = alias.locArr[i];
    }
  }

  return alias;
}

def BlockArr.dsiReallocate(d: domain) {
  //
  // For the default arithmetic array, this function changes the data
  // vector in the array class so that it is setup once the default
  // arithmetic domain is changed.  For this distributed array class,
  // we don't need to do anything, because changing the domain will
  // change the domain in the local array class which will change the
  // data in the local array class.  This will only work if the domain
  // we are reallocating to has the same distribution, but domain
  // assignment is defined so that only the indices are transferred.
  // The distribution remains unchanged.
  //
}

//
// the accessor for the local array -- assumes the index is local
//
def LocBlockArr.this(i) var {
  return myElems(i);
}

//
// Privatization
//
def Block.Block(other: Block, privateData,
                param rank = other.rank,
                type idxType = other.idxType) {
  boundingBox = [(...privateData(1))];
  targetLocDom = [(...privateData(2))];
  dataParTasksPerLocale = privateData(3);
  dataParIgnoreRunningTasks = privateData(4);
  dataParMinGranularity = privateData(5);

  for i in targetLocDom {
    targetLocales(i) = other.targetLocales(i);
    locDist(i) = other.locDist(i);
  }
}

def Block.dsiSupportsPrivatization() param return true;

def Block.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity);
}

def Block.dsiPrivatize(privatizeData) {
  return new Block(this, privatizeData);
}

def Block.dsiGetReprivatizeData() return boundingBox.dims();

def Block.dsiReprivatize(other, reprivatizeData) {
  boundingBox = [(...reprivatizeData)];
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}

def Block.writeBinBlock_real(f: file, in rank : int) {

  var gres=0:int(64);
  var gerr=0:int;
  var fpos=f.chpl_ftell();
  if debugBlockDist then writeln("writeBinBlock, ftell:",fpos);

  binfwrite(rank,4,1,f._fp,gres,gerr);

  for dim in 1..rank do {
    binfwrite(boundingBox.dim(dim)._low,4,1,f._fp,gres,gerr);
    binfwrite(boundingBox.dim(dim)._high,4,1,f._fp,gres,gerr);
    binfwrite(boundingBox.dim(dim)._stride,4,1,f._fp,gres,gerr);
    if debugBlockDist then {
                writeln("Writting Dom dim:",dim," : ",boundingBox.dim(dim)._low);
                writeln("Writting Dom dim:",dim," : ",boundingBox.dim(dim)._high);
                writeln("Writting Dom dim:",dim," : ",boundingBox.dim(dim)._stride);
    }
  }

}


def Block.writeBinBlock(f: file) {
   writeBinBlock_real(f,rank);
}

def Block.readBinBlock_real(f: file, in rank : int) {

  var gres=0:int(64);
  var gerr=0:int;
  for dim in 1..rank do {
    if debugBlockDist then {
                writeln("before read Block dim:",dim," : ",boundingBox.dim(dim)._low);
                writeln("before read Block dim:",dim," : ",boundingBox.dim(dim)._high);
                writeln("before read Block dim:",dim," : ",boundingBox.dim(dim)._stride);
    }
  }
  var fpos=f.chpl_ftell();
  if debugBlockDist then writeln("readBinBlock begin, ftell:",fpos);

  binfread(rank,4,1,f._fp,gres,gerr);
  if debugBlockDist then writeln("Read Block rank:",rank);

  var rr:int;
  for dim in 1..rank do {
    binfread(boundingBox.dim(dim)._low,4,1,f._fp,gres,gerr);
    binfread(boundingBox.dim(dim)._high,4,1,f._fp,gres,gerr);
    binfread(boundingBox.dim(dim)._stride,4,1,f._fp,gres,gerr);
    if debugBlockDist then {
                writeln("Read Block dim:",dim," : ",boundingBox.dim(dim)._low," rr:",rr);
                writeln("Read Block dim:",dim," : ",boundingBox.dim(dim)._high);
                writeln("Read Block dim:",dim," : ",boundingBox.dim(dim)._stride);
    }
  }
  fpos=f.chpl_ftell();
  if debugBlockDist then writeln("readBinDom end, ftell:",fpos);


}


def Block.readBinBlock(f: file) {
   readBinBlock_real(f,rank);
}




def BlockDom.dsiSupportsPrivatization() param return true;

def BlockDom.dsiGetPrivatizeData() return (dist.pid, whole.dims());

def BlockDom.dsiPrivatize(privatizeData) {
  var distpid = privatizeData(1);
  var thisdist = dist;
  var privdist = __primitive("chpl_getPrivatizedClass", thisdist, distpid);
  var c = new BlockDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist);
  for i in c.dist.targetLocDom do
    c.locDoms(i) = locDoms(i);
  c.whole = [(...privatizeData(2))];
  return c;
}

def BlockDom.dsiGetReprivatizeData() return whole.dims();

def BlockDom.dsiReprivatize(other, reprivatizeData) {
  for i in dist.targetLocDom do
    locDoms(i) = other.locDoms(i);
  whole = [(...reprivatizeData)];
}

def BlockArr.dsiSupportsPrivatization() param return true;

def BlockArr.dsiGetPrivatizeData() return dom.pid;

def BlockArr.dsiPrivatize(privatizeData) {
  var dompid = privatizeData;
  var thisdom = dom;
  var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
  var c = new BlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
  for localeIdx in c.dom.dist.targetLocDom {
    c.locArr(localeIdx) = locArr(localeIdx);
    if c.locArr(localeIdx).locale.uid == here.uid then
      c.myLocArr = c.locArr(localeIdx);
  }
  return c;
}
