//
// Block Distribution
// 
//      Block       BlockDom     BlockArr
//
//   LocBlock    LocBlockDom  LocBlockArr
//

//
// TODO List
//
// 1. Make multi-dimensional
// 2. Support strided domains of locales
// 3. Support strided Block domains
// 4. Support slices
//
// Limitations
//
// 1. Changes to Block.tasksPerLocale are not made in privatized
//    copies of this distribution.
//


config param debugBlockDist = false; // internal development flag (debugging)


////////////////////////////////////////////////////////////////////////////////
// Block Distribution Class
//
class Block : BaseDist {
  param rank: int = 1;
  type idxType = int(64);

  const boundingBox: domain(rank, idxType);
  const targetLocDom: domain(rank);
  const targetLocs: [targetLocDom] locale;
  const locDist: [targetLocDom] LocBlock(rank, idxType);

  var tasksPerLocale: int; // tasks per locale for forall iteration

  def Block(param rank: int,
            type idxType = int(64),
            bbox: domain(rank, idxType),
            targetLocales: [] locale = thisRealm.Locales, 
            tasksPerLocale = 0) {
    boundingBox = bbox;
    if rank == 1 {
      targetLocDom = [0..#targetLocales.numElements]; // 0-based for simplicity
      targetLocs = targetLocales;
    } else if targetLocales.rank == 1 then {
      const factors = _factor(rank, targetLocales.numElements);
      var ranges: rank*range;
      for param i in 1..rank do
        ranges(i) = 0..factors(i)-1;
      targetLocDom = [(...ranges)];
      for (loc1, loc2) in (targetLocs, targetLocales) do
        loc1 = loc2;
      if debugBlockDist {
        writeln(targetLocDom);
        writeln(targetLocs);
      }
    } else {
      if targetLocales.rank != rank then
        compilerError("locales array rank must be one or match distribution rank");
      halt("case 2 not handled yet");
    }

    coforall locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlock(rank, idxType, locid, this);

    if tasksPerLocale == 0 then
      this.tasksPerLocale = min reduce targetLocs.numCores;
    else
      this.tasksPerLocale = tasksPerLocale;

    if debugBlockDist then
      for loc in locDist do writeln(loc);
  }

  // copy constructor for privatization
  def Block(param rank: int, type idxType, other: Block(rank, idxType)) {
    boundingBox = other.boundingBox;
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    locDist = other.locDist;
    tasksPerLocale = other.tasksPerLocale;
  }
}

//
// create a new arithmetic domain over this distribution
//
def Block.newArithmeticDom(param rank: int, type idxType, param stridable: bool, param alias: bool = false) {
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
    x.writeln("  [", locid, "] locale ", locid, " owns chunk: ", locDist(locid).myChunk);
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
  return locDist(locid).myChunk((...inds.getIndices()));
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

def Block.ind2locInd(ind: rank*idxType) {
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
                 dist: Block(rank, idxType)) { // reference to glob dist
    if rank == 1 {
      const lo = dist.boundingBox.low;
      const hi = dist.boundingBox.high;
      const numelems = hi - lo + 1;
      const numlocs = dist.targetLocDom.numIndices;
      const (blo, bhi) = _computeBlock(min(idxType), numelems, lo,
                                       max(idxType), numlocs, locid);
      myChunk = [blo..bhi];
    } else {
      var tuple: rank*range(idxType);
      for param i in 1..rank {
        const lo = dist.boundingBox.low(i);
        const hi = dist.boundingBox.high(i);
        const numelems = hi - lo + 1;
        const numlocs = dist.targetLocDom.dim(i).length;
        const (blo, bhi) = _computeBlock(min(idxType), numelems, lo,
                                         max(idxType), numlocs, locid(i));
        tuple(i) = blo..bhi;
      }
      myChunk = [(...tuple)];
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
// Block Domain Class
//
class BlockDom: BaseArithmeticDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  param alias: bool = false;
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
  const whole: domain(rank, idxType);

  var pid: int = -1; // privatized object id
}

def BlockDom.dim(d: int) return whole.dim(d);

def BlockDom.these() {
  for i in whole do
    yield i;
}

def BlockDom.these(param tag: iterator) where tag == iterator.leader {
  const precomputedNumTasks = dist.tasksPerLocale;
  const precomputedWholeLow = whole.low;
  coforall locDom in locDoms do on locDom {
    const locBlock = locDom.myBlock - precomputedWholeLow,
      numTasks = precomputedNumTasks;
    if (numTasks == 1) {
      yield locBlock;
    } else {
      coforall taskid in 0..#numTasks {
        if rank == 1 {
          const (lo,hi) = _computeBlock(locBlock.low, locBlock.numIndices,
                                        locBlock.low, locBlock.high,
                                        numTasks, taskid);
          yield [lo..hi];
        } else {
          var tuple: rank*range(idxType);
          const (lo,hi) = _computeBlock(locBlock.low(1), locBlock.dim(1).length,
                                        locBlock.low(1), locBlock.high(1),
                                        numTasks, taskid);
          tuple(1) = lo..hi;
          for param i in 2..rank do
            tuple(i) = locBlock.low(i)..locBlock.high(i);
          yield [(...tuple)];
        }
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
  const followThis = follower + whole.low;

  for i in followThis {
    yield i;
  }
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
  halt("this is never called");
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
  return whole;
}

def BlockDom.getDist(): Block(idxType) {
  return dist;
}

def BlockDom.setup() {
  coforall localeIdx in dist.targetLocDom do
    on dist.targetLocs(localeIdx) do
      if (locDoms(localeIdx) == nil) then
        locDoms(localeIdx) = new LocBlockDom(rank, idxType, stridable, this, 
                                               dist.getChunk(whole, localeIdx));
  else
    locDoms(localeIdx).myBlock = dist.getChunk(whole, localeIdx);
  if debugBlockDist then
    for loc in dist.targetLocDom do writeln(loc, " owns ", locDoms(loc));
 
}

def BlockDom.supportsPrivatization() param return true;

def BlockDom.privatize() {
  var privateDist = new Block(rank, idxType, dist);
  var c = new BlockDom(rank=rank, idxType=idxType, stridable=stridable, dist=privateDist);
  c.locDoms = locDoms;
  c.whole = whole;
  return c;
}

def BlockDom.reprivatize(other) {
  locDoms = other.locDoms;
  whole = other.whole;
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
  var myBlock: domain(rank, idxType);
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
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      locArr(localeIdx) = new LocBlockArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx));
      if this.locale == here then
        myLocArr = locArr(localeIdx);
    }
  }
}

def BlockArr.supportsPrivatization() param return true;

def BlockArr.privatize() {
  var dompid = dom.pid;
  var thisdom = dom;
  var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
  var c = new BlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
  c.locArr = locArr;
  for localeIdx in dom.dist.targetLocDom do
    if c.locArr(localeIdx).locale == here then
      c.myLocArr = c.locArr(localeIdx);
  return c;
}

//
// the global accessor for the array
//
// TODO: Do we need a global bounds check here or in ind2locind?
//
def BlockArr.this(i: idxType) var where rank == 1 {
  if myLocArr then local {
    if myLocArr.locDom.myBlock.member(i) then
      return myLocArr.this(i);
  }
  return locArr(dom.dist.ind2locInd(i))(i);
}

def BlockArr.this(i: rank*idxType) var {
//   const myLocArr = locArr(here.id);
//   local {
//     if myLocArr.locDom.myBlock.member(i) then
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
    const locBlock = locDom.myBlock - precomputedWholeLow,
      numTasks = precomputedNumTasks;
    if (numTasks == 1) {
      yield locBlock;
    } else {
      coforall taskid in 0..#numTasks {
        if rank == 1 {
          const (lo,hi) = _computeBlock(locBlock.low, locBlock.numIndices,
                                        locBlock.low, locBlock.high,
                                        numTasks, taskid);
          yield [lo..hi];
        } else {
          var tuple: rank*range(idxType);
          const (lo,hi) = _computeBlock(locBlock.low(1), locBlock.dim(1).length,
                                        locBlock.low(1), locBlock.high(1),
                                        numTasks, taskid);
          if hi >= lo {
            tuple(1) = lo..hi;
            for param i in 2..rank do
              tuple(i) = locBlock.low(i)..locBlock.high(i);
            yield [(...tuple)];
          }
        }
      }
    }
  }
}

def BlockArr.supportsAlignedFollower() param return true;

def BlockArr.these(param tag: iterator, follower, param aligned: bool = false) var where tag == iterator.follower {
  const followThis = follower + dom.low;

  //
  // TODO: The following is a buggy hack that will only work when we're
  // distributing across the entire Locales array.  I still think the
  // locArr/locDoms arrays should be associative over locale values.
  //
  const myLocArr = locArr(dom.dist.ind2locInd(followThis.low));
  if aligned {
    local {
      for i in followThis {
        yield myLocArr.this(i);
      }
    }
  } else {
    //
    // we don't own all the elements we're following
    //
    def accessHelper(i) var {
//      if myLocArr.locale == here {
//	local {
//          if myLocArr.locDom.myBlock.member(i) then
//            return myLocArr.this(i);
//        }
//      }
      return this(i);
    }
    for i in followThis {
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
def _computeBlock(waylo, numelems, lo, wayhi, numblocks, blocknum) {
  def procToData(x, lo)
    return lo + (x:lo.type) + (x:real != x:int:real):lo.type;

  const blo =
    if blocknum == 0 then waylo
      else procToData((numelems:real * blocknum) / numblocks, lo);
  const bhi =
    if blocknum == numblocks - 1 then wayhi
      else procToData((numelems:real * (blocknum+1)) / numblocks, lo) - 1;

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
