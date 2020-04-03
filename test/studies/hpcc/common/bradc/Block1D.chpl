// TODO: Convert to Steve's leader/follower scheme (implemented in compiler)

// TODO: Make multidimensional

// TODO: Make this work for a strided domain of locales; for a strided
// domain implemented using this distribution.

// TODO: Have the global class leader/follower iterators defer to the
// local class leader/followers once we're within a locale?

// TODO: Make these into an official distribution?

// TODO: implement the slicing interface?

config param debugBlock1D = false;

//
// The distribution class
//
class Block1D : Distribution {

  // GENERICS:

  //
  // The distribution's index type and domain's global index type
  //
  type idxType = int(64);


  // STATE:

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


  // LINKAGE:

  //
  // DOWN: an array of local distribution class descriptors -- set up
  // in the class constructor
  //
  const locDist: [targetLocDom] LocBlock1DDist(idxType);


  // CONSTRUCTORS:

  proc Block1D(type idxType = int(64), bbox: domain(1, idxType),
              targetLocales: [] locale = Locales) {
    boundingBox = bbox;
    //
    // 0-base the local capture of the targetLocDom for simplicity
    // later on
    //
    // TODO: Create a helper function to create a domain like this for
    // arbitrary dimensions (since the k-D case is a bit harder?)
    //
    targetLocDom = {0..#targetLocales.size};
    targetLocs = targetLocales;

    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocBlock1DDist(idxType, locid, this);

    //    writeln("Created a Block1D:\n", this);
  }


  // DISTRIBUTION INTERFACE:

  //
  // Create a new domain over this distribution with the given index
  // set (inds) and global index type (idxType, idxType)
  //
  // TODO: What should we do if domIdxType did not match idxType?
  //
  proc newArithmeticDomain(param rank: int, type domIdxType, param stridable: bool) 
        where domIdxType != idxType {
    compilerError("Trying to create a domain whose index type does not match the distribution's");
  }

  proc newArithmeticDomain(param rank: int, type domIdxType, param stridable: bool) 
        where rank != 1 {
    compilerError("Block1D only supports 1D domains currently");
  }

  //
  // INTERFACE NOTES: Should we support a form that passes in an
  // initial index set if one exists?  If not, we should rewrite the
  // global domain construction to not do anything with whole...
  //
  proc newArithmeticDomain(param rank: int, type idxType, param stridable: bool) {
    var dom = new Block1DDom(idxType=idxType, dist=this, stridable=stridable);
    dom.setup();
    return dom;
  }


  //
  // print out the distribution
  //
  proc writeThis(x:Writer) throws {
    x.writeln("Block1DPar");
    x.writeln("---------------");
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
  proc idxToLocale(ind: idxType) {
    return targetLocs(idxToLocaleInd(ind));
  }


  // INTERNAL INTERFACE:

  //
  // compute what chunk of inds is owned by a given locale -- assumes
  // it's being called on the locale in question
  //
  proc getChunk(inds, locid) {
    // use domain slicing to get the intersection between what the
    // locale owns and the domain's index set
    //
    // TODO: Should this be able to be written as myChunk[inds] ???
    //
    // TODO: Does using David's detupling trick work here?
    //
    return locDist(locid).myChunk(inds.dim(0));
  }
  
  //
  // Determine which locale owns a particular index
  //
  // TODO: I jotted down a note during the code review asking whether
  // targetLocs.size and boundingbox.size should be
  // captured locally, or captured in the default dom/array implementation
  // or inlined.  Not sure what that point was anymore, though.  Maybe
  // someone else can help me remember it (since it was probably someone
  // else's suggestion).
  //
  proc idxToLocaleInd(ind: idxType) {
    //    writeln("distribution = ", this);
    const ind0 = ind - boundingBox.low;
    const locInd = (ind0 * targetLocs.size:idxType) / boundingBox.size;
    return locInd: index(targetLocDom);
  }
}

//
// A per-locale local distribution class
//
class LocBlock1DDist {

  // GENERICS:

  // 
  // The distribution's index type and domain's global index type
  //
  type idxType;


  // STATE:

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


  // CONSTRUCTORS:

  //
  // Constructor computes what chunk of index(1) is owned by the
  // current locale
  //
  proc LocBlock1DDist(type idxType, 
                     _localeIdx: int, // the locale index from the target domain
                     dist: Block1D(idxType) // reference to glob dist
                     ) {
    const localeIdx = _localeIdx;
    loc = dist.targetLocs(localeIdx);
    //
    // TODO: Create these assertions for other local classes as well
    //
    if (loc != here) {
      halt("Creating a local distribution class on the wrong locale");
    }
    const lo = dist.boundingBox.low;
    const hi = dist.boundingBox.high;
    const numelems = hi - lo + 1;
    const numlocs = dist.targetLocDom.size;
    const blo = if (localeIdx == 0) then min(idxType)
                else procToData((numelems: real * localeIdx) / numlocs, lo);
    const bhi = if (localeIdx == numlocs - 1) then max(idxType)
                else procToData((numelems: real * (localeIdx+1)) / numlocs, lo) - 1;
    myChunk = {blo..bhi};
    if debugBlock1D then
      writeln(this);
  }


  // INTERNAL INTERFACE:

  //
  // a helper function for mapping processors to indices
  //
  proc procToData(x, lo)
    return (lo + (x: lo.type) + (x:real != x:int:real): lo.type);

  //
  // print out the local distribution class
  //
  proc writeThis(x:Writer) throws {
    x.write("locale ", loc.id, " owns chunk: ", myChunk);
  }
}


//
// The global domain class
//
class Block1DDom: BaseArithmeticDomain {
  // GENERICS:

  //
  // The index types of the domain
  //
  type idxType;

  //
  // The rank of the domain
  //
  // TODO: remove the default value; make this work for rank != 1
  //
  param rank = 1;

  param stridable: bool; // sjd: added stridable parameter

  // LINKAGE:

  //
  // LEFT: a pointer to the parent distribution
  //
  const dist: Block1D(idxType);

  //
  // DOWN: an array of local domain class descriptors -- set up in
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


  // STATE:

  //
  // a domain describing the complete domain
  //
  const whole: domain(1, idxType, stridable);

  var pid: int;

  // GLOBAL DOMAIN INTERFACE:

  //
  // the iterator for the domain -- currently sequential
  //
  //
  // TODO: This really should go over the elements in row-major order,
  // not the block orders.
  //
  proc these() {
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
  proc these(param tag: iterKind) where tag == iterKind.leader {
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
    coforall locDom in locDoms do
      on locDom {
        yield locDom.myBlock - whole.low;
      }
  }

  proc these(param tag: iterKind, followThis) where tag == iterKind.follower {
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

  proc strideBy(str : int) {
    var x = new Block1DDom(idxType=idxType, rank=rank, stridable=true, dist=dist);
    x.whole = whole by str;
    x.setup();
    return x;
  }

  //
  // the print method for the domain
  //
  proc writeThis(x:Writer) throws {
    x.write(whole);
  }

  //
  // how to allocate a new array over this domain
  //
  proc buildArray(type elemType) {
    var arr = new Block1DArr(idxType, elemType, stridable, this);
    arr.setup();
    return arr;
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


  //
  // INTERFACE NOTES: Could we make setIndices() for an arithmetic
  // domain take a domain rather than something else?
  //
  proc setIndices(x: domain) {
    if x.rank != 1 then
      compilerError("rank mismatch in domain assignment");
    if x._value.idxType != idxType then
      compilerError("index type mismatch in domain assignment");
    whole = x;
    setup();
  }

  proc setIndices(x) {
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

  proc getIndices() {
    return whole;
  }

  //
  // INTERNAL INTERFACE
  //
  proc dsiMyDist(): Block1D(idxType) {
    return dist;
  }

  proc setup() {
    for localeIdx in dist.targetLocDom do
      on dist.targetLocs(localeIdx) do
        if (locDoms(localeIdx) == nil) then
          locDoms(localeIdx) = new LocBlock1DDom(idxType, stridable, this, 
                                                 dist.getChunk(whole, localeIdx));
        else
          locDoms(localeIdx).myBlock = dist.getChunk(whole, localeIdx);
    if debugBlock1D then
      [loc in dist.targetLocDom] writeln(loc, " owns ", locDoms(loc));

  }

  proc supportsPrivatization() param return true;
  proc privatize1() {
    var c = new Block1DDom(idxType=idxType, rank=rank, stridable=stridable, dist=dist);
    c.locDoms = locDoms;
    c.whole = whole;
    return c;
  }
  proc privatize2(pid: int) {
    this.pid = pid;
  }
}


//
// the local domain class
//
class LocBlock1DDom {

  // GENERICS:

  //
  // The index type of the domain
  //
  type idxType;

  param stridable: bool; // sjd: added stridable parameter


  // LINKAGE:

  //
  // UP: a reference to the parent global domain class
  //
  const wholeDom: Block1DDom(idxType, 1, stridable);


  // STATE:

  //
  // a local domain describing the indices owned by this locale
  //
  // TODO: Steve would like to see the strided parameter pushed into
  // the global and local domain classes.
  //
  // NOTE: I used to use a local index type for this, but that would
  // require a glbIdxType offset in order to get from the global
  // indices back to the local index type.
  //
  var myBlock: domain(1, idxType, stridable);


  // LOCAL DOMAIN INTERFACE:

  //
  // iterator over this locale's indices
  //
  proc these() {
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
  proc these(param tag: iterKind) where tag == iterKind.leader {
    halt("This is bogus");
    yield {1..100};
  }

  proc these(param tag: iterKind, followThis) where tag == iterKind.follower {
    halt("This is bogus");
    yield 2;
  }

  //
  // how to write out this locale's indices
  //
  proc writeThis(x:Writer) throws {
    x.write(myBlock);
  }


  // INTERNAL INTERFACE:

  //
  // queries for this locale's number of indices, low, and high bounds
  //
  // TODO: I believe these are only used by the random number generator
  // in stream -- will they always be required once that is rewritten?
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
class Block1DArr: BaseArray {
  // GENERICS:

  //
  // The index type of the domain
  //
  type idxType;

  //
  // the array's element type
  //
  type eltType;

  param stridable: bool; // sjd: added stridable parameter

  // LINKAGE:
  
  //
  // LEFT: the global domain descriptor for this array
  //
  var dom: Block1DDom(idxType, 1, stridable);

  //
  // DOWN: an array of local array classes
  //
  // TODO: would like this to be const and initialize in-place,
  // removing the initialize method; would want to be able to use
  // an on-clause at the expression list to make this work.
  // Otherwise, would have to move the allocation into a function
  // just to get it at the statement level.
  // SJD: note cannot do this anymore because constructor does not
  // setup (for privatization reasons)
  //
  var locArr: [dom.dist.targetLocDom] LocBlock1DArr(idxType, eltType, stridable);

  proc setup() {
    coforall localeIdx in dom.dist.targetLocDom do
      on dom.dist.targetLocs(localeIdx) do
        locArr(localeIdx) = new LocBlock1DArr(idxType, eltType, stridable, dom.locDoms(localeIdx));
  }

  proc supportsPrivatization() param return true;
  proc privatize1() {
    var c = new Block1DArr(idxType, eltType, stridable, dom);
    c.locArr = locArr;
    return c;
  }
  proc privatize2(pid) {
    this.dom = chpl_getPrivatizedCopy(dom.type, dom.pid);
  }

  // GLOBAL ARRAY INTERFACE:

  //
  // the global accessor for the array
  //
  // TODO: Do we need a global bounds check here or in idxToLocaleind?
  //
  proc this(i: idxType) ref {
    const myLocArr = locArr(here.id);
    local {
      if myLocArr.locDom.myBlock.contains(i) then
        return myLocArr.this(i);
    }
    return locArr(dom.dist.idxToLocaleInd(i))(i);
  }

  //
  // the iterator over the array's elements, currently sequential
  //
  proc these() ref {
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
  // this is the parallel iterator for the global array, see the
  // example for general notes on the approach
  //
  proc these(param tag: iterKind) where tag == iterKind.leader {
    //
    // TODO: Rewrite this to reuse more of the global domain iterator
    // logic?  (e.g., can we forward the forall to the global domain
    // somehow?
    //
    coforall locDom in dom.locDoms do
      on locDom do
        yield locDom.myBlock - dom.whole.low;
  }

  proc supportsAlignedFollower() param return true;

  proc these(param tag: iterKind, followThis, param aligned: bool = false) ref where tag == iterKind.follower {
    //
    // TODO: Would like to write this as followThis += dom.low;
    //
    const myFollowThis = followThis + dom.low;

    //
    // TODO: The following is a buggy hack that will only work when we're
    // distributing across the entire Locales array.  I still think the
    // locArr/locDoms arrays should be associative over locale values.
    //
    const myLocArr = locArr(here.id);
    if aligned {
      local {
        for i in myFollowThis {
          yield myLocArr.this(i);
        }
      }
    } else {
      //
      // we don't own all the elements we're following
      //
      // TODO: could do something smarter to only bring the non-local
      // elements over.
      proc accessHelper(i) ref {
        local {
          if myLocArr.locDom.myBlock.contains(i) then
            return myLocArr.this(i);
        }
        return this(i);
      }
      for i in myFollowThis {
        yield accessHelper(i);
      }
    }


    // SJD: This was here before I added aligned:
    /*
    //
    // TODO: The following is a buggy hack that will only work when we're
    // distributing across the entire Locales array.  I still think the
    // locArr/locDoms arrays should be associative over locale values.
    //
    const myLocArr = locArr(here.id);
    if (myLocArr.owns(myFollowThis)) {
      // we own all the elements we're following
      //

      //
      // TODO: Want this, but local does not currently work within
      // follower iterators:
      //
      //      local {
        for i in myFollowThis {
          yield myLocArr.this(i);
        }
        //      }
      //
    } else {
      writeln("Warning: doing expensive these iteration");
      //
      // we don't own all the elements we're following
      //
      // TODO: could do something smarter to only bring the non-local
      // elements over.
      for i in myFollowThis {
        yield this(i);
      }
    }
    */
  }

  //
  // how to print out the whole array, sequentially
  //
  proc writeThis(x: Writer) throws {

    var first = true;
    for loc in dom.dist.targetLocDom {
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

  // GENERICS:

  //
  // The index type of the domain
  //
  type idxType;

  //
  // the element type
  //
  type eltType;


  param stridable: bool; // sjd: added stridable parameter


  // LINKAGE:

  //
  // LEFT: a reference to the local domain class for this array and locale
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
  proc this(i: idxType) ref {
    return myElems(i);
  }

  //
  // iterator over the elements owned by this locale
  //
  proc these() ref {
    for elem in myElems {
      yield elem;
    }
  }

  //
  // this is the parallel iterator for the local array, see global
  // domain parallel iterators for general notes on the approach
  //
  proc these(param tag: iterKind) where tag == iterKind.leader {
    halt("This is bogus");
    yield {1..100};
  }

  proc these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
    yield myElems(0);
  }


  //
  // prints out this locale's piece of the array
  //
  proc writeThis(x: Writer) throws {
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

  // INTERNAL INTERFACE:

  proc owns(x) {
    //
    // TODO: When this is multidimensional need to do a reduction or something:
    //
    return locDom.myBlock.dim(0).boundsCheck(x.dim(0));
  }
}
