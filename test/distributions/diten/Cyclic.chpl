// TODO: Have the global class leader/follower iterators defer to the

// A cyclic distribution starting from min(glbIdxType) on locale 0.

// TODO: Make these into an official distribution?

// TODO: Would using nested classes allow me to avoid so much
// passing around of globIndexType and locIndexType?

config param debugCyclic1D = false;

// TODO: This would need to be moved somewhere more standard
enum IteratorType { leader, follower };

//
// The distribution class
//
class Cyclic1DDist {
  //
  // The distribution's index type and domain's global index type
  //
  type glbIdxType = int(64);

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
  // initializer below
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initializer; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  // WANTED:
  //
  /*
  const locDist: [loc in targetLocDom] LocCyclic1DDist(glbIdxType)
                   = on targetLocs(loc) do 
                       new LocCyclic1DDist(glbIdxType, targetLocDom.indexOrder(loc), this);
  */
  //
  // but this doesn't work yet because an array forall initializer
  // apparently can't refer to a local member domain.
  // Also, cannot use 'this' when initializing a field of 'this'.
  //
  const locDist: [targetLocDom] unmanaged LocCyclic1DDist(glbIdxType)?;
  //
  // WORKAROUND: Initialize in the constructor instead
  //

  proc init(type glbIdxType = int(64),
            targetLocales: [?targetLocalesDomain] locale) {
    targetLocDom = targetLocalesDomain;
    targetLocs = targetLocales;

    this.complete();

    //
    // WANT TO DO:
    /*
    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocCyclic1DDist(glbIdxType, locid, this);
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
        locDist(locid) = new unmanaged LocCyclic1DDist(glbIdxType, locid, _to_unmanaged(this));
  }

  proc deinit() {
    for locid in targetLocDom do
      on targetLocs(locid) do
        delete locDist(locid);
  }
  //
  // END WORKAROUND
  //


  proc writeThis(x) {
    x.writeln("Cyclic1DPar");
    x.writeln("---------------");
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
  proc newDomain(inds, type idxType = glbIdxType) {
    // Note that I'm fixing the global and local index types to be the
    // same, but making this a generic function would fix this
    return new unmanaged Cyclic1DDom(idxType, _to_unmanaged(this), inds);
  }

  //
  // compute what chunk of inds is owned by a given locale -- assumes
  // it's being called on the locale in question
  //
  proc getChunk(inds, locid) {
    // use domain slicing to get the intersection between what the
    // locale owns and the domain's index set

    // Force this to be strided
    return locDist(locid)!.myChunk(inds.dim(1) by 1);
  }

  //
  // Determine which locale owns a particular index
  //
  proc idxToLocaleInd(ind: glbIdxType) {
    const numlocs = targetLocDom.numIndices;
    return (mod(mod(ind, numlocs) - mod(min(glbIdxType), numlocs),
               numlocs) + targetLocDom.low):index(targetLocs.domain);
  }

  proc idxToLocale(ind: glbIdxType) {
    return targetLocs(idxToLocaleInd(ind));
  }
}

//
// A per-locale local distribution class
//
class LocCyclic1DDist {
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
  const myChunk: domain(1, glbIdxType, true);
  const locid: int;
  const loc: locale;

  //
  // Compute what chunk of index(1) is owned by the current locale
  // Arguments:
  //
  proc init(type glbIdxType, 
            _locid: int, // the locale index from the target domain
            dist: unmanaged Cyclic1DDist(glbIdxType) // reference to glob dist
                     ) {
    this.glbIdxType = glbIdxType;

    const locid0 = dist.targetLocDom.indexOrder(_locid); // 0-based locale ID
    const lo = min(glbIdxType) + locid0;
    const hi = max(glbIdxType);
    const numlocs = dist.targetLocDom.numIndices;
    myChunk = {lo..hi by numlocs};
    locid = _locid;
    loc = dist.targetLocs(locid);
    this.complete();
    if debugCyclic1D then
      writeln("locale ", locid, " owns ", myChunk);
  }

  proc writeThis(x) {
    x.write("locale ", loc.id, " owns chunk: ", myChunk);
  }
}


//
// The global domain class
//
class Cyclic1DDom {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;

  //
  // a pointer to the parent distribution
  //
  const dist: unmanaged Cyclic1DDist(glbIdxType);

  //
  // a domain describing the complete domain
  //
  const whole: domain(1, glbIdxType, false);

  //
  // an array of local domain class descriptors -- set up in
  // initializer below
  //
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initializer; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  // Beware that we cannot pass 'this' to 'new unmanaged LocCyclic1DDom'
  // while initializing 'this.locDoms'.
  //
  var locDoms: [dist.targetLocDom] unmanaged LocCyclic1DDom(glbIdxType)?;

  proc init(type idxType, myDist, myDom) {
    glbIdxType = idxType;
    dist = myDist;
    whole = myDom;

    this.complete();
    for locid in dist.targetLocDom do
      on dist.targetLocs(locid) do {
        locDoms(locid) = new unmanaged LocCyclic1DDom(glbIdxType, _to_unmanaged(this), 
                                           dist.getChunk(whole, locid));
      }
    if debugCyclic1D then
      [loc in dist.targetLocDom] writeln(loc, " owns ", locDoms(loc));
  }

  proc deinit() {
    for locid in dist.targetLocDom do
      on dist.targetLocs(locid) do
        delete locDoms(locid);
  }

  //
  // the iterator for the domain -- currently sequential
  //
  iter these() {
    for blk in locDoms do
      // TODO: Would want to do something like:     
      // on blk do
      // But can't currently have yields in on clauses
        for ind in blk! do
        {
	  if debugCyclic1D then
            writeln("yielding: ", ind, " in: ", blk);
          yield ind;
	}
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
      yield locDom!.myBlock.translate(-whole.low);
  }


  iter newThese(param iterator: IteratorType, followThis)
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
    for i in followThis
    {
      if debugCyclic1D then
        writeln("yielding: ", i);
      yield i + whole.low;
    }
  }

  //
  // the print method for the domain
  //
  proc writeThis(x) {
    x.write(whole);
  }

  //
  // how to allocate a new array over this domain
  //
  proc newArray(type elemType) {
    return new unmanaged Cyclic1DArr(glbIdxType, elemType, _to_unmanaged(this));
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
class LocCyclic1DDom {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;

  //
  // a reference to the parent global domain class
  //
  const wholeDom: unmanaged Cyclic1DDom(glbIdxType);

  //
  // a local domain describing the indices owned by this locale
  //
  var myBlock: domain(1, glbIdxType, true);

  //
  // iterator over this locale's indices
  //
  iter these() {
    // May want to do something like:     
    // on this do
    // But can't currently have yields in on clauses
    for ind in myBlock do
    {
      if debugCyclic1D then
        writeln("now yielding: ", ind);
      yield ind;
    }
  }

  //
  // this is the parallel iterator for the local domain, see global
  // domain parallel iterators for general notes on the approach
  //
  proc newThese(param iterator: IteratorType)
        where iterator == IteratorType.leader {
  }

  proc newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
  }


  //
  // how to write out this locale's indices
  //
  proc writeThis(x) {
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

private use IO;

//
// the global array class
//
class Cyclic1DArr {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;

  //
  // the array's element type
  //
  type elemType;
  
  //
  // the global domain descriptor for this array
  //
  var dom: unmanaged Cyclic1DDom(glbIdxType);

  //
  // an array of local array classes
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  //
  var locArr: [dom.dist.targetLocDom] unmanaged LocCyclic1DArr(glbIdxType, elemType)?;

  proc init(type idxType, type eltType, myDom) {
    glbIdxType = idxType;
    elemType = eltType;
    dom = myDom;
    this.complete();
    for locid in dom.dist.targetLocDom do
      on dom.dist.targetLocs(locid) do
        locArr(locid) = new unmanaged LocCyclic1DArr(glbIdxType, elemType, dom.locDoms(locid)!);
  }

  proc deinit() {
    for locid in dom.dist.targetLocDom do
      on dom.dist.targetLocs(locid) do
        delete locArr(locid);
  }

  //
  // the global accessor for the array
  //
  proc this(i: glbIdxType) ref {
    return locArr(dom.dist.idxToLocaleInd(i))!(i);
  }

  //
  // the iterator over the array's elements, currently sequential
  //
  iter these() ref {
    for loc in dom.dist.targetLocDom {
      // TODO: May want to do something like:     
      // on this do
      // But can't currently have yields in on clauses
      for elem in locArr(loc)! {
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
    for blk in dom.newThese(IteratorType.leader) do
      yield blk;
  }

  iter newThese(param iterator: IteratorType, followThis) ref
        where iterator == IteratorType.follower {
    for i in followThis {
      if debugCyclic1D then
        writeln("yielding ", i + dom.low);
      yield this(i + dom.low);
    }
  }

  //
  // how to print out the whole array, sequentially
  //
  proc writeThis(x) {
    var first = true;
    for loc in dom.dist.targetLocDom {
      // May want to do something like the following:
      //      on loc {
      // but it causes deadlock -- see writeThisUsingOn.chpl
        if (locArr(loc)!.numElements >= 1) {
          if (first) {
            first = false;
          } else {
            x.write(" ");
          }
	  if debugCyclic1D then
            writeln("Writing elements on locale: ", loc);
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
class LocCyclic1DArr {
  //
  // The index types of the global and local domain portions
  //
  type glbIdxType;

  //
  // the element type
  //
  type elemType;

  //
  // a reference to the local domain class for this array and locale
  //
  const locDom: unmanaged LocCyclic1DDom(glbIdxType);

  //
  // the block of local array data
  //
  var myElems: [locDom.myBlock] elemType;

  //
  // the accessor for the local array -- assumes the index is local
  //
  proc this(i: glbIdxType) ref {
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
  }

  proc newThese(param iterator: IteratorType, followThis)
        where iterator == IteratorType.follower {
  }


  //
  // prints out this locale's piece of the array
  //
  proc writeThis(x) {
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
