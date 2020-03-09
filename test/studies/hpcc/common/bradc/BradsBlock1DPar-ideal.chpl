// TODO: Have the global class leader/follower iterators defer to the
// local class leader/followers once we're within a locale?

// TODO: Make these into an official distribution?

// TODO: Would using nested classes allow me to avoid so much
// passing around of globIndexType and locIndexType?

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
  type glbIdxType = int(64);

  //
  // The bounding box that defines the block distribution
  //
  const boundingBox: domain(1, glbIdxType);


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
  const locDist: [loc in targetLocDom] LocBlock1DDist(glbIdxType)
                   = on targetLocs(loc) do 
                       new LocBlock1DDist(glbIdxType, targetLocDom.order(loc), this);
  */
  //
  // but this doesn't work yet because an array forall initializer
  // apparently can't refer to a local member domain.
  //
  const locDist: [targetLocDom] LocBlock1DDist(glbIdxType);
  //
  // WORKAROUND: Initialize in the constructor instead
  //

  proc Block1DDist(type glbIdxType = int(64), bbox: domain(1, glbIdxType),
                  targetLocales: [?targetLocalesDomain] locale) {
    boundingBox = bbox;
    targetLocDom = targetLocalesDomain;
    targetLocs = targetLocales;
    //
    // WANT TO DO:
    /*
    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlock1DDist(glbIdxType, locid, this);
    */
    //
    // BUT results in a _heapAlloc(type int(64)) call.
    // WORKAROUND:
    helpConstruct();
  }
  //
  // WORKAROUND CONTINUED:
  //
  proc helpConstruct() {
    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlock1DDist(glbIdxType, locid, this);
  }
  //
  // END WORKAROUND
  //


  proc writeThis(x) throws {
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
  // set (inds) and the given global and local index type (idxType,
  // locIdxType)
  //
  proc newDomain(inds, type idxType = glbIdxType, type locIdxType = idxType) {
    // Note that I'm fixing the global and local index types to be the
    // same, but making this a generic function would fix this
    return new Block1DDom(idxType, locIdxType, this, inds);
  }

  //
  // compute what chunk of inds is owned by a given locale -- assumes
  // it's being called on the locale in question
  //
  proc getChunk(inds, locid) {
    // use domain slicing to get the intersection between what the
    // locale owns and the domain's index set

    // TODO: Should this be able to be written as myChunk[inds] ???
    return locDist(locid).myChunk(inds.dim(1));
  }
  
  //
  // Determine which locale owns a particular index
  //
  proc idxToLocaleInd(ind: glbIdxType) {
    const ind0 = ind - boundingBox.low;
    const loc0 = (ind0 * targetLocs.numElements) / boundingBox.numIndices;
    const locInd = loc0: index(targetLocs.domain) + targetLocs.domain.low;
    return locInd;
  }

  proc idxToLocale(ind: glbIdxType) {
    return targetLocs(idxToLocaleInd(ind));
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
  const locid: int;
  const loc: locale;

  //
  // Compute what chunk of index(1) is owned by the current locale
  // Arguments:
  //
  proc LocBlock1DDist(type glbIdxType, 
                     _locid: int, // the locale index from the target domain
                     dist: Block1DDist(glbIdxType) // reference to glob dist
                     ) {
    locid = _locid;
    loc = dist.targetLocs(locid);
    const lo = dist.boundingBox.low;
    const hi = dist.boundingBox.high;
    const numelems = hi - lo + 1;
    const numlocs = dist.targetLocDom.numIndices;
    const locid0 = dist.targetLocDom.order(locid); // 0-based locale ID
    const blo = if (locid == 0) then min(glbIdxType)
                else procToData((numelems: real * locid) / numlocs, lo);
    const bhi = if (locid == numlocs - 1) then max(glbIdxType)
                else procToData((numelems: real * (locid+1)) / numlocs, lo) - 1;
    myChunk = {blo..bhi};
    if debugBradsBlock1D then
      writeln("locale ", locid, " owns ", myChunk);
  }

  //
  // a helper function for mapping processors to indices
  //
  proc procToData(x, lo)
    return (lo + (x: lo.type) + (x:real != x:int:real));

  proc writeThis(x) throws {
    x.write("locale ", loc.id, " owns chunk: ", myChunk);
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
  // an array of local domain class descriptors -- set up in
  // postinit() below
  //
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the postinit method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  var locDoms: [dist.targetLocDom] LocBlock1DDom(glbIdxType, lclIdxType);

  proc postinit() {
    for locid in dist.targetLocDom do
      on dist.targetLocs(locid) do
        locDoms(locid) = new LocBlock1DDom(glbIdxType, lclIdxType, this, 
                                           dist.getChunk(whole, locid));
    if debugBradsBlock1D then
      [loc in dist.targetLocDom] writeln(loc, " owns ", locDoms(loc));
  }

  //
  // the iterator for the domain -- currently sequential
  //
  iter these() {
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
  iter newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
    //
    // TODO: And really want to split the inter- and intra-locale
    // parallelism into two separate stages for communication
    // optimization and the like.  Not sure that even this current
    // approach that uses both global and local iterators achieves
    // this.
    //
    // TODO: Abstract this subtraction of low into a function?
    // Note relationship between this operation and the
    // order/position functions -- any chance for creating similar
    // support? (esp. given how frequent this seems likely to be?)
    //
    coforall locDom in locDoms do
      on locDom do
        forall i in locDom do
          yield i;
  }


  iter newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
    //
    // TODO: Abstract this addition of low into a function?
    // Note relationship between this operation and the
    // order/position functions -- any chance for creating similar
    // support? (esp. given how frequent this seems likely to be?)
    //
    // TODO: As noted above, I still have concerns about whether
    // this strategy will help do things like efficient communication
    // on a locale- rather than core basis.
    //
    for i in followThis {
      yield i + whole.low;
    }
  }

  //
  // the print method for the domain
  //
  proc writeThis(x) throws {
    x.write(whole);
  }

  //
  // how to allocate a new array over this domain
  //
  proc newArray(type elemType) {
    return new Block1DArr(glbIdxType, lclIdxType, elemType, this);
  }

  //
  // queries for the number of indices, low, and high bounds
  //
  proc numIndices {
    return whole.numIndices;
  }

  proc low {
    return whole.low;
  }

  proc high {
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
  iter these() {
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
  iter newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
    const numChunks = here.maxTaskPar();
    coforall (core, blk) in (numChunks, BlockSchedule(myBlock, numChunks)) do
      yield blk - wholeDom.whole.low;
  }

  iter newThese(param iterator: IteratorType, followThis0)
        where iterator == IteratorType.follower {
    const followThis = followThis0 + wholeDom.whole.high;
    forall i in followThis do
      yield i;
  }


  //
  // how to write out this locale's indices
  //
  proc writeThis(x) throws {
    x.write(myBlock);
  }

  //
  // queries for this locale's number of indices, low, and high bounds
  //
  proc numIndices {
    return myBlock.numIndices;
  }

  proc low {
    return myBlock.low;
  }

  proc high {
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
  // an array of local array classes
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the postinit method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  var locArr: [dom.dist.targetLocDom] LocBlock1DArr(glbIdxType, lclIdxType, elemType);

  proc postinit() {
    for locid in dom.dist.targetLocDom do
      on dom.dist.targetLocs(locid) do
        locArr(locid) = new LocBlock1DArr(glbIdxType, lclIdxType, elemType, dom.locDoms(locid));
  }

  //
  // the global accessor for the array
  //
  proc this(i: glbIdxType) ref {
    return locArr(dom.dist.idxToLocaleInd(i))(i);
  }

  //
  // the iterator over the array's elements, currently sequential
  //
  iter these() ref {
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

  iter newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
    coforall loc in locArr do
      on locArr do
        forall i in locArr.locDom do
          yield i;
  }

  proc newThese(param iterator: IteratorType, followThis) ref
        where iterator == IteratorType.follower {

    // TODO: Determine which locale we're running on, find the
    // appropriate local array descriptor, and defer this to its
    // follower iterator
  }

  //
  // how to print out the whole array, sequentially
  //
  proc writeThis(x) throws {
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
  proc numElements {
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
  proc this(i: lclIdxType) ref {
    return myElems(i);
  }

  //
  // iterator over the elements owned by this locale
  //
  iter these() ref {
    for elem in myElems {
      yield elem;
    }
  }

  //
  // this is the parallel iterator for the local array, see global
  // domain parallel iterators for general notes on the approach
  //
  proc newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
    // TODO: Break into per-core chunks as we did in the local domain
    // class
  }

  proc newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
    //
    // TODO: I view this one as having at least two cases: (1) the
    // followThis block is owned completely locally (an aligned
    // zippered iteration) -- great!  Let's blast through it quickly.
    // (2) we don't own some or all of the elements in question.  Need
    // to bring them over locally, either all at once, an element at a
    // time, or some clever midpoint that pipelines communication and
    // computation (using a double buffer e.g.)
    //
  }


  //
  // prints out this locale's piece of the array
  //
  proc writeThis(x) throws {
    // May want to do something like the following:
    //      on loc {
    // but it causes deadlock -- see writeThisUsingOn.chpl
    x.write(myElems);
  }

  //
  // query for the number of local array elements
  //
  proc numElements {
    return myElems.numElements;
  }
}
