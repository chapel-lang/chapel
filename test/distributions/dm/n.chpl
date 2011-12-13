// WrapperDist redirects most operations to the global descriptor classes
// that it wrapps. Each wrapper kind must provide proc origIx(userIx) to
// map user indices to indices in the wrapped distribution's coordnates.

// Presently wrapper-specific data is 'const'.

// TODO: reference counting; destruction
// (e.g. when a Wrapper object is destroyed, need to
// decrement the ref count of its 'orig' object).

enum WrapperKind { reindex, rankchange };

config var verboseWrapperPrivHelper = false;

class WrapperDist : BaseDist {
  const origDist;

  param kind: WrapperKind;
  const _data;

  // kind = 'reindex'
  //
  // _data is a tuple of ints. Each component indicates the offset, in that
  // dimension, from the user index to the index in origDist's coordinates.
  //
  proc deltaToOrig { compilerAssert(isReindexing()); return _data; }


  // kind = 'rankchange'
  //
  // _data is an origRank-tuple, whose components are the arguments to
  // the slicing operation. Each component is either integral or a range;
  // ints indicate collapsed dimensions.
  //
  proc sliceDef { compilerAssert(isRankChange()); return _data; }

  // for privatization
  var pid: int = -1;
}

class WrapperRectDom : BaseRectangularDom {
  // required
  param rank: int;
  type idxType;
  param stridable: bool;
  const dist;

  // Wrapper-specific
  const origDom;
  const _data = initialData();

  // local cache, reprivatized
  var whole: origDom.whole.type;

  proc kind param  return dist.kind;

  // kind = 'reindex'
  proc deltaToOrig { compilerAssert(isReindexing()); return _data; }

  // kind = 'rankchange'
  // TODO

  // for privatization
  var pid: int = -1;
}

class WrapperArr: BaseArr {
  // required
  type eltType;
  const dom;

  // Wrapper-specific
  const origArr;
  const _data = initialData();

  proc kind param  return dom.kind;

  // kind = 'reindex'
  proc deltaToOrig { compilerAssert(isReindexing()); return _data; }

  // kind = 'rankchange'
  // TODO

  // for privatization
  var pid: int = -1;
}

/////////////////////////////////////////////////////////////////////////////
// privatization

// helper for privatization
// todo: move to DSIUtil.chpl
//
inline proc ensureHasBeenPrivatized(arg): void {
  if arg.dsiSupportsPrivatization() {
    if arg.pid == -1 {
      if verboseWrapperPrivHelper then
        writeln("privatizing ", typeToString(arg.type));
      _newPrivatizedClass(arg);
    }
  } else {
    // this writeln is reasonable only when this is an uncommon case
    if verboseWrapperPrivHelper then
      writeln("does not support privatization ", typeToString(arg.type));
  }
}

proc _checkPrivatized(context, arg) {
  assert(arg.pid != -1,
         typeToString(arg.type), "  from  ", typeToString(context.type));
}

proc WrapperDist.dsiSupportsPrivatization() param
  return origDist.dsiSupportsPrivatization();

proc WrapperDist.dsiGetPrivatizeData() {
  _checkPrivatized(this, origDist);
  return (origDist.pid, _data);
}

proc WrapperDist.dsiPrivatize(privatizeData) {
  const privOrigDist = chpl_getPrivatizedCopy(objectType = this.origDist.type,
                                              objectPid  = privatizeData(1));
  return new WrapperDist(origDist = privOrigDist,
                         kind     = this.kind,
                         _data    = privatizeData(2));
}

proc WrapperRectDom.dsiSupportsPrivatization() param
  return origDom.dsiSupportsPrivatization();

proc WrapperRectDom.dsiGetPrivatizeData() {
  _checkPrivatized(this, dist);
  _checkPrivatized(this, origDom);

  return (dist.pid, origDom.pid, whole, _data);
}

proc WrapperRectDom.dsiPrivatize(privatizeData) {
  const privDist = chpl_getPrivatizedCopy(objectType = this.dist.type,
                                          objectPid  = privatizeData(1));
  const privOrigDom = chpl_getPrivatizedCopy(objectType = this.origDom.type,
                                             objectPid = privatizeData(2));

  return new WrapperRectDom(rank      = this.rank,
                            idxType   = this.idxType,
                            stridable = this.stridable,
                            dist      = privDist,
                            origDom   = privOrigDom,
                            whole     = privatizeData(3),
                            _data     = privatizeData(4));
}

proc WrapperRectDom.dsiGetReprivatizeData() {
  return (origDom, origDom.dsiGetReprivatizeData(), whole);
}

proc WrapperRectDom.dsiReprivatize(other, reprivatizeData) {
  this.origDom.dsiReprivatize(reprivatizeData(1), reprivatizeData(2));
  this.whole = reprivatizeData(3);
}

proc WrapperArr.dsiSupportsPrivatization() param
  return origArr.dsiSupportsPrivatization();

proc WrapperArr.dsiGetPrivatizeData() {
  _checkPrivatized(this, dom);
  _checkPrivatized(this, origArr);

  return (dom.pid, origArr.pid, _data);
}

proc WrapperArr.dsiPrivatize(privatizeData) {
  const privDom = chpl_getPrivatizedCopy(objectType = this.dom.type,
                                         objectPid  = privatizeData(1));
  const privOrigArr = chpl_getPrivatizedCopy(objectType = this.origArr.type,
                                             objectPid = privatizeData(2));

  return new WrapperArr(eltType = this.eltType,
                        dom     = privDom,
                        origArr = privOrigArr,
                        _data   = privatizeData(3));
}


/////////////////////////////////////////////////////////////////////////////
// other generic functionality

proc WrapperDist.isReindexing() param     return kind==WrapperKind.reindex;
proc WrapperRectDom.isReindexing() param  return kind==WrapperKind.reindex;
proc WrapperArr.isReindexing() param      return kind==WrapperKind.reindex;

proc WrapperDist.isRankChange() param     return kind==WrapperKind.rankchange;
proc WrapperRectDom.isRankChange() param  return kind==WrapperKind.rankchange;
proc WrapperArr.isRankChange() param      return kind==WrapperKind.rankchange;


//== simple methods

proc WrapperDist.dsiClone()  return this;

proc WrapperDist.dsiIndexToLocale(indexx): locale
  return origDist.dsiIndexToLocale(origIx(indexx));

proc WrapperRectDom.dsiMyDist() return dist;

proc WrapperRectDom.dsiDims()             return whole.dims();
proc WrapperRectDom.dsiDim(d)             return whole.dim(d);
proc WrapperRectDom.dsiDim(param d)       return whole.dim(d);
proc WrapperRectDom.dsiLow                return whole.low;
proc WrapperRectDom.dsiHigh               return whole.high;
proc WrapperRectDom.dsiStride             return whole.stride;
proc WrapperRectDom.dsiNumIndices         return whole.numIndices;
proc WrapperRectDom.dsiMember(indexx)     return whole.member(indexx);
proc WrapperRectDom.dsiIndexOrder(indexx) return whole.indexOrder(indexx);

proc WrapperRectDom.dsiSerialWrite(f: Writer): void { f.write(whole); }

proc WrapperArr.idxType type return dom.idxType;

proc WrapperArr.dsiGetBaseDom() return dom;


//== creation, SetIndices

proc WrapperDist.dsiNewRectangularDom(param rank: int,
                                          type idxType,
                                          param stridable: bool)
{
  const origdom = origDist.dsiNewRectangularDom(rank, idxType, stridable);

  ensureHasBeenPrivatized(this);
  ensureHasBeenPrivatized(origdom);

  // 'kind' is determined by this.kind and affects initialData()
  return new WrapperRectDom(rank=rank, idxType=idxType, stridable=stridable,
                            dist=this, origDom=origdom);
}

proc WrapperRectDom.dsiSetIndices(newIndices: domain): void {
  whole = newIndices;
  origDom.dsiSetIndices(origIx(newIndices));
}

proc WrapperRectDom.dsiSetIndices(newRanges): void where isTuple(newRanges) {
  whole = [(...newRanges)];
  origDom.dsiSetIndices(origIx(newRanges));
}

proc WrapperRectDom.dsiGetIndices()
  return whole.dims();

proc WrapperRectDom.dsiBuildRectangularDom(param rank: int,
                                           type idxType,
                                           param stridable: bool,
                                           ranges: rank * range(idxType,
                                                 BoundedRangeType.bounded,
                                                                stridable))
{
  const origdom = origDom.dsiBuildRectangularDom(rank, idxType, stridable,
                                                origIx(ranges));
  ensureHasBeenPrivatized(this.dist);
  ensureHasBeenPrivatized(origdom);

  // 'kind' is determined by this.dist.kind and affects initialData()
  return new WrapperRectDom(rank=rank, idxType=idxType, stridable=stridable,
                            dist=this.dist, origDom = origdom,
                            whole = [(...ranges)]);
}

proc WrapperRectDom.dsiBuildArray(type eltType) {
  const origarr = origDom.dsiBuildArray(eltType);

  ensureHasBeenPrivatized(this);
  ensureHasBeenPrivatized(origarr);

  // 'kind' is determined by this.kind and affects initialData()
  return new WrapperArr(eltType = eltType, dom = this, origArr = origarr);
}

proc WrapperArr.dsiAccess(indexx) var: eltType
  return origArr.dsiAccess(origIx(indexx));


/////////////////////////////////////////////////////////////////////////////
// 'reindex' WrapperDist

// creating and using it - see e.g.
// DimensionalDist.dsiCreateReindexDist(), DimensionalArr.dsiReindex()

// For 'origRank', use origDist.rank if available,
// otherwise newSpace.size or oldSpace.size.
//
inline
proc genericDsiCreateReindexDist(reindexeeDist, param origRank, newSpace, oldSpace)
  where reindexeeDist: BaseDist
{
  compilerAssert(origRank == newSpace.size);
  compilerAssert(origRank == oldSpace.size);

  ensureHasBeenPrivatized(reindexeeDist);

  const new2old = n2oAll(origRank);

  // verify our multidimensional forumla in the case of 2d
  if origRank == 2 then
    assert(new2old == (n2oOne(1),n2oOne(2)));

  return newWrapperReindexDist(reindexeeDist = reindexeeDist, deltaToOrig = new2old);

  // helper functions

  proc n2oAll(param numDims)
    return if numDims == 1 then tuple(n2oOne(1))
           else ((...n2oAll(numDims-1)), n2oOne(numDims));

  proc n2oOne(param dim) {
    const newr = newSpace(dim), oldr = oldSpace(dim);
    if newr.stride != oldr.stride then
      halt("reindexing from ", oldr, " to ", newr,
           " is not supported by ", oldr.toString,
           " due to a change in stride");
    //writeln("dim", dim, "  ", oldr, " >> ", newr,
    //        "  delta ", oldr.low - newr.low);
    return oldr.low - newr.low;
  }
}

proc genericDsiReindex(reindexeeArr, reindexDef: WrapperRectDom)
  where reindexeeArr: BaseArr  &&  reindexDef.isReindexing()
{
  ensureHasBeenPrivatized(reindexDef);
  ensureHasBeenPrivatized(reindexeeArr);

  // todo: do anything special in this case?
  //if reindexeeArr.dom == reindexDef then return reindexeeArr;

  return new WrapperArr(eltType = reindexeeArr.eltType,
                        dom = reindexDef,
                        origArr = reindexeeArr);
}


//== reindex-specific constructors
// todo: which are needed? if just the Dist one, move it up

proc newWrapperReindexDist(reindexeeDist, deltaToOrig)
{
  const result = new WrapperDist(origDist = reindexeeDist,
                                 kind     = WrapperKind.reindex,
                                _data     = deltaToOrig);
  compilerAssert(result.isReindexing());
  return result;
}

proc WrapperRectDom.initialData() where isReindexing()  return dist.deltaToOrig;
proc WrapperArr.initialData()     where isReindexing()  return dom.deltaToOrig;


/*
proc newWrapperReindexRectDom(param rank: int,
                              type idxType,
                              param stridable: bool,
                              dist, origDom)
{
  const result = new WrapperRectDom(rank = rank,
                                    idxType = idxType,
                                    stridable = stridable,
                                    dist = dist,
                                    origDom = origDom,
                                    _data = dist.deltaToOrig);
  compilerAssert(result.kind == WrapperKind.reindex);
  return result;
}

proc newWrapperReindexArr(type eltType, dom, origArr)
{
  const result = new WrapperArr(eltType = eltType,
                                dom = dom,
                                origArr = origArr,
                                _data = dom.deltaToOrig);

  compilerAssert(result.kind == WrapperKind.reindex);
  return result;
}
*/


//== reindex-specific functionality

// Given a user index (an int or a tuple of ints),
// return the index in origDist's coordinates.
// Or visa versa.
//
proc WrapperDist.origIx(userIx)             where isReindexing()  return userIx + deltaToOrig;
proc WrapperRectDom.origIx(userIx)          where isReindexing()  return userIx + deltaToOrig;
proc WrapperRectDom.fromOrigIx(originalIx)  where isReindexing()  return originalIx - deltaToOrig;
proc WrapperArr.origIx(userIx)              where isReindexing()  return userIx + deltaToOrig;


proc WrapperArr.dsiSerialWrite(f: Writer): void {
  origArr.dsiSerialWrite(f);
}

proc WrapperArr.dsiSlice(sliceDef:WrapperRectDom) {
  const slicee = this;

//writeln("slicing WrapperArr ", slicee.dom.dsiDims(), "  with WrapperRectDom ", sliceDef.dsiDims());

  // The index difference slicee->slicee.origArr is always the same
  // as sliceDef->sliceDef.origDom ?
  assert(slicee.deltaToOrig == sliceDef.deltaToOrig);

  // ... if so, no need to shift "orig" indices anywhere,
  // nether for slicee nor for sliceDef.
  const origSliceDef = sliceDef.origDom;

  const origarr = slicee.origArr.dsiSlice(origSliceDef);

  ensureHasBeenPrivatized(sliceDef);
  ensureHasBeenPrivatized(origarr);

  return new WrapperArr(eltType = this.eltType,
                        dom = sliceDef,
                        origArr = origarr);
}


// TODO: slicing, reindexing, rank change


//== iterators - domain

iter WrapperRectDom.these() {
  for ix in whole do
    yield ix;
}

iter WrapperRectDom.these(param tag: iterKind) where tag == iterKind.leader {
  // reindexing does not affect densified indices
  for flw in origDom.these(tag) do
    yield flw;
}

iter WrapperRectDom.these(param tag: iterKind, followThis) where tag == iterKind.follower {

  for ix in origDom.these(tag, followThis) do
    yield fromOrigIx(ix);
}


//== iterators - array

iter WrapperArr.these() var {
  // not affected by reindexing
  for a in origArr.these() do
    yield a;
}

iter WrapperArr.these(param tag: iterKind) where tag == iterKind.leader {
  // redirect to the domain
  for followThis in dom.these(tag) do
    yield followThis;
}

iter WrapperArr.these(param tag: iterKind, followThis) var where tag == iterKind.follower {

  // reindexing does not affect densified indices
  for v in origArr.these(tag, followThis) do
    yield v;
}


/////////////////////////////////////////////////////////////////////////////
// 'rankchange' WrapperDist

// creating and using it - see e.g.
// DimensionalDist.dsiCreateRankChangeDist(), DimensionalArr.dsiRankChange()

inline
proc genericCreateRankChangeDist(sliceeDist, param newRank: int, args)
  where sliceeDist: BaseDist
{
  const result = newWrapperRankChangeDist(sliceeDist = sliceeDist, sliceDef = args);
  compilerAssert(result.rank == newRank);
  return result;
}


//== rankchange-specific constructors

proc newWrapperRankChangeDist(sliceeDist, sliceDef) {
  const result = new WrapperDist(origDist = sliceeDist,
                                 kind     = WrapperKind.rankchange,
                                _data     = sliceDef);
  compilerAssert(result.isRankChange());
}

proc WrapperRectDom.initialData() where isRankChange()  return dist._data; // TODO ???
proc WrapperArr.initialData()     where isRankChange()  return dom._data;  // TODO ???


//== rankchange-specific functionality

// Given a user index (an int or a tuple of ints),
// return the index in origDist's coordinates.
//
proc WrapperDist.origIx(userIx)    where isRankChange()  return _origIxFrom(userIx, 1, 1);
proc WrapperRectDom.origIx(userIx) where isRankChange()  return dist.origIx(userIx);
proc WrapperArr.origIx(userIx)     where isRankChange()  return dom.origIx(userIx);


// The rank supported by 'origDist'.
proc WrapperDist.origRank param  return sliceDef.size;

// The rank supported by this RankChangeDist.
proc WrapperDist.rank param  return _numFullDimensionsFrom(1);

// helper: define it on types as well
// TODO: move to ChapelArray.chpl
//proc isCollapsedDimension(type t) param
//{ const x: t; return isCollapsedDimension(x); }

proc WrapperDist._numFullDimensionsFrom(param startDim) param {
  if startDim > origRank then
    return 0;
  param isCollapsed = isCollapsedDimension(sliceDef(startDim));
  return isCollapsed:int + _numFullDimensionsFrom(startDim + 1);
}

proc debugPrint(rd: WrapperDist) where rd.isRankChange() {
  writeln("WrapperDist rankchange\n",
          "  sliceDef  ", rd.sliceDef, "\n",
          "  origRank ", rd.origRank, "  user rank ", rd.rank, "\n",
          "  origDist ", typeToString(rd.origDist.type), "\n",
          "");
}


// Given a user index, combine it with collapsed dimensions from sliceDef
// to produce origDist's index.
inline
proc WrapperDist._origIxFrom(userIx, param origDim, param userDim)
{
  const isCollapsed = isCollapsedDimension(sliceDef(origDim));
  if origDim == origRank {
    compilerAssert(userDim == rank);
    if isCollapsed then
      return tuple(sliceDef(origDim));
    else
      return tuple(userIx(userDim));
  } else {
    if isCollapsed then
      return (sliceDef(origDim), (..._origIxFrom(userIx, origDim+1, userDim)));
    else
      return (userIx(userDim), (..._origIxFrom(userIx, origDim+1, userDim+1)));
  }
}

