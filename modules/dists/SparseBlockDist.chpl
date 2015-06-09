//
// The SparseBlock distribution is defined with six classes:
//
//   SparseBlock       : distribution class
//   SparseBlockDom    : domain class
//   SparseBlockArr    : array class
//   LocSparseBlock    : local distribution class (per-locale instances)
//   LocSparseBlockDom : local domain class (per-locale instances)
//   LocSparseBlockArr : local array class (per-locale instances)
//
// When a distribution, domain, or array class instance is created, a
// correponding local class instance is created on each locale that is
// mapped to by the distribution.
//

//
// TO DO List
//
// 1. refactor pid fields from distribution, domain, and array classes
//

use DSIUtil;
use ChapelUtil;
use BlockDist;

//
// These flags are used to output debug information and run extra
// checks when using SparseBlock.  Should these be promoted so that they can
// be used across all distributions?  This can be done by turning them
// into compiler flags or adding config parameters to the internal
// modules, perhaps called debugDists and checkDists.
//
config param debugSparseBlockDist = false;
config param debugSparseBlockDistBulkTransfer = false;

// This flag is used to enable bulk transfer when aliased arrays are
// involved.  Currently, aliased arrays are not eligible for the
// optimization due to a bug in bulk transfer for rank changed arrays
// in which the last (right-most) dimension is collapsed.  Disabling
// the optimization for all aliased arrays is very conservative, so
// we provide this flag to allow the user to override the decision,
// with the caveat that it will likely not work for the above case.
config const disableAliasedBulkTransfer = true;

config param sanityCheckDistribution = false;

//
// SparseBlock Distribution Class
//
//   The fields dataParTasksPerLocale, dataParIgnoreRunningTasks, and
//   dataParMinGranularity can be changed, but changes are
//   not reflected in privatized copies of this distribution.  Perhaps
//   this is a feature, not a bug?!
//
// rank : generic rank that must match the rank of domains and arrays
//        declared over this distribution
//
// idxType: generic index type that must match the index type of
//          domains and arrays declared over this distribution
//
// boundingBox: a non-distributed domain defining a bounding box used
//              to partition the space of all indices across the array
//              of target locales; the indices inside the bounding box
//              are partitioned "evenly" across the locales and
//              indices outside the bounding box are mapped to the
//              same locale as the nearest index inside the bounding
//              box
//
// targetLocDom: a non-distributed domain over which the array of
//               target locales and the array of local distribution
//               classes are defined
//
// targetLocales: a non-distributed array containing the target
//                locales to which this distribution partitions indices
//                and data
//
// locDist: a non-distributed array of local distribution classes
//
// dataParTasksPerLocale: an integer that specifies the number of tasks to
//                        use on each locale when iterating in parallel over
//                        a SparseBlock-distributed domain or array
//
// dataParIgnoreRunningTasks: a boolean what dictates whether the number of
//                            task use on each locale should be limited
//                            by the available parallelism
//
// dataParMinGranularity: the minimum required number of elements per
//                        task created
//
/*
class SparseBlock : BaseDist {
  param rank: int;
  type idxType = int;
  var boundingBox: domain(rank, idxType);
  var targetLocDom: domain(rank);
  var targetLocales: [targetLocDom] locale;
  var locDist: [targetLocDom] LocSparseBlock(rank, idxType);
  var dataParTasksPerLocale: int;
  var dataParIgnoreRunningTasks: bool;
  var dataParMinGranularity: int;
  var pid: int = -1; // privatized object id (this should be factored out)
}
*/

//
// Local SparseBlock Distribution Class
//
// rank : generic rank that matches SparseBlock.rank
// idxType: generic index type that matches SparseBlock.idxType
// myChunk: a non-distributed domain that defines this locale's indices
//
/*
class LocSparseBlock {
  param rank: int;
  type idxType;
  const myChunk: domain(rank, idxType);
}
*/

//
// SparseBlock Domain Class
//
// rank:      generic domain rank
// idxType:   generic domain index type
// stridable: generic domain stridable parameter
// dist:      reference to distribution class
// locDoms:   a non-distributed array of local domain classes
// whole:     a non-distributed domain that defines the domain's indices
//
class SparseBlockDom: BaseSparseDom {
  param rank: int;
  type idxType;
  param stridable: bool = false;  // TODO: remove default value eventually
  const dist: Block(rank, idxType);
  var parentDom;
  const whole: domain(rank=rank, idxType=idxType, stridable=stridable); 
  var locDoms: [dist.targetLocDom] LocSparseBlockDom(rank, idxType, stridable);
  var pid: int = -1; // privatized object id (this should be factored out)

  proc initialize() {
    setup();
    //    writeln("Exiting initialize");
  }

  proc setup() {
    //    writeln("In setup");
    if locDoms(dist.targetLocDom.low) == nil {
      coforall localeIdx in dist.targetLocDom do {
	on dist.targetLocales(localeIdx) do {
          //                    writeln("Setting up on ", here.id);
          //                    writeln("setting up on ", localeIdx, ", whole is: ", whole, ", chunk is: ", dist.getChunk(whole,localeIdx));
	  locDoms(localeIdx) = new LocSparseBlockDom(rank, idxType, stridable,
						     dist.getChunk(whole, localeIdx));
          //                    writeln("Back on ", here.id);
        }
      }
      //      writeln("Past coforall");
    } else {
      halt("Don't know how to reallocate yet");
      /*
      coforall localeIdx in dist.targetLocDom do {
	on dist.targetLocales(localeIdx) do
	  locDoms(localeIdx).mySparseBlock = dist.getChunk(whole, localeIdx);
      }
      */
    }
    //    writeln("Exiting setup()");
  }

  // TODO: For some reason I have to make all the methods for these classes primary
  // rather than secondary methods.  This doesn't seem right, but I couldn't boil
  // it down to a smaller test case in the time I spent on it.
  proc dsiAdd(ind: rank*idxType) {
    on dist.dsiIndexToLocale(ind) {
      locDoms[dist.targetLocsIdx(ind)].dsiAdd(ind);      
    }
  }

  proc dsiAdd(ind: idxType) where this.rank == 1 {
    dsiAdd((ind,));
  }

  //
  // output domain
  //
  proc dsiSerialWrite(f:Writer) {
    if (rank == 1) {
      f.write("{");
      for locdom in locDoms do {
        //	on locdom do {
	  if (locdom.dsiNumIndices > 1) {
	    locdom.dsiSerialWrite(f);
          }
          //	}
      }
      f.write("}");
    } else {
      compilerError("Can't write out multidimensional sparse distributed domains yet");
    }
  }


  //
  // how to allocate a new array over this domain
  //
  proc dsiBuildArray(type eltType) {
    var arr = new SparseBlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=this);
    arr.setup();
    return arr;
  }

  // stopgap to avoid accessing locDoms field (and returning an array)
  proc getLocDom(localeIdx) return locDoms(localeIdx);

  var dummy:idxType;

  iter these() {
    halt("Haven't implemented serial iteration yet");
    yield dummy;
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    coforall locDom in locDoms do on locDom {
      for followThis in locDom.mySparseBlock._value.these(tag) do
	yield followThis;
    }
  }

  //
  // TODO: Abstract the addition of low into a function?
  // Note relationship between this operation and the
  // order/position functions -- any chance for creating similar
  // support? (esp. given how frequent this seems likely to be?)
  //
  // TODO: Is there some clever way to invoke the leader/follower
  // iterator on the local blocks in here such that the per-core
  // parallelism is expressed at that level?  Seems like a nice
  // natural composition and might help with my fears about how
  // stencil communication will be done on a per-locale basis.
  //
  iter these(param tag: iterKind, followThis:(?,?,?)) where tag == iterKind.follower {
    for i in followThis(1).these(tag, followThis) do
      yield i;
  }
    
  proc dsiDims() return whole.dims();
  
  proc dsiMember(ind) {
    on parentDom.dist.idxToLocale(ind) {
      writeln("Need to add support for mapping locale to local domain");
    }
  }

  proc dsiClear() {
    coforall locDom in locDoms do
      on locDom do
	locDom.dsiClear();
  }

  proc dsiMyDist() return dist;


}

//
// Local SparseBlock Domain Class
//
// rank: generic domain rank
// idxType: generic domain index type
// stridable: generic domain stridable parameter
// mySparseBlock: a non-distributed domain that defines the local indices
//
class LocSparseBlockDom {
  param rank: int;
  type idxType;
  param stridable: bool;
  var parentDom: domain(rank, idxType, stridable);
  var mySparseBlock: sparse subdomain(parentDom);

  proc initialize() {
    //    writeln("On locale ", here.id, " LocSparseBlockDom = ", this);
  }
  
  proc dsiAdd(ind: rank*idxType) {
    mySparseBlock.add(ind);
  }

  proc dsiMember(ind: rank*idxType) {
    return mySparseBlock.member(ind);
  }

  proc dsiClear() {
    mySparseBlock.clear();
  }

  proc dsiSerialWrite(w: Writer) {
    mySparseBlock._value.dsiSerialWrite(w, printBrackets=false);
    // w.write(mySparseBlock); // works, but gets brackets printed out redundantly
    //    w <~> mySparseBlock;
  }

  proc dsiNumIndices {
    return mySparseBlock.numIndices;
  }
}

//
// SparseBlock Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// dom: reference to domain class
// locArr: a non-distributed array of local array classes
// myLocArr: optimized reference to here's local array class (or nil)
//
class SparseBlockArr: BaseArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  var dom; //: SparseBlockDom(rank, idxType, stridable);
  var locArr: [dom.dist.targetLocDom] LocSparseBlockArr(eltType, rank, idxType, stridable);
  var myLocArr: LocSparseBlockArr(eltType, rank, idxType, stridable);
  var pid: int = -1; // privatized object id (this should be factored out)

  proc setup() {
    var thisid = this.locale.id;
    coforall localeIdx in dom.dist.targetLocDom {
      on dom.dist.targetLocales(localeIdx) {
	const locDom = dom.getLocDom(localeIdx);
	locArr(localeIdx) = new LocSparseBlockArr(eltType, rank, idxType, stridable, locDom);
	if thisid == here.id then
	  myLocArr = locArr(localeIdx);
      }
    }
  }

  var dummy: eltType;

  iter these() ref {
    halt("Haven't implemented serial iteration yet");
    yield dummy;
  }

  //
  // TODO: Rewrite this to reuse more of the global domain iterator
  // logic?  (e.g., can we forward the forall to the global domain
  // somehow?
  //
  iter these(param tag: iterKind) where tag == iterKind.leader {
    for followThis in dom.these(tag) do
      yield followThis;
  }

  iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
    for i in followThis(1).these(tag, followThis) do
      yield locArr[here.id].dsiAccess(i);

    //    for e in myLocArr.myElems._value.these(followThis) do yield e;
    /*
    writeln("warning, yielding dummy");
    yield dummy;
    */
  }

  proc dsiAccess(i: rank*idxType) ref {
    //    writeln("In SparseBlockDist's dsiAccess, got i = ", i);
    //    local { // TODO: Turn back on once privatization is on
      if myLocArr != nil && myLocArr.locDom.dsiMember(i) {
        //        writeln("In local case");
        //        writeln("myLocArr.locDom = ", myLocArr.locDom);
	return myLocArr.dsiAccess(i);
        //      }
    }
      //      writeln("In general case, and finding that locale is ", dom.dist.targetLocsIdx(i));
    return locArr[dom.dist.targetLocsIdx(i)].dsiAccess(i);
  }

  proc dsiAccess(i: idxType...rank) ref
    return dsiAccess(i);


  proc dsiGetBaseDom() return dom;

}

//
// Local SparseBlock Array Class
//
// eltType: generic array element type
// rank: generic array rank
// idxType: generic array index type
// stridable: generic array stridable parameter
// locDom: reference to local domain class
// myElems: a non-distributed array of local elements
//
class LocSparseBlockArr {
  type eltType;
  param rank: int;
  type idxType;
  param stridable: bool;
  const locDom: LocSparseBlockDom(rank, idxType, stridable);
  var myElems: [locDom.mySparseBlock] eltType;

  proc dsiAccess(i) ref {
    //    writeln("In dsiAccess(", i, ") on ", here.id, " locDom = ", locDom, " mySparseBlock = ", locDom.mySparseBlock);
    return myElems[i];
  }

}

//
// SparseBlock constructor for clients of the SparseBlock distribution
//
/*
proc SparseBlock.SparseBlock(boundingBox: domain,
                targetLocales: [] locale = Locales,
                dataParTasksPerLocale=getDataParTasksPerLocale(),
                dataParIgnoreRunningTasks=getDataParIgnoreRunningTasks(),
                dataParMinGranularity=getDataParMinGranularity(),
                param rank = boundingBox.rank,
                type idxType = boundingBox.idxType) {
  if rank != boundingBox.rank then
    compilerError("specified SparseBlock rank != rank of specified bounding box");
  if idxType != boundingBox.idxType then
    compilerError("specified SparseBlock index type != index type of specified bounding box");

  this.boundingBox = boundingBox;

  setupTargetLocalesArray(targetLocDom, this.targetLocales, targetLocales);

  const boundingBoxDims = boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();
  coforall locid in targetLocDom do
    on this.targetLocales(locid) do
      locDist(locid) =  new LocSparseBlock(rank, idxType, locid, boundingBoxDims,
                                     targetLocDomDims);

  // NOTE: When these knobs stop using the global defaults, we will need
  // to add checks to make sure dataParTasksPerLocale<0 and
  // dataParMinGranularity<0
  this.dataParTasksPerLocale = if dataParTasksPerLocale==0 then here.numCores
                               else dataParTasksPerLocale;
  this.dataParIgnoreRunningTasks = dataParIgnoreRunningTasks;
  this.dataParMinGranularity = dataParMinGranularity;
  
  if debugSparseBlockDist {
    writeln("Creating new SparseBlock distribution:");
    dsiDisplayRepresentation();
  }
}

proc SparseBlock.dsiAssign(other: this.type) {
  coforall locid in targetLocDom do
    on targetLocales(locid) do
      delete locDist(locid);
  boundingBox = other.boundingBox;
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
  const boundingBoxDims = boundingBox.dims();
  const targetLocDomDims = targetLocDom.dims();
  coforall locid in targetLocDom do
    on targetLocales(locid) do
      locDist(locid) = new LocSparseBlock(rank, idxType, locid, boundingBoxDims,
                                    targetLocDomDims);
}

proc SparseBlock.dsiClone() {
  return new SparseBlock(boundingBox, targetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity);
}

proc SparseBlock.dsiDestroyDistClass() {
  coforall ld in locDist do {
    on ld do
      delete ld;
  }
}

proc SparseBlock.dsiDisplayRepresentation() {
  writeln("boundingBox = ", boundingBox);
  writeln("targetLocDom = ", targetLocDom);
  writeln("targetLocales = ", for tl in targetLocales do tl.id);
  writeln("dataParTasksPerLocale = ", dataParTasksPerLocale);
  writeln("dataParIgnoreRunningTasks = ", dataParIgnoreRunningTasks);
  writeln("dataParMinGranularity = ", dataParMinGranularity);
  for tli in targetLocDom do
    writeln("locDist[", tli, "].myChunk = ", locDist[tli].myChunk);
}

proc SparseBlock.dsiNewRectangularDom(param rank: int, type idxType,
                              param stridable: bool) {
  if idxType != this.idxType then
    compilerError("SparseBlock domain index type does not match distribution's");
  if rank != this.rank then
    compilerError("SparseBlock domain rank does not match distribution's");
  
  var dom = new SparseBlockDom(rank=rank, idxType=idxType, dist=this, stridable=stridable);
  dom.setup();
  if debugSparseBlockDist {
    writeln("Creating new SparseBlock domain:");
    dom.dsiDisplayRepresentation();
  }
  return dom;
}

//
// output distribution
//
proc SparseBlock.writeThis(x:Writer) {
  x.writeln("SparseBlock");
  x.writeln("-------");
  x.writeln("distributes: ", boundingBox);
  x.writeln("across locales: ", targetLocales);
  x.writeln("indexed via: ", targetLocDom);
  x.writeln("resulting in: ");
  for locid in targetLocDom do
    x.writeln("  [", locid, "] locale ", locDist(locid).locale.id, " owns chunk: ", locDist(locid).myChunk);
}

proc SparseBlock.dsiIndexToLocale(ind: idxType) where rank == 1 {
  return targetLocales(targetLocsIdx(ind));
}

proc SparseBlock.dsiIndexToLocale(ind: rank*idxType) where rank > 1 {
  return targetLocales(targetLocsIdx(ind));
}

//
// compute what chunk of inds is owned by a given locale -- assumes
// it's being called on the locale in question
//
proc SparseBlock.getChunk(inds, locid) {
  // use domain slicing to get the intersection between what the
  // locale owns and the domain's index set
  //
  // TODO: Should this be able to be written as myChunk[inds] ???
  //
  // TODO: Does using David's detupling trick work here?
  //
  const chunk = locDist(locid).myChunk((...inds.getIndices()));
  if sanityCheckDistribution then
    if chunk.numIndices > 0 {
      if targetLocsIdx(chunk.low) != locid then
        writeln("[", here.id, "] ", chunk.low, " is in my chunk but maps to ",
                targetLocsIdx(chunk.low));
      if targetLocsIdx(chunk.high) != locid then
        writeln("[", here.id, "] ", chunk.high, " is in my chunk but maps to ",
                targetLocsIdx(chunk.high));
    }
  return chunk;
}

//
// get the index into the targetLocales array for a given distributed index
//
proc SparseBlock.targetLocsIdx(ind: idxType) where rank == 1 {
  return targetLocsIdx(tuple(ind));
}

proc SparseBlock.targetLocsIdx(ind: rank*idxType) {
  var result: rank*int;
  for param i in 1..rank do
    result(i) = max(0, min((targetLocDom.dim(i).length-1):int,
                           (((ind(i) - boundingBox.dim(i).low) *
                             targetLocDom.dim(i).length:idxType) /
                            boundingBox.dim(i).length):int));
  return if rank == 1 then result(1) else result;
}

proc SparseBlock.dsiCreateReindexDist(newSpace, oldSpace) {
  proc anyStridable(space, param i=1) param
    return if i == space.size then space(i).stridable
           else space(i).stridable || anyStridable(space, i+1);

  // Should this error be in ChapelArray or not an error at all?
  if newSpace(1).idxType != oldSpace(1).idxType then
    compilerError("index type of reindex domain must match that of original domain");
  if anyStridable(newSpace) || anyStridable(oldSpace) then
    compilerWarning("reindexing stridable SparseBlock arrays is not yet fully supported");

  /* To shift the bounding box, we must perform the following operation for
   *  each dimension:
   *
   *   bbox(r).low - (oldSpace(r).low - newSpace(r).low)
   *   bbox(r).high - (oldSpace(r).low - newSpace(r).low)
   *
   * The following is guaranteed on entry:
   *
   *   oldSpace(r).low-newSpace(r).low = oldSpace(r).high-newSpace(r).high
   *
   * We need to be able to do this without going out of range of the index
   *  type.  The approach we take is to check if there is a way to perform
   *  the calculation without having any of the intermediate results go out
   *  of range.
   *
   *    newBbLow = bbLow - (oldLow - newLow)
   *    newBbLow = bbLow - oldLow + newLow
   *
   * Can be performed as:
   *
   *    t = oldLow-newLow;
   *    newBbLow = bbLow-t;
   * or
   *    t = bbLow-oldLow;
   *    newBbLow = t+newLow;
   * or
   *    t = bbLow+newLow;
   *    newBbLow = t-oldLow;
   *
   */
  proc adjustBound(bbound, oldBound, newBound) {
    var t: bbound.type;
    if safeSub(oldBound, newBound) {
      t = oldBound-newBound;
      if safeSub(bbound, t) {
        return (bbound-t, true);
      }
    }
    if safeSub(bbound, oldBound) {
      t = bbound-oldBound;
      if safeAdd(t, newBound) {
        return (t+newBound, true);
      }
    }
    if safeAdd(bbound, newBound) {
      t = bbound+newBound;
      if safeSub(t, oldBound) {
        return(t-oldBound, true);
      }
    }
    return (bbound, false);
  }

  var myNewBbox = boundingBox.dims();
  for param r in 1..rank {
    var oldLow = oldSpace(r).low;
    var newLow = newSpace(r).low;
    var oldHigh = oldSpace(r).high;
    var newHigh = newSpace(r).high;
    var valid: bool;
    if oldLow != newLow {
      (myNewBbox(r)._base._low,valid) = adjustBound(myNewBbox(r).low,oldLow,newLow);
      if !valid then // try with high
        (myNewBbox(r)._base._low,valid) = adjustBound(myNewBbox(r).low,oldHigh,newHigh);
      if !valid then
        halt("invalid reindex for SparseBlock: distribution bounding box (low) out of range in dimension ", r);

      (myNewBbox(r)._base._high,valid) = adjustBound(myNewBbox(r).high,oldHigh,newHigh);
      if !valid then
        (myNewBbox(r)._base._high,valid) = adjustBound(myNewBbox(r).high,oldLow,newLow);
      if !valid then // try with low
        halt("invalid reindex for SparseBlock: distribution bounding box (high) out of range in dimension ", r);
    }
  }
  var d = {(...myNewBbox)};
  var newDist = new SparseBlock(d, targetLocales, 
                          dataParTasksPerLocale, dataParIgnoreRunningTasks,
                          dataParMinGranularity);
  return newDist;
}

/*
proc LocSparseBlock.LocSparseBlock(param rank: int,
                      type idxType, 
                      locid, // the locale index from the target domain
                      boundingBox: rank*range(idxType),
                      targetLocBox: rank*range) {
  if rank == 1 {
    const lo = boundingBox(1).low;
    const hi = boundingBox(1).high;
    const numelems = hi - lo + 1;
    const numlocs = targetLocBox(1).length;
    const (blo, bhi) = _computeSparseBlock(numelems, numlocs, locid,
                                     max(idxType), min(idxType), lo);
    myChunk = {blo..bhi};
  } else {
    var tuple: rank*range(idxType);
    for param i in 1..rank {
      const lo = boundingBox(i).low;
      const hi = boundingBox(i).high;
      const numelems = hi - lo + 1;
      const numlocs = targetLocBox(i).length;
      const (blo, bhi) = _computeSparseBlock(numelems, numlocs, locid(i),
                                       max(idxType), min(idxType), lo);
      tuple(i) = blo..bhi;
    }
    myChunk = {(...tuple)};
  }
}
*/

proc SparseBlockDom.dsiNewSpsSubDom(parentDomVal) {
  return new SparseBlockDom(rank, idxType, dist, parentDomVal);
}

proc SparseBlockDom.dsiDisplayRepresentation() {
  writeln("whole = ", whole);
  for tli in dist.targetLocDom do
    writeln("locDoms[", tli, "].mySparseBlock = ", locDoms[tli].mySparseBlock);
}

proc SparseBlockDom.dsiDim(d: int) return whole.dim(d);


//
// Given a tuple of scalars of type t or range(t) match the shape but
// using types rangeType and scalarType e.g. the call:
// _matchArgsShape(range(int(32)), int(32), (1:int(64), 1:int(64)..5, 1:int(64)..5))
// returns the type: (int(32), range(int(32)), range(int(32)))
//
proc _matchArgsShape(type rangeType, type scalarType, args) type {
  proc tuple(type t ...) type return t;
  proc helper(param i: int) type {
    if i == args.size {
      if isCollapsedDimension(args(i)) then
        return tuple(scalarType);
      else
        return tuple(rangeType);
    } else {
      if isCollapsedDimension(args(i)) then
        return (scalarType, (... helper(i+1)));
      else
        return (rangeType, (... helper(i+1)));
    }
  }
  return helper(1);
}


proc SparseBlock.dsiCreateRankChangeDist(param newRank: int, args) {
  var collapsedDimLocs: rank*idxType;

  for param i in 1..rank {
    if isCollapsedDimension(args(i)) {
      collapsedDimLocs(i) = args(i);
    } else {
      collapsedDimLocs(i) = 0;
    }
  }
  const collapsedLocInd = targetLocsIdx(collapsedDimLocs);
  var collapsedBbox: _matchArgsShape(range(idxType=idxType), idxType, args);
  var collapsedLocs: _matchArgsShape(range(idxType=int), int, args);

  for param i in 1..rank {
    if isCollapsedDimension(args(i)) {
      // set indicies that are out of bounds to the bounding box low or high.
      collapsedBbox(i) = if args(i) < boundingBox.dim(i).low then boundingBox.dim(i).low else if args(i) > boundingBox.dim(i).high then boundingBox.dim(i).high else args(i);
      collapsedLocs(i) = collapsedLocInd(i);
    } else {
      collapsedBbox(i) = boundingBox.dim(i);
      collapsedLocs(i) = targetLocDom.dim(i);
    }
  }

  const newBbox = boundingBox[(...collapsedBbox)];
  const newTargetLocales = targetLocales((...collapsedLocs));
  return new SparseBlock(newBbox, newTargetLocales,
                   dataParTasksPerLocale, dataParIgnoreRunningTasks,
                   dataParMinGranularity);
}

proc SparseBlockDom.dsiLow return whole.low;
proc SparseBlockDom.dsiHigh return whole.high;
proc SparseBlockDom.dsiStride return whole.stride;

//
// INTERFACE NOTES: Could we make dsiSetIndices() for a rectangular
// domain take a domain rather than something else?
//
proc SparseBlockDom.dsiSetIndices(x: domain) {
  if x.rank != rank then
    compilerError("rank mismatch in domain assignment");
  if x._value.idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  whole = x;
  setup();
  if debugSparseBlockDist {
    writeln("Setting indices of SparseBlock domain:");
    dsiDisplayRepresentation();
  }
}

proc SparseBlockDom.dsiSetIndices(x) {
  if x.size != rank then
    compilerError("rank mismatch in domain assignment");
  if x(1).idxType != idxType then
    compilerError("index type mismatch in domain assignment");
  //
  // TODO: This seems weird:
  //
  whole.setIndices(x);
  setup();
  if debugSparseBlockDist {
    writeln("Setting indices of SparseBlock domain:");
    dsiDisplayRepresentation();
  }
}

proc SparseBlockDom.dsiGetIndices() {
  return whole.getIndices();
}

// dsiLocalSlice
proc SparseBlockDom.dsiLocalSlice(param stridable: bool, ranges) {
  return whole((...ranges));
}

proc SparseBlockDom.dsiIndexOrder(i) {
  return whole.indexOrder(i);
}

//
// build a new rectangular domain using the given range
//
proc SparseBlockDom.dsiBuildRectangularDom(param rank: int, type idxType,
                                   param stridable: bool,
                                   ranges: rank*range(idxType,
                                                      BoundedRangeType.bounded,
                                                      stridable)) {
  if idxType != dist.idxType then
    compilerError("SparseBlock domain index type does not match distribution's");
  if rank != dist.rank then
    compilerError("SparseBlock domain rank does not match distribution's");
  
  var dom = new SparseBlockDom(rank=rank, idxType=idxType,
                         dist=dist, stridable=stridable);
  dom.dsiSetIndices(ranges);
  return dom;
}

//
// Added as a performance stopgap to avoid returning a domain
//
proc LocSparseBlockDom.member(i) return mySparseBlock.member(i);

proc SparseBlockArr.dsiDisplayRepresentation() {
  for tli in dom.dist.targetLocDom {
    writeln("locArr[", tli, "].myElems = ", for e in locArr[tli].myElems do e);
  }
}

inline proc _remoteAccessData.getDataIndex(param stridable, ind: rank*idxType) {
  // modified from DefaultRectangularArr below
  var sum = origin;
  if stridable {
    for param i in 1..rank do
      sum += (ind(i) - off(i)) * blk(i) / abs(str(i)):idxType;
  } else {
    for param i in 1..rank do
      sum += ind(i) * blk(i);
    sum -= factoredOffs;
  }
  return sum;
}

//
// output array
//
proc SparseBlockArr.dsiSerialWrite(f: Writer) {
  if dom.dsiNumIndices == 0 then return;
  var i : rank*idxType;
  for dim in 1..rank do
    i(dim) = dom.dsiDim(dim).low;
  label next while true {
    f.write(dsiAccess(i));
    if i(rank) <= (dom.dsiDim(rank).high - dom.dsiDim(rank).stride:idxType) {
      f.write(" ");
      i(rank) += dom.dsiDim(rank).stride:idxType;
    } else {
      for dim in 1..rank-1 by -1 {
        if i(dim) <= (dom.dsiDim(dim).high - dom.dsiDim(dim).stride:idxType) {
          i(dim) += dom.dsiDim(dim).stride:idxType;
          for dim2 in dim+1..rank {
            f.writeln();
            i(dim2) = dom.dsiDim(dim2).low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

proc SparseBlockArr.dsiSlice(d: SparseBlockDom) {
  var alias = new SparseBlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, dom=d, pid=pid);
  var thisid = this.locale.id;
  coforall i in d.dist.targetLocDom {
    on d.dist.targetLocales(i) {
      alias.locArr[i] = new LocSparseBlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=d.stridable, locDom=d.locDoms[i], myElems=>locArr[i].myElems[d.locDoms[i].mySparseBlock]);
      if thisid == here.id then
        alias.myLocArr = alias.locArr[i];
    }
  }
  return alias;
}

proc SparseBlockArr.dsiLocalSlice(ranges) {
  var low: rank*idxType;
  for param i in 1..rank {
    low(i) = ranges(i).low;
  }
  var A => locArr(dom.dist.targetLocsIdx(low)).myElems((...ranges));
  return A;
}

proc _extendTuple(type t, idx: _tuple, args) {
  var tup: args.size*t;
  var j: int = 1;
  
  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idx(j);
      j += 1;
    }
  }
  return tup;
}

proc _extendTuple(type t, idx, args) {
  var tup: args.size*t;
  var idxTup = tuple(idx);
  var j: int = 1;

  for param i in 1..args.size {
    if isCollapsedDimension(args(i)) then
      tup(i) = args(i);
    else {
      tup(i) = idxTup(j);
      j += 1;
    }
  }
  return tup;
}


proc SparseBlockArr.dsiRankChange(d, param newRank: int, param stridable: bool, args) {
  var alias = new SparseBlockArr(eltType=eltType, rank=newRank, idxType=idxType, stridable=stridable, dom=d);
  var thisid = this.locale.id;
  coforall ind in d.dist.targetLocDom {
    on d.dist.targetLocales(ind) {
      const locDom = d.getLocDom(ind);
      // locSlice is a tuple of ranges and scalars. It will match the basic
      // shape of the args argument. 
      var locSlice: _matchArgsShape(range(idxType=idxType, stridable=stridable), idxType, args);
      // collapsedDims stores the value any collapsed dimension is down to.
      // For any non-collapsed dimension, that position is ignored.
      // This tuple is then passed to the targetLocsIdx function to build up a
      // partial index into this.targetLocDom with correct values set for all
      // collapsed dimensions. The rest of the dimensions get their values from
      // ind - an index into the new rank changed targetLocDom.
      var collapsedDims: rank*idxType;
      var locArrInd: rank*int;

      var j = 1;
      for param i in 1..args.size {
        if isCollapsedDimension(args(i)) {
          locSlice(i) = args(i);
          collapsedDims(i) = args(i);
        } else {
          locSlice(i) = locDom.mySparseBlock.dim(j)(args(i));
          j += 1;
        }
      }
      locArrInd = dom.dist.targetLocsIdx(collapsedDims);
      j = 1;
      // Now that the locArrInd values are known for the collapsed dimensions
      // Pull the rest of the dimensions values from ind
      for param i in 1..args.size {
        if !isCollapsedDimension(args(i)) {
          if newRank > 1 then
            locArrInd(i) = ind(j);
          else
            locArrInd(i) = ind;
          j += 1;
        }
      }

      alias.locArr[ind] =
        new LocSparseBlockArr(eltType=eltType, rank=newRank, idxType=d.idxType,
                        stridable=d.stridable, locDom=locDom,
                        myElems=>locArr[(...locArrInd)].myElems[(...locSlice)]);

      if thisid == here.id then
        alias.myLocArr = alias.locArr[ind];
    }
  }
  return alias;
}

proc SparseBlockArr.dsiReindex(d: SparseBlockDom) {
  var alias = new SparseBlockArr(eltType=eltType, rank=d.rank, idxType=d.idxType,
                           stridable=d.stridable, dom=d);
  const sameDom = d==dom;

  var thisid = this.locale.id;
  coforall i in d.dist.targetLocDom {
    on d.dist.targetLocales(i) {
      const locDom = d.getLocDom(i);
      var locAlias: [locDom.mySparseBlock] => locArr[i].myElems;
      alias.locArr[i] = new LocSparseBlockArr(eltType=eltType,
                                        rank=rank, idxType=d.idxType,
                                        stridable=d.stridable,
                                        locDom=locDom,
                                        myElems=>locAlias);
      if thisid == here.id then
        alias.myLocArr = alias.locArr[i];
      }
    }
  }

  return alias;
}

proc SparseBlockArr.dsiReallocate(d: domain) {
  //
  // For the default rectangular array, this function changes the data
  // vector in the array class so that it is setup once the default
  // rectangular domain is changed.  For this distributed array class,
  // we don't need to do anything, because changing the domain will
  // change the domain in the local array class which will change the
  // data in the local array class.  This will only work if the domain
  // we are reallocating to has the same distribution, but domain
  // assignment is defined so that only the indices are transferred.
  // The distribution remains unchanged.
  //
}

proc SparseBlockArr.dsiPostReallocate() {
  // Call this *after* the domain has been reallocated
}

//
// the accessor for the local array -- assumes the index is local
//
proc LocSparseBlockArr.this(i) ref {
  return myElems(i);
}

//
// Privatization
//
proc SparseBlock.SparseBlock(other: SparseBlock, privateData,
                param rank = other.rank,
                type idxType = other.idxType) {
  boundingBox = {(...privateData(1))};
  targetLocDom = {(...privateData(2))};
  dataParTasksPerLocale = privateData(3);
  dataParIgnoreRunningTasks = privateData(4);
  dataParMinGranularity = privateData(5);

  for i in targetLocDom {
    targetLocales(i) = other.targetLocales(i);
    locDist(i) = other.locDist(i);
  }
}

// TODO: Will want to privatize eventually

proc SparseBlock.dsiSupportsPrivatization() param return false;

proc SparseBlock.dsiGetPrivatizeData() {
  writeln("In getPrivatizeData");
  return (boundingBox.dims(), targetLocDom.dims(),
          dataParTasksPerLocale, dataParIgnoreRunningTasks, dataParMinGranularity);
}

proc SparseBlock.dsiPrivatize(privatizeData) {
  writeln("In dsiPrivatize");
  return new SparseBlock(this, privatizeData);
}

proc SparseBlock.dsiGetReprivatizeData() { writeln("In dsiGetReprivatizeData"); return boundingBox.dims(); }

proc SparseBlock.dsiReprivatize(other, reprivatizeData) {
  writeln("In dsiReprivatize");
  boundingBox = {(...reprivatizeData)};
  targetLocDom = other.targetLocDom;
  targetLocales = other.targetLocales;
  locDist = other.locDist;
  dataParTasksPerLocale = other.dataParTasksPerLocale;
  dataParIgnoreRunningTasks = other.dataParIgnoreRunningTasks;
  dataParMinGranularity = other.dataParMinGranularity;
}
*/

proc SparseBlockDom.dsiSupportsPrivatization() param return false;

proc SparseBlockDom.dsiGetPrivatizeData() return (dist.pid, whole.dims());

proc SparseBlockDom.dsiPrivatize(privatizeData) {
  var privdist = chpl_getPrivatizedCopy(dist.type, privatizeData(1));
  var c = new SparseBlockDom(rank=rank, idxType=idxType, stridable=stridable, dist=privdist, parentDom=parentDom);
  for i in c.dist.targetLocDom do
    c.locDoms(i) = locDoms(i);
  c.whole = {(...privatizeData(2))};
  return c;
}

proc SparseBlockDom.dsiGetReprivatizeData() return whole.dims();

proc SparseBlockDom.dsiReprivatize(other, reprivatizeData) {
  for i in dist.targetLocDom do
    locDoms(i) = other.locDoms(i);
  whole = {(...reprivatizeData)};
}

proc SparseBlockArr.dsiSupportsPrivatization() param return false;

proc SparseBlockArr.dsiGetPrivatizeData() return dom.pid;

proc SparseBlockArr.dsiPrivatize(privatizeData) {
  var privdom = chpl_getPrivatizedCopy(dom.type, privatizeData);
  var c = new SparseBlockArr(eltType=eltType, rank=rank, idxType=idxType, stridable=stridable, dom=privdom);
  for localeIdx in c.dom.dist.targetLocDom {
    c.locArr(localeIdx) = locArr(localeIdx);
    if c.locArr(localeIdx).locale.id == here.id then
      c.myLocArr = c.locArr(localeIdx);
  }
  return c;
}

proc SparseBlockArr.dsiSupportsBulkTransfer() param return true;

proc SparseBlockArr.doiCanBulkTransfer() {
  if dom.stridable then
    for param i in 1..rank do
      if dom.whole.dim(i).stride != 1 then return false;

  // See above note regarding aliased arrays
  if disableAliasedBulkTransfer then
    if _arrAlias != nil then return false;

  return true;
}

proc SparseBlockArr.doiBulkTransfer(B) {
  if debugSparseBlockDistBulkTransfer then resetCommDiagnostics();
  var sameDomain: bool;
  // We need to do the following on the locale where 'this' was allocated,
  //  but hopefully, most of the time we are initiating the transfer
  //  from the same locale (local on clauses are optimized out).
  on this do sameDomain = dom==B._value.dom;
  // Use zippered iteration to piggyback data movement with the remote
  //  fork.  This avoids remote gets for each access to locArr[i] and
  //  B._value.locArr[i]
  coforall (i, myLocArr, BmyLocArr) in (dom.dist.targetLocDom,
                                        locArr,
                                        B._value.locArr) do
    on dom.dist.targetLocales(i) {

    if sameDomain &&
      chpl__useBulkTransfer(myLocArr.myElems, BmyLocArr.myElems) {
      // Take advantage of DefaultRectangular bulk transfer
      if debugSparseBlockDistBulkTransfer then startCommDiagnosticsHere();
      local {
        myLocArr.myElems._value.doiBulkTransfer(BmyLocArr.myElems);
      }
      if debugSparseBlockDistBulkTransfer then stopCommDiagnosticsHere();
    } else {
      if debugSparseBlockDistBulkTransfer then startCommDiagnosticsHere();
      if (rank==1) {
        var lo=dom.locDoms[i].mySparseBlock.low;
        const start=lo;
        //use divCeilPos(i,j) to know the limits
        //but i and j have to be positive.
        for (rid, rlo, size) in ConsecutiveChunks(dom,B._value.dom,i,start) {
          if debugSparseBlockDistBulkTransfer then writeln("Local Locale id=",i,
                                            "; Remote locale id=", rid,
                                            "; size=", size,
                                            "; lo=", lo,
                                            "; rlo=", rlo
                                            );
          // NOTE: This does not work with --heterogeneous, but heterogeneous
          // compilation does not work right now.  This call should be changed
          // once that is fixed.
          var dest = myLocArr.myElems._value.data;
          const src = B._value.locArr[rid].myElems._value.data;
          __primitive("chpl_comm_get",
                      __primitive("array_get", dest,
                                  myLocArr.myElems._value.getDataIndex(lo)),
                      rid,
                      __primitive("array_get", src,
                                  B._value.locArr[rid].myElems._value.getDataIndex(rlo)),
                      size);
          lo+=size;
        }
      } else {
        var orig=dom.locDoms[i].mySparseBlock.low(dom.rank);
        for coord in dropDims(dom.locDoms[i].mySparseBlock, dom.locDoms[i].mySparseBlock.rank) {
          var lo=if rank==2 then (coord,orig) else ((...coord), orig);
          const start=lo;
          for (rid, rlo, size) in ConsecutiveChunksD(dom,B._value.dom,i,start) {
            if debugSparseBlockDistBulkTransfer then writeln("Local Locale id=",i,
                                        "; Remote locale id=", rid,
                                        "; size=", size,
                                        "; lo=", lo,
                                        "; rlo=", rlo
                                        );
          var dest = myLocArr.myElems._value.data;
          const src = B._value.locArr[rid].myElems._value.data;
          __primitive("chpl_comm_get",
                      __primitive("array_get", dest,
                                  myLocArr.myElems._value.getDataIndex(lo)),
                      dom.dist.targetLocales(rid).id,
                      __primitive("array_get", src,
                                  B._value.locArr[rid].myElems._value.getDataIndex(rlo)),
                      size);
            lo(rank)+=size;
          }
        }
      }
      if debugSparseBlockDistBulkTransfer then stopCommDiagnosticsHere();
    }
  }
  if debugSparseBlockDistBulkTransfer then writeln("Comms:",getCommDiagnostics());
}

iter ConsecutiveChunks(d1,d2,lid,lo) {
  var elemsToGet = d1.locDoms[lid].mySparseBlock.numIndices;
  const offset   = d2.whole.low - d1.whole.low;
  var rlo=lo+offset;
  var rid  = d2.dist.targetLocsIdx(rlo);
  while (elemsToGet>0) {
    const size = min(d2.numRemoteElems(rlo,rid),elemsToGet):int;
    yield (rid,rlo,size);
    rid +=1;
    rlo += size;
    elemsToGet -= size;
  }
}

iter ConsecutiveChunksD(d1,d2,i,lo) {
  const rank=d1.rank;
  var elemsToGet = d1.locDoms[i].mySparseBlock.dim(rank).length;
  const offset   = d2.whole.low - d1.whole.low;
  var rlo = lo+offset;
  var rid = d2.dist.targetLocsIdx(rlo);
  while (elemsToGet>0) {
    const size = min(d2.numRemoteElems(rlo(rank):int,rid(rank):int),elemsToGet);
    yield (rid,rlo,size);
    rid(rank) +=1;
    rlo(rank) += size;
    elemsToGet -= size;
  }
}

proc SparseBlockDom.numRemoteElems(rlo,rid){
  var blo,bhi:dist.idxType;
  if rid==(dist.targetLocDom.dim(rank).length - 1) then
    bhi=whole.dim(rank).high;
  else
      bhi=dist.boundingBox.dim(rank).low +
	intCeilXDivByY((dist.boundingBox.dim(rank).high - dist.boundingBox.dim(rank).low +1)*(rid+1),
                   dist.targetLocDom.dim(rank).length) - 1;
  
  return(bhi - rlo + 1);
}

//Brad's utility function. It drops from Domain D the dimensions
//indicated by the subsequent parameters dims.
proc dropDims(D: domain, dims...) {
  var r = D.dims();
  var r2: (D.rank-dims.size)*r(1).type;
  var j = 1;
  for i in 1..D.rank do
    for k in 1..dims.size do
      if dims(k) != i {
        r2(j) = r(i);
        j+=1;
      }
  var DResult = {(...r2)};
  return DResult;
}

