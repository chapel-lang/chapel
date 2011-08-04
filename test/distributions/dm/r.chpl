// One-dimensional distributions for use with Dimensional.

use d;


/////////////////////////////////////////////////////////////////////////////
// 1-d distribution - replicated

class vdist {
  // REQ over how many locales
  // todo: can the Dimensional do without this one?
  var numLocales: int;

  // locale ID in our dimension of the locale this instance is on
  var localLocID = invalidLocID;
  var localLocIDlegit = false;
}

class vdom {
  // REQ the parameters of our dimension of the domain being created
  type idxType;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);
//todo-remove?  proc domainT type return domain(1, idxType, stridable);

  // our range
  var wholeR: rangeT;

  // locale ID in our dimension of the locale this instance is on
  var localLocID = invalidLocID;
  var localLocIDlegit = false;
}

class vlocdom {
  // our copy of wholeR
  var locWholeR;
}


/////////// privatization - start

// REQ does this class support privatization?
proc vdist.dsiSupportsPrivatization1d() param return true;

// REQ if privatization is supported - same purpose as dsiGetPrivatizeData()
proc vdist.dsiGetPrivatizeData1d() {
  return tuple(numLocales);
}

// REQ if privatization is supported - same purpose as dsiPrivatize()
proc vdist.dsiPrivatize1d(privatizeData) {
  return new vdist(numLocales = privatizeData(1));
}

// REQ does this class need -- and provide -- the localLocID?
// dsiStoreLocalLocID1d() will be invoked on privatized copies
// only when dsiSupportsPrivatization1d is true (obviously).
proc vdist.dsiUsesLocalLocID1d() param return true;

// REQ if dsiUsesLocalLocID1d: store the localLocID
// If 'legit' is false, this is a privatized copy on a locale
// that's not among our Dimensional distribution's target locales.
proc vdist.dsiStoreLocalLocID1d(localLocID: locIdT, legit: bool) {
  // no big deal, but currently we intend to update this just once
  assert(this.localLocID == invalidLocID);
  this.localLocID = localLocID;
  this.localLocIDlegit = legit;
}

// REQ if dsiUsesLocalLocID1d: retrieve the localLocID and its legitimacy
proc vdist.dsiGetLocalLocID1d(): (locIdT, bool) {
  assert(this.localLocID != invalidLocID);
  return (this.localLocID, this.localLocIDlegit);
}

// REQ does this class support privatization?
proc vdom.dsiSupportsPrivatization1d() param return true;

// REQ if privatization is supported - same purpose as dsiGetPrivatizeData()
proc vdom.dsiGetPrivatizeData1d() {
  return 0; // no data
}

// REQ if privatization is supported - same purpose as dsiPrivatize()
// 'privDist' is the corresponding 1-d distribution descriptor,
// privatized (if it supports privatization).
proc vdom.dsiPrivatize1d(privDist, privatizeData) {
  return new vdom(idxType   = this.idxType,
                  stridable = this.stridable);
}

// REQ if privatization is supported - same purpose as dsiGetReprivatizeData()
proc vdom.dsiGetReprivatizeData1d() {
  return tuple(wholeR);
}

// REQ if privatization is supported - same purpose as dsiReprivatize()
proc vdom.dsiReprivatize1d(other, reprivatizeData) {
  assert(other.idxType   == this.idxType,
         other.stridable == this.stridable);

  this.wholeR = reprivatizeData(1);
}

// REQ does this class need the localLocID?
// (See also vdist.dsiUsesLocalLocID1d.)
proc vdom.dsiUsesLocalLocID1d() param return true;

// REQ if dsiUsesLocalLocID1d: store the localLocID
proc vdom.dsiStoreLocalLocID1d((localLocID, legit): (locIdT, bool)) {
  // no big deal, but currently we intend to update this just once
  assert(this.localLocID == invalidLocID);
  this.localLocID = localLocID;
  this.localLocIDlegit = legit;
}

// REQ if dsiUsesLocalLocID1d: retrieve the localLocID
proc vdom.dsiGetLocalLocID1d(): (locIdT, bool) {
  assert(this.localLocID != invalidLocID);
  return (this.localLocID, this.localLocIDlegit);
}

// REQ does a local domain descriptor use a pointer
// to the privatized global domain descriptor?
// Consulted only when dsiSupportsPrivatization1d is true.
proc vdom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

// REQ if dsiLocalDescUsesPrivatizedGlobalDesc1d: store the pointer to
// the privatized copy of the global descriptor that is on vlocdom's locale.
// Note: this is invoked only while privatizing the 1-d global descriptor.
// In particular, this is not invoked when the local descriptors are
// initially created.
// todo: we already pass globDD to all methods on vlocdom, so probably
// we do not need this
//
proc vlocdom.dsiStoreLocalDescToPrivatizedGlobalDesc1d(privGlobDesc) {
  compilerError("should not need dsiStoreLocalDescToPrivatizedGlobalDesc1d");
  // e.g.: this.privGlobDesc = privGlobDesc;
}

/////////// privatization - end


// REQ create a 1-d global domain descriptor for dsiNewRectangularDom()
// where our dimension is a range(idxType, bounded, stridable)
proc vdist.dsiNewRectangularDom1d(type idxType, param stridable: bool) {
  return new vdom(idxType, stridable);
}

// REQ is this a replicated distribution?
proc vdom.dsiIsReplicated1d() param return true;

// REQ produce a local domain descriptor for our dimension
// on a locale for the given locale index locId, for dsiNewRectangularDom().
// Note: may be invoked multiple times/locales with the same locId.
// - one for each point in the other dimension(s).
proc vdom.dsiNewLocalDom1d(locId: locIdT) {
  return new vlocdom(wholeR);
}

// REQ given our dimension of the array index, on which locale is it located?
proc vdist.dsiIndexToLocale1d(indexx): locIdT {
  return localLocID;
}

// REQ update our data structures as needed upon dsiSetIndices()
proc vdom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// REQ update our local data structures as needed upon dsiSetIndices();
// return a range of "storage indices" to be stored locally.
//
// Each "storage index" corresponds to some "user index" (i.e.
// an index used to index into the array mapped with the Dimensional
// distribution) that is allocated on the receiver's locale.
//
// The returned range indicates *how many* of user indices are stored locally.
// It does NOT indicate *which* user indices those are. I.e. this range
// may contain exactly the user indices that are stored locally - or not
// (in which case dsiAccess1d() et al. will need to map each user index
// to an index in the range this returns).
//
// 'globDD' is the global domain descriptor (a 'vdom', in this case).
//
proc vlocdom.dsiSetLocalIndices1d(globDD, locId: locIdT): globDD.rangeT {
  locWholeR = globDD.wholeR;
  return locWholeR;
}

// REQ given one dimension of the array index, return
// (locale id, storage index on that locale) for that index.
// The storage index must have the type DimensionalDom.stoIndexT.
proc vdom.dsiAccess1d(indexx: idxType): (locIdT, idxType) {
  // todo: provide access from non-"legit" locales;
  // maybe to locId=0?
  assert(localLocIDlegit);
  return (localLocID, indexx);
}

// REQ yield the densified range to be used in the leader iterator
// on this descriptor's locale, when there is only a single task.
iter vlocdom.dsiMyDensifiedRangeForSingleTask1d(globDD) {
  yield 0:globDD.idxType..#locWholeR.length;
}

// REQ whether this distribution can handle only a single task per locale.
proc vdom.dsiSingleTaskPerLocaleOnly1d() param return true;

// REQ unless dsiSingleTaskPerLocaleOnly() is true.
// Yield the densified range to be used in the leader iterator
// on this descriptor's locale for task 'taskid' of 0..#numTasks.
// Note: should be densified w.r.t. the entire global range in this dimension.
proc vlocdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int) {
  const (startIx, endIx) = _computeChunkStartEnd(locWholeR.length,
                                                 numTasks, taskid+1);
  return startIx - 1 .. endIx - 1;
}

// REQ for iterating over array elements.
// Yield tuples (locId, iterable) s.t. the following results in
// traversing all array elements in order:
//   for (l,itr) in dsiSerialArrayIterator1d() do
//    for ix in itr do
//      yield (local storage on locale l)[ix]
iter vdom.dsiSerialArrayIterator1d() {
  assert(localLocIDlegit);
  yield (localLocID, wholeR);
}

// REQ for array follower iterator.
//
// Yield pairs (locId, storageOffset) that traverse storage for
// the array elements corresponding to the indices given by:
//   unDensify(denseRange, wholeRangeInMyDimension)
// preserving the order in which those indices occur.
//
// This is similar to dsiSerialArrayIterator1d(), except
// * only the indicated elements must be traversed, in the indicated order;
// * one storage offset is yielded each time.
//
// The latter is chosen to simplify dsiFollowerArrayIterator1d implementations.
// It also eliminates one loop nest per dimension in DimensionalArr follower.
//
iter vdom.dsiFollowerArrayIterator1d(denseRange): (locIdT, idxType) {
  // The dense indices are just fine here.
  for i in unDensify(denseRange, wholeR) do
    yield (localLocID, i);
}


/////////////////////////////////////////////////////////////////////////////
// 1-d distribution - block

class sdist {
  const numLocales: int;

  // the type of bbStart, bbLength
  // (also (ab)used as the idxType of the domains created over this dist.)
  // (todo - straighten that out)
  type idxType;

  // the .low and .length of BlockDist's 'boundingBox' for our dimension
  const bbStart: idxType;
  const bbLength: idxType;
}

class sdom {
  type idxType;
  param stridable: bool;

  // convenience
  proc rangeT type  return range(idxType, BoundedRangeType.bounded, stridable);

  // our range
  var wholeR: rangeT;

  // privatized distribution descriptor
  const pdist;
}

class slocdom {
  const myLocId: locIdT; // presently unused
  var myRange;
}


/////////// privatization - start

proc sdist.dsiSupportsPrivatization1d() param return true;

proc sdist.dsiGetPrivatizeData1d() {
  return (numLocales, bbStart, bbLength);
}

proc sdist.dsiPrivatize1d(privatizeData) {
  return new sdist(privatizeData, this.idxType);
}

// constructor for privatization
proc sdist.sdist(privatizeData, type idxType) {
  numLocales = privatizeData(1);
  bbStart = privatizeData(2);
  bbLength = privatizeData(3);
}

proc sdist.dsiUsesLocalLocID1d() param return false;

proc sdom.dsiSupportsPrivatization1d() param return true;

proc sdom.dsiGetPrivatizeData1d() {
  return 0;  // no data
}

proc sdom.dsiPrivatize1d(privDist, privatizeData) {
  assert(privDist.locale == here); // sanity check
  return new sdom(idxType   = this.idxType,
                  stridable = this.stridable,
                  pdist     = privDist);
}

proc sdom.dsiGetReprivatizeData1d() {
  return tuple(wholeR);
}

proc sdom.dsiReprivatize1d(other, reprivatizeData) {
  assert(other.idxType   == this.idxType,
         other.stridable == this.stridable);

  this.wholeR = reprivatizeData(1);
}

proc sdom.dsiUsesLocalLocID1d() param return false;

proc sdom.dsiLocalDescUsesPrivatizedGlobalDesc1d() param return false;

/////////// privatization - end


// Constructor. idxType is inferred from the 'bbLow' argument
// (alternative: default to 'int' instead).
proc sdist.sdist(nLocales, bbLow, bbHigh, type idxType = bbLow.type) {
  if !(bbLow <= bbHigh) then halt("'sdist' distribution must have a non-empty bounding box between bbLow and bbHigh; got ", bbLow, " .. ", bbHigh);
  assert(nLocales > 0); // so we can cast it to any int type
  this.numLocales = nLocales;
  this.bbStart = bbLow;
  this.bbLength = (bbHigh - bbLow + 1):idxType;
  assert(this.bbLength > 0);
}

proc sdist.dsiNewRectangularDom1d(type idxType, param stridable: bool) {
  if idxType != this.idxType then
    compilerError("The index type ", typeToString(idxType),
                  " does not match the index type ",typeToString(this.idxType),
                  " of the 'sdist' 1-d distribution");
  return new sdom(idxType = idxType, stridable = stridable, pdist = this);
}

proc sdom.dsiIsReplicated1d() param return false;

proc sdom.dsiNewLocalDom1d(locId: locIdT) {
  var defaultVal: rangeT;
  return new slocdom(myLocId = locId, myRange = defaultVal);
}

proc sdist.dsiIndexToLocale1d(indexx): locIdT {
  if indexx <= bbStart then
    return 0;

  // (numLocales-1) is the answer when
  //   indexx >= bbStart + (numLocales-1) * (bbLength+1) / numLocales
  // whose r.h.s. could be stored in 'sdist', trading memory for computation.
  // We simplify the condition to 'indexx >= bbStart + bbLength'.
  // (Note: (bbStart + bbLength - 1) will not always map to (numLocales - 1)).
  // If the condition doesn't hold, the formula gives the answer.
  // (Compared to the formula in Block.targetLocsIdx, we run comparisons
  // *before* multiplying and dividing.)
  //
  if indexx >= bbStart + bbLength then
    return numLocales - 1;

  // need to think what to do if numLocales is not int
  type adjT =
    if numLocales.type == int && idxType == uint(64)
    then uint(64)
    else numLocales.type;

  const result = (indexx - bbStart) * numLocales:adjT / bbLength;
  assert(0 <= result && result < numLocales:adjT);
  return result:locIdT;
}

proc sdom.dsiSetIndices1d(rangeArg: rangeT): void {
  wholeR = rangeArg;
}

// Helper: compute the chunk to be stored on locID
proc sdom._dsiComputeMyRange(locId): rangeT {
  const dom = this;
  const dist = dom.pdist;
  // see LocBlock.LocBlock()
  const (blo, bhi) = _computeBlock(dist.bbLength, dist.numLocales,
           locId, max(dom.idxType), min(dom.idxType), dist.bbStart);
  const myChunk = [blo..bhi];
  // see Block.getChunk()
  const chunk = myChunk(dom.wholeR);
  return chunk.dim(1);
}

proc slocdom.dsiSetLocalIndices1d(globDD, locId): globDD.rangeT {
  // todo: create local *distribution* descriptors for sdist, and
  // store myChunk there, like it's done in BlockDist?

  myRange = globDD._dsiComputeMyRange(locId);
  return myRange;
}

proc sdom.dsiAccess1d(indexx: idxType): (locIdT, idxType) {
  return (pdist.dsiIndexToLocale1d(indexx), indexx);
}

iter slocdom.dsiMyDensifiedRangeForSingleTask1d(globDD) {
  const locRange = densify(myRange, globDD.wholeR, userErrors=false);
  yield locRange;
}

proc sdom.dsiSingleTaskPerLocaleOnly1d() param return false;

proc slocdom.dsiMyDensifiedRangeForTaskID1d(globDD, taskid:int, numTasks:int) {
  const locRange = densify(myRange, globDD.wholeR, userErrors=false);
  // Copied straight from BlockDom leader - replace locBlock(parDim)->locRange.
  const (lo, hi) = _computeBlock(locRange.length, numTasks, taskid,
                                 locRange.high, locRange.low, locRange.low);

  // If this can occasionally be an empty range, add a check to Dimensional
  // to not yield anything in such a case.
  return lo..hi;
}

iter sdom.dsiSerialArrayIterator1d() {
  // The Block distribution assigns indices to locales contiguously and
  // so that (i1<i2) => (locId1<=locId2). This is defined by the domain map.
  // As we traverse the range (i.e. this.wholeR), locIds will increase
  // if the stride is positive, and decrease otherwise.
  // On a single locale, slocdom.myRange (aka this._dsiComputeMyRange())
  // already happens to reflect the desired direction.

  for locId in (0..#pdist.numLocales) by sgn(wholeR.stride) {
    // We do not go to slocdom for myRange - see above for rationale.
    const localRange = _dsiComputeMyRange(locId);
//writeln("sdom.dsiSerialArrayIterator1d  ", locId, ", ", localRange);
    yield (locId, localRange);
  }
}

iter sdom.dsiFollowerArrayIterator1d(denseRange): (locIdT, idxType) {
  const undensRange = unDensify(denseRange, wholeR);
//writeln("sdom.dsiFollowerArrayIterator1d  undensRange ", undensRange);

  const heuristic_weight_rangepoint = 2; // measure of work in (a) below
  const heuristic_weight_locale = 3;     // measure of work in (b)

  if heuristic_weight_locale * undensRange.stride >
      heuristic_weight_rangepoint * pdist.bbLength / pdist.numLocales
  {
//writeln("sdom.dsiFollowerArrayIterator1d case A");
    // (a) no benefit to factor out computations for a particular locale
    for ix in undensRange do
      yield dsiAccess1d(ix);

  } else {
    // (b) per-locale computation is amortized over enough undensRange indices
    const lowLocId = pdist.dsiIndexToLocale1d(undensRange.alignedLow);
    const highLocId = pdist.dsiIndexToLocale1d(undensRange.alignedHigh);

    // check for a supposedly more common case of following our own leader
    if lowLocId == highLocId {
//writeln("sdom.dsiFollowerArrayIterator1d case B1  lowLocId ", lowLocId);
      for i in undensRange do
        yield (lowLocId, i);

    } else {
      assert(lowLocId < highLocId);
      for locId in lowLocId .. highLocId by sgn(undensRange.stride) {
        const thisLocaleRange = _dsiComputeMyRange(locId);
        const localRangeToFollow = undensRange(thisLocaleRange);
//writeln("sdom.dsiFollowerArrayIterator1d case B2  locId ", locId, "  localRangeToFollow ", localRangeToFollow);
        for i in localRangeToFollow do
          yield (locId, i);
      }
    }
  }
}
