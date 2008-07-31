// TODO: Make this work for a strided domain of locales; for a strided
// domain implemented using this distribution.

// TODO: Have the global class leader/follower iterators defer to the
// local class leader/followers once we're within a locale?

// TODO: Make these into an official distribution?

// TODO: Label which routines are the compiler's interface vs. an
// internal interface.

// TODO: Label the linkage fields.

// TODO: implement the slicing interface?

config param debugBradsBlock1D = false;

// TODO: This would need to be moved somewhere more standard
enum IteratorType { leader, follower };

//
// The distribution class
//
class Block1DDist {
  //
  // The distribution's index type and domain's global index type
  //
  type idxType = int(64);

  //
  // The bounding box that defines the block distribution
  //
  const boundingBox: domain(1, idxType);


  //
  // a domain and array describing the set of target locales to which
  // the indices are mapped
  //
  const targetLocDom: domain(1);
  const targetLocs: [targetLocDom] locale;
  //
  // NOTE: originally I wrote this as:
  //
  //   const targetLocs = Locales;
  //   const targetLocDom = targetLocs.domain;
  //
  // but this had two interesting problems:
  //
  // (1) the capture of the targetLocs array only makes a copy of the
  //     array, not of its domain, so if someone were to resize the
  //     LocaleSpace domain outside of the class, this would inadvertantly
  //     get resized as well.  We really want to make a copy of both the
  //     domain and array (which I now do in the constructor).
  //
  // (2) Initializing targetLocs with Locales prevents us from assigning
  //     anything other than a numLocales-ary array of locales to it
  //     which is stricter than I wanted -- I only wanted to restrict
  //     us to a 1D array of locales.  In our current model, this would
  //     require us to have some sort of [domain(1)] locale declaration
  //     to make it work.  Or we could think about whether a model in
  //     which only the array's static type information is inferred
  //     for a field?


  //
  // an array of local distribution class descriptors -- set up in
  // initialize() below
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  // WANTED:
  //
  /*
  const locDist: [loc in targetLocDom] LocBlock1DDist(idxType)
                   = on targetLocs(loc) do 
                       new LocBlock1DDist(idxType, targetLocDom.order(loc), this);
  */
  //
  // but this doesn't work yet because an array forall initializer
  // apparently can't refer to a local member domain.
  //
  const locDist: [targetLocDom] LocBlock1DDist(idxType);
  //
  // WORKAROUND: Initialize in the constructor instead
  //


  //
  // TODO: 0-base the target locales domain in the constructor rather
  // than whenever it's used for simplicity?
  //
  def Block1DDist(type idxType = int(64), bbox: domain(1, idxType),
                  targetLocales: [?targetLocalesDomain] locale = Locales) {
    boundingBox = bbox;
    targetLocDom = targetLocalesDomain;
    targetLocs = targetLocales;
    //
    // WANT TO DO:
    /*
    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlock1DDist(idxType, locid, this);
    */
    //
    // BUT results in a _heapAlloc(type int(64)) call.
    // WORKAROUND:
    helpConstruct();
  }
  //
  // WORKAROUND CONTINUED:
  //
  def helpConstruct() {
    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlock1DDist(idxType, locid, this);
  }
  //
  // END WORKAROUND
  //


  def writeThis(x:Writer) {
    x.writeln("BradsBlock1DPar");
    x.writeln("---------------");
    x.writeln("distributes: ", boundingBox);
    x.writeln("across locales: ", targetLocs);
    x.writeln("indexed via: ", targetLocDom);
    x.writeln("resulting in: ");
    for locid in targetLocDom do
      x.writeln("  [", locid, "] ", locDist(locid));
  }


  //
  // Create a new domain over this distribution with the given index
  // set (inds) and global index type (idxType, idxType)
  //
  // TODO: What would we do if domIdxType did not match idxType?
  //
  def newDomain(inds, type domIdxType = idxType) where domIdxType == idxType {
    return new Block1DDom(idxType, this, inds);
  }

  //
  // compute what chunk of inds is owned by a given locale -- assumes
  // it's being called on the locale in question
  //
  def getChunk(inds, locid) {
    // use domain slicing to get the intersection between what the
    // locale owns and the domain's index set

    // TODO: Should this be able to be written as myChunk[inds] ???
    return locDist(locid).myChunk(inds.dim(1));
  }
  
  //
  // Determine which locale owns a particular index
  //
  // TODO: If targetLocsDom was 0-based, would this function be easier?
  //
  // TODO: I jotted down a note during the code review asking whether
  // targetLocs.numElements and boundingbox.numIndices should be
  // captured locally, or captured in the default dom/array implementation
  // or inlined.  Not sure what that point was anymore, though.
  //
  def ind2locInd(ind: idxType) {
    const ind0 = ind - boundingBox.low;
    const loc0 = (ind0 * targetLocs.numElements) / boundingBox.numIndices;
    const locInd = loc0: index(targetLocs.domain) + targetLocs.domain.low;
    return locInd;
  }

  def ind2loc(ind: idxType) {
    return targetLocs(ind2locInd(ind));
  }
}

//
// A per-locale local distribution class
//
class LocBlock1DDist {
  // 
  // The distribution's index type and domain's global index type
  //
  type idxType;

  //
  // This stores the piece of the global bounding box owned by
  // the locale.  Note that my original guess that we'd want
  // to use lclIdxType here is wrong since we're talking about
  // the section of the global index space owned by the locale.
  //
  // TODO: Rename locid -- it's not very clear
  //
  const myChunk: domain(1, idxType);
  const locid: int;
  const loc: locale;

  //
  // Compute what chunk of index(1) is owned by the current locale
  // Arguments:
  //
  def LocBlock1DDist(type idxType, 
                     _locid: int, // the locale index from the target domain
                     dist: Block1DDist(idxType) // reference to glob dist
                     ) {
    locid = _locid;
    loc = dist.targetLocs(locid);
    //
    // TODO -- assert that "loc == here" at this point for safety?
    //
    const lo = dist.boundingBox.low;
    const hi = dist.boundingBox.high;
    const numelems = hi - lo + 1;
    const numlocs = dist.targetLocDom.numIndices;
    const locid0 = dist.targetLocDom.order(locid); // 0-based locale ID
    const blo = if (locid0 == 0) then min(idxType)
                else procToData((numelems: real * locid0) / numlocs, lo);
    const bhi = if (locid0 == numlocs - 1) then max(idxType)
                else procToData((numelems: real * (locid0+1)) / numlocs, lo) - 1;
    myChunk = [blo..bhi];
    if debugBradsBlock1D then
      writeln(this);
  }

  //
  // a helper function for mapping processors to indices
  //
  def procToData(x, lo)
    return (lo + (x: lo.type) + (x:real != x:int:real));

  def writeThis(x:Writer) {
    x.write("locale ", loc.id, " owns chunk: ", myChunk);
  }
}


//
// The global domain class
//
class Block1DDom {
  //
  // The index types of the domain
  //
  type idxType;

  //
  // a pointer to the parent distribution
  //
  const dist: Block1DDist(idxType);

  //
  // a domain describing the complete domain
  //
  const whole: domain(1, idxType);

  //
  // an array of local domain class descriptors -- set up in
  // initialize() below
  //
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  var locDoms: [dist.targetLocDom] LocBlock1DDom(idxType);

  def initialize() {
    for locid in dist.targetLocDom do
      on dist.targetLocs(locid) do
        locDoms(locid) = new LocBlock1DDom(idxType, this, 
                                           dist.getChunk(whole, locid));
    if debugBradsBlock1D then
      [loc in dist.targetLocDom] writeln(loc, " owns ", locDoms(loc));
  }

  //
  // the iterator for the domain -- currently sequential
  //
  def these() {
    for blk in locDoms do
      // TODO: Would want to do something like:     
      // on blk do
      // But can't currently have yields in on clauses
        for ind in blk do
          yield ind;
  }

  //
  // this is the parallel iterator for the global domain, following
  // a variation on Steve and David's proposals -- I've split the
  // single iterator into two iterators, distinguished by where
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
  // TODO: Steve's question: how would a leader containing an on
  // clause interact with a loop that used an on clause explicitly
  // within its body?  How would it be done efficiently?
  //
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
    // pushing it into the leader iterator.  Need to get an on clause
    // into here somehow.  That is:
    //
    //   coforall locDom in locDoms do
    //     on locDom do
    //       yield locDom.myBlock - whole.low;
    //
    // TODO: And really want to split the inter- and intra-locale
    // parallelism into two separate stages for communication
    // optimization and the like
    //
    // TODO: Abstract this subtraction of low into a function?
    // Note relationship between this operation and the
    // order/position functions -- any chance for creating similar
    // support? (esp. given how frequent this seems likely to be?)
    //
    for locDom in locDoms do
      yield locDom.myBlock - whole.low;
  }


  def newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
    //
    // TODO: Abstract this addition of low into a function?
    // Note relationship between this operation and the
    // order/position functions -- any chance for creating similar
    // support? (esp. given how frequent this seems likely to be?)
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
    return new Block1DArr(idxType, elemType, this);
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
  // The index type of the domain
  //
  type idxType;

  //
  // a reference to the parent global domain class
  //
  const wholeDom: Block1DDom(idxType);

  //
  // a local domain describing the indices owned by this locale
  //
  //
  // TODO: I used to use a local index type for this, but that would
  // require a glbIdxType offset in order to get from the global
  // indices back to the local index type.
  //
  // TODO: Steve would like to see the strided parameter pushed into
  // the global and local domain classes.
  //
  var myBlock: domain(1, idxType);

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
  // TODO: Are these used internally only?
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
  // The index type of the domain
  //
  type idxType;

  //
  // the array's element type
  //
  type elemType;
  
  //
  // the global domain descriptor for this array
  //
  var dom: Block1DDom(idxType);

  //
  // an array of local array classes
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  var locArr: [dom.dist.targetLocDom] LocBlock1DArr(idxType, elemType);

  def initialize() {
    for locid in dom.dist.targetLocDom do
      on dom.dist.targetLocs(locid) do
        locArr(locid) = new LocBlock1DArr(idxType, elemType, dom.locDoms(locid));
  }

  //
  // the global accessor for the array
  //
  // TODO: Do we need a global bounds check here or in ind2locind?
  //
  def this(i: idxType) var {
    return locArr(dom.dist.ind2locInd(i))(i);
  }

  //
  // the iterator over the array's elements, currently sequential
  //
  def these() var {
    for loc in dom.dist.targetLocDom {
      // TODO: May want to do something like:     
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


//
// the local array class
//
class LocBlock1DArr {
  //
  // The index type of the domain
  //
  type idxType;

  //
  // the element type
  //
  type elemType;

  //
  // a reference to the local domain class for this array and locale
  //
  const locDom: LocBlock1DDom(idxType);

  //
  // the block of local array data
  //
  var myElems: [locDom.myBlock] elemType;

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
  def newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
  }

  def newThese(param iterator: IteratorType, followThis) var
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
