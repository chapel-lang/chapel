// TODO: Would using nested classes allow me to avoid so much
// passing around of globIndexType and locIndexType?

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
  // Create a new domain over this distribution
  //
  def newDomain(inds) {
    // Note that I'm fixing the global and local index types to be the
    // same, but making this a generic function would fix this
    return new Block1DDom(glbIdxType, glbIdxType, this, inds);
  }

  //
  // a helper function for mapping processors to indices
  //
  def procToData(x, lo)
    return (lo + (x: lo.type) + (x:real != x:int:real));

  //
  // compute what chunk of inds is owned by locale #num (0-based)
  //
  def getChunk(inds, num) {
    const lo = bbox.low;
    const hi = bbox.high;
    const numelems = hi - lo + 1;
    const numchunks = targetLocs.numElements;
    const blo = procToData((numelems: real * num) / numchunks, lo);
    const bhi = procToData((numelems: real * (num+1)) / numchunks, lo) - 1;
    return inds[blo..bhi];  // using domain slicing to intersect

    // TODO: In retrospect, I think the above is incorrect -- instead,
    // compute each locale's piece of R**1 once and then slice that
    // with inds?
  }
  
  //
  // Determine which locale owns a particular index
  //
  def ind2loc(ind: glbIdxType) {
    return targetLocs((((ind-bbox.low)*targetLocs.numElements)/bbox.numIndices):index(targetLocs.domain));
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
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method
  //
  var locDom: [dist.targetLocDom] LocBlock1DDom(glbIdxType, lclIdxType);

  def initialize() {
    for (loc, locid) in (dist.targetLocs, 0..) do
      on loc do
        locDom(loc) = new LocBlock1DDom(glbIdxType, lclIdxType, this, dist.getChunk(whole, locid));
    //    [loc in dist.targetLocs] writeln(loc, " owns ", locDom(loc));
  }

  //
  // the iterator for the domain -- currently sequential
  //
  def these() {
    for blk in locDom do
      // May want to do something like:     
      // on blk do
      // But can't currently have yields in on clauses
        for ind in blk do
          yield ind;
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
  // removing the initialize method
  //
  var locArr: [dom.dist.targetLocDom] LocBlock1DArr(glbIdxType, lclIdxType, elemType);

  def initialize() {
    for loc in dom.dist.targetLocs do
      on loc do
        locArr(loc) = new LocBlock1DArr(glbIdxType, lclIdxType, elemType, dom.locDom(loc));
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
