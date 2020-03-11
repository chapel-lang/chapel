/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


config param debugUserMapAssoc = false;


// Documentation only - this could be an interface one day
// Returns an integer index into targetLocales
// b/c this matches best with the expected use and it is
// easy to guarantee that the returned locale is in the target set.
record AbstractMapper {
  proc this(const ref ind, const ref targetLocales: [?D] locale) : D.idxType {
    return 0;
  }
}

record DefaultMapper {
  proc this(ind, targetLocales: [?D] locale) : D.idxType {
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

/*

The Hashed distribution maps an associative domain and its array to a set of
target locales. Each index (or key) in the domain is mapped to a locale based
upon its value. When constructing a Hashed distribution, you can optionally
provide a `mapper` function or function object to compute the destination locale
for each index.

The `mapper` provided can be a class, record, or first class function.  When
called, it will be passed the index and the targetLocales array that the Hashed
distribution was initialized with. For example, the following record
declares a custom mapper:

.. code-block:: chapel

  record CustomMapper {
    proc this(ind:string, targetLocs: [?D] locale) : D.idxType {
      const numlocs = targetLocs.domain.size;
      // use the first digit of the string to choose the destination locale
      var byte: int = ind.byte(1);
      return byte % numlocs;
    }
  }

If a custom mapper is not provided, a default mapper will be used. The default
mapper computes the target locale based upon a hash of the index.

.. note::

  Hashed is not yet interface stable.

**Example**

.. code-block:: chapel

  var D: domain(string) dmapped Hashed(idxType=string);
  // Now D is a distributed associative domain (set) of strings
  D += "one";
  D += "two";

  var A: [D] int;
  // Now A is a distributed associative array (map) from string to int
  forall a in A do
    a = a.locale.id;

  forall (key, value) in zip(D, A) {
    writeln(key, " -> ", value);
  }


**Initializer Arguments**

The `Hashed` domain map initializer is defined as follows:

.. code-block:: chapel

  proc init(type idxType,
            mapper:?t = new DefaultMapper(),
            targetLocales: [] locale = Locales)

 */
class Hashed : BaseDist {

  // GENERICS:

  //
  // The distribution's index type and domain's global index type
  //
  type idxType = int(64);
  // Should implement `this` to map index to locale
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

  proc init(type idxType,
            mapper:?t = new DefaultMapper(),
            targetLocales: [] locale = Locales) {
    this.idxType = idxType;
    this.mapper = mapper;
    //
    // 0-base the local capture of the targetLocDom for simplicity
    // later on
    //
    // TODO: Create a helper function to create a domain like this for
    // arbitrary dimensions (since the k-D case is a bit harder?)
    //
    targetLocDom = {0..#targetLocales.size};
    this.targetLocales = targetLocales;

    // setting locDist commented out b/c not currently used
    //for locid in targetLocDom do
    //  on targetLocales(locid) do
    //    locDist(locid) = new LocUserMapAssocDist(idxType, mapper.type, locid, this);
  }

  //
  // builds up a privatized (replicated copy)
  //
  proc init(type idxType,
            mapper,
            other: unmanaged Hashed(idxType, mapper.type)) {
    this.idxType = idxType;
    this.mapper = mapper; // normally == other.mapper;
    targetLocDom = other.targetLocDom;
    this.targetLocales = other.targetLocales;
    // commented out b/c locDist is not currently used
    //locDist = other.locDist;
  }

  proc dsiClone() {
    return new unmanaged Hashed(idxType, mapper, targetLocales);
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
  override proc dsiNewAssociativeDom(type idxType, param parSafe: bool) {
    var dom = new unmanaged UserMapAssocDom(idxType=idxType, mapperType=mapper.type, dist=_to_unmanaged(this));
    dom.setup();
    return dom;
  }


  // debugging print
  override proc dsiDisplayRepresentation() {
    writeln("targetLocDom = ", targetLocDom);
    writeln("targetLocales = ", for tl in targetLocales do tl.id);
    //for tli in targetLocDom do
    //  writeln("locDist[", tli, "] = ", locDist[tli]);
    //}
  }

  //
  // print out the distribution
  //
  proc writeThis(x) throws {
    x <~> "Hashed\n";
    x <~> "-------\n";
    x <~> "distributed using: " <~> mapper <~> "\n";
    x <~> "across locales: " <~> targetLocales <~> "\n";
    x <~> "indexed via: " <~> targetLocDom <~> "\n";
    x <~> "resulting in: " <~> "\n";
    //for locid in targetLocDom do
    //  x <~> "  [" <~> locid <~> "] " <~> locDist(locid) <~> "\n";
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

  // Why does indexToLocaleIndex return an index into targetLocales
  // rather than a locale?
  //
  // indexToLocaleIndex is used below to efficiently compute
  // the local domain implementation, so the locDoms array can be
  // simple (1-D, not associative), and so targetLocales can be
  // flexible (e.g. include subdomains).

  proc indexToLocaleIndex(ind: idxType) {
    const locIdx = mapper(ind, targetLocales);
    if boundsChecking then
      if !targetLocDom.contains(locIdx) then
        halt("mapper provided invalid locale index: ",
             locIdx,
             " is not in domain ",
             targetLocDom);
    return locIdx;
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
  type mapperType;

  // LINKAGE:

  //
  // LEFT: a pointer to the parent distribution
  //
  const dist: unmanaged Hashed(idxType, mapperType);

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
  var locDoms: [dist.targetLocDom] unmanaged LocUserMapAssocDom(idxType, mapperType)?;


  // STATE:

  //
  // a domain describing the complete domain
  //

  var pid: int = -1;

  // GLOBAL DOMAIN INTERFACE:

  override proc dsiMyDist() {
    return dist;
  }

  proc dsiAdd(i: idxType) {
    return locDoms(dist.indexToLocaleIndex(i))!.add(i);
  }

  proc dsiRemove(i: idxType) {
    return locDoms(dist.indexToLocaleIndex(i))!.remove(i);
  }

  proc dsiMember(i: idxType) {
    return locDoms(dist.indexToLocaleIndex(i))!.contains(i);
  }

  override proc dsiClear() {
    for locDom in locDoms do on locDom {
      locDom!.clear();
    }
  }

  proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    if !lhsPrivate then
      halt("Hashed domain assignment not yet supported");
    for i in rhs do
      dsiAdd(i);
  }

  proc dsiRequestCapacity(numKeys:int) {
    // Multiplies by 2 to account for some expected load imbalance
    var nLocales = min(1, locDoms.domain.size);
    const numKeysPer = 2 * numKeys / nLocales;
    for locDom in locDoms do on locDom {
      locDom!.capacity(numKeysPer);
    }
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
    //      "shallow write/read" or "nonpersistent write/read"
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
      for ind in blk! {
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
        for ind in blk! do
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
        for follow in locDom!.myInds.these(tag) do
          yield (follow, localeIndex);
        //  for followThis in tmpBlock.these(iterKind.leader, maxTasks,
        //                                   myIgnoreRunning, minSize,
        //                                   locOffset)
        //yield locDom;
      }
    }
  }

  // follower iterator for the distributed domain
  iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
    var (locFollowThis, localeIndex) = followThis;

    var locDom = locDoms[localeIndex]!;

    for i in locDom.myInds.these(tag, locFollowThis) do
      yield i;
  }

  iter these(param tag: iterKind) where tag == iterKind.standalone {
    coforall locDom in locDoms do on locDom {
      // Forward to associative domain standalone iterator
      for i in locDom!.myInds.these(tag) {
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

        x <~> locDom;
      //      }
  }

  //
  // how to allocate a new array over this domain
  //
  proc dsiBuildArray(type elemType) {
    var arr = new unmanaged UserMapAssocArr(idxType=idxType, mapperType=mapperType, eltType=elemType, dom=_to_unmanaged(this));
    arr.setup();
    return arr;
  }

  //
  // queries for the number of indices, low, and high bounds
  //
  proc dsiNumIndices {
    return + reduce [loc in dist.targetLocDom] locDoms[loc]!.myInds.size;
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
  override proc dsiMyDist(): unmanaged Hashed(idxType, mapperType) {
    return dist;
  }

  proc setup() {
    coforall localeIdx in dist.targetLocDom do
      on dist.targetLocales(localeIdx) do
        if (locDoms(localeIdx) == nil) then
          locDoms(localeIdx) = new unmanaged LocUserMapAssocDom(idxType=idxType,
              mapperType=mapperType, _to_unmanaged(this));
    if debugUserMapAssoc then
      [loc in dist.targetLocDom] writeln(loc, " owns ", locDoms(loc));

  }

  proc dsiSupportsPrivatization() param return true;
  proc dsiGetPrivatizeData() return 0;
  proc dsiGetReprivatizeData() return 0;
  proc dsiPrivatize(privatizeData) {
    var privateDist = new unmanaged Hashed(idxType, dist.mapper, dist);
    var c = new unmanaged UserMapAssocDom(idxType=idxType, mapperType=mapperType, dist=privateDist);
    c.locDoms = locDoms;
    return c;
  }
  proc dsiReprivatize(other, privatizeData) {
    locDoms = other.locDoms;
  }

  override proc dsiDestroyDom() {
    coforall localeIdx in dist.targetLocDom do
      on dist.targetLocales(localeIdx) do
        delete locDoms(localeIdx);
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
  const wholeDom: unmanaged UserMapAssocDom(idxType, mapperType, parSafe=true);


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

  proc contains(i: idxType) {
    return myInds.contains(i);
  }

  proc clear() {
    myInds.clear();
  }

  proc capacity(numKeys:int) {
    myInds.requestCapacity(numKeys);
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
  proc writeThis(x) throws {
    x.write(myInds);
  }


  // INTERNAL INTERFACE:

  //
  // queries for this locale's number of indices, low, and high bounds
  //
  // TODO: I believe these are only used by the random number generator
  // in stream -- will they always be required once that is rewritten?
  //
  proc size {
    return myInds.size;
  }
}


//
// the global array class
//
class UserMapAssocArr: AbsBaseArr {
  // GENERICS:

  //
  // The index type of the domain
  //
  type idxType;
  type mapperType;

  // LINKAGE:

  //
  // LEFT: the global domain descriptor for this array
  //
  var dom: unmanaged UserMapAssocDom(idxType, mapperType, parSafe=true);

  //
  // DOWN: an array of local array classes
  var locArrs: [dom.dist.targetLocDom] unmanaged LocUserMapAssocArr(idxType, mapperType, eltType)?;
  //var locAssocDoms: domain(BaseAssociativeDom);
  //var locArrsByAssoc: [locAssocDoms] LocUserMapAssocArr(idxType, mapperType, eltType);

  var pid: int = -1; // privatized object id

  override proc dsiGetBaseDom() return dom;

  proc setup() {
    coforall localeIdx in dom.dist.targetLocDom do
      on dom.dist.targetLocales(localeIdx) do
        locArrs(localeIdx) = new unmanaged LocUserMapAssocArr(idxType, mapperType, eltType, dom.locDoms(localeIdx)!);
    for localeIdx in dom.dist.targetLocDom {
      var locDomImpl = dom.locDoms(localeIdx)!.myInds._value;
      //locAssocDoms += locDomImpl;
      //locArrsByAssoc[locDomImpl] = locArrs(localeIdx);
    }
  }

  override proc dsiDestroyArr() {
    coforall localeIdx in dom.dist.targetLocDom do
      on dom.dist.targetLocales(localeIdx) do
        delete locArrs(localeIdx);
  }

  proc dsiSupportsPrivatization() param return true;
  proc dsiGetPrivatizeData() return 0;
  proc dsiPrivatize(privatizeData) {
    var privdom = chpl_getPrivatizedCopy(dom.type, dom.pid);
    var c = new unmanaged UserMapAssocArr(idxType=idxType, mapperType=mapperType, eltType=eltType, dom=privdom);
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
    const locArr = locArrs[localeIndex]!;
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
    const locArr = locArrs[localeIndex]!;
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
    const locArr = locArrs[localeIndex]!;
    if locArr.locale == here {
      local {
        return locArr[i];
      }
    }
    return locArr[i];
  }

  inline proc dsiLocalAccess(i) ref {
    const localeIndex = dom.dist.indexToLocaleIndex(i);
    const locArr = locArrs[localeIndex]!;
    return locArr[i];
  }

  inline proc dsiLocalAccess(i)
  where shouldReturnRvalueByValue(eltType) {
    const localeIndex = dom.dist.indexToLocaleIndex(i);
    const locArr = locArrs[localeIndex]!;
    return locArr[i];
  }

  inline proc dsiLocalAccess(i) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    const localeIndex = dom.dist.indexToLocaleIndex(i);
    const locArr = locArrs[localeIndex]!;
    return locArr[i];
  }

  proc dsiTargetLocales() {
    return dom.dist.targetLocales;
  }

  proc dsiHasSingleLocalSubdomain() param return false;

  iter dsiLocalSubdomains(loc: locale) {
    for (idx,l) in zip(dom.dist.targetLocDom, dom.dist.targetLocales) {
      if l == loc {
        yield dom.locDoms[idx]!.myInds;
      }
    }
  }

  //
  // sequential iterator over the array's elements
  //
  iter these() ref {
    for loc in dom.dist.targetLocDom {
      // TODO: May want to do something like:
      // on this do
      // But can't currently have yields in on clauses
      for elem in locArrs(loc)!.myElems {
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

    var locArr = locArrs[localeIndex]!;

    // forward to locArr
    for i in locArr.myElems.these(tag, locFollowThis) do
      yield i;
  }

  iter these(param tag: iterKind) ref where tag == iterKind.standalone {
    coforall locArr in locArrs do on locArr {
      // Forward to associative array standalone iterator
      for i in locArr!.myElems.these(tag) {
        yield i;
      }
    }
  }


  //
  // how to print out the whole array, sequentially
  //
  proc dsiSerialWrite(x) {
    use IO;

    var first = true;
    for locArr in locArrs {
      if locArr!.size {
        if first {
          first = false;
        } else {
          x <~> " ";
        }
      }
      x <~> locArr;
      try! stdout.flush();
    }
  }

  override proc dsiDisplayRepresentation() {

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
  const locDom: unmanaged LocUserMapAssocDom(idxType, mapperType);


  // STATE:

  //
  // the block of local array data
  //
  pragma "local field" pragma "unsafe" // initialized separately
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
  proc writeThis(x) throws {
    // May want to do something like the following:
    //      on loc {
    // but it causes deadlock -- see writeThisUsingOn.chpl
    x.write(myElems);
  }

  //
  // query for the number of local array elements
  //
  proc size {
    return myElems.size;
  }

  // INTERNAL INTERFACE:

  proc owns(x) {
    //
    // TODO: When this is multidimensional need to do a reduction or something:
    //
    return locDom.myInds.dim(1).boundsCheck(x.dim(1));
  }
}


