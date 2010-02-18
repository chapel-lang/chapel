//
// Block Distribution
// 
//      Block       BlockDom     BlockArr
//
//   LocBlock    LocBlockDom  LocBlockArr
//

use DSIUtil;
use ChapelUtil;

//
// Limitations
//
// 1. Changes to Block.tasksPerLocale are not made in privatized
//    copies of this distribution.
//

config param debugBlockDist = false; // internal development flag (debugging)
config param sanityCheckDistribution = false; // ditto; should promote to compiler flag

////////////////////////////////////////////////////////////////////////////////
// Block Distribution Class
//
class Block : BaseDist {
  param rank: int;
  type idxType = int;

  const boundingBox: domain(rank, idxType);
  const targetLocDom: domain(rank);
  const targetLocs: [targetLocDom] locale;
  const locDist: [targetLocDom] LocBlock(rank, idxType);

  var tasksPerLocale: int; // tasks per locale for forall iteration

  var pid: int = -1; // privatized object id

  def Block(param rank: int,
            type idxType = int,
            bbox: domain(rank, idxType),
            targetLocales: [] locale = thisRealm.Locales, 
            tasksPerLocale = 0) {
    boundingBox = bbox;
    if rank != 1 && targetLocales.rank == 1 {
      const factors = _factor(rank, targetLocales.numElements);
      var ranges: rank*range;
      for param i in 1..rank do
        ranges(i) = 0..#factors(i);
      targetLocDom = [(...ranges)];
      targetLocs = reshape(targetLocales, targetLocDom);
    } else {
      if targetLocales.rank != rank then
	compilerError("targetLocales rank must equal distribution rank or one");

      var ranges: rank*range;
      for param i in 1..rank do
	ranges(i) = 0..#targetLocales.domain.dim(i).length;

      targetLocDom = [(...ranges)];
      targetLocs = targetLocales;
    }

    if debugBlockDist {
      writeln(targetLocDom);
      writeln(targetLocs);
    }

    const boundingBoxDims = boundingBox.dims();
    const targetLocDomDims = targetLocDom.dims();
    coforall locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) =  new LocBlock(rank, idxType, locid, boundingBoxDims,
                                       targetLocDomDims);

    if tasksPerLocale == 0 then
      this.tasksPerLocale = min reduce targetLocs.numCores;
    else
      this.tasksPerLocale = tasksPerLocale;

    if debugBlockDist then
      for loc in locDist do writeln(loc);
  }

  // copy constructor for privatization
  def Block(param rank: int, type idxType, other: Block(rank, idxType), privateData) {
    boundingBox = [(...privateData(1))];
    targetLocDom = [(...privateData(2))];
    tasksPerLocale = privateData(3);
    for i in targetLocDom {
      targetLocs(i) = other.targetLocs(i);
      locDist(i) = other.locDist(i);
    }
  }

  def dsiClone() {
    return new Block(rank, idxType, boundingBox, targetLocs, tasksPerLocale);
  }

  def dsiDestroyDistClass() {
    coforall ld in locDist do {
      on ld do
        delete ld;
    }
  }
}

def Block.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocs = ", for tl in targetLocs do tl.id);
  writeln("tasksPerLocale = ", tasksPerLocale);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

def Block.dsiSupportsPrivatization() param return true;

def Block.dsiGetPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(), tasksPerLocale);
}

def Block.dsiPrivatize(privatizeData) {
  return new Block(rank=rank, idxType=idxType, this, privatizeData);
}

def Block.dsiReprivatize(other) {
  boundingBox = other.boundingBox;
  targetLocDom = other.targetLocDom;
  targetLocs = other.targetLocs;
  locDist = other.locDist;
  tasksPerLocale = other.tasksPerLocale;
}

//
// create a new arithmetic domain over this distribution
//
def Block.dsiNewArithmeticDom(param rank: int, type idxType,
                           param stridable: bool) {
  if idxType != this.idxType then
    compilerError("Block domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("Block domain rank does not match distribution's");
  
  var dom = new BlockDom(rank=rank, idxType=idxType, dist=this, stridable=stridable);
  dom.setup();
  return dom;
}

//
// output distribution
//
def Block.writeThis(x:Writer) {
  x.writeln("Block");
  x.writeln("-------");
  x.writeln("distributes: ", boundingBox);
  x.writeln("across locales: ", targetLocs);
  x.writeln("indexed via: ", targetLocDom);
  x.writeln("resulting in: ");
  for locid in targetLocDom do
    x.writeln("  [", locid, "] ", locDist(locid));
}

//
// convert an index into a locale value
//
// dsiIndexLocale
//  note - we only need the second one
def Block.ind2loc(ind: idxType) where rank == 1 {
  return targetLocs(ind2locInd(ind));
}

def Block.ind2loc(ind: rank*idxType) {
  return targetLocs(ind2locInd(ind));
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
      if ind2locInd(chunk.low) != locid then
        writeln("[", here.id, "] ", chunk.low, " is in my chunk but maps to ",
                ind2locInd(chunk.low));
      if ind2locInd(chunk.high) != locid then
        writeln("[", here.id, "] ", chunk.high, " is in my chunk but maps to ",
                ind2locInd(chunk.high));
    }
  return chunk;
}

//
// determine which locale owns a particular index
//
// TODO: I jotted down a note during the code review asking whether
// targetLocs.numElements and boundingbox.numIndices should be
// captured locally, or captured in the default dom/array implementation
// or inlined.  Not sure what that point was anymore, though.  Maybe
// someone else can help me remember it (since it was probably someone
// else's suggestion).
//
def Block.ind2locInd(ind: idxType) where rank == 1 {
  const ind0 = ind - boundingBox.low;
  const locInd = (ind0 * targetLocs.numElements:idxType) / boundingBox.numIndices;
  return max(0, min(locInd:int, (targetLocDom.dim(1).length-1):int));
}

def Block.ind2locInd(ind: rank*idxType) where rank == 1 {
  return ind2locInd(ind(1));
}

def Block.ind2locInd(ind: rank*idxType) where rank != 1 {
  var locInd: rank*int;
  for param i in 1..rank {
    const ind0 = ind(i) - boundingBox.low(i);
    const dimLocInd = (ind0 * targetLocDom.dim(i).length:idxType) / boundingBox.dim(i).length;
    locInd(i) = max(0, min(dimLocInd:int, (targetLocDom.dim(i).length-1):int));
  }
  return locInd;
}


def Block.dsiCreateReindexDist(newSpace, oldSpace) {
  // Should this error be in ChapelArray or not an error at all?
  if newSpace(1).eltType != oldSpace(1).eltType then
    compilerError("index type of reindex domain must match that of original domain");
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
   *    newBblow = bbLow - oldLow + newLow
   *
   * Can be performed as:
   *
   *    t = bbLow-oldLow;
   *    newBbLow = t+newLow;
   * or
   *    t = oldLow-newLow;
   *    newBbLow = t+bbLow;
   * or
   *    t = bbLow+newLow;
   *    newBbLow = t-oldLow;
   *
   */
  def adjustBound(bbound, oldBound, newBound) {
    var t: bbound.type;
    if !subUnderflowsType(bbound, oldBound) {
      t = bbound-oldBound;
      if !addOverflowsType(t, newBound) {
        return (t+newBound, true);
      }
    }
    if !subUnderflowsType(oldBound, newBound) {
      t = oldBound-newBound;
      if !addOverflowsType(t, bbound) {
        return (t+bbound, true);
      }
    }
    if !addOverflowsType(bbound, newBound) {
      t = bbound+newBound;
      if !subUnderflowsType(t, oldBound) {
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
  var newDist = new Block(rank, idxType, d, targetLocs, tasksPerLocale);
  return newDist;
}


////////////////////////////////////////////////////////////////////////////////
// Block Local Distribution Class
//
class LocBlock {
  param rank: int;
  type idxType;

  //
  // This stores the piece of the global bounding box owned by
  // the locale.  Note that my original guess that we'd want
  // to use lclIdxType here is wrong since we're talking about
  // the section of the global index space owned by the locale.
  //
  const myChunk: domain(rank, idxType);

  //
  // Constructor computes what chunk of index(1) is owned by the
  // current locale
  //
  def LocBlock(param rank: int,
               type idxType, 
               locid,   // the locale index from the target domain
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
}


def LocBlock.writeThis(x:Writer) {
  var localeid: int;
  on this {
    localeid = here.uid;
  }
  x.write("locale ", localeid, " owns chunk: ", myChunk);
}


////////////////////////////////////////////////////////////////////////////////
// Block Domain Class
//
class BlockDom: BaseArithmeticDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  //
  // LEFT LINK: a pointer to the parent distribution
  //
  const dist: Block(rank, idxType);

  //
  // DOWN LINK: an array of local domain class descriptors -- set up in
  // setup() below
  //
  var locDoms: [dist.targetLocDom] LocBlockDom(rank, idxType, stridable);

  //
  // a domain describing the complete domain
  //
  const whole: domain(rank=rank, idxType=idxType, stridable=stridable);

  var pid: int = -1; // privatized object id

  def getBaseDist() return dist;
}

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
  const collapsedLocInd = ind2locInd(collapsedDimLocs);
  var collapsedBbox: _matchArgsShape(range(eltType=idxType), idxType, args);
  var collapsedLocs: _matchArgsShape(range(eltType=int), int, args);

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
  const newTargetLocs = targetLocs((...collapsedLocs));
  return new Block(rank=newRank, idxType=idxType, bbox=newBbox,
                   targetLocales=newTargetLocs,
                   tasksPerLocale=tasksPerLocale);
}

def BlockDom.these() {
  for i in whole do
    yield i;
}

def BlockDom.these(param tag: iterator) where tag == iterator.leader {
  const precomputedNumTasks = dist.tasksPerLocale;
  const precomputedWholeLow = whole.low;
  coforall locDom in locDoms do on locDom {
    var tmpBlock = locDom.myBlock - precomputedWholeLow;
    const numTasks = precomputedNumTasks;

    var locBlock: rank*range(idxType);
    for param i in 1..tmpBlock.rank {
      locBlock(i) = (tmpBlock.dim(i).low/tmpBlock.dim(i).stride:idxType)..#(tmpBlock.dim(i).length);
    }
    if (numTasks == 1) {
      yield locBlock;
    } else {
      coforall taskid in 0..#numTasks {
        var tuple: rank*range(idxType) = locBlock;
        const (lo,hi) = _computeBlock(locBlock(1).length, numTasks, taskid,
                                      locBlock(1).high,
                                      locBlock(1).low, locBlock(1).low);
        tuple(1) = lo..hi;
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
  var t: rank*range(idxType, stridable=stridable);
  for param i in 1..rank {
    var stride = whole.dim(i).stride: idxType;
    var low = stride * follower(i).low;
    var high = stride * follower(i).high;
    t(i) = (low..high by stride:int) + whole.dim(i).low;
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
}

def BlockDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(1).eltType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  setup();
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
      on dist.targetLocs(localeIdx) do
        locDoms(localeIdx) = new LocBlockDom(rank, idxType, stridable, this, 
                                             dist.getChunk(whole, localeIdx));
    }
  } else {
    coforall localeIdx in dist.targetLocDom do {
      on dist.targetLocs(localeIdx) do
        locDoms(localeIdx).myBlock = dist.getChunk(whole, localeIdx);
    }
  }
  if debugBlockDist then
    for loc in dist.targetLocDom do writeln(loc, " owns ", locDoms(loc));
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

////////////////////////////////////////////////////////////////////////////////
// Block Local Domain Class
//
class LocBlockDom {
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // UP LINK: a reference to the parent global domain class
  //
  const wholeDom: BlockDom(rank, idxType, stridable);

  //
  // a local domain describing the indices owned by this locale
  //
  // NOTE: I used to use a local index type for this, but that would
  // require a glbIdxType offset in order to get from the global
  // indices back to the local index type.
  //
  var myBlock: domain(rank, idxType, stridable);
}

//
// output local domain piece
//
def LocBlockDom.writeThis(x:Writer) {
  x.write(myBlock);
}

//
// Added as a performance stopgap to avoid returning a domain
//
def LocBlockDom.member(i) return myBlock.member(i);

////////////////////////////////////////////////////////////////////////////////
// Block Array Class
//
class BlockArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: the global domain descriptor for this array
  //
  var dom: BlockDom(rank, idxType, stridable);

  //
  // DOWN LINK: an array of local array classes
  //
  var locArr: [dom.dist.targetLocDom] LocBlockArr(eltType, rank, idxType, stridable);

  //
  // optimized reference to a local LocBlockArr instance (or nil)
  //
  var myLocArr: LocBlockArr(eltType, rank, idxType, stridable);

  var pid: int = -1; // privatized object id
}

def BlockArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom do
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
}

def BlockArr.dsiGetBaseDom() return dom;

def BlockArr.setup() {
  var thisid = this.locale.uid;
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      const locDom = dom.getLocDom(localeIdx);
      locArr(localeIdx) = new LocBlockArr(eltType, rank, idxType, stridable, locDom);
      if thisid == here.uid then
        myLocArr = locArr(localeIdx);
    }
  }
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

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in ind2locind?
//
def BlockArr.dsiAccess(i: idxType) var where rank == 1 {
  if myLocArr then local {
    if myLocArr.locDom.member(i) then
      return myLocArr.this(i);
  }
  return locArr(dom.dist.ind2locInd(i))(i);
}

def BlockArr.dsiAccess(i: rank*idxType) var {
  if rank == 1 {
    return dsiAccess(i(1));
  } else {
    if myLocArr then local {
      if myLocArr.locDom.member(i) then
        return myLocArr.this(i);
    }
    return locArr(dom.dist.ind2locInd(i))(i);
  }
}

def BlockArr.these() var {
  for i in dom do
    yield dsiAccess(i);
}

//
// TODO: Rewrite this to reuse more of the global domain iterator
// logic?  (e.g., can we forward the forall to the global domain
// somehow?
//
def BlockArr.these(param tag: iterator) where tag == iterator.leader {
  const precomputedNumTasks = dom.dist.tasksPerLocale;
  const precomputedWholeLow = dom.whole.low;
  coforall locDom in dom.locDoms do on locDom {
    var tmpBlock = locDom.myBlock - precomputedWholeLow;
    const numTasks = precomputedNumTasks;
    var locBlock: rank*range(idxType);
    for param i in 1..tmpBlock.rank {
      locBlock(i) = (tmpBlock.dim(i).low/tmpBlock.dim(i).stride:idxType)..#(tmpBlock.dim(i).length);
    }


    if (numTasks == 1) {
      yield locBlock;
    } else {
      coforall taskid in 0..#numTasks {
        var tuple: rank*range(idxType) = locBlock;
        const (lo,hi) = _computeBlock(locBlock(1).length, numTasks, taskid,
                                      locBlock(1).high,
                                      locBlock(1).low, locBlock(1).low);
          
        tuple(1) = lo..hi;
        yield tuple;

      }
    }
  }
}

def BlockArr.dsiSupportsAlignedFollower() param return true;

def BlockArr.these(param tag: iterator, follower, param aligned: bool = false) var where tag == iterator.follower {
  var followThis: rank*range(eltType=idxType, stridable=stridable);
  var lowIdx: rank*idxType;

  for param i in 1..rank {
    var stride = dom.whole.dim(i).stride;
    var low = follower(i).low * stride;
    var high = follower(i).high * stride;
    followThis(i) = (low..high by stride) + dom.whole.dim(i).low;
    lowIdx(i) = followThis(i).low;
  }
  const followThisDom = [(...followThis)];

  //
  // TODO: The following is a buggy hack that will only work when we're
  // distributing across the entire Locales array.  I still think the
  // locArr/locDoms arrays should be associative over locale values.
  //
  var arrSection = locArr(dom.dist.ind2locInd(lowIdx));
  if aligned {
    //
    // if arrSection is not local and we're aligned, it means that
    // followThisDom is empty; make arrSection local so that we can
    // use the local block below
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
      local {
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
    on d.dist.targetLocs(i) {
      alias.locArr[i] = new LocBlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, locDom=d.locDoms[i]);
      alias.locArr[i].myElems => locArr[i].myElems[alias.locArr[i].locDom.myBlock];
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
  var A => locArr(dom.dist.ind2locInd(low)).myElems((...ranges));
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
    on d.dist.targetLocs(ind) {
      const locDom = d.getLocDom(ind);
      // locSlice is a tuple of ranges and scalars. It will match the basic
      // shape of the args argument. 
      var locSlice: _matchArgsShape(range(eltType=idxType, stridable=stridable), idxType, args);
      // collapsedDims stores the value any collapsed dimension is down to.
      // For any non-collapsed dimension, that position is ignored.
      // This tuple is then passed to the ind2LocInd function to build up a
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
      locArrInd = dom.dist.ind2locInd(collapsedDims);
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

      alias.locArr[ind] = new LocBlockArr(eltType=eltType, rank=newRank, idxType=d.idxType, stridable=d.stridable, locDom=locDom);
      // Set the local portion of the new array to alias to a rank changed slice
      // of the local portion of this
      alias.locArr[ind].myElems =>
        locArr[(...locArrInd)].myElems[(...locSlice)];

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
    on d.dist.targetLocs(i) {
      const locDom = d.getLocDom(i);
      alias.locArr[i] = new LocBlockArr(eltType=eltType,
                                        rank=rank, idxType=d.idxType,
                                        stridable=d.stridable,
                                        locDom=locDom);
      alias.locArr[i].myElems => locArr[i].myElems;
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

////////////////////////////////////////////////////////////////////////////////
// Block Local Array Class
//
class LocBlockArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: a reference to the local domain class for this array and locale
  //
  const locDom: LocBlockDom(rank, idxType, stridable);


  // STATE:

  //
  // the block of local array data
  //
  var myElems: [locDom.myBlock] eltType;
}

//
// the accessor for the local array -- assumes the index is local
//
def LocBlockArr.this(i) var {
  return myElems(i);
}

//
// output local array piece
//
def LocBlockArr.writeThis(x: Writer) {
  // note on this fails; see writeThisUsingOn.chpl
  x.write(myElems);
}

