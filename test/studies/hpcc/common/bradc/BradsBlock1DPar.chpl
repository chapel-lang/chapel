// TODO: Have the global class leader/follower iterators defer to the
// local class leader/followers once we're within a locale?

// TODO: Make these into an official distribution?

// TODO: Would using nested classes allow me to avoid so much
// passing around of globIndexType and locIndexType?

config param debugBradsBlock1D = false;

// TODO: This would need to be moved somewhere more standard
enum IteratorType { solo, leader, follower };

//
// The distribution class
//
class Block1DDist {
  //
  // The distribution's index type and domain's global index type
  //
  type glbIdxType = int(64);

  //
  // The bounding box that defines the block distribution
  //
  const bbox: domain(1, glbIdxType);

  //
  // the set of target locales to which the indices are mapped
  //
  const targetLocs = Locales; // TODO: would like to assert that this is a 1D 
                              // array of locales and maybe remove the default 
                              // initializer

  //
  // an associative domain over the set of target locales
  //
  // TODO: Try to write this a different way without using
  // associative domains/arrays of locales
  //
  const targetLocDom: domain(locale) = targetLocs;

  //
  // an associative array of local distribution class descriptors --
  // set up in initialize() below
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  // (and also to be able to map from an index back to an order in a
  // domain in order to avoid the need for the zippered iteration?)
  //
  var locDist: [targetLocDom] LocBlock1DDist(glbIdxType);

  def initialize() {
    for (loc, locid) in (targetLocs, 0..) do
      on loc do
        locDist(loc) = new LocBlock1DDist(glbIdxType, locid, this);
  }

  //
  // Create a new domain over this distribution with the given index
  // set (inds) and the given global and local index type (idxType,
  // locIdxType)
  //
  def newDomain(inds, type idxType = glbIdxType, type locIdxType = idxType) {
    // Note that I'm fixing the global and local index types to be the
    // same, but making this a generic function would fix this
    return new Block1DDom(idxType, locIdxType, this, inds);
  }

  //
  // compute what chunk of inds is owned by a given locale -- assumes
  // it's being called on the locale in question
  //
  def getChunk(inds) {
    // use domain slicing to get the intersection between what the
    // locale owns and the domain's index set

    // TODO: Should this be able to be written as myChunk[inds] ???
    return locDist(here).myChunk(inds.dim(1));
  }
  
  //
  // Determine which locale owns a particular index
  //
  def ind2loc(ind: glbIdxType) {
    const indFrom0 = ind - bbox.low;
    const locFrom0 = (indFrom0 * targetLocs.numElements) / bbox.numIndices;
    const locInd = locFrom0: index(targetLocs.domain) + targetLocs.domain.low;
    return targetLocs(locInd);
  }
}

//
// A per-locale local distribution class
//
class LocBlock1DDist {
  // 
  // The distribution's index type and domain's global index type
  //
  type glbIdxType;

  //
  // This stores the piece of the global bounding box owned by
  // the locale.  Note that my original guess that we'd want
  // to use lclIdxType here is wrong since we're talking about
  // the section of the global index space owned by the locale.
  //
  const myChunk: domain(1, glbIdxType);

  //
  // a helper function for mapping processors to indices
  //
  def procToData(x, lo)
    return (lo + (x: lo.type) + (x:real != x:int:real));

  //
  // Compute what chunk of index(1) is owned by the current locale
  // Arguments:
  //
  def LocBlock1DDist(type glbIdxType, 
                     locid: int, // the locale ID as a 0-based integer
                     dist: Block1DDist(glbIdxType) // reference to glob dist
                     ) {
    const lo = dist.bbox.low;
    const hi = dist.bbox.high;
    const numelems = hi - lo + 1;
    const numlocs = dist.targetLocs.numElements;
    const blo = if (locid == 0) then min(glbIdxType)
                else procToData((numelems: real * locid) / numlocs, lo);
    const bhi = if (locid == numlocs - 1) then max(glbIdxType)
                else procToData((numelems: real * (locid+1)) / numlocs, lo) - 1;
    myChunk = [blo..bhi];
    if debugBradsBlock1D then
      writeln("locale ", locid, " owns ", myChunk);
  }
}


//
// The global domain class
//
class Block1DDom {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;
  type lclIdxType;

  //
  // a pointer to the parent distribution
  //
  const dist: Block1DDist(glbIdxType);

  //
  // a domain describing the complete domain
  //
  const whole: domain(1, glbIdxType);

  //
  // an associative array of local domain class descriptors -- set up
  // in initialize() below
  //
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  var locDoms: [dist.targetLocDom] LocBlock1DDom(glbIdxType, lclIdxType);

  def initialize() {
    for loc in dist.targetLocs do
      on loc do
        locDoms(loc) = new LocBlock1DDom(glbIdxType, lclIdxType, this, 
                                        dist.getChunk(whole));
    if debugBradsBlock1D then
      [loc in dist.targetLocs] writeln(loc, " owns ", locDoms(loc));
  }

  //
  // the iterator for the domain -- currently sequential
  //
  def these() {
    for blk in locDoms do
      // May want to do something like:     
      // on blk do
      // But can't currently have yields in on clauses
        for ind in blk do
          yield ind;
  }

  //
  // this is the parallel iterator for the global domain, following
  // a variation on Steve and David's proposals -- I've split the
  // single iterator into multiple iterators, distinguished by where
  // clauses on their parameter values.  This permits each to only
  // take the parameters it cares about; permits the leader to be
  // defined in an inlineable way; and permits the follower for an
  // array iterator to be a var iterator
  //
  // I've named these methods newThese() for the time being
  // to avoid conflicting with the above which is currently targeted
  // by the compiler.  My current assumption is that we would want to
  // overload these() to serve this purpose in the final language
  // definition.
  //
  def newThese(param iterator: IteratorType) 
        where iterator == IteratorType.solo {
    //
    // TODO: Should still have this move around between locales even
    // though it's serial
    //
    for i in these() do
      yield i;
  }

  def newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
    //
    // TODO: This currently only results in a single level of
    // per-locale parallelism -- no per-core parallelism; maybe
    // approach this by deferring to the local leader/follower
    // iterators at some point?
    //
    // TODO: Really want the parallelism across the target locales
    // to be expressed more independently of the distribution by
    // pushing it into the leader iterator
    //
    // TODO: And really want to split the inter- and intra-locale
    // parallelism into two separate stages for communication
    // optimization and the like
    //
    // TODO: Need to get an on clause into here somehow
    //
    // TODO: Abstract this subtraction of low into a function?
    //
    for locDom in locDoms do
      yield locDom.myBlock - whole.low;
  }


  def newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
    //
    // TODO: Abstract this addition of low into a function?
    //
    // TODO: Is there some clever way to invoke the leader/follower
    // iterator on the local blocks in here such that the per-core
    // parallelism is expressed at that level?  Seems like a nice
    // natural composition and might help with my fears about how
    // stencil communication will be done on a per-locale basis.
    for i in followThis {
      yield i + whole.low;
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
  def newArray(type elemType) {
    return new Block1DArr(glbIdxType, lclIdxType, elemType, this);
  }

  //
  // queries for the number of indices, low, and high bounds
  //
  def numIndices {
    return whole.numIndices;
  }

  def low {
    return whole.low;
  }

  def high {
    return whole.high;
  }
}


//
// the local domain class
//
class LocBlock1DDom {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;
  type lclIdxType;

  //
  // a reference to the parent global domain class
  //
  const wholeDom: Block1DDom(glbIdxType, lclIdxType);

  //
  // a local domain describing the indices owned by this locale
  //
  var myBlock: domain(1, lclIdxType);

  //
  // iterator over this locale's indices
  //
  def these() {
    // May want to do something like:     
    // on this do
    // But can't currently have yields in on clauses
    for ind in myBlock do
      yield ind;
  }

  //
  // this is the parallel iterator for the local domain, see global
  // domain parallel iterators for general notes on the approach
  //
  def newThese(param iterator: IteratorType) 
        where iterator == IteratorType.solo {
  }

  def newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
  }

  def newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
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


//
// the global array class
//
class Block1DArr {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;
  type lclIdxType;

  //
  // the array's element type
  //
  type elemType;
  
  //
  // the global domain descriptor for this array
  //
  var dom: Block1DDom(glbIdxType, lclIdxType);

  //
  // an associative array of local array classes, indexed by locale
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  var locArr: [dom.dist.targetLocDom] LocBlock1DArr(glbIdxType, lclIdxType, elemType);

  def initialize() {
    for loc in dom.dist.targetLocs do
      on loc do
        locArr(loc) = new LocBlock1DArr(glbIdxType, lclIdxType, elemType, dom.locDoms(loc));
  }

  //
  // the global accessor for the array
  //
  def this(i: glbIdxType) var {
    return locArr(dom.dist.ind2loc(i))(i);
  }

  //
  // the iterator over the array's elements, currently sequential
  //
  def these() var {
    for loc in dom.dist.targetLocs {
      // May want to do something like:     
      // on this do
      // But can't currently have yields in on clauses
      for elem in locArr(loc) {
        yield elem;
      }
    }
  }

  //
  // this is the parallel iterator for the global array, see th
  // example for general notes on the approach
  //
  def newThese(param iterator: IteratorType) 
        where iterator == IteratorType.solo {
    //
    // TODO: Should still have this move around between locales even
    // though it's serial
    //
    for i in these() do
      yield i;
  }

  def newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
    for blk in dom.newThese(IteratorType.leader) do
      yield blk;
  }

  def newThese(param iterator: IteratorType, followThis) var
        where iterator == IteratorType.follower {
    for i in followThis {
      yield this(i + dom.low);
    }
  }

  //
  // how to print out the whole array, sequentially
  //
  def writeThis(x: Writer) {
    var first = true;
    for loc in dom.dist.targetLocs {
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


//
// the local array class
//
class LocBlock1DArr {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;
  type lclIdxType;

  //
  // the element type
  //
  type elemType;

  //
  // a reference to the local domain class for this array and locale
  //
  const locDom: LocBlock1DDom(glbIdxType, lclIdxType);

  //
  // the block of local array data
  //
  var myElems: [locDom.myBlock] elemType;

  //
  // the accessor for the local array -- assumes the index is local
  //
  def this(i: lclIdxType) var {
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
  def newThese(param iterator: IteratorType) 
        where iterator == IteratorType.solo {
  }

  def newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
  }

  def newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
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
}
