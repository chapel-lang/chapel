// TODO: Convert to Steve's leader/follower scheme (implemented in compiler)

// TODO: Make multidimensional

// TODO: Make this work for a strided domain of locales; for a strided
// domain implemented using this distribution.

// TODO: Have the global class leader/follower iterators defer to the
// local class leader/followers once we're within a locale?

// TODO: Make these into an official distribution?

// TODO: implement the slicing interface?

config param debugUserMapAssoc = false;


// TODO: Need to make this generic
class AbstractMapper {
  proc idxToLocaleInd(ind, numlocs: int) {
    return 0;
  }
}


//
// The distribution class
//
class UserMapAssoc : BaseDist {

  // GENERICS:

  //
  // The distribution's index type and domain's global index type
  //
  type idxType = int(64);


  // STATE:

  const mapper : AbstractMapper;


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
  const locDist: [targetLocDom] LocUserMapAssocDist(idxType);


  // CONSTRUCTORS:

  proc UserMapAssoc(type idxType = int(64), initMapper: AbstractMapper,
                   targetLocales: [] locale = Locales) {
    mapper = initMapper;
    //
    // 0-base the local capture of the targetLocDom for simplicity
    // later on
    //
    // TODO: Create a helper function to create a domain like this for
    // arbitrary dimensions (since the k-D case is a bit harder?)
    //
    targetLocDom = {0..#targetLocales.numElements};
    targetLocs = targetLocales;

    for locid in targetLocDom do
      on targetLocs(locid) do
        locDist(locid) = new LocUserMapAssocDist(idxType, locid, this);
  }

  //
  // builds up a privatized (replicated copy)
  //
  proc UserMapAssoc(type idxType = int(64),
                   other: UserMapAssoc(idxType)) {
    mapper = other.mapper;
    targetLocDom = other.targetLocDom;
    targetLocs = other.targetLocs;
    locDist = other.locDist;
  }

  proc dsiClone() {
    return new UserMapAssoc(idxType, mapper, targetLocs);
  }

  // DISTRIBUTION INTERFACE:

  //
  // Create a new domain over this distribution with the given index
  // set (inds) and global index type (idxType, idxType)
  //
  // TODO: What should we do if domIdxType did not match idxType?
  //
  proc dsiNewAssociativeDom(type domIdxType, param parSafe: bool) where domIdxType != idxType {
    compilerError("Trying to create a domain whose index type does not match the distribution's");
  }

  //
  // INTERFACE NOTES: Should we support a form that passes in an
  // initial index set if one exists?  If not, we should rewrite the
  // global domain construction to not do anything with whole...
  //
  proc dsiNewAssociativeDom(type idxType, param parSafe: bool) {
    var dom = new UserMapAssocDom(idxType=idxType, dist=this);
    dom.setup();
    return dom;
  }


  //
  // print out the distribution
  //
  proc WriteThis(x:Writer) {
    x.writeln("UserMapAssoc");
    x.writeln("-------");
    x.writeln("distributed using: ", mapper);
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

  proc idxToLocaleInd(ind: idxType) {
    return mapper.idxToLocaleInd(ind, targetLocDom.numIndices);
  }
}

pragma "auto copy fn"
proc chpl__autoCopy(x: UserMapAssoc) {
  if ! noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: UserMapAssoc) {
  if !noRefCount {
    var cnt = x.destroyDist();
    if cnt == 0 then
      delete x;
  }
}

//
// A per-locale local distribution class
//
class LocUserMapAssocDist {

  // GENERICS:

  // 
  // The distribution's index type and domain's global index type
  //
  type idxType;


  // STATE:

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
  proc LocUserMapAssocDist(type idxType, 
                          _localeIdx: int, // the locale index from the target domain
                          dist: UserMapAssoc(idxType) // reference to glob dist
                         ) {
    const localeIdx = _localeIdx;
    loc = dist.targetLocs(localeIdx);
    //
    // TODO: Create these assertions for other local classes as well
    //
    if (loc != here) {
      halt("Creating a local distribution class on the wrong locale");
    }

    if debugUserMapAssoc then
      writeln(this);
  }


  // INTERNAL INTERFACE:

  //
  // print out the local distribution class
  //
  proc writeThis(x:Writer) {
    x.write("locale ", loc.id, " owns some indices");
  }
}


//
// The global domain class
//
class UserMapAssocDom: BaseAssociativeDom {
  param parSafe: bool=true; // we need this because of the wrapper interface

  // GENERICS:

  //
  // The index types of the domain
  //
  type idxType;

  // LINKAGE:

  //
  // LEFT: a pointer to the parent distribution
  //
  const dist: UserMapAssoc(idxType);

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
  var locDoms: [dist.targetLocDom] LocUserMapAssocDom(idxType);


  // STATE:

  //
  // a domain describing the complete domain
  //

  var pid: int = -1;

  // GLOBAL DOMAIN INTERFACE:

  proc dsiAdd(i: idxType) {
    locDoms(dist.idxToLocaleInd(i)).add(i);
  }

  //
  // the iterator for the domain -- currently sequential
  //
  //
  // TODO: This really should go over the elements in row-major order,
  // not the block orders.
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
  // TODO: Steve's question: how would a leader containing an on
  // clause interact with a loop that used an on clause explicitly
  // within its body?  How would it be done efficiently?
  //
  iter these(param tag: iterKind) where tag == iterKind.leader {
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
        yield locDom;
      }
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
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
      yield i;
    }
  }

  //
  // the print method for the domain
  //
  proc dsiSerialWrite(x:Writer) {
    for locDom in locDoms do
      // TODO: This doesn't work -- accesses a bad file descriptor
      //      on locDom {
      //

      // TODO: The following doesn't work -- deadlock:
      //
      // writeln(""locale" + here.id + " owns: ");

      // TODO: The following didn't work either -- seg fault:
      //
      //        ("locale" + here.id + " owns: ").writeThis(x);

        x.write(locDom);
      //      }
  }

  //
  // how to allocate a new array over this domain
  //
  proc dsiBuildArray(type elemType) {
    var arr = new UserMapAssocArr(idxType=idxType, eltType=elemType, dom=this);
    arr.setup();
    return arr;
  }

  //
  // queries for the number of indices, low, and high bounds
  //
  proc dsiNumIndices {
    return + reduce [loc in dist.targetLocDom] locDoms[loc].myInds.numIndices;
  }

  //
  // INTERFACE NOTES: Could we make setIndices() for an arithmetic
  // domain take a domain rather than something else?
  //
  proc dsiSetIndices(x: domain) {
    writeln("setIndices not yet implemented");
  }

  proc dsiSetIndices(x) {
    writeln("setIndices not yet implemented");
  }

  proc dsiGetIndices() {
    writeln("getIndices not yet implemented");
  }

  //
  // INTERNAL INTERFACE
  //
  proc dsiMyDist(): UserMapAssoc(idxType) {
    return dist;
  }

  proc setup() {
    for localeIdx in dist.targetLocDom do
      on dist.targetLocs(localeIdx) do
        if (locDoms(localeIdx) == nil) then
          locDoms(localeIdx) = new LocUserMapAssocDom(idxType, this);
    if debugUserMapAssoc then
      [loc in dist.targetLocDom] writeln(loc, " owns ", locDoms(loc));

  }

  proc dsiSupportsPrivatization() param return true;
  proc dsiGetPrivatizeData() return 0;
  proc dsiPrivatize(privatizeData) {
    var privateDist = new UserMapAssoc(idxType, dist);
    var c = new UserMapAssocDom(idxType=idxType, dist=privateDist);
    c.locDoms = locDoms;
    return c;
  }
  proc dsiReprivatize(other) {
    locDoms = other.locDoms;
  }
}

pragma "auto copy fn"
proc chpl__autoCopy(x: UserMapAssocDom) {
  if ! noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: UserMapAssocDom) {
  if !noRefCount {
    var cnt = x.destroyDom();
    if cnt == 0 then
      delete x;
  }
}


//
// the local domain class
//
class LocUserMapAssocDom {

  // GENERICS:

  //
  // The index type of the domain
  //
  type idxType;


  // LINKAGE:

  //
  // UP: a reference to the parent global domain class
  //
  const wholeDom: UserMapAssocDom(idxType, parSafe=true);


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
  var myInds: domain(idxType, parSafe=true);


  // LOCAL DOMAIN INTERFACE:

  proc add(i: idxType) {
    myInds += i;
  }

  //
  // iterator over this locale's indices
  //
  iter these() {
    // May want to do something like:     
    // on this do
    // But can't currently have yields in on clauses
    for ind in myInds do
      yield ind;
  }

  //
  // this is the parallel iterator for the local domain, see global
  // domain parallel iterators for general notes on the approach
  //
  iter these(param tag: iterKind) where tag == iterKind.leader {
    halt("This is bogus");
    yield {1..100};
  }

  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    halt("This is bogus");
    yield 2;
  }

  //
  // how to write out this locale's indices
  //
  proc writeThis(x:Writer) {
    x.write(myInds);
  }


  // INTERNAL INTERFACE:

  //
  // queries for this locale's number of indices, low, and high bounds
  //
  // TODO: I believe these are only used by the random number generator
  // in stream -- will they always be required once that is rewritten?
  //
  proc numIndices {
    return myInds.numIndices;
  }
}


//
// the global array class
//
class UserMapAssocArr: BaseArr {
  // GENERICS:

  //
  // The index type of the domain
  //
  type idxType;

  //
  // the array's element type
  //
  type eltType;

  // LINKAGE:
  
  //
  // LEFT: the global domain descriptor for this array
  //
  var dom: UserMapAssocDom(idxType, parSafe=true);

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
  var locArrs: [dom.dist.targetLocDom] LocUserMapAssocArr(idxType, eltType);

  var pid: int = -1; // privatized object id

  proc dsiGetBaseDom() return dom;

  proc setup() {
    coforall localeIdx in dom.dist.targetLocDom do
      on dom.dist.targetLocs(localeIdx) do
        locArrs(localeIdx) = new LocUserMapAssocArr(idxType, eltType, dom.locDoms(localeIdx));
  }

  proc dsiSupportsPrivatization() param return true;
  proc dsiGetPrivatizeData() return 0;
  proc dsiPrivatize(privatizeData) {
    var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
    var c = new UserMapAssocArr(idxType=idxType, eltType=eltType, dom=privdom);
    c.locArrs = locArrs;
    return c;
  }


  // GLOBAL ARRAY INTERFACE:

  //
  // the global accessor for the array
  //
  // TODO: Do we need a global bounds check here or in idxToLocaleind?
  //
  proc dsiAccess(i: idxType) ref {
    const myLocArr = locArrs(here.id);
    local {
      if myLocArr.locDom.myInds.member(i) then
        return myLocArr.this(i);
    }
    return locArrs(dom.dist.idxToLocaleInd(i))(i);
  }

  //
  // the iterator over the array's elements, currently sequential
  //
  iter these() ref {
    for loc in dom.dist.targetLocDom {
      // TODO: May want to do something like:     
      // on this do
      // But can't currently have yields in on clauses
      for elem in locArrs(loc) {
        yield elem;
      }
    }
  }

  //
  // this is the parallel iterator for the global array, see the
  // example for general notes on the approach
  //
  iter these(param tag: iterKind) where tag == iterKind.leader {
    //
    // TODO: Rewrite this to reuse more of the global domain iterator
    // logic?  (e.g., can we forward the forall to the global domain
    // somehow?
    //
    coforall locDom in dom.locDoms do
      on locDom {
        yield locDom;
      }
  }

  proc supportsAlignedFollower() param return true;

  iter these(param tag: iterKind, followThis, param aligned: bool = false) ref where tag == iterKind.follower {
    //
    // TODO: Would like to write this as followThis += dom.low;
    //
    const myFollowThis = followThis;

    //
    // TODO: The following is a buggy hack that will only work when we're
    // distributing across the entire Locales array.  I still think the
    // locArrs/locDoms arrays should be associative over locale values.
    //
    const myLocArr = locArrs(here.id);
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
          if myLocArr.locDom.myInds.member(i) then
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
    // locArrs/locDoms arrays should be associative over locale values.
    //
    const myLocArr = locArrs(here.id);
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
  proc dsiSerialWrite(x: Writer) {

    var first = true;
    for locArr in locArrs {
      if locArr.numElements {
        if first {
          first = false;
        } else {
          x.write(" ");
        }
      }
      x.write(locArr);
      stdout.flush();
    }
  }

  //
  // a query for the number of elements in the array
  //
  proc dsiNumElements {
    return dom.dsiNumIndices;
  }
}

pragma "auto copy fn"
proc chpl__autoCopy(x: UserMapAssocArr) {
  if ! noRefCount then
    x.incRefCount();
  return x;
}

proc chpl__autoDestroy(x: UserMapAssocArr) {
  if !noRefCount {
    var cnt = x.destroyArr();
    if cnt == 0 then
      delete x;
  }
}


//
// the local array class
//
class LocUserMapAssocArr {

  // GENERICS:

  //
  // The index type of the domain
  //
  type idxType;

  //
  // the element type
  //
  type eltType;



  // LINKAGE:

  //
  // LEFT: a reference to the local domain class for this array and locale
  //
  const locDom: LocUserMapAssocDom(idxType);


  // STATE:

  //
  // the block of local array data
  //
  var myElems: [locDom.myInds] eltType;


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
  iter these() ref {
    for elem in myElems {
      yield elem;
    }
  }

  //
  // this is the parallel iterator for the local array, see global
  // domain parallel iterators for general notes on the approach
  //
  iter these(param tag: iterKind) where tag == iterKind.leader {
    halt("This is bogus");
    yield {1..100};
  }

  iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
    yield myElems(0);
  }


  //
  // prints out this locale's piece of the array
  //
  proc writeThis(x: Writer) {
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

  // INTERNAL INTERFACE:

  proc owns(x) {
    //
    // TODO: When this is multidimensional need to do a reduction or something:
    //
    return locDom.myInds.dim(1).boundsCheck(x.dim(1));
  }
}


//
// a helper function for blocking index ranges:
//

proc chpl_computeBlock(waylo, numelems, lo, wayhi, numblocks, blocknum) {
  proc procToData(x, lo)
    return lo + (x: lo.type) + (x:real != x:int:real): lo.type;

  const blo = if (blocknum == 0) then waylo
              else procToData((numelems: real * blocknum) / numblocks, lo);
  const bhi = if (blocknum == numblocks - 1) then wayhi
              else procToData((numelems: real * (blocknum+1)) / numblocks, lo) - 1;

  return (blo, bhi);
}
