// WrapperDist redirects most operations to the global descriptor classes
// that it wrapps. Each wrapper kind must provide proc origIx(userIx) to
// map user indices to indices in the wrapped distribution's coordnates.

// Presently wrapper-specific data is 'const'.

// TODO
//
// * reference counting; destruction
//  e.g. when a Wrapper object is destroyed, need to
//  decrement the ref count of its 'orig' object
//
// * efficient iterators
//  e.g. do not iterate over indices that won't be yielded;
//  do not use dsiAccess in array iterators
//
// * rewrite each of WrapperDist/Dom/Arr as a superclass + subclasses
//  but check that dynamic dispatch is not used in that scenario
//
// * run the robustness suite over WrapperDist itself,
//  with various wrappees

//
// wrapper functionality
//
enum WrapperKind { reindex, rankchange };


class WrapperDist : BaseDist {
  const origDist;

  param kind: WrapperKind;
  const _data;

  // kind = 'reindex'
  //
  // _data is a rank-tuple of ints. Each component gives the offset, in that
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

  // local cache (reprivatized)
  var whole: domain(rank, idxType, stridable);

  proc kind param  return dist.kind;

  // kind = 'reindex'
  proc deltaToOrig { compilerAssert(isReindexing()); return _data; }

  // kind = 'rankchange' - nothing for now

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

  // kind = 'rankchange' - nothing for now

  // for privatization
  var pid: int = -1;
}


/////////////////////////////////////////////////////////////////////////////
// debugging

// todo: replace with 'param'
config var wdverb = false;
config var verboseWrapperPrivHelper = false;
config var setixverb = wdverb;
config var trackcreate = wdverb;
config var explainUserToOrigFT = wdverb;
config var verbleaderiter = wdverb;
config var explainDsiSliceOnRankChange = wdverb;

proc _wdadr(o:object):string
  return "0x%xu".format(__primitive("object2int", o));


proc WrapperDist.toString() {
  return "WrapperDist(" + this.kind:string + "," +
         "rank:" + this.rank:string + "<-" + this.origRank:string + "," +
         this._data.type:string + "," +
         "origDist:" + this.origDist.type:string + "," +
         _wdadr(this) + ")";
}

proc WrapperRectDom.toString() {
  return "WrapperRectDom(" + this.kind:string + "," + this.rank:string + "," +
         "dims:" + this.dsiGetIndices():string +
         "<-" + this.origDom.dsiGetIndices():string + "," +
         _wdadr(this) + ")";
}


proc reportNewArr(msg, result: WrapperArr) {
  if trackcreate then writeln(msg,
    "  result ", result.dom.dist.kind, " ", _wdadr(result),
    "  result.origArr ", _wdadr(result.origArr),
    "  result.dom ", _wdadr(result.dom),
    "  result.dom.origDom ", _wdadr(result.dom.origDom),
    "  result.origArr.dom ", if result.dom.origDom == result.origArr.dom
     then "same" else ("diff: " + _wdadr(result.origArr.dom))
  );
  return result;
}

proc reportNewDom(msg, result: WrapperRectDom) {
  if trackcreate then writeln(msg,
 "  result ", result.dist.kind, " ", _wdadr(result),
 "  result.dist ", _wdadr(result.dist),
 "  result.origDom ", _wdadr(result.origDom));
  return result;
}

proc reportNewDist(msg, result: WrapperDist) {
  if trackcreate then writeln(msg,
 "  result ", result.kind, " ", _wdadr(result),
 "  result.origDist ", _wdadr(result.origDist));
  return result;
}

proc WrapperRectDom._reportSetIndices(newRanges): void {
  reportNewDom("dsiSetIndices", this);
  if setixverb {
    const origRanges = origIx(newRanges);
    writeln(toString(), ".SetIndices", newRanges,
            "  origRanges ", origRanges, "  data ", dist._data);
  }
}


/////////////////////////////////////////////////////////////////////////////
// privatization

// helper for privatization
// todo: move to DSIUtil.chpl
//
inline
proc ensureHasBeenPrivatized(arg): void {
  if arg.dsiSupportsPrivatization() {
    if arg.pid == -1 {
      if verboseWrapperPrivHelper then
        writeln("privatizing ", arg.type:string);
      _newPrivatizedClass(arg);
    }
  } else {
    // this writeln is reasonable only when this is an uncommon case
    if verboseWrapperPrivHelper then
      writeln("does not support privatization ", arg.type:string);
  }
}

proc _checkPrivatized(context, arg) {
  assert(arg.pid != -1,
         arg.type:string, "  from  ", context.type:string);
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
proc WrapperRectDom.dsiNumIndices         return whole.size;
proc WrapperRectDom.dsiMember(indexx)     return whole.contains(indexx);
proc WrapperRectDom.dsiIndexOrder(indexx) return whole.indexOrder(indexx);

proc WrapperRectDom.dsiSerialWrite(f: Writer): void { f.write(whole); }

proc WrapperArr.idxType type  return dom.idxType;

proc WrapperArr.dsiGetBaseDom()  return dom;

proc WrapperRectDom.dsiGetIndices()  return whole.dims();


//== creation, SetIndices, Access

proc WrapperDist.dsiNewRectangularDom(param rank: int,
                                      type idxType,
                                      param stridable: bool,
                                      inds)
{
  const origdom = origDist.dsiNewRectangularDom(this.origRank,
                                                idxType, stridable, inds);
  ensureHasBeenPrivatized(this);
  ensureHasBeenPrivatized(origdom);

  // 'kind' is determined by this.kind and affects initialData()
  const result = new WrapperRectDom(rank=rank, idxType=idxType,
                                    stridable=stridable, dist=this,
                                    origDom=origdom);
  reportNewDom("dsiNewRectangularDom", result);
  result.dsiSetIndices(inds);
  return result;
}

proc WrapperRectDom.dsiSetIndices(newIndices: domain): void {
  whole = newIndices;
  const newRanges = newIndices.dims();
  _reportSetIndices(newRanges);
  origDom.dsiSetIndices(origIx(newRanges));
}

proc WrapperRectDom.dsiSetIndices(newRanges): void where isTuple(newRanges) {
  whole = {(...newRanges)};
  _reportSetIndices(newRanges);
  origDom.dsiSetIndices(origIx(newRanges));
}

proc WrapperRectDom.dsiBuildArray(type eltType) {
  const origarr = origDom.dsiBuildArray(eltType);

  ensureHasBeenPrivatized(this);
  ensureHasBeenPrivatized(origarr);

  // 'kind' is determined by this.kind and affects initialData()
  const result =
    new WrapperArr(eltType = eltType, dom = this, origArr = origarr);

  reportNewArr("dsiBuildArray", result);

  return result;
}

proc WrapperArr.dsiAccess(indexx) ref: eltType {
  const ixt = if isIntegralType(indexx.type) then (indexx,) else indexx;
  return origArr.dsiAccess(origIx(ixt));
}


//== iterators - domain

iter WrapperRectDom.these() {
  for ix in whole do
    yield ix;
}


proc WrapperRectDom.initialData() where isReindexing()  return dist.deltaToOrig;
proc WrapperArr.initialData()     where isReindexing()  return dom.deltaToOrig;


//== reindex main properties

proc WrapperDist.isReindexing() param     return kind==WrapperKind.reindex;
proc WrapperRectDom.isReindexing() param  return kind==WrapperKind.reindex;
proc WrapperArr.isReindexing() param      return kind==WrapperKind.reindex;

proc WrapperDist.rank param  where isReindexing()  return this.deltaToOrig.size;
proc WrapperDist.origRank param  where isReindexing()  return this.rank;

// Given a user index (an int or a tuple of ints),
// return the index in origDist's coordinates.
//
proc WrapperDist.origIx(userIx)         where isReindexing()  return userIx + deltaToOrig;
proc WrapperRectDom.origIx(userIx)      where isReindexing()  return userIx + deltaToOrig;
proc WrapperArr.origIx(userIx)          where isReindexing()  return userIx + deltaToOrig;

// visa versa
proc WrapperRectDom.fromOrigIx(origInd) where isReindexing()  return origInd - deltaToOrig;


//== reindex-specific functionality

proc WrapperArr.dsiSlice(sliceDef:WrapperRectDom) where isReindexing() {
  // unclear what to do otherwise
  compilerAssert(sliceDef.isReindexing());

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

  const result = new WrapperArr(eltType = this.eltType,
                        dom = sliceDef,
                        origArr = origarr);
  reportNewArr("dsiSlice", result);
  return result;
}


//== writing

proc WrapperArr.dsiSerialWrite(f: Writer): void where isReindexing() {
  origArr.dsiSerialWrite(f);
}


//== iterators - domain

iter WrapperRectDom.these(param tag: iterKind) where tag == iterKind.leader && isReindexing() {
  // reindexing does not affect densified indices
  for flw in origDom.these(tag) do
    yield flw;
}

iter WrapperRectDom.these(param tag: iterKind, followThis) where tag == iterKind.follower && isReindexing() {
  // translate the indices back in the user space
  for ix in origDom.these(tag, followThis) do
    yield fromOrigIx(ix);
}


//== iterators - array

iter WrapperArr.these() ref where isReindexing() {
  // not affected by reindexing
  for a in origArr.these() do
    yield a;
}

iter WrapperArr.these(param tag: iterKind) where tag == iterKind.leader && isReindexing() {
  // redirect to the domain
  for followThis in dom.these(tag) do
    yield followThis;
}

iter WrapperArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower && isReindexing() {

  // reindexing does not affect densified indices
  for v in origArr.these(tag, followThis) do
    yield v;
}


proc WrapperRectDom.initialData() where isRankChange()  return 0;  // nothing
proc WrapperArr.initialData()     where isRankChange()  return 0;  // nothing


//== rankchange main properties

proc WrapperDist.isRankChange() param     return kind==WrapperKind.rankchange;
proc WrapperRectDom.isRankChange() param  return kind==WrapperKind.rankchange;
proc WrapperArr.isRankChange() param      return kind==WrapperKind.rankchange;


proc WrapperDist.rank param  where isRankChange()
  return _numFullDimensionsFrom(1);

proc WrapperDist.origRank param  where isRankChange()
  return this.sliceDef.size;

// like for isReindexing
proc WrapperDist.origIx(userIx)      where isRankChange() return _origIxFrom(userIx, 1, 1);
proc WrapperRectDom.origIx(userIx)   where isRankChange() return dist.origIx(userIx);
proc WrapperArr.origIx(userIx)       where isRankChange() return dom.origIx(userIx);

proc WrapperDist.fromOrigIx(origInd) where isRankChange() {
  //??  if boundsChecking then _origIndBoundsCheck(origInd);
  return _fromOrigIxFrom(origInd, 1, 1);
}


//== rankchange auxiliary helpers

// helper: is this dimension in origDist's coordinates
// collapsed in this rank-changing slice?
proc WrapperDist.isCollapsedOrigDim(param origDim) param
  return isCollapsedDimension(sliceDef(origDim));

proc WrapperDist._numFullDimensionsFrom(param startOrigDim) param {
  if startOrigDim > this.origRank then
    return 0;
  return isCollapsedOrigDim(startOrigDim):int +
         _numFullDimensionsFrom(startOrigDim + 1);
}

proc WrapperDist._highestUncollapsedOrigDimension() param {
  for param dim in 1..this.origRank by -1 do
    if !isCollapsedOrigDim(dim) then
      return dim;

  // If all-collapsed-dimensions slicing is allowed,
  // review the uses of this function to handle such a case.
  // E.g. return 0 in this case.
  compilerError("The slice ", sliceDef.type:string,
                " contains only collapsed dimensions");
}

// Given a user index, combine it with collapsed dimensions from sliceDef
// to produce origDist's index.
inline
proc WrapperDist._origIxFrom(userIx, param origDim, param userDim)
{
  param collapsed = isCollapsedOrigDim(origDim);
  proc curIx() {
    if collapsed {
      const origInd = sliceDef(origDim);
      // If we start from ranges, gotta produce ranges e.g. for dsiSetIndices.
      return if isRange(userIx(1))
             then (origInd..origInd): userIx(1).type
             else origInd;
    } else {
      return userIx(userDim);
    }
  }

  if origDim == this.origRank {
    compilerAssert(userDim == if collapsed then this.rank + 1 else this.rank);
    return (curIx(),);
  } else {
    param nextUserDim = if collapsed then userDim else userDim + 1;
    return (curIx(), (..._origIxFrom(userIx, origDim+1, nextUserDim)));
  }
}

// Given an index in origDist's coordinates, convert it to the user index
// by omitting the collapsed dimensions.
proc WrapperDist._fromOrigIxFrom(origInd, param origDim, param userDim)
{
  if userDim == _highestUncollapsedOrigDimension() {
    compilerAssert(!isCollapsedOrigDim(origDim));
    return (origInd(origDim),);

  } else if isCollapsedOrigDim(origDim) {
    return _fromOrigIxFrom(origInd, origDim+1, userDim);

  } else {
    return (origInd(origDim),
            (..._fromOrigIxFrom(origInd, origDim+1, userDim+1)));
  }
}


//== rankchange-specific functionality

proc WrapperArr.dsiSlice(sliceDef:WrapperRectDom) where isRankChange() {
  // unclear what to do otherwise
  compilerAssert(sliceDef.isRankChange());

  const slicee = this;

//writeln("slicing WrapperArr ", slicee.dom.dsiDims(), "  with WrapperRectDom ", sliceDef.dsiDims());

  // The rank change slicee->slicee.origArr is always the same
  // as sliceDef->sliceDef.origDom ?
  assert(slicee.dom.dist.sliceDef == sliceDef.dist.sliceDef);

  // Example:
  //  take an array over     {1..20, 1..20}
  //  apply rank change with {2..18, 19}
  // => get 'slicee'
  //  slicee.origArr.dom = {1..20, 1..20}
  //  slicee.dom         = {2..18}
  //  slicee.dom.origDom = {2..18, 19..19}
  //
  // When sliced with sliceDef = {2..15}
  //  sliceDef.origDom         = {2..15, 19..19}

  const origSliceDef = sliceDef.origDom;

  if explainDsiSliceOnRankChange {
    writeln("dsiSlice");
    writeln("  slicee.dom ", slicee.dom.toString());
    writeln("  sliceDef   ", sliceDef.toString());
    writeln("  slicee.origArr.dom ", slicee.origArr.dom.dsiDims());
    writeln("  sliceDef.origDom   ", sliceDef.origDom.dsiDims());
    writeln();
  }

  const origarr = slicee.origArr.dsiSlice(origSliceDef);

  ensureHasBeenPrivatized(sliceDef);
  ensureHasBeenPrivatized(origarr);

  const result = new WrapperArr(eltType = this.eltType,
                        dom = sliceDef,
                        origArr = origarr);
  reportNewArr("dsiSlice", result);
  return result;
}


//== writing

// This is a just copy of BlockArr.dsiSerialWrite(). OK?
proc WrapperArr.dsiSerialWrite(f: Writer): void where isRankChange() {
reportNewArr("dsiSerialWrite", this);
  param rank = dom.rank;       // added from BlockArr.dsiSerialWrite
  type idxType = dom.idxType;  // ditto
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


//== iterators - domain

iter WrapperRectDom.these(param tag: iterKind) where tag == iterKind.leader && isRankChange() {
  const dist = this.dist;
  const userWhole = this.whole;

  // TODO: avoid iterating over the indices that will not be yielded
  for denseOrigFlwThis in origDom.these(tag) {
    const (intersects, denseUserFlwThis) =
      _origToUserDenseFT(denseOrigFlwThis);
    if intersects then
{ if verbleaderiter then writeln("WrapperRectDom leader  yield=", denseUserFlwThis);
      yield denseUserFlwThis;
}
  }
}

// 'intersects' indicates whether original followThis does not intersect
// with the user index space.
proc WrapperRectDom._origToUserDenseFT(denseOrigFT) where isRankChange()
{ const dist = this.dist;
  // todo: can we do without the undensify/densify sandwich?
  const undensOrigFT = unDensify(denseOrigFT, origDom.dsiGetIndices());
  var intersects = true;
  // todo: abort processing once 'intersects' becomes false
  // this will require determining the result type in advance
  const undensUserFT = origToUserRangesFrom(undensOrigFT, 1);
  const denseUserFT = densify(undensUserFT, dsiGetIndices());
  return (intersects, denseUserFT);

  proc origToUserRangesFrom(origRanges, param origDim) {
    if dist.isCollapsedOrigDim(origDim) {
      handleCDim(origRanges, origDim); // intersection check
      return origToUserRangesFrom(origRanges, origDim+1);
    } else if origDim == dist._highestUncollapsedOrigDimension() {
        for param oDim in origDim+1..dist.origRank do
          handleCDim(origRanges, oDim);
        return (handleUDim(origRanges, origDim),);
    } else {
      return (handleUDim(origRanges, origDim),
              (...origToUserRangesFrom(origRanges, origDim + 1)));
    }
  }

  // the intersection check
  proc handleCDim(origRanges, param origDim) {
    compilerAssert(dist.isCollapsedOrigDim(origDim));
    if !origRanges(origDim).contains(dist.sliceDef(origDim)) then
        intersects = false;
  }
  // slice one of the origRanges
  proc handleUDim(origRanges, param origDim) {
    compilerAssert(!dist.isCollapsedOrigDim(origDim));
    return (origRanges(origDim)) (dist.sliceDef(origDim));
  }
}

iter WrapperRectDom.these(param tag: iterKind, followThis) where tag == iterKind.follower && isRankChange() {
  const dist = this.dist;

  // translate the indices back in the user space
  for origInd in origDom.these(tag, _userToOrigDenseFT(followThis, origDom, "WrapperRectDom follower")) do
    yield dist.fromOrigIx(origInd);
}

proc WrapperRectDom._userToOrigDenseFT(denseUserFT, origdom, debug_msg) where isRankChange() {
  // todo: can we do without the unDensify/densify sandwich?
  const undensUserFT = unDensify(denseUserFT, dsiGetIndices());
  const undensOrigFT = dist.origIx(undensUserFT);
  const denseOrigFT = densify(undensOrigFT, origdom.dsiGetIndices());

  if explainUserToOrigFT then writeln(
 "_userToOrigDenseFT ", debug_msg, "\n",
 "  denseUserFT ", denseUserFT, "\n",
 "  user.whole ", dsiGetIndices(), "\n",
 "  undensUserFT ", undensUserFT, "\n",
 "  undensOrigFT ", undensOrigFT, "\n",
 "  orig.whole ", origdom.dsiGetIndices(), "\n",
 "  denseOrigFT ", denseOrigFT);

  return denseOrigFT;
}


//== iterators - array

iter WrapperArr.these() ref where isRankChange() {
  // todo: is it possible to make this more efficient?
  for ix in this.dom.whole do
    yield this.dsiAccess(ix);
}

iter WrapperArr.these(param tag: iterKind) where tag == iterKind.leader && isRankChange() {
  // redirect to the domain
  for followThis in dom.these(tag) do
    yield followThis;
}

iter WrapperArr.these(param tag: iterKind, followThis) ref where tag == iterKind.follower && isRankChange() {

  reportNewArr("WrapperArr follower on " + followThis:string, this);
  for v in origArr.these(tag, this.dom._userToOrigDenseFT(followThis, this.origArr.dom, "WrapperArr follower")) do
    yield v;
}
