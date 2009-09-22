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


config param debugBlockCyclicDist = false; // internal development flag (debugging)


////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Distribution Class
//
class BlockCyclic : BaseDist {
  param rank: int;
  type idxType = int(64);

  const lowIdx: rank*idxType;
  const blocksize: rank*int(32);
  const targetLocDom: domain(rank);
  const targetLocs: [targetLocDom] locale;
  const locDist: [targetLocDom] LocBlockCyclic(rank, idxType);

  var tasksPerLocale: int; // tasks per locale for forall iteration

  def BlockCyclic(param rank: int,
                  type idxType = int(64),
                  low: rank*idxType,
                  blk: rank*int(32),
                  targetLocales: [] locale = thisRealm.Locales, 
                  tasksPerLocale = 0) {
    lowIdx = low;
    blocksize = blk;
    if rank == 1 {
      targetLocDom = [0..#targetLocales.numElements]; // 0-based for simplicity
      targetLocs = targetLocales;
    } else if targetLocales.rank == 1 then {

      // BLC: Common code, factor out

      const factors = _factor(rank, targetLocales.numElements);
      var ranges: rank*range;
      for param i in 1..rank do
        ranges(i) = 0..factors(i)-1;
      targetLocDom = [(...ranges)];
      for (loc1, loc2) in (targetLocs, targetLocales) do
        loc1 = loc2;
      if debugBlockCyclicDist {
        writeln(targetLocDom);
        writeln(targetLocs);
      }
    } else {
      if targetLocales.rank != rank then
	compilerError("locales array rank must be one or match distribution rank");

      var ranges: rank*range;
      for param i in 1..rank do {
	var thisRange = targetLocales.domain.dim(i);
	ranges(i) = 0..#thisRange.length; 
      }
      
      targetLocDom = [(...ranges)];
      if debugBlockCyclicDist then writeln(targetLocDom);

      targetLocs = reshape(targetLocales, targetLocDom);
      if debugBlockCyclicDist then writeln(targetLocs);
    }

    coforall locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlockCyclic(rank, idxType, locid, this);

    if tasksPerLocale == 0 then
      this.tasksPerLocale = here.numCores;
    else
      this.tasksPerLocale = tasksPerLocale;

    if debugBlockCyclicDist then
      for loc in locDist do writeln(loc);
  }

  // copy constructor for privatization
  def BlockCyclic(param rank: int, type idxType, other: BlockCyclic(rank, idxType)) {
    lowIdx = other.lowIdx;
    blocksize = other.blocksize;
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    locDist = other.locDist;
    tasksPerLocale = other.tasksPerLocale;
  }
}

//
// create a new arithmetic domain over this distribution
//
def BlockCyclic.newArithmeticDom(param rank: int, type idxType,
                           param stridable: bool) {
  if idxType != this.idxType then
    compilerError("BlockCyclic domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("BlockCyclic domain rank does not match distribution's");

  var dom = new BlockCyclicDom(rank=rank, idxType=idxType, dist=this, stridable=stridable);
  dom.setup();
  return dom;
}

//
// output distribution
//
def BlockCyclic.writeThis(x:Writer) {
  x.writeln("BlockCyclic");
  x.writeln("-------");
  x.writeln("distributes: ", lowIdx, "...");
  x.writeln("in chunks of: ", blocksize);
  x.writeln("across locales: ", targetLocs);
  x.writeln("indexed via: ", targetLocDom);
  x.writeln("resulting in: ");
  for locid in targetLocDom do
    x.writeln("  [", locid, "] ", locDist(locid));
}

//
// convert an index into a locale value
//
def BlockCyclic.ind2loc(ind: idxType) where rank == 1 {
  return targetLocs(ind2locInd(ind));
}

def BlockCyclic.ind2loc(ind: rank*idxType) {
  return targetLocs(ind2locInd(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
def BlockCyclic.getStarts(inds, locid) {
  // use domain slicing to get the intersection between what the
  // locale owns and the domain's index set
  //
  // TODO: Should this be able to be written as myChunk[inds] ???
  //
  // TODO: Does using David's detupling trick work here?
  //
  var D: domain(rank, idxType, stridable=true);
  var R: rank*range(idxType, stridable=true);
  for i in 1..rank {
    var lo, hi: idxType;
    const domlo = inds.dim(i).low, 
          domhi = inds.dim(i).high;
    const mylo = locDist(locid).myStarts(i).low;
    const mystr = locDist(locid).myStarts(i).stride;
    if (domlo != lowIdx(i)) {
      if (domlo <= domhi) {
        writeln("domlo = ", domlo, "; lowIdx(", i, ") = ", lowIdx(i));
        halt("BLC: need to handle the case where low bound is not the start");
      } else {
        lo = domlo;
        hi = domhi;
      }
    } else {
      lo = mylo;
      hi = domhi;
    }
    R(i) = lo..hi by mystr;
  }
  D.setIndices(R);
  return D;
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
def BlockCyclic.ind2locInd(ind: idxType) where rank == 1 {
  const ind0 = ind - lowIdx;
  const locInd = (ind0 / blocksize) / targetLocs.numElements:idxType;
  return max(0, min(locInd:int, (targetLocDom.dim(1).length-1):int));
}

def BlockCyclic.ind2locInd(ind: rank*idxType) where rank == 1 {
  return ind2locInd(ind(1));
}

def BlockCyclic.ind2locInd(ind: rank*idxType) where rank != 1 {
  var locInd: rank*int;
  for param i in 1..rank {
    const ind0 = ind(i) - lowIdx(i);
    const dimLocInd = (ind0 / blocksize(i)) / targetLocDom.dim(i).length;
    locInd(i) = max(0, min(dimLocInd:int, (targetLocDom.dim(i).length-1):int));
  }
  return locInd;
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Local Distribution Class
//
class LocBlockCyclic {
  param rank: int;
  type idxType;

  //
  // This stores the piece of the global bounding box owned by
  // the locale.  Note that my original guess that we'd want
  // to use lclIdxType here is wrong since we're talking about
  // the section of the global index space owned by the locale.
  //
  const myStarts: rank*range(idxType, BoundedRangeType.boundedLow, stridable=true);

  //
  // Constructor computes what chunk of index(1) is owned by the
  // current locale
  //
  def LocBlockCyclic(param rank: int,
                 type idxType, 
                 locid,   // the locale index from the target domain
                 dist: BlockCyclic(rank, idxType)) { // reference to glob dist
    if rank == 1 {
      const lo = dist.lowIdx(1) + (locid * dist.blocksize(1));
      const str = dist.blocksize(1) * dist.targetLocDom.numIndices;
      myStarts(1) = lo.. by str;
    } else {
      for param i in 1..rank {
        const lo = dist.lowIdx(i) + (locid(i) * dist.blocksize(i));
        const str = dist.blocksize(i) * dist.targetLocDom.dim(i).length;
        myStarts(i) = lo.. by str;
      }
    }
  }
}


def LocBlockCyclic.writeThis(x:Writer) {
  var localeid: int;
  on this {
    localeid = here.id;
  }
  x.write("locale ", localeid, " owns blocks: ", myStarts);
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Domain Class
//
class BlockCyclicDom: BaseArithmeticDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  //
  // LEFT LINK: a pointer to the parent distribution
  //
  const dist: BlockCyclic(rank, idxType);

  //
  // DOWN LINK: an array of local domain class descriptors -- set up in
  // setup() below
  //
  var locDoms: [dist.targetLocDom] LocBlockCyclicDom(rank, idxType, stridable);

  //
  // a domain describing the complete domain
  //
  const whole: domain(rank=rank, idxType=idxType, stridable=stridable);
  //  const startLoc: index(dist.targetLocDom);

  var pid: int = -1; // privatized object id
}

def BlockCyclicDom.dim(d: int) return whole.dim(d);

def BlockCyclicDom.these() {
  for i in whole do
    yield i;
}

def BlockCyclicDom.these(param tag: iterator) where tag == iterator.leader {
  const precomputedNumTasks = dist.tasksPerLocale;
  const precomputedWholeLow = whole.low;
  coforall locDom in locDoms do on locDom {
    var tmpBlockCyclic = locDom.myStarts - precomputedWholeLow;
    const numTasks = precomputedNumTasks;

    var locBlockCyclic: rank*range(idxType);
    for param i in 1..tmpBlockCyclic.rank {
      locBlockCyclic(i) = (tmpBlockCyclic.dim(i).low/tmpBlockCyclic.dim(i).stride:idxType)..#(tmpBlockCyclic.dim(i).length);
    }
    if (numTasks == 1) {
      yield locBlockCyclic;
    } else {
      coforall taskid in 0..#numTasks {
        var tuple: rank*range(idxType) = locBlockCyclic;
        const (lo,hi) = _computeBlockCyclic(locBlockCyclic(1).low, locBlockCyclic(1).length,
                                      locBlockCyclic(1).low, locBlockCyclic(1).high,
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
def BlockCyclicDom.these(param tag: iterator, follower) where tag == iterator.follower {
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

def BlockCyclicDom.strideBy(str: int) {
  var alias = new BlockCyclicDom(rank=rank, idxType=idxType, stridable=true, dist=dist);
  var t: rank*range(eltType=idxType, stridable=true);
  for i in 1..rank {
    t(i) = this.dim(i) by str;
  }
  alias.setIndices(t);
  return alias;
}

def BlockCyclicDom.strideBy(str: rank*int) {
  var alias = new BlockCyclicDom(rank=rank, idxType=idxType, stridable=true, dist=dist);
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
def BlockCyclicDom.writeThis(x:Writer) {
  x.write(whole);
}

//
// how to allocate a new array over this domain
//
/*
def BlockCyclicDom.buildArray(type eltType) {
  var arr = new BlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
  arr.setup();
  return arr;
}
*/

def BlockCyclicDom.numIndices return whole.numIndices;
def BlockCyclicDom.low return whole.low;
def BlockCyclicDom.high return whole.high;

//
// INTERFACE NOTES: Could we make setIndices() for an arithmetic
// domain take a domain rather than something else?
//
def BlockCyclicDom.setIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
}

def BlockCyclicDom.setIndices(x) {
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

def BlockCyclicDom.getIndices() {
  return whole.getIndices();
}

def BlockCyclicDom.getDist(): BlockCyclic(idxType) {
  return dist;
}

def BlockCyclicDom.slice(param stridable: bool, ranges) {
  var d = new BlockCyclicDom(rank=rank, idxType=idxType, dist=dist, stridable=stridable||this.stridable);
  d.setIndices(whole((...ranges)).getIndices());
  return d;
}

def BlockCyclicDom.setup() {
  coforall localeIdx in dist.targetLocDom do
    on dist.targetLocs(localeIdx) do
      if (locDoms(localeIdx) == nil) then
        locDoms(localeIdx) = new LocBlockCyclicDom(rank, idxType, stridable, this, 
                                                   dist.getStarts(whole, localeIdx));
      else
        locDoms(localeIdx).myStarts = dist.getStarts(whole, localeIdx);
  if debugBlockCyclicDist then
    enumerateBlocks();
}

def BlockCyclicDom.enumerateBlocks() {
  for locidx in dist.targetLocDom {
    on dist.targetLocs(locidx) do locDoms(locidx).enumerateBlocks();
  }
}

def BlockCyclicDom.supportsPrivatization() param return true;

def BlockCyclicDom.privatize() {
  var privateDist = new BlockCyclic(rank, idxType, dist);
  var c = new BlockCyclicDom(rank=rank, idxType=idxType, stridable=stridable, dist=privateDist);
  c.locDoms = locDoms;
  c.whole = whole;
  return c;
}

def BlockCyclicDom.reprivatize(other) {
  locDoms = other.locDoms;
  whole = other.whole;
}

def BlockCyclicDom.member(i) {
  return whole.member(i);
}

def BlockCyclicDom.order(i) {
  return whole.order(i);
}

def BlockCyclicDom.position(i) {
  return whole.position(i);
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Local Domain Class
//
class LocBlockCyclicDom {
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // UP LINK: a reference to the parent global domain class
  //
  const wholeDom: BlockCyclicDom(rank, idxType, stridable);

  //
  // a local domain describing the indices owned by this locale
  //
  // NOTE: I used to use a local index type for this, but that would
  // require a glbIdxType offset in order to get from the global
  // indices back to the local index type.
  //
  var myStarts: domain(rank, idxType, stridable=true);
}

//
// output local domain piece
//
def LocBlockCyclicDom.writeThis(x:Writer) {
  x.write(myStarts);
}

def LocBlockCyclicDom.enumerateBlocks() {
  for i in myStarts {
    write(here.id, ": [");
    for param j in 1..rank {
      if (j != 1) {
        write(", ");
      }
      // TODO: support a tuple-oriented iteration of vectors to avoid this?
      var lo: idxType;
      if rank == 1 then
        lo = i;
      else
        lo = i(j);
      write(lo, "..", min(lo + wholeDom.dist.blocksize(j)-1, 
                          wholeDom.whole.dim(j).high));
    }
    writeln("]");
  } 
}
  

//
// queries for this locale's number of indices, low, and high bounds
//
// TODO: I believe these are only used by the random number generator
// in stream -- will they always be required once that is rewritten?
//
def LocBlockCyclicDom.numIndices {
  return myStarts.numIndices;
}

def LocBlockCyclicDom.low {
  return myStarts.low;
}

def LocBlockCyclicDom.high {
  return myStarts.high;
}

/*
////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Array Class
//
class BlockCyclicArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: the global domain descriptor for this array
  //
  var dom: BlockCyclicDom(rank, idxType, stridable);

  //
  // DOWN LINK: an array of local array classes
  //
  var locArr: [dom.dist.targetLocDom] LocBlockCyclicArr(eltType, rank, idxType, stridable);

  //
  // optimized reference to a local LocBlockCyclicArr instance (or nil)
  //
  var myLocArr: LocBlockCyclicArr(eltType, rank, idxType, stridable);

  var pid: int = -1; // privatized object id
}

def BlockCyclicArr.getBaseDom() return dom;

def BlockCyclicArr.setup() {
  coforall localeIdx in dom.dist.targetLocDom {
    on dom.dist.targetLocs(localeIdx) {
      locArr(localeIdx) = new LocBlockCyclicArr(eltType, rank, idxType, stridable, dom.locDoms(localeIdx));
      if this.locale == here then
        myLocArr = locArr(localeIdx);
    }
  }
}

def BlockCyclicArr.supportsPrivatization() param return true;

def BlockCyclicArr.privatize() {
  var dompid = dom.pid;
  var thisdom = dom;
  var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
  var c = new BlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
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
def BlockCyclicArr.this(i: idxType) var where rank == 1 {
  if myLocArr then local {
    if myLocArr.locDom.myStarts.member(i) then
      return myLocArr.this(i);
  }
  return locArr(dom.dist.ind2locInd(i))(i);
}

def BlockCyclicArr.this(i: rank*idxType) var {
//   const myLocArr = locArr(here.id);
//   local {
//     if myLocArr.locDom.myStarts.member(i) then
//       return myLocArr.this(i);
//   }
  if rank == 1 {
    return this(i(1));
  } else {
    return locArr(dom.dist.ind2locInd(i))(i);
  }
}

def BlockCyclicArr.these() var {
  for i in dom do
    yield this(i);
}

//
// TODO: Rewrite this to reuse more of the global domain iterator
// logic?  (e.g., can we forward the forall to the global domain
// somehow?
//
def BlockCyclicArr.these(param tag: iterator) where tag == iterator.leader {
  const precomputedNumTasks = dom.dist.tasksPerLocale;
  const precomputedWholeLow = dom.whole.low;
  coforall locDom in dom.locDoms do on locDom {
    var tmpBlockCyclic = locDom.myStarts - precomputedWholeLow;
    const numTasks = precomputedNumTasks;
    var locBlockCyclic: rank*range(idxType);
    for param i in 1..tmpBlockCyclic.rank {
      locBlockCyclic(i) = (tmpBlockCyclic.dim(i).low/tmpBlockCyclic.dim(i).stride:idxType)..#(tmpBlockCyclic.dim(i).length);
    }


    if (numTasks == 1) {
      yield locBlockCyclic;
    } else {
      coforall taskid in 0..#numTasks {
        var tuple: rank*range(idxType) = locBlockCyclic;
        const (lo,hi) = _computeBlockCyclic(locBlockCyclic(1).low, locBlockCyclic(1).length,
                                      locBlockCyclic(1).low, locBlockCyclic(1).high,
                                      numTasks, taskid);
          
        tuple(1) = lo..hi;
        yield tuple;

      }
    }
  }
}

def BlockCyclicArr.supportsAlignedFollower() param return true;

def BlockCyclicArr.these(param tag: iterator, follower, param aligned: bool = false) var where tag == iterator.follower {
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
  const myLocArr = locArr(dom.dist.ind2locInd(lowIdx));
  if aligned {
    local {
      for i in followThisDom {
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
//          if myLocArr.locDom.myStarts.member(i) then
//            return myLocArr.this(i);
//        }
//      }
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
def BlockCyclicArr.writeThis(f: Writer) {
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

def BlockCyclicArr.numElements return dom.numIndices;

def BlockCyclicArr.checkSlice(ranges) {
  for param i in 1..rank do
    if !dom.dim(i).boundsCheck(ranges(i)) then {
      writeln(dom.dim(i), " ", ranges(i), " ", dom.dim(i).boundsCheck(ranges(i)));
      halt("array slice out of bounds in dimension ", i, ": ", ranges(i));
    }
}


def BlockCyclicArr.slice(d: BlockCyclicDom) {
  var alias = new BlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d, pid=pid);
  for i in dom.dist.targetLocDom {
    on dom.dist.targetLocs(i) {
      alias.locArr[i] = new LocBlockCyclicArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, locDom=d.locDoms[i]);
      alias.locArr[i].myElems => locArr[i].myElems[alias.locArr[i].locDom.myStarts];
    }
  }

  return alias;
}

////////////////////////////////////////////////////////////////////////////////
// BlockCyclic Local Array Class
//
class LocBlockCyclicArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;

  //
  // LEFT LINK: a reference to the local domain class for this array and locale
  //
  const locDom: LocBlockCyclicDom(rank, idxType, stridable);


  // STATE:

  //
  // the block of local array data
  //
  var myElems: [locDom.myStarts] eltType;
}

//
// the accessor for the local array -- assumes the index is local
//
def LocBlockCyclicArr.this(i) var {
  return myElems(i);
}

//
// output local array piece
//
def LocBlockCyclicArr.writeThis(x: Writer) {
  // note on this fails; see writeThisUsingOn.chpl
  x.write(myElems);
}

def LocBlockCyclicArr.numElements return myElems.numElements;


//
// helper function for blocking index ranges
//
def _computeBlockCyclic(waylo, numelems, lo, wayhi, numblocks, blocknum) {
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
*/


// BLC: Common code, factor out

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
