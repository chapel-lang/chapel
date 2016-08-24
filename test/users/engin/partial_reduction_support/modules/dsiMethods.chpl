use utilities;
use BlockDist;
use CyclicDist;
use BlockCycDist;

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
iter DefaultRectangularDom.dsiPartialThese(onlyDim, otherIdx) {

  if !dsiPartialDomain(onlyDim).member(otherIdx) then return;
  for i in ranges(onlyDim) do yield i;
}

iter dsiPartialThese(onlyDim, otherIdx, param tag: iterKind)
  where tag == iterKind.leader {

    if !dsiPartialDomain(onlyDim).member(otherIdx) then return;
    for i in ranges(onlyDim).these(tag) do yield i;
  }

iter DefaultRectangularDom.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in ranges(onlyDim).these(tag, followThis=followThis) do
      yield i;
  }

iter DefaultRectangularDom.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone {

    if !dsiPartialDomain(onlyDim).member(otherIdx) then return;
    for i in ranges(onlyDim).these(tag) do yield i;
  }

proc DefaultRectangularDom.dsiPartialDomain(exceptDim) where rank > 1 {
  return {(...ranges.strip(exceptDim))};
}

iter DefaultRectangularArr.dsiPartialThese(onlyDim, otherIdx) {

  for i in dom.dsiPartialThese(onlyDim,otherIdx) do
    yield dsiAccess(otherIdx.merge(onlyDim, i));
}

iter DefaultRectangularArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

    for followThis in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag) do
      yield followThis;
}

iter DefaultRectangularArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag,
        followThis) do
      yield dsiAccess(i);
}

// FIXME this standaloen iterator forwarding hits a compiler bug.
// The assertion in astutil.cpp:622 triggers. Engin
/*
iter DefaultRectangularArr.dsiPartialThese(onlyDim,
    otherIdx=createTuple(rank-1, idxType, 0:idxType),
    param tag: iterKind) where tag == iterKind.standalone {

  for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag) do
    yield dsiAccess(i);
}
*/
//
// end DefaultRectangular support
//

//
// DefaultSparse support
//

proc DefaultSparseDom.dsiPartialDomain(exceptDim) where rank > 1 {
  return parentDom._value.dsiPartialDomain(exceptDim);
}

proc DefaultSparseDom.__private_findRowRange(r) {

  //do async binary search in both directions
  var start = parentDom.dim(rank).low-1;
  var end = parentDom.dim(rank).low-1;

  var startDummy = parentDom.dim(rank).low-1;
  var endDummy = parentDom.dim(rank).high+1;
  var done: atomic bool;
  begin with (ref end) {
    var found: bool;
    (found, end) = binarySearch(indices, ((...r),endDummy));
    done.write(true);
  }
  var found: bool;
  (found, start) = binarySearch(indices, ((...r),startDummy));
  done.waitFor(true);
  return start..min(nnz,end-1);
}

iter DefaultSparseDom.dsiPartialThese(onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity) {

  if onlyDim<0 || onlyDim>rank then
    halt("Invalid dsiPartialThese dimension in DefaultSparse: ", onlyDim);
  const otherIdxTup = if isTuple(otherIdx) then otherIdx else (otherIdx, );

  if onlyDim != this.rank {
    compilerWarning("PERFORMANCE WARNING:\
        Partial iteration over dimension other than last is expensive");

    for i in nnzDom.low..#nnz do
      if indices[i].strip(onlyDim) == otherIdxTup then 
        yield indices[i][onlyDim];
  }
  else { //here we are sure that we are looking for the last index
    for i in __private_findRowRange(otherIdxTup) do
      yield indices[i][onlyDim];
  }
}

iter DefaultSparseDom.dsiPartialThese(onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind) where tag==iterKind.leader {

  if onlyDim<0 || onlyDim>rank then
    halt("Invalid dsiPartialThese dimension in DefaultSparse: ", onlyDim);

  const otherIdxTup = if isTuple(otherIdx) then otherIdx else (otherIdx, );

  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  var rowRange: range;
  if onlyDim==rank then rowRange = __private_findRowRange(otherIdxTup);

  const l = if onlyDim!=rank then nnzDom.low else rowRange.low;
  const h = if onlyDim!=rank then nnzDom.low+nnz else rowRange.high;
  const numElems = h-l+1;
  coforall t in 0..#numTasks {
    const myChunk = _computeBlock(numElems, numTasks, t, h-l, 0, 0);
    yield (myChunk[1]..min(nnz, myChunk[2]),);
  }
}

iter DefaultSparseDom.dsiPartialThese(onlyDim: int, otherIdx, 
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind, followThis) where tag==iterKind.follower {

  const otherIdxTup = if isTuple(otherIdx) then otherIdx else (otherIdx, );

  const l = if onlyDim!=rank then nnzDom.low else
    __private_findRowRange(otherIdxTup).low;
  const followRange = followThis[1].translate(l);

  if onlyDim!=rank then
    for i in followRange do
      if indices[i].strip(onlyDim) == otherIdxTup then
        yield indices[i][onlyDim];
      else 
        for i in followRange do
          yield indices[i][onlyDim];
}

iter DefaultSparseDom.dsiPartialThese(onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind) where tag==iterKind.standalone {

  if onlyDim<0 || onlyDim>rank then
    halt("Invalid dsiPartialThese dimension in DefaultSparse: ", onlyDim);

  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  const otherIdxTup = if isTuple(otherIdx) then otherIdx else (otherIdx, );

  var rowRange: range;
  if onlyDim==rank then rowRange = __private_findRowRange(otherIdxTup);

  const l = if onlyDim!=rank then indices.domain.low else rowRange.low;
  const h = if onlyDim!=rank then nnz else rowRange.high;
  const numElems = h-l+1;
  if numElems <= -2 then return;

  if onlyDim != rank {
    coforall t in 0..#numTasks {
      const myChunk = _computeBlock(numElems, numTasks, t, h, l, l);
      for i in myChunk[1]..min(nnz,myChunk[2]) do
        if indices[i].strip(onlyDim) == otherIdxTup then
          yield indices[i][onlyDim];
    }
  }
  else {
    coforall t in 0..#numTasks {
      const myChunk = _computeBlock(numElems, numTasks, t, h, l, l);
      for i in myChunk[1]..myChunk[2] do {
        yield indices[i][onlyDim];
      }
    }
  }
}


// FIXME I tried to move these iterators up in class hierarchy by
// implementing a dummy dsiAccess in those classes. But wasn't able
// to compile.
iter DefaultSparseArr.dsiPartialThese(onlyDim: int, otherIdx) {
  for i in dom.dsiPartialThese(onlyDim, otherIdx) {
    yield dsiAccess(otherIdx.merge(onlyDim, i));
  }
}

iter DefaultSparseArr.dsiPartialThese(onlyDim: int, otherIdx, 
    param tag) where tag==iterKind.leader {
  for followThis in dom.dsiPartialThese(onlyDim,otherIdx,tag=tag) {
    yield followThis;
  }
}

iter DefaultSparseArr.dsiPartialThese(onlyDim: int, otherIdx, 
    param tag, followThis) where tag==iterKind.follower {
  for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag, 
      followThis) {
    yield dsiAccess(otherIdx.merge(onlyDim, i));
  }
}

iter DefaultSparseArr.dsiPartialThese(onlyDim: int, otherIdx, 
    param tag) where tag==iterKind.standalone {
  for i in dom.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
    yield dsiAccess(otherIdx.merge(onlyDim, i));
  }
}
//
// end DefaultSparse support
//

//
// LayoutCSR support
//

proc CSRDom.dsiPartialDomain(exceptDim) where rank > 1 {
  return parentDom._value.dsiPartialDomain(exceptDim);
}

iter CSRDom.dsiPartialThese(onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity){

  if onlyDim<1 || onlyDim>2 then 
    halt("Invalid dimension for CSR.dsiPartialThese(). Only 1 and 2 \
        allowed.");

  if onlyDim==1 {
    // Should we have a compiler warning about this expensive operation?
    for i in nnzDom.low..#nnz {
      if colIdx[i] == otherIdx {
        const (found, loc) = BinarySearch(rowStart, i);
        yield if found then loc else loc-1;
      }
    }
  }
  else {
    for i in rowStart[otherIdx]..rowStop[otherIdx] do
      yield colIdx[i];
  }
}

iter CSRDom.dsiPartialThese(onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind) where tag==iterKind.leader {

  if onlyDim<1 || onlyDim>2 then 
    halt("Invalid dimension for CSR.dsiPartialThese(). Only 1 and 2 \
        allowed.");

  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  const l = if onlyDim==1 then nnzDom.low else rowStart[otherIdx];
  const h = if onlyDim==1 then nnzDom.low+nnz-1 else rowStop[otherIdx];
  const numElems = h-l+1;

  coforall t in 0..#numTasks {
    const myChunk = _computeBlock(numElems, numTasks, t, h-l, 0, 0);
    yield(myChunk[1]..myChunk[2], );
  }
}

iter CSRDom.dsiPartialThese(onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind, followThis) where tag==iterKind.follower {

  const l = if onlyDim==1 then nnzDom.low else rowStart[otherIdx];
  const followRange = followThis[1].translate(l);

  if onlyDim==1 {
    for i in followRange {
      if colIdx[i] == otherIdx {
        const (found, loc) = BinarySearch(rowStart, i);
        yield if found then loc else loc-1;
      }
    }
  }
  else {
    for i in followRange {
      yield colIdx[i];
    }
  }
}

iter CSRDom.dsiPartialThese(onlyDim: int, otherIdx,
    tasksPerLocale = dataParTasksPerLocale,
    ignoreRunning = dataParIgnoreRunningTasks,
    minIndicesPerTask = dataParMinGranularity,
    param tag: iterKind)  where tag==iterKind.standalone {

  if onlyDim<1 || onlyDim>2 then 
    halt("Invalid dimension for CSR.dsiPartialThese(). Only 1 and 2 \
        allowed.");

  const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
    tasksPerLocale;

  if onlyDim==1 {
    const l = nnzDom.low, h = nnzDom.high;
    const numElems = colIdx.size;

    coforall t in 0..#numTasks {
      const myChunk = _computeBlock(numElems, numTasks, t, h-l, 0, 0);
      for i in myChunk {
        if colIdx[i] == otherIdx {
          const (found, loc) = BinarySearch(rowStart, i);
          yield if found then loc else loc-1;
        }
      }
    }
  }
  else {
    const l = rowStart[otherIdx], h = rowStop[otherIdx];
    const numElems = h-l+1;

    const numTasks = if tasksPerLocale==0 then here.maxTaskPar else
      tasksPerLocale;

    const  numChunks = if __primitive("task_get_serial") 
      then 1 
      else _computeNumChunks(numTasks, ignoreRunning, 
          minIndicesPerTask, numElems);

    if numChunks == 1 {
      for i in l..h do yield i;
    }
    else {
      coforall t in 0..#numTasks {
        const myChunk = _computeChunk(numElems, numTasks, t, h, l, l);
        for i in myChunk do yield i;
      }
    }
  }
}

// FIXME I tried to move these iterators up in class hierarchy by
// implementing a dummy dsiAccess in those classes. But wasn't able
// to compile.
iter CSRArr.dsiPartialThese(onlyDim: int, otherIdx) {
  for i in dom.dsiPartialThese(onlyDim, otherIdx[1]) {
    yield dsiAccess(otherIdx.merge(onlyDim, i));
  }
}

iter CSRArr.dsiPartialThese(onlyDim: int, otherIdx, 
    param tag) where tag==iterKind.leader {
  for followThis in dom.dsiPartialThese(onlyDim,otherIdx[1],tag=tag) {
    yield followThis;
  }
}

iter CSRArr.dsiPartialThese(onlyDim: int, otherIdx, 
    param tag, followThis) where tag==iterKind.follower {
  for i in dom.dsiPartialThese(onlyDim, otherIdx[1], tag=tag, 
      followThis) {
    yield dsiAccess(otherIdx.merge(onlyDim, i));
  }
}

iter CSRArr.dsiPartialThese(onlyDim: int, otherIdx, 
    param tag) where tag==iterKind.standalone {
  for i in dom.dsiPartialThese(onlyDim, otherIdx[1], tag=tag) {
    yield dsiAccess(otherIdx.merge(onlyDim, i));
  }
}
//
// end LayoutCSR support
//

//
// Block Distribution support
//
proc LocBlockArr.clone() {
  return new LocBlockArr(eltType,rank,idxType,stridable,locDom,
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

      yield (followThis[1]+locDom.myBlock.dim(onlyDim).low, );
    }
    }
  }
}

iter BlockDom.dsiPartialThese(param onlyDim, otherIdx, param tag,
    followThis) where tag==iterKind.follower {

  for i in followThis[1] {
    yield i;
  }
}

iter BlockDom.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.standalone {

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
      otherIdx.merge(onlyDim, whole.dim(onlyDim).low));

  return locDoms[(...__lineSliceMask(this, onlyDim, baseLocaleIdx))];
}

proc BlockDom.dsiPartialDomain(param exceptDim) {

  var ranges = whole._value.ranges.strip(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped Block(space, targetLocales =
      dist.targetLocales[(...__faceSliceMask(this, exceptDim))]);

  return ret;
}

proc LocBlockDom.dsiPartialDomain(exceptDim) {
  return myBlock._value.dsiPartialDomain(exceptDim);
}

iter LocBlockArr.dsiPartialThese(onlyDim, otherIdx) {

  for i in myElems._value.dsiPartialThese(onlyDim,otherIdx) do 
    yield i;
}

iter LocBlockArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  for followThis in myElems._value.dsiPartialThese(onlyDim, otherIdx,
      tag=tag) do

    yield followThis;
}

iter LocBlockArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

  for i in myElems._value.dsiPartialThese(onlyDim, otherIdx, tag=tag,
      followThis) do 
    yield i;
}

iter LocBlockArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone {

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

  var ranges = whole._value.ranges.strip(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped
    Cyclic(startIdx=this.dist.startIdx.strip(exceptDim), 
        targetLocales=dist.targetLocs[(...__faceSliceMask(this, 
            exceptDim))]);

  return ret;
}

proc LocCyclicDom.dsiPartialDomain(exceptDim) {
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
    where tag==iterKind.standalone {

  for i in myBlock._value.dsiPartialThese(onlyDim, otherIdx,
      tag=iterKind.standalone) {
    yield i;
  }
}

proc LocCyclicArr.dsiGetBaseDom() { return locDom; }

proc LocCyclicArr.clone() {
  return new LocCyclicArr(eltType,rank,idxType,stridable,
      locDom,locRAD,locCyclicRAD,myElems,locRADLock);
}

iter LocCyclicArr.dsiPartialThese(param onlyDim, otherIdx) {
  for i in locDom.dsiPartialThese(onlyDim, otherIdx) do
    yield this(otherIdx.merge(onlyDim,i));
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
    yield this(otherIdx.merge(onlyDim,i));
  }
}

iter LocCyclicArr.dsiPartialThese(param onlyDim, otherIdx, param tag)
    where tag==iterKind.standalone {

  for i in locDom.dsiPartialThese(onlyDim, otherIdx,
      tag=iterKind.standalone) {
    yield this(otherIdx.merge(onlyDim,i));
  }
}
//
// end Cyclic Distribution support
//

//
// BlockCyclic distribution support
//
proc BlockCyclicDom.dsiPartialDomain(param exceptDim) {

  var ranges = whole._value.ranges.strip(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped
    BlockCyclic(startIdx=this.dist.lowIdx.strip(exceptDim),
        blocksize=this.dist.blocksize.strip(exceptDim),
        targetLocales=
            dist.targetLocales[(...__faceSliceMask(this, exceptDim))]);

  return ret;
}

proc LocBlockCyclicDom.dsiPartialDomain(param exceptDim) {

  const parentDomain = globDom.whole._value.dsiPartialDomain(exceptDim);
  var retDomain: sparse subdomain(parentDomain);

  on this {
    for i in globDom.dsiLocalSubdomains() {
      retDomain += i._value.dsiPartialDomain(exceptDim);
    }
  }
  return retDomain;
}

iter LocBlockCyclicDom.dsiPartialThese(onlyDim, otherIdx) {

  for i in globDom.dsiLocalSubdomains() {
    for ii in i._value.dsiPartialThese(onlyDim, otherIdx) {
      yield ii;
    }
  }
}

iter LocBlockCyclicDom.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  coforall i in globDom.dsiLocalSubdomains() {
    for ii in i._value.dsiPartialThese(onlyDim, otherIdx, tag) {
      yield (i, ii);
    }
  }
}

iter LocBlockCyclicDom.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in followThis[1]._value.dsiPartialThese(onlyDim, otherIdx,
        tag=tag, followThis=followThis[2]) do
      yield i;
}

proc LocBlockCyclicArr.clone() {
  return new LocBlockCyclicArr(eltType,rank,idxType,stridable,
      allocDom,indexDom);
}

proc LocBlockCyclicArr.dsiGetBaseDom() { return indexDom; }

iter LocBlockCyclicArr.dsiPartialThese(onlyDim, otherIdx) {

  for i in indexDom.dsiPartialThese(onlyDim, otherIdx) {
      yield this(otherIdx.merge(onlyDim, i));
  }
}

iter LocBlockCyclicArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  coforall i in do_dsiLocalSubdomains(indexDom) {
    for ii in i._value.dsiPartialThese(onlyDim, otherIdx, tag=tag) {
      yield (i, ii);
    }
  }
}

iter LocBlockCyclicArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

    for i in followThis[1]._value.dsiPartialThese(onlyDim, otherIdx,
        tag=tag, followThis=followThis[2]) {
      /*writeln("Yielding ", otherIdx.merge(onlyDim, i));*/
      yield this(otherIdx.merge(onlyDim, i));
    }
}

iter LocBlockCyclicArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone {

  for i in myElems._value.dsiPartialThese(onlyDim, otherIdx, tag=tag)
      do

    yield i;
}
//
// end BlockCyclic distribution support
//

//
// SparseBlockDist suppprt
//

proc SparseBlockDom.dsiPartialDomain(param exceptDim) {

  var ranges = whole._value.ranges.strip(exceptDim);
  var space = {(...ranges)};
  var ret = space dmapped Block(space, targetLocales =
      dist.targetLocales[(...__faceSliceMask(this, exceptDim))]);

  return ret;
}

proc LocSparseBlockDom.dsiPartialDomain(param exceptDim) {
  return parentDom._value.dsiPartialDomain(exceptDim);
}

proc LocSparseBlockArr.dsiGetBaseDom() { return locDom; }

iter LocSparseBlockArr.dsiPartialThese(onlyDim, otherIdx) {

  for i in myElems._value.dsiPartialThese(onlyDim,otherIdx) do 
    yield i;
}

iter LocSparseBlockArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.leader {

  for followThis in 
      myElems._value.dsiPartialThese(onlyDim, otherIdx, tag=tag) do

    yield followThis;
}

iter LocSparseBlockArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind, followThis) where tag == iterKind.follower {

  for i in myElems._value.dsiPartialThese(onlyDim,otherIdx,tag=tag,
      followThis) do 

    yield i;
}

iter LocSparseBlockArr.dsiPartialThese(onlyDim, otherIdx,
    param tag: iterKind) where tag == iterKind.standalone {

  for i in myElems._value.dsiPartialThese(onlyDim, otherIdx, tag) do
    yield i;
}
//
// end SparseBlockDist suppprt
//
