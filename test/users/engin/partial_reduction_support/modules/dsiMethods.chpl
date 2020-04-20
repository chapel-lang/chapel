use utilities;
use BlockDist;
use CyclicDist;
use BlockCycDist;
use Search;
use LayoutCS;

//
// DefaultRectangular support
//

// dsiPartialThese implementations

// otherIdx doesn't make much sense other than conformance to sparse
// domain interface. When/if there is a support for ragged domains ,
// then it would be useful

// strictly swpeaking I want createTuple calls to have rank-1. But
// that would require `where rank == 1` special implementations, and
// I kept hitting resolution issues there(may or may not be a bug).
// since otherIdx is not used at this point, I am moving with this
// implementation
iter DefaultRectangularDom.dsiPartialThese(param onlyDim, otherIdx) {

  if !dsiPartialDomain(onlyDim).contains(otherIdx) then return;
  for i in ranges(onlyDim) do yield i;
}

iter DefaultRectangularDom.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

    if !dsiPartialDomain(onlyDim).contains(otherIdx) then return;
    for i in ranges(onlyDim).these(tag) do yield i;
  }

iter DefaultRectangularDom.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in ranges(onlyDim).these(tag, followThis=followThis) do
      yield i;
  }

iter DefaultRectangularDom.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone &&
      __primitive("method call resolves",
                  ranges(if rank==1 then 0 else onlyDim), "these", tag) {

  if rank==1 then
    compilerError("dsiPartialThese is called on 1D domain");

    if !dsiPartialDomain(onlyDim).contains(otherIdx) then return;
    for i in ranges(onlyDim).these(tag) do yield i;
  }

proc DefaultRectangularDom.dsiPartialDomain(param exceptDim) where rank > 1 {
  return {(...ranges.withoutIdx(exceptDim))};
}

iter DefaultRectangularArr.dsiPartialThese(param onlyDim, otherIdx) {

  for i in dom.dsiPartialThese(onlyDim,otherIdx) do
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
}

iter DefaultRectangularArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

    for followThis in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag) do
      yield followThis;
}

iter DefaultRectangularArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag,
        followThis) do
      yield dsiAccess(otherIdx.withIdx(onlyDim, i));
}

iter DefaultRectangularArr.dsiPartialThese(param onlyDim,
    otherIdx,
    param tag: iterKind) where tag == iterKind.standalone &&
      __primitive("method call resolves", dom, "dsiPartialThese",
                                          onlyDim, otherIdx, tag=tag) {
    if rank == 1 then
      compilerError("dsiPartialThese on 1D array");
  for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag) do
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
}
//
// end DefaultRectangular support
//

//
// DefaultSparse support
//

proc DefaultSparseDom.dsiPartialDomain(param exceptDim) where rank > 1 {
  return parentDom._value.dsiPartialDomain(exceptDim);
}

proc DefaultSparseDom.__private_findRowRange(r) {

  //do async binary search in both directions
  var start = parentDom.dim(rank-1).low-1;
  var end = parentDom.dim(rank-1).low-1;

  var startDummy = parentDom.dim(rank-1).low-1;
  var endDummy = parentDom.dim(rank-1).high+1;
  var done: atomic bool;
  begin with (ref end) {
    var found: bool;
    (found, end) = binarySearch(_indices, ((...r),endDummy), hi=_nnz);
    done.write(true);
  }
  var found: bool;
  (found, start) = binarySearch(_indices, ((...r),startDummy), hi=_nnz);
  done.waitFor(true);
  return start..min(_nnz,end-1);
}

proc partialIterationDimCheck(param onlyDim, param rank) {
  if onlyDim < 0 || onlyDim >= rank then
    compilerError("Cannot perform partial iteration in dimension ",
                  onlyDim:string, ". Only dimensions between 0 and ",
                  (rank-1):string, " are allowed.");
}

iter DefaultSparseDom.dsiPartialThese(param onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity) {

  partialIterationDimCheck(onlyDim, rank);
  const otherIdxTup = chpl__tuplify(otherIdx);

  if onlyDim != this.rank-1 {
    for i in nnzDom.low..#_nnz do
      if _indices[i].withoutIdx(onlyDim) == otherIdxTup then 
        yield _indices[i][onlyDim];
  }
  else { //here we are sure that we are looking for the last index
    for i in __private_findRowRange(otherIdxTup) do
      yield _indices[i][onlyDim];
  }
}

iter DefaultSparseDom.dsiPartialThese(param onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind) where tag==iterKind.leader {

  partialIterationDimCheck(onlyDim, rank);
  const otherIdxTup = chpl__tuplify(otherIdx);

  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  var rowRange: range;
  if onlyDim==rank-1 then rowRange = __private_findRowRange(otherIdxTup);

  const l = if onlyDim!=rank-1 then nnzDom.low else rowRange.low;
  const h = if onlyDim!=rank-1 then nnzDom.low+_nnz else rowRange.high;
  const numElems = h-l+1;
  coforall t in 0..#numTasks {
    const myChunk = _computeBlock(numElems, numTasks, t, h-l, 0, 0);
    yield (myChunk[0]..min(_nnz, myChunk[1]),);
  }
}

iter DefaultSparseDom.dsiPartialThese(param onlyDim: int, otherIdx, 
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind, followThis) where tag==iterKind.follower {

  const otherIdxTup = chpl__tuplify(otherIdx);

  const l = if onlyDim!=rank-1 then nnzDom.low else
    __private_findRowRange(otherIdxTup).low;
  const followRange = followThis[1].translate(l);

  if onlyDim!=rank-1 then
    for i in followRange do
      if _indices[i].withoutIdx(onlyDim) == otherIdxTup then
        yield _indices[i][onlyDim];
      else 
        for i in followRange do
          yield _indices[i][onlyDim];
}

iter DefaultSparseDom.dsiPartialThese(param onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind) where tag==iterKind.standalone {

  partialIterationDimCheck(onlyDim, rank);
  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  const otherIdxTup = chpl__tuplify(otherIdx);

  var rowRange: range;
  if onlyDim==rank-1 then rowRange = __private_findRowRange(otherIdxTup);

  const l = if onlyDim!=rank-1 then _indices.domain.low else rowRange.low;
  const h = if onlyDim!=rank-1 then _nnz else rowRange.high;
  const numElems = h-l+1;
  if numElems <= -2 then return;

  if onlyDim != rank-1 {
    coforall t in 0..#numTasks {
      const myChunk = _computeBlock(numElems, numTasks, t, h, l, l);
      for i in myChunk[0]..min(_nnz,myChunk[1]) do
        if _indices[i].withoutIdx(onlyDim) == otherIdxTup then
          yield _indices[i][onlyDim];
    }
  }
  else {
    coforall t in 0..#numTasks {
      const myChunk = _computeBlock(numElems, numTasks, t, h, l, l);
      for i in myChunk[0]..myChunk[1] do {
        yield _indices[i][onlyDim];
      }
    }
  }
}


// FIXME I tried to move these iterators up in class hierarchy by
// implementing a dummy dsiAccess in those classes. But wasn't able
// to compile.
iter DefaultSparseArr.dsiPartialThese(param onlyDim, otherIdx) {
  for i in dom.dsiPartialThese(onlyDim, otherIdx) {
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
  }
}

iter DefaultSparseArr.dsiPartialThese(param onlyDim, otherIdx, 
    param tag) where tag==iterKind.leader {
  for followThis in dom.dsiPartialThese(onlyDim,otherIdx,tag=tag) {
    yield followThis;
  }
}

iter DefaultSparseArr.dsiPartialThese(param onlyDim, otherIdx, 
    param tag, followThis) where tag==iterKind.follower {
  for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag, 
      followThis) {
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
  }
}

iter DefaultSparseArr.dsiPartialThese(param onlyDim, otherIdx, 
    param tag) where tag==iterKind.standalone &&
      __primitive("method call resolves", dom, "dsiPartialThese",
                                          onlyDim, otherIdx, tag=tag) {
  for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
  }
}
//
// end DefaultSparse support
//

//
// LayoutCS support
//

proc CSDom.dsiPartialDomain(param exceptDim) where rank > 1 {
  return parentDom._value.dsiPartialDomain(exceptDim);
}

iter CSDom.dsiPartialThese(param onlyDim, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity){

  partialIterationDimCheck(onlyDim, 2);

  if onlyDim==0 {
    // Should we have a compiler warning about this expensive operation?
    for i in nnzDom.low..#_nnz {
      if idx[i] == otherIdx {
        const (found, loc) = binarySearch(startIdx, i);
        yield if found then loc else loc-1;
      }
    }
  }
  else {
    for i in startIdx[otherIdx]..stopIdx[otherIdx] do
      yield idx[i];
  }
}

iter CSDom.dsiPartialThese(param onlyDim, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind) where tag==iterKind.leader {

  partialIterationDimCheck(onlyDim, 2);
  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  const l = if onlyDim==0 then nnzDom.low else startIdx[otherIdx];
  const h = if onlyDim==0 then nnzDom.low+_nnz-1 else stopIdx[otherIdx];
  const numElems = h-l+1;

  coforall t in 0..#numTasks {
    const myChunk = _computeBlock(numElems, numTasks, t, h-l, 0, 0);
    yield(myChunk[0]..myChunk[1], );
  }
}

iter CSDom.dsiPartialThese(param onlyDim, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind, followThis) where tag==iterKind.follower {

  const l = if onlyDim==0 then nnzDom.low else startIdx[otherIdx];
  const followRange = followThis[1].translate(l);

  if onlyDim==0 {
    for i in followRange {
      if idx[i] == otherIdx {
        const (found, loc) = binarySearch(startIdx, i);
        yield if found then loc else loc-1;
      }
    }
  }
  else {
    for i in followRange {
      yield idx[i];
    }
  }
}

iter CSDom.dsiPartialThese(param onlyDim, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind)  where tag==iterKind.standalone {

  partialIterationDimCheck(onlyDim, 2);
  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  if onlyDim==0 {
    const l = nnzDom.low, h = nnzDom.low+_nnz-1;
    const numElems = _nnz;

    coforall t in 0..#numTasks {
      const myChunk = _computeBlock(numElems, numTasks, t, h-l, 0, 0);
      for i in myChunk[0]+l..myChunk[1]+l {
        if idx[i] == otherIdx {
          const (found, loc) = binarySearch(startIdx, i);
          yield if found then loc else loc-1;
        }
      }
    }
  }
  else {
    const l = startIdx[otherIdx], h = stopIdx[otherIdx];
    const numElems = h-l+1;

    const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
      tasksPerLocale;

    const  numChunks = if __primitive("task_get_serial") 
      then 1 
      else _computeNumChunks(numTasks, ignoreRunning, 
          minIndicesPerTask, numElems);

    if numChunks == 1 {
      for i in l..h do yield idx[i];
    }
    else {
      coforall t in 0..#numTasks {
        const myChunk = _computeBlock(numElems, numTasks, t, h, l, l);
        for i in myChunk[0]..myChunk[1] do yield idx[i];
      }
    }
  }
}

// FIXME I tried to move these iterators up in class hierarchy by
// implementing a dummy dsiAccess in those classes. But wasn't able
// to compile.
iter CSArr.dsiPartialThese(param onlyDim, otherIdx) {
  for i in dom.dsiPartialThese(onlyDim, otherIdx[0]) {
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
  }
}

iter CSArr.dsiPartialThese(param onlyDim, otherIdx, 
    param tag) where tag==iterKind.leader {
  for followThis in dom.dsiPartialThese(onlyDim,otherIdx[0],tag=tag) {
    yield followThis;
  }
}

iter CSArr.dsiPartialThese(param onlyDim, otherIdx, 
    param tag, followThis) where tag==iterKind.follower {
  for i in dom.dsiPartialThese(onlyDim, otherIdx[0], tag=tag, 
      followThis) {
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
  }
}

iter CSArr.dsiPartialThese(param onlyDim, otherIdx, 
    param tag) where tag==iterKind.standalone &&
      __primitive("method call resolves", dom, "dsiPartialThese",
                                          onlyDim, otherIdx[0], tag=tag) {
  for i in dom.dsiPartialThese(onlyDim, otherIdx[0], tag=tag) {
    yield dsiAccess(otherIdx.withIdx(onlyDim, i));
  }
}
//
// end LayoutCS support
//

//
// Block Distribution support
//
proc LocBlockArr.clone() {
  return new unmanaged LocBlockArr(eltType,rank,idxType,stridable,locDom,
      locRAD, myElems, locRADLock);
}

inline proc LocBlockArr.dsiGetBaseDom() { return locDom; }

iter BlockDom.dsiPartialThese(param onlyDim, otherIdx) {
  for i in whole._value.dsiPartialThese(onlyDim, otherIdx) do
    yield i;
}

iter BlockDom.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.leader {

  coforall locDom in __partialTheseLocDoms(onlyDim, otherIdx) {
    on locDom {
    for followThis in
        locDom.myBlock._value.dsiPartialThese(onlyDim, otherIdx, tag) {

      yield (followThis[0]+locDom.myBlock.dim(onlyDim).low, );
    }
    }
  }
}

iter BlockDom.dsiPartialThese(param onlyDim, otherIdx, param tag,
    followThis) where tag==iterKind.follower {

  for i in followThis[0] {
    yield i;
  }
}

iter BlockDom.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.standalone &&
          __primitive("method call resolves",
                      locDoms[dist.targetLocDom.first].myBlock._value,
                      "dsiPartialThese", onlyDim, otherIdx, tag) {

  coforall locDom in __partialTheseLocDoms(onlyDim, otherIdx) {
    on locDom {
      for i in locDom.myBlock._value.dsiPartialThese(onlyDim,
          otherIdx, tag){
        yield i;
      }
    }
  }
}

proc BlockDom.__partialTheseLocDoms(param onlyDim, otherIdx) {
  const baseLocaleIdx = dist.targetLocsIdx(
      otherIdx.withIdx(onlyDim, whole.dim(onlyDim).low));

  return locDoms[(...lineSliceMask(this, onlyDim, baseLocaleIdx))];
}

proc BlockDom.dsiPartialDomain(param exceptDim) {

  var ranges = whole._value.ranges.withoutIdx(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped Block(space, targetLocales =
      dist.targetLocales[(...faceSliceMask(this, exceptDim))]);

  return ret;
}

proc LocBlockDom.dsiPartialDomain(param exceptDim) {
  return myBlock._value.dsiPartialDomain(exceptDim);
}

iter LocBlockArr.dsiPartialThese(param onlyDim, otherIdx) {

  for i in myElems._value.dsiPartialThese(onlyDim,otherIdx) do 
    yield i;
}

iter LocBlockArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  for followThis in myElems._value.dsiPartialThese(onlyDim, otherIdx,
      tag=tag) do

    yield followThis;
}

iter LocBlockArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

  for i in myElems._value.dsiPartialThese(onlyDim, otherIdx, tag=tag,
      followThis) do 
    yield i;
}

iter LocBlockArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone &&
      __primitive("method call resolves", myElems._value, "dsiPartialThese",
                                                    onlyDim, otherIdx, tag) {

  for i in myElems._value.dsiPartialThese(onlyDim, otherIdx, tag) do
    yield i;
}
//
// end Block Distribution support
//

//
// Cyclic Distribution support
//

proc CyclicDom.dsiPartialDomain(param exceptDim) {

  var ranges = whole._value.ranges.withoutIdx(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped
    Cyclic(startIdx=this.dist.startIdx.withoutIdx(exceptDim), 
        targetLocales=dist.targetLocs[(...faceSliceMask(this, 
            exceptDim))]);

  return ret;
}

proc LocCyclicDom.dsiPartialDomain(param exceptDim) {
  return myBlock._value.dsiPartialDomain(exceptDim);
}


iter LocCyclicDom.dsiPartialThese(param onlyDim, otherIdx) {
  for i in myBlock._value.dsiPartialThese(onlyDim, otherIdx) do
    yield i;
}

iter LocCyclicDom.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.leader {

  for followThis in myBlock._value.dsiPartialThese(onlyDim, otherIdx,
      tag=iterKind.leader) do
    yield followThis;
}

iter LocCyclicDom.dsiPartialThese(param onlyDim, otherIdx,
    param tag, followThis) where tag==iterKind.follower {

  for i in myBlock._value.dsiPartialThese(onlyDim, otherIdx, 
      tag=iterKind.follower, followThis=followThis) {
    yield i;
  }
}

iter LocCyclicDom.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.standalone &&
      __primitive("method call resolves", myBlock._value, "dsiPartialThese",
                                          onlyDim, otherIdx, tag=tag) {

  for i in myBlock._value.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
    yield i;
  }
}

proc LocCyclicArr.dsiGetBaseDom() { return locDom; }

proc LocCyclicArr.clone() {
  return new unmanaged LocCyclicArr(eltType,rank,idxType,
      locDom,locRAD,locCyclicRAD,myElems,locRADLock);
}

iter LocCyclicArr.dsiPartialThese(param onlyDim, otherIdx) {
  for i in locDom.dsiPartialThese(onlyDim, otherIdx) do
    yield this(otherIdx.withIdx(onlyDim,i));
}

iter LocCyclicArr.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.leader {

  for followThis in locDom.dsiPartialThese(onlyDim, otherIdx,
      tag=iterKind.leader) do
    yield followThis;
}

iter LocCyclicArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag, followThis) where tag==iterKind.follower {

  for i in locDom.dsiPartialThese(onlyDim, otherIdx, 
      tag=iterKind.follower, followThis=followThis) {
    yield this(otherIdx.withIdx(onlyDim,i));
  }
}

iter LocCyclicArr.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.standalone &&
          __primitive("method call resolves", locDom, "dsiPartialThese",
                                              onlyDim, otherIdx, tag=tag) {

  for i in locDom.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
    yield this(otherIdx.withIdx(onlyDim,i));
  }
}
//
// end Cyclic Distribution support
//

//
// BlockCyclic distribution support
//
proc BlockCyclicDom.dsiPartialDomain(param exceptDim) {

  var ranges = whole._value.ranges.withoutIdx(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped
    BlockCyclic(startIdx=this.dist.lowIdx.withoutIdx(exceptDim),
        blocksize=this.dist.blocksize.withoutIdx(exceptDim),
        targetLocales=
            dist.targetLocales[(...faceSliceMask(this, exceptDim))]);

  return ret;
}

proc LocBlockCyclicDom.dsiPartialDomain(param exceptDim) {

  const parentDomain = globDom.whole._value.dsiPartialDomain(exceptDim);
  var retDomain: sparse subdomain(parentDomain);

  on this {
    for i in globDom.dsiLocalSubdomains(here) {
      retDomain += i._value.dsiPartialDomain(exceptDim);
    }
  }
  return retDomain;
}

iter LocBlockCyclicDom.dsiPartialThese(param onlyDim, otherIdx) {

  for i in globDom.dsiLocalSubdomains(here) {
    for ii in i._value.dsiPartialThese(onlyDim, otherIdx) {
      yield ii;
    }
  }
}

iter LocBlockCyclicDom.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  coforall i in globDom.dsiLocalSubdomains(here) {
    for ii in i._value.dsiPartialThese(onlyDim, otherIdx, tag) {
      yield (i, ii);
    }
  }
}

iter LocBlockCyclicDom.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in followThis[0]._value.dsiPartialThese(onlyDim, otherIdx,
        tag=tag, followThis=followThis[1]) do
      yield i;
}

proc LocBlockCyclicArr.clone() {
  return new unmanaged LocBlockCyclicArr(eltType,rank,idxType,stridable,
      allocDom,indexDom);
}

proc LocBlockCyclicArr.dsiGetBaseDom() { return indexDom; }

iter LocBlockCyclicArr.dsiPartialThese(param onlyDim, otherIdx) {

  for i in indexDom.dsiPartialThese(onlyDim, otherIdx) {
      yield this(otherIdx.withIdx(onlyDim, i));
  }
}

iter LocBlockCyclicArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  coforall i in do_dsiLocalSubdomains(indexDom) {
    for ii in i._value.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
      yield (i, ii);
    }
  }
}

iter LocBlockCyclicArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in followThis[0]._value.dsiPartialThese(onlyDim, otherIdx,
        tag=tag, followThis=followThis[1]) {
      yield this(otherIdx.withIdx(onlyDim, i));
    }
}

iter LocBlockCyclicArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone &&
      __primitive("method call resolves", myElems._value, "dsiPartialThese",
                                          onlyDim, otherIdx, tag=tag) {

  for i in indexDom.dsiPartialThese(onlyDim, otherIdx) {
    yield this(otherIdx.withIdx(onlyDim, i));
  }
}
//
// end BlockCyclic distribution support
//

//
// SparseBlockDist suppprt
//

proc SparseBlockDom.dsiPartialDomain(param exceptDim) {

  var ranges = whole._value.ranges.withoutIdx(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped Block(space, targetLocales =
      dist.targetLocales[(...faceSliceMask(this, exceptDim))]);

  return ret;
}

proc LocSparseBlockDom.dsiPartialDomain(param exceptDim) {
  return parentDom._value.dsiPartialDomain(exceptDim);
}

proc LocSparseBlockArr.dsiGetBaseDom() { return locDom; }

iter LocSparseBlockArr.dsiPartialThese(param onlyDim, otherIdx) {

  for i in myElems._value.dsiPartialThese(onlyDim,otherIdx) do 
    yield i;
}

iter LocSparseBlockArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  for followThis in 
      myElems._value.dsiPartialThese(onlyDim, otherIdx, tag=tag) do

    yield followThis;
}

iter LocSparseBlockArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

  for i in myElems._value.dsiPartialThese(onlyDim,otherIdx,tag=tag,
      followThis) do 

    yield i;
}

iter LocSparseBlockArr.dsiPartialThese(param onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone &&
      __primitive("method call resolves", myElems._value, "dsiPartialThese",
                                          onlyDim, otherIdx, tag) {

  for i in myElems._value.dsiPartialThese(onlyDim, otherIdx, tag) do
    yield i;
}
//
// end SparseBlockDist suppprt
//
