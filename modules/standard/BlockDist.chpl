//
// Block Distribution
// 
//      Block       BlockDom     BlockArr
//
//   LocBlock    LocBlockDom  LocBlockArr
//

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
  type idxType = int(64);

  const boundingBox: domain(rank, idxType);
  const targetLocDom: domain(rank);
  const targetLocs: [targetLocDom] locale;
  const locDist: [targetLocDom] LocBlock(rank, idxType);

  var tasksPerLocale: int; // tasks per locale for forall iteration

  var pid: int = -1; // privatized object id

  def Block(param rank: int,
            type idxType = int(64),
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

  def clone() {
    return new Block(rank, idxType, boundingBox, targetLocs, tasksPerLocale);
  }

  def destroyDistributionDescriptor() {
    coforall ld in locDist do {
      on ld do
        delete ld;
    }
  }
}

def Block.supportsPrivatization() param return true;

def Block.getPrivatizeData() {
  return (boundingBox.dims(), targetLocDom.dims(), tasksPerLocale);
}

def Block.privatize(privatizeData) {
  return new Block(rank=rank, idxType=idxType, this, privatizeData);
}

def Block.reprivatize(other) {
  boundingBox = other.boundingBox;
  targetLocDom = other.targetLocDom;
  targetLocs = other.targetLocs;
  locDist = other.locDist;
  tasksPerLocale = other.tasksPerLocale;
}

//
// create a new arithmetic domain over this distribution
//
def Block.newArithmeticDom(param rank: int, type idxType,
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
      const (blo, bhi) = _computeBlock(min(idxType), numelems, lo,
                                       max(idxType), numlocs, locid);
      myChunk = [blo..bhi];
    } else {
      var tuple: rank*range(idxType);
      for param i in 1..rank {
        const lo = boundingBox(i).low;
        const hi = boundingBox(i).high;
        const numelems = hi - lo + 1;
        const numlocs = targetLocBox(i).length;
        const (blo, bhi) = _computeBlock(min(idxType), numelems, lo,
                                         max(idxType), numlocs, locid(i));
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

def BlockDom.dims() return whole.dims();

def BlockDom.dim(d: int) return whole.dim(d);

// stopgap to avoid accessing locDoms field (and returning an array)
def BlockDom.getLocDom(localeIdx) return locDoms(localeIdx);

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
        const (lo,hi) = _computeBlock(locBlock(1).low, locBlock(1).length,
                                      locBlock(1).low, locBlock(1).high,
                                      numTasks, taskid);
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

//------------------- 12/8/09 to here!!!

def BlockDom.strideBy(str: int) {
  var alias = new BlockDom(rank=rank, idxType=idxType, stridable=true, dist=dist);
  var t: rank*range(eltType=idxType, stridable=true);
  for i in 1..rank {
    t(i) = this.dim(i) by str;
  }
  alias.setIndices(t);
  return alias;
}

def BlockDom.strideBy(str: rank*int) {
  var alias = new BlockDom(rank=rank, idxType=idxType, stridable=true, dist=dist);
  var t: rank*range(eltType=idxType, stridable=true);
  for i in 1..rank {
    t(i) = this.dim(i) by str(i);
  }
  alias.setIndices(t);
  return alias;
}


//
// output domain
//
def BlockDom.writeThis(x:Writer) {
  x.write(whole);
}

//
// how to allocate a new array over this domain
//
def BlockDom.buildArray(type eltType) {
  var arr = new BlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}

def BlockDom.numIndices return whole.numIndices;
def BlockDom.low return whole.low;
def BlockDom.high return whole.high;

//
// INTERFACE NOTES: Could we make setIndices() for an arithmetic
// domain take a domain rather than something else?
//
def BlockDom.setIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
}

def BlockDom.setIndices(x) {
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

def BlockDom.getIndices() {
  return whole.getIndices();
}

def BlockDom.getDist(): Block(idxType) {
  return dist;
}

def BlockDom.slice(param stridable: bool, ranges) {
  var d = new BlockDom(rank=rank, idxType=idxType, dist=dist, stridable=stridable||this.stridable);
  d.setIndices(whole((...ranges)).getIndices());
  return d;
}

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

def BlockDom.supportsPrivatization() param return true;

def BlockDom.getPrivatizeData() return (dist.pid, whole.dims());

def BlockDom.privatize(privatizeData) {
  var distpid = privatizeData(1);
  var thisdist = dist;
  var privdist = __primitive("chpl_getPrivatizedClass", thisdist, distpid);
  var c = new BlockDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist);
  for i in c.dist.targetLocDom do
    c.locDoms(i) = locDoms(i);
  c.whole = [(...privatizeData(2))];
  return c;
}

def BlockDom.getReprivatizeData() return whole.dims();

def BlockDom.reprivatize(other, reprivatizeData) {
  for i in dist.targetLocDom do
    locDoms(i) = other.locDoms(i);
  whole = [(...reprivatizeData)];
}

def BlockDom.member(i) {
  return whole.member(i);
}

def BlockDom.order(i) {
  return whole.order(i);
}

def BlockDom.position(i) {
  return whole.position(i);
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
// queries for this locale's number of indices, low, and high bounds
//
// TODO: I believe these are only used by the random number generator
// in stream -- will they always be required once that is rewritten?
//
def LocBlockDom.numIndices {
  return myBlock.numIndices;
}

def LocBlockDom.low {
  return myBlock.low;
}

def LocBlockDom.high {
  return myBlock.high;
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

def BlockArr.getBaseDom() return dom;

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

def BlockArr.supportsPrivatization() param return true;

def BlockArr.getPrivatizeData() return dom.pid;

def BlockArr.privatize(privatizeData) {
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
def BlockArr.this(i: idxType) var where rank == 1 {
  if myLocArr then local {
    if myLocArr.locDom.member(i) then
      return myLocArr.this(i);
  }
  return locArr(dom.dist.ind2locInd(i))(i);
}

def BlockArr.this(i: rank*idxType) var {
//   const myLocArr = locArr(here.id);
//   local {
//     if myLocArr.locDom.member(i) then
//       return myLocArr.this(i);
//   }
  if rank == 1 {
    return this(i(1));
  } else {
    return locArr(dom.dist.ind2locInd(i))(i);
  }
}

def BlockArr.these() var {
  for i in dom do
    yield this(i);
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
        const (lo,hi) = _computeBlock(locBlock(1).low, locBlock(1).length,
                                      locBlock(1).low, locBlock(1).high,
                                      numTasks, taskid);
          
        tuple(1) = lo..hi;
        yield tuple;

      }
    }
  }
}

def BlockArr.supportsAlignedFollower() param return true;

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
    // if arrSection is not local and we're not aligned, it means that
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
      return this(i);
    }
    for i in followThisDom {
      yield accessHelper(i);
    }
  }
}

//
// output array
//
def BlockArr.writeThis(f: Writer) {
  if dom.numIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dim(dim)._low;
  label next while true {
    f.write(this(i));
    if i(rank) <= (dom.dim(rank)._high - dom.dim(rank)._stride:idxType) {
      f.write(" ");
      i(rank) += dom.dim(rank)._stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dim(dim)._high - dom.dim(dim)._stride:idxType) {
          i(dim) += dom.dim(dim)._stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dim(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

def BlockArr.numElements return dom.numIndices;

def BlockArr.checkSlice(ranges) {
  for param i in 1..rank do
    if !dom.dim(i).boundsCheck(ranges(i)) then {
      writeln(dom.dim(i), " ", ranges(i), " ", dom.dim(i).boundsCheck(ranges(i)));
      halt("array slice out of bounds in dimension ", i, ": ", ranges(i));
    }
}


def BlockArr.slice(d: BlockDom) {
  var alias = new BlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d, pid=pid);
  for i in dom.dist.targetLocDom {
    on dom.dist.targetLocs(i) {
      alias.locArr[i] = new LocBlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, locDom=d.locDoms[i]);
      alias.locArr[i].myElems => locArr[i].myElems[alias.locArr[i].locDom.myBlock];
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

def LocBlockArr.numElements return myElems.numElements;


//
// helper function for blocking index ranges
//
// 12/8/09 note: move this for sharing with forall over default
//               arrays, etc.
//
def _computeBlock(waylo, numelems, lo, wayhi, numblocks, blocknum) {
  def intCeilXDivByY(x, y) return ((x + (y-1)) / y);

  const blo =
    if blocknum == 0 then waylo
    else lo + intCeilXDivByY(numelems * blocknum:lo.type, numblocks:lo.type);
  const bhi =
    if blocknum == numblocks - 1 then wayhi
    else lo + intCeilXDivByY(numelems * (blocknum+1):lo.type, numblocks:lo.type) - 1;

  return (blo, bhi);
}

//
// naive routine for dividing numLocales into rank factors
//
def _factor(param rank: int, value) {
  var factors: rank*int;
  for param i in 1..rank do
    factors(i) = 1;
  if value >= 1 {
    var iv = value;
    var factor = 1;
    while iv > 1 {
      for i in 2..iv {
        if iv % i == 0 {
          var j = 1;
          for i in 2..rank {
            if factors(i) < factors(j) then
              j = i;
          }
          factors(j) *= i;
          iv = iv / i;
          break;
        }
      }
    }
  }
  for i in 1..rank do
    for j in i+1..rank do
      if factors(i) < factors(j) then
        factors(i) <=> factors(j);
  return factors;
}
