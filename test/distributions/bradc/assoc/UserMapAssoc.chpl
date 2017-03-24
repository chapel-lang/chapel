// TODO: Convert to Steve's leader/follower scheme (implemented in compiler)

// TODO: Make multidimensional

// TODO: Make this work for a strided domain of locales; for a strided
// domain implemented using this distribution.

// TODO: Have the global class leader/follower iterators defer to the
// local class leader/followers once we're within a locale?

// TODO: Make these into an official distribution?

// TODO: implement the slicing interface?

config param debugUserMapAssoc = false;


// Documentation only - this could be an interface one day
// Returns an integer index into targetLocales
// b/c this matches best with the expected use and it is
// easy to guarantee that the returned locale is in the target set.
record AbstractMapper {
  proc indexToLocaleIndex(ind, targetLocales: [] locale) : int {
    return 0;
  }
}

record DefaultMapper {
  proc indexToLocaleIndex(ind, targetLocales: [] locale) : int {
    const hash = chpl__defaultHashWrapper(ind);
    // Mix the bits around
    const mixed = _gen_key(hash);
    const int_hash = (mixed & max(int)): int;
    const numlocs = targetLocales.domain.size;
    // Now extract the low bits
    const idx = int_hash % numlocs;
    return idx;
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
  // Should implement indexToLocale, ...
  const mapper;

  // STATE:



  //
  // a domain and array describing the set of target locales to which
  // the indices are mapped
  //
  const targetLocDom: domain(1);
  const targetLocales: [targetLocDom] locale;


  // privatized object id
  var pid: int = -1;

  // LINKAGE:

  //
  // DOWN: an array of local distribution class descriptors -- set up
  // in the class constructor
  //
  // commented out b/c it is not currently used
  //const locDist: [targetLocDom] LocUserMapAssocDist(idxType);


  // CONSTRUCTORS:

  proc UserMapAssoc(type idxType = int(64),
                    mapper:?t = new DefaultMapper(),
                    targetLocales: [] locale = Locales) {
    //
    // 0-base the local capture of the targetLocDom for simplicity
    // later on
    //
    // TODO: Create a helper function to create a domain like this for
    // arbitrary dimensions (since the k-D case is a bit harder?)
    //
    targetLocDom = {0..#targetLocales.numElements};
    this.targetLocales = targetLocales;

    // setting locDist commented out b/c not currently used
    //for locid in targetLocDom do
    //  on targetLocales(locid) do
    //    locDist(locid) = new LocUserMapAssocDist(idxType, mapper.type, locid, this);
  }

  //
  // builds up a privatized (replicated copy)
  //
  proc UserMapAssoc(type idxType = int(64),
                    mapper,
                    other: UserMapAssoc(idxType, mapper.type)) {
    this.mapper = mapper; // normally == other.mapper;
    targetLocDom = other.targetLocDom;
    this.targetLocales = other.targetLocales;
    // commented out b/c locDist is not currently used
    //locDist = other.locDist;
  }

  proc dsiClone() {
    return new UserMapAssoc(idxType, mapper, targetLocales);
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
    var dom = new UserMapAssocDom(idxType=idxType, mapperType=mapper.type, dist=this);
    dom.setup();
    return dom;
  }


  // debugging print
  proc dsiDisplayRepresentation() {
    writeln("targetLocDom = ", targetLocDom);
    writeln("targetLocales = ", for tl in targetLocales do tl.id);
    //for tli in targetLocDom do
    //  writeln("locDist[", tli, "] = ", locDist[tli]);
    //}
  }

  //
  // print out the distribution
  //
  proc WriteThis(x) {
    x.writeln("UserMapAssoc");
    x.writeln("-------");
    x.writeln("distributed using: ", mapper);
    x.writeln("across locales: ", targetLocales);
    x.writeln("indexed via: ", targetLocDom);
    x.writeln("resulting in: ");
    //for locid in targetLocDom do
    //  x.writeln("  [", locid, "] ", locDist(locid));
  }

  //
  // convert an index into a locale value
  //
  proc indexToLocale(ind: idxType) {
    const locIdx = indexToLocaleIndex(ind);
    return targetLocales[locIdx];
  }

  // Domain map interface version of the same
  proc dsiIndexToLocale(ind: idxType) {
    return indexToLocale(ind);
  }

  proc indexToLocaleIndex(ind: idxType) {
    const locIdx = mapper.indexToLocaleIndex(ind, targetLocales);
    if boundsChecking then
      if !targetLocDom.member(locIdx) then
        halt("mapper provided invalid locale index: ",
             locIdx,
             " is not in domain ",
             targetLocDom);
    return locIdx;
  }
}

//
// A per-locale local distribution class
//
// commented out b/c not currently used
/*class LocUserMapAssocDist {

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
                           type mapperType,
                          _localeIdx: int, // the locale index from the target domain
                          dist: UserMapAssoc(idxType,mapperType) // reference to glob dist
                         ) {
    const localeIdx = _localeIdx;
    loc = dist.targetLocales(localeIdx);
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
  proc writeThis(x) {
    x.write("locale ", loc.id, " owns some indices");
  }
}
*/

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
  type mapperType;

  // LINKAGE:

  //
  // LEFT: a pointer to the parent distribution
  //
  const dist: UserMapAssoc(idxType, mapperType);

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
  var locDoms: [dist.targetLocDom] LocUserMapAssocDom(idxType, mapperType);


  // STATE:

  //
  // a domain describing the complete domain
  //

  var pid: int = -1;

  // GLOBAL DOMAIN INTERFACE:

  proc dsiMyDist() {
    return dist;
  }

  proc dsiAdd(i: idxType) {
    return locDoms(dist.indexToLocaleIndex(i)).add(i);
  }

  proc dsiRemove(i: idxType) {
    return locDoms(dist.indexToLocaleIndex(i)).remove(i);
  }

  proc dsiMember(i: idxType) {
    return locDoms(dist.indexToLocaleIndex(i)).member(i);
  }

  proc dsiClear() {
    for locDom in locDoms do on locDom {
      locDom.clear();
    }
  }


  proc dsiRequestCapacity(numKeys:int) {
    // TODO
  }

  iter dsiSorted(comparator) {
    use Sort;
    // This function creates a local copy of an entire distributed
    // data structure, which is probably a bad idea.
    // Alternatives include:
    //  * create per-locale sorted arrays, merge them to output;
    //    ideally this would use a tournament tree/heap;
    //    it would also be possible to iterate through
    //    locDom.sorted() and pipe it through a channel or something.
    //     (such a use of a channel would probably benefit from
    //      "shallow write/read" or "nonpersistant write/read"
    //      meaning that the channel would just store the same information
    //      as a GET).
    //  * include in mapper something to indicate if indexToLocaleIndex
    //    is order-preserving. If it is, this iterator could just yield
    //    from each locale, one at a time

    // compute the number of indices
    var numIndices = dsiNumIndices;
    // create a local, non-distributed  array with that many elements
    var tableCopy: [0..#numIndices] idxType;

    // this would work, but get "whole array assignment has been serialized"
    // error
    //tableCopy = this; // run these()

    var idx = 0;
    for blk in locDoms {
      for ind in blk {
        tableCopy[idx] = ind;
        idx += 1;
      }
    }

    sort(tableCopy, comparator);

    for ind in tableCopy do
      yield ind;
  }

  //
  // the iterator for the domain -- sequential version
  //
  // It is reasonable to visit the elements block by block,
  // since associative domains do not define their order of iteration.
  //
  iter these() {
    for blk in locDoms do
      // TODO: Would want to do something like:     
      //on blk do
      // But can't currently have yields in on clauses:
      // invalid use of 'yield' within 'on' in serial iterator
        for ind in blk do
          yield ind;
  }

  // leader iterator for the distributed domain
  iter these(param tag: iterKind) where tag == iterKind.leader {
    //
    // TODO -- how does this divide into tasks?
    // Iterator forwarding, as in Block line 779 ?
    coforall (locDom, localeIndex) in zip(locDoms,dist.targetLocDom) {
      on locDom {
        // redirect to the DefaultAssociative's leader
        // note that DefaultAssociative's leader returns (lo..hi, this)
        // ie. a range of table slots and then the DefaultAssociativeDom.
        for follow in locDom.myInds._value.these(tag) do
          yield (follow, localeIndex);
        //  for followThis in tmpBlock._value.these(iterKind.leader, maxTasks,
        //                                           myIgnoreRunning, minSize,
        //                                           locOffset)
        //yield locDom;
      }
    }
  }

  // follower iterator for the distributed domain
  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    var (locFollowThis, localeIndex) = followThis;

    var locDom = locDoms[localeIndex];

    for i in locDom.myInds._value.these(tag, locFollowThis) do
      yield i;
  }

  iter these(param tag: iterKind) where tag == iterKind.standalone {
    coforall locDom in locDoms do on locDom {
      // Forward to associative domain standalone iterator
      for i in locDom.myInds._value.these(tag) {
        yield i;
      }
    }
  }

  //
  // the print method for the domain
  //
  proc dsiSerialWrite(x) {
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
    var arr = new UserMapAssocArr(idxType=idxType, mapperType=mapperType, eltType=elemType, dom=this);
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
  // These are for regular domains only!
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
  proc dsiMyDist(): UserMapAssoc(idxType, mapperType) {
    return dist;
  }

  proc setup() {
    for localeIdx in dist.targetLocDom do
      on dist.targetLocales(localeIdx) do
        if (locDoms(localeIdx) == nil) then
          locDoms(localeIdx) = new LocUserMapAssocDom(idxType=idxType,
              mapperType=mapperType, this);
    if debugUserMapAssoc then
      [loc in dist.targetLocDom] writeln(loc, " owns ", locDoms(loc));

  }

  proc dsiSupportsPrivatization() param return true;
  proc dsiGetPrivatizeData() return 0;
  proc dsiPrivatize(privatizeData) {
    var privateDist = new UserMapAssoc(idxType, dist.mapper, dist);
    var c = new UserMapAssocDom(idxType=idxType, mapperType=mapperType, dist=privateDist);
    c.locDoms = locDoms;
    return c;
  }
  proc dsiReprivatize(other) {
    locDoms = other.locDoms;
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
  type mapperType;


  // LINKAGE:

  //
  // UP: a reference to the parent global domain class
  //
  const wholeDom: UserMapAssocDom(idxType, mapperType, parSafe=true);


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
    return myInds.add(i);
  }
  proc remove(i: idxType) {
    return myInds.remove(i);
  }

  proc member(i: idxType) {
    return myInds.member(i);
  }

  proc clear() {
    myInds.clear();
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
  proc writeThis(x) {
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
  type mapperType;

  //
  // the array's element type
  //
  type eltType;

  // LINKAGE:
  
  //
  // LEFT: the global domain descriptor for this array
  //
  var dom: UserMapAssocDom(idxType, mapperType, parSafe=true);

  //
  // DOWN: an array of local array classes
  var locArrs: [dom.dist.targetLocDom] LocUserMapAssocArr(idxType, mapperType, eltType);
  //var locAssocDoms: domain(BaseAssociativeDom);
  //var locArrsByAssoc: [locAssocDoms] LocUserMapAssocArr(idxType, mapperType, eltType);

  var pid: int = -1; // privatized object id

  proc dsiGetBaseDom() return dom;

  proc setup() {
    coforall localeIdx in dom.dist.targetLocDom do
      on dom.dist.targetLocales(localeIdx) do
        locArrs(localeIdx) = new LocUserMapAssocArr(idxType, mapperType, eltType, dom.locDoms(localeIdx));
    for localeIdx in dom.dist.targetLocDom {
      var locDomImpl = dom.locDoms(localeIdx).myInds._value;
      //locAssocDoms += locDomImpl;
      //locArrsByAssoc[locDomImpl] = locArrs(localeIdx);
    }
  }

  proc dsiSupportsPrivatization() param return true;
  proc dsiGetPrivatizeData() return 0;
  proc dsiPrivatize(privatizeData) {
    var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
    var c = new UserMapAssocArr(idxType=idxType, mapperType=mapperType, eltType=eltType, dom=privdom);
    c.locArrs = locArrs;
    //c.locAssocDoms = locAssocDoms;
    //c.locArrsByAssoc = locArrsByAssoc;
    return c;
  }


  // GLOBAL ARRAY INTERFACE:

  //
  // the global accessor for the array
  //
  // TODO: Do we need a global bounds check here or in idxToLocaleind?
  //
  proc dsiAccess(i: idxType) ref {
    const localeIndex = dom.dist.indexToLocaleIndex(i);
    const locArr = locArrs[localeIndex];
    if locArr.locale == here {
      local {
        return locArr[i];
      }
    }
    return locArr[i];
  }
  proc dsiAccess(i: idxType)
  where shouldReturnRvalueByValue(eltType) {
    const localeIndex = dom.dist.indexToLocaleIndex(i);
    const locArr = locArrs[localeIndex];
    if locArr.locale == here {
      local {
        return locArr[i];
      }
    }
    return locArr[i];
  }
  proc dsiAccess(i: idxType) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    const localeIndex = dom.dist.indexToLocaleIndex(i);
    const locArr = locArrs[localeIndex];
    if locArr.locale == here {
      local {
        return locArr[i];
      }
    }
    return locArr[i];
  }



  //
  // sequential iterator over the array's elements
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
    // forward to UserMapAssocDom leader
    for followThis in dom.these(tag) do
      yield followThis;
  }

  iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
    var (locFollowThis, localeIndex) = followThis;

    var locArr = locArrs[localeIndex];

    // forward to locArr
    for i in locArr.myElems._value.these(tag, locFollowThis) do
      yield i;
  }

  iter these(param tag: iterKind) ref where tag == iterKind.standalone {
    coforall locArr in locArrs do on locArr {
      // Forward to associative array standalone iterator
      for i in locArr.myElems._value.these(tag) {
        yield i;
      }
    }
  }


  //
  // how to print out the whole array, sequentially
  //
  proc dsiSerialWrite(x) {

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

  proc dsiDisplayRepresentation() {

    writeln("dsiDisplayRepresentation TODO");
  }


  //
  // a query for the number of elements in the array
  //
  proc dsiNumElements {
    return dom.dsiNumIndices;
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
  type mapperType;

  //
  // the element type
  //
  type eltType;



  // LINKAGE:

  //
  // LEFT: a reference to the local domain class for this array and locale
  //
  const locDom: LocUserMapAssocDom(idxType, mapperType);


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
  proc this(i: idxType)
  where shouldReturnRvalueByValue(eltType) {
    return myElems(i);
  }
  proc this(i: idxType) const ref
  where shouldReturnRvalueByConstRef(eltType) {
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

  // INTERNAL INTERFACE:

  proc owns(x) {
    //
    // TODO: When this is multidimensional need to do a reduction or something:
    //
    return locDom.myInds.dim(1).boundsCheck(x.dim(1));
  }
}


