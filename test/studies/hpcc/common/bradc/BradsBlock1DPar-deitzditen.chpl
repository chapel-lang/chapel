// TODO: Make these into an official distribution?

// TODO: Fold in leader/follower iterator ideas

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
  // set up in postinit() below
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the postinit method
  //
  // TODO: Remove second generic parameter -- seems confusing and wrong.  Replace
  // with explicit typing of locid field in LocBlock1DDist class.  Particularly
  // since I'm passing in a value from 0.. rather than an actual index value.
  //
  var locDist: [targetLocDom] LocBlock1DDist(glbIdxType, index(targetLocs.domain));

  proc postinit() {
    for (loc, locid) in (targetLocs, 0..) do
      on loc do
        locDist(loc) = new LocBlock1DDist(glbIdxType, locid, this);
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
  proc getChunk(inds) {
    // use domain slicing to get the intersection between what the
    // locale owns and the domain's index set

    // TODO: Could this be written myChunk[inds] ???
    return locDist(here).myChunk[inds.low..inds.high];
  }
  
  //
  // Determine which locale owns a particular index
  //
  // TODO: Is this correct if targetLocs doesn't start with 0?
  //
  proc idxToLocale(ind: glbIdxType) {
    return targetLocs((((ind-bbox.low)*targetLocs.size)/bbox.size):index(targetLocs.domain));
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
  // The following members store (a) the locale ID as a 0-based
  // integer; (b) a reference to the global distribution
  //
  // TODO: don't really want to store these here; should be able to
  // make the things it wants access to arguments to a user-defined
  // constructor instead (but we don't support those for generic
  // classes well yet).
  //
  const locid;
  const dist: Block1DDist(glbIdxType);

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
  proc procToData(x, lo)
    return (lo + (x: lo.type) + (x:real != x:int:real));

  //
  // Compute what chunk of index(1) is owned by the current locale
  //
  proc postinit() {
    const lo = dist.bbox.low;
    const hi = dist.bbox.high;
    const numelems = hi - lo + 1;
    const numlocs = dist.targetLocs.size;
    const blo = if (locid == 0) then min(glbIdxType)
                else procToData((numelems: real * locid) / numlocs, lo);
    const bhi = if (locid == numlocs - 1) then max(glbIdxType)
                else procToData((numelems: real * (locid+1)) / numlocs, lo) - 1;
    myChunk = {blo..bhi};
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
  // in postinit() below
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the postinit method
  //
  var locDom: [dist.targetLocDom] LocBlock1DDom(glbIdxType, lclIdxType);

  proc postinit() {
    for loc in dist.targetLocs do
      on loc do
        locDom(loc) = new LocBlock1DDom(glbIdxType, lclIdxType, this, dist.getChunk(whole));
    if debugBradsBlock1D then
      [loc in dist.targetLocs] writeln(loc, " owns ", locDom(loc));
  }

  //
  // the iterator for the domain -- currently sequential
  //
  iter these() {
    for blk in locDom do
      // May want to do something like:     
      // on blk do
      // But can't currently have yields in on clauses
        for ind in blk do
          yield ind;
  }

  //
  // this is the parallel iterator for the global domain, following
  // Steve and David's proposals -- I've renamed it for the time being
  // to avoid conflicting with the above which is currently targeted
  // by the compiler.  My current assumption is that we would want to
  // overload these() to serve this purpose in the final language
  // definition.
  //
  iter newThese(param iterator: IteratorType, followThis) {
    select iterator {

    when IteratorType.solo {
      for i in these() do
        yield i;
    }

    when IteratorType.leader {
      //
      // TODO: This currently only results in a single level of
      // per-locale parallelism -- no per-core parallelism
      //
      // TODO: Really want the parallelism across the target locales
      // to be expressed more independently of the distribution
      //
      // TODO: And really want to split the inter- and intra-locale
      // parallelism into two separate stages for communication
      // optimization and the like
      //
      // TODO: Need to get an on clause into here somehow
      //
      // TODO: Abstract this subtraction of low into a function?
      //
      for blk in locDom.myBlock do
        yield blk - whole.low;
    }
    
    when IteratorType.follower {
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
    return whole.size;
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
  // this is the parallel iterator for the local domain, following
  // Steve and David's proposals -- I've renamed it for the time being
  // to avoid conflicting with the above which is currently targeted
  // by the compiler.
  //
  iter newThese(param iterator: IteratorType, followThis) {
    select iterator {
    when IteratorType.solo {
      for i in these() do
        yield i;
    }
    when IteratorType.leader {
    }
    when IteratorType.follower {
    }
    }
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
    return myBlock.size;
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
  // an associative array of local array classes, indexed by locale
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the postinit method
  //
  var locArr: [dom.dist.targetLocDom] LocBlock1DArr(glbIdxType, lclIdxType, elemType);

  proc postinit() {
    for loc in dom.dist.targetLocs do
      on loc do
        locArr(loc) = new LocBlock1DArr(glbIdxType, lclIdxType, elemType, dom.locDom(loc));
  }

  //
  // the global accessor for the array
  //
  proc this(i: glbIdxType) ref {
    return locArr(dom.dist.idxToLocale(i))(i);
  }

  //
  // the iterator over the array's elements, currently sequential
  //
  iter these() ref {
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
  // this is the parallel iterator for the global array, following
  // Steve and David's proposals -- I've renamed it for the time being
  // to avoid conflicting with the above which is currently targeted
  // by the compiler.
  //
  // TODO: Note that this should logically be a var iterator, but
  // that doesn't seem to work.
  //
  iter newThese(param iterator: IteratorType, followThis) {
    select iterator {
    when IteratorType.solo {
      for i in these() do
        yield i;
    }
    //
    // TODO: Note that without some more work/cleverness, I'm expressing
    // iteration over arrays in terms of the domain.  Seems suboptimal.
    //
    when IteratorType.leader {
      for blk in dom.newThese(iterator, followThis) do
        yield blk;
    }
    //
    // TODO: Note that here I'm not only going through the domain indices,
    // but I'm also going through the global array's accessor.  We really
    // want to be only working in terms of the local array in the follower.
    //
    when IteratorType.follower {
      for i in followThis do 
        yield this(i + dom.low);
    }
    }
  }

  //
  // how to print out the whole array, sequentially
  //
  proc writeThis(x) throws {
    var first = true;
    for loc in dom.dist.targetLocs {
      // May want to do something like the following:
      //      on loc {
      // but it causes deadlock -- see writeThisUsingOn.chpl
        if (locArr(loc).size >= 1) {
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
    return dom.size;
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
  // this is the parallel iterator for the local array, following
  // Steve and David's proposals -- I've renamed it for the time being
  // to avoid conflicting with the above which is currently targeted
  // by the compiler.
  //
  iter newThese(param iterator: IteratorType, followThis) ref {
    select iterator {
    when IteratorType.solo {
      for i in these() do
        yield i;
    }
    when IteratorType.leader {
    }
    when IteratorType.follower {
    }
    }
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
    return myElems.size;
  }
}
