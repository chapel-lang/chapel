//
// Block1D Distribution
// 
//      Block1D       Block1DDom     Block1DArr
//
//   LocBlock1DDist  LocBlock1DDom  LocBlock1DArr
//

// TODO: Make multidimensional

// TODO: Make this work for a strided domain of locales; for a strided
// domain implemented using this distribution.

// TODO: implement the slicing interface?

config param debugBlock1D = false;


class Block1D : BaseDist {
  type idxType = int(64); // distribution's index type, for domain

  const boundingBox: domain(1, idxType);

  //
  // a domain and array describing the set of target locales to which
  // the indices are mapped
  //
  const targetLocDom: domain(1);
  const targetLocs: [targetLocDom] locale;

  //
  // sjd: note this is a variable but it shouldn't be changed after
  // privatization, because it won't be reflected everywhere.  should
  // this be a constant?  how should we handle such?
  //
  var tasksPerLoc: int;

  //
  // DOWN LINK: an array of local distribution class descriptors --
  // set up in the class constructor
  //
  const locDist: [targetLocDom] LocBlock1DDist(idxType);

  def Block1D(type idxType = int(64), bbox: domain(1, idxType),
              targetLocales: [] locale = thisRealm.Locales, 
	      tasksPerLocale = 0) {
    boundingBox = bbox;
    targetLocDom = [0..#targetLocales.numElements]; // 0-based for simplicity
    targetLocs = targetLocales;

    coforall locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlock1DDist(idxType, locid, this);

    tasksPerLoc = tasksPerLocale;
    if (tasksPerLoc == 0) then tasksPerLoc = min reduce targetLocs.numCores;

    if debugBlock1D then
      for loc in locDist do writeln(loc);
  }

  //
  // builds up a privatized (replicated copy)
  //
  def Block1D(type idxType = int(64), other: Block1D(idxType)) {
    boundingBox = other.boundingBox;
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    tasksPerLoc = other.tasksPerLoc;
    locDist = other.locDist;
  }

  //
  // Create a new domain over this distribution with the given index
  // set (inds) and global index type (idxType, idxType)
  //
  // INTERFACE NOTES: Should we support a form that passes in an
  // initial index set if one exists?  If not, we should rewrite the
  // global domain construction to not do anything with whole...
  //
  def newArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    if idxType != this.idxType then
      compilerError("Block1D domain index type does not match distribution's");
    if rank != 1 then
      compilerError("Block1D domains are restricted to 1D (rank 1)");

    var dom = new Block1DDom(idxType=idxType, dist=this, stridable=stridable);
    dom.setup();
    return dom;
  }

  //
  // print out the distribution
  //
  def writeThis(x:Writer) {
    x.writeln("Block1D");
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
  def ind2loc(ind: idxType) {
    return targetLocs(ind2locInd(ind));
  }

  //
  // compute what chunk of inds is owned by a given locale -- assumes
  // it's being called on the locale in question
  //
  def getChunk(inds, locid) {
    // use domain slicing to get the intersection between what the
    // locale owns and the domain's index set
    //
    // TODO: Should this be able to be written as myChunk[inds] ???
    //
    // TODO: Does using David's detupling trick work here?
    //
    return locDist(locid).myChunk(inds.dim(1));
  }
  
  //
  // Determine which locale owns a particular index
  //
  // TODO: I jotted down a note during the code review asking whether
  // targetLocs.numElements and boundingbox.numIndices should be
  // captured locally, or captured in the default dom/array implementation
  // or inlined.  Not sure what that point was anymore, though.  Maybe
  // someone else can help me remember it (since it was probably someone
  // else's suggestion).
  //
  def ind2locInd(ind: idxType) {
    //    writeln("distribution = ", this);
    const ind0 = ind - boundingBox.low;
    const locInd = (ind0 * targetLocs.numElements:idxType) / boundingBox.numIndices;
    return max(0, min(locInd:int, (targetLocDom.dim(1).length-1):int));
  }
}


class LocBlock1DDist {
  type idxType;

  //
  // This stores the piece of the global bounding box owned by
  // the locale.  Note that my original guess that we'd want
  // to use lclIdxType here is wrong since we're talking about
  // the section of the global index space owned by the locale.
  //
  const myChunk: domain(1, idxType);

  //
  // The locale owning this class
  //
  // TODO: This is only used in the writeThis() class -- can we remove it?
  //
  const loc: locale;

  //
  // Constructor computes what chunk of index(1) is owned by the
  // current locale
  //
  def LocBlock1DDist(type idxType, 
                     _localeIdx: int, // the locale index from the target domain
                     dist: Block1D(idxType) // reference to glob dist
                     ) {
    const localeIdx = _localeIdx;
    loc = dist.targetLocs(localeIdx);
    //
    // TODO: Create these assertions for other local classes as well
    //
    if (loc != here) then
      halt("Creating a local distribution class on the wrong locale");

    const lo = dist.boundingBox.low;
    const hi = dist.boundingBox.high;
    const numelems = hi - lo + 1;
    const numlocs = dist.targetLocDom.numIndices;
    const (blo, bhi) = _computeBlock(min(idxType), numelems, lo,
                                     max(idxType), numlocs, localeIdx);
    
    myChunk = [blo..bhi];
  }

  //
  // print out the local distribution class
  //
  def writeThis(x:Writer) {
    x.write("locale ", loc.id, " owns chunk: ", myChunk);
  }
}


class Block1DDom: BaseArithmeticDom {
  type idxType;
  param rank = 1;
  param stridable: bool;

  //
  // LEFT LINK: a pointer to the parent distribution
  //
  const dist: Block1D(idxType);

  //
  // DOWN LINK: an array of local domain class descriptors -- set up in
  // setup() below
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  // SJD: note cannot do this anymore because constructor does not
  // setup (for privatization reasons)
  //
  var locDoms: [dist.targetLocDom] LocBlock1DDom(idxType, stridable);

  //
  // a domain describing the complete domain
  //
  const whole: domain(1, idxType);

  var pid: int = -1; // privatized object id

  //
  // TODO: This really should go over the elements in row-major order,
  // not the block orders.
  //
  def these() {
    for blk in locDoms do
      for ind in blk do
        yield ind;
  }

  //
  // TODO: Steve's question: how would a leader containing an on
  // clause interact with a loop that used an on clause explicitly
  // within its body?  How would it be done efficiently?
  //
  // TODO: Abstract this subtraction of low into a function?
  // Note relationship between this operation and the
  // order/position functions -- any chance for creating similar
  // support? (esp. given how frequent this seems likely to be?)
  //
  def these(param tag: iterator) where tag == iterator.leader {
    const precomputedNumTasks = dist.tasksPerLoc;
    coforall locDom in locDoms do on locDom {
      const locBlock = locDom.myBlock - whole.low,
            numTasks = precomputedNumTasks;
      if (numTasks == 1) {
        yield locBlock;
      } else {
        coforall taskid in 0..#numTasks {
          const (lo,hi) = _computeBlock(locBlock.low, locBlock.numIndices,
                                        locBlock.low, locBlock.high, 
                                        numTasks, taskid);
          yield [lo..hi];
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
  def these(param tag: iterator, follower) where tag == iterator.follower {
    const followThis = follower + whole.low;

    for i in followThis {
      yield i;
    }
  }

  //
  // the print method for the domain
  //
  def writeThis(x:Writer) {
    x.write(whole);
  }

  //
  // how to allocate a new array over this domain
  //
  def buildArray(type elemType) {
    var arr = new Block1DArr(idxType, elemType, stridable, this);
    arr.setup();
    return arr;
  }

  //
  // queries for the number of indices, low, and high bounds
  //
  def numIndices return whole.numIndices;
  def low return whole.low;
  def high return whole.high;

  //
  // INTERFACE NOTES: Could we make setIndices() for an arithmetic
  // domain take a domain rather than something else?
  //
  def setIndices(x: domain) {
    if x.rank != 1 then
      compilerError("rank mismatch in domain assignment");
    if x._value.idxType != idxType then
      compilerError("index type mismatch in domain assignment");
    whole = x;
    setup();
  }

  def setIndices(x) {
    if x.size != 1 then
      compilerError("rank mismatch in domain assignment");
    if x(1).eltType != idxType then
      compilerError("index type mismatch in domain assignment");
    //
    // TODO: This seems weird:
    //
    whole = x(1);
    setup();
  }

  def getIndices() {
    return whole;
  }

  def getDist(): Block1D(idxType) {
    return dist;
  }

  def setup() {
    coforall localeIdx in dist.targetLocDom do
      on dist.targetLocs(localeIdx) do
        if (locDoms(localeIdx) == nil) then
          locDoms(localeIdx) = new LocBlock1DDom(idxType, stridable, this, 
                                                 dist.getChunk(whole, localeIdx));
        else
          locDoms(localeIdx).myBlock = dist.getChunk(whole, localeIdx);
    if debugBlock1D then
      for loc in dist.targetLocDom do writeln(loc, " owns ", locDoms(loc));

  }

  def supportsPrivatization() param return true;
  def privatize() {
    var privateDist = new Block1D(idxType, dist);
    var c = new Block1DDom(idxType=idxType, rank=rank, stridable=stridable, dist=privateDist);
    c.locDoms = locDoms;
    c.whole = whole;
    return c;
  }
  def reprivatize(other) {
    locDoms = other.locDoms;
    whole = other.whole;
  }
}


class LocBlock1DDom {
  type idxType;
  param stridable: bool;

  //
  // UP LINK: a reference to the parent global domain class
  //
  const wholeDom: Block1DDom(idxType, 1, stridable);

  //
  // a local domain describing the indices owned by this locale
  //
  // NOTE: I used to use a local index type for this, but that would
  // require a glbIdxType offset in order to get from the global
  // indices back to the local index type.
  //
  var myBlock: domain(1, idxType);

  def these() {
    for ind in myBlock do
      yield ind;
  }

  //
  // this is the parallel iterator for the local domain, see global
  // domain parallel iterators for general notes on the approach
  //
  def these(param tag: iterator) where tag == iterator.leader {
    halt("This is bogus");
    yield [1..100];
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    halt("This is bogus");
    yield 2;
  }

  //
  // how to write out this locale's indices
  //
  def writeThis(x:Writer) {
    x.write(myBlock);
  }

  //
  // queries for this locale's number of indices, low, and high bounds
  //
  // TODO: I believe these are only used by the random number generator
  // in stream -- will they always be required once that is rewritten?
  //
  def numIndices {
    return myBlock.numIndices;
  }

  def low {
    return myBlock.low;
  }

  def high {
    return myBlock.high;
  }
}


class Block1DArr: BaseArr {
  type idxType;
  type eltType;
  param stridable: bool;

  //
  // LEFT LINK: the global domain descriptor for this array
  //
  var dom: Block1DDom(idxType, 1, stridable);

  //
  // DOWN LINK: an array of local array classes
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  // SJD: note cannot do this anymore because constructor does not
  // setup (for privatization reasons)
  // SJD update: maybe can do this again with privatize simplification
  // SJD: actually, this can only be const if the Chapel array is const?
  //
  var locArr: [dom.dist.targetLocDom] LocBlock1DArr(idxType, eltType, stridable);

  var pid: int = -1; // privatized object id

  def setup() {
    coforall localeIdx in dom.dist.targetLocDom do
      on dom.dist.targetLocs(localeIdx) do
        locArr(localeIdx) = new LocBlock1DArr(idxType, eltType, stridable, dom.locDoms(localeIdx));
  }

  def supportsPrivatization() param return true;
  def privatize() {
    var dompid = dom.pid;
    var thisdom = dom;
    var privdom = __primitive("chpl_getPrivatizedClass", thisdom, dompid);
    var c = new Block1DArr(idxType, eltType, stridable, privdom);
    c.locArr = locArr;
    return c;
  }

  //
  // the global accessor for the array
  //
  // TODO: Do we need a global bounds check here or in ind2locind?
  //
  def this(i: idxType) var {
    const myLocArr = locArr(here.id);
    local {
      if myLocArr.locDom.myBlock.member(i) then
        return myLocArr.this(i);
    }
    return locArr(dom.dist.ind2locInd(i))(i);
  }

  def these() var {
    for loc in dom.dist.targetLocDom {
      for elem in locArr(loc) {
        yield elem;
      }
    }
  }

  //
  // TODO: Rewrite this to reuse more of the global domain iterator
  // logic?  (e.g., can we forward the forall to the global domain
  // somehow?
  //
  def these(param tag: iterator) where tag == iterator.leader {
    const precomputedNumTasks = dom.dist.tasksPerLoc;
    const precomputedWholeLow = dom.whole.low;
    coforall locDom in dom.locDoms do on locDom {
      const locBlock = locDom.myBlock - precomputedWholeLow,
            numTasks = precomputedNumTasks;
      if (numTasks == 1) {
        yield locBlock;
      } else {
        coforall taskid in 0..#numTasks {
          const (lo,hi) = _computeBlock(locBlock.low, locBlock.numIndices,
                                        locBlock.low, locBlock.high, 
                                        numTasks, taskid);
          yield [lo..hi];
        }
      }
    }
  }

  def supportsAlignedFollower() param return true;

  def these(param tag: iterator, follower, param aligned: bool = false) var where tag == iterator.follower {
    const followThis = follower + dom.low;

    //
    // TODO: The following is a buggy hack that will only work when we're
    // distributing across the entire Locales array.  I still think the
    // locArr/locDoms arrays should be associative over locale values.
    //
    const myLocArr = locArr(here.id);
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
        local {
          if myLocArr.locDom.myBlock.member(i) then
            return myLocArr.this(i);
        }
        return this(i);
      }
      for i in followThis {
        yield accessHelper(i);
      }
    }
  }

  //
  // how to print out the whole array, sequentially
  //
  def writeThis(x: Writer) {

    var first = true;
    for loc in dom.dist.targetLocDom {
      // May want to do something like the following:
      //      on loc {
      // but it causes deadlock -- see writeThisUsingOn.chpl
        if (locArr(loc).numElements >= 1) {
          if (first) {
            first = false;
          } else {
            x.write(" ");
          }
          x.write(locArr(loc));
        }
        //    }
      stdout.flush();
    }
  }

  //
  // a query for the number of elements in the array
  //
  def numElements {
    return dom.numIndices;
  }
}


class LocBlock1DArr {
  type idxType;
  type eltType;
  param stridable: bool;

  //
  // LEFT LINK: a reference to the local domain class for this array and locale
  //
  const locDom: LocBlock1DDom(idxType, stridable);


  // STATE:

  //
  // the block of local array data
  //
  var myElems: [locDom.myBlock] eltType;


  // LOCAL ARRAY INTERFACE:

  //
  // the accessor for the local array -- assumes the index is local
  //
  def this(i: idxType) var {
    return myElems(i);
  }

  //
  // iterator over the elements owned by this locale
  //
  def these() var {
    for elem in myElems {
      yield elem;
    }
  }

  //
  // this is the parallel iterator for the local array, see global
  // domain parallel iterators for general notes on the approach
  //
  def these(param tag: iterator) where tag == iterator.leader {
    halt("This is bogus");
    yield [1..100];
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    yield myElems(0);
  }


  //
  // prints out this locale's piece of the array
  //
  def writeThis(x: Writer) {
    // May want to do something like the following:
    //      on loc {
    // but it causes deadlock -- see writeThisUsingOn.chpl
    x.write(myElems);
  }

  //
  // query for the number of local array elements
  //
  def numElements {
    return myElems.numElements;
  }

  def owns(x) {
    return locDom.myBlock.dim(1).boundsCheck(x.dim(1));
  }
}


//
// helper function for blocking index ranges
//
def _computeBlock(waylo, numelems, lo, wayhi, numblocks, blocknum) {
  def procToData(x, lo)
    return lo + (x: lo.type) + (x:real != x:int:real): lo.type;

  const blo =
    if (blocknum == 0) then waylo
      else procToData((numelems: real * blocknum) / numblocks, lo);
  const bhi =
    if (blocknum == numblocks - 1) then wayhi
      else procToData((numelems: real * (blocknum+1)) / numblocks, lo) - 1;

  return (blo, bhi);
}
